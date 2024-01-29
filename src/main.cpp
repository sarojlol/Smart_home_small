#include <Arduino.h>
#include <pins_define.h>

#define BLYNK_TEMPLATE_ID "TMPL6b4ap6ORG"
#define BLYNK_TEMPLATE_NAME "Smart Home"
//#define BLYNK_AUTH_TOKEN             "2oZEri2JfLLGI_s5OhaAA4ph4nSZrS4P"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#include <BlynkEdgent.h>

#define BLYNK_PRINT Serial
#define BLYNK_DEBUG

#define APP_DEBUG

byte sw_pins[4] = {sw1, sw2, sw3, sw4};
bool sw_status[4];
bool sw_flag[4];
byte led_pins[4] = {LED1, LED2, LED3, LED4};
bool led_status[4];

BLYNK_CONNECTED() {
  Blynk.syncAll();
}

BLYNK_WRITE(V0)
{
  led_status[0] = param.asInt();
  digitalWrite(led_pins[0], !led_status[0]);
}
BLYNK_WRITE(V1)
{
  led_status[1] = param.asInt();
  digitalWrite(led_pins[1], !led_status[1]);
}
BLYNK_WRITE(V2)
{
  led_status[2] = param.asInt();
  digitalWrite(led_pins[2], !led_status[2]);
}
BLYNK_WRITE(V3)
{
  led_status[3] = param.asInt();
  digitalWrite(led_pins[3], !led_status[3]);
}


void setup()
{
  for (int i = 0; i < 4; i++)
  {
    pinMode(led_pins[i], OUTPUT);
    pinMode(sw_pins[i], INPUT_PULLUP);
  }
  
  Serial.begin(115200);
  BlynkEdgent.begin();
}

void loop()
{
  BlynkEdgent.run();

  static unsigned long sw_filter;
  if ((millis() - sw_filter) > 10)
  {
    for (int i = 0; i < 4; i++)
    {
      sw_status[i] = digitalRead(sw_pins[i]);
      if ((sw_status[i] == LOW) &! sw_flag[i])
      {
        led_status[i] =! led_status[i];
        digitalWrite(led_pins[i], !led_status[i]);
        Serial.println(sw_status[0]);
        switch (i)
        {
        case 0:
          Blynk.virtualWrite(V0, led_status[0]);
          break;
        case 1:
          Blynk.virtualWrite(V1, led_status[1]);
          break;
        case 2:
          Blynk.virtualWrite(V2, led_status[2]);
          break;
        case 3:
          Blynk.virtualWrite(V3, led_status[3]);
          break;
        }
        sw_flag[i] == true;
      }

      else if ((sw_status[i] == HIGH) && sw_flag[i])
      {
        sw_flag[i] = false;
      }
    }
    sw_filter = millis();
  }
}
