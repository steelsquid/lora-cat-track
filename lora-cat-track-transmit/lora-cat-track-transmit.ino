#include <Steelsquid.h>
#include <FeatherwingGPS.h>
#include <FeatherRfm9X.h>

#define FREQUENCY 434.0
#define DEVICE_ADDRESS 66
#define TO_ADDRESS 67

FeatherwingGPS gps;
FeatherRfm9X rfm9X = FeatherRfm9X(FREQUENCY, TP_23, BW_125_0, SF_10, CR_4_8);

byte buffer[17];


//##########################################################################################
void setup() {
  Steelsquid:init();
  gps.init();
  rfm9X.init(DEVICE_ADDRESS, TO_ADDRESS);
}


//##########################################################################################
void loop() {
  if(gps.hasLock()){
    double latiF = gps.latitude();
    double longiF = gps.longitude();
    byte *latitude = (byte *)&latiF;
    byte *longitude = (byte *)&longiF;
    for(int i=0; i<8; i++){
      buffer[i] = latitude[i];
      buffer[i+8] = longitude[i];
    }
    buffer[16] = FeatherRfm9X::batteryPercent();
    if(Steelsquid::executeEvery(0, 2000)){
      rfm9X.transmitBytes(buffer, 17);
    }
  }
  else if(Steelsquid::executeEvery(1, 2000)){
    for(int i=0; i<8; i++){
      buffer[i] = 0;
      buffer[i+8] = 0;
    }
    buffer[16] = FeatherRfm9X::batteryPercent();
    rfm9X.transmitBytes(buffer, 17);
  }
}
