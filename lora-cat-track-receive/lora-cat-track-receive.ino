#include <Steelsquid.h>
#include <FeatherRfm9X.h>
#include <FeatherwingHttpServer.h>

#define FREQUENCY 434.0
#define DEVICE_ADDRESS 67
#define TO_ADDRESS 66


FeatherRfm9X rfm9X = FeatherRfm9X(FREQUENCY, TP_23, BW_125_0, SF_10, CR_4_8);

//##########################################################################################
void setup() {
  Steelsquid::init();
  rfm9X.init(DEVICE_ADDRESS, TO_ADDRESS);

  Serial1.begin(19200);
  delay(100);
}


//##########################################################################################
void loop() {
  if(rfm9X.available()){
    int packageType = rfm9X.receive();
    if(packageType == TYPE_BYTES && rfm9X.lastRxLength()==17){
      byte* buffer = rfm9X.receivedBytes();
      double latitude = Steelsquid::toDouble(buffer, 0);
      double longitude = Steelsquid::toDouble(buffer, 8);

      String bat = String(buffer[16]);
      String snr = String(rfm9X.lastSnr());
      String rssi = String(rfm9X.lastRssi());
      String lat = String(latitude, 7);
      String lon = String(longitude, 7);
      sendToWebServer(bat, snr, rssi, lat, lon);
    }    
  }
}


//##########################################################################################
void sendToWebServer(String bat, String snr, String rssi, String lat, String lony) {
    Serial1.print(bat);
    Serial1.print('|');
    Serial1.print('?');
    Serial1.print('|');
    Serial1.print(snr);
    Serial1.print('|');
    Serial1.print(rssi);
    Serial1.print('|');
    Serial1.print(lat);
    Serial1.print('|');
    Serial1.print(lony);
    Serial1.print('\n');
    Serial1.flush();
    Serial.println("kjhkjh");
}
