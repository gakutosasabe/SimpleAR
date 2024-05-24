#include <M5Unified.h>
#include <M5GFX.h>
#include <M5UnitGLASS2.h>
#include <TinyGPSPlus.h>

HardwareSerial GPSRaw(2);
//M5UnitGLASS2 display(26,32,40000); // SDA, SCL, FREQ(ATOM ECHO)
//M5UnitGLASS2 display(2,1,40000); // SDA, SCL, FREQ(ATOM ECHO)

//M5Canvas canvas(&display);
TinyGPSPlus gps;

static constexpr char text0[] = "hello user";
static constexpr char text1[] = "this";
static constexpr char text2[] = "is";
static constexpr char text3[] = "RunningVision";
static constexpr char text4[] = "support";
static constexpr char text5[] = "Your";
static constexpr char text6[] = "Runnning";
static constexpr char text7[] = "ver 0.5";
static constexpr const char* text[] = { text0, text1, text2, text3, text4, text5, text6, text7 };

static void smartDelay(unsigned long ms){
    unsigned long start = millis();
    do {
        while (GPSRaw.available()) gps.encode(GPSRaw.read()); // シリアルポートからデータが読み込まれ、GPSオブジェクトにエンコードされる間、繰り返し処理を実行
    }
    while (millis() - start < ms);
}

void setup() {
    M5.begin();
    M5.Power.begin();
    Serial.begin(9600);
    //GPSRaw.begin(9600,SERIAL_8N1,21,25);//baudrate, config,rx,tx(ATOM ECHO)
    GPSRaw.begin(9600,SERIAL_8N1,2,1);//baudrate, config,rx,tx(ATOM S3)
    //GPSRaw.begin(9600);

}

void loop() {
    // if (GPSRaw.available()){
    //     int ch = GPSRaw.read();
        
    //     Serial.println((char)ch);
    //     canvas.print((char)ch);
    //     gps.encode(ch);
    // }

    M5.Lcd.setCursor(0, 0);           // 表示座標指定
    M5.Lcd.setTextFont(1);            // 文字サイズ指定
    M5.Lcd.setTextColor(CYAN, BLACK); // 文字色, 背景色
    M5.Lcd.drawCentreString("< TinyGPSPlus TEST >\n", 20, 0, 1); // テキスト中央表示(文字列, x座標, y座標, フォント番号)
    M5.Lcd.setCursor(0, 26);          // 表示座標指定
    M5.Lcd.setTextColor(WHITE, BLACK);                            // 文字色, 背景色
    M5.Lcd.printf("Satelites : %02d\n", gps.satellites.value());  // 衛星の数を表示する
    M5.Lcd.printf("HDOP      : %d \n", gps.hdop.value());         // HDOP値（水平方向の測定の正確さ）を表示する
    M5.Lcd.setTextColor(RED, BLACK);                              // 文字色, 背景色
    M5.Lcd.printf("N : %f  \n", gps.location.lat());              // 緯度を表示する
    M5.Lcd.setTextColor(GREEN, BLACK);                            // 文字色, 背景色
    M5.Lcd.printf("E : %f  \n", gps.location.lng());              // 経度を表示する
    M5.Lcd.setTextColor(WHITE, BLACK);                            // 文字色, 背景色
    M5.Lcd.printf("Location : %.3f  \n", gps.location.age());     // GPS位置情報の更新からの経過時間を表示する
    M5.Lcd.printf("Altitude   : %.1fm  \n", gps.altitude.meters()); // 高度をメートル単位で表示する
    M5.Lcd.printf("Course    : %.1f'  \n", gps.course.deg());       // 方位角を度数法で表示する
    M5.Lcd.printf("Speed     : %.1fkm  \n", gps.speed.kmph());      // 速度をキロメートル/時単位で表示する
    M5.Lcd.printf("Receivewords  : %1f  \n", gps.charsProcessed());      // 速度をキロメートル/時単位で表示する

    if (millis() > 5000 && gps.charsProcessed() < 10)
      M5.Lcd.println(F("No GPS data received: check wiring"));



    static int count = 0;
    //canvas.printf("%04d:%s\r\n", count, text[count & 7]);
    //canvas.pushSprite(0, 0);
    //++count;

    smartDelay(100);
}