// Definición de pines
#define btnA 22
#define btnQ 23
#define led1 21
#define led2 19
#define led3 18
#define led4 4
#define led5 2

// Lista de LEDs
const int leds[] = {led1, led2, led3, led4, led5};
const int numLeds = 5;

int top = -1; // Índice de la pila (-1 = vacía)
unsigned long lastPressTimePush = 0;
unsigned long lastPressTimePop = 0;
const int debounceDelay = 200; // Tiempo de rebote en ms

// Variables para detectar cambios en los botones
int lastBtnAState = HIGH;
int lastBtnQState = HIGH;

void setup() {
  Serial.begin(115200); // Para depuración

  pinMode(btnA, INPUT_PULLUP); // Botón de agregar con resistencia pullup
  pinMode(btnQ, INPUT_PULLUP); // Botón de quitar con resistencia pullup

  // Configurar LEDs como salida y apagarlos
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
  
  Serial.println("Sistema iniciado - Pila vacía");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Leer el estado actual de los botones
  int btnAState = digitalRead(btnA);
  int btnQState = digitalRead(btnQ);

  // Botón para agregar (PUSH) - Solo detecta cuando pasa de HIGH a LOW
  if (btnAState == LOW && lastBtnAState == HIGH && (currentTime - lastPressTimePush > debounceDelay)) {
    lastPressTimePush = currentTime;
    if (top < numLeds - 1) { // Si hay espacio en la pila
      top++;
      digitalWrite(leds[top], HIGH);
      Serial.println("LED encendido en posición: " + String(top));
    } else {
      Serial.println("Pila llena, no se pueden encender más LEDs");
    }
  }

  // Botón para quitar (POP) - Solo detecta cuando pasa de HIGH a LOW
  if (btnQState == LOW && lastBtnQState == HIGH && (currentTime - lastPressTimePop > debounceDelay)) {
    lastPressTimePop = currentTime;
    if (top >= 0) { // Si hay LEDs encendidos
      digitalWrite(leds[top], LOW);
      Serial.println("LED apagado en posición: " + String(top));
      top--;
    } else {
      Serial.println("Pila vacía, no hay LEDs que apagar");
    }
  }
  
  // Actualizar los estados anteriores de los botones
  lastBtnAState = btnAState;
  lastBtnQState = btnQState;
}