//
// Created by patates on 7/9/18.
//
#include <stdio.h>
#include <MQTTClient.h>
#include <stdlib.h>
#include <memory.h>

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "EmreBSubs"
#define TOPIC       "test"
#define QOS         1
#define TIMEOUT     10000L

int main(int argc, char* argv[]) {
    char *topic;
    char *address;

    topic = TOPIC;
    address = ADDRESS;
    if(argc % 2 == 0){
        printf("Usage : ./pub   | default \n"
               "./pub -h <address> -m <message> -t <topic> \n");
        return 1;
    }
    for (int i = 1; i < argc; ++i) {
        if(strncmp(argv[i],"-h",2) == 0){
            ++i;
            address = argv[i];
        }
        else if(strncmp(argv[i],"-t",2) == 0){
            i++;
            topic = argv[i];
        }
    }
    printf("PARAMS : %s , %s \n",address,topic);

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, address, CLIENTID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);


    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }

    if( (rc =  MQTTClient_subscribe(client,topic,QOS)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to subscribe, return code %d\n",rc);
        exit(-1);
    }
    MQTTClient_message *message;
    int lenTopic;
    lenTopic = strlen(topic);
    int i = 0;
    char *mes;

    while(i < 10) {
        if ((rc = MQTTClient_receive(client, &topic, &lenTopic, &message, TIMEOUT)) != MQTTCLIENT_SUCCESS) {
            perror("Failed ");
            exit(-1);
        }
        if(message != NULL) {
            mes = malloc(message->payloadlen+1);
            strncpy(mes,message->payload,message->payloadlen);
            mes[message->payloadlen] = '\0';
            printf("%s \n", mes);
            i++;
            free(mes);
        }
        else{
            printf("No message Received \n");
        }
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
}

