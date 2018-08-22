#include <stdio.h>
#include <wiringPi.h>

//Pins that enable each one of the 4 displays
const int digits[4] =
{
  26, // Enable digit 0
  28, // Enable digit 1
  27, // Enable digit 2
  29  // Enable digit 3
};

//Pins for the segments of the display

const int segments[7] =
{
  2, // Segment A
  0, // Segment B
  6, // Segment C
  5, // Segment D
  4, // Segment E
  7, // Segment E
  1  // Segment F
};

// Values for each segment to represent the numbers from 0 to 9.

const int nums[10][7] = { { 0,0,0,0,0,0,1 },    // 0
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

enum mode{
  RUNNING,
  CONFIG,
  EDIT_DIGIT_0, EDIT_DIGIT_1, EDIT_DIGIT_2, EDIT_DIGIT_3
};

int digitToEdit = 0;
int actualCount = 0;
enum mode actualMode = RUNNING;




void displaySingleNum(int num){
  int *actualNum = nums[num];
  int i;
  for(i = 0; i < 7; i++){
    digitalWrite (segments[i], actualNum[i]);
  }
}

void enableDigit(int digit_number){
  int i;
  for(i =0; i< 4; i++){
    if(digit_number % 4 == i){
      digitalWrite (digits[i], HIGH);
    }else{
      digitalWrite (digits[i], LOW);
    }
  }
}

void displayNumber(int number){
	int i;
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

PI_THREAD (timer){
  for(;;){
    if(actualMode == RUNNING){
      if(actualCount>0){
        actualCount --;
      }
      delay(1000);
    }
  }
}

int myPow(int number, int exponential){
  int i;
  if(exponential == 0){
    return 1;
  }
  int output = number;
  for(i =1; i <= exponential; i++){
    output = output * number;
  }
  return output;
}

void waitForBounce(){
  delay(50);
}



void button0Interrupt(void){
  printf("Interrupt 0: PAUSE/RUNNING\n");
  waitForBounce();
  if(actualMode == RUNNING){
    actualMode = CONFIG;
    actualCount = 0;
    digitToEdit = 0;
  }else{
    actualMode = RUNNING;
  }
}

void button1Interrupt(void){
  printf("Interrupt 1: Changing Digit\n");
  waitForBounce();
  if(actualMode == CONFIG){
    digitToEdit++;
    if(digitToEdit >=4){
      digitToEdit = 0;
    }
  }
}

void button2Interrupt(void){
  printf("Interrupt 2: Adding number\n");
  waitForBounce();

  int multiplier = myPow(10, digitToEdit);
  int count = actualCount;
  int currentDigit = (count / multiplier) % 10;
  count = count - currentDigit * multiplier;
  currentDigit = (currentDigit + 1);
  if(currentDigit > 9){
    currentDigit = 0;
  }
  count = count + currentDigit * multiplier;
  actualCount = count;
}

void button3Interrupt(void){
  printf("Interrupt 3: Substracting number\n");
  waitForBounce();
  int multiplier = myPow(10, digitToEdit);
  int count = actualCount;
  int currentDigit = (count / multiplier) % 10;
  count = count - currentDigit * multiplier;
  currentDigit = (currentDigit - 1);
  if(currentDigit < 0){
    currentDigit = 9;
  }
  count = count + currentDigit * multiplier;
  actualCount = count;
}


void initDisplays(){
    int i, total;

    // Init enable displays
    total = sizeof(digits)/sizeof(digits[0]);
    for(i =0; i < total; i++){
      pinMode (digits[i], OUTPUT) ;
      digitalWrite (digits[i], HIGH) ;	// On
    }

    // Init segments of diplays
    total = sizeof(segments)/sizeof(segments[0]);
    for(i =0; i < total; i++){
      pinMode (segments[i], OUTPUT) ;

    }

    // Init imputs buttons
    pinMode (BUTTON_0, INPUT) ;
    pinMode (BUTTON_1, INPUT) ;
    pinMode (BUTTON_2, INPUT) ;
    pinMode (BUTTON_3, INPUT) ;
}


int initInterrupts(){
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
  return 0;
}
void initThreads(){
  piThreadCreate (displayThread);
  piThreadCreate (timer);
}

int main (void)
{
  printf ("Raspberry Pi blink\n") ;

  wiringPiSetup();
	initDisplays();
  initInterrupts();
  initThreads();

  printf("Starting main loop...\n");
  actualCount = 100;
  for (;;);
  return 0 ;
}
