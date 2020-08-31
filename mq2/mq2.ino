#include<Wire.h> //Wire könyvtár beillesztése az I2C busz használatához
#include<LiquidCrystal_I2C.h> //Az I2C Folyékony kristályos LCD kijelző kezelő könyvtára
LiquidCrystal_I2C lcd(0x27, 16, 2); //Az általunk használt kijelző karakterkészlete 16 karakter és 2 sor
#include <MQ2.h>  //Az MQ2 szenzor használatához szükséges könyvtár 
#include <Wire.h> //A Wire könyvtár beillesztése az I2C busz használatához

const int Analog_Input = A0; //konstans globális integer típusú változó mely az MQ2 szenzor pin-jét tárolja
const int led = 12; //konstans globális integer típusú változó mely a LED dióda pin-jét tárolja
const int piezo = 11; //konstans globális integer típusú változó mely a Piezo elem pin-jét tárolja

int lpg, co, smoke; //globális integer típusú változók az lpg, co, és a fust értékenk a tárolására
MQ2 mq2(Analog_Input); //Az MQ2 szenzor inicializálása és az általa használt bemenet hozzárendelése

void setup()
{
  lcd.init(); //Az LCD kijelző inicializálása
  lcd.backlight(); //Az LCD kijelző háttérvilágításának bekapcsolása
  pinMode(led, OUTPUT); //A LED diódát tartalmazó Pin kimenetté alakítása
  pinMode(piezo, OUTPUT); //A Piezo elemet tartalmazó Pin kimenetté alakítása
  Serial.begin(9600); //A soros porton történő kommunikáció bitrátája
  mq2.begin(); //Az MQ2-es szenzorral való kommunikáció indítása
  lcd.clear(); //Az LCD kijelző tartalmának a törlése
}

void loop() //ciklus
{
  lcd.setCursor(0, 0); //Kurzor pozicionálás ez esetben 0. karakter a 0. sorban
  lcd.print("LPG:"); //Megadott karakterlánc kiíratása
  lcd.setCursor(6, 0); //Kurzor pozicionálás ez esetben 6. karakter a 0. sorban
  lpg = mq2.readLPG(); //Az lpg értékének kiolvasása és változóban tárolása
  lcd.print(lpg); //A változó értékének kiíratása
  lcd.setCursor(10, 0); //Kurzor pozicionálás ez esetben 10. karakter a 0. sorban
  lcd.print("CO:"); //Megadott karakterlánc kiíratása
  co = mq2.readCO(); //A co értékének kiolvasása és változóban tárolása
  lcd.setCursor(15, 0); //Kurzor pozicionálás ez esetben 15. karakter a 0. sorban
  lcd.print(co); //A változó értékének kiíratása
  lcd.setCursor(5, 1); //Kurzor pozicionálás ez esetben 5. karakter a 1. sorban
  lcd.print("FUST:"); //Megadott karakterlánc kiíratása
  smoke = mq2.readSmoke(); //A füst értékének kiolvasása és változóban tárolása
  lcd.setCursor(13, 1); //Kurzor pozicionálás ez esetben 13. karakter a 1. sorban
  lcd.print(smoke); //A változó értékének kiíratása
  if ((smoke * 2) > 400.00) //Feltétel vizsgálat ha a füst értékének kétszerese nagyobb, mint 400.00 akkor:
  {
    lcd.clear(); //Az LCD kijelző tartalmának a törlése
    lcd.setCursor(7, 0); //Kurzor pozicionálás ez esetben 7. karakter a 0. sorban
    lcd.print("FUST"); //Megadott karakterlánc kiíratása
    lcd.setCursor(4, 1); //Kurzor pozicionálás ez esetben 4. karakter a 1. sorban
    lcd.print("ESZLELVE!"); //Megadott karakterlánc kiíratása
    
    for (int i = 0; i < 10; i++) //for ciklus mely i = 0-tól i < 10-ig fut:
    {
      tone(piezo, 1400); //Piezo elem megszólaltatása 1400 hz frekvencián
      digitalWrite(led, HIGH); //A led dióda világít
      delay(100); //Várakozás 100 mikroszekundum ideig
      tone(piezo, 1200); //Piezo elem megszólaltatása 1200 hz frekvencián
      digitalWrite(led, LOW); //A led dióda nem világít
      delay(100); //Várakozás 100 mikroszekundum ideig
      tone(piezo, 1000); //Piezo elem megszólaltatása 1000 hz frekvencián
      digitalWrite(led, HIGH); //A led dióda világít
      delay(100); //Várakozás 100 mikroszekundum ideig
      tone(piezo, 800); //Piezo elem megszólaltatása 800 hz frekvencián
      digitalWrite(led, LOW); //A led dióda nem világít
      delay(100); //Várakozás 100 mikroszekundum ideig
      tone(piezo, 600); //Piezo elem megszólaltatása 600 hz frekvencián
      digitalWrite(led, HIGH); //A led dióda világít
      delay(100); //Várakozás 100 mikroszekundum ideig
    }
    lcd.clear(); //Az LCD kijelző tartalmának a törlése
  }
  else //egyébként
  {
    digitalWrite(led, LOW); //A led dióda nem világít
    noTone(piezo); //A piezo elem deaktiválása
  }
}
