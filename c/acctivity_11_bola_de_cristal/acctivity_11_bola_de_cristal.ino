#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Definiciones para la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definiciones para botones y LEDs
#define BTN1 18  // Botón jugador 1
#define BTN3 21  // Botón respuesta correcta
#define BTN4 25  // Botón respuesta incorrecta
#define BTN6 27  // Botón jugador 2
#define LED1 19  // LED jugador 1
#define LED2 26  // LED jugador 2

const int buttonPins[] = {BTN1, BTN3, BTN4, BTN6};
const char *buttonNames[] = {"BTN1", "BTN3", "BTN4", "BTN6"};
int btnState[4] = {0, 0, 0, 0};
int lastBtnState[4] = {1, 1, 1, 1};  // Inicialmente en HIGH porque INPUT_PULLUP

bool gameActive = false;     // Indica si hay una pregunta activa
int activePlayer = 0;        // 0: Ninguno, 1: Jugador 1, 2: Jugador 2
int score1 = 0, score2 = 0;  // Puntuaciones
String currentQuestion = "";
bool failOccurred = false;   // Indica si ya hubo un error para esta pregunta

// Banco de preguntas
String questions[] = {
  "¿Cuál es la capital de España?",
  "¿Cuántos continentes hay en el mundo?",
  "¿Quién escribió 'Don Quijote de la Mancha'?",
  "¿Cuántos planetas tiene el sistema solar?",
  "¿Cuál es el océano más grande del mundo?",
  "¿Qué elemento químico tiene el símbolo 'O'?",
  "¿Cuál es el río más largo del mundo?",
  "¿En qué año llegó Cristóbal Colón a América?",
  "¿Qué animal es conocido como el rey de la selva?",
  "¿Cuántos lados tiene un hexágono?",
  "¿Qué país tiene forma de bota?",
  "¿Quién pintó la Mona Lisa?",
  "¿Cuál es el metal más abundante en la corteza terrestre?",
  "¿Cuántos huesos tiene el cuerpo humano?",
  "¿Qué gas necesitamos para respirar?",
  "¿Cuál es el país más grande del mundo?",
  "¿Qué científico formuló la teoría de la relatividad?",
  "¿Qué animal es el símbolo de Australia?",
  "¿Qué instrumento musical tiene teclas blancas y negras?",
  "¿Cuál es el idioma más hablado en el mundo?",
  "¿Qué vitamina se obtiene del sol?",
  "¿Cómo se llama el animal terrestre más rápido?",
  "¿Cuál es la capital de México?",
  "¿Cuántos colores tiene el arcoíris?",
  "¿Qué inventó Thomas Edison?",
  "¿Cuántos corazones tiene un pulpo?",
  "¿Cuál es el país más poblado del mundo?",
  "¿Qué es más grande, un protón o un electrón?",
  "¿Cómo se llama el himno nacional de Francia?",
  "¿Qué planeta es conocido como el planeta rojo?",
  "¿Cuál es el ave que no puede volar más grande del mundo?",
  "¿Quién escribió 'Cien años de soledad'?",
  "¿Cuántas patas tiene una araña?",
  "¿Cuál es el idioma oficial de Brasil?",
  "¿Cuántos dientes tiene un ser humano adulto?",
  "¿Cómo se llama el satélite natural de la Tierra?",
  "¿Qué gas usan los globos aerostáticos para elevarse?",
  "¿En qué país se encuentra la Torre Eiffel?",
  "¿Qué animal puede cambiar de color para camuflarse?",
  "¿Cuál es el hueso más largo del cuerpo humano?",
  "¿En qué continente se encuentra Egipto?",
  "¿Qué instrumento usa un violinista?",
  "¿Cuántos años tiene un lustro?",
  "¿Quién descubrió la penicilina?",
  "¿Cuál es el desierto más grande del mundo?",
  "¿Cuántos lados tiene un pentágono?",
  "¿Qué moneda se usa en Japón?",
  "¿Qué deporte se juega en Wimbledon?",
  "¿Qué animal produce la seda?",
  "¿Cómo se llama el proceso por el cual las plantas producen oxígeno?",
  "¿Cuál es el metal líquido a temperatura ambiente?",
  "¿En qué país se encuentra la Gran Muralla China?",
  "¿Cuántos segundos tiene un minuto?",
  "¿Cuál es el océano más pequeño del mundo?",
  "¿Quién fue el primer ser humano en viajar al espacio?",
  "¿Cuál es la capital de Argentina?",
  "¿Cómo se llama el mamífero más grande del mundo?",
  "¿Qué planeta tiene anillos a su alrededor?",
  "¿Qué montaña es la más alta del mundo?",
  "¿Cuántos lados tiene un triángulo?",
  "¿Cuál es el símbolo químico del oro?",
  "¿Cuántos pulmones tiene el ser humano?",
  "¿Quién pintó 'La última cena'?",
  "¿Qué país es famoso por sus pirámides?",
  "¿Cuál es la bebida más consumida en el mundo después del agua?",
  "¿Qué significa la sigla 'ONU'?",
  "¿Cuántos músculos tiene el cuerpo humano?",
  "¿Qué gas exhalamos al respirar?",
  "¿Cuál es el metal más caro del mundo?",
  "¿Cómo se llama el proceso de conversión de agua en vapor?",
  "¿Qué filósofo escribió 'La República'?",
  "¿Cuántos anillos tiene el logotipo de los Juegos Olímpicos?",
  "¿Qué animal tiene la mordida más fuerte del mundo?",
  "¿Cómo se llama la línea imaginaria que divide la Tierra en hemisferio norte y sur?",
  "¿Qué país tiene más premios Nobel?",
  "¿Qué gas es responsable del efecto invernadero?",
  "¿Cuál es el animal más venenoso del mundo?",
  "¿Quién inventó la bombilla?",
  "¿Cuántos continentes existen?",
  "¿En qué año ocurrió la Revolución Francesa?",
  "¿Qué país es conocido como la tierra del sol naciente?",
  "¿Cómo se llama el órgano encargado de bombear la sangre en el cuerpo humano?",
  "¿Qué país tiene la mayor cantidad de islas?",
  "¿Qué civilización construyó Machu Picchu?",
  "¿Cuál es el nombre del satélite de Marte?",
  "¿Qué gas se utiliza para llenar los neumáticos de los aviones?",
  "¿Cómo se llama el proceso por el cual los árboles pierden sus hojas en otoño?",
  "¿Qué ciudad fue destruida por la erupción del Vesubio en el año 79 d.C.?",
  "¿Qué país tiene el mayor número de volcanes activos?",
  "¿Cómo se llama el líquido dentro de los cocos?",
  "¿Quién escribió 'La Odisea'?",
  "¿Cuál es el hueso más pequeño del cuerpo humano?",
  "¿Qué país tiene la selva amazónica?",
  "¿Cuántos dientes de leche tiene un niño?",
  "¿Qué es más grande, un milímetro o un centímetro?",
  "¿Qué colores primarios se combinan para obtener el color verde?",
  "¿Cómo se llama el gas inflamable presente en el agua?",
  "¿En qué año cayó el Muro de Berlín?",
  "¿Cuántas estrellas hay en la bandera de Estados Unidos?",
  "¿Qué parte del cuerpo humano sigue creciendo después de la muerte?",
  "¿Qué país es famoso por la pizza y la pasta?",
  "¿Qué deporte se juega con un bate y una pelota en el béisbol?",
  "¿Cuál es el órgano más grande del cuerpo humano?",
  "¿Cuántos corazones tiene un gusano de tierra?",
  "¿Quién fue el primer presidente de Estados Unidos?",
  "¿Cuántos lados tiene un octágono?",
  "¿Qué es un tsunami?",
  "¿Cómo se llama la sustancia que le da color a la piel?",
  "¿Cuántos ojos tienen las arañas?",
  "¿Quién escribió 'Romeo y Julieta'?",
  "¿En qué año se llegó a la Luna por primera vez?",
  "¿Cuál es el país más frío del mundo?",
  "¿Cómo se llama el fenómeno de luz en el cielo en los polos?",
  "¿Cuál es el nombre del dios griego del mar?",
  "¿Qué país produce más café en el mundo?",
  "¿Qué gas es esencial para la vida en la Tierra?",
  "¿Cuál es el animal más rápido del mundo en el agua?",
  "¿Qué famosa torre de Italia está inclinada?",
  "¿Qué país tiene más lagos en el mundo?",
  "¿Qué es más pesado, un litro de agua o un litro de aceite?",
  "¿Qué parte de la planta realiza la fotosíntesis?",
  "¿Cuál es el país más pequeño del mundo?",
  "¿Quién es el autor de 'El Principito'?",
  "¿Cuál es el color del rubí?",
  "¿Cómo se llama la lengua oficial de China?",
  "¿Cómo se llama el rey de la mitología nórdica?"
};

