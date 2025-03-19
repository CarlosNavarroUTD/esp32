#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Definiciones para la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definiciones para LEDs
#define RGB_R 5
#define RGB_G 4
#define RGB_B 2
#define LED_G 14
#define LED_Y 13
#define LED_R 12

// Definiciones para sensores y botones
#define LDR_PIN 35
#define BUTTON_R 18
#define BUTTON_B 19
#define BUTTON_W 21
#define POT_PIN 27

// Definiciones para el juego
#define PADDLE_WIDTH 4
#define PADDLE_HEIGHT 10
#define BALL_SIZE 3
#define PADDLE_SPEED 3
#define BALL_SPEED_X 2
#define BALL_SPEED_Y 2
#define SCORE_LIMIT 5

#define LDR_PIN 35

// Variables de estado de la fotoresistencia
int ldrValueData = 0;  
int *ldrValue = &ldrValueData;

// Variables de estado
int BR_state = 0;
int last_BR_state = 0;
int sequence = 0;  // Variable para la secuencia

int BB_state = 0;
int BW_state = 0;


// Variables para el potenciómetro
int potValue = 0;
int potLevel = 0;
int lastPotLevel = -1;  // Para detectar cambios
const int NUM_LEVELS = 7;
int top = -1;  // Índice de la pila (-1 = vacía)
const int leds[] = {LED_G, LED_Y, LED_R, RGB_R, RGB_G, RGB_B};
const int numLeds = 6;

// Variables para el juego
int paddleY = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
int ballX = SCREEN_WIDTH / 2;
int ballY = SCREEN_HEIGHT / 2;
int ballSpeedX = BALL_SPEED_X;
int ballSpeedY = BALL_SPEED_Y;
int score = 0;
bool gameOver = false;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
bool startScreen = true;
bool buttonPressed = false;

void setup() {
  Serial.begin(115200);
  Wire.begin(22, 23);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error al inicializar pantalla OLED");
    while (true);  // Bucle infinito si la pantalla no se encuentra
  }

  // Inicializar pantalla
  display.clearDisplay();
  display.display();
  delay(500);

  // Configurar pines
  pinMode(LDR_PIN, INPUT);
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(BUTTON_R, INPUT_PULLUP);
  pinMode(BUTTON_W, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);

  // Asegurar que todos los LEDs estén apagados al inicio
  all_off();
}

void loop() {
  *ldrValue = analogRead(LDR_PIN);
  BR_state = digitalRead(BUTTON_R);
  //BB_state = digitalRead(BUTTON_B);
  //BG_state = digitalRead(BUTTON_G);
  Serial.print("LDR Value:");
  Serial.println(*ldrValue);
  
  if (BR_state == LOW && last_BR_state == HIGH) {
    all_off();  // Apagar todo al cambiar de secuencia
    sequence = (sequence + 1) % 11;  // Incrementa y reinicia si supera 9
    Serial.print("Secuencia cambiada a: ");
    Serial.println(sequence);
  }
  last_BR_state = BR_state;

  // Si estamos en modo potenciómetro o Game, no limpiar la pantalla en el loop principal
  if (sequence != 9 && sequence != 10) {
    clearScreen();
    printScreen();
    display.display();
  }

  ejecutarSecuencia(sequence);
}

void ejecutarSecuencia(int seq) {
  switch (seq) {
    case 0: all_off(); break;
    case 1: g_on(); break;
    case 2: y_on(); break;
    case 3: r_on(); break;
    case 4: rgbR_on(); break;
    case 5: rgbG_on(); break;
    case 6: rgbB_on(); break;
    case 7: all_on(); break;
    case 8: carrusel(); break;
    case 9: potentiometerScreen(); break;
    case 10: gameLoop(); break;
  }
}

void g_on() {
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_Y, LOW);
  digitalWrite(LED_R, LOW);
}

void y_on() {
  digitalWrite(LED_Y, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_R, LOW);
}

void r_on() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_Y, LOW);
}

void rgbR_on() {
  all_off();

  digitalWrite(RGB_R, HIGH);
  digitalWrite(RGB_G, LOW);
  digitalWrite(RGB_B, LOW);
  
  if(*ldrValue <= 3000){
    r_on();
  }
}

void rgbG_on() {
  all_off();

  digitalWrite(RGB_R, LOW);
  digitalWrite(RGB_G, HIGH);
  digitalWrite(RGB_B, LOW);

  if(*ldrValue <= 3000){
    g_on();
  }
}

void rgbB_on() {
  all_off();

  digitalWrite(RGB_R, LOW);
  digitalWrite(RGB_G, LOW);
  digitalWrite(RGB_B, HIGH);

  if(*ldrValue <= 3000){
  y_on();
  }

}

void all_off() {
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_Y, LOW);
  digitalWrite(LED_R, LOW);
  digitalWrite(RGB_R, LOW);
  digitalWrite(RGB_G, LOW);
  digitalWrite(RGB_B, LOW);
}

