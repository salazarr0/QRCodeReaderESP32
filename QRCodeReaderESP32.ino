// Usar AI THINKER ESP32-CAM

#include <Arduino.h>
#include <ESP32QRCodeReader.h>

ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);

// A função onQrCodeTask, realiza uma varredura contínua em busca de novos QR codes por meio 
// de um laço 'while (true)' e, ao detectar um QR code válido, imprime os dados no Monitor Serial
void onQrCodeTask(void *pvParameters)
{
  struct QRCodeData qrCodeData;

  while (true)
  {
    if (reader.receiveQrCode(&qrCodeData, 100))
    {
      Serial.println("QRCode encontrado!");
      if (qrCodeData.valid)
      {
        Serial.print("Payload: ");
        Serial.println((const char *)qrCodeData.payload);
      }
      else
      {
        Serial.print("Inválido: ");
        Serial.println((const char *)qrCodeData.payload);
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  reader.setup(); // Inicia o hardware da câmera usando as configurações padrão da biblioteca para a placa AI-Thinker

  Serial.println("Configurando o leitor de QRCode");

  reader.beginOnCore(1); // O leitor da câmera será executado no núcleo 1 da CPU (o chip ESP32 possui dois núcleos)

  Serial.println("Inicia no core 1");

  // Por fim, crie uma tarefa do FreeRTOS para executar a função 'onQrCodeTask'.
  // Isso nos permite continuar executando o leitor de código QR em segundo plano.
  xTaskCreate(onQrCodeTask, "onQrCode", 4 * 1024, NULL, 4, NULL); 
}

void loop()
{
  delay(1000);

  // Aqui entra o restante do código, chamada de API, validações, etc.
  
}