#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Definiciones para la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definiciones para botones
#define BUTTON_R 18  // Botón derecho
#define BUTTON_B 19  // Botón izquierdo

// Definiciones para el juego
#define PADDLE_WIDTH 4
#define PADDLE_HEIGHT 10
#define BALL_SIZE 3
#define PADDLE_SPEED 3
#define BALL_SPEED_X 2
#define BALL_SPEED_Y 2
#define SCORE_LIMIT 5

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
    while (true); // Bucle infinito si la pantalla no se encuentra
  }
  
  // Inicializar pantalla
  display.clearDisplay();
  display.display();
  delay(500);
  
  // Configurar pines
  pinMode(BUTTON_R, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  
  // Mostrar pantalla de inicio
  showStartScreen();
}

void loop() {
  // Verificar estado de los botones (con anti-rebote básico)
  bool currentButtonR = (digitalRead(BUTTON_R) == LOW);
  bool currentButtonB = (digitalRead(BUTTON_B) == LOW);
  
  // Si cualquiera de los botones está presionado
  bool anyButtonPressed = currentButtonR || currentButtonB;
  
  // Comprobar si estamos en la pantalla de inicio
  if (startScreen) {
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
    return; // No continúa con el resto del loop mientras esté en pantalla inicial
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
  if (currentButtonR && paddleY > 0) {
    paddleY -= PADDLE_SPEED;
  }
  
  // Botón B mueve hacia abajo
  if (currentButtonB && paddleY < SCREEN_HEIGHT - PADDLE_HEIGHT) {
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

void all_off() {
  // Aquí podrías apagar LEDs si los tuvieras en tu circuito
}