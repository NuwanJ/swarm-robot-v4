

#include "algorithm.h"

#include "modules/neopixel/neopixel.h"
#include "config/global_variables.h"
#include "mqtt/mqtt.h"

#ifdef ALGO_COMMUNICATION
// -----------------------------------------------------------------------------

int robotState = ROBOT_BEGIN;

long startTime;
int responseCount = 0;
long roundDelay[20];
char tmp[16];

// int stopT = millis();

void algorithm_setup(){
    Serial.println("algorithm: setup");
    pixelShowColor(0,0,0);

    // Initiate delay registers
    for(int i=0;i<20;i++){
        roundDelay[i] = 0;
    }
}

void algorithm_loop(){

    if(robotState==ROBOT_RUN){

        if(responseCount == 1){
            Serial.println("All responses received successfully !");

            for (int i = 0; i < 20; i++) {
                if (roundDelay[i] != 0) {
                    sprintf(tempString1, "%s/%s", CHANNEL,TOPIC_LOG);
                    sprintf(tempString2, "{\"id\":\"%d\",\"msg\":\"%ld\"}", i, roundDelay[i]);
                    client.publish(tempString1, tempString2, false);
                }
            }

            // Reset the robot
            algorithm_reset();
        }
        delay(50);

    }else if(robotState==ROBOT_BEGIN){
        algorithm_setup();
        robotState = ROBOT_WAIT;

    }else{
        // wait
        delay(100);
    }
}

void algorithm_interrupt(robot_interrupt_t interrupt, char* msg){
    if(interrupt == INT_COMM_IN){
        // Serial.printf("algorithm: execute %s \n", msg);

        int originId, responderId;
        char tempStr[16];
        int spaceCount = 0;

        for(int k=0; msg[k]!='\0';k++){
            // printf("%c ", msg[k]);
            if( msg[k] == ' ') spaceCount++;
        }
        // printf("%d\n", spaceCount);

        if(spaceCount==1){
            int formatLen = sscanf(msg, "%d %s", &originId, tempStr);

            // This is a message from the origin, lets reply
            // Format: <Origin> REQ

            if(formatLen == 2 && originId != mqtt_robot_id){
                Serial.println("Received REQ message");

                // Send an ACK message to the originator
                // <Origin> <Responder> REQ
                sprintf(tempString2, "%d %d ACK", originId, mqtt_robot_id);
                mqtt_comm_out_simple(tempString2);

                pixelShowColor(0,0,128);
            }else{
                Serial.printf("%s isn't in correct format\n\n", msg);
            }

        } else if(spaceCount == 2){

            int formatLen = sscanf(msg, "%d %d ACK", &originId, &responderId);

            if(formatLen ==2){
                Serial.println("Received ACK message");
                roundDelay[responderId] = millis() - startTime;
                responseCount++;
                Serial.printf("%d > %ld from %d\n", responseCount, roundDelay[responderId], responderId);

            }else{
                Serial.printf("%s isn't in correct format\n\n", msg);
            }

        }else{
            Serial.println("Incorrect message format");
        }
    }
}

// functional part of the pattern
void algorithm_execute(){
    delay(50);
}

// instruct to start the pattern
void algorithm_start(){
    robotState = ROBOT_RUN;
    Serial.println("algorithm: start");

    pixelShowColor(0,128,0);

    responseCount = 0;
    startTime = millis();

    sprintf(tempString2, "%d REQ", mqtt_robot_id);
    mqtt_comm_out_simple(tempString2);

    // Is needed a while loop here ?
}

// reset the pattern variables and state
void algorithm_reset(){
    robotState = ROBOT_BEGIN;
    pixelShowColor(0,0,0);
    Serial.println("algorithm: reset");
}

// stop the execution of the pattern
void algorithm_stop(){
    robotState = ROBOT_WAIT;
    Serial.println("algorithm: wait");
}


#endif
