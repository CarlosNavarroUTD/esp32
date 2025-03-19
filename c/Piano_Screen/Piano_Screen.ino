#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int pinBoton = 25;  // Pin analógico donde están los botones
const int buzzer = 26;    // Pin del buzzer (ajústalo según tu conexión)

const int notas[] = {262, 296, 328, 360, 396, 426}; // Notas musicales: Do, Re, Mi, Fa, Sol, La
const String notaNames[] = {"DO", "RE", "MI", "FA", "SOL", "LA"};

int currentNote = -1;     // Nota actual (-1 significa ninguna)
int animationPhase = 0;   // Fase de la animación
bool buttonPressed = false;

void setup() {
    Serial.begin(115200);
    Wire.begin(23, 22);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        while (true); // Bucle infinito si la pantalla no se encuentra
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Analizador de ondas");
    display.setCursor(0, 10);
    display.println("Presiona un boton");
    display.display();

    pinMode(buzzer, OUTPUT);
}

void loop() {
    // Leer el valor del pin de botones
    int valor = analogRead(pinBoton);
    Serial.println(valor);  // Ver el valor en el Monitor Serie para ajuste fino

    // Detectar qué botón se presionó
    int oldNote = currentNote;
    currentNote = -1;
    
    if (valor > 4020) {  
        currentNote = 0; // Nota DO
    } 
    else if (valor > 3800 && valor < 4010) {  
        currentNote = 1; // Nota RE
    } 
    else if (valor > 3200 && valor < 3700) {  
        currentNote = 2; // Nota MI
    } 
    else if (valor > 1800 && valor < 2200) {  
        currentNote = 3; // Nota FA
    } 
    else if (valor > 500 && valor < 600) {  
        currentNote = 4; // Nota SOL
    } 
    else if (valor > 200 && valor < 300) {  
        currentNote = 5; // Nota LA
    }

    // Manejar la reproducción de sonido
    if (currentNote != -1) {
        tone(buzzer, notas[currentNote]);
        buttonPressed = true;
    } else {
        noTone(buzzer);
        buttonPressed = false;
    }

    // Actualizar la pantalla
    updateDisplay(currentNote);
    
    // Actualizar fase de animación solo si hay un botón presionado
    if (buttonPressed) {
        animationPhase = (animationPhase + 1) % 8;  // Ciclo de 8 fases para la animación
    }

    delay(50);  // Mantener el retardo original para estabilidad
}

void updateDisplay(int noteIndex) {
    display.clearDisplay();
    
    // Mostrar título y estado
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Analizador de ondas");
    
    // Mostrar información de la nota actual
    display.setCursor(0, 12);
    if (noteIndex != -1) {
        display.print("Nota: ");
        display.println(notaNames[noteIndex]);
        display.setCursor(80, 12);
        display.print(notas[noteIndex]);
        display.println(" Hz");
    } else {
        display.println("Esperando...");
    }
    
    // Dibujar línea base
    display.drawLine(0, 40, SCREEN_WIDTH, 40, SSD1306_WHITE);
    
    // Dibujar visualización de onda según la nota seleccionada
    if (noteIndex != -1) {
        drawWaveform(noteIndex);
    }
    
    display.display();
}

void drawWaveform(int noteIndex) {
    int amplitude = 15;  // Amplitud máxima de la onda
    int centerY = 40;    // Línea central
    int frequency = (noteIndex + 1);  // Frecuencia relativa (más alta para notas más agudas)
    
    // Dibujar forma de onda basada en seno
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        // Calcular fase ajustada por animación
        float phase = (float)animationPhase / 4.0;
        
        // Calcular y dibujar la onda senoidal
        int y = centerY - amplitude * sin((x * frequency / 8.0 + phase) * (M_PI / 16.0));
        display.drawPixel(x, y, SSD1306_WHITE);
        
        // Dibujar una segunda onda más fina como efecto
        if (x % 2 == 0) {
            int y2 = centerY - (amplitude * 0.7) * sin((x * frequency / 7.0 - phase) * (M_PI / 14.0));
            display.drawPixel(x, y2, SSD1306_WHITE);
        }
    }
    
    // Dibujar barras de intensidad dinámicas basadas en la animación
    int barHeight = amplitude * 1.5;
    int numBars = 10;
    int barWidth = SCREEN_WIDTH / numBars;
    
    for (int i = 0; i < numBars; i++) {
        // Altura variable basada en la fase de animación y la frecuencia
        float heightMod = sin((i + animationPhase) * (M_PI / (numBars/2))) * 0.5 + 0.5;
        int height = (int)(barHeight * heightMod * (0.7 + 0.3 * (noteIndex / 5.0)));
        
        // Dibujar barra
        int barX = i * barWidth + 2;
        display.fillRect(barX, centerY + 5, barWidth - 4, height, SSD1306_WHITE);
    }
}