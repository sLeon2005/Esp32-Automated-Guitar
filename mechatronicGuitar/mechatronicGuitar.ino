#include <SPI.h>
#include <SD.h>
#include <Adafruit_NeoPixel.h>

// pin CS micro SD
const uint8_t pinCSmicroSD = 10;

// pines solenoides
const uint8_t solenoide1 = 4;
const uint8_t solenoide2 = 5;
const uint8_t solenoide3 = 6;
const uint8_t solenoide4 = 7;
const uint8_t solenoide5 = 15;
const uint8_t solenoide6 = 16;
const uint8_t solenoide7 = 17;
const uint8_t solenoide8 = 18;
const uint8_t solenoide9 = 8;

const uint8_t solenoideX = 255;  // este es el "pin" de las notas que son cuerdas al aire, se interpreta como que no se activa solenoide

// pin neopixel
const uint8_t pinNeopixel = 9;
const uint8_t numPixels = 3;

Adafruit_NeoPixel strip(numPixels, pinNeopixel, NEO_GRB + NEO_KHZ800);

// pines dip switch
const uint8_t dipSwitch1 = 37;
const uint8_t dipSwitch2 = 36;
const uint8_t dipSwitch3 = 35;

// servos
const uint8_t canalCuerda_E = 3;
const uint8_t canalCuerda_A = 1;
const uint8_t canalCuerda_D = 2;
const uint8_t canalCuerda_G = 0;
const uint8_t canalCuerda_B = 4;
const uint8_t canalCuerda_e = 5;

const uint8_t pinCuerda_E = 42;
const uint8_t pinCuerda_A = 41;
const uint8_t pinCuerda_D = 40;
const uint8_t pinCuerda_G = 39;
const uint8_t pinCuerda_B = 38;
const uint8_t pinCuerda_e = 14;

const uint8_t freqServo = 50;
const uint8_t resolucionServo = 10;

const uint8_t dutyPosA = 67;
const uint8_t dutyPosB = 83;

struct NotaInstrumento {
  uint8_t midiID;        // ID de la nota MIDI (0–127)
  uint8_t pinSolenoide;  // Pin de salida para activar el solenoide
  uint8_t canalPWM;      // Objeto Servo asociado
};

NotaInstrumento notas[] = {
  // midi, pin, servo
  { 48, solenoideX, canalCuerda_D },  // C3, traste 0
  { 49, solenoide8, canalCuerda_D },
  { 50, solenoide1, canalCuerda_D },  // D3, traste 2
  { 51, solenoide2, canalCuerda_D },  // D#3, traste 3
  { 53, solenoide3, canalCuerda_D },  // F3, traste 5
  { 54, solenoide4, canalCuerda_D },  // F#3, traste 6
  { 55, solenoide5, canalCuerda_D },  // G3, traste 7
  { 56, solenoide6, canalCuerda_D },  // G#3, traste 8
  { 57, solenoide7, canalCuerda_D },  // A3, traste 9
  { 58, solenoide9, canalCuerda_D }   // A#3, traste 10

};

const int numNotas = sizeof(notas) / sizeof(notas[0]);

