#define MHPA 35      // Sensor fotosensible (LDR)
#define MHPD 21      // Sensor fotosensible (LDR)

void setup() {
    Serial.begin(9600); // Iniciar comunicación serial
    pinMode(MHPA, INPUT);
    pinMode(MHPD, INPUT);

}

void loop() {
    int analogValud = analogRead(MHPA);   // Leer valor del LDR (0-1023)
    //int digitalValue = digitalRead(MHPD);   // Leer valor del LDR (0-1023)


    Serial.print(" | Analog: ");
    Serial.println(analogValud);s

    //delay(500);
    //Serial.print(" | Digital: ");
    //Serial.println(digitalValue);

    delay(50); // Esperar 500ms antes de leer nuevamente
}