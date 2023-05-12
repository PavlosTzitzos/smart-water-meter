#include <SPI.h>  // Arduino SPI (serial peripheral interface) library  (I believe this is native with the Arduino IDE)
#include <SD.h>   // Sparkfun SD library version 1.2.4 (enables reading/writing SD cards; install this through the library manager in this IDE)

int loop_counter;

File dataFile;

void setup()
  {
  // Open serial communications
  Serial.begin(9600);
  
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);  //-3- I may have to set Arduino Pin 10 HIGH here to keep the logger selected (slave-select)
  // See if the card is present and can be initialized
  Serial.print("Initializing SD card...");
  while(1)
    {
    if (SD.begin()) break;
    Serial.println("Failed to initialize, or card not present.  Trying again...");
    delay(2000);
    }
  Serial.println("card initialized.");
  loop_counter = 0;
  // Open the file. Note that only one file can be open at a time.
  // To close this file:  dataFile.close();
  // FILE_READ: open the file for reading, starting at the BEGINNING of the file.
  // FILE_WRITE: open the file for reading and writing, starting at the END of the file.
  // The SD card library uses 8.3 format filenames and is case-insensitive.
  // So "datalog.txt" is the same as "DATALOG.txt"
  Serial.println("Opening datalog.txt...");
  while(1)
     {
     dataFile = SD.open("datalog.txt", FILE_WRITE); 
     if (dataFile) break;
     Serial.println("Failed to open datalog.txt.  Trying again...");
     delay(2000);
     }
  Serial.println("Opened datalog.txt");
  }

void loop()
  {
  char my_string[100];
  String dataString = "";  // NOTE: dataFile.println() can take char * or String;  Strings might be easier to format.
  
  sprintf(my_string, "My Test %d", loop_counter);
  
  // If the log file is available, write to it:
  if (dataFile)
    {
    dataFile.println(my_string);
    dataFile.flush();  // NOTE: if you don't flush() or close the file, you'll lose a lot of data you THOUGHT was written to it.
    // print to the serial port too:
    Serial.println(my_string);
    }
  else
    {
    // If the file isn't open, print an error, and try again to open the file.
    Serial.println(" Error opening datalog.txt");
    dataFile = SD.open("datalog.txt", FILE_WRITE); 
    }
    
  loop_counter++;
  delay(1000);
  }