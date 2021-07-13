#include "neopixel.h"
#include "mqtt/mqtt.h"

#ifdef ENABLE_NEOPIXEL_RING

Adafruit_NeoPixel neopixel(NEOPIXEL_LED_COUNT, PIN_NEOPIXEL_LED, NEO_GRB + NEO_KHZ800);

void beginNeoPixel() {
    neopixel.begin();
    neopixel.clear();
    neopixel.setBrightness(NEOPIXEL_BRIGHTNESS);
    neopixel.show();
    Serial.println(F(">> NeoPixel\t:enabled"));
}

void pixelUpdateLED(uint8_t i, int r, int g, int b){
    neopixel.setPixelColor(i, neopixel.Color(r, g, b));
    neopixel.show();
}
void neoPixelTest() {

    colorWipe(neopixel.Color(255,   0,   0)     , 50); // Red
    colorWipe(neopixel.Color(  0, 255,   0)     , 50); // Green
    colorWipe(neopixel.Color(  0,   0, 255)     , 50); // Blue
    whiteOverRainbow(75, 5);
    rainbowFade2White(3, 3, 1);
}

void pixelShowColor(int r, int g, int b) {

    if(r==0 && g==0 && b==0){
        pixelOff();
    }else{
        neopixel.clear();
        neopixel.fill(neopixel.Color(r, g, b), 0, NEOPIXEL_LED_COUNT);
        delay(500);
        neopixel.show();
    }

    #ifdef ENABLE_MQTT
    // Acknowledge the server about change
    sprintf(tempString1, "%s/%s", CHANNEL,TOPIC_NEOPIXEL_PUBLISH);
    sprintf(tempString2, "{\"id\":%d,\"R\":%d,\"G\":%d,\"B\":%d}", mqtt_robot_id, r, g, b);
    mqtt_publish(tempString1, tempString2, false);
    #endif
}
void pixelColorWave(int r, int g, int b) {

    neopixel.clear();
    for (int i = 1; i <= NEOPIXEL_LED_COUNT / 2; i++) {
        neopixel.setPixelColor(i, neopixel.Color(r, g, b));
        neopixel.setPixelColor(NEOPIXEL_LED_COUNT - 1 - i, neopixel.Color(r, g, b));
        neopixel.show();
        delay(DELAYVAL);
    }
    delay(DELAYVAL * 5);
    pixelOff();
}
void colorWipe(uint32_t color, int wait) {
    for (int i = 1; i < neopixel.numPixels(); i++) {
        neopixel.setPixelColor(i, color);         //  Set pixel's color (in RAM)
        neopixel.show();                          //  Update strip to match
        delay(wait);                           //  Pause for a moment
    }
}
void pixelOff() {
    neopixel.clear();
    neopixel.fill(neopixel.Color(0,0,0), 0, NEOPIXEL_LED_COUNT);
    delay(500);
    neopixel.show();
}

// Functions written for testing the performance of the LEDs
void whiteOverRainbow(int whiteSpeed, int whiteLength) {

    if (whiteLength >= neopixel.numPixels()) whiteLength = neopixel.numPixels() - 1;

    int      head          = whiteLength - 1;
    int      tail          = 0;
    int      loops         = 3;
    int      loopNum       = 0;
    uint32_t lastTime      = millis();
    uint32_t firstPixelHue = 0;

    for (;;) {
        for (int i = 1; i < neopixel.numPixels(); i++) {
            if (((i >= tail) && (i <= head)) ||
            ((tail > head) && ((i >= tail) || (i <= head)))) {
                neopixel.setPixelColor(i, neopixel.Color(255,255,255));
            } else {
                int pixelHue = firstPixelHue + (i * 65536L / neopixel.numPixels());
                neopixel.setPixelColor(i, neopixel.gamma32(neopixel.ColorHSV(pixelHue)));
            }
        }
        neopixel.show();
        firstPixelHue += 40;

        if ((millis() - lastTime) > whiteSpeed) {
            if (++head >= neopixel.numPixels()) {
                head = 0;
                if (++loopNum >= loops) return;
            }
            if (++tail >= neopixel.numPixels()) tail = 0;
            lastTime = millis();
        }
    }
}
void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
    int fadeVal = 0, fadeMax = 100;
    for (uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops * 65536; firstPixelHue += 256) {
        for (int i = 1; i < neopixel.numPixels(); i++) {
            uint32_t pixelHue = firstPixelHue + (i * 65536L / neopixel.numPixels());
            neopixel.setPixelColor(i, neopixel.gamma32(neopixel.ColorHSV(pixelHue, 255, 255 * fadeVal / fadeMax)));
        }
        neopixel.show();
        delay(wait);

        if (firstPixelHue < 65536) {
            if (fadeVal < fadeMax) fadeVal++;
        } else if (firstPixelHue >= ((rainbowLoops - 1) * 65536)) {
            if (fadeVal > 0) fadeVal--;
        } else {
            fadeVal = fadeMax;
        }
    }
    // delay(500);
}

#else

void beginNeoPixel() {
    Serial.println(F(">> NeoPixel\t:disabled"));
}
void pixelUpdateLED(uint8_t i, int r, int g, int b){}
void neoPixelTest() {}

void pixelShowColor(int r, int g, int b) {}
void pixelColorWave(int r, int g, int b) {}
void colorWipe(uint32_t color, int wait) {}
void pixelOff() {}

void whiteOverRainbow(int whiteSpeed, int whiteLength) {}
void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {}

// ------------------------------------------------------------- end of else
#endif
