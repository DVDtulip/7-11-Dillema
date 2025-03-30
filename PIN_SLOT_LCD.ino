#include "lcd_i2c.h"

lcd_i2c lcd(0x3E, 16, 2);
const int buttonPin = 7;
const int buzzerPin = 4;

bool running = true;
int spinCount = 0;
int clickCount = 0;
const char* results[3]; // Array to store string results
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 50;

void setup() {
  lcd.begin();
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as output
  randomSeed(analogRead(0));
  delay(2000);
  Greeting();
  clickCount++;
  if (clickCount == 2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Slot Machine");
    delay(2000);
    clickCount++;
  }
}

void loop() {
  if (digitalRead(buttonPin) == LOW && clickCount == 3) {
    delay(100); // Short delay
    lastButtonPress = millis();
    if (running) {
      spinCount++;
      spin();
    }
  }
}

void spin() {
  lcd.clear();
  const char* randomStrings[] = {":D", ";/", ":X"};
  for (int i = 0; i < 3; i++) {
    results[i] = randomStrings[random(0, 3)]; // Store string result
    lcd.setCursor(2 + i * 5, 1);
    lcd.print(results[i]);
    delay(300);
  }

  delay(300);

  if (spinCount >= 50) {
    running = false;
  }

  delay(100);
  checkWin();
}

void checkWin() {
  if (strcmp(results[0], results[1]) == 0 && strcmp(results[1], results[2]) == 0) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("You Win!");
    // Winning sound: 5 quick beeps
    //for (int i = 0; i < 5; i++) {
    //  tone(buzzerPin, 1000, 200);
    //  delay(300);
    //}
  } else {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Try Again");
    // Losing sound: 1 short beep
    //tone(buzzerPin, 500, 1000);
    //delay(300);
  }
  delay(2000);
}

void Greeting() {
  unsigned long lastButtonPress = 0;
  const unsigned long debounceDelay = 50;

  lcd.clear();

  for (int positionCounter = 0; positionCounter < 15; positionCounter++) {
    lcd.setCursor(0, 0);
    lcd.print("Welcome to ProfHack's Casino!");
    lcd.scrollDisplayLeft();
    delay(300);

    if (digitalRead(buttonPin) == LOW && (millis() - lastButtonPress) > debounceDelay) {
      lastButtonPress = millis();
      return;
    }
  }

  delay(300);

  if (digitalRead(buttonPin) == LOW && (millis() - lastButtonPress) > debounceDelay) {
    lastButtonPress = millis();
    return;
  }
  clickCount++;
}