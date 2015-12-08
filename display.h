

// Display init.
SSD1306 display(I2C, SDA, SCL);
int frameCount = 2;

void actualWeather(int x, int y) {
  // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
  // Besides the default fonts there will be a program to convert TrueType fonts into this format
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);

  float data[3];
  if (!readData(data)) {
    return;
  }
  
  display.drawString(0 + x, 0 + y, "Temperature: " + String(data[0]));
  display.drawString(0 + x, 16 + y, "Humidity: " + String(data[1]));
  display.drawString(0 + x, 32 + y, "Heat index: " + String(data[2]));
  display.drawString(0 + x, 48 + y, String((int) (millis() / 1000) - readDataCounter));
 }

 void forecast(int x, int y) {
  // Demonstrates the 3 included default sizes. The fonts come from SSD1306Fonts.h file
  // Besides the default fonts there will be a program to convert TrueType fonts into this format
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  
  display.drawString(0 + x, 0 + y, "City: ");
  display.drawString(0 + x, 16 + y, "Temperature: ");
  display.drawString(0 + x, 32 + y, "Humidity: ");
  display.drawString(0 + x, 48 + y, String((int) (millis() / 1000) - readDataCounter));
}

// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
void (*frameCallbacks[])(int x, int y) = {actualWeather, forecast};
