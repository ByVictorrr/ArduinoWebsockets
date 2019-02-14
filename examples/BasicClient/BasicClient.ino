#include "ArduinoWebsockets.h"
#include <WifiConnection.h>

using namespace websockets;
using namespace websockets::network;

WebsocketsClient client(new Esp8266TcpClient);
void setup() {
    Serial.begin(115200);
    // Connect to wifi
    WifiConnection::tryConnect("SSID", "PASSWORD");

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && !WifiConnection::isConnected(); i++) {
        Serial.print(".");
        delay(1000);
    }

    // Check if connected to wifi
    if(!WifiConnection::isConnected()) {
        Serial.println("No Wifi!");
        return;
    }

    Serial.println("Connected to Wifi, Connection to server.");
    // try to connect to Websockets server
    bool connected = client.connect("SERVER_IP", "/", 8080);
    if(connected) {
        Serial.println("Connecetd!");
        client.send("Hello Server");
    } else {
        Serial.println("Not Connected!");
    }
    
    // run callback when messages are received
    client.onMessage([&](WebsocketsMessage message){
        Serial.print("Got Data: ");
        Serial.println(message.data().c_str());
    });
}

void loop() {
    // let the websockets client check for incoming messages
    if(client.available()) {
        client.poll();
    }
    delay(500);
}