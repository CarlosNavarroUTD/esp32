
#define LED_1 12
#define LED_2 13
#define LED_3 14

#define SENSOR 21   

const float Temp_Ref = 20.0;
float Temperatura = 0.0;

void setup() {
  Serial.begin(115200);
  delay(1000);


  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(SENSOR, OUTPUT);

  Serial.println("Iniciando sensor DHT11...");

}

void loop() {

  digitalWrite(SENSOR, HIGH);
  delay(1000);
  digitalWrite(SENSOR, LOW);

  int Sensor_Value = analogRead(SENSOR);
  
  Serial.print("Valor del sensor: ");
  Serial.println(Sensor_Value);
  
  
  if(Temperatura < Temp_Ref) {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
  }else if(Temperatura >= Temp_Ref+2 && Temperatura <= Temp_Ref+4){
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
  }else if(Temperatura >= Temp_Ref+4 && Temperatura <= Temp_Ref+6){
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, LOW);
  }else if(Temperatura >= Temp_Ref+6){
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, HIGH);
  }
    delay(1500);
}
