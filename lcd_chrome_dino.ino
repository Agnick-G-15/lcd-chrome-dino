#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Dino Game
#define SPRITE_RUN1  1
#define SPRITE_RUN2  2
#define SPRITE_JUMP1 3
#define SPRITE_JUMP2 4
#define SPRITE_CACTUS1 5
#define SPRITE_CACTUS2 6
#define SPRITE_BIRD  7   
#define SPRITE_JUMP_UPPER 0   

// Pins
#define PIN_AUTOPLAY 8   
#define BUTTON_PIN 2

// Game state
int dinoY = 1;
bool jumping = false;
unsigned long jumpStart = 0;
unsigned long frame = 0;
int jumpCount = 0;

int obstacleType = SPRITE_CACTUS1;  // current obstacle
int obstacleRow = 1;                

bool gameOver = false;

// Objects & Animation
byte graphics[][8] = {
  // SPRITE_JUMP_UPPER (head for jump)
  { B01110, B01110, B11110, B01100, B11110, B00100, B00000, B00100 },

  // SPRITE_RUN1 (legs position 1)
  { B01110, B01110, B11110, B01100, B11110, B01110, B00100, B01110 },

  // SPRITE_RUN2 (legs position 2)
  { B01110, B01110, B11110, B01100, B11110, B01110, B01110, B00100 },

  // SPRITE_JUMP1 (tucked legs)
  { B01110, B01110, B11110, B01100, B11110, B00100, B00000, B00100 },

  // SPRITE_JUMP2 (slightly different tuck)
  { B01110, B01110, B11110, B01100, B11110, B00100, B00000, B00000 },

  // SPRITE_CACTUS1
  { B00010, B00110, B00110, B00110, B00110, B01110, B01110, B01110 },

  // SPRITE_CACTUS2
  { B00100, B00100, B00100, B01110, B00100, B00100, B00100, B01110 },

  // SPRITE_BIRD
  {  B00100, B01100, B11100, B11110, B11100, B01100, B00100, B00000 }  
 };


void setup() {
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  pinMode(BUTTON_PIN, INPUT_PULLUP);


  // Load custom chars for graphics[][8]
  for (int i = 0; i < 8; i++) {
    lcd.createChar(i, graphics[i]);
  }

  pinMode(PIN_AUTOPLAY, OUTPUT);
  digitalWrite(PIN_AUTOPLAY, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Chrome Dino Game");
  lcd.setCursor(3, 1);
  lcd.print("by- Agnick");
  delay(1000);
  lcd.clear();
}

void loop() {
// --- Restart handling when game is over ---
    if (gameOver) {
        if (digitalRead(BUTTON_PIN) == LOW) {
            // Reset variables to start new game
            gameOver = false;
            frame = 0;
            jumping = false;
            dinoY = 1;
            jumpCount = 0; 

            lcd.clear();
        }
        return; // stop here until restart
    }

  lcd.clear();
  frame++;
  //Shows Score
  lcd.setCursor(14, 0);   // top-right corner (column 14, row 0)
  lcd.print(jumpCount);   // show number of jumps


  // Simple running animation
  int sprite = (frame % 2) ? SPRITE_RUN1 : SPRITE_RUN2;

  if (jumping) {
    if (millis() - jumpStart < 600) {
      sprite = SPRITE_JUMP1;
      dinoY = 0;
    } else {
      jumping = false;
      dinoY = 1;
    }
  }

  lcd.setCursor(0, dinoY);
  lcd.write(sprite);

  // Simple obstacle with random type
  int pos = 15 - (frame % 16);

  // When obstacle enters screen (right side), pick new type
  if (pos == 15) {
    int r = random(0, 4); // 0,1,2
    if (r == 0) obstacleType = SPRITE_CACTUS1;
    else if (r == 1) obstacleType = SPRITE_CACTUS2;
    else obstacleType = SPRITE_BIRD;

    // Decide row
    if (obstacleType == SPRITE_BIRD) {
    obstacleRow = random(0, 2); // row 0 or 1
    } else {
    obstacleRow = 1; // cactus on ground
    }
  }

  // Draw chosen obstacle

  //lcd.setCursor(pos, (obstacleType == SPRITE_BIRD) ? 0 : 1);  
  //lcd.write(obstacleType);
  lcd.setCursor(pos, obstacleRow);
  lcd.write(obstacleType);

  // ---- Collision check ----
  if (pos == 0 && dinoY == obstacleRow) {
    gameOver = true;
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("GAME OVER!");
    lcd.setCursor(0, 1);
    lcd.print("Press to Restart");
    delay(500);
}

  // Simple jump trigger
  // Jump when button is pressed
  if (!jumping && digitalRead(BUTTON_PIN) == LOW) {
    jumping = true;
    jumpStart = millis();
    jumpCount++;   // count every jump
}

  

  delay(150);
}
