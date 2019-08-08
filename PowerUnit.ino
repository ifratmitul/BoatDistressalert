#include <LiquidCrystal.h>
LiquidCrystal lcd(11, 10, 9, 8, 7, 6);
float battery, solar;
byte charge = 0;
byte load = 0;
const int charging_pin = 2;
const int load_pin = 3;
void setup()
{
  pinMode(12, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(charging_pin, OUTPUT);
  pinMode(load_pin, OUTPUT);
  digitalWrite(12, HIGH);
  digitalWrite(5, HIGH);
  delay(100);
  lcd.begin(16, 2);
  delay(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SOLAR POWER");
  lcd.setCursor(0, 1);
  lcd.print("BANK");
  delay(3000);
  lcd.clear();
}

void loop()
{
  //read battery
  battery = 0;
  for (int k = 0; k < 20; k++)
  {
    battery += analogRead(A0) * 0.0275;
    delay(10);
  }
  battery /= 20;
  battery += 0.7;

  lcd.setCursor(0, 0);
  lcd.print("BAT:");
  lcd.print(battery, 1);
  lcd.print("V");




  if (battery > 9.6)charge = 1; //full charged
  if (battery < 8.0)charge = 0; //need charging

  if (battery > 8.0)load = 1; //load active
  if (battery < 7.0)load = 0; //load deactive



  solar = 0;
  for (int k = 0; k < 20; k++)
  {
    solar += analogRead(A5);
  }
  solar /= 20;

  if (solar > 10)
  {
    if (charge == 1)
    {
      digitalWrite(charging_pin, LOW);
      lcd.setCursor(0, 1);
      lcd.print("CHR:CHARGE FULL ");
    }
    else
    {
      digitalWrite(charging_pin, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("CHR:CHARGING    ");
    }
  }
  else
  {
    digitalWrite(charging_pin, LOW);
    lcd.setCursor(0, 1);
    lcd.print("SOLAR UNAVAILABLE");
  }
  if (load == 1)
  {
    digitalWrite(load_pin, HIGH);
    lcd.setCursor(9, 0);
    lcd.print("LOAD:ON ");
  }
  else
  {
    digitalWrite(load_pin, LOW);
    lcd.setCursor(9, 0);
    lcd.print("LOAD:OFF");
  }

}









//
