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
  int code;
  
  if (readData()) {
    // Build response.
    root["humidity"] = humidityBuffer;
    root["temperature"] = temperatureBuffer;
    root["heat_index"] = heatIndexBuffer;
    code = 200;
  }
  else {
    root["message"] = "Unable to get reading";
    code = 500;
  }
  
  // Print.
  String temp;
  root.printTo(temp);
  server.send(code, "application/json", temp);
}

bool parseClientResponse(WiFiClient &client, String &result) {
  
  // Skip irrelevant lines;
  while(client.available() && (result != "\n")) {
    result = client.readStringUntil('\r');
    Serial.print(result);
  }

  while (client.available()) {
    // Parse JSON
    result = client.readStringUntil('\r');
    if (result.length() > 5) {
      result = result.substring(1);
      return true;
    }
  }

  return false;
}

bool getLocation() {
  
  String result;
  
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
  
  if (!parseClientResponse(client, result)) {
    Serial.println("unable to parse response");
    return false;
  }
  
  int size = result.length() + 1;
  char json[size];
  result.toCharArray(json, size);
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  
  if (!root.success()) {
    Serial.println("unable to parse json");
    return false;
  }
  
  lat = root["lat"];
  Serial.println(lat);
  lon = root["lon"];
  Serial.println(lon);
  city = root["city"];
  Serial.println(city);

  return true;
}

float temperatureRemoteBuffer;
float humidityRemoteBuffer;
float pressureRemoteBuffer;
unsigned long readForecastCounter;
bool readForecast(bool cache = true) {

  if (lat == 0 || lon == 0) {
    if (!getLocation()) {
      return false;
    }
  }

  if (isnan(temperatureBuffer) || isnan(humidityBuffer) || isnan(heatIndexBuffer)) {
    cache = false;
  }
  
  int tmp = millis() / 1000;
  if (((tmp - readForecastCounter) < frequencyRemote) && cache) {
    return true;
  }

  String result;
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  const char* host = "api.openweathermap.org";
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return false;
  }
  
  // We now create a URI for the request
  String url = "/data/2.5/weather";
  url += "?";
  url += "units=metric";
  url += "&";
  url += "lat=";
  url += lat;
  url += "&";
  url += "lon=";
  url += lon;
  url += "&";
  url += "appid=";
  url += APPID;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  if (!parseClientResponse(client, result)) {
    Serial.println(result);
    return false;
  }

  int size = result.length() + 1;
  char json[size];
  result.toCharArray(json, size);
  StaticJsonBuffer<2000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  
  if (!root.success()) {
    server.send(200, "application/json", result);
    return false;
  }

  temperatureRemoteBuffer = root["main"]["temp"];
  humidityRemoteBuffer = root["main"]["humidity"];
  pressureRemoteBuffer = root["main"]["pressure"];
  
  readForecastCounter = tmp;
  return true;
}

