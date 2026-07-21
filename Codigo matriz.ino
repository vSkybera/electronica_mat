#include <LedControl.h>

int din = 11;
int cs  = 10;
int clk = 9;

int ldr1 = A1; 
int ldr2 = A0; 
int ldr3 = A3; 
int ldr4 = A2; 

int encendido = 520; 

LedControl lc = LedControl(din, clk, cs, 1);

void setup() {
  Serial.begin(9600);
  lc.shutdown(0, false);       
  lc.setIntensity(0, 2);       
  lc.clearDisplay(0);          
}

void loop() {
  int cuadro1 = analogRead(ldr1); 
  int cuadro2 = analogRead(ldr2); 
  int cuadro3 = analogRead(ldr3); 
  int cuadro4 = analogRead(ldr4); 

  Serial.print("A1: "); Serial.print(cuadro1);
  Serial.print(" | A0: "); Serial.print(cuadro2);
  Serial.print(" | A3: "); Serial.print(cuadro3);
  Serial.print(" | A2: "); Serial.println(cuadro4);

  byte matrizFinal[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  int filaInicio = 2;
  int colInicio = 2;

  if (cuadro1 <= encendido) {
    filaInicio -= 2; 
  }
  if (cuadro2 <= encendido) {
    filaInicio += 2; 
  }

  if (cuadro3 <= encendido) {
    colInicio -= 2; 
  }
  if (cuadro4 <= encendido) {
    colInicio += 2; 
  }

  if (filaInicio < 0) filaInicio = 0;
  if (filaInicio > 4) filaInicio = 4;
  if (colInicio < 0) colInicio = 0;
  if (colInicio > 4) colInicio = 4;

  for (int f = 0; f < 4; f++) {       
    for (int c = 0; c < 4; c++) {     
      int filaActual = filaInicio + f;
      int colActual = colInicio + c;
      matrizFinal[filaActual] |= (B10000000 >> colActual);
    }
  }

  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, matrizFinal[i]);
  }

  delay(50); 
}