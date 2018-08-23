#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define BUZZER 25

const int DEBUG = 1;

const float notes[] = {16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.97};

const char * HIGGHWAY_TO_HELL = "8e1 4- 4- 4- 4- 4d2 4d2 8d2 4- 4- 4- 4- 4#c2 4#c2 8#c2 4- 4- 4- 4- 4g2 4e2 4d2 4b1 4b1 4a1 4g1 8e1";

char * ACTUAL_SONG = "8e1 4- 4- 4- 4- 4d2 4d2 8d2 4- 4- 4- 4- 4#c2 4#c2 8#c2 4- 4- 4- 4- 4g2 4e2 4d2 4b1 4b1 4a1 4g1 8e1";

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

int getScaleNote(char *note, int scale){
  float currentNote = getBaseNote(note) * (scale +2);
  int intCurrentNote = (int) currentNote;
  if((currentNote - intCurrentNote) > 0.5){
    intCurrentNote ++;
  }
  return intCurrentNote;
}

void playNote(char *note, int scale, int duration){
    if(DEBUG != 0){
      printf("Playing note %s for %d\n", note, duration);
    }
    softToneWrite (BUZZER, getScaleNote(note, scale));
    int realDelay = 1000/duration;
    delay(realDelay);
    softToneWrite (BUZZER, 0);
}

int main (void)
{
  printf ("Raspberry Pi buzzer\n") ;
  wiringPiSetup () ;
  softToneCreate(BUZZER);



  char **arr = NULL;

  int c = split(ACTUAL_SONG, ' ', &arr);
  int i;
  printf("found %d tokens.\n", c);

  for (i = 0; i < c; i++){
    printf("string #%d: %s\n", i, arr[i]);
    char str [4];
    int scale, duration;
    sscanf(arr[i],"%d%2[#a-z]%d", &duration, str, &scale);
    printf("DURATION: '%d'  NOTE: '%s' SCALE: '%d'\n",duration,str,scale);
    playNote(str,scale,duration);
  }


  return 0 ;
}
