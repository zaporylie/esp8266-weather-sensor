
// Display init.
SSD1306 display(I2C, SDA, SCL);
int frameCount = 2;

void actualWeather(int x, int y) {
  // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
  // Besides the default fonts there will be a program to convert TrueType fonts into this format
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0 + x, 48 + y, String((uint8_t) (readDataCounter + frequencyLocal - (millis() / 1000))));

  float data[3];
  if (!readData(data)) {
    return;
  }
  
  display.setFont(ArialMT_Plain_24);
  display.drawString(0 + x, 12 + y, String(data[0]) + " *C");

  
  display.setFont(ArialMT_Plain_10);
  display.drawString(0 + x, 35 + y, String(data[1]) + " %");
  display.drawString(64 + x, 35 + y, String(data[2]) + " pt");
 }

 void forecast(int x, int y) {
  // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
  // Besides the default fonts there will be a program to convert TrueType fonts into this format
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0 + x, 48 + y, String((uint32_t) (readForecastCounter + frequencyRemote - (millis() / 1000))));
  
  if (!readForecast()) {
//  if (true) {
    display.drawString(0 + x, 0 + y, "Connection failed");
    return;
  }
  
  display.setFont(ArialMT_Plain_24);
  display.drawString(0 + x, 12 + y, String(temperatureRemoteBuffer) + " *C");

  display.setFont(ArialMT_Plain_10);
  display.drawString(0 + x, 35 + y, String(humidityRemoteBuffer) + " %");
  display.drawString(64 + x, 35 + y, String(pressureRemoteBuffer) + " hPa");
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  if (sizeof(city) < 2) {
    display.drawString(128 + x, 0 + y, String(lat) + ", " + String(lon));
  }
  else {
    display.drawString(128 + x, 0 + y, city);
  }
}

// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
void (*frameCallbacks[])(int x, int y) = {actualWeather, forecast};
