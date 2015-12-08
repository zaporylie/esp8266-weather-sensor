// Wifi init.
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send (404, "text/plain", message);
}

void handleData() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  float data[3];
  int code;
  
  if (readData(data)) {
    // Build response.
    root["humidity"] = data[0];
    root["temperature"] = data[1];
    root["heat_index"] = data[2];
    code = 200;
  }
  else {
    root["message"] = "Unable to get reading";
    code = 500;
  }
  
  // Print.
  String temp;
  root.printTo(temp);
  server.send(code, "application/javascript", temp);
}

bool getLocation() {
  
  String response;
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  const char* host = "ip-api.com";
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return false;
  }
  
  // We now create a URI for the request
  String url = "/json";
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    response += client.readStringUntil('\r');
  }

  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(response);
  lat = root["lat"];
  lon = root["lon"];
  city = root["city"];
  server.send(200, "text/plain", response);
}
