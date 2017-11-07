/********************************************************************** 
  Obtain measurements from a Honeywell HIH8000 Series sensor connected to an
  Arduino board using I2C protocol (not SPI):
  https://sensing.honeywell.com/sensors/humidity-sensors/HIH8000-series
  
  Utilizes the HIH8000I2C library:
  https://github.com/kiatAWDSA/HIH8000_I2C
  
  Please note that the pins used for I2C communication are different for each
  Arduino board. For Arduino Uno, the SDA and SCL pins are A4 and A5, respectively.
  A list of boards and corresponding pins can be found at:
  https://www.arduino.cc/en/Reference/Wire


  *************************************************************************************
  *                               INSTRUCTIONS                                        *
  *************************************************************************************
  1. Connect your sensor according to the recommended layout in pg10 of the datasheet
     linked above. Make sure the SDA and SCL pins of the sensor are connected to the
     appropriate pins on your Arduino board (see above link to the Wire documentation).
  2. Open your serial monitor and send 'g' without the quotes.
  3. To stop acquiring data, send 's' without the quotes.
  *************************************************************************************
  
  
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

#include <Wire.h>
#include "HIH8000_I2C.h"

HIH8000_I2C hihSensor = HIH8000_I2C(0x27);

bool getReading = false;
char serialReadBuffer[2];

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Wire.setClock(100000);
  Serial.begin(9600);
}

void loop() {
  if (getReading)
  {
    hihSensor.fetchMeasurement();
    
    switch (hihSensor.getStatus())
    {
      case 0:
        Serial.println(hihSensor.getHumidity());
        Serial.println(hihSensor.getTemperature());
        break;

      case 1:
        Serial.println("Stale data");
        Serial.println(hihSensor.getHumidity());
        Serial.println(hihSensor.getTemperature());
        break;
        
      case 2:
        Serial.println("Command mode");
        break;

      case 3:
        Serial.println("Diagnostic mode");
        break;

      default:
        Serial.println("Invalid status");
        break;
    }
    
    hihSensor.triggerMeasurement();
    
    delay(500);
  }
}

void serialEvent()
{
  Serial.readBytes(serialReadBuffer, 1);

  if (serialReadBuffer[0] == 'g')
  {
    getReading = true;
  }
  else if (serialReadBuffer[0] == 's')
  {
    getReading = false;
  }
}
