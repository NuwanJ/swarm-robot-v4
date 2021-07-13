#include "algorithm.h"

#include "modules/neopixel/neopixel.h"
#include "modules/motors/motors.h"

#include "config/global_variables.h"
#include "mqtt/mqtt.h"

#ifdef ALGO_MOVE_ROBOT
// -----------------------------------------------------------------------------

int robotState = ROBOT_BEGIN;

void algorithm_setup(){
    Serial.println("algorithm: setup");
    motors.stop();
}

void algorithm_loop(){

    if(robotState==ROBOT_RUN){
        algorithm_execute();
        delay(1000);

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
        // What to do if message received to comm/in/{robotId} ?
    }
}

// functional part of the pattern
void algorithm_execute(){
    motors.stop();
    int d = distance_read();
    Serial.printf("algo_dist: %d\n", d);

    if(d < 25){
        int random = (rand() % 100);
        int sign = (random%2 == 0) ? 1 : -1;

        Serial.printf("random: %d, sign: %d \n", random, sign);

        // Go back for 1 second
        motors.write(-150,-150);
        mqtt_delay(1000);
        motors.stop();

        while(distance_read() < 30) {
            // TODO: avoid infinity loop in here
            Serial.println("rotate until dist < 50\n");
            motors.write(75*sign,-75*sign);
            mqtt_delay(1000);
            motors.stop();
        }

        // turn for 2 more second
        motors.write(75*sign,-75*sign);
        mqtt_delay(2000);
        motors.stop();

    }else{
        motors.write(150,150);
        mqtt_delay(1000);
    }

}

// instruct to start the pattern
void algorithm_start(){
    robotState = ROBOT_RUN;
    Serial.println("algorithm: start");
}

// reset the pattern variables and state
void algorithm_reset(){
    robotState = ROBOT_BEGIN;
    Serial.println("algorithm: reset");
}

// stop the execution of the pattern
void algorithm_stop(){
    robotState = ROBOT_WAIT;
    Serial.println("algorithm: wait");
    motors.stop();
}

#endif
