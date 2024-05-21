#include <M5Unified.h>
#include <M5GFX.h>
#include <M5UnitGLASS2.h>
#include <U8g2lib.h>
#include <ScreenBitMap.h>
#include <Kalman.h>
#include <TinyGPSPlus.h>

/* Glass2ユニット設定 */

//M5UnitGLASS2 display(26,32,40000); // SDA, SCL, FREQ(ATOM ECHO)
M5UnitGLASS2 display(2,1,40000); // SDA, SCL, FREQ(ATOM S3)
//M5Canvas canvas(&display);
U8G2_SSD1309_128X64_NONAME0_F_HW_I2C u8g2(U8G2_R2, /* clock=*/ 1, /* data=*/ 2, /* reset=*/ U8X8_PIN_NONE); //* clock=*/ SCL, /* data=*/ SDA

/* GPSユニット設定*/
HardwareSerial GPSRaw(2);
static const uint32_t gpsBaud = 9600;
TinyGPSPlus gps;


/* ボタン設定 */
const int buttonPin = 41;
const int buttonDownTime = 100; //自動ボタンたち下がり時間(ms)
const int longPressDuration = 1000; //ボタン長押し判定時間(ms)

/* 画面設定 */
const int numItems = 3;
const int maxItemLength = 20;

char menuItems [numItems][maxItemLength] = {
    { "RunningVision" },
    { "DebugMode" },
    { "Settings" }
};


/* プロトタイプ設定 */

void startUpScreen();
void mainMenuScreen();
void readGyro();
float getPitch();
void calculatePitch();
void debugScreen();
void showControlBar();
void updateControl();
void updateButtonState();
void runningVisionTitleScreen();
void runningVisionRunningScreen();
void runningVisionPauseScreen();
static void smartDelay(unsigned long ms);

/* 状態変数*/
enum ScreenState {
    TITLE,
    MAIN_MENU,
    RUN_VISION_START,
    RUN_VISION_RUNNING,
    RUN_VISION_PAUSE,
    DEBUG
};

enum ControlState {
    UP,
    DOWN,
    CENTER
};

enum ButtonState {
    NOT_PRESSED,
    PRESSED,
    LONG_PRESSED,
};

ScreenState currentScreen = TITLE; //現在の画面状態
ControlState currentControl = CENTER; //現在の頭方向状態
ButtonState currentButtonState = NOT_PRESSED; //現在のボタン押下状態

/* 加速度センサー変数*/
float acc[3];
float gyro[3];
float kalAnglePitch;
Kalman kalmanPitch;
unsigned long lastMs = ESP.getCycleCount();



void setup() {
    M5.begin();
    M5.Power.begin();
    Serial.begin(9600);

    //IMU設定
    M5.Imu.init();
    readGyro();
    kalmanPitch.setAngle(getPitch());

    //ボタン設定
    pinMode(buttonPin, INPUT);

    //GPS Unit 設定
    //GPSRaw.begin(9600,SERIAL_8N1,21,25);//baudrate, config,rx,tx(ATOM ECHO)
    GPSRaw.begin(gpsBaud,SERIAL_8N1,39,38);//baudrate, config,rx,tx(ATOM S3)
    
    //u8g2設定
    u8g2.begin(); // start the u8g2 library
    display.begin();
}

void loop() {
    //unsigned long currentMicros = ESP.getCycleCount();
    //Serial.println(currentMicros); // シリアル出力でmicrosの動作確認
    //Serial.println("loop now!");


    switch (currentScreen) {
        case TITLE:
            u8g2.firstPage();
            startUpScreen();
            currentScreen = MAIN_MENU;
            u8g2.nextPage();
            break;
        case DEBUG:
            debugScreen();
            if(currentButtonState == PRESSED){
                currentScreen = MAIN_MENU;
                u8g2.nextPage();
            }
            break;
        case MAIN_MENU:
            mainMenuScreen();
            break;
        case RUN_VISION_START:
            runningVisionTitleScreen();
            if(currentButtonState == PRESSED){
                currentScreen = MAIN_MENU;
                u8g2.nextPage();
            }
            if(currentButtonState == LONG_PRESSED){
                currentScreen = RUN_VISION_RUNNING;
                u8g2.nextPage();
            }
            break;
        case RUN_VISION_RUNNING:
            runningVisionRunningScreen();
            if(currentButtonState == PRESSED){
                currentScreen = RUN_VISION_PAUSE;
                u8g2.nextPage();
            }
            break;
        case RUN_VISION_PAUSE:
            runningVisionPauseScreen();
            if(currentButtonState == PRESSED){
                currentScreen = RUN_VISION_RUNNING;
                u8g2.nextPage();
            }
            if(currentButtonState == LONG_PRESSED){
                currentScreen = MAIN_MENU;
                u8g2.nextPage();
            }
        
    }

    updateButtonState();
    calculatePitch();
    updateControl();
    showControlBar();
    

    u8g2.sendBuffer();

    //if (GPSRaw.available()){
    //    int ch = GPSRaw.read();
    //    Serial.write(ch);
    //    //canvas.print(ch);
    //}

    Serial.println(currentButtonState);

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

void debugScreen(){
    char buffer[20];
    u8g2.clearBuffer();		// clear the internal memory
    u8g2.setFont(u8g2_font_tenthinguys_tf); // set custom font
    dtostrf(kalAnglePitch,6,2,buffer);
    u8g2.drawStr(0,20,buffer);
    dtostrf(currentButtonState,6,2,buffer);
    u8g2.drawStr(0,40,buffer);
}

void runningVisionTitleScreen(){
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_luBIS08_tr);
    u8g2.drawStr(20,20,"Running Vision"); // print some message
    u8g2.drawStr(40,35,"Ver 0.5");
    u8g2.setFont(u8g2_font_squeezed_r6_tr);
    u8g2.drawStr(30,50,"Hold down the button");
    u8g2.drawStr(42,60,"Start running");
}

