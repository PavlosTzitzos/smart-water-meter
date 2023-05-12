#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1015 ads;

#define vd_power_pin 2        // 5V for the voltage divider
#define nominal_resistance 10000       //Nominal resistance at 25⁰C
#define nominal_temeprature 25   // temperature for nominal resistance (almost always 25⁰ C)
#define samplingrate 5    // Number of samples
#define beta 3950  // The beta coefficient or the B value of the thermistor (usually 3000-4000) check the datasheet for the accurate value.
#define Rref 10000   //Value of  resistor used for the voltage divider

void setup(void)
{
Serial.begin(9600);
Serial.println("Hello!");

Serial.println("Getting single-ended readings from AIN0..3");
Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");

ads.begin();
}

void loop(void)
{
int16_t adc0, adc1, adc2, adc3;

adc0 = ads.readADC_SingleEnded(0);
adc1 = ads.readADC_SingleEnded(1);
adc2 = ads.readADC_SingleEnded(2);
adc3 = ads.readADC_SingleEnded(3);
Serial.print("AIN0: ");
Serial.println(adc0);
Serial.print("AIN1: ");
Serial.println(adc1);
Serial.print("AIN2: ");
Serial.println(adc2);
Serial.print("AIN3: ");
Serial.println(adc3);
Serial.println(" ");

delay(1000);
}