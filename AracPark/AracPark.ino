#include <SPI.h>
#include <LiquidCrystal.h>
#include <MFRC522.h>
#include "pitches.h"
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define servoPin 6
#define kirmiziLed 8
#define yesilLed 7
#define Buzzer 5

LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);
Servo kapiServo;
MFRC522 rfid(SS_PIN, RST_PIN);

byte gecerliKartBir[4] = {
  99,
  97,
  41,
  17
};

byte gecerliKartIki[4] = {
  243,
  19,
  217,
  18
};

byte gecerliKartUc[4] = {
  147,
  229,
  233,
  18
};

byte gecerliKartDort[4] = {
  35,
  247,
  154,
  13
};

//giriş yapıp yapmama durumu, kaldığı süre
int girisYaptiBir = 0;
int kalmaSuresiBir = 0;

int girisYaptiIki = 0;
int kalmaSuresiIki = 0;

int girisYaptiUc = 0;
int kalmaSuresiUc = 0;

int girisYaptiDort = 0;
int kalmaSuresiDort = 0;

int okutulanKart = 0;
int otoparkKapasitesi = 4;
int saatlikUcret = 100;
int buzzerMelodi[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int notaDizisi[] = {4, 8, 8, 4, 4, 4, 4, 4};

void setup() {
  pinMode(kirmiziLed, OUTPUT);
  pinMode(yesilLed, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  kapiServo.attach(servoPin);
  Serial.begin(9600);
  lcd.begin(16, 2);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  genelBilgiLCD();
  delay(2000);

  if(kartKontrolBir()) {
    okutulanKart = 1;
  } else if(kartKontrolIki()) {
    okutulanKart = 2;
  } else if(kartKontrolUc()) {
    okutulanKart = 3;
  } else if(kartKontrolDort()) {
    okutulanKart = 4;
  }
  
  if (okutulanKart == 1) {
    kalmaSuresiBir = millis();
    ekranaYazdir(2);
    genelSistem(1);
  } else if(okutulanKart == 2) {
    kalmaSuresiIki = millis();
    ekranaYazdir(2);
    genelSistem(2);
  } else if(okutulanKart == 3) {
    kalmaSuresiUc = millis();
    ekranaYazdir(2);
    genelSistem(3);
  } else if(okutulanKart == 4) {
    kalmaSuresiDort = millis();
    ekranaYazdir(2);
    genelSistem(4);
  } else {
    ekranaYazdir(1);
  }
  rfid.PICC_HaltA();
}

boolean kartKontrolBir() {
  for(int i=0; i<4; i++) {
    if(rfid.uid.uidByte[i] != gecerliKartBir[i]) {
      return false;
    }
  }
  return true;
}

boolean kartKontrolIki() {
  for(int i=0; i<4; i++) {
    if(rfid.uid.uidByte[i] != gecerliKartIki[i]) {
      return false;
    }
  }
  return true;
}

boolean kartKontrolUc() {
  for(int i=0; i<4; i++) {
    if(rfid.uid.uidByte[i] != gecerliKartUc[i]) {
      return false;
    }
  }
  return true;
}

boolean kartKontrolDort() {
  for(int i=0; i<4; i++) {
    if(rfid.uid.uidByte[i] != gecerliKartDort[i]) {
      return false;
    }
  }
  return true;
}

void genelBilgiLCD() {
  char bosYerSayisiString[4];
  sprintf(bosYerSayisiString, "%d", otoparkKapasitesi);
  lcd.clear();
  lcd.print("BOS YER: ");
  lcd.setCursor(0, 1);
  lcd.print(bosYerSayisiString);
  delay(1000);
  lcd.clear();
  lcd.print("Karti Okutun: ");
  lcd.setCursor(0, 1);
  lcd.print("ID: X");
}

void ekranaYazdir(int type) {
  if(type == 1) {
    lcd.clear();
    lcd.print("Hatali Kart !");
    lcd.setCursor(0, 1);
    lcd.print("ID");
    for(int k=0; k<4; k++) {
      lcd.print(rfid.uid.uidByte[k]);
      lcd.print(" ");
    }
  } else if(type == 2) {
    lcd.clear();
    lcd.print("Onay Verildi !");
    lcd.setCursor(0, 1);
    lcd.print("ID");
    for(int k=0; k<4; k++) {
      lcd.print(rfid.uid.uidByte[k]);
      lcd.print(" ");
    }
  } else if(type == 3) {
    lcd.clear();
    lcd.print("OTOPARK DOLU !");
    lcd.setCursor(0, 1);
    lcd.print("SONRA GEL ! :D");
  } else if(type == 4) {
    lcd.clear();
    lcd.print("HOSGELDIN!");
    lcd.setCursor(0, 1);
    delay(1000);
    lcd.clear();
    lcd.print("ISTEDIGIN YERE");
    lcd.setCursor(0, 1);
    lcd.print("PARKEDEBILIRSIN!");
  } else if(type == 5) {
    lcd.clear();
    lcd.print("Girisiniz Aktif");
    lcd.setCursor(0, 1);
    lcd.print("Otoparka Girin!");
  } else if(type == 6) {
    lcd.clear();
    for(int i=10; i>0; i--) {
      lcd.print("Otopark Girisi");
      lcd.setCursor(0, 1);
      lcd.print(i);
      lcd.print(" sn. Kapanacak");
      delay(1000);
      lcd.clear();
    }
  } else if(type == 7) {
    lcd.clear();
    lcd.print("Otopark Kapisi");
    lcd.setCursor(0, 1);
    lcd.print("Kapaniyor!");
  } else if(type == 8) {
    char bosYerSayisiString[4];
    sprintf(bosYerSayisiString, "%d", otoparkKapasitesi);
    lcd.clear();
    lcd.print("Bos Yer Sayisi: ");
    lcd.setCursor(0, 1);
    lcd.print(bosYerSayisiString);
    delay(500);
  } else if(type == 9) {
    lcd.clear();
    lcd.print("HOSGELDIN!");
    lcd.setCursor(0, 1);
    delay(100);
    lcd.clear();
    lcd.print("UYGUN YERE");
    lcd.setCursor(0, 1);
    lcd.print("PARKEDEBILIRSIN!");
  } else if(type == 10) {
    lcd.clear();
    lcd.print("HOSCAKALIN !");
  }
}

void onayMelodi() {
  digitalWrite(yesilLed, HIGH);
  int i=0;
  while(i < 2) {
    for(int j=0; j<12; j++) {
      int beklemeSuresi = 1000 / notaDizisi[j];
      tone(5, buzzerMelodi[j], beklemeSuresi);
      int notalarArasiSure = beklemeSuresi * 1.30;
      delay(notalarArasiSure);
    }
    i = i+1;
    delay(500);
  }
}

void redMelodi() {
  for(int i=0; i<7; i++) {
    digitalWrite(Buzzer, HIGH);
    digitalWrite(kirmiziLed, HIGH);
    delay(500);
    digitalWrite(Buzzer, LOW);
    digitalWrite(kirmiziLed, LOW);
    delay(500);
  }
}

void genelSistem(int cardType) {
  if(cardType == 1) {
    if(girisYaptiBir == 0) {
      if(otoparkKapasitesi == 4) {
        otoparkKapasitesi--;
        girisYaptiBir = 1;
        ekranaYazdir(4);
        //onayMelodi();
        delay(500);
        kapiServo.write(180);
        delay(200);
        ekranaYazdir(5);
        delay(200);
        ekranaYazdir(6);
        kapiServo.write(0);
        digitalWrite(yesilLed, LOW);
        delay(200);
        ekranaYazdir(7);
        ekranaYazdir(8);
      } else if(otoparkKapasitesi < 4 && otoparkKapasitesi != 0) {
        otoparkKapasitesi--;
        girisYaptiBir = 1;
        ekranaYazdir(9);
        //onayMelodi();
        delay(500);
        kapiServo.write(180);
        delay(200);
        ekranaYazdir(5);
        delay(200);
        ekranaYazdir(6);
        kapiServo.write(0);
        digitalWrite(yesilLed, LOW);
        delay(200);
        ekranaYazdir(7);
        ekranaYazdir(8);
      } else {
        ekranaYazdir(3);
        //redMelodi();
      }
    } else {
      otoparkKapasitesi++;
      girisYaptiBir = 0;
      ekranaYazdir(10);
      int gecenSure = millis() - kalmaSuresiBir;
      int toplamUcret = saatlikUcret * gecenSure + 7;
      char toplamUcretString[4];
      sprintf(toplamUcretString, "%d", toplamUcret);
      lcd.clear();
      lcd.print("Toplam Ucret: ");
      lcd.print("");
      lcd.print(toplamUcretString);
      delay(2000);
      ekranaYazdir(8);
      delay(200);
    }
  } else if(cardType == 2) {
    if(girisYaptiIki == 0) {
      if(otoparkKapasitesi == 4) {
        otoparkKapasitesi--;
        girisYaptiIki = 1;
        ekranaYazdir(4);
        //onayMelodi();
        delay(500);
        kapiServo.write(180);
        delay(200);
        ekranaYazdir(5);
        delay(200);
        ekranaYazdir(6);
        kapiServo.write(0);
        digitalWrite(yesilLed, LOW);
        delay(200);
        ekranaYazdir(7);
        ekranaYazdir(8);
      } else if(otoparkKapasitesi < 4 && otoparkKapasitesi != 0) {
        otoparkKapasitesi--;
        girisYaptiIki = 1;
        ekranaYazdir(9);
        //onayMelodi();
        delay(500);
        kapiServo.write(180);
        delay(200);
        ekranaYazdir(5);
        delay(200);
        ekranaYazdir(6);
        kapiServo.write(0);
        digitalWrite(yesilLed, LOW);
        delay(200);
        ekranaYazdir(7);
        ekranaYazdir(8);
      } else {
        ekranaYazdir(3);
        //redMelodi();
      }
    } else {
      otoparkKapasitesi++;
      girisYaptiIki = 0;
      ekranaYazdir(10);
      int gecenSure = millis() - kalmaSuresiIki;
      int toplamUcret = saatlikUcret * gecenSure + 7;
      char toplamUcretString[4];
      sprintf(toplamUcretString, "%d", toplamUcret);
      lcd.clear();
      lcd.print("Toplam Ucret: ");
      lcd.print("");
      lcd.print(toplamUcretString);
      delay(2000);
      ekranaYazdir(8);
      delay(200);
    }
  } else if(cardType == 3) {
    if(girisYaptiUc == 0) {
      if(otoparkKapasitesi == 4) {
        otoparkKapasitesi--;
        girisYaptiUc = 1;
        ekranaYazdir(4);
        //onayMelodi();
        delay(500);
        kapiServo.write(180);
        delay(200);
        ekranaYazdir(5);
        delay(200);
        ekranaYazdir(6);
        kapiServo.write(0);
        digitalWrite(yesilLed, LOW);
        delay(200);
        ekranaYazdir(7);
        ekranaYazdir(8);
      } else if(otoparkKapasitesi < 4 && otoparkKapasitesi != 0) {
        otoparkKapasitesi--;
        girisYaptiUc = 1;
        ekranaYazdir(9);
        //onayMelodi();
        delay(500);
        kapiServo.write(180);
        delay(200);
        ekranaYazdir(5);
        delay(200);
        ekranaYazdir(6);
        kapiServo.write(0);
        digitalWrite(yesilLed, LOW);
        delay(200);
        ekranaYazdir(7);
        ekranaYazdir(8);
      } else {
        ekranaYazdir(3);
        //redMelodi();
      }
    } else {
      otoparkKapasitesi++;
      girisYaptiUc = 0;
      ekranaYazdir(10);
      int gecenSure = millis() - kalmaSuresiUc;
      int toplamUcret = saatlikUcret * gecenSure + 7;
      char toplamUcretString[4];
      sprintf(toplamUcretString, "%d", toplamUcret);
      lcd.clear();
      lcd.print("Toplam Ucret: ");
      lcd.print("");
      lcd.print(toplamUcretString);
      delay(2000);
      ekranaYazdir(8);
      delay(200);
    }
  } else if(cardType == 4) {
    if(girisYaptiDort == 0) {
      if(otoparkKapasitesi == 4) {
        otoparkKapasitesi--;
        girisYaptiDort = 1;
        ekranaYazdir(4);
        //onayMelodi();
        delay(500);
        kapiServo.write(180);
        delay(200);
        ekranaYazdir(5);
        delay(200);
        ekranaYazdir(6);
        kapiServo.write(0);
        digitalWrite(yesilLed, LOW);
        delay(200);
        ekranaYazdir(7);
        ekranaYazdir(8);
      } else if(otoparkKapasitesi < 4 && otoparkKapasitesi != 0) {
        otoparkKapasitesi--;
        girisYaptiDort = 1;
        ekranaYazdir(9);
        //onayMelodi();
        delay(500);
        kapiServo.write(180);
        delay(200);
        ekranaYazdir(5);
        delay(200);
        ekranaYazdir(6);
        kapiServo.write(0);
        digitalWrite(yesilLed, LOW);
        delay(200);
        ekranaYazdir(7);
        ekranaYazdir(8);
      } else {
        ekranaYazdir(3);
        //redMelodi();
      }
    } else {
      otoparkKapasitesi++;
      girisYaptiDort = 0;
      ekranaYazdir(10);
      int gecenSure = millis() - kalmaSuresiDort;
      int toplamUcret = saatlikUcret * gecenSure + 7;
      char toplamUcretString[4];
      sprintf(toplamUcretString, "%d", toplamUcret);
      lcd.clear();
      lcd.print("Toplam Ucret: ");
      lcd.print("");
      lcd.print(toplamUcretString);
      delay(2000);
      ekranaYazdir(8);
      delay(200);
    }
  }
}
