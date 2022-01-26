#include <Adafruit_SSD1306.h>
#include "bsec.h"
#include <Adafruit_DotStar.h>

Adafruit_DotStar strip = Adafruit_DotStar(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

Bsec iaqSensor;
String output;

void setup() {
  Serial.begin(9600);
  //while (!Serial);
  
  Serial.println(F("BME680 test"));
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  Serial.println("OLED begun");

  display.display();
  delay(100);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setRotation(0);
  
  iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire);
  output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
  Serial.println(output);
  checkIaqSensorStatus();
  bsec_virtual_sensor_t sensorList[10] = {
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
  };

  iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();
  // Print the header
  output = "Timestamp [ms], raw temperature [°C], pressure [hPa], raw relative humidity [%], gas [Ohm], IAQ, IAQ accuracy, temperature [°C], relative humidity [%], Static IAQ, CO2 equivalent, breath VOC equivalent";
  Serial.println(output);

  strip.setPixelColor(0, 0, 64, 0); 
  strip.show();
}

void loop() {
  display.setCursor(0,0);
  display.clearDisplay();

  unsigned long time_trigger = millis();
  if (! iaqSensor.run()) { // If no data is available
    checkIaqSensorStatus();
    return;
  }
  
  output = String(time_trigger);
  output += ", " + String(iaqSensor.rawTemperature);
  output += ", " + String(iaqSensor.pressure);
  output += ", " + String(iaqSensor.rawHumidity);
  output += ", " + String(iaqSensor.gasResistance);
  output += ", " + String(iaqSensor.iaq);
  output += ", " + String(iaqSensor.iaqAccuracy);
  output += ", " + String(iaqSensor.temperature);
  output += ", " + String(iaqSensor.humidity);
  output += ", " + String(iaqSensor.staticIaq);
  output += ", " + String(iaqSensor.co2Equivalent);
  output += ", " + String(iaqSensor.breathVocEquivalent);
  Serial.println(output);

  
  Serial.print("Temperature = "); Serial.print(iaqSensor.temperature); Serial.println(" *C");
  display.print("Temperature: "); display.print(iaqSensor.temperature); display.println(" *C");

  Serial.print("Humidity = "); Serial.print(iaqSensor.humidity); Serial.println(" %");
  display.print("Humidity: "); display.print(iaqSensor.humidity); display.println(" %");

  Serial.print("IAQ = "); Serial.print(iaqSensor.staticIaq); Serial.println("");
  display.print("IAQ: "); display.print(iaqSensor.staticIaq); display.println("");
  
  Serial.print("CO2 equiv = "); Serial.print(iaqSensor.co2Equivalent); Serial.println("ppm");
  display.print("CO2eq: "); display.print(iaqSensor.co2Equivalent); display.println("ppm");
  
  Serial.print("Breath VOC = "); Serial.print(iaqSensor.breathVocEquivalent); Serial.println("ppm");
  display.print("VOC: "); display.print(iaqSensor.breathVocEquivalent); display.println("ppm");

  Serial.print("Pressure = "); Serial.print(iaqSensor.pressure / 1000.0 + 20.0); Serial.println(" kPa");
  display.print("Pressure: "); display.print(iaqSensor.pressure / 1000 + 20); display.println(" kPa");
  
  Serial.println();
  display.display();
  delay(1000);
}


// Helper function definitions
void checkIaqSensorStatus(void)
{
  if (iaqSensor.status != BSEC_OK) {
    if (iaqSensor.status < BSEC_OK) {
      output = "BSEC error code : " + String(iaqSensor.status);
      Serial.println(output);
      display.setCursor(0,0);
      display.println(output);
      display.display();
      for (;;)  delay(10);
    } else {
      output = "BSEC warning code : " + String(iaqSensor.status);
      Serial.println(output);
    }
  }

  if (iaqSensor.bme680Status != BME680_OK) {
    if (iaqSensor.bme680Status < BME680_OK) {
      output = "BME680 error code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
      display.setCursor(0,0);
      display.println(output);
      display.display();
      for (;;)  delay(10);
    } else {
      output = "BME680 warning code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
    }
  }
}
