#include <DHT.h> // Biblioteca para el sensor DHT

// Configuración del sensor DHT11
#define DHTPIN 15        // Pin de datos del sensor DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); // Crear un objeto DHT

// Pines de los LEDs
#define LED_TEMP_ALTA 23  // LED rojo para temperatura alta
#define LED_TEMP_BAJA 21  // LED verde para temperatura baja
#define LED_HUM_ALTA 19   // LED rojo para humedad alta
#define LED_HUM_BAJA 18   // LED verde para humedad baja

// Pin del botón
#define BOTON 4 

// Variables para almacenar temperatura y humedad
float temperatura = 0.0;
float humedad = 0.0;
float *puntero = nullptr; // Inicialmente apagado (nullptr)

// Variables para control de botón y estado del sistema
unsigned long tiempoBoton = 0;
const unsigned long debounceDelay = 50;
int estadoSistema = 0; // 0 = Apagado, 1 = Monitoreo Temperatura, 2 = Monitoreo Humedad

void setup() {
  // Inicializar el monitor serie
  Serial.begin(115200);
  dht.begin();

  // Configurar los pines de los LEDs como salida
  pinMode(LED_TEMP_ALTA, OUTPUT);
  pinMode(LED_TEMP_BAJA, OUTPUT);
  pinMode(LED_HUM_ALTA, OUTPUT);
  pinMode(LED_HUM_BAJA, OUTPUT);

  // Configurar el botón como entrada con resistencia pull-down
  pinMode(BOTON, INPUT_PULLDOWN);

  Serial.println("Sistema iniciado. Presiona el botón para comenzar.");
}

void loop() {
  // Leer valores del sensor DHT11
  temperatura = dht.readTemperature();
  humedad = dht.readHumidity();

  // Validar si los datos son válidos
  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Error al leer el sensor DHT11");
    apagarLeds();
    delay(2000);
    return;
  }

  // Detección del botón con antirrebote
  if (digitalRead(BOTON) == HIGH) {
    if (millis() - tiempoBoton > debounceDelay) {
      tiempoBoton = millis(); // Actualizar tiempo del botón

      // Cambiar estado del sistema: 0 → 1 → 2 → 0 (Apagado → Temp → Hum → Apagado)
      estadoSistema = (estadoSistema + 1) % 3;

      // Asignar puntero según estado
      if (estadoSistema == 1) {
        puntero = &temperatura;
        Serial.println("Monitoreando Temperatura");
      } else if (estadoSistema == 2) {
        puntero = &humedad;
        Serial.println("Monitoreando Humedad");
      } else {
        puntero = nullptr; // Apagar sistema
        apagarLeds();
        Serial.println("Sistema apagado");
      }
    }
  }

  // Si el sistema está apagado, no hacer nada
  if (puntero == nullptr) return;

  // Imprimir el valor monitoreado
  Serial.print("Valor monitoreado: ");
  Serial.println(*puntero);

  // Lógica de los LEDs
  if (puntero == &temperatura) {
    digitalWrite(LED_TEMP_ALTA, (*puntero > 20.0) ? HIGH : LOW);
    digitalWrite(LED_TEMP_BAJA, (*puntero <= 20.0) ? HIGH : LOW);
    apagarLedsHumedad();
  } else {
    digitalWrite(LED_HUM_ALTA, (*puntero > 50.0) ? HIGH : LOW);
    digitalWrite(LED_HUM_BAJA, (*puntero <= 50.0) ? HIGH : LOW);
    apagarLedsTemperatura();
  }

  delay(300);
}

// Funciones para apagar LEDs
void apagarLeds() {
  digitalWrite(LED_TEMP_ALTA, LOW);
  digitalWrite(LED_TEMP_BAJA, LOW);
  digitalWrite(LED_HUM_ALTA, LOW);
  digitalWrite(LED_HUM_BAJA, LOW);
}

void apagarLedsTemperatura() {
  digitalWrite(LED_TEMP_ALTA, LOW);
  digitalWrite(LED_TEMP_BAJA, LOW);
}

void apagarLedsHumedad() {
  digitalWrite(LED_HUM_ALTA, LOW);
  digitalWrite(LED_HUM_BAJA, LOW);
}