
#include <SD.h>

#define CS_PIN 8

void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card ...");

  if (!SD.begin(CS_PIN)) {
    Serial.println("Failed to initialize, or card not present.  Trying again...");
    return;
  }
  Serial.println("card initialized.");
}

void loop(){
  File dataFile = SD.open("LOG.txt", FILE_WRITE);
  if (dataFile) {
    Serial.println("O arquivo foi aberto com sucesso.");
    dataFile.print(" | ");
    dataFile.close();
  }
  else {
    Serial.println("Error opening LOG.txt");
  }
  delay(2000);
 
}

