// Exemple pour ESP32 avec écran 320*240px
// F1ATB - January 2026
// Version V3

#include "Ecran.h"
#include "CST820.h"
String SerialIn = "";
uint16_t Calibre[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int numEcran = -1;
// commandes disponibles par port serie
const char* helpSerial = R"====(
**************
Commandes pour configuration par port série  (respect majuscules, ponctuation et terminer par touche 'Enter'):
Serial port configuration commands (respect capitalization, punctuation, and end with the 'Enter' key)

E:0           | ESP32_2432S028_R_ST7789_BL21
E:1           | ESP32_024_R_ST7789_BL27
E:2           | ESP32_2432S028_R_ILI9341_BL21
E:3           | ESP32_2432S024_R_ILI9341_BL27
E:4           | ESP32_2432S024_C_ILI9341_BL27
E:5           | JC2432W328_C_ST7789_BL27
R:X           | Rotation X=0,1,2 ou 3
restart       | Pour redémarrer l'ESP32 sans appui sur le bouton EN

**************
)====";


int rotation = 1;

byte LEDs[] = { 4, 16, 17 };  //Pour les S024 RGB ILI9341. 4,17,16 pour ST7789
int cpt = 0;

LGFX* lcd = nullptr;

CST820* touch = nullptr;


void setup() {
  //Ports Série ESP
  Serial.begin(115200);
  Serial.println(helpSerial);
}


void lcd_init(ScreenType type) {
  if (type == ESP32_2432S024_C_ILI9341_BL27 || type == JC2432W328_C_ST7789_BL27) {  //Capacitive
    touch = new CST820(I2C_SDA, I2C_SCL, TP_RST, TP_INT);
    touch->begin();
  }
  lcd = new LGFX(type);
  lcd->init();
  int R = rotation;
  if (type == JC2432W328_C_ST7789_BL27) R = (R + 2) % 4;  //Rotation/touchscreen
  lcd->setRotation(R);
  lcd->fillScreen(TFT_NAVY);
  lcd->setTextColor(TFT_WHITE, TFT_NAVY);
  lcd->setTextSize(3);
  int W = lcd->textWidth("F1ATB") / 2;
  int X = lcd->width() / 2;
  lcd->setCursor(X - W, lcd->height() / 2);
  lcd->println("F1ATB");
  lcd->drawRect(10, 10, lcd->width() - 20, lcd->height() - 20, TFT_YELLOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  LireSerial();
  delay(100);
  if (numEcran >= 0 && numEcran < 4) {  //Resistive touch
    int16_t ClickX, ClickY;
    if (lcd->getTouch(&ClickX, &ClickY)) {
      Serial.println("touched");
      lcd->setBrightness(255);                //
      if (Calibre[7] == 0 && numEcran < 4) {  //Pas encore calibré les résistifs //Need to calibrate resistive
        TraceCalibr();
      } else {
        lcd->fillScreen(TFT_NAVY);

        PrintCentre(String(ClickX) + " , " + String(ClickY), -1, lcd->height() / 2 + 50, 2);
      }
    }
  }
  if (numEcran == 4 || numEcran == 5) {  //Capacitive touch
    uint8_t gesture;
    uint16_t touchX, touchY, temp;
    if (touch->getTouch(&touchX, &touchY, &gesture)) {
      switch (rotation) {
        case 0:
          touchX = 239 - touchX;
          touchY = 319 - touchY;
          break;
        case 1:
          temp = touchX;
          touchX = 319 - touchY;
          touchY = temp;
          break;
        case 3:
          temp = touchX;
          touchX = touchY;
          touchY = 239 - temp;
          break;
      }
      Serial.println("touched");
      lcd->fillScreen(TFT_NAVY);
      PrintCentre(String(touchX) + " , " + String(touchY), -1, lcd->height() / 2 + 50, 2);
      lcd->setBrightness(255);
    }
  }
  cpt++;
  cpt = cpt % 30;
  if ((numEcran >= 0) && (cpt % 10) == 0) {
    for (int i = 0; i < 3; i++) {
      pinMode(LEDs[i], OUTPUT);
      digitalWrite(LEDs[i], HIGH);
    }
    int idx = 0;
    if (cpt == 10) idx = 1;
    if (cpt == 20) idx = 2;
    ;
    digitalWrite(LEDs[idx], LOW);
    Serial.println("GPIO : " + String(LEDs[idx]) + " / LED ON");
  }
}
void LireSerial() {
  int inbyte;
  //Port Serie
  while (Serial.available() > 0) {
    inbyte = Serial.read();

    if ((inbyte == 10) || (inbyte == 13)) {
      DecodeSerial();
    } else {
      SerialIn += String(char(inbyte));
    }
  }
}
void DecodeSerial() {
  String sw;
  String valeur = "";
  int p;
  SerialIn.trim();

  p = SerialIn.indexOf(":");
  if (p > 0) {
    sw = SerialIn.substring(0, p + 1);
    valeur = SerialIn.substring(p + 1);
    sw.trim();
    valeur.trim();
  } else {
    sw = SerialIn;
  }

  if (sw.indexOf("restart") >= 0) {
    ESP.restart();
  }
  if (sw.indexOf("E:") >= 0) {

    numEcran = valeur.toInt();
    Serial.println(numEcran);
  }


  if ((sw.indexOf("H") >= 0 || sw.indexOf("?") >= 0) && p == -1) {
    Serial.println(helpSerial);
  }

  if (sw.indexOf("R:") >= 0) {
    rotation = valeur.toInt();
    Serial.println(rotation);
  }
  SerialIn = "";
  if (numEcran >= 0) {
    lcd_init((ScreenType)numEcran);
  }
}

void TraceCalibr() {  //Calibration
  lcd->fillScreen(TFT_NAVY);
  lcd->setTextColor(TFT_WHITE, TFT_NAVY);
  PrintCentre("Calibration", -1, lcd->height() / 2, 2.5);
  PrintCentre("Cliquez dans le coin", -1, lcd->height() / 2 + 50, 2);
  delay(1000);
  lcd->calibrateTouch(Calibre, TFT_MAGENTA, TFT_WHITE, 30);  // Runs a test that has you touch the corners of the screen
  lcd->setTouchCalibrate(Calibre);                           // setTouch actually implements the data form calibrateTouch
  lcd->fillScreen(TFT_NAVY);
}
void PrintCentre(String S, int X, int Y, float Sz) {
  if (X < 0) X = lcd->width() / 2;
  lcd->setTextSize(Sz);
  int W = lcd->textWidth(S);
  lcd->setCursor(X - W / 2, Y + 3);
  lcd->print(S);
}
