

void setupWiFiAndTime() {
  IPAddress local_IP(192, 168, 100, 4);
  IPAddress gateway(192, 168, 100, 1);
  IPAddress subnet(255, 255, 255, 0); 
  IPAddress dns(192, 168, 100, 1);
  WiFi.config(local_IP, gateway, subnet, dns);

  WiFi.begin(ssid, password);
  //Serial.print("Connecting to Wifi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  
  //Serial.println("\nConnected");
  //Serial.print("IP-address: ");
  //Serial.println(WiFi.localIP());

  // NTP time 
  configTime(0, 0, "pool.ntp.org");
  setenv("TZ", "EET-2EEST,M3.5.0/3,M10.5.0/4", 1);
  tzset();
  //Serial.print("Sync");
  while (time(nullptr) < 100000) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("\nDone");
}

void setupServer() {
  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.on("/now",HTTP_GET, handleNow);
  server.on("/reset",HTTP_GET, handleReset);
  server.on("/skip", HTTP_GET, handleSkip);
}

void handleRoot() { 

  String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'><title>Tuitun ruokitin 6000</title></head><body>";
  html += "<h2>Tuitun ruokitin 6000</h2>";

  html += "<form action='/set'>";
  html += "Aseta aika 1 (HH:MM): <input type='time' name='time1' value='" + time1 + "'><br>";
  html += "Aseta aika 2 (HH:MM): <input type='time' name='time2' value='" + time2 + "'><br><br>";
  html += "<input type='submit' value='Aseta'>";
  html += "</form><br>";

  html += "<form action='/now'>";
  html += "<input type='submit' value='NYT'>";
  html += "</form><br><br>";

  html += "<form action='/reset'>";
  html += "<input type='submit' value='Nollaa'>";
  html += "</form><br>";

  html+= "<form action='/skip'>";
  html+= "<input type='submit' value='Ohita'>";
  html+= "</form>";

  html += "</body></html>";
  server.send(200, "text/html", html);

}
void handleNow() {
  nowFlag = true;
  server.sendHeader("Location", "/");
  server.send(303);
}


void handleReset() {
  time1 = "";
  time2 = "";
  alarmHour1 = alarmMin1 = -1;
  alarmHour2 = alarmMin2 = -1;
  skipFlag = false;
  server.sendHeader("Location", "/");
  server.send(303);
  tone(piezoPin, 900,100);
  delay(100);
  tone(piezoPin,300,100);
}

void handleSet() {
  if (server.hasArg("time1")) {
    time1 = server.arg("time1");
    if (time1.length() == 5) {
      alarmHour1 = time1.substring(0, 2).toInt();
      alarmMin1 = time1.substring(3, 5).toInt();
    }
  }
  if (server.hasArg("time2")) {
    time2 = server.arg("time2");
    if (time2.length() == 5) {
      alarmHour2 = time2.substring(0, 2).toInt();
      alarmMin2 = time2.substring(3, 5).toInt();
    }
  }
  server.sendHeader("Location", "/");
  server.send(303);
  tone(piezoPin, 300,100);
  delay(100);
  tone(piezoPin,900,100);
}

void handleSkip(){

  skipFlag = !skipFlag;
  server.sendHeader("Location", "/");
  server.send(303);
  if(skipFlag){
    tone(piezoPin,300,100);
  }else{
    tone(piezoPin,900,100);
  }
  
}
