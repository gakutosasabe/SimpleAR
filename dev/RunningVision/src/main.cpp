/*Press button to record,released button to playback*/

#include <M5Unified.h>

HardwareSerial GPSRaw(2);


void setup() {
    M5.begin();
    M5.Power.begin();
    Serial.begin(9600);
    GPSRaw.begin(9600,SERIAL_8N1,32, 26);
}

void loop() {
 
    if (GPSRaw.available()){
        int ch = GPSRaw.read();
        Serial.write(ch);
    }

    delay(100);
}