void runningVisionRunningScreen(){
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_luBIS08_tr);
    u8g2.drawStr(40,20,"Dis: 7.78km");
    u8g2.drawStr(40,30,"Pace: 5'54/km");
    u8g2.drawStr(40,40,"Step: 5500");
}

void runningVisionPauseScreen(){
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_tenthinguys_tf);
    u8g2.drawStr(45,30,"Pause"); // print some message
    u8g2.setFont(u8g2_font_squeezed_r6_tr);
    u8g2.drawStr(30,50,"Hold down the button");
    u8g2.drawStr(35,60,"back to main menu");
}


void mainMenuScreen(){
    u8g2.clearBuffer();	
    static int itemSelected = 0;
    static unsigned long selectedTime =0;
    unsigned long currentTime = millis();
    int itemSelPrevious;
    int itemSelNext;

    //背景諸々描写
    u8g2.drawBitmap(0,20,128/8,24,epd_bitmap_background);//選択枠

    //選択アイテム更新
    if(currentControl == UP && currentButtonState == PRESSED){
        itemSelected = itemSelected -1;
        if(itemSelected < 0){
            if(currentTime - selectedTime > 500){
                itemSelected = numItems -1;
                selectedTime = currentTime;
            }
        }
    }
    if(currentControl == DOWN && currentButtonState == PRESSED){
        itemSelected = itemSelected +1;
        if(itemSelected >= numItems){
            if(currentTime - selectedTime > 500){
                itemSelected = 0;
                selectedTime = currentTime;
            }
        }
    }
    if(currentControl == CENTER && currentButtonState == PRESSED){
        if(currentTime - selectedTime > 500){
            if(itemSelected == 0){
                currentScreen = RUN_VISION_START;
            }
            if(itemSelected == 1){
                currentScreen = DEBUG;
            }
            selectedTime = currentTime;
        }
    }

    itemSelPrevious = itemSelected -1;
    if(itemSelPrevious < 0){
        itemSelPrevious = numItems -1;
    }
    itemSelNext = itemSelected +1;
    if(itemSelNext >= numItems){
        itemSelNext = 0;
    }

    //メニューアイコン&メニュータイトル描画
    u8g2.setFont(u8g_font_7x14);
    u8g2.drawStr(25,15, menuItems[itemSelPrevious]);
    u8g2.drawBitmap(4,2, 16/8, 16, epd_bitmap_icons[itemSelPrevious]);

    u8g2.setFont(u8g_font_7x14B);
    u8g2.drawStr(25,15+20+2, menuItems[itemSelected]);
    u8g2.drawBitmap(4,24, 16/8, 16, epd_bitmap_icons[itemSelected]);

    u8g2.setFont(u8g_font_7x14);
    u8g2.drawStr(25,15+20+20+2+2, menuItems[itemSelNext]);
    u8g2.drawBitmap(4,46, 16/8, 16, epd_bitmap_icons[itemSelNext]);

}

void readGyro(){
    M5.Imu.getGyroData(&gyro[0], &gyro[1], &gyro[2]);
    M5.Imu.getAccelData(&acc[0], &acc[1], &acc[2]);  
}


float getPitch(){
    return atan2(-acc[1], acc[0]) * RAD_TO_DEG;
}


void calculatePitch(){
    readGyro();
    unsigned long currentMicros = ESP.getCycleCount();
    float dt = (currentMicros - lastMs) / 1000000.0f;
    lastMs = currentMicros;
    float pitch = getPitch();
    kalAnglePitch = kalmanPitch.getAngle(pitch, gyro[2], dt);
}

void updateControl(){
    if( kalAnglePitch >= 30){
        currentControl = UP;
    }
    if(kalAnglePitch > -30 && kalAnglePitch < 30){
        currentControl = CENTER;
    }
    if( kalAnglePitch <= -30){
        currentControl = DOWN;
    }
}

void showControlBar(){

    if(currentControl == UP){
        u8g2.drawBitmap(110,49,16/8,14,epd_bitmap_cross_button_up);
    }
    if(currentControl == CENTER){
        u8g2.drawBitmap(110,49,16/8,14,epd_bitmap_cross_button_center);
    }
    if(currentControl == DOWN){
        u8g2.drawBitmap(110,49,16/8,14,epd_bitmap_cross_button_down);
    }

}

void updateButtonState(){
    static int lastButtonRead = HIGH;
    static unsigned long buttonReleasedTime = 0;
    static unsigned long buttonPressedTime = 0;
    int readButton = digitalRead(buttonPin);
    unsigned long currentTime = millis();

    //ボタンの状態監視
    if(readButton != lastButtonRead){
        if(readButton == LOW){
            buttonPressedTime = currentTime;
            Serial.println("ButtonPressed!!");
        }
        if(readButton == HIGH){
            currentButtonState = PRESSED;
            buttonReleasedTime = currentTime;
            Serial.println("ButtonReleased!!");
        }
    }

    //ボタンの長押し判定
    if(((buttonReleasedTime - buttonPressedTime) > longPressDuration) && ((buttonReleasedTime - buttonPressedTime) > 0)){
        currentButtonState = LONG_PRESSED;
    }

    //ボタン押下後の初期化
    if ((currentButtonState == PRESSED || currentButtonState == LONG_PRESSED) && (currentTime - buttonReleasedTime > buttonDownTime)){
        currentButtonState = NOT_PRESSED;
    }
    
    lastButtonRead = readButton;

}