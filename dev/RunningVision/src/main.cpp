#include <M5Unified.h>
#include <M5GFX.h>
#include <M5UnitGLASS2.h>

HardwareSerial GPSRaw(2);
//M5UnitGLASS2 display(26,32,40000); // SDA, SCL, FREQ(ATOM ECHO)
M5UnitGLASS2 display(2,1,40000); // SDA, SCL, FREQ(ATOM ECHO)

M5Canvas canvas(&display);

static constexpr char text0[] = "hello user";
static constexpr char text1[] = "this";
static constexpr char text2[] = "is";
static constexpr char text3[] = "RunningVision";
static constexpr char text4[] = "support";
static constexpr char text5[] = "Your";
static constexpr char text6[] = "Runnning";
static constexpr char text7[] = "ver 0.5";
static constexpr const char* text[] = { text0, text1, text2, text3, text4, text5, text6, text7 };


void setup() {
    M5.begin();
    M5.Power.begin();
    Serial.begin(9600);
    //GPSRaw.begin(9600,SERIAL_8N1,21,25);//baudrate, config,rx,tx(ATOM ECHO)
    GPSRaw.begin(9600,SERIAL_8N1,39,38);//baudrate, config,rx,tx(ATOM S3)

    display.begin();

    canvas.setColorDepth(1); // mono color
    canvas.setRotation(180);
    canvas.createSprite(display.width(), display.height());
    canvas.setTextSize((float)canvas.width() / 120);
    canvas.setTextScroll(true);
}

void loop() {
    if (GPSRaw.available()){
        int ch = GPSRaw.read();
        Serial.write(ch);
        canvas.print(ch);
    }

    static int count = 0;
    //canvas.printf("%04d:%s\r\n", count, text[count & 7]);
    canvas.pushSprite(0, 0);
    ++count;

    delay(100);
}