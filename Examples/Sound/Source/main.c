#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>

#define BUZZER 25





int main (void)
{
  printf ("Raspberry Pi buzzer\n") ;
  wiringPiSetup () ;
  softToneCreate (BUZZER) ;
  int count = 100;
  for (;;)
  {
    delay(3000);
    softToneWrite (BUZZER, count) ;
    count = count + 100;
    printf("Frequency: %i\n",count);
  }
  return 0 ;
}
