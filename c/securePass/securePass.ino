#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int pinBoton = 25;  // Pin analógico donde están los botones
const int buzzer = 26;    // Pin del buzzer

const int notas[] = {262, 296, 328, 360, 396, 426}; // Notas musicales: Do, Re, Mi, Fa, Sol, La
const int PASSWORD_LENGTH = 5;                      // Longitud de la contraseña
const int correctPassword[PASSWORD_LENGTH] = {0, 1, 2, 3, 4}; // Contraseña correcta (ajustar a tus necesidades)

int currentPassword[PASSWORD_LENGTH] = {-1, -1, -1, -1, -1}; // Contraseña ingresada por el usuario
int currentPosition = 0;                           // Posición actual de entrada
bool securityActivated = false;                    // Estado del sistema de seguridad
bool passwordMode = false;                         // Modo de ingreso de contraseña
unsigned long lastButtonPress = 0;                 // Tiempo de la última pulsación
bool buttonReleased = true;                        // Estado del botón (liberado o presionado)

void setup() {
    Serial.begin(115200);
    Wire.begin(23, 22);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        while (true); // Bucle infinito si la pantalla no se encuentra
    }

    display.clearDisplay();
    display.display();

    pinMode(buzzer, OUTPUT);
    
    // Mensaje inicial
    displayWelcomeScreen();
}

void loop() {
    // Leer el valor de los botones
    int valor = analogRead(pinBoton);
    int botonPresionado = -1;
    
    // Determinar qué botón se presionó
    if (valor > 4020) {
        botonPresionado = 0; // Botón 1
    } else if (valor > 3800 && valor < 4010) {
        botonPresionado = 1; // Botón 2
    } else if (valor > 3200 && valor < 3700) {
        botonPresionado = 2; // Botón 3
    } else if (valor > 1800 && valor < 2200) {
        botonPresionado = 3; // Botón 4
    } else if (valor > 500 && valor < 600) {
        botonPresionado = 4; // Botón 5
    } else if (valor > 200 && valor < 300) {
        botonPresionado = 5; // Botón 6 (confirmar)
    }
    
    // Control de rebote de botones y procesamiento
    if (botonPresionado != -1 && buttonReleased && (millis() - lastButtonPress > 200)) {
        buttonReleased = false;
        lastButtonPress = millis();
        
        // Reproducir el tono asociado al botón
        if (botonPresionado < 6) {
            tone(buzzer, notas[botonPresionado], 100);
        }
        
        // Procesar la entrada según el modo
        processButtonInput(botonPresionado);
    } else if (botonPresionado == -1) {
        buttonReleased = true;
        noTone(buzzer);
    }
    
    // Actualizar la pantalla según el estado
    updateDisplay();
    
    delay(50);  // Mantener el mismo retardo para estabilidad
}

void processButtonInput(int button) {
    if (button == 5) { // Botón de confirmación
        if (!passwordMode) {
            // Iniciar el modo de entrada de contraseña
            passwordMode = true;
            currentPosition = 0;
            resetCurrentPassword();
            displayMessage("Ingrese Contrasena");
        } else {
            // Verificar si se ha completado la contraseña
            if (currentPosition == PASSWORD_LENGTH) {
                checkPassword();
            } else {
                displayMessage("Faltan digitos");
                delay(1000);
            }
        }
    } else if (passwordMode && currentPosition < PASSWORD_LENGTH) {
        // Registrar el dígito en la posición actual y avanzar
        currentPassword[currentPosition] = button;
        currentPosition++;
    }
}

void resetCurrentPassword() {
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        currentPassword[i] = -1;
    }
}

void checkPassword() {
    bool correct = true;
    
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        if (currentPassword[i] != correctPassword[i]) {
            correct = false;
            break;
        }
    }
    
    if (correct) {
        securityActivated = !securityActivated;
        if (securityActivated) {
            displayMessage("Sistema Activado");
            // Aquí podrías añadir código para activar algún relay, LED, etc.
        } else {
            displayMessage("Sistema Desactivado");
            // Aquí podrías añadir código para desactivar algún relay, LED, etc.
        }
        
        // Reproducir secuencia de tonos al activar/desactivar
        for (int i = 0; i < 3; i++) {
            tone(buzzer, securityActivated ? 1000 : 500, 100);
            delay(200);
        }
    } else {
        displayMessage("Contrasena Incorrecta");
        // Sonido de error
        tone(buzzer, 150, 500);
    }
    
    delay(1500);
    passwordMode = false;
    resetCurrentPassword();
    currentPosition = 0;
}

void updateDisplay() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    if (!passwordMode) {
        // Pantalla normal cuando no estamos ingresando contraseña
        display.setCursor(0, 0);
        display.println("Estado:");
        display.setCursor(40, 0);
        display.println(securityActivated ? "ACTIVADO" : "DESACTIVADO");
        
        display.setCursor(0, 20);
        display.println("Presiona CONF para");
        display.setCursor(0, 30);
        display.println(securityActivated ? "desactivar" : "activar");
    } else {
        // Pantalla para ingreso de contraseña
        display.setCursor(0, 0);
        display.println("Ingresando contrasena:");
        
        display.setCursor(0, 20);
        display.print(">");
        
        // Mostrar asteriscos por cada dígito ingresado
        for (int i = 0; i < PASSWORD_LENGTH; i++) {
            display.setCursor(10 + (i * 10), 20);
            if (i < currentPosition) {
                display.print("*");
            } else {
                display.print("_");
            }
        }
        
        display.setCursor(0, 40);
        display.println("CONF para verificar");
    }
    
    display.display();
}

void displayMessage(String message) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor((SCREEN_WIDTH - message.length() * 6) / 2, SCREEN_HEIGHT / 2 - 4);
    display.println(message);
    display.display();
}

void displayWelcomeScreen() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    display.setCursor(10, 5);
    display.println("SISTEMA DE SEGURIDAD");
    
    display.setCursor(15, 25);
    display.println("Iniciando...");
    
    display.display();
    delay(1500);
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("1-5: Digitos");
    display.setCursor(0, 10);
    display.println("6: Confirmar");
    display.setCursor(0, 30);
    display.println("Presiona CONF para");
    display.setCursor(0, 40);
    display.println("comenzar");
    display.display();
    delay(2000);
}