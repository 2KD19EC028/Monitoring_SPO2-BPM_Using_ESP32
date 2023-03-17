
#define BLYNK_TEMPLATE_ID "TMPLI7XReOKk"
#define BLYNK_DEVICE_NAME "pulse oximeter"
#define BLYNK_AUTH_TOKEN "jy8vDTTR9yMDqdq9v-SFH4uffXuW8GN5"




#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define REPORTING_PERIOD_MS 1000

char auth[] = BLYNK_AUTH_TOKEN;       // You should get Auth Token in the Blynk App.
char ssid[] = "$";                    // Your Wi-Fi Credentials
char pass[] = "99999999";

// Connections : SCL PIN - D1 , SDA PIN - D2 , INT PIN - D0
PulseOximeter pox;

float BPM, SpO2;
uint32_t tsLastReport = 0;


void onBeatDetected()
{
    Serial.println("Beat Detected!");
}

void setup()
{
    Serial.begin(115200);

    pinMode(19, OUTPUT);
    Blynk.begin(auth, ssid, pass);

    Serial.print("Initializing Pulse Oximeter..");

    if (!pox.begin())
    {
         Serial.println("FAILED");
         for(;;);
    }
    else
    {
         Serial.println("SUCCESS");
         pox.setOnBeatDetectedCallback(onBeatDetected);
    }

    // The default current for the IR LED is 50mA and it could be changed by uncommenting the following line.
        pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

}

void loop()
{
    pox.update();
    Blynk.run();

    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        Serial.print("Heart rate:");
        Serial.print(BPM);
        Serial.print(" bpm / SpO2:");
        Serial.print(SpO2);
        Serial.println(" %");

        Blynk.virtualWrite(V3, BPM);
        Blynk.virtualWrite(V4, SpO2);

        tsLastReport = millis();
    }
}
