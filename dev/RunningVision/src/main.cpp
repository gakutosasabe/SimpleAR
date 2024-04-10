#include <M5Unified.h>
#include <M5GFX.h>
#include <M5UnitGLASS2.h>
#include <U8g2lib.h>

HardwareSerial GPSRaw(2);
//M5UnitGLASS2 display(26,32,40000); // SDA, SCL, FREQ(ATOM ECHO)
M5UnitGLASS2 display(2,1,40000); // SDA, SCL, FREQ(ATOM S3)
M5Canvas canvas(&display);
U8G2_SSD1309_128X64_NONAME0_F_HW_I2C u8g2(U8G2_R2, /* clock=*/ 1, /* data=*/ 2, /* reset=*/ U8X8_PIN_NONE); //* clock=*/ SCL, /* data=*/ SDA

void displayStartUp();

void setup() {
    M5.begin();
    M5.Power.begin();
    Serial.begin(9600);
    //GPS Unit 設定
    //GPSRaw.begin(9600,SERIAL_8N1,21,25);//baudrate, config,rx,tx(ATOM ECHO)
    GPSRaw.begin(9600,SERIAL_8N1,39,38);//baudrate, config,rx,tx(ATOM S3)
    
    //u8g2設定
    u8g2.begin(); // start the u8g2 library
    display.begin();
    canvas.setColorDepth(1); // mono color
    canvas.createSprite(display.width(), display.height());
    canvas.setTextSize((float)canvas.width() / 120);
    canvas.setTextScroll(true);
}

void loop() {
    if (GPSRaw.available()){
        int ch = GPSRaw.read();
        Serial.write(ch);
        //canvas.print(ch);
    }

    //canvas.pushSprite(0, 0);
    displayStartUp();

    delay(100);
}

void displayStartUp(){
    u8g2.clearBuffer();		// clear the internal memory
    u8g2.setFont(u8g2_font_tenthinguys_tf); // set custom font
    u8g2.drawStr(25,30,"M5 Scouter"); // print some message
    u8g2.drawStr(40,45,"Ver 1.01"); // print some message
    u8g2.sendBuffer();		// transfer internal memory to the display
}