#include <stdio.h>
#include <wiringPi.h>


#define DIGIT_0 26
#define DIGIT_1 28
#define DIGIT_2 27
#define DIGIT_3 29

int digits[4] = {DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3};


#define SEGMENT_A 2
#define SEGMENT_B 0
#define SEGMENT_C 6
#define SEGMENT_D 5
#define SEGMENT_E 4
#define SEGMENT_F 7
#define SEGMENT_G 1

int SEGMENTS[7] = {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F, SEGMENT_G};

int nums[10][7] = { { 0,0,0,0,0,0,1 },    // 0
                    { 1,0,0,1,1,1,1 },    // 1
                    { 0,0,1,0,0,1,0 },    // 2
                    { 0,0,0,0,1,1,0 },    // 3
                    { 1,0,0,1,1,0,0 },    // 4
                    { 0,1,0,0,1,0,0 },    // 5
                    { 0,1,0,0,0,0,0 },    // 6
                    { 0,0,0,1,1,1,1 },    // 7
                    { 0,0,0,0,0,0,0 },    // 8
                    { 0,0,0,1,1,0,0 }};   // 9

int actualCount = 0;

PI_THREAD (displayThread){
  for(;;){
    displayNumber(actualCount);
  }
}

void displaySingleNum(int num){
  int *actualNum = nums[num];
  int i;
  for(i = 0; i < 7; i++){
    digitalWrite (SEGMENTS[i], actualNum[i]);
  }
}

void enableDigit(int digit_number){
  int i;
  for(int i =0; i< 4; i++){
    if(digit_number % 4 == i){
      digitalWrite (digits[i], HIGH);
    }else{
      digitalWrite (digits[i], LOW);
    }
  }
}

void displayNumber(int number){
	int i, j;
  for(i = 3; i >= 0 ; i --){
    int toDisplay = number%10;
    number = number /10;
    enableDigit(i);
    displaySingleNum(toDisplay);
    delay(3);
  }
}


void initDisplays(){
		pinMode (DIGIT_0, OUTPUT) ;
		pinMode (DIGIT_1, OUTPUT) ;
		pinMode (DIGIT_2, OUTPUT) ;
		pinMode (DIGIT_3, OUTPUT) ;

		pinMode (SEGMENT_A, OUTPUT) ;
		pinMode (SEGMENT_B, OUTPUT) ;
		pinMode (SEGMENT_C, OUTPUT) ;
		pinMode (SEGMENT_D, OUTPUT) ;
		pinMode (SEGMENT_E, OUTPUT) ;
		pinMode (SEGMENT_F, OUTPUT) ;
		pinMode (SEGMENT_G, OUTPUT) ;


		digitalWrite (DIGIT_0, HIGH) ;	// On
		digitalWrite (DIGIT_1, HIGH) ;	// On
		digitalWrite (DIGIT_2, HIGH) ;	// On
		digitalWrite (DIGIT_3, HIGH) ;	// On
}

int main (void)
{
  printf ("Raspberry Pi blink\n") ;

  wiringPiSetup () ;
	initDisplays();

  printf("Starting main loop...\n");
  int count = 0;
  int print = 0;
  actualCount = 100;
  piThreadCreate (displayThread) ;
  for (;;)
  {
    actualCount --;
    delay(1000);
  }
  return 0 ;
}
