/*This is the code for the catfeeders "brains"
* Gets real time data, hosts UI-website and pulls the output pin high when set time is reached.
* Feeder has 2 timeslots since Tuittu gets kibble twice a day and I also added some complementary features such erase, now and skip
* Beware that the website is written in finnish so you may need to adjust that.
*/

#include <WiFi.h>
#include <WebServer.h>
#include <time.h>

#define WAIT 0
#define NORMAL 1
#define NOALARM 2
#define SKIP 3

//You cannot use my wifi sorry
const char* ssid = "";
const char* password = "";

//port
WebServer server(80);

//Time
String time1 = "";
String time2 = "";
int alarmHour1 = -1, alarmMin1 = -1;
int alarmHour2 = -1, alarmMin2 = -1;
//Flags
bool alarmTrigg = true; //makes sure that alarm does not repeat for the whole minute
bool nowFlag = false;
bool skipFlag = false;
// GPIO-pins
const int outputPin = 25;
const int g = 14;
const int r = 12;
const int b = 13;
const int alarmLed1 = 27;
const int alarmLed2 = 26;
const int piezoPin = 33;
//tunes
const int D4 = 294;
const int G4 = 392;
long piezoTimer = 0;
bool tuneStart = false;
int piezoStep = 0;


void setup() {
  //Serial.begin(115200);           Use the find and replace tool to if need be to debug

  pinMode(outputPin, OUTPUT);
  pinMode(r,OUTPUT);
  pinMode(g,OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(alarmLed1,OUTPUT);
  pinMode(alarmLed2,OUTPUT);

  digitalWrite(outputPin, LOW);

  statusLed(WAIT);
  setupWiFiAndTime(); // this takes care the wifi and also the time sync
  setupServer(); // this take care of the UI
  server.begin();
  statusLed(NORMAL);
  //Serial.println("Web-server online");
}

void loop() {
  server.handleClient();
  bool alm1 = isAlarm1Set(); // these just check if the  min and hour values are not -1
  bool alm2 = isAlarm2Set();
  digitalWrite(alarmLed1,alm1); 
  digitalWrite(alarmLed2,alm2);

  if(nowFlag){
    setAlarm();
    nowFlag = false;
    alarmTrigg =true;
  }

  if (alm1 || alm2) {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    int currentHour = timeinfo->tm_hour;
    int currentMin = timeinfo->tm_min;
    
    if ((currentHour == alarmHour1 && currentMin == alarmMin1) ||
        (currentHour == alarmHour2 && currentMin == alarmMin2)) {
      
      if(alarmTrigg){
        alarmTrigg = false;
        if(!skipFlag){
          setAlarm();
        }else{
          skipFlag = false;
          statusLed(NORMAL);
        }
      }
      
    }else{
      alarmTrigg = true;
    }
  }
  
  if(!skipFlag){ // if skip -> purple, if not then green if there is alarms or blue if not
    if(alm1 || alm2){
      statusLed(NORMAL);
    }else{
      statusLed(NOALARM);
    }
  }else{
    statusLed(SKIP);
  }
  if(tuneStart) alarmTune(); //non-blocking way to play some tunes
}
