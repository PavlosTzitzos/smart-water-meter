#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <LiquidCrystal_I2C.h>

Adafruit_ADS1115 ads;

#define CS_PIN 4
//#define vd_power_pin 5        // 5V for the voltage divider
#define nominal_resistance_in 10000       //Nominal resistance at 25⁰C
#define nominal_resistance_out 100000       //Nominal resistance at 25⁰C
#define nominal_temperature 25   // temperature for nominal resistance (almost always 25⁰ C)
#define sampling_rate 5    // Number of samples
#define beta_in 3950  // The beta coefficient or the B value of the thermistor (usually 3000-4000) check the datasheet for the accurate value.
#define beta_out 3950  // The beta coefficient or the B value of the thermistor (usually 3000-4000) check the datasheet for the accurate value.
#define Rref_in 1000   //Value of  resistor used for the voltage divider
#define Rref_out 4700   //Value of  resistor used for the voltage divider

int samples_in = 0;   //array to store the samples
int samples_out = 0;   //array to store the samples

LiquidCrystal_I2C lcd(0x27, 16, 2);

double waterFlow;
int FlowSensorPin = 4;
int FlowSensorState = 0;
float CountFlow = 0.0;
int tmp = 0;

void setup(void)
{
  pinMode(FlowSensorPin, INPUT);
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0); lcd.print(" ");
  lcd.setCursor(0,0); lcd.print("Tin Tout(C) F(L)");
  lcd.setCursor(0,1); lcd.print(" ");
  //pinMode(vd_power_pin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Hello!");
  Serial.println("Getting single-ended readings from AIN0..3");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");
  ads.begin();
}

void loop(void)
{
  int16_t adc0, adc1;
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  Serial.println("\n \n");
  Serial.print("ADC readings inside ");
  Serial.println(adc0);
  Serial.print("ADC readings outside");
  Serial.println(adc1);
  // Voltage :
  float voltage_adc0 = ads.computeVolts(adc0); // calculate voltage
  float voltage_adc1 = ads.computeVolts(adc1); // calculate voltage
  Serial.println(voltage_adc0);
  Serial.println(voltage_adc1);
  // Resistance :
  float R_in = Rref_in / ( 3.3/voltage_adc0 - 1);
  float R_out = Rref_out / ( 3.3/voltage_adc1 - 1);
  // Calculate NTC resistance
  Serial.print("Thermistor resistance inside");
  Serial.println(R_in);
  Serial.print("Thermistor resistance outside");
  Serial.println(R_out);

  float temperature_in;
  temperature_in = R_in / nominal_resistance_in;     // (R/Ro)
  temperature_in = log(temperature_in);                  // ln(R/Ro)
  temperature_in /= beta_in;                   // 1/B * ln(R/Ro)
  temperature_in += 1.0 / (nominal_temperature + 273.15); // + (1/To)
  temperature_in = 1.0 / temperature_in;                 // Invert
  temperature_in -= 273.15;                         // convert absolute temp to C
  
  float temperature_out;
  temperature_out = R_out / nominal_resistance_out;     // (R/Ro)
  temperature_out = log(temperature_out);                  // ln(R/Ro)
  temperature_out /= beta_out;                   // 1/B * ln(R/Ro)
  temperature_out += 1.0 / (nominal_temperature + 273.15); // + (1/To)
  temperature_out = 1.0 / temperature_out;                 // Invert
  temperature_out -= 273.15;                         // convert absolute temp to C
  Serial.print("Temperature inside ");
  Serial.print(temperature_in);
  Serial.println(" *C");
  Serial.print("Temperature outside ");
  Serial.print(temperature_out);
  Serial.println(" *C");
  tmp = digitalRead(FlowSensorPin);
  if(FlowSensorState != tmp){
    CountFlow += 1.0 / 150.0; // 150 pulses=1L (refer to product specification）
    FlowSensorState = tmp;
  }
  delay(2000);
  lcd.setCursor(0, 1);
lcd.print(temperature_in, 1);
lcd.print(" ");
lcd.print(temperature_out, 1);
lcd.print(" ");
lcd.print(waterFlow,2);
}
