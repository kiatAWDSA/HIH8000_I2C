# HIH8000_I2C
Obtain measurements from a Honeywell HIH8000 Series sensor connected to an Arduino board using I2C protocol (NOT SPI):
https://sensing.honeywell.com/sensors/humidity-sensors/HIH8000-series

To adjust the internal settings of the sensor such as its I2C address and alarms, use the HIH8000CommandI2C library instead:
https://github.com/kiatAWDSA/HIH8000Command_I2C

NOTE: This library applies mainly to the 4-pin through-hole variants of the Honeywell HIH8000 family of sensors, which utilize the I2C protocol. I have not tested this library for the surface-mount variants (some of which uses the SPI protocol).

Useful documents (the third document is especially useful if you want to modify the library):
- Datasheet: https://sensing.honeywell.com/hih8000-datasheet-009075-7-en.pdf
- Installation instructions: https://sensing.honeywell.com/humidicon-sip-4-pin-versions-install-50085524-en-rev-b-final-30may13.pdf
- I2C communication with the sensor: https://sensing.honeywell.com/i2c-comms-humidicon-tn-009061-2-en-final-07jun12.pdf

# Instructions for use with Arduino boards
For an example, see the examples folder.

Setting up:
1. Connect the sensor to the appropriate Arduino pins using the recommended configuration (pg 10 of the datasheet)
2. Include Wire.h and HIH8000_I2C.h at the top of your Arduino code
3. Initialize a HIH8000_I2C object with the address of your sensor (default address is 0x27)
4. In the setup section of your Arduino code, call Wire.begin()

Getting measurements:
1. Call triggerMeasurement()
2. There needs to be a delay of at least 36.65 to 50 ms here
3. Call fetchMeasurement()
3. Now, you can get the status, humidity, or temperature values by calling getStatus(), getHumidity, or getTemperature(), respectively

Due to the delay requirement, it may be preferable to put triggerMeasurement() at the end of the loop, where there is usually a delay() function. Then call fetchMeasurement() at the start of the loop.

Please note that the pins used for I2C communication are different for each
Arduino board. For Arduino Uno, the SDA and SCL pins are A4 and A5, respectively.
A list of boards and corresponding pins can be found at:
https://www.arduino.cc/en/Reference/Wire

# How does the sensor work?
The sensor works in the following manner:
1. User triggers the sensor to perform a measurement.
2. Measurement is made and the raw data stored in the sensor. (The measurement process typically takes 36.65 ms. See pg3 of the third "useful documents" linked above.)
3. User fetches the raw data.
4. The raw data is processed by the user into humidity and temperature values (in this case, it is done by the fetchMeasurement() function of this library).

Compare this workflow to the "Getting measurements" instructions above. Due to the way the sensor works, you MUST trigger and fetch measurements if you want to get new humidity and temperature values. Otherwise, you will keep getting "old" humidity and temperature values.