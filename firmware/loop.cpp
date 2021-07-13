#include "loop.h"

int modeController(){
    uint8_t buttonStatus = (!digitalRead(PIN_SW_1) * 2) + (!digitalRead(PIN_SW_0));

    printf("MODE %d\n", buttonStatus);

    if(buttonStatus==1) return mode= MODE1;
    if(buttonStatus==2) return mode= MODE2;
    if(buttonStatus==3) return mode= MODE3;
    return  BEGIN;
}


void loopModes() {
    // printf("%d\n", mode);

    switch (mode) {
        case BEGIN:
        mode=modeController();
        delay(200);

        break;

        // Mode 1 --------------------------------------------------------------
        case MODE1:
        algorithm_loop();

        break;

        // Mode 2 --------------------------------------------------------------
        case MODE2:
        // What should happen on mode 2
        // mode_sensorTest();
        whiteOverRainbow(75, 5);
        // rainbowFade2White(3, 3, 1);
        break;

        // Mode 3 --------------------------------------------------------------
        case MODE3:
        // What should happen on mode 3
        delay(2000);
        // motors.write(200,200);

        distance_read();
        // delay(10000);
        // mode = BEGIN;

        break;

        // Mode WAIT --------------------------------------------------------------
        case WAIT:
        delay(100);
        break;
    }

}

void loop(){

    // Main loop, which runs algorithms
    loopModes();

    // Handle OTA requests, if it is enabled
    #ifdef ENABLE_OTA_UPLOAD
    ArduinoOTA.handle();
    #endif

    // WiFi Monitor : Only support on MODE_0
    #ifdef ENABLE_WIFI_MONITOR
    wifiMonitor.handleClient();
    #endif

    #ifdef ESP_NOW_MASTER
    scanForSlave();
    #endif

    #ifdef ENABLE_MQTT
    // printf("handle mqtt\n");
    mqtt_handle();
    #endif

    delay(100);
}


void mode_sensorTest(){
    distance.test();
    compass.test();
    colorSensor.test();
    //ir.sendTestSignal();
}
