#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // if this address is wrong, run I2C scanner and update it

const int buttonPin = 2;
int buttonState = 0;
int dinoPos = 1;
int obstaclePos = 15;
bool jumping = false;
unsigned long jumpStart = 0;
unsigned long lastMove = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  lcd.init();        // ✅ correct for your library
  lcd.backlight();   // turn on backlight
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dino Game Ready!");
  delay(1000);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && !jumping) {
    jumping = true;
    jumpStart = millis();
  }

  if (jumping) {
    if (millis() - jumpStart < 700) {
      dinoPos = 0;
    } else {
      jumping = false;
      dinoPos = 1;
    }
  }

  if (millis() - lastMove > 300) {
    lastMove = millis();
    obstaclePos--;

    if (obstaclePos < 0) {
      obstaclePos = 15;
    }

    lcd.clear();
    lcd.setCursor(0, dinoPos);
    lcd.print("D");

    lcd.setCursor(obstaclePos, 1);
    lcd.print("#");

    if (obstaclePos == 0 && dinoPos == 1) {
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Game Over!");
      while (true);
    }
  }
}

