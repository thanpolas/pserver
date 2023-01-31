#include <pserver.h>

/**
 * Will listen for a request and return the full body of the request along
 * with the client object.
 *
 * @param	wifiserver server	The server to liste for requests.
 * @return	ServerListen
 */
ServerListen serverListen(WiFiServer& server) {
  // Listen for incoming clients
  WiFiClient client = server.available();
  if (!client) {
    ServerListen request;
    request.hasRequest = false;
    return request;
  }
  Serial.printf("Request incoming...\n");

  // Current time
  unsigned long currentTime = millis();
  // Previous time
  unsigned long previousTime = 0;

  // make a String to hold incoming data from the client
  String currentLine = "";
  // Make a string to hold the
  String header = "";

  previousTime = currentTime;

  // A Client is connected, loop to receive the entire message or until
  // the defined timeout time is exhausted.
  while (client.connected() && currentTime - previousTime <= TIMEOUT_TIME) {
    currentTime = millis();

    // If no data available, yield
    if (!client.available()) {
      yield();
    }

    // Data available, read them...
    char c = client.read();
    header += c;

    // if the byte is a newline character
    if (c == '\n') {
      // if the current line is blank, you got two newline characters in a row.
      // that's the end of the client HTTP request, so return the request.
      if (currentLine.length() == 0) {
        ServerListen request = processRequest(client, header);
        return request;
      } else {
        currentLine = "";
      }

      // Next while loop
      continue;
    }

    // If received anything other than a carriage char, append it.
    if (c != '\r') {
      currentLine += c;
    }
  }

  client.println("HTTP/1.1 408 Request Timeout");
  client.println();
  client.stop();

  ServerListen request;
  request.hasRequest = false;
  return request;
};

ServerListen processRequest(WiFiClient& client, String header) {
  int getIndex = header.indexOf("GET /");
  int spaceIndex = header.indexOf(" ", getIndex + 5);
  String command = header.substring(getIndex + 5, spaceIndex);

  ServerListen request = {
    true,
    command,
    client,
    header,
  };

  client.println("HTTP/1.1 200 OK");
  client.println();
  client.stop();

  return request;
};
