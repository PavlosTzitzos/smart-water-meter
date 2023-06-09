
#define THINGER_SERIAL_DEBUG
#include <ThingerESP8266.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <LiquidCrystal_I2C.h>

Adafruit_ADS1115 ads;

#define USERNAME "pavltt12"
#define DEVICE_ID "smart_water_meter"
#define DEVICE_CREDENTIAL "S5yN5aYG@8$o3!-D"

#define SSID "Nova-05FEE0"
#define SSID_PASSWORD "hA69FbMAbfJgHNhb"
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

#define nominal_resistance_in 10000       //Nominal resistance at 25⁰C
#define nominal_resistance_out 100000       //Nominal resistance at 25⁰C
#define nominal_temperature 25   // temperature for nominal resistance (almost always 25⁰ C)
#define beta_in 3950  // The beta coefficient or the B value of the thermistor (usually 3000-4000) check the datasheet for the accurate value.
#define beta_out 3950  // The beta coefficient or the B value of the thermistor (usually 3000-4000) check the datasheet for the accurate value.
#define Rref_in 1000   //Value of  resistor used for the voltage divider
#define Rref_out 4700   //Value of  resistor used for the voltage divider

LiquidCrystal_I2C lcd(0x27, 16, 2);

//const int FlowSensorPin = 0;
int FlowSensorState = 0;
float CountFlow = 0.0;
int tmp = 0;
//volatile double waterFlow = 0;

//#define relayPin 16

float temperature_in;
float temperature_out;

void setup(void)
{
//  pinMode(FlowSensorPin, INPUT);
  //attachInterrupt(digitalPinToInterrupt(FlowSensorPin), pulse, RISING);  //DIGITAL Pin 3: Interrupt 0
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0); lcd.print(" ");
  lcd.setCursor(0,0); lcd.print("Tin Tout(C) F(L)");
  lcd.setCursor(0,1); lcd.print(" ");
  //pinMode(vd_power_pin, OUTPUT);
  Serial.begin(9600);
  thing.add_wifi(SSID, SSID_PASSWORD);
//  pinMode(LED_BUILTIN, OUTPUT);
//  pinMode(relayPin, OUTPUT);
  //thing["valve"] << digitalPin(relayPin);
  thing["meter"] >> [](pson& out)
  {
    out["temp_in"] = temperature_in;
    out["temp_out"] = temperature_out;
    out["flow"] = CountFlow;
  };
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

  temperature_in = R_in / nominal_resistance_in;     // (R/Ro)
  temperature_in = log(temperature_in);                  // ln(R/Ro)
  temperature_in /= beta_in;                   // 1/B * ln(R/Ro)
  temperature_in += 1.0 / (nominal_temperature + 273.15); // + (1/To)
  temperature_in = 1.0 / temperature_in;                 // Invert
  temperature_in -= 273.15;                         // convert absolute temp to C
  
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
/*  tmp = digitalRead(FlowSensorPin);
  if(FlowSensorState != tmp){
    CountFlow += 1.0 / 150.0; // 150 pulses=1L (refer to product specification）
    FlowSensorState = tmp;
  }*/
  //delay(2000);
  lcd.setCursor(0, 1);
  lcd.print(temperature_in, 1);
  lcd.print(" ");
  lcd.print(temperature_out, 1);
  lcd.print(" ");
  //lcd.print(waterFlow,2);
  thing.handle();
}
/*
void pulse()   //measure the quantity of square wave
{
  waterFlow += 1.0 / 150.0; // 150 pulses=1L (refer to product specification）
}*/
