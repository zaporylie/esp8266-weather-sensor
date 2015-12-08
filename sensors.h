
float lat;
float lon;
const char* city;


float temperatureBuffer;
float humidityBuffer;
float heatIndexBuffer;
unsigned long readDataCounter;
bool readData(float data[], bool cache = true) {

  if (isnan(temperatureBuffer) || isnan(humidityBuffer) || isnan(heatIndexBuffer)) {
    cache = false;
  }
  int tmp = millis() / 1000;
  if (((tmp - readDataCounter) < frequencyLocal) && cache) {
    data[0] = temperatureBuffer;
    data[1] = humidityBuffer;
    data[2] = heatIndexBuffer;
    return true;
  }
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  temperatureBuffer = data[0] = dht.readHumidity();
  // Read temperature as Celsius (the default)
  humidityBuffer = data[1] = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(data[0]) || isnan(data[1])) {
    Serial.println("Failed to read from DHT sensor!");
    data[0] = temperatureBuffer;
    data[1] = humidityBuffer;
    data[2] = heatIndexBuffer;
    return false;
  }
  
  // Compute heat index in Celsius (isFahreheit = false)
  heatIndexBuffer = data[2] = dht.computeHeatIndex(data[0], data[1], false);
  readDataCounter = tmp;
  return true;
}

bool readForcast() {
  if (isnan(lat) || isnan(lon)) {
//    if (!getlocation()) {
      return false;
//    }
  }
}

