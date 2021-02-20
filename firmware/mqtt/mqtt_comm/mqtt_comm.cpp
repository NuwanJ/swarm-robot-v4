#include "mqtt_comm.h"

#ifdef ENABLE_MQTT

void mqtt_comm_out(char* msg){
   sprintf(tempString1, "%s/%s", CHANNEL,TOPIC_COMM_OUT_SIMPLE);
   sprintf(tempString2, "{\"id\": %d,\"msg\":\"%s\"}", mqtt_robot_id, msg);
   mqtt_publish(tempString1, tempString2, false);
}

void mqtt_comm_out(char* msg, int dist){
   // Can specify the booadcast distance
   sprintf(tempString1, "%s/%s", CHANNEL,TOPIC_COMM_OUT_SIMPLE);
   sprintf(tempString2, "{\"id\": %d,\"msg\":\"%s\",\"dist\":\"%d\"}", mqtt_robot_id, msg, dist);
   mqtt_publish(tempString1, tempString2, false);
}
#endif
