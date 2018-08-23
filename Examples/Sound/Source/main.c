#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define BUZZER 25

const int DEBUG = 1;


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

const float notes[] = {16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.97};

#define BACK_IN_BLACK "8e1 4- 4- 4- 4- 4d2 4d2 8d2 4- 4- 4- 4- 4#c2 4#c2 8#c2 4- 4- 4- 4- 4g2 4e2 4d2 4b1 4b1 4a1 4g1 8e1"

#define MISION_IMPOSIBLE "16g2 8- 16g2 8- 16f2 16- 16#f2 16- 16g2 8- 16g2 8- 16#a2 16- 16c3 16- 16g2 8- 16g2 8- 16f2 16- 16#f2 16- 16g28- 16g2 8- 16#a2 16- 16c3 16- 16#a2 16g2 2d2 32- 16#a2 16g2 2#c2 32- 16#a2 16g2 2c2 16- 16#a1 16c2"

#define MAMA_MIA "32f2 32#d2 32f2 8#d2 32#d2 32#d2 32f2 32g2 32f2 16.#d2 32- 16f2 8#d2 16#g2 32#g2 32#g2 32#g2 16g2 16#d232- 8#a2 32#a2 32#a2 16#a2 16f2 16g2 8#g2 16g2 16g2 32g2 16g2 16d2 16#d2 8f2 16f2 8#d2 16#g2 32#g2 32#g232#g2 32g2 32#d2 32f2 16#d2"

#define HIGHWAY_TO_HELL "4#d1 4#d1 8#d1 4- 4- 4- 4- 4c1 4c1 8#c1 4- 4- 4- 4- 4c1 4c1 2#c1 4c1 4c1 8#c1 8c1 4#d1 8#d1"

#define NOKIA "8e2 8d2 4#f2 4#f2 4e2 4d2 2#f2 8d2 8e2 4d2 2d2 8a1 8b1 4d2 4e2 4#c2 4b1 8e2 8d2 4#f2 4#f2 4e2 4d2 2#f2 8d2 8e2 4d2 2d2 8a1 8b1 4d2 4e2 4#c2 4b1 8a1 8a1"
char * ACTUAL_SONG = MISION_IMPOSIBLE;

void displaySingleNum(int num){
  int *actualNum = nums[num];
  int i;
  for(i = 0; i < 7; i++){
    digitalWrite (segments[i], actualNum[i]);
  }
}

// Enable one of the 4 displays
void enableAll(){
  int i;
  for(i =0; i< 4; i++){
    digitalWrite (digits[i], HIGH);
  }
}


void disableAll(){
  int i;
  for(i =0; i< 4; i++){
    digitalWrite (digits[i], LOW);
  }
}

int split (const char *str, char c, char ***arr)
{
    int count = 1;
    int token_len = 1;
    int i = 0;
    char *p;
    char *t;

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
            count++;
        p++;
    }

    *arr = (char**) malloc(sizeof(char*) * count);
    if (*arr == NULL)
        exit(1);

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
        {
            (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
            if ((*arr)[i] == NULL)
                exit(1);

            token_len = 0;
            i++;
        }
        p++;
        token_len++;
    }
    (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
    if ((*arr)[i] == NULL)
        exit(1);

    i = 0;
    p = str;
    t = ((*arr)[i]);
    while (*p != '\0')
    {
        if (*p != c && *p != '\0')
        {
            *t = *p;
            t++;
        }
        else
        {
            *t = '\0';
            i++;
            t = ((*arr)[i]);
        }
        p++;
    }

    return count;
}

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
  }else if(strcmp(note, "b") == 0){
    return notes[11];
  }else{
    return 0;
  }
}

//Custom implementation of an exponential function
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

int getScaleNote(char *note, int scale){
  int offset = 3 ;
  if(scale >= 0 && scale < 5){
    scale = scale + offset;
    float currentNote = getBaseNote(note) * myPow(2,scale);
    if(DEBUG != 0){
      printf("Playing frequency %f\n", currentNote);
    }
    int intCurrentNote = (int) currentNote;
    if((currentNote - intCurrentNote) > 0.5){
      intCurrentNote ++;
    }
    return intCurrentNote;
  }
  return 0;
}

void playNote(char *note, int scale, int duration){
    if(DEBUG != 0){
      printf("Playing note %s for %d\n", note, duration);
    }
    softToneWrite (BUZZER, getScaleNote(note, scale));
    int realDelay = 2000/duration;
    disableAll();
    delay(realDelay);
    enableAll();
}

void stop(){
  softToneWrite (BUZZER, 0);
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
}

int main (void)
{
  printf ("Raspberry Pi buzzer\n") ;
  wiringPiSetup () ;
  softToneCreate(BUZZER);
  initDisplays();
  displaySingleNum(0);

  char **arr = NULL;

  int c = split(ACTUAL_SONG, ' ', &arr);
  int i;
  printf("found %d tokens.\n", c);
  char * testNote = "a";

  for (i = 0; i < c; i++){
    printf("string #%d: %s\n", i, arr[i]);
    int length = strlen(arr[i]);

    char str [4];
    int scale, duration;
    if(length < 3){
      sscanf(arr[i],"%d%s", &duration, str);
      scale = 0;
    }else{
      sscanf(arr[i],"%d%2[#a-z]%d", &duration, str, &scale);
    }
    printf("DURATION: '%d'  NOTE: '%s' SCALE: '%d'\n",duration,str,scale);
    playNote(str,scale,duration);
  }
  stop();

  return 0 ;
}
