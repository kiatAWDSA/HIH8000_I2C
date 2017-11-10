/********************************************************************** 
  Obtain measurements from a Honeywell HIH8000 Series sensor connected to an
  Arduino board using I2C protocol (not SPI):
  https://sensing.honeywell.com/sensors/humidity-sensors/HIH8000-series
  
  To adjust the internal settings of the
  sensor such as its I2C address and alarms, use the HIH8000CommandI2C library instead:
  https://github.com/kiatAWDSA/HIH8000Command_I2C
  
  Please note that the pins used for I2C communication are different for each
  Arduino board. For Arduino Uno, the SDA and SCL pins are A4 and A5, respectively.
  A list of boards and corresponding pins can be found at:
  https://www.arduino.cc/en/Reference/Wire
  
  
  Copyright 2017 Soon Kiat Lau
  
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
***********************************************************************/

#ifndef HIH8000_I2C_H
#define HIH8000_I2C_H

#include "Arduino.h"
#include "Wire.h"

class HIH8000_I2C {
  public:
    HIH8000_I2C(uint8_t address);
    HIH8000_I2C();
    ~HIH8000_I2C();
    void setAddress(uint8_t newAddress);
    void triggerMeasurement();
    void fetchMeasurement();
    uint8_t getStatus();
    uint8_t getAddress();
    float getHumidity();
    float getTemperature();
  
  private:
    // This is used when fetching data. The documentation says 4 bytes will be transmitted from the sensor.
    static const uint8_t dataBytes_    = 4;
  
    bool addressSet_            = 0;
    uint8_t address_            = 0;
    uint8_t status_             = 0;
    uint16_t humidityBuffer_    = 0;
    uint16_t temperatureBuffer_ = 0;
    float humidity_             = 0;
    float temperature_          = 0;
};

#endif  // HIH8000_I2C_H