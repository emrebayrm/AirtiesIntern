#include <stdio.h>
#include <MQTTClient.h>
#include <stdlib.h>
#include <memory.h>

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "test"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

int main(int argc, char* argv[])
{
    char *message;
    char *topic;
    char *address;
    message = PAYLOAD;
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
        if(strncmp(argv[i],"-m",2) == 0){
            i++;
            message = argv[i];
        }
        else if(strncmp(argv[i],"-t",2) == 0){
            i++;
            topic = argv[i];
        }
    }

    printf("%s %s %s \n",address,message,topic);

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
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
    pubmsg.payload = message;
    pubmsg.payloadlen = strlen(message);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    printf("Waiting for up to %d seconds for publication of %s\n"
           "on topic %s for client with ClientID: %s\n",
           (int)(TIMEOUT/1000), message, topic, CLIENTID);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}