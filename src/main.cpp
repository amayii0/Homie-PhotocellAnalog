/*********************************************************************************************************************
 ** 2017-05-22, RDU: Sample to use an analog photocell
 *********************************************************************************************************************/
#include <Homie.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Globals
  // Software specifications
    #define FW_NAME    "D1Mini-PhotocellAnalog"
    #define FW_VERSION "0.17.5.22"


  // Photocell
    #define PHOTOCELL_PIN A0

    // Sensor consts/vars
      HomieNode photocellNode("lightLevel", "lightLevel");

    // Measure loop
        const float MEASURE_INTERVAL = 0.5f; // How often to poll sensor (seconds)
        unsigned long lastMeasureSent = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Homie Setup Handler
void setupHandler() {
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Homie loop Handler
void loopHandler() {
  if (millis() - lastMeasureSent >= MEASURE_INTERVAL * 1000UL || lastMeasureSent == 0) {
    float lightLevel = analogRead(PHOTOCELL_PIN) ;
    lightLevel = map(lightLevel, 0, 1023, 0, 255); // If you want to reduce it from 0-1023 down to 0-255 (1 byte)
    if (isnan(lightLevel)) {
      Homie.getLogger() << F("Failed to read from sensor!");
    } else {
      Homie.getLogger() << F("Light level = ") << lightLevel << endl;
      photocellNode.setProperty("lightLevel").send(String(lightLevel));
    }
    lastMeasureSent = millis();
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Arduino Setup Handler
void setup() {
  Serial.begin(115200); // Required to enable serial output

  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);

  // Photocell
    photocellNode.advertise("lightLevel");

  Homie.setup();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Arduino Loop Handler
void loop() {
  Homie.loop();
}
