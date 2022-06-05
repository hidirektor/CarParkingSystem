#include <RFID.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include "pitches.h"

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 6
#define kirmiziLed 8
#define yesilLed 7
#define Buzzer 5

LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);
Servo kapiServo;
RFID rfid(SS_PIN, RST_PIN);

int gecerliKartHEX[][5] = {
  {99,97,41,17,58}
};
int aracVerileri[][3] = {
  {0, 0, 0}  
};
int otoparkKapasitesi = 4;
int saatlikUcret = 100;
int buzzerMelodi[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int notaDizisi[] = {4, 8, 8, 4, 4, 4, 4, 4};

void setup() {
    pinMode(kirmiziLed,OUTPUT);
    pinMode(yesilLed,OUTPUT);
    pinMode(Buzzer,OUTPUT);

    kapiServo.attach(SERVO_PIN);
    Serial.begin(9600);
    lcd.begin(16, 2);
    SPI.begin();
    rfid.init();
}

void loop() {
    for(int y=0; y<sizeof(gecerliKartHEX); y++) {
      aracVerileri[x][0] = 1;
      aracVerileri[x][1] = 0;
    }
    kapiServo.write(0);
    lcd.clear();
    lcd.print("Bos Yer: ");
    char kapasite[1];
    sprintf(kapasite, "%d", otoparkKapasitesi);
    lcd.print(kapasite);
    lcd.setCursor(0,1);
    lcd.print("Karti Okutun:  ");

    if(rfid.isCard()) {
        if(rfid.readCardSerial()) {
            lcd.clear();
            lcd.print("KART ID:- ");
            delay(500);
            lcd.setCursor(0, 1);
            for( int i = 0; i < 5; i++ ) {
                Serial.print(rfid.serNum[i], HEX);
                Serial.print(" ");
                lcd.print(rfid.serNum[i], HEX);
                lcd.print(" ");
            }
            delay(500);
            
            for(int x = 0; x < sizeof(gecerliKartHEX); x++) {
              for(int i=0; i<5; i++) {
                if(gecerliKartHEX[x][i] != rfid.serNum[i]) {
                    aracVerileri[x][0] = 0;
                    aracVerileri[x][1] = 0;
                    break;
                }
              }
            }
        }
        Serial.println();
        delay(1000);
        for(int z=0; z<sizeof(gecerliKartHEX); z++) {
          if(aracVerileri[z][0] == 1 && aracVerileri[z][2] == 0) {
            if(otoparkKapasitesi == 4) {
              Serial.println("\nHosgeldiniz!");
              otoparkKapasitesi--;
              aracVerileri[z][2] = 1;
              aracVerileri[z][1] = millis();
              lcd.clear();
              lcd.print("Istediginiz");
              lcd.setCursor(0, 1);
              lcd.print("Yere Parkedin!");
              delay(2000);
              digitalWrite(yesilLed, HIGH);
              int i = 0;
              while(i < 2) {
                  for (int j=0; j<12; j++) {
                      int beklemeSuresi = 1000 / notaDizisi[j];
                      tone(5, buzzerMelodi[j], beklemeSuresi);
                      int notalarArasiSure = beklemeSuresi * 1.30;
                      delay(notalarArasiSure);
                  }
                  i = i + 1;
                  delay(500);
              }
              delay(1000);
              kapiServo.write(180);
              delay(200);

              lcd.clear();
              lcd.print("Girisiniz Aktif");
              lcd.setCursor(0,1);
              lcd.print("Otoparka Girin!");

              delay(2000);
              lcd.clear();

              for(int i=10; i>0; i--) {
                  lcd.print("Otopark Girisi");
                  lcd.setCursor(0, 1);
                  lcd.print(i);
                  lcd.print(" Saniye Sonra");
                  lcd.print(" Kapanacak!");
                  delay(1000);
                  lcd.clear();
              }
              kapiServo.write(0);
              digitalWrite(yesilLed, LOW);
              delay(200);

              lcd.clear();
              lcd.print("Otopark Kapisi");
              lcd.setCursor(0, 1);
              lcd.print("Kapaniyor!");
              delay(200);

              lcd.clear();
              lcd.print("Bos Yer Sayisi: ");
              lcd.print("      ");
              lcd.print(otoparkKapasitesi);
              delay(2000);
          } else if() {
            Serial.println("\nHosgeldiniz!");
            otoparkKapasitesi--;
            lcd.clear();
            lcd.print("Uygun Bir");
            lcd.setCursor(0, 1);
            lcd.print("Yere Parkedin!");
            delay(2000);
            digitalWrite(yesilLed, HIGH);
            int i = 0;
            while(i < 2) {
                for (int j=0; j<12; j++) {
                    int beklemeSuresi = 1000 / notaDizisi[j];
                    tone(5, buzzerMelodi[j], beklemeSuresi);
                    int notalarArasiSure = beklemeSuresi * 1.30;
                    delay(notalarArasiSure);
                }
                i = i + 1;
                delay(500);
            }
            delay(1000);
            kapiServo.write(180);
            delay(200);

            lcd.clear();
            lcd.print("Girisiniz Aktif");
            lcd.setCursor(0,1);
            lcd.print("Otoparka Girin!");

            delay(2000);
            lcd.clear();

            for(int i=10; i>0; i--) {
                lcd.print("Otopark Girisi");
                lcd.setCursor(0, 1);
                lcd.print(i);
                lcd.print(" Saniye Sonra");
                lcd.print(" Kapanacak!");
                delay(1000);
                lcd.clear();
            }
            kapiServo.write(0);
            digitalWrite(yesilLed, LOW);
            delay(200);

            lcd.clear();
            lcd.print("Otopark Kapisi");
            lcd.setCursor(0, 1);
            lcd.print("Kapaniyor!");
            delay(200);

            lcd.clear();
            lcd.print("Bos Yer Sayisi: ");
            lcd.print("      ");
            lcd.print(otoparkKapasitesi);
            delay(2000);
          } else {
            Serial.println("\nLutfen Terkedin !");
            lcd.clear();
            lcd.print("Bos Yer Yok !");
            lcd.setCursor(0, 1);
            lcd.print("Lutfen Terkedin!");

            for(int i=0; i<7; i++) {
                digitalWrite(Buzzer, HIGH);
                digitalWrite(kirmiziLed, HIGH);
                delay(500);
                digitalWrite(Buzzer, LOW);
                digitalWrite(kirmiziLed, LOW);
                delay(500);
            }
            delay(1000);
          }
        } else if(aracVerileri[z][0] == 1 && aracVerileri[z][2] == 1) {
              otoparkKapasitesi++;
              int gecenSure = millis() - aracVerileri[z][1];
              int toplamUcret = saatlikUcret * gecenSure;
              lcd.clear();
              lcd.print("Toplam Ucret: ");
              lcd.print();
              lcd.print(toplamUcret);
              delay(200);
              lcd.clear();
              lcd.print("Bos Yer Sayisi: ");
              lcd.print("      ");
              lcd.print(otoparkKapasitesi);
              delay(2000);
        } else {
            Serial.println("\nLutfen Terkedin !");
            lcd.clear();
            lcd.print("Kart Bulunamadi !");
            lcd.setCursor(0, 1);
            lcd.print("Lutfen Terkedin!");

            for(int i=0; i<7; i++) {
                digitalWrite(Buzzer, HIGH);
                digitalWrite(kirmiziLed, HIGH);
                delay(500);
                digitalWrite(Buzzer, LOW);
                digitalWrite(kirmiziLed, LOW);
                delay(500);
            }
            delay(1000);
         }
       }
    }
    rfid.halt();
}
