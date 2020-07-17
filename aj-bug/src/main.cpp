#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

const char *json = R"({"wifi":{"wifi":{"status":8248,"mode":1,"ch":1,"sta":{"ssid":"YYYY","bssid":"AA:BB:CC:DD:EE:FF","mac":"AA:BB:CC:DD:EE:FF","ip":"192.168.0.111","gw":"192.168.0.1","mask":"255.255.255.0","rssi":-60}}}})";

extern "C" void app_main()
{
  initArduino();
  Serial.begin(115200);
  SPIFFS.begin();
  File f = SPIFFS.open("/test.json", FILE_WRITE);
  f.print(json);
  f.close();
  for (size_t i = 200; i < 400; i++)
  {
    f = SPIFFS.open("/test.json");
    Serial.printf("trying size %i", i);
    DynamicJsonDocument doc(i);
    if (!heap_caps_check_integrity_all(true))
    {
      Serial.println("heap corrupted before calling deserializeJson() - should not happen");
      break;
    }

    deserializeJson(doc, f);
    f.close();
    if (!heap_caps_check_integrity_all(true))
    {
      Serial.println("heap corrupted aftr calling deserializeJson() ");
      break; // remove this and the code will eventially crash when subsequent malloc finds corruption
    }
  }
}