void setup() {
  Serial.begin(115200);
  if (!SD.begin(pinCSmicroSD)) {
    Serial.println("No se pudo iniciar la SD");
    return;
  }

  // inicializar pines solenoides
  pinMode(solenoide1, OUTPUT);
  pinMode(solenoide2, OUTPUT);
  pinMode(solenoide3, OUTPUT);
  pinMode(solenoide4, OUTPUT);
  pinMode(solenoide5, OUTPUT);
  pinMode(solenoide6, OUTPUT);
  pinMode(solenoide7, OUTPUT);
  pinMode(solenoide8, OUTPUT);
  pinMode(solenoide9, OUTPUT);

  for (int i = 0; i < numNotas; i++) {
    // pone todos los pines en output y low
    if (notas[i].pinSolenoide != 255) {
      digitalWrite(notas[i].pinSolenoide, LOW);

      // anuncia la conexión entre el pin y nota midi
      Serial.print("Se conectó el pin ");
      Serial.print(notas[i].pinSolenoide);
      Serial.print(" a la nota midi ");
      Serial.println(notas[i].midiID);
    }
  }

  // incializar pines dipswitch
  pinMode(dipSwitch1, INPUT_PULLUP);
  pinMode(dipSwitch2, INPUT_PULLUP);
  pinMode(dipSwitch3, INPUT_PULLUP);

  // servos
  // ledcSetup(canalCuerda_E, freqServo, resolucionServo); // configuración de cada canal
  // ledcSetup(canalCuerda_A, freqServo, resolucionServo);
  ledcSetup(canalCuerda_D, freqServo, resolucionServo);
  // ledcSetup(canalCuerda_G, freqServo, resolucionServo);
  // ledcSetup(canalCuerda_B, freqServo, resolucionServo);
  // ledcSetup(canalCuerda_e, freqServo, resolucionServo);

  // ledcAttachPin(pinCuerda_E, canalCuerda_E); // bindear canal con pin de servo
  // ledcAttachPin(pinCuerda_A, canalCuerda_A);
  ledcAttachPin(pinCuerda_D, canalCuerda_D);
  // ledcAttachPin(pinCuerda_G, canalCuerda_G);
  // ledcAttachPin(pinCuerda_B, canalCuerda_B);
  // ledcAttachPin(pinCuerda_e, canalCuerda_e);

  // ledcWrite(canalCuerda_E, dutyPosA); // poner servos en posición A
  // ledcWrite(canalCuerda_A, dutyPosA);
  ledcWrite(canalCuerda_D, dutyPosA);
  // ledcWrite(canalCuerda_G, dutyPosA);
  // ledcWrite(canalCuerda_B, dutyPosA);
  // ledcWrite(canalCuerda_e, dutyPosA);

  // iniciar leds
  strip.begin();
  strip.show();
  strip.setBrightness(20);

  strip.setPixelColor(0, strip.Color(150, 0, 0));
  strip.show();
  delay(500);

  strip.setPixelColor(1, strip.Color(0, 150, 0));
  strip.show();
  delay(500);

  strip.setPixelColor(2, strip.Color(0, 0, 150));
  strip.show();
}

void loop() {

  int b0 = !digitalRead(dipSwitch3);  // bit 0
  int b1 = !digitalRead(dipSwitch2);  // bit 1
  int b2 = !digitalRead(dipSwitch1);  // bit 2

  // Forma el número binario
  uint8_t idCancion = (b2 << 2) | (b1 << 1) | b0;
  String nombreArchivo = "/cancion" + String(idCancion) + ".txt";
  Serial.println(nombreArchivo);

  File f = SD.open(nombreArchivo);
  if (!f) {
    // en caso de que no jale...
    Serial.println("No se pudo abrir el archivo de la canción...");
    return;
  }

  while (f.available()) {
    // se lee el archivo línea por línea, hasta el line break
    String line = f.readStringUntil('\n');
    int commaIndex = line.indexOf(',');
    if (commaIndex > 0) {
      // conseguir id de la nota
      int note = line.substring(0, commaIndex).toInt();

      // conseguir duración de la nota en millis
      int duration = line.substring(commaIndex + 1).toInt();

      // imprimir nota midi y duración en millis
      Serial.print(note);
      Serial.print("     ");
      Serial.println(duration);

      // en este for se leen todas las notas y tiempos
      for (int i = 0; i < numNotas; i++) {

        // checa el diccionario que relaciona notas midi con solenoide/servo
        if (notas[i].midiID == note) {

          // función para accionar los servos dependiendo de la posición en la que estén
          if (ledcRead(notas[i].canalPWM) == dutyPosA) {
            ledcWrite(notas[i].canalPWM, dutyPosB);
          } else {
            ledcWrite(notas[i].canalPWM, dutyPosA);
          }

          // checa si es cuerda al aire, para no activar solenoide
          if (notas[i].pinSolenoide == solenoideX) {  // sin solenoide
            delay(duration);
          } else {                                      // con solenoide
            digitalWrite(notas[i].pinSolenoide, HIGH);  // Activa solenoide
            delay(duration);
            digitalWrite(notas[i].pinSolenoide, LOW);  // Desactiva
          }
        }
      }
    }
  }

  f.close();

  delay(2400);
}
