#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>
#include <string.h>

#define BUZZER 25

float notes[] = {16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.97};


int getBaseNote(char *note){
  if(strcmp(note ,"c") == 0){
    return notes[0];
  }else if(strcmp(note , "#c") == 0){
    return notes[1];
  }else if(strcmp(note,"d") == 0){
    return notes[2];
  }else if(strcmp(note, "#d") ==0){
    return notes[3];
  }else if(strcmp(note, "e") == 0){
    return notes[4];
  }else if(strcmp(note, "f") == 0){
    return notes[5];
  }else if(strcmp(note, "#f") == 0){
    return notes[6];
  }else if(strcmp(note, "g") == 0){
    return notes[7];
  }else if(strcmp(note, "#g") == 0){
    return notes[8];
  }else if(strcmp(note ,"a") == 0){
    return notes[9];
  }else if(strcmp(note, "#a") == 0){
    return notes[10];
  }else{
    return notes[11];
  }
}

int getScaleNote(char *note, int scale){
  float currentNote = getBaseNote(note);
  int intCurrentNote = (int) currentNote;
  if((currentNote - intCurrentNote) > 0.5){
    intCurrentNote ++;
  }
  return intCurrentNote;
}

void playNote(char *note, int scale, int duration){
    softToneWrite (BUZZER, getScaleNote(note, scale));
    delay(duration);
    softToneWrite (BUZZER, 0);
}

int main (void)
{
  printf ("Raspberry Pi buzzer\n") ;
  wiringPiSetup () ;
  softToneCreate(BUZZER);
  int count = 100;
  char s[]="c";
  for (;;)
  {

    *s="d";
    playNote(s,1,3000);

    *s="e";
    playNote(s,1,3000);


    *s="f";
    playNote(s,1,3000);


    *s="g";
    playNote(s,1,3000);

    *s="a";
    playNote(s,1,3000);

    *s="b";
    playNote(s,1,3000);

  }
  return 0 ;
}
