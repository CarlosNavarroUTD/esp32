int switchState = 0;

#define LED_Y 12
#define LED_R 13
#define LED_G 14
#define BUTTON 19


void setup() {
  pinMode(LED_G, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  switchState = digitalRead(BUTTON);
  
  if (switchState == LOW){
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_Y, LOW);
    digitalWrite(LED_R, LOW);
  } else {

    digitalWrite(LED_G, LOW);
    digitalWrite(LED_Y, LOW);
    digitalWrite(LED_R, HIGH);

    delay(250);
    digitalWrite(LED_Y, HIGH);
    digitalWrite(LED_R, LOW);
    delay(250);

  }


}
