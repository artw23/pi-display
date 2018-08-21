#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>

#define BUZZER 25

int notes[] = {1635, 1732, 1835, 1945, 2060, 2183, 2312, 2450, 2596, 2750, 2914, 3097};


int charToSound(char *check){
  if(strcmp(check ,'c') == 0){
    return notes[0];
  }else if(strcmp(check , '#c') == 0){
    return notes[1];
  }else if(strcmp(check,'d') == 0){
    return notes[2];
  }else if(strcmp(check, '#d') ==0){
    return notes[3];
  }else if(strcmp(check, 'e') == 0){
    return notes[4];
  }else if(strcmp(check, 'f') == 0){
    return notes[5];
  }else if(strcmp(check, '#f') == 0){
    return notes[6];
  }else if(strcmp(check, 'g') == 0){
    return notes[7];
  }else if(strcmp(check, '#g') == 0){
    return notes[8];
  }else if(strcmp(check ,'a') == 0){
    return notes[9];
  }else if(strcmp(check, '#a') == 0){
    return notes[10];
  }else{
    return notes[11];
  }
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
    delay(3000);
    softToneWrite (BUZZER, charToSound(s)/100);

    *s="d";
    delay(3000);
    softToneWrite (BUZZER, charToSound(s)/100);

    *s="e";
    delay(3000);
    softToneWrite (BUZZER, charToSound(s)/100);

    *s="f";
    delay(3000);
    softToneWrite (BUZZER, charToSound(s)/100);

    *s="g";
    delay(3000);
    softToneWrite (BUZZER, charToSound(s)/100);

    *s="a";
    delay(3000);
    softToneWrite (BUZZER, charToSound(s)/100);

    *s="b";
    delay(3000);
    softToneWrite (BUZZER, charToSound(s)/100);
  }
  return 0 ;
}
