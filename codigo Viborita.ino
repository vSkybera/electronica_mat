#include <LedControl.h>

LedControl lc = LedControl(12, 11, 10, 1);

const int btnArriba = 2;
const int btnAbajo = 4;
const int btnIzquierda = 8;
const int btnDerecha = 7;

int serpienteX[64];
int serpienteY[64];
int tamanoSerpiente = 3;

int comidaX, comidaY;
int dirX = 1;
int dirY = 0;

unsigned long ultimaActualizacion = 0;
int velocidad = 500; 
int vidas = 3;
bool juegoTerminado = false;

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 4);
  lc.clearDisplay(0);

  pinMode(btnArriba, INPUT_PULLUP);
  pinMode(btnAbajo, INPUT_PULLUP);
  pinMode(btnIzquierda, INPUT_PULLUP);
  pinMode(btnDerecha, INPUT_PULLUP);

  iniciarJuegoCompleto();
}

void loop() {
  if (juegoTerminado) {
    pantallaGameOver();
    if (digitalRead(btnArriba) == LOW || digitalRead(btnAbajo) == LOW || 
        digitalRead(btnIzquierda) == LOW || digitalRead(btnDerecha) == LOW) {
      iniciarJuegoCompleto();
    }
    return;
  }

  leerBotones();

  if (millis() - ultimaActualizacion > velocidad) {
    moverSerpiente();
    ultimaActualizacion = millis();
  }
}

void iniciarJuegoCompleto() {
  vidas = 3;
  tamanoSerpiente = 3; 
  juegoTerminado = false;
  restablecerRonda();
}

void restablecerRonda() {
  for (int i = 0; i < tamanoSerpiente; i++) {
    serpienteX[i] = 3 - i; 
    serpienteY[i] = 4;
  }
  
  dirX = 1; dirY = 0;
  velocidad = 500; 
  generarComida();
  dibujarJuego();
}

void leerBotones() {
  if (digitalRead(btnArriba) == LOW && dirY == 0) {
    dirX = 0; dirY = -1;
  }
  if (digitalRead(btnAbajo) == LOW && dirY == 0) {
    dirX = 0; dirY = 1;
  }
  if (digitalRead(btnIzquierda) == LOW && dirX == 0) {
    dirX = -1; dirY = 0;
  }
  if (digitalRead(btnDerecha) == LOW && dirX == 0) {
    dirX = 1; dirY = 0;
  }
}

void moverSerpiente() {
  int nuevaCabezaX = serpienteX[0] + dirX;
  int nuevaCabezaY = serpienteY[0] + dirY;

  // Única condición de daño: Chocar contra las paredes externas
  if (nuevaCabezaX < 0 || nuevaCabezaX > 7 || nuevaCabezaY < 0 || nuevaCabezaY > 7) {
    perderVida();
    return;
  }

  // Se eliminó la colisión contra el propio cuerpo aquí

  if (nuevaCabezaX == comidaX && nuevaCabezaY == comidaY) {
    tamanoSerpiente++;
    if (velocidad > 150) {
      velocidad -= 35;
    }
    generarComida();
  }

  for (int i = tamanoSerpiente - 1; i > 0; i--) {
    serpienteX[i] = serpienteX[i - 1];
    serpienteY[i] = serpienteY[i - 1];
  }

  serpienteX[0] = nuevaCabezaX;
  serpienteY[0] = nuevaCabezaY;

  dibujarJuego();
}

void perderVida() {
  vidas--;
  
  for (int i = 0; i < 3; i++) {
    lc.clearDisplay(0);
    delay(150);
    dibujarJuego();
    delay(150);
  }

  if (vidas <= 0) {
    juegoTerminado = true;
  } else {
    restablecerRonda(); 
  }
}

void generarComida() {
  bool posicionValida;
  do {
    posicionValida = true;
    comidaX = random(0, 8);
    comidaY = random(0, 8);
    
    for (int i = 0; i < tamanoSerpiente; i++) {
      if (serpienteX[i] == comidaX && serpienteY[i] == comidaY) {
        posicionValida = false;
        break;
      }
    }
  } while (!posicionValida);
}

void dibujarJuego() {
  lc.clearDisplay(0);

  lc.setLed(0, comidaY, comidaX, true);

  for (int i = 0; i < tamanoSerpiente; i++) {
    lc.setLed(0, serpienteY[i], serpienteX[i], true);
  }
}

void pantallaGameOver() {
  byte triste[8] = {
    B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100
  };
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, triste[i]);
  }
}