#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int pinBoton = 25;  // Pin analógico donde están los botones
const int buzzer = 12;    // Pin del buzzer

// Definición de notas musicales
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988

// Melodía 1: Cumpleaños feliz
int melodia1[] = {
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,
  NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
};
int duraciones1[] = {
  4, 4, 2, 2, 2, 1,
  4, 4, 2, 2, 2, 1,
  4, 4, 2, 2, 4, 4, 2,
  4, 4, 2, 4, 4, 1
};
int longMelodia1 = sizeof(melodia1) / sizeof(melodia1[0]);

// Melodía 2: Star Wars (Imperial March)
int melodia2[] = {
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_DS4, NOTE_AS4, NOTE_G4, NOTE_DS4, NOTE_AS4, NOTE_G4
};
int duraciones2[] = {
  4, 4, 4, 8, 16, 4, 8, 16, 2
};
int longMelodia2 = sizeof(melodia2) / sizeof(melodia2[0]);

// Melodía 3: Super Mario Bros
int melodia3[] = {
  NOTE_E5, NOTE_E5, 0, NOTE_E5, 0, NOTE_C5, NOTE_E5, 0,
  NOTE_G5, 0, 0, 0, NOTE_G4, 0, 0, 0
};
int duraciones3[] = {
  8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8
};
int longMelodia3 = sizeof(melodia3) / sizeof(melodia3[0]);

// Melodía 4: La Cucaracha
int melodia4[] = {
  NOTE_C4, 0, NOTE_C4, 0, NOTE_C4, 0, NOTE_F4, 0, NOTE_A4, 0,
  0, NOTE_F4, 0, NOTE_C4, 0, NOTE_C4, 0, NOTE_C4, 0,
  NOTE_F4, 0, NOTE_A4, 0, 0, 0
};
int duraciones4[] = {
  8, 16, 8, 16, 8, 16, 8, 16, 8, 4,
  4, 8, 16, 8, 16, 8, 16, 8, 16,
  8, 16, 8, 8, 4, 4
};
int longMelodia4 = sizeof(melodia4) / sizeof(melodia4[0]);

// Melodía 5: Tetris
int melodia5[] = {
  NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
  NOTE_A4, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
  NOTE_C5, NOTE_A4, NOTE_A4
};
int duraciones5[] = {
  4, 8, 8, 4, 8, 8,
  4, 8, 8, 4, 8, 8,
  4, 8, 8, 4, 4,
  4, 4, 2
};
int longMelodia5 = sizeof(melodia5) / sizeof(melodia5[0]);

// Melodía 6: Jingle Bells
int melodia6[] = {
  NOTE_E4, NOTE_E4, NOTE_E4, 0,
  NOTE_E4, NOTE_E4, NOTE_E4, 0,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4, 0,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4, 0, NOTE_G4
};
int duraciones6[] = {
  4, 4, 2, 4,
  4, 4, 2, 4,
  4, 4, 4, 4, 2, 4,
  4, 4, 4, 4, 4, 4, 4, 8, 8,
  4, 4, 4, 4, 2, 4, 2
};
int longMelodia6 = sizeof(melodia6) / sizeof(melodia6[0]);

// Variables para el control de reproducción
bool reproduciendo = false;
int melodiaActual = 0;
int notaActual = 0;
unsigned long tiempoUltimaActualizacion = 0;
int duracionNota = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(23, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true); // Bucle infinito si la pantalla no se encuentra
  }

  display.clearDisplay();
  display.display();

  pinMode(buzzer, OUTPUT);
  pinMode(pinBoton, INPUT);
}

