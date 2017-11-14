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

#include "HIH8000_I2C.h"

// Instantiate with device address.
HIH8000_I2C::HIH8000_I2C(uint8_t address) {
  setAddress(address);
}

// If user prefers to instantiate class first and set the address later.
HIH8000_I2C::HIH8000_I2C() {
  addressSet_ = false;
}

// No specific destruction subroutines necessary (ending Wire here might affect other devices on I2C line).
HIH8000_I2C::~HIH8000_I2C() {}

// Set/change I2C address of the sensor
// Address MUST be 7-bits (max value is 0x7F i.e. 127)
void HIH8000_I2C::setAddress(uint8_t newAddress) {
  if (newAddress <= 0x7F) {
    address_ = newAddress;
    addressSet_ = true;
  }
}

// To initiate a measurement, need to send a write request to the sensor without any data. This is equivalent to initiating a transmission in the Wire libary.
// NOTE: It typically takes 36.65 ms for a reading to be generated.
HIH8000_I2C_comStatus HIH8000_I2C::triggerMeasurement() {
  if (addressSet_) {
    Wire.beginTransmission(address_);
    endTransErr_ = Wire.endTransmission();
    
    if (endTransErr_ == 0) { // Should return 0 if success, otherwise something is wrong
      return HIH8000_I2C_comStatus_OK;
    } else {
      switch (endTransErr_) {
        case 1:
          return HIH8000_I2C_comStatus_LONGDATA;
          break;
        case 2:
          return HIH8000_I2C_comStatus_NACKADD;
          break;
        case 3:
          return HIH8000_I2C_comStatus_NACKDATA;
          break;
        case 4:
        default:
          return HIH8000_I2C_comStatus_OTHER;
          break;
      }
    }
  } else {
    return HIH8000_I2C_comStatus_ADDRESS;
  }
}

// Get the data stored in the sensor's registers and process them. These data is from the most recent triggered measurement.
HIH8000_I2C_comStatus HIH8000_I2C::fetchMeasurement(){
  if (addressSet_) {
    if (Wire.requestFrom(address_, dataByteCount_) == dataByteCount_) { // Should return dataByteCount_ byte(s), otherwise something is wrong
      // Get raw humidity data
      humidityBuffer_ = Wire.read();
      humidityBuffer_ <<= 8;
      humidityBuffer_ |= Wire.read();

      // Get raw temperature data
      temperatureBuffer_ = Wire.read();
      temperatureBuffer_ <<= 8;
      temperatureBuffer_ |= Wire.read();
      temperatureBuffer_ >>= 2;  // Remove the last two "Do Not Care" bits

      // Extract out status bits and clean raw humidity data
      status_ = 0xC000 & humidityBuffer_;         // The integer constant is 1100000000000000 in binary
      status_ >>= 14;                             // Push it to become 2 digits
      humidityBuffer_ = 0x3FFF & humidityBuffer_; // The integer constant is 0011111111111111 in binary
      
      // Process the raw values. 16382 is 2^14 - 2.
      humidity_ = humidityBuffer_/16382.0 * 100;
      temperature_ = temperatureBuffer_/16382.0 * 165 - 40;
      
      return HIH8000_I2C_comStatus_OK;
      
    } else {
      return HIH8000_I2C_comStatus_BYTECOUNT;
    }
  } else {
    return HIH8000_I2C_comStatus_ADDRESS;
  }
}

// Get the address set for the sensor.
uint8_t HIH8000_I2C::getAddress() {
  return address_;
}

// Get the status code given by the sensor.
uint8_t HIH8000_I2C::getStatus() {
  return status_;
}

// Get the processed humidity value.
float HIH8000_I2C::getHumidity() {
  return humidity_;
}

// Get the processed temperature value.
float HIH8000_I2C::getTemperature() {
  return temperature_;
}