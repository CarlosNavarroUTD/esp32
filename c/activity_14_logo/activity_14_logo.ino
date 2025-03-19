#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Definiciones para la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definiciones para botones
#define BTN1 18  // RESET
#define BTN3 21  // ZOOM
#define BTN4 25  // LEFT
#define BTN6 27  // RIGHT

// Definiciones para LEDs (no usados en este código, pero pueden añadirse)
#define LED1 19
#define LED2 26

// Pines de los botones en un array
const int buttonPins[] = {BTN1, BTN3, BTN4, BTN6};
const char *buttonNames[] = {"RESET", "ZOOM", "LEFT", "RIGHT"};

// Variables del logo
int logoX = 40, logoY = 10;  // Posición inicial del logo
int logoSize = 40;           // Tamaño del logo
int zoomStep = 5;            // Incremento del zoom
int moveStep = 5;            // Movimiento del logo
unsigned long lastDebounceTime = 0;  // Tiempo del último debounce
unsigned long debounceDelay = 200;    // Tiempo de debounce en ms

void setup() {
  Serial.begin(115200);
  
  // Iniciar comunicación I2C con los pines correctos para ESP32
  Wire.begin(22, 23);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error al inicializar pantalla OLED");
    while (true);  // Bucle infinito si la pantalla no se encuentra
  }

  // Configurar pines de botones como entrada con pull-up
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    Serial.println("Configurando pin " + String(buttonPins[i]) + " como entrada");
  }

  // Mostrar logo inicial
  mostrarLogo();
  
  Serial.println("Inicialización completada");
}

void loop() {
  // Control de tiempo para evitar rebotes en los botones
  if (millis() - lastDebounceTime > debounceDelay) {
    bool buttonPressed = false;
    
    // RESET - Botón 1
    if (digitalRead(BTN1) == LOW) {
      Serial.println("Botón RESET presionado");
      logoX = 40;
      logoY = 10;
      logoSize = 40;
      buttonPressed = true;
    }
    
    // ZOOM - Botón 3
    if (digitalRead(BTN3) == LOW) {
      Serial.println("Botón ZOOM presionado");
      logoSize += zoomStep;  // Aumentar tamaño del logo
      if (logoSize > 60) logoSize = 60;
      
      // Ajustar posición si el logo se sale de la pantalla
      if (logoX + logoSize > SCREEN_WIDTH) 
        logoX = SCREEN_WIDTH - logoSize;
      if (logoY + logoSize > SCREEN_HEIGHT) 
        logoY = SCREEN_HEIGHT - logoSize;
        
      buttonPressed = true;
    }
    
    // LEFT - Botón 4
    if (digitalRead(BTN4) == LOW) {
      Serial.println("Botón LEFT presionado");
      logoX -= moveStep;  // Mover a la izquierda
      if (logoX < 0) logoX = 0;
      buttonPressed = true;
    }
    
    // RIGHT - Botón 6
    if (digitalRead(BTN6) == LOW) {
      Serial.println("Botón RIGHT presionado");
      logoX += moveStep;  // Mover a la derecha
      if (logoX > SCREEN_WIDTH - logoSize) logoX = SCREEN_WIDTH - logoSize;
      buttonPressed = true;
    }

    // Si se presionó algún botón, actualizar la pantalla y el tiempo de debounce
    if (buttonPressed) {
      mostrarLogo();
      lastDebounceTime = millis();
    }
  }
  
  // Pequeño delay para estabilizar el loop
  delay(50);
}

void mostrarLogo() {
  display.clearDisplay();

  // Dibujar un óvalo representando el logo de Arduino
  display.drawCircle(logoX, logoY + logoSize / 2, logoSize / 2, SSD1306_WHITE);
  display.drawCircle(logoX + logoSize, logoY + logoSize / 2, logoSize / 2, SSD1306_WHITE);
  display.drawLine(logoX, logoY + logoSize / 2, logoX + logoSize, logoY + logoSize / 2, SSD1306_WHITE);
  display.drawPixel(logoX + logoSize / 2, logoY + logoSize / 2, SSD1306_WHITE);

  // Dibujar una "∞" para simular el logo de Arduino
  display.drawCircle(logoX + logoSize / 4, logoY + logoSize / 2, logoSize / 6, SSD1306_WHITE);
  display.drawCircle(logoX + 3 * logoSize / 4, logoY + logoSize / 2, logoSize / 6, SSD1306_WHITE);

  // Mostrar en pantalla
  display.display();
  
  // Información de debug
  Serial.println("Logo actualizado - X: " + String(logoX) + " Y: " + String(logoY) + " Tamaño: " + String(logoSize));
}