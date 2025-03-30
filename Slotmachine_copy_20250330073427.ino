#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdlib.h>
#include <time.h>
#include <Wire.h>
#include <math.h> // For sin() function

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define the simple text symbols for the slot machine reels
const char* symbols[] = {"$", "7", "*", "@", "#", "&", "+", "!"}; // Dollar, Seven, Asterisk, At, Hash, Ampersand, Plus, Exclamation
const int numSymbols = sizeof(symbols) / sizeof(symbols[0]);
const int buzzerPin = 5; // Pin for the buzzer
int buttonState = 0;

const int buttonPin = 2;
bool gameStarted = false;

int spinReel() {
  return random(numSymbols);
}

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  randomSeed(time(NULL));
  pinMode(buttonPin, INPUT_PULLUP);

  // Display the start menu
  display.setTextSize(2);
  display.setCursor(20, 20);
  display.println("SLOT");
  display.setCursor(30, 40);
  display.println("MACHINE");
  display.setTextSize(1);
  display.setCursor(10, 55);
  display.println("Press Button to Start");

  // Draw a slightly larger sketch of an owl in the top right corner
  int startX_owl = SCREEN_WIDTH - 26; // Adjusted X for owl
  int startY_owl = 6;              // Adjusted Y for owl

  // Head Outline
  display.drawLine(startX_owl + 3, startY_owl, startX_owl + 6, startY_owl, SSD1306_WHITE);
  display.drawLine(startX_owl + 2, startY_owl + 1, startX_owl + 7, startY_owl + 1, SSD1306_WHITE);
  display.drawLine(startX_owl + 1, startY_owl + 2, startX_owl + 8, startY_owl + 2, SSD1306_WHITE);
  display.drawLine(startX_owl + 1, startY_owl + 3, startX_owl + 8, startY_owl + 3, SSD1306_WHITE);
  display.drawLine(startX_owl + 2, startY_owl + 4, startX_owl + 7, startY_owl + 4, SSD1306_WHITE);
  display.drawLine(startX_owl + 3, startY_owl + 5, startX_owl + 6, startY_owl + 5, SSD1306_WHITE);

  // Ear Tufts
  display.drawLine(startX_owl + 2, startY_owl - 1, startX_owl + 3, startY_owl + 1, SSD1306_WHITE);
  display.drawLine(startX_owl + 6, startY_owl - 1, startX_owl + 7, startY_owl + 1, SSD1306_WHITE);

  // Eyes (larger dots, spaced further)
  display.drawPixel(startX_owl + 3, startY_owl + 2, SSD1306_WHITE);
  display.drawPixel(startX_owl + 6, startY_owl + 2, SSD1306_WHITE);
  display.drawPixel(startX_owl + 4, startY_owl + 3, SSD1306_WHITE);
  display.drawPixel(startX_owl + 5, startY_owl + 3, SSD1306_WHITE);

  // Beak (more defined 'V', slightly longer)
  display.drawLine(startX_owl + 4, startY_owl + 4, startX_owl + 5, startY_owl + 4, SSD1306_WHITE);
  display.drawLine(startX_owl + 3, startY_owl + 5, startX_owl + 6, startY_owl + 5, SSD1306_WHITE);

  // Body (more pronounced)
  display.drawLine(startX_owl + 4, startY_owl + 6, startX_owl + 5, startY_owl + 7, SSD1306_WHITE);
  display.drawLine(startX_owl + 3, startY_owl + 7, startX_owl + 6, startY_owl + 8, SSD1306_WHITE);

  display.display();
}

