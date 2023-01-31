#ifndef HEADER_PSERVER
#define HEADER_PSERVER

#include <ESP8266WiFi.h>

// Define timeout time in milliseconds (example: 2000ms = 2s)
#define TIMEOUT_TIME 2000

struct ServerListen {
  bool hasRequest;
  String command;
  WiFiClient client;
  String header;
};

ServerListen serverListen(WiFiServer& server);
ServerListen processRequest(WiFiClient& client, String header);

#endif
