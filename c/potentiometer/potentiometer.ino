#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Definiciones de pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin para potenciómetro
#define POT_PIN 27

// Variables para el potenciómetro
int potValue = 0;
int potLevel = 0;
int lastPotLevel = -1; // Para detectar cambios
const int NUM_LEVELS = 6;
const char* levelNames[NUM_LEVELS] = {
  "Nivel 1 (Muy Bajo)",
  "Nivel 2 (Bajo)",
  "Nivel 3 (Medio-Bajo)",
  "Nivel 4 (Medio-Alto)",
  "Nivel 5 (Alto)",
  "Nivel 6 (Máximo)"
};

void setup() {
  Serial.begin(115200);
  Wire.begin(22, 23); // SDA, SCL

  // Inicializar pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Error: No se pudo inicializar la pantalla SSD1306"));
    while (true); // Bucle infinito si la pantalla no se encuentra
  }

  display.clearDisplay();
  display.display();
  delay(1000);

  // Configurar el pin del potenciómetro
  pinMode(POT_PIN, INPUT);

  // Mensaje inicial
  displayWelcomeScreen();
  delay(2000);
}

void loop() {
  // Leer valor del potenciómetro (0-4095 en ESP32)
  potValue = analogRead(POT_PIN);
  
  // Convertir a nivel (1-6)
  potLevel = map(potValue, 0, 4095, 0, NUM_LEVELS - 1);
  
  // Si el nivel cambió, actualizar la pantalla
  if (potLevel != lastPotLevel) {
    updateDisplay();
    lastPotLevel = potLevel;
  }
  
  // Imprimir en serial para depuración
  Serial.print("Valor: ");
  Serial.print(potValue);
  Serial.print(" - Nivel: ");
  Serial.println(potLevel + 1);
  
  delay(100); // Pequeño retraso para estabilidad
}

void displayWelcomeScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Medidor de Potenciometro");
  display.println("------------------------");
  display.println("Gira el potenciometro");
  display.println("para ver los cambios");
  display.println("de nivel");
  display.display();
}

void updateDisplay() {
  display.clearDisplay();
  
  // Mostrar título y valor actual
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Potenciometro:");
  display.println("------------------------");
  
  // Mostrar nivel actual
  display.setTextSize(1);
  display.println(levelNames[potLevel]);
  
  // Mostrar valor numérico
  display.print("Valor: ");
  display.println(potValue);
  
  // Dibujar barra de progreso
  display.drawRect(0, 40, 128, 10, SSD1306_WHITE);
  int barWidth = map(potValue, 0, 4095, 0, 126);
  display.fillRect(1, 41, barWidth, 8, SSD1306_WHITE);
  
  // Dibujar marcadores para cada nivel
  for (int i = 0; i < NUM_LEVELS; i++) {
    int x = map(i, 0, NUM_LEVELS - 1, 0, 128);
    display.drawLine(x, 52, x, 56, SSD1306_WHITE);
    display.setCursor(x - 2, 57);
    display.print(i + 1);
  }
  
  // Indicar dirección del cambio
  if (potLevel > lastPotLevel && lastPotLevel != -1) {
    display.setCursor(100, 30);
    display.println("(Subiendo)");
  } else if (potLevel < lastPotLevel) {
    display.setCursor(100, 30);
    display.println("(Bajando)");
  }
  
  display.display();
}