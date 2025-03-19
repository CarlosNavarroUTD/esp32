#include <DHT.h>

// Definir pines y variables
// PASO 1. Ingresa el num de pin donde está conectado el sensor, el led rojo y verde.
#define DHTPIN 15
#define DHTTYPE DHT11
#define LED_R 22 
#define LED_G 23

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);

    dht.begin();
}

void loop() {
// PASO 2. Declarar un array float para almacenar "temperaturas" (tamaño: 5) y una variable  "promedio" para almacenar temperaturas.
    float temperaturas[4];
    float *ptr = temperaturas; // Puntero apuntando al array
    float suma = 0;
    float promedio = 0;
    
    // Recorrer array con puntero y almacenar valores
    for (int i = 0; i < 5; i++) {
        *(ptr + i) = dht.readTemperature();  // Guardar temperatura usando puntero
        Serial.print("Temperatura ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(*(ptr + i)); // Mostrar valor leído por sensor
        delay(1000); // Espera entre lecturas
    }

    // Calcular el promedio
    for (int i = 0; i < 5; i++) {
        suma += *(ptr + i); // Acceder a los valores del array con el puntero
    }

    promedio = suma / 5;

//PASO 3. Implementa una condición if para encender el LED ROJO si el promedio de temperatura es mayor a 24°C
// si es menor, el LED VERDE deberá encender.





    // Mostrar resultado en el monitor serie
    Serial.print("Temperatura promedio: ");
    Serial.println(promedio);

    delay(3000);

    if (promedio > 25) {
      temperaturaAlta();
    }
    else{
      temperaturaBaja();
    }
}

void temperaturaAlta(){
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_R, HIGH);
}
void temperaturaBaja(){
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_R, LOW);
}
