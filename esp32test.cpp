#include <Servo.h>
#include <ESP32QRCodeReader.h>
Servo myservo;
int pos = 0;
int servoPin = 2;
ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);
struct QRCodeData qrCodeData;
void loop()
{
  myservo.write(++pos %= 180);
  if (reader.receiveQrCode(&qrCodeData, 100))
  {
    Serial.println("Found QRCode");
    if (qrCodeData.valid)
    {
      Serial.print("Payload: ");
      Serial.println((const char*)qrCodeData.payload);
    }
    else
    {
      Serial.print("Invalid: ");
      Serial.println((const char*)qrCodeData.payload);
    }
  }
}
void setup()
{
  Serial.begin(115200);
  Serial.println();
  reader.setup();
  reader.begin();
  myservo.attach(servoPin, 15, 0, 180);
}