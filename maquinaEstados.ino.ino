/* Máquina de estados. Implementa tres botones. Uno activa el sistema,
otro disminuye el tono y el último incrementa el tono. Datos visualizados 
en el monitor serial. */

//Definición de pines para los botones.
const int LESS_BUTTON_PIN = 12;        //Botón para disminuir el tono.
const int ENABLE_BUTTON_PIN = 14;      //Botón para activar o desactivar el pedal.
const int MORE_BUTTON_PIN = 27;        //Botón para aumentar el tono.
//Estados iniciales de los botones.
int lessButtonState = HIGH;
int moreButtonState = HIGH;
//Estados iniciales de las variables.
float Tone = 0.0;
int Semitone = 0;
volatile bool systemEnabled = false;
//Variables para evitar rebotes.
volatile unsigned long lastInterruptTime = 0;
unsigned long debounceDelay = 200;
//Función que genera la interrupción.
void IRAM_ATTR enableISR() {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > debounceDelay) {
    systemEnabled = !systemEnabled;
    lastInterruptTime = interruptTime;
  }
}
void setup() {
  Serial.begin(115200);

  //Se establecen los botones como entradas usando la resistencia interna.
  pinMode(LESS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ENABLE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MORE_BUTTON_PIN, INPUT_PULLUP);

  //Se asocia la interrupción al botón "enable".
  attachInterrupt(digitalPinToInterrupt(ENABLE_BUTTON_PIN), enableISR, FALLING);

  Serial.println("Pedal Apagado");
}
void loop() {
  //Interfaz de sondeo para los botones de tono.
  int lessReading = digitalRead(LESS_BUTTON_PIN);
  int moreReading = digitalRead(MORE_BUTTON_PIN);

  //Se actualiza "Tone" según sea el caso.
  if (systemEnabled) {
    if (lessReading == LOW && Tone > -2.0) {
      Tone -= 0.5;
      updateSemitone();
    }
    if (moreReading == LOW && Tone < 2.0) {
      Tone += 0.5;
      updateSemitone();
    }
  }

  //Verifica si el estado del sistema ha cambiado
  static bool lastSystemState = false;
  if (systemEnabled != lastSystemState) {
    Serial.println(systemEnabled ? "Pedal Encendido" : "Pedal Apagado");
    lastSystemState = systemEnabled;
  }

  //Delay para evitar rebotes
  delay(200);     //Controla la sensibilidad de los botones de tono.
}
//Función para actualizar el semitono
void updateSemitone() {
  Semitone = abs(round(Tone / 0.5));
  Serial.print("# de Tonos: ");
  Serial.print(Tone);
  Serial.print("y # de Semitonos: ");
  Serial.println(Semitone);
}