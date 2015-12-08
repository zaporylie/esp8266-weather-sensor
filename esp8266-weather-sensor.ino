/*
  To upload through terminal you can use: curl -F "image=@esp8266-weather-sensor.cpp.generic.bin" <HOSTNAME>.local/update
*/


// Wifi libraries.
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h> 
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoJson.h>

// Settings library.
#include "settings.h"

// Sensor init.
#include <DHT.h>
DHT dht(DHTPIN, DHTTYPE);
#include "sensors.h"

// Server init.
#include "server.h"


// Display libraries.
#include <Wire.h>
#include "ssd1306_i2c.h"
#include "display.h"



void setup() {
  // Start serial.
  Serial.begin(115200);

  // Start wifi manager.
  WiFiManager wifi;

  // Autoconnecto or use esp8266 network
  wifi.autoConnect(HOSTNAME);

  // Visit esp8266.local to manage this device.
  if (MDNS.begin(HOSTNAME)) {
    Serial.println("MDNS responder started");
  }

  // Handle not found.
  server.onNotFound(handleNotFound);

  // Handle data.
  server.on("/", handleData);
  server.on("/test", getLocation);

  // Add updater.
  httpUpdater.setup(&server);

  // Start server.
  server.begin();

  // Notify.
  Serial.println("HTTP server started");

  // Start sensor.
  dht.begin();

  // Start display.
  display.init();
  display.flipScreenVertically();
  // set the drawing functions
  display.setFrameCallbacks(frameCount, frameCallbacks);
  // how many ticks does a slide of a frame take?
  display.setFrameTransitionTicks(10);
  // defines how many ticks the driver waits between frame transitions
  display.setFrameWaitTicks(500);

  display.clear();
  display.display();
}

void loop() {
  if (display.getFrameState() == display.FRAME_STATE_FIX) {
    // Wait for request.
    server.handleClient();
  }
  // clear the frame
  display.clear();

  // Tell the driver to render the next frame.
  // This enables the frame mode including the transition
  // and the drawing of the frame indicators
  display.nextFrameTick();
  
  // copy the buffer to the display
  display.display();
}
