#include "features.h"
#include "mqtt.h"
#include "sensors/color/color.h"
#include "config/global_variables.h"

uint8_t color_lock;
// uint8_t color_virt[4];    // TODO: replace by a struct
struct Color color_virt;

#ifdef ENABLE_MQTT

void mqtt_color_handle(char* msg){
    // Serial.printf("color update from server: %d -> %s \n", atoi(g[3]), msg);

    uint8_t r,g,b,c;
    int resp = sscanf(msg, "%u %u %u %u",(int*)&r,(int*)&g,(int*)&b,(int*)&c);

    if ( resp == 4){
        color_lock = 0;
         color_virt.G = constrain(r,0,255);
         color_virt.R = constrain(g,0,255);
         color_virt.B = constrain(b,0,255);
         color_virt.C = constrain(c,0,255);
    }else{

         color_virt.G = 0;
         color_virt.R = 0;
         color_virt.B = 0;
         color_virt.C = 0;
        Serial.print(F("Error_ColorResponseArgs"));
    }
}

void color_read(Color* color){

    // Get physical sensor reading
    struct Color color_phy;

    colorSensor.getColor(&color_phy.R,&color_phy.G,&color_phy.B,&color_phy.C);

    // Publish: sensor/color
    //      { "id":[robotID], "R":[R], "G":[G], "B":[B], "ambient":[ambient], "reality": "V" }
    sprintf(tempString1, "%s/%s", CHANNEL,TOPIC_COLOR_REQ_TO_SERVER);
    sprintf(tempString2, "{\"id\":\"%d\",\"R\":\"%d\",\"G\":\"%d\",\"B\":\"%d\",\"ambient\":\"%d\",\"reality\":\"V\"}", \
        mqtt_robot_id, color_phy.R, color_phy.G, color_phy.B, color_phy.C);

    color_lock=1;
    mqtt_publish(tempString1, tempString2, false);

    mqtt_wait(&color_lock);

    if(color_lock == 0){
        // have both sensor readings, return the average reading
        // TODO: Is this approach OK ?
        color->R = (color_virt.R == 0) ? color_phy.R : (color_phy.R + color_virt.R)/2;
        color->G = (color_virt.G == 0) ? color_phy.G : (color_phy.G + color_virt.G)/2;
        color->B = (color_virt.B == 0) ? color_phy.B : (color_phy.B + color_virt.B)/2;
        color->C = (color_virt.C == 0) ? color_phy.C : (color_phy.C + color_virt.C)/2;

    }else{
        // only physical readings
        color->R = color_phy.R;
        color->G = color_phy.G;
        color->B = color_phy.B;
        color->C = color_phy.C;
    }

    Serial.printf("Color: phy: %3u,%3u,%3u,%3u\t virt: %3u,%3u,%3u,%3u\t return: %3u,%3u,%3u,%3u\n\n", \
        color_phy.R,color_phy.G,color_phy.B,color_phy.C, \
        color_virt.R,color_virt.G,color_virt.B,color_virt.C, \
        color->R,color->G,color->B,color->C);

}

#endif