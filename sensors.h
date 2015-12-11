
float lat;
float lon;
const char* city;


float temperatureBuffer;
float humidityBuffer;
float heatIndexBuffer;
unsigned long readDataCounter;
bool readData(bool cache = true) {

  if (isnan(temperatureBuffer) || isnan(humidityBuffer) || isnan(heatIndexBuffer)) {
    cache = false;
  }
  int tmp = millis() / 1000;
  if (((tmp - readDataCounter) < frequencyLocal) && cache) {
    return true;
  }
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  temperatureBuffer = dht.readHumidity();
  // Read temperature as Celsius (the default)
  humidityBuffer = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(temperatureBuffer) || isnan(humidityBuffer)) {
    Serial.println("Failed to read from DHT sensor!");
    return false;
  }
  
  // Compute heat index in Celsius (isFahreheit = false)
  heatIndexBuffer = dht.computeHeatIndex(temperatureBuffer, humidityBuffer, false);
  readDataCounter = tmp;
  return true;
}

