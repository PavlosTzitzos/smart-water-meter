#include <SD.h>

const int chipSelect = 4;
const int numSamples = 5;
int samples[numSamples];
int sampleIndex = 0;
float temperature;
File dataFile;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("Error initializing SD card.");
    return;
  }

  // Create new file for data logging
  dataFile = SD.open("data.txt", FILE_WRITE);
  if (!dataFile) {
    Serial.println("Error creating file.");
    return;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
// Take 5 analog readings and store them in an array
  for (int i = 0; i < numSamples; i++) {
    samples[i] = analogRead(A0);
    delay(1000);
  }

  // Calculate the average value of the samples
  int sum = 0;
  for (int i = 0; i < numSamples; i++) {
    sum += samples[i];
  }
  float avg = (float)sum / numSamples;

  // Convert analog reading to temperature
  temperature = (avg * 5.0 / 1023.0 - 0.5) * 100.0;

  // Write data to SD card
  dataFile.println(temperature);
  dataFile.flush();
  Serial.println("Data logged to SD card.");

  // Wait for 1 hour before taking the next reading
  delay(3600000);
}
