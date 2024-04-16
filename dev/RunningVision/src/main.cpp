#include <M5Unified.h>
#include <M5GFX.h>
#include <M5UnitGLASS2.h>
#include <U8g2lib.h>
#include <ScreenBitMap.h>
#include <Kalman.h>

/* ハードウェア設定 */

HardwareSerial GPSRaw(2);
//M5UnitGLASS2 display(26,32,40000); // SDA, SCL, FREQ(ATOM ECHO)
M5UnitGLASS2 display(2,1,40000); // SDA, SCL, FREQ(ATOM S3)
M5Canvas canvas(&display);
U8G2_SSD1309_128X64_NONAME0_F_HW_I2C u8g2(U8G2_R2, /* clock=*/ 1, /* data=*/ 2, /* reset=*/ U8X8_PIN_NONE); //* clock=*/ SCL, /* data=*/ SDA


/* プロトタイプ設定 */

void startUpScreen();
void mainMenuScreen();
void readGyro();
float getRoll();
float getPitch();
void calculateXY();
void testScreen();

/* 画面系変数*/
enum ScreenState {
    TITLE,
    MAIN_MENU,
    RUN_VISION_START,
    TEST
};

ScreenState currentScreen = TITLE;

/* 加速度センサー変数*/
float acc[3];
float gyro[3];
float kalAngleX;
float kalAngleY;
Kalman kalmanX;
Kalman kalmanY;
long lastMs = 0;



void setup() {
    M5.begin();
    M5.Power.begin();
    Serial.begin(9600);

    //IMU設定
    M5.Imu.init();
    readGyro();
    kalmanX.setAngle(getRoll());
    kalmanY.setAngle(getPitch());

    //GPS Unit 設定
    //GPSRaw.begin(9600,SERIAL_8N1,21,25);//baudrate, config,rx,tx(ATOM ECHO)
    GPSRaw.begin(9600,SERIAL_8N1,39,38);//baudrate, config,rx,tx(ATOM S3)
    
    //u8g2設定
    u8g2.begin(); // start the u8g2 library
    display.begin();
}

void loop() {
    switch (currentScreen) {
        case TITLE:
            startUpScreen();
            currentScreen = TEST;
            break;
        case TEST:
            calculateXY();
            testScreen();
            break;
        case MAIN_MENU:
            mainMenuScreen();
            break;
        case RUN_VISION_START:
            break;
    }


    if (GPSRaw.available()){
        int ch = GPSRaw.read();
        Serial.write(ch);
        //canvas.print(ch);
    }

    delay(100);
}

void startUpScreen(){
    u8g2.clearBuffer();		// clear the internal memory
    u8g2.setFont(u8g2_font_tenthinguys_tf); // set custom font
    u8g2.drawStr(25,30,"M5 Scouter"); // print some message
    u8g2.drawStr(40,45,"Ver 1.01"); // print some message
    u8g2.sendBuffer();		// transfer internal memory to the display
    delay(2000);
    u8g2.clearBuffer(); 
}

void testScreen(){
    char buffer[20];
    u8g2.clearBuffer();		// clear the internal memory
    u8g2.setFont(u8g2_font_tenthinguys_tf); // set custom font
    dtostrf(kalAngleX,6,2,buffer);
    u8g2.drawStr(0,20,buffer);
    dtostrf(kalAngleY,6,2,buffer);
    u8g2.drawStr(0,40,buffer);
    u8g2.sendBuffer();
}

void mainMenuScreen(){
	u8g2.drawBitmap(0,0,128/8,64,epd_bitmap_Main_Menu);
	u8g2.drawBitmap(110,49,16/8,14,epd_bitmap_cross_button_center);
	u8g2.sendBuffer();
}

void readGyro(){
    M5.Imu.getGyroData(&gyro[0], &gyro[1], &gyro[2]);
    M5.Imu.getAccelData(&acc[0], &acc[1], &acc[2]);  
}

float getRoll(){
    return atan2(acc[1], acc[2]) * RAD_TO_DEG;
}

float getPitch(){
    return atan(-acc[0] / sqrt(acc[1]*acc[1] + acc[2]*acc[2])) * RAD_TO_DEG;
}

void calculateXY(){
    readGyro();
    float dt = (micros() - lastMs) / 1000000.0;
    lastMs = micros();
    float roll = getRoll();
    float pitch = getPitch();
    kalAngleX = kalmanX.getAngle(roll, gyro[0], dt);
    kalAngleY = kalmanY.getAngle(pitch, gyro[1], dt);
}