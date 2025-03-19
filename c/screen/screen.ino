#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PIR_PIN 18       // Sensor de movimiento PIR
#define OBSTACLE_PIN 19  // Sensor de obstáculo IR
#define LDR_PIN 34      // Sensor fotosensible (LDR)
#define LED_Y 14
#define LED_R 12
#define LED_G 13
#define BOCINA 5

void setup() {
    Serial.begin(9600);
    Wire.begin(22, 21);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        while (true); // Bucle infinito si la pantalla no seo encuentra
    }

    display.clearDisplay();
    display.display();

    pinMode(PIR_PIN, INPUT);
    pinMode(OBSTACLE_PIN, INPUT);
    pinMode(LED_Y, OUTPUT);
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(BOCINA, OUTPUT);
}

void loop() {
    int pirState = digitalRead(PIR_PIN);
    int obstacleState = digitalRead(OBSTACLE_PIN);
    int ldrValue = analogRead(LDR_PIN);

    // Limpiar pantalla
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);

    // Mostrar estado de sensores en OLED
    display.println("Sensor PIR: " + String(pirState == HIGH ? "Movimiento" : "Sin mov."));
    display.println("Sensor IR: " + String(obstacleState == LOW ? "Obstaculo" : "Libre"));
    display.println("Luz: " + String(ldrValue));
    
    display.display();

    // Control de LEDs y bocina
    digitalWrite(LED_G, pirState == HIGH ? HIGH : LOW);
    digitalWrite(LED_R, obstacleState == LOW ? HIGH : LOW);
    digitalWrite(BOCINA, obstacleState == LOW ? LOW : HIGH);
    digitalWrite(LED_Y, ldrValue >= 3000 ? HIGH : LOW);

    delay(500);
}
