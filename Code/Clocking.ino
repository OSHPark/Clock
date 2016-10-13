// Given Hour, Minutes, Seconds

int procHour(int arm) { 
 int lights = 0;
 if (arm == hour()) {
    lights = 100-(minute()*100)/60;
 }
 if (arm == (hour()+1) % 12) {
    lights = minute()*100/60;
 }
 return lights;
}

int milliseconds() {
  return (millis() - milli_offset) % 1000;
}

int procMinute(int arm) {
  int lights = 0;
  int minuteArm = (minute() * 2) / 10;
  int remainder = (minute() * 2) % 10;

  if (arm == minuteArm)     { lights = 100-remainder; }
  if (arm == minuteArm + 1) { lights = remainder;     }
  
 // lights += map(second(),0,60,0,10);
  return lights;
}

int procSecond(int arm) {
  int lights = 0;
  int secondArm = (second() * 2) / 10;
  int remainder = (second() % 5);
  int milli = map(milliseconds(),0,1000,0,20);
  remainder = map(remainder,0,5,0,100) + milli;
  
  if (arm == secondArm)     { lights = 100-remainder; }
  if (arm == secondArm + 1) { lights = remainder;     }

  return lights;
}

void updateClock(int arm, int h, int m, int s, int milli) {
  
  int reds[ledsPerArm+1];
  int greens[ledsPerArm+1];
  int blues[ledsPerArm+1];

  for (int i = 0; i<ledsPerArm; i++) {
    reds[i]   = 0;
    greens[i] = 0;
    blues[i]  = 0;
  }
  
  int hours   = map(h,0,100,0,ledsPerArm);
  int minutes = map(m,0,100,0,ledsPerArm);
  int seconds = map(s,0,100,0,ledsPerArm);

  // Set reds (hours)
  for (int i = 0 ; i<hours ; i++) {
    reds[i] = 0xFF;
  }
  // Set the final LED to a partial.
  reds[hours] = map(m,0,100,0,255);

  // Set greens (minutes)
  for (int i = 0 ; i<minutes; i++) {
    greens[i] = 0xFF;
  }
  greens[minutes] = map(s,0,100,0,255);

  // Set blues (seconds)
  for (int i = 0; i<seconds ; i++) {
    blues[i] = 0xFF;
  }
  //blues[seconds] = map(milli,0,1000,0,255);
  
  for (int i = 0; i<ledsPerArm; i++) {
    lightPixelOnArm(i,arm,reds[i],greens[i],blues[i]);
  }
}
