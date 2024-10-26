#include <URTouch.h>
#include <URTouchCD.h>
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7789.h> 
#include <SPI.h>
#include <EEPROM.h>  // Include EEPROM library

// Display and touch screen pins
#define TFT_DC 9              
#define TFT_CS 10             
#define TFT_RST 8             
#define TFT_MISO 12           
#define TFT_MOSI 11           
#define TFT_SCLK 13 

#define t_SCK 3              
#define t_CS 4                
#define t_MOSI 5              
#define t_MISO 6             
#define t_IRQ 7  

// Create an instance of the ST7789 display
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Create an instance of the TouchScreen
URTouch ts(t_SCK, t_CS, t_MOSI, t_MISO, t_IRQ);

// Variables to store the counts
int dailyCount = 0;
int monthlyCount = 0;  // This will be stored in EEPROM

// Button coordinates and dimensions
int button1_x = 120; // New position for Button 1
int button1_y = 30;  // New position for Button 1
int button1_width = 100;  
int button1_height = 40;  

int button2_x = 120; // New position for Button 2
int button2_y = 80;  // New position for Button 2
int button2_width = 100;  
int button2_height = 40;  

int button3_x = 120; // New position for Button 3
int button3_y = 130; // New position for Button 3
int button3_width = 100;  
int button3_height = 40;  

// EEPROM address to store the monthly count
const int monthlyCountEEPROMAddress = 0;

void setup(void) {
  Serial.begin(9600);
  tft.init(240, 320); 
  tft.setRotation(2); // Rotate the display to the desired orientation

  ts.InitTouch();                   
  ts.setPrecision(PREC_EXTREME);
  tft.fillScreen(ST77XX_BLACK);

  // Title
  tft.setTextColor(ST77XX_RED);  
  tft.setTextSize(2);               
  tft.setCursor(10, 5);              
  tft.print("Counter App");

  // Load monthly count from EEPROM
  monthlyCount = EEPROM.read(monthlyCountEEPROMAddress);

  // Button 1 (Increment Daily Count)
  tft.fillRect(button1_x, button1_y, button1_width, button1_height, ST77XX_BLUE);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(button1_x + 5, button1_y + 10);
  tft.setTextSize(2);
  tft.print("Adicionar");

  // Button 2 (Reset Daily, Add to Monthly)
  tft.fillRect(button2_x, button2_y, button2_width, button2_height, ST77XX_GREEN);
  tft.setCursor(button2_x + 5, button2_y + 10);
  tft.setTextSize(2);
  tft.print("Reset diario");

  // Button 3 (Reset Monthly Count)
  tft.fillRect(button3_x, button3_y, button3_width, button3_height, ST77XX_RED);
  tft.setCursor(button3_x + 5, button3_y + 10);
  tft.setTextSize(2);
  tft.print("Reset Mes");

  // Initial counts display
  updateCounts();
}

void loop() {
  int x, y;
  
  // Check for touch input
  if (ts.dataAvailable()) {
    ts.read();
    x = ts.getX();
    y = ts.getY();
    
    if (x != -1 && y != -1) {
      // Check if button1 (Add) is pressed
      if (x > button1_x && x < button1_x + button1_width && y > button1_y && y < button1_y + button1_height) {
        dailyCount++; // Increment daily count
        updateCounts();
      }

      // Check if button2 (Reset Daily, Add to Monthly) is pressed
      if (x > button2_x && x < button2_x + button2_width && y > button2_y && y < button2_y + button2_height) {
        monthlyCount += dailyCount; // Add daily count to monthly total
        dailyCount = 0;             // Reset daily count
        EEPROM.write(monthlyCountEEPROMAddress, monthlyCount);  // Save monthly count to EEPROM
        updateCounts();
      }

      // Check if button3 (Reset Monthly) is pressed
      if (x > button3_x && x < button3_x + button3_width && y > button3_y && y < button3_y + button3_height) {
        monthlyCount = 0;           // Reset monthly count
        EEPROM.write(monthlyCountEEPROMAddress, monthlyCount);  // Clear EEPROM
        updateCounts();
      }
    }
  }
}

void updateCounts() {
  // Clear previous counts
  tft.fillRect(20, 80, 200, 60, ST77XX_BLACK);

  // Display daily count
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(20, 80);
  tft.print("Diario: ");
  tft.print(dailyCount);

  // Display monthly count
  tft.setCursor(20, 110);
  tft.print("Mes: ");
  tft.print(monthlyCount);
}
