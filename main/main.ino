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

Bounce * sw = new Bounce[nSW];

void setup() {
  Serial.begin(9600);
  Serial.println("Begining setup");
  AFMS.begin();

  for(i = 0; i < nSW; i++) {
//    pinMode(SW[i], INPUT_PULLUP);
    sw[i].attach(SW[i], INPUT_PULLUP);
    sw[i].interval(1);
  }
  for(i = 0; i < nLED; i++) {
    pinMode(LED[i], OUTPUT);
  }
  phase = 0;
  Serial.println("Setup complete (bounc2)");
  illuminate(0);
}

void loop() {
 for(i = 0; i< nSW; i++) {
//    if(digitalRead(SW[i]) == LOW) {
//      depressed(i);
//    }
    sw[i].update();
    if(sw[i].fell()) {
      depressed(i);
    }
  }

  t = millis();
  if(t - last > interval) {
    last = t;
    M1s = M1s + ramp;
    M1->setSpeed(M1s);
    if(M1s < 100 || M1s >= 255) {
      ramp = 0;
    }
  }
}

void depressed(int sw) {
   Serial.print(sw);
   switch(sw){
     case 0:
      buttonZero();
      break;
     case 1:
      buttonOne();
      break;
     case 2:
      buttonTwo();
      break;
     case 3:
      buttonThree();
      break;
     case 4:
      buttonFour();
      break;
     case 5:
      buttonFive();
      break;
     default:
      M1->run(RELEASE);
      M1s = 0;
      ramp = 0;
      Serial.println("Error: invalid phase");
      break;
   }
}

void buttonZero() {
  if (phase == 0) {
    phase = 1;
    Serial.println("Phase 1: Ramping up, forward");
    illuminate(1);
    M1->run(FORWARD);
    M1s = 100;
    ramp = 1;
  }
}

void buttonOne() {
  if (phase == 7) {
    phase = 0;
    Serial.println("Phase 0; Full stop, near end");
    illuminate(0);
    M1->run(RELEASE);
    M1s = 0;
    ramp = 0;
  }
}

void buttonTwo() {
  if (phase == 1) {
    phase = 2;
    Serial.println("Phase 2: Full speed, forward");
    illuminate(2);
    M1s = 255;
    ramp = 0;
  } else if (phase == 6) {
    phase = 7;
    Serial.println("Phase 7: Ramping down, backward");
    illuminate(1);
    ramp = -1;
  }
}

void buttonThree() {
  if (phase == 2) {
    phase = 3;
    Serial.println("Phase 3: Ramping down, forward");
    illuminate(4);
    ramp = -1;
  } else if (phase == 5) {
    phase = 6;
    Serial.println("Phase 6: Full speed, backward");
    illuminate(2);
    M1s = 255;
    ramp = 0;
  }
}

void buttonFour() {
  if (phase == 3) {
    phase = 4;
    Serial.println("Phase 4: Full Stop, far end");
    illuminate(5);
    M1->run(RELEASE);
    M1s = 0;
    ramp = 0;
  }
}

void buttonFive() {
  if (phase == 4) {
    phase = 5;
    Serial.println("Phase 5: Ramping up, backward");
    illuminate(4);
    M1->run(BACKWARD);
    M1s = 100;
    ramp = 1;
  }
}

void emergencyStop(int wrongButton) {
  M1->run(RELEASE);
  M1s = 0;
  ramp = 0;
  Serial.print("Emergency stop in phase: ");
  Serial.println(phase);
  Serial.print("Button pressed out of turn: ");
  Serial.println(wrongButton);
}

void illuminate( int n) {
  for( i = 0; i < nLED; i++) {
    if( i == n) {
      digitalWrite(LED[i], HIGH);
    } else {
      digitalWrite(LED[i], LOW);
    }
  }
  if( n == 2) {
    digitalWrite(LED[3], HIGH);
  }
}


