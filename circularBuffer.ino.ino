/* Este programa implementa un buffer circular.
La entrada de datos del buffer es un pin analógico del ESP32.
Está diseñado para agregar otras etapas del proyecto en sus funciones.
Visualización de resultados en el serial.*/

#define BUFFER_SIZE 16 // Tamaño del buffer
#define BUFFER_MASK (BUFFER_SIZE - 1)  // Enmascaramiento
#define ADC_PIN 34  // Pin ADC
 
// En el buffer se van a almacenar dos datos
struct BufferItem {
  uint16_t value; // Dato analógico
  bool processed; // ¿Procesado?
};
 
BufferItem circularBuffer[BUFFER_SIZE]; // Tamaño
uint32_t bufferIndex = 0; // Índice
uint32_t itemsInBuffer = 0; // Valor en el índice
 
void setup() {
  Serial.begin(115200);
  analogReadResolution(12);  // Resolución del ADC en 12 bits
 
  // Inicializa todos los índices en el buffer
  for (int i = 0; i < BUFFER_SIZE; i++) {
    circularBuffer[i] = {0, false};  // Se inicializa la bandera en falso.
  }
}
 
void loop() {
  // Lectura analógica.
  uint16_t adcValue = analogRead(ADC_PIN);
 
  // Agrega un nuevo valor al buffer
  circularBuffer[bufferIndex & BUFFER_MASK] = {adcValue, false};  // Aún no se procesa el dato
  bufferIndex++;
  if (itemsInBuffer < BUFFER_SIZE) itemsInBuffer++;
 
  // Simula el procesamiento
  processData();
 
  // Imprime el estado del buffer
  printBufferState();
 
  delay(1000);  // 1 sec para visualización
}
 
void processData() {
  // Este ciclo for hace el corrimiento a la derecha
  for (uint32_t i = 0; i < itemsInBuffer; i++) {
    uint32_t index = (bufferIndex - 1 - i) & BUFFER_MASK;
    if (!circularBuffer[index].processed) {
      // Simula el procesamiento
      circularBuffer[index].value = complexOperation(circularBuffer[index].value);
      circularBuffer[index].processed = true; // Al terminar el procesamiento se pone la bandera en verdadero.
    }
  }
}
 
uint16_t complexOperation(uint16_t value) {
  // Aquí van las transformadas y lo de MATLAB
  // Para demostrar que sí sirve hacemos un corrimiento a la derecha por los jajas
  //uint16_t result = value << 1;  // Multiplica x2
  return (value << 1) & 0x0FFF;  // Aseguramos que sea un valor que se pueda expresar en 12 bits (0-4095)
}
 
// Función para imprimir el dato
void printBufferState() {
  Serial.println("Buffer state (Value, Processed):");
  for (int i = 0; i < BUFFER_SIZE; i++) {
    uint32_t index = (bufferIndex - 1 - i) & BUFFER_MASK;
    Serial.print("(");
    Serial.print(circularBuffer[index].value);
    Serial.print(", ");
    Serial.print(circularBuffer[index].processed ? "T" : "F");
    Serial.print(") ");
  }
  Serial.println();
}