void all_on() {
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_Y, HIGH);
  digitalWrite(LED_R, HIGH);
  digitalWrite(RGB_R, HIGH);
  digitalWrite(RGB_G, HIGH);
  digitalWrite(RGB_B, HIGH);
}

void checkldr() {
  Serial.print("LDR Value: ");
  //Serial.println(ldrValue);
  //delay(50);  // Reducido para mejorar la respuesta
}

void carrusel() {
  g_on();
  delay(100);
  y_on();
  delay(100);
  r_on();
  delay(100);
  rgbR_on();
  delay(100);
  rgbG_on();
  delay(100);
  rgbB_on();
  delay(100);
  // Regreso
  rgbG_on();
  delay(100);
  rgbR_on();
  delay(100);
  r_on();
  delay(100);
  y_on();
  delay(100);
  //g_on();
  //delay(50);
}

void clearScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
}

void potentiometerScreen() {
  // Leer valor del potenciómetro (0-4095 en ESP32)
  potValue = analogRead(POT_PIN);
  
  // Invertir la lectura del potenciómetro
  potValue = 4095 - potValue;
  
  // Convertir a nivel (0-6)
  potLevel = map(potValue, 0, 4095, 0, NUM_LEVELS - 1);
  
  // Si el nivel cambió o si es la primera vez que se muestra
  if (potLevel != lastPotLevel || lastPotLevel == -1) {
    updatePotDisplay();
    lastPotLevel = potLevel;
  }
  
  // Imprimir en serial para depuración
  Serial.print("Valor: ");
  Serial.print(potValue);
  Serial.print(" - Nivel: ");
  Serial.println(potLevel + 1);
  
  delay(50);  // Pequeño retraso para estabilidad
}

void updatePotDisplay() {
  BB_state = digitalRead(BUTTON_B);
  BW_state = digitalRead(BUTTON_W);
  
  display.clearDisplay();
  
  // Mostrar título y valor actual
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("MOD:");
  display.println("Potenciometro");
  display.println("---------------------");
  
  // Dibujar barra de progreso
  display.drawRect(0, 40, 128, 10, SSD1306_WHITE);
  int barWidth = (top >= 0) ? map(potValue, 0, 4095, 0, 126) : 0;
  display.fillRect(1, 41, barWidth, 8, SSD1306_WHITE);
  
  // Dibujar solo las líneas de posición sin números
  for (int i = 0; i < NUM_LEVELS; i++) {
    int x = map(i, 0, NUM_LEVELS - 1, 0, 128);
    display.drawLine(x, 52, x, 56, SSD1306_WHITE);
  }

  // Indicar dirección del cambio 
  if (potLevel > lastPotLevel && lastPotLevel != -1 | BB_state == LOW ) {
    if (top < numLeds - 1) {  // Si hay espacio en la pila
      top++;
      digitalWrite(leds[top], HIGH);
      Serial.println("LED encendido en posición: " + String(top));
    } else {
      Serial.println("Pila llena, no se pueden encender más LEDs");
    }
  } else if (potLevel < lastPotLevel | BW_state == LOW) {
    if (top >= 0) {  // Si hay LEDs encendidos
      digitalWrite(leds[top], LOW);
      Serial.println("LED apagado en posición: " + String(top));
      top--;
    } else {
      Serial.println("Pila vacía, no hay LEDs que apagar");
    }
  }

  display.display();
}

