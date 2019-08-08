#include <TinyGPS++.h>
#include <SoftwareSerial.h>
SoftwareSerial GPS_SoftSerial(10, 9);
TinyGPSPlus gps;
#include<LiquidCrystal.h>
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
SoftwareSerial gsm_module(11, 12);
volatile float minutes, seconds;
volatile int degree, secs, mins;
int button_cnt = 0;
byte scan_gps_enable = 0;
byte GPS_read_ok = 0;
unsigned int time_counter = 0;
double lat_val, lng_val, alt_m_val;
uint8_t hr_val, min_val, sec_val;
bool loc_valid, alt_valid, time_valid;
char link[] = "Google map link: http://maps.google.com/maps?q=";
int gps_time_out_counter = 0;
const int buzzer = 3;
const int panic_button = 13;
unsigned int button_timer = 0;
unsigned long start;

void beep()
{
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(300);
}





void setup()
{
  lcd.begin(16, 2);
  pinMode(13, INPUT_PULLUP); //panic button
  lcd.clear();
  beep();
  gsm_module.begin(9600);
  delay(2000);
  gsm_module.println("AT"); delay(100);
  gsm_module.println(F("AT+CMGF=1\r")); delay(100);
  gsm_module.println(F("AT+CMGS=\"01763700800\"")); delay(100);
  gsm_module.println(char (26)); delay(2000);
  gsm_module.end(); delay(10);
  GPS_SoftSerial.begin(9600); delay(10);
  smartDelay(1000);
  lcd.clear(); delay(20);
  lcd.setCursor(0, 0);
  lcd.print("GPS-GSM-PANIC");
}

void loop()
{


  if (digitalRead(panic_button) == LOW)
  {
    beep();
    delay(400);//find short press
    if (digitalRead(panic_button) == HIGH)
    {
      beep();
      Send_sms();
      Send_sms1();
      beep();
    }
    else
    {
      if (digitalRead(panic_button) == LOW)
      {
        delay(2500);
        if (digitalRead(panic_button) == LOW)//found long press
        {
          beep(); beep(); beep(); beep();
          lcd.clear(); delay(20);
          lcd.setCursor(0, 0);
          lcd.print("Calling...");
          gsm_module.println("AT"); delay(300);
          gsm_module.println("ATD01763700800;"); delay(30000);//30sec delay
          lcd.setCursor(0, 0);
          lcd.print("Call end");
          delay(2000);
          lcd.clear();
        }
      }

    }
  }
  else
  {
    lcd.setCursor(0, 0);
    
  }


  send_AT();


}//end of void loop()


void Send_sms()
{
  beep();
  lcd.clear(); delay(20);
  lcd.setCursor(0, 0);
  lcd.print("Sending SMS1...");
  Get_GPS();
  gsm_module.println("AT"); delay(300);
  gsm_module.println("AT+CMGF=1"); delay(300);
  gsm_module.println("AT+CMGS=\"01763700800\""); delay(300);
  gsm_module.print("Position, Lat:");
  gsm_module.print(lat_val, 6);
  gsm_module.print("  Lon:");
  gsm_module.print(lng_val, 6);
  gsm_module.print(" Google map:");
  gsm_module.print(link);
  gsm_module.print(lat_val, 6);
  gsm_module.print(",");
  gsm_module.print(lng_val, 6);
  gsm_module.println("");
  gsm_module.println((char)26); delay(5000);
  gsm_module.end();
  GPS_SoftSerial.begin(9600);
  beep();
  lcd.clear(); delay(20);
  lcd.setCursor(0, 0);
  lcd.print("SMS1 Sent!");
  delay(30000);
  beep();
  lcd.clear();
}


void Send_sms1()
{
  beep();
  lcd.clear(); delay(20);
  lcd.setCursor(0, 0);
  lcd.print("Sending SMS2...");
  Get_GPS();
  gsm_module.println("AT"); delay(300);
  gsm_module.println("AT+CMGF=1"); delay(300);
  gsm_module.println("AT+CMGS=\"01787161449\""); delay(300);
  gsm_module.print("Position, Lat:");
  gsm_module.print(lat_val, 6);
  gsm_module.print("  Lon:");
  gsm_module.print(lng_val, 6);
  gsm_module.print(" Google map:");
  gsm_module.print(link);
  gsm_module.print(lat_val, 6);
  gsm_module.print(",");
  gsm_module.print(lng_val, 6);
  gsm_module.println("");
  gsm_module.println((char)26); delay(5000);
  gsm_module.end();
  GPS_SoftSerial.begin(9600);
  beep();
  lcd.clear(); delay(20);
  lcd.setCursor(0, 0);
  lcd.print("SMS2 Sent!");
  delay(30000);
  beep();
  lcd.clear();
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

void Get_GPS()
{
  GPS_SoftSerial.begin(9600);
  smartDelay(1000);

  lat_val = gps.location.lat(); /* Get latitude data */
  loc_valid = gps.location.isValid(); /* Check if valid location data is available */
  lng_val = gps.location.lng(); /* Get longtitude data */
  alt_m_val = gps.altitude.meters();  /* Get altitude data in meters */
  alt_valid = gps.altitude.isValid(); /* Check if valid altitude data is available */
  hr_val = gps.time.hour(); /* Get hour */
  min_val = gps.time.minute();  /* Get minutes */
  sec_val = gps.time.second();  /* Get seconds */
  time_valid = gps.time.isValid();  /* Check if valid time data is available */
  if (!loc_valid)
  {
    Serial.print("Latitude : ");
    Serial.println("*****");
    Serial.print("Longitude : ");
    Serial.println("*****");
    lcd.setCursor(0, 0);
    lcd.print("Lat:********    ");
    lcd.setCursor(0, 1);
    lcd.print("Lon:********    ");
  }
  else
  {
    DegMinSec(lat_val);
    Serial.print("Latitude in Decimal Degrees : ");
    Serial.println(lat_val, 6);
    Serial.print("Latitude in Degrees Minutes Seconds : ");
    Serial.print(degree);
    Serial.print("\t");
    Serial.print(mins);
    Serial.print("\t");
    Serial.println(secs);
    DegMinSec(lng_val); /* Convert the decimal degree value into degrees minutes seconds form */
    Serial.print("Longitude in Decimal Degrees : ");
    Serial.println(lng_val, 6);
    Serial.print("Longitude in Degrees Minutes Seconds : ");
    Serial.print(degree);
    Serial.print("\t");
    Serial.print(mins);
    Serial.print("\t");
    Serial.println(secs);
    lcd.setCursor(0, 0);
    lcd.print("Lat:");
    lcd.print(lat_val, 6);
    lcd.setCursor(0, 1);
    lcd.print("Lon:");
    lcd.print(lng_val, 6);
  }
  GPS_SoftSerial.end(); delay(10);
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (GPS_SoftSerial.available())  /* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
    /* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}

void DegMinSec( double tot_val)   /* Convert data in decimal degrees into degrees minutes seconds form */
{
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}


/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
void send_AT()
{
  GPS_SoftSerial.end(); delay(10);
  gsm_module.begin(9600); delay(10);
  gsm_module.println("AT"); delay(10);
  gsm_module.end(); delay(10);
  GPS_SoftSerial.begin(9600); delay(10);
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/




//
