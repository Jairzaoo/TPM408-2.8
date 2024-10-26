#include <URTouch.h>
#include <URTouchCD.h>
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7789.h> 
#include <SPI.h>
#include <EEPROM.h> 


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


Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


URTouch ts(t_SCK, t_CS, t_MOSI, t_MISO, t_IRQ);


int contDiario = 0;
int contMes = 0;  


int button1_x = 20;
int button1_y = 150;
int button1_width = 120;  
int button1_height = 50;  

int button2_x = 140;
int button2_y = 150;
int button2_width = 120;  
int button2_height = 50;  

int button3_x = 80;  
int button3_y = 220;
int button3_width = 120;
int button3_height = 50;


const int contMesEEPROM = 0;

void setup(void) {
  Serial.begin(9600);
  tft.init(240, 320); 
  tft.setRotation(2);  

  ts.InitTouch();                   
  ts.setPrecision(PREC_EXTREME);
  tft.fillScreen(ST77XX_BLACK);

  
  tft.setTextColor(ST77XX_RED);  
  tft.setTextSize(2);               
  tft.setCursor(60, 5);              
  tft.print("Contador");

  
  contMes = EEPROM.read(contMesEEPROM);

  
  tft.fillRect(button1_x, button1_y, button1_width, button1_height, ST77XX_BLUE);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(button1_x + 10, button1_y + 15);
  tft.setTextSize(2);
  tft.print("Adicionar");

  
  tft.fillRect(button2_x, button2_y, button2_width, button2_height, ST77XX_GREEN);
  tft.setCursor(button2_x + 10, button2_y + 15);
  tft.setTextSize(2);
  tft.print("Reset diario");

  
  tft.fillRect(button3_x, button3_y, button3_width, button3_height, ST77XX_RED);
  tft.setCursor(button3_x + 10, button3_y + 15);
  tft.setTextSize(2);
  tft.print("Reset Mes");

  
  updateCounts();
}

void loop() {
  int x, y;
  
  
  if (ts.dataAvailable()) {
    ts.read();
    x = ts.getX();
    y = ts.getY();
    
  
    int displayX = map(y, 0, tft.height(), tft.height(), 0);
    int displayY = map(x, 0, tft.width(), tft.width(), 0);
    
    if (displayX != -1 && displayY != -1) {
      
      if (displayX > button1_x && displayX < button1_x + button1_width && displayY > button1_y && displayY < button1_y + button1_height) {
        contDiario++;
        updateCounts();
      }

     
      if (displayX > button2_x && displayX < button2_x + button2_width && displayY > button2_y && displayY < button2_y + button2_height) {
        contMes += contDiario; 
        contDiario = 0;            
        EEPROM.write(contMesEEPROM, contMes); 
        updateCounts();
      }

      
      if (displayX > button3_x && displayX < button3_x + button3_width && displayY > button3_y && displayY < button3_y + button3_height) {
        contMes = 0;         
        EEPROM.write(contMesEEPROM, contMes); 
        updateCounts();
      }
    }
  }
}

void updateCounts() {
  
  tft.fillRect(20, 80, 200, 60, ST77XX_BLACK);

  
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(20, 80);
  tft.print("Diario: ");
  tft.print(contDiario);

  t
  tft.setCursor(20, 110);
  tft.print("Mes: ");
  tft.print(contMes);
}