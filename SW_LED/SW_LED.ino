/*
 * Switches and LEDs refactored with internal pullups 
 */
int SW[] = {2,3,4,5,6,7};
int LED[] = {8,9,10,11,12,13};

int nSW = sizeof(SW) / sizeof(int);
int nLED = sizeof(LED) / sizeof(int);;

void setup() {
  for (int  i = 0; i < nSW; i++){
    pinMode(SW[i], INPUT_PULLUP);
  }
  for (int  i = 0; i < nLED; i++) {
    pinMode(LED[i], OUTPUT);
  }
}

void loop() {
  for( int i = 0; i < nLED; i++) {
    bool pinState = digitalRead(SW[i]);
    digitalWrite(LED[i], !pinState);
  }
}
