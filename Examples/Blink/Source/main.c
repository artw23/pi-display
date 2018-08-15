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

#define BUTTON_0 21
#define BUTTON_1 22
#define BUTTON_2 23
#define BUTTON_3 24

enum mode{RUNNING, EDIT_DIGIT_0, EDIT_DIGIT_1, EDIT_DIGIT_2, EDIT_DIGIT_3};
int actualDigits[4] = {0,0,0,0};
int actualCount = 0;
enum mode actualMode = RUNNING;




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
    delay(5);
  }
}

PI_THREAD (displayThread){
  for(;;){
    displayNumber(actualCount);
  }
}

void waitForBounce(){
  delay(300);
}

void button0Interrupt(void){
  printf("Interrupt 0\n");
  waitForBounce();
  if(actualMode == RUNNING){
    actualMode = EDIT_DIGIT_0;
  }else{
    actualMode = RUNNING;
  }
}

void button1Interrupt(void){
  printf("Interrupt 1\n");
  waitForBounce();
  if(actualMode == RUNNING){
    return;
  }
  if(actualMode == EDIT_DIGIT_0){
    actualMode = EDIT_DIGIT_1;
  }else if(actualMode == EDIT_DIGIT_1){
    actualMode = EDIT_DIGIT_2;
  }else if(actualMode == EDIT_DIGIT_2){
    actualMode = EDIT_DIGIT_3;
  }else{
    actualMode = EDIT_DIGIT_0;
  }
}


int editCount(){
  return actualDigits[0] * 1000 + actualDigits[1] * 100 + actualDigits[2] * 10 + actualDigits[3];
}

void button2Interrupt(void){
  printf("Interrupt 2\n");
  waitForBounce();
  if(actualMode == RUNNING){
    return;
  }
  if(actualMode == EDIT_DIGIT_0){
    actualDigits[0] = (actualDigits[0] + 1) % 10;
  }else if(actualMode == EDIT_DIGIT_1){
      actualDigits[1] = (actualDigits[1] + 1) % 10;
  }else if(actualMode == EDIT_DIGIT_2){
      actualDigits[2] = (actualDigits[2] + 1) % 10;
  }else{
      actualDigits[3] = (actualDigits[3] + 1) % 10;
  }
  printf("Editing %i\n", editCount());
}

void button3Interrupt(void){
  printf("Interrupt 3\n");
  waitForBounce();
  if(actualMode == RUNNING){
    return;
  }
  if(actualMode == EDIT_DIGIT_0){
    int newDigit = (actualDigits[0] - 1);
    if(newDigit < 0){
      newDigit = 9;
    }
    actualDigits[0] = newDigit;
  }else if(actualMode == EDIT_DIGIT_1){
    int newDigit = (actualDigits[1] - 1);
    if(newDigit < 0){
      newDigit = 9;
    }
    actualDigits[1] = newDigit;
  }else if(actualMode == EDIT_DIGIT_2){
    int newDigit = (actualDigits[2] - 1);
    if(newDigit < 0){
      newDigit = 9;
    }
    actualDigits[2] = newDigit;
  }else{
    int newDigit = (actualDigits[3] - 1);
    if(newDigit < 0){
      newDigit = 9;
    }
    actualDigits[3] = newDigit;
  }
  printf("Editing %i\n", editCount());
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

  if ( wiringPiISR (BUTTON_0, INT_EDGE_FALLING, &button0Interrupt) < 0 ) {
      printf("Error creating interruption...\n");
      return 1;
  }

  if ( wiringPiISR (BUTTON_1, INT_EDGE_FALLING, &button1Interrupt) < 0 ) {
      printf("Error creating interruption...\n");
      return 1;
  }

  if ( wiringPiISR (BUTTON_2, INT_EDGE_FALLING, &button2Interrupt) < 0 ) {
      printf("Error creating interruption...\n");
      return 1;
  }

  if ( wiringPiISR (BUTTON_3, INT_EDGE_FALLING, &button3Interrupt) < 0 ) {
      printf("Error creating interruption...\n");
      return 1;
  }

  printf("Starting main loop...\n");
  int count = 0;
  int print = 0;
  actualCount = 100;
  piThreadCreate (displayThread) ;
  for (;;)
  {
    switch (actualMode) {
      case RUNNING:
        if(actualCount>0){
          actualCount --;
        }
        delay(1000);
      break;
      default:
        actualCount = editCount();
      break;
    }
  }
  return 0 ;
}
