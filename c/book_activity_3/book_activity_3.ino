#include <DHT.h> // Biblioteca para el sensor DHT

#define LED_1 12
#define LED_2 13
#define LED_3 14

#define DHTPIN 21       // Pin de datos del sensor DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); // Crear un objeto DHT

const float Temp_Ref = 20.0;
float Temperatura = 0.0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  dht.begin();

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);

  Serial.println("Iniciando sensor DHT11...");

}

void loop() {

  

  Temperatura = dht.readTemperature();
  
  if (isnan(Temperatura)) {
    Serial.println("Error al leer el sensor DHT11");
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    dht.begin();

    return;
  } else {
    Serial.print("Temperatura: ");
    Serial.print(Temperatura);
    Serial.println(" °C");
  }
  
  if(Temperatura < Temp_Ref) {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
  }else if(Temperatura >= Temp_Ref+2 && Temperatura <= Temp_Ref+3){
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
  }else if(Temperatura >= Temp_Ref+3 && Temperatura <= Temp_Ref+5){
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, LOW);
  }else if(Temperatura >= Temp_Ref+5){
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, HIGH);
  }
    delay(1500);
}
