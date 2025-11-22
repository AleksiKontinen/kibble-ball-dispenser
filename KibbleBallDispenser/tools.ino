
void statusLed(int i){
  if( i < 0 || i > 3) return;
  digitalWrite(r,LOW);
  digitalWrite(g,LOW);
  digitalWrite(b,LOW);
  switch(i){
    case 0:
      digitalWrite(r,HIGH);
      break;
    case 1:
      digitalWrite(g,HIGH);
      break;
    case 2:
      digitalWrite(b,HIGH);
      break;
    case 3:
      digitalWrite(r,HIGH);
      digitalWrite(b,HIGH);
      break;
  }

}

void setAlarm(){
  //Serial.println("Alarm time reached"); 
  statusLed(WAIT);
  digitalWrite(outputPin,HIGH);
  delay(200);
  digitalWrite(outputPin,LOW);
  tuneStart = true;

}
bool isAlarm1Set(){
  return (alarmHour1 != -1 && alarmMin1 != -1);
}
bool isAlarm2Set(){
  return (alarmHour2 != -1 && alarmMin2 != -1);
}

void alarmTune(){

  
  if(millis() - piezoTimer >= 500){
    piezoTimer = millis();
    tone(piezoPin,2000 + (piezoStep % 3) * 1000,200);
    piezoStep++;

  }
  if(piezoStep >= 9){
    tuneStart = false;
    piezoStep = 0;
  }

}


