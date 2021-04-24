#include <Steelsquid.h>
#include <FeatherwingHttpServer.h>

#define HTML "<!DOCTYPE html> <html lang=en> <head> <title>LoRa Cat Tracker</title> <style>html,body{height:100%;width:100%;margin:0;font:1.5em Arial}table,td{padding:0}#head{height:2.7em}#info{vertical-align:top color:red}.key{text-align:right;padding-right:.4em}#map{position:absolute;top:2.7em;bottom:0;width:100%}</style> <script>var map,m,$=function(a){return document.getElementById(a)};function re(){setTimeout(function(){get()},1E3)}function initMap(){map=new google.maps.Map($(\"map\"),{zoom:20,streetViewControl:!1,fullscreenControl:!1,mapTypeControl:!1,mapTypeId:\"satellite\"});re()}function get(){fetch(\"d\",{cache:\"no-store\"}).then(function(a){if(a.ok)return a.text();throw Error(a.statusText);}).then(function(a){if(a.length>10){a=a.split(\"|\");$(\"bat\").innerHTML=a[0]+\"%\";$(\"time\").innerHTML=a[1]+\"s\";$(\"stre\").innerHTML=a[2];$(\"rssi\").innerHTML=a[3];var b=parseFloat(a[4]),c=parseFloat(a[5]);if(0!=b&&0!=c&&b&&c)a={lat:parseFloat(a[4]),lng:parseFloat(a[5])},m&&map.getBounds()?(m.setPosition(a),map.getBounds().contains(m.getPosition())||map.panTo(a)):(m=new google.maps.Marker({position:a,map:map}),map.panTo(a));$(\"info\").innerHTML=\"\";$(\"data\").style.display=\"block\";$(\"info\").style.display=\"none\"}re()}).catch(function(a){$(\"info\").innerHTML=a;$(\"data\").style.display=\"none\";$(\"info\").style.display=\"block\";re()})};</script> </head> <body> <div id=head> <div id=info> </div> <table id=data> <tr> <td class=key>Battery:</td><td id=bat>---</td><td class=key style=padding-left:4em>Signal:</td><td id=stre>---</td> </tr> <tr> <td class=key>Last lock:</td><td id=time>---</td><td class=key>RSSI:</td><td id=rssi>---</td> </tr> </table> </div> <div id=map></div> <script src=\"https://maps.googleapis.com/maps/api/js?key=<YoutGoogleMapKey>&callback=initMap&libraries=&v=weekly\" async></script> </body> </html>"

FeatherwingHttpServer httpServer;
String buffer = "";
String response = "";
unsigned long lastLock = -1;

//##########################################################################################
void setup() {
  Steelsquid::init();
  pinMode(8, INPUT_PULLUP);
  httpServer.init();

  Serial1.begin(19200);
  delay(100);
}


//##########################################################################################
void loop() {
  if(httpServer.hasRequest()){
    String request = httpServer.request();
    if(request == "/"){
      httpServer.responseHtml(HTML);
    }
    else if(request == "/d"){
      if(lastLock == -1){
        httpServer.responseText(" ");
      }
      else{
        double t = (double)(millis()-lastLock) / (double)1000;
        if(t>999){
          t=999;
        }
        String send = response;
        send.replace("?", String(t, 1));
        httpServer.responseText(send);
      }
    }
    else{
      httpServer.responseNotFound();
    }
  }
  if (Serial1.available()) {
    char c = Serial1.read();
    if(c != -1){
      if(c == '\n'){
        if(buffer[0] != '0'){
          lastLock = millis();
        }
        response = buffer;
        buffer = "";
      }
      else if(buffer.length()>40){
        buffer = "";
      }
      else{
        buffer.concat(c);
      }
    }
  }
}
