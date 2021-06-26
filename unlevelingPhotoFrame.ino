#include <Stepper.h>
#define STEPS 2038
#define A 2
#define B 3
#define C 4
#define D 5

const bool DEBUG = true;

const int LIGHT_SENSOR_PIN = A0;
const int TILT_SENSOR_PIN = A1;
int lightSensorValue = 0;
int tiltSensorValue = 0;
bool lightsOff = false;
bool tilted = false;
Stepper stepper(STEPS, A, C, B, D);

int noLightCount = 0;
int secondsBeforeMotor = 5;

void setup(){
  Serial.begin(9600);
  
  pinMode(LIGHT_SENSOR_PIN, OUTPUT);
  pinMode(TILT_SENSOR_PIN, OUTPUT);
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);
}

void Tighten(int rotations, int rpm) {
  stepper.setSpeed(rpm);
  stepper.step(STEPS * rotations);
}

void Loosen(int rotations, int rpm) {
  stepper.setSpeed(rpm);
  stepper.step(-STEPS * rotations);
}

void OutputDebugInfo() {
    if (lightsOff) {
      Serial.println("LIGHTS OFF");
    }

    if (tilted) {
      Serial.println("TILTED RIGHT");
    }
}

void loop(){
  lightSensorValue = analogRead(LIGHT_SENSOR_PIN);
  delay(5);
  tiltSensorValue = analogRead(TILT_SENSOR_PIN);
  delay(5);

  lightsOff = lightSensorValue < 10;
  tilted = tiltSensorValue == 0;

  if (DEBUG) {
    OutputDebugInfo();
  }
  
  if (!tilted & lightsOff) {
    delay(200);
    noLightCount++;

    if (noLightCount >= 50) {
      Loosen(1, 2);
    }
  } else if (!lightsOff) {
    noLightCount == 0;
  }
}