int numQuestions = sizeof(questions) / sizeof(questions[0]);

// Para debounce de botones
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  
  // Inicialización del bus I2C
  Wire.begin(22, 23);
  
  // Inicialización de la pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error al inicializar pantalla OLED");
    while (true);
  }
  
  // Configuración de pines
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  // Asegurar que los LEDs estén apagados al inicio
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  
  // Mostrar pantalla inicial
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("QUIZ GAME");
  display.setCursor(0, 20);
  display.println("Presiona BTN3 para");
  display.println("iniciar el juego");
  display.display();
}

void loop() {
  // Leer estado de todos los botones
  for (int i = 0; i < 4; i++) {
    int reading = digitalRead(buttonPins[i]);
    
    // Aplicar debounce para evitar lecturas falsas
    if (reading != lastBtnState[i]) {
      lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != btnState[i]) {
        btnState[i] = reading;
        
        // Si se presionó el botón (cambio de HIGH a LOW)
        if (btnState[i] == LOW) {
          Serial.print("Botón presionado: ");
          Serial.println(buttonNames[i]);
          
          // Manejar la acción del botón según el estado del juego
          procesarBoton(i);
        }
      }
    }
    
    lastBtnState[i] = reading;
  }
  
  // Actualizar la pantalla constantemente
  mostrarEstado();
  
  // Pequeña pausa para estabilidad
  delay(10);
}

