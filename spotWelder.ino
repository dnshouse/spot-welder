#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_PIN 2
#define WELDER_PIN 3
#define TRIMMER_1_PIN A0
#define TRIMMER_2_PIN A1
#define TRIMMER_3_PIN A2

int buttonState, trimmer1Value, trimmer2Value, trimmer3Value;
int lastButtonState = 0;
int welderState = LOW;

long time, lastReadingsTime = 0;
long debounce = 200;

void setup() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.display();

  pinMode(WELDER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(TRIMMER_1_PIN, INPUT);
  pinMode(TRIMMER_2_PIN, INPUT);
  pinMode(TRIMMER_3_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
  
  readInputs();
  displaySettings();

  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH && lastButtonState == LOW && millis() - time > debounce) {
    weld();
    time = millis();    
  }

  lastButtonState = buttonState;
  
}

void readInputs() {
  buttonState = digitalRead(BUTTON_PIN);

  if(millis() - lastReadingsTime > debounce) {

    if(abs(trimmer1Value - analogRead(TRIMMER_1_PIN)) > 5) {
      trimmer1Value = analogRead(TRIMMER_1_PIN);
    }

    if(abs(trimmer2Value - analogRead(TRIMMER_2_PIN)) > 5) {
      trimmer2Value = analogRead(TRIMMER_2_PIN);
    }

    if(abs(trimmer3Value - analogRead(TRIMMER_3_PIN)) > 5) {
      trimmer3Value = analogRead(TRIMMER_3_PIN);
    }

    lastReadingsTime = millis();
    
  }
}

void displaySettings() {
  
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0,0);
  display.print("P1-");
  display.print(trimmer1Value);

  display.setCursor(50,0);
  display.print("W-");
  display.print(trimmer2Value);

  display.setCursor(0,20);
  display.print("P2-");
  display.print(trimmer3Value);

  display.display();
  
}

void displayWelding() {
  
  display.clearDisplay();
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  
  display.setCursor(0,0);
  display.print("Welding");
  
  display.display();
  
}

void weld() {
  
  displayWelding();
  
  welderState = HIGH;
  digitalWrite(WELDER_PIN, welderState);
  delay(trimmer1Value);

  welderState = LOW;
  digitalWrite(WELDER_PIN, welderState);
  delay(trimmer2Value);

  welderState = HIGH;
  digitalWrite(WELDER_PIN, welderState);
  delay(trimmer3Value);

  welderState = LOW;
  digitalWrite(WELDER_PIN, welderState);
  
}
