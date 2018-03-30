#include <Bounce2.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *M1 = AFMS.getMotor(1);
int M1s; // Motor_1_speed

int phase; // 0 := at begining, 1 := ramping up, 2:= full speed forward, 3:= ramping down, 4 := at end,
// 5:= ramping up, 6:= full speed back, 7 := ramping down, 
int i; // all perpuse iterator

unsigned long t; // time since commencment
unsigned long last; // time since last count
int interval = 50; // count interval


int SW[] = {2,3,4,5,6,7};
int LED[] = {8,9,10,11,12,13};
int nSW = sizeof(SW) / sizeof(int);
int nLED = sizeof(LED) / sizeof(int);
int ramp;

Bounce * buttons = new Bounce[nSW];

void setup() {
  Serial.begin(9600);
  Serial.println("Begining setup");
  AFMS.begin();

  for(i = 0; i < nSW; i++) {
//    pinMode(SW[i], INPUT_PULLUP);
    buttons[i].attach(SW[i], INPUT_PULLUP);
    buttons[1].interval(1);
  }
  for(i = 0; i < nLED; i++) {
    pinMode(LED[i], OUTPUT);
  }
//  phase = 0;
  M1->run(BACKWARD);
  M1->setSpeed(255);
  
  Serial.println("Setup complete (bounce)");
}

void loop() {
  for(i = 0; i < nSW; i++) {
    buttons[i].update();
    if(buttons[i].fell() ) {
      Serial.println(i);
    }
  }
}