void procesarBoton(int botonIndex) {
  // Botón 3 (BTN3) - Iniciar juego o respuesta correcta
  if (botonIndex == 1) {  // Índice 1 corresponde a BTN3
    if (!gameActive) {
      // Si el juego no está activo, mostrar una nueva pregunta
      nuevaPregunta();
    } 
    else if (activePlayer > 0) {
      // Si hay un jugador activo, es respuesta correcta
      if (activePlayer == 1) {
        score1++;
      } else {
        score2++;
      }
      Serial.print("Punto para Jugador ");
      Serial.println(activePlayer);
      
      // Nueva pregunta después de respuesta correcta
      nuevaPregunta();
    }
    return;
  }
  
  // Botón 4 (BTN4) - Respuesta incorrecta
  if (botonIndex == 2 && gameActive && activePlayer > 0) {
    if (!failOccurred) {
      // Primer fallo, cambiar al otro jugador
      failOccurred = true;
      if (activePlayer == 1) {
        activePlayer = 2;
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        Serial.println("Turno para Jugador 2");
      } 
      else {
        activePlayer = 1;
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        Serial.println("Turno para Jugador 1");
      }
    } 
    else {
      // Segundo fallo, nueva pregunta
      Serial.println("Ambos jugadores fallaron. Nueva pregunta.");
      nuevaPregunta();
    }
    return;
  }
  
  // Botones 1 y 6 (BTN1 y BTN6) - Selección de jugador
  if ((botonIndex == 0 || botonIndex == 3) && gameActive && activePlayer == 0) {
    activePlayer = (botonIndex == 0) ? 1 : 2;
    digitalWrite(LED1, activePlayer == 1 ? HIGH : LOW);
    digitalWrite(LED2, activePlayer == 2 ? HIGH : LOW);
    Serial.print("Jugador ");
    Serial.print(activePlayer);
    Serial.println(" seleccionado");
    return;
  }
}

void nuevaPregunta() {
  // Seleccionar pregunta aleatoria
  currentQuestion = questions[random(numQuestions)];
  
  // Reiniciar estado del juego
  gameActive = true;
  activePlayer = 0;
  failOccurred = false;  // Reiniciar contador de fallos
  
  // Apagar LEDs
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  
  Serial.println("Nueva pregunta: " + currentQuestion);
}

void mostrarEstado() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  if (!gameActive) {
    // Pantalla inicial
    display.setCursor(0, 0);
    display.println("QUIZ GAME");
    display.setCursor(0, 20);
    display.println("Presiona BTN3 para");
    display.println("iniciar el juego");
  } else {
    // Mostrar pregunta y puntuaciones
    display.setCursor(0, 0);
    display.println(currentQuestion);
    display.setCursor(0, 20);
    display.print("P1: ");
    display.print(score1);
    display.print("  P2: ");
    display.println(score2);
    
    // Mostrar estado actual
    display.setCursor(0, 40);
    if (activePlayer == 0) {
      display.println("Presiona BTN1 o BTN6");
    } else {
      display.print("Turno: Jugador ");
      display.println(activePlayer);
      if (failOccurred) {
        display.setCursor(0, 50);
        display.println("Última oportunidad!");
      }
    }
  }
  
  display.display();
}