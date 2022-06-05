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

int gecerliKartHEX[5] = {0xE9,0xD6,0xA4,0x59,0xC2};
int otoparkKapasitesi = 4;
boolean gecerliKartDurum = false;
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
    gecerliKartDurum = true;
    kapiServo.write(0);
    lcd.clear();
    lcd.print("Boş Yer Sayısı: ");
    lcd.print(otoparkKapasitesi);
    lcd.print(" \nLütfen Kartınızı Okutun:  ");

    if(rfid.isCard()) {
        if(rfid.readCardSerial()) {
            lcd.clear();
            lcd.print("UNIQUE ID:- ");
            delay(500);
            lcd.setCursor(0, 1);
            for( int i = 0; i < 5; i++ ) {
                Serial.print(rfid.serNum[i], HEX);
                Serial.print(" ");
                lcd.print(rfid.serNum[i], HEX);
                lcd.print(" ");
            }
            delay(500);

            for(int i=0; i<5; i++) {
                if(gecerliKartHEX[i] != rfid.serNum[i]) {
                    gecerliKartDurum = false;
                    break;
                }
            }
        }
        Serial.println();
        delay(1000);
        if(gecerliKartDurum) {
          if(otoparkKapasitesi > 0) {
            Serial.println("\nOtoparka Hoşgeldiniz!");
            otoparkKapasitesi--;
            lcd.clear();
            lcd.print("Aracınızı İstediğiniz");
            lcd.setCursor(0, 1);
            lcd.print("Yere Parkedebilirsiniz!");
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
            lcd.print("Otopark Girişiniz Aktif");
            lcd.setCursor(0,1);
            lcd.print("Lütfen Otoparka Girin!");

            delay(2000);
            lcd.clear();

            for(int i=10; i>0; i--) {
                lcd.print("Otopark Girişi");
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
            lcd.print("Otopark Kapısı");
            lcd.setCursor(0, 1);
            lcd.print("Kapanıyor!");
            delay(200);

            lcd.clear();
            lcd.print("Otoparkımızdaki Boş Yer Sayısı: ");
            lcd.print("      ");
            lcd.print(otoparkKapasitesi);
            delay(2000);
          } else {
            Serial.println("\nLütfen Otoparkı Terkedin !");
            lcd.clear();
            lcd.print("Otoparkımızda Boş Yer Yok !");
            lcd.setCursor(0, 1);
            lcd.print("Lütfen Otoparkı Terkedin!");

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
        } else {
            Serial.println("\nLütfen Otoparkı Terkedin !");
            lcd.clear();
            lcd.print("Otopark Kartı Bulunamadı !");
            lcd.setCursor(0, 1);
            lcd.print("Lütfen Otoparkı Terkedin!");

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
    rfid.halt();
}