void loop() {
  if (!gameStarted) {
    // Fireworks effect on the start screen
    unsigned long currentTime = millis();
    if ((currentTime / 200) % 2 == 0) { // Toggle every 200ms
      display.fillRect(SCREEN_WIDTH - 20, 10, 3, 3, SSD1306_WHITE);
      display.fillRect(SCREEN_WIDTH - 30, 5, 2, 2, SSD1306_WHITE);
      display.fillRect(SCREEN_WIDTH - 15, 15, 4, 4, SSD1306_WHITE);
    } else {
      display.fillRect(SCREEN_WIDTH - 20, 10, 3, 3, SSD1306_BLACK);
      display.fillRect(SCREEN_WIDTH - 30, 5, 2, 2, SSD1306_BLACK);
      display.fillRect(SCREEN_WIDTH - 15, 15, 4, 4, SSD1306_BLACK);
    }
    display.display();

    // Wait for the button press to start the game
    if (digitalRead(buttonPin) == LOW) {
      delay(200); // Debounce
      while (digitalRead(buttonPin) == LOW); // Wait for release
      gameStarted = true;
      display.clearDisplay();
    }
    delay(10); // Small delay to avoid rapid checking
    return; // Go back to the beginning of the loop and wait
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("SLOTS");

  int reel1, reel2, reel3;
  bool buttonPressed = false;
  bool delayActive = false;
  int boxYOffset = 28;
  int symbolYOffset = 26;
  int boxHeight = 20;
  int boxWidth = 30;
  int xStart1 = 15;
  int xStart2 = 55;
  int xStart3 = 95;

  // Draw the boxes
  display.drawRect(xStart1, boxYOffset - (boxHeight / 2) - 2, boxWidth, boxHeight + 4, SSD1306_WHITE);
  display.drawRect(xStart2, boxYOffset - (boxHeight / 2) - 2, boxWidth, boxHeight + 4, SSD1306_WHITE);
  display.drawRect(xStart3, boxYOffset - (boxHeight / 2) - 2, boxWidth, boxHeight + 4, SSD1306_WHITE);

  while (!buttonPressed) {
    reel1 = spinReel();
    reel2 = spinReel();
    reel3 = spinReel();

    display.setTextSize(2);
    display.setCursor(xStart1 + (boxWidth - 12) / 2, symbolYOffset);
    display.print(symbols[reel1]);
    display.setCursor(xStart2 + (boxWidth - 12) / 2, symbolYOffset);
    display.print(symbols[reel2]);
    display.setCursor(xStart3 + (boxWidth - 12) / 2, symbolYOffset);
    display.print(symbols[reel3]);

    // Draw the "dancing dots"
    int lineY = SCREEN_HEIGHT - 5; // Position near the bottom
    for (int x = 0; x < SCREEN_WIDTH; x += 5) { // Increment x to create spacing between dots
      int yOffset = 2 * sin(x * 0.1 + millis() * 0.01); // Adjust amplitude and speed
      display.drawPixel(x, lineY + yOffset, SSD1306_WHITE);
    }

    display.display();
    delay(100);

    if (digitalRead(buttonPin) == LOW && !delayActive) {
      buttonPressed = true;
      delay(200);
      while (digitalRead(buttonPin) == LOW);
      delayActive = true; // Set the delay flag
    }
    display.clearDisplay(); // Keep this clear for the next frame
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("SLOTS");
    // Redraw the boxes during spin
    display.drawRect(xStart1, boxYOffset - (boxHeight / 2) - 2, boxWidth, boxHeight + 4, SSD1306_WHITE);
    display.drawRect(xStart2, boxYOffset - (boxHeight / 2) - 2, boxWidth, boxHeight + 4, SSD1306_WHITE);
    display.drawRect(xStart3, boxYOffset - (boxHeight / 2) - 2, boxWidth, boxHeight + 4, SSD1306_WHITE);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(xStart1 + (boxWidth - 12) / 2, symbolYOffset);
  display.print(symbols[reel1]);
  display.setCursor(xStart2 + (boxWidth - 12) / 2, symbolYOffset);
  display.print(symbols[reel2]);
  display.setCursor(xStart3 + (boxWidth - 12) / 2, symbolYOffset);
  display.print(symbols[reel3]);

  // Draw the "dancing dots" after the spin stops as well
  int lineY = SCREEN_HEIGHT - 5; // Position near the bottom
  for (int x = 0; x < SCREEN_WIDTH; x += 5) { // Increment x for dots
    int yOffset = 2 * sin(x * 0.1 + millis() * 0.01); // Adjust amplitude and speed
    display.drawPixel(x, lineY + yOffset, SSD1306_WHITE);
  }

  if (reel1 == reel2 && reel2 == reel3) {
    display.setTextSize(2);
    display.setCursor(0, 50);
    display.println("**WIN!**");
    // Winning sound: 5 quick beeps
    for (int i = 0; i < 5; i++) {
      tone(buzzerPin, 1000, 200); // Play tone at 1000 Hz for 200ms
      delay(300); // Short delay between beeps
    }
  }

  display.display();
  delay(2000); // Display result for 2 seconds (adjust as needed)
  // Losing sound: 1 short beep
  tone(buzzerPin, 500, 1000); // Play tone at 500 Hz for 1000ms
  delay(300);

  // Implement the 5-second countdown after the result
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Next spin in...");
  display.setTextSize(3);
  for (int i = 5; i > 0; i--) {
    display.setCursor(40, 25);
    display.print(i);
    display.display();
    delay(1000);
    if (i > 1) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Next spin in...");
    }
  }
  delayActive = false; // Reset the delay flag to allow another spin
}