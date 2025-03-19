#define PIR_PIN 22       // Sensor de movimiento PIR
#define OBSTACLE_PIN 23  // Sensor de obstáculo IR
#define LDR_PIN 35      // Sensor fotosensible (LDR)
#define LED_Y 19
#define LED_R 18
#define LED_G 21
#define bocina 5

void setup() {
    Serial.begin(9600); // Iniciar comunicación serial
    pinMode(PIR_PIN, INPUT);
    pinMode(OBSTACLE_PIN, INPUT);
    pinMode(LED_Y, OUTPUT);
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(bocina, OUTPUT);
}

void loop() {
    int pirState = digitalRead(PIR_PIN);  // Leer estado del PIR
    int obstacleState = digitalRead(OBSTACLE_PIN); // Leer estado del IR
    int ldrValue = analogRead(LDR_PIN);   // Leer valor del LDR (0-1023)

    // Mostrar datos en el Monitor Serie
    Serial.print("PIR Sensor: ");
    Serial.print(pirState == HIGH ? "Movimiento Detectado" : "Sin movimiento");
    Serial.print(" | IR Sensor: ");
    Serial.print(obstacleState == LOW ? "Obstáculo Detectado" : "Libre");
    Serial.print(" | LDR Value: ");
    Serial.println(ldrValue);

    if(pirState == HIGH){
      digitalWrite(LED_G, HIGH);
    } else {y
      digitalWrite(LED_G ,LOW);
    };

    if(obstacleState == LOW){
      digitalWrite(LED_R, HIGH);
      digitalWrite(bocina, LOW);

    } else {
      digitalWrite(LED_R ,LOW);
      digitalWrite(bocina, HIGH);
    };

    if(ldrValue >= 3000){
      digitalWrite(LED_Y, HIGH);
    } else {
      digitalWrite(LED_Y ,LOW);
    };

    delay(500); // Esperar 500ms antes de leer nuevamente
}