void loop() {
  // Leer el valor del botón
  int valor = analogRead(pinBoton);
  Serial.println(valor);

  // Mostrar en pantalla
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Valor: ");
  display.println(valor);
  
  // Seleccionar melodía según el botón presionado
  if (valor > 4020) {
    iniciarMelodia(1, "Cumpleanos Feliz");
  } 
  else if (valor > 3800 && valor < 4010) {
    iniciarMelodia(2, "Star Wars");
  } 
  else if (valor > 3200 && valor < 3700) {
    iniciarMelodia(3, "Super Mario");
  } 
  else if (valor > 1800 && valor < 2200) {
    iniciarMelodia(4, "La Cucaracha");
  } 
  else if (valor > 500 && valor < 600) {
    iniciarMelodia(5, "Tetris");
  } 
  else if (valor > 200 && valor < 300) {
    iniciarMelodia(6, "Jingle Bells");
  } 
  else if (valor < 50) {
    // Ningún botón presionado, detener reproducción
    if (reproduciendo) {
      noTone(buzzer);
      reproduciendo = false;
      melodiaActual = 0;
      notaActual = 0;
    }
  }

  // Si estamos reproduciendo una melodía, actualizar la reproducción
  if (reproduciendo) {
    unsigned long tiempoActual = millis();
    
    if (tiempoActual - tiempoUltimaActualizacion >= duracionNota) {
      // Es tiempo de tocar la siguiente nota
      siguienteNota();
      tiempoUltimaActualizacion = tiempoActual;
    }

    display.setCursor(0, 20);
    display.println("Reproduciendo:");
    display.setCursor(0, 30);
    switch (melodiaActual) {
      case 1: display.println("Cumpleanos Feliz"); break;
      case 2: display.println("Star Wars"); break;
      case 3: display.println("Super Mario"); break;
      case 4: display.println("La Cucaracha"); break;
      case 5: display.println("Tetris"); break;
      case 6: display.println("Jingle Bells"); break;
    }
    
    // Mostrar progreso
    display.setCursor(0, 50);
    display.print("Nota: ");
    display.print(notaActual);
    display.print("/");
    int totalNotas = 0;
    switch (melodiaActual) {
      case 1: totalNotas = longMelodia1; break;
      case 2: totalNotas = longMelodia2; break;
      case 3: totalNotas = longMelodia3; break;
      case 4: totalNotas = longMelodia4; break;
      case 5: totalNotas = longMelodia5; break;
      case 6: totalNotas = longMelodia6; break;
    }
    display.println(totalNotas);
  }

  display.display();
  delay(10);
}

void iniciarMelodia(int melodia, const char* nombre) {
  // Si ya estamos reproduciendo esta melodía, no hacer nada
  if (reproduciendo && melodiaActual == melodia) {
    return;
  }
  
  // Detener cualquier nota que esté sonando
  noTone(buzzer);
  
  // Iniciar la nueva melodía
  reproduciendo = true;
  melodiaActual = melodia;
  notaActual = 0;
  tiempoUltimaActualizacion = millis();
  
  // Tocar la primera nota
  siguienteNota();
}

void siguienteNota() {
  // Obtener la nota y duración actuales según la melodía seleccionada
  int nota = 0;
  int duracion = 0;
  int totalNotas = 0;
  
  switch (melodiaActual) {
    case 1:
      if (notaActual < longMelodia1) {
        nota = melodia1[notaActual];
        duracion = duraciones1[notaActual];
      }
      totalNotas = longMelodia1;
      break;
    
    case 2:
      if (notaActual < longMelodia2) {
        nota = melodia2[notaActual];
        duracion = duraciones2[notaActual];
      }
      totalNotas = longMelodia2;
      break;
    
    case 3:
      if (notaActual < longMelodia3) {
        nota = melodia3[notaActual];
        duracion = duraciones3[notaActual];
      }
      totalNotas = longMelodia3;
      break;
    
    case 4:
      if (notaActual < longMelodia4) {
        nota = melodia4[notaActual];
        duracion = duraciones4[notaActual];
      }
      totalNotas = longMelodia4;
      break;
    
    case 5:
      if (notaActual < longMelodia5) {
        nota = melodia5[notaActual];
        duracion = duraciones5[notaActual];
      }
      totalNotas = longMelodia5;
      break;
    
    case 6:
      if (notaActual < longMelodia6) {
        nota = melodia6[notaActual];
        duracion = duraciones6[notaActual];
      }
      totalNotas = longMelodia6;
      break;
  }

  // Calcular la duración en milisegundos (tempo base: 1000ms = negra)
  int tempoBase = 1000;
  duracionNota = tempoBase / duracion;
  
  // Tocar la nota (o silencio si nota = 0)
  if (nota == 0) {
    noTone(buzzer);
  } else {
    tone(buzzer, nota);
  }
  
  // Avanzar a la siguiente nota
  notaActual++;
  
  // Si llegamos al final de la melodía, volver a empezar
  if (notaActual >= totalNotas) {
    notaActual = 0;
  }
}