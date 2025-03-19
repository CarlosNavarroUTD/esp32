#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int pinBoton = 25 
;  // Pin analógico donde están los botones
const int buzzer = 26;  // Pin del buzzer (ajústalo según tu conexión)

const int notas[] = {262, 296, 328, 360, 396, 426}; // Notas musicales: Do, Re, Mi, Fa

void setup() {
    Serial.begin(9600);
    Wire.begin(23, 22);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        while (true); // Bucle infinito si la pantalla no seo encuentra
    }

    display.clearDisplay();
    display.display();

    pinMode(buzzer, OUTPUT);
}

void loop() {

        // Limpiar pantalla
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);



    int valor = analogRead(pinBoton); // Leer el voltaje del pin
    Serial.println(valor);  // Ver el valor en el Monitor Serie para ajuste fino

    display.println("Value: " + valor);
    display.display();

    if (valor > 4020) {  
        tone(buzzer, notas[0]); // Nota DO
    } 
    else if (valor > 3800 & valor < 4010) {  
        tone(buzzer, notas[1]); // Nota RE
    } 
    else if (valor > 3200 & valor < 3700) {  
        tone(buzzer, notas[2]); // Nota RE
    } 
    else if (valor > 1800 & valor < 2200) {  
        tone(buzzer, notas[3]); // Nota RE
    } 
    else if (valor > 500 & valor < 600) {  
        tone(buzzer, notas[4]); // Nota MI
    } 
    else if (valor > 200 & valor < 300) {  
        tone(buzzer, notas[5]); // Nota FA
    } 
    else {  
        noTone(buzzer); // No hay botón presionado
    }

    


    delay(50);  // Pequeño retardo para estabilidad
}