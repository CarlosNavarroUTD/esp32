#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Definiciones para la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definiciones para botones
#define BTN1 18
#define BTN3 21
#define BTN4 25
#define BTN6 27

// LEDS
#define LED1 19
#define LED2 26

// Variables de estado
int btnState1 = 0, btnState3 = 0, btnState4 = 0, btnState6 = 0;
int lastBtnState1 = 1, lastBtnState3 = 1, lastBtnState4 = 1, lastBtnState6 = 1;  // Inicialmente en HIGH porque INPUT_PULLUP

void setup() {
  Serial.begin(115200);
  Wire.begin(22, 23);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error al inicializar pantalla OLED");
    while (true);
  }

  // Inicializar pantalla
  display.clearDisplay();
  display.display();

  // Configurar pines
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);
  pinMode(BTN6, INPUT_PULLUP);
}

void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Estado de botones:");

  // Leer estado de cada botón
  btnState1 = digitalRead(BTN1);
  btnState3 = digitalRead(BTN3);
  btnState4 = digitalRead(BTN4);
  btnState6 = digitalRead(BTN6);

  // BTN1
  if (btnState1 == LOW && lastBtnState1 == HIGH) {
    Serial.println("BTN1 presionado");
    // Implementar acción para BTN1
  }
  lastBtnState1 = btnState1;
  display.setCursor(0, 10);
  display.print("BTN1: ");
  display.println(btnState1 == LOW ? "PRES" : "SOLT");

  // BTN3
  if (btnState3 == LOW && lastBtnState3 == HIGH) {
    Serial.println("BTN3 presionado");
    // Implementar acción para BTN3
  }
  lastBtnState3 = btnState3;
  display.setCursor(0, 20);
  display.print("BTN3: ");
  display.println(btnState3 == LOW ? "PRES" : "SOLT");

  // BTN4
  if (btnState4 == LOW && lastBtnState4 == HIGH) {
    Serial.println("BTN4 presionado");
    // Implementar acción para BTN4
  }
  lastBtnState4 = btnState4;
  display.setCursor(0, 30);
  display.print("BTN4: ");
  display.println(btnState4 == LOW ? "PRES" : "SOLT");

  // BTN6
  if (btnState6 == LOW && lastBtnState6 == HIGH) {
    Serial.println("BTN6 presionado");
    // Implementar acción para BTN6
  }
  lastBtnState6 = btnState6;
  display.setCursor(0, 40);
  display.print("BTN6: ");
  display.println(btnState6 == LOW ? "PRES" : "SOLT");

  display.display(); // Refrescar la pantalla
  delay(100); // Pequeño delay para evitar rebotes
}