void gameLoop() {
  static bool gameInitialized = false;

  // Inicializar el juego la primera vez
  if (!gameInitialized) {
    showStartScreen();
    startScreen = true;
    gameOver = false;
    gameInitialized = true;
  }

  // Verificar si hemos salido del modo juego
  if (sequence != 10) {
    gameInitialized = false;
    return;
  }

  // Verificar estado de los botones (con anti-rebote básico)
  bool currentButtonB = (digitalRead(BUTTON_B) == LOW);
  bool currentButtonW = (digitalRead(BUTTON_W) == LOW);
  
  // Si cualquiera de los botones está presionado
  bool anyButtonPressed = currentButtonB || currentButtonW;
  
  // Comprobar si estamos en la pantalla de inicio
  if (startScreen) {
    if (!gameInitialized) {
      showStartScreen();
      gameInitialized = true;
    }
    
    // Solo actuar ante una presión nueva (no continua)
    if (anyButtonPressed && !buttonPressed) {
      buttonPressed = true;
      startScreen = false;
      gameOver = false;
      resetGame();
      delay(300); // Esperar para evitar rebotes
    }
    
    // Actualizar estado del botón cuando se suelta
    if (!anyButtonPressed) {
      buttonPressed = false;
    }
    return; // No continúa con el resto del loop mientras esté en pantalla inicio
  }
  
  // Comprobar si el juego ha terminado
  if (gameOver) {
    // Solo actuar ante una presión nueva (no continua)
    if (anyButtonPressed && !buttonPressed) {
      buttonPressed = true;
      startScreen = true; // Volver a la pantalla de inicio
      showStartScreen();
      delay(300); // Esperar para evitar rebotes
    }
    // Actualizar estado del botón cuando se suelta
    if (!anyButtonPressed) {
      buttonPressed = false;
    }
    return; // No continúa con el resto del loop mientras esté en game over
  }
  
  // Si llegamos aquí, estamos en el juego activo
  
  // Actualizar estado del botón cuando se suelta
  if (!anyButtonPressed) {
    buttonPressed = false;
  }
  
  // Mover la pala según los botones
  // Botón R mueve hacia arriba
  if (currentButtonB && paddleY > 0) {
    paddleY -= PADDLE_SPEED;
  }
  
  // Botón W mueve hacia abajo
  if (currentButtonW && paddleY < SCREEN_HEIGHT - PADDLE_HEIGHT) {
    paddleY += PADDLE_SPEED;
  }
  
  // Mover la pelota
  ballX += ballSpeedX;
  ballY += ballSpeedY;
  
  // Comprobar colisiones con los bordes superior e inferior
  if (ballY <= 0 || ballY >= SCREEN_HEIGHT - BALL_SIZE) {
    ballSpeedY = -ballSpeedY;
  }
  
  // Comprobar colisión con la pala
  if (ballX <= PADDLE_WIDTH && ballY + BALL_SIZE >= paddleY && ballY <= paddleY + PADDLE_HEIGHT) {
    ballSpeedX = -ballSpeedX;
    score++;
    
    // Aumentar velocidad después de cada 2 puntos
    if (score % 2 == 0 && abs(ballSpeedX) < 5) {
      if (ballSpeedX > 0) ballSpeedX++;
      else ballSpeedX--;
      
      if (ballSpeedY > 0) ballSpeedY++;
      else ballSpeedY--;
    }
  }
  
  // Comprobar si la pelota salió por el lado derecho
  if (ballX >= SCREEN_WIDTH - BALL_SIZE) {
    ballSpeedX = -ballSpeedX;
  }
  
  // Comprobar si la pelota salió por el lado izquierdo (perder)
  if (ballX <= 0) {
    gameOver = true;
    showGameOverScreen();
    return;
  }
  
  // Comprobar victoria
  if (score >= SCORE_LIMIT) {
    gameOver = true;
    showVictoryScreen();
    return;
  }
  
  // Dibujar todo
  updateDisplay();
  
  // Pequeña pausa para controlar la velocidad del juego
  delay(30);
}

void showStartScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 20);
  display.println("PONG GAME");
  display.setCursor(10, 35);
  display.println("Presiona un boton");
  display.setCursor(15, 45);
  display.println("para comenzar");
  display.display();
}

void showGameOverScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30, 20);
  display.println("GAME OVER");
  display.setCursor(20, 35);
  display.print("Puntuacion: ");
  display.println(score);
  display.setCursor(5, 50);
  display.println("Presiona para reiniciar");
  display.display();
}

void showVictoryScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("FELICIDADES!");
  display.setCursor(5, 35);
  display.println("Has ganado el juego!");
  display.setCursor(5, 50);
  display.println("Presiona para reiniciar");
  display.display();
}

void updateDisplay() {
  display.clearDisplay();
  
  // Dibujar pala (jugador)
  display.fillRect(0, paddleY, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);
  
  // Dibujar pelota con un patrón para simular color
  display.fillRect(ballX, ballY, BALL_SIZE, BALL_SIZE, SSD1306_WHITE);
  
  // Dibujar puntuación
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(SCREEN_WIDTH - 30, 5);
  display.print(score);
  
  // Actualizar pantalla
  display.display();
}

void resetGame() {
  paddleY = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
  ballX = SCREEN_WIDTH / 2;
  ballY = SCREEN_HEIGHT / 2;
  ballSpeedX = BALL_SPEED_X;
  ballSpeedY = BALL_SPEED_Y;
  score = 0;
}

void printScreen() {
  // Mostrar estado de sensores en OLED
  display.println("MOD:");
  
  // Mostrar la modalidad activa según el valor de sequence
  switch (sequence) {
    case 0: display.println("Apagado"); break;
    case 1: display.println("LED Verde"); break;
    case 2: display.println("LED Amarillo"); break;
    case 3: display.println("LED Rojo"); break;
    case 4: display.println("RGB Rojo"); break;
    case 5: display.println("RGB Verde"); break;
    case 6: display.println("RGB Azul"); break;
    case 7: display.println("Todos Encendidos"); break;
    case 8: display.println("Carrusel"); break;
    case 9: display.println("Potenciometro"); break;
    case 10: display.println("game"); break;
    default: display.println("Desconocido"); break;
  }

  display.println("");

}