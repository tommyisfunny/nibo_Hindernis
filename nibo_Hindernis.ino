#include <NIBOburger.h>

enum {
  CLEAR = 0,
  LINE_LEFT  = 1,
  LINE_RIGHT = 2,
  LINE_BOTH  = 3
};

const int triggerDist = 20;
const int speedDiffTurn = 55;
const int slowWheelSpeedOnTurn = -1;

uint8_t environment = CLEAR;

uint8_t calculateEnvironment() {
  uint16_t left  = SensorFL.get();
  uint16_t right = SensorFR.get();

  int diff = left - right;

  Serial.print("diff:");Serial.println(diff);
  
  if (diff < triggerDist && diff > -triggerDist) {
    return CLEAR;
  }
  if (diff < -triggerDist) {
    return LINE_RIGHT;
  }
  if (diff > triggerDist) {
    return LINE_LEFT;
  }
  return CLEAR;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Hello World");
  NIBOburger.begin();
}

void loop() {
  NIBOburger.waitAnalogUpdate();
  NIBOburger.checkVoltage();
  uint8_t new_env = calculateEnvironment();
  
  if (new_env!=environment) {
    environment = new_env;
    switch (new_env) {
      case CLEAR:
        Serial.println("Clear");
        NIBOburger.setLed(LED2, 0);
        NIBOburger.setLed(LED3, 0);
        Engine.setSpeed(+25, +25);
        break;

      case LINE_LEFT:
        Serial.println("Line left");
        NIBOburger.setLed(LED2, 1);
        NIBOburger.setLed(LED3, 0);
        Engine.setSpeed(slowWheelSpeedOnTurn, speedDiffTurn);
        break;

      case LINE_RIGHT:
        Serial.println("Line right");
        NIBOburger.setLed(LED2, 0);
        NIBOburger.setLed(LED3, 1);
        Engine.setSpeed(speedDiffTurn, slowWheelSpeedOnTurn);
        break;

      case LINE_BOTH:
        Serial.println("Obstacel both");
        NIBOburger.setLed(LED2, 1);
        NIBOburger.setLed(LED3, 1);
        Engine.setSpeed(0, 0);
        break;
    } 
  }
}
