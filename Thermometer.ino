#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

int main()
{
  init();
  Serial.begin(9600);

  LiquidCrystal_I2C lcd(0x27, 16, 2);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  
  const int thermistorPin = A0;
  const float VCC = 5.0; // 5V
  const float R_FIXED = 220.0; // using 220ohm resistor
  const float R0 = 33.0;
  const float BETA = 3300.0;
  const float T0 = 298.15; // 25°C in Kalvin

  float lastTempC = -1;
  while (true) {
      int16_t reading = analogRead(thermistorPin); // thermistor reading
      float measuredVoltage = reading / 1023.0 * VCC; // convert to volts

      // converts thermistor reading to the actual voltage
      // example: reading = 884, VCC = 5V -> measuredVoltage ≈ 4.32 V.
      float thermistorResistance = R_FIXED * (VCC / measuredVoltage - 1.0);

      // calculating temp in celcius
      float tempC = 1.0 / (1.0 / T0 + log(thermistorResistance / R0) / BETA) - 273.15;

      if (abs(lastTempC - tempC) > 1) {
        lcd.setCursor(6, 0); // move cursor after "Temp: "
        lcd.print("      "); // clear previous value

        lcd.setCursor(6, 0);
        lcd.print(tempC, 1); // 1 decimal place
        lcd.write(223);
        lcd.print("C");

        lastTempC = tempC;
        delay(500);
      }
  }

  return 0;
}