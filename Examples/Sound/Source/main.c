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

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
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
  float currentNote = getBaseNote(note);
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

  char** notes = str_split(ACTUAL_SONG, ' ');

  printf("checking notes...");
  if (notes)
  {
      int i;
      for (i = 0; *(notes + i); i++)
      {
        printf("%s\n", *(notes + i));
        char str [4];
        int scale, duration;
        sscanf(*(notes + i),"%d%s%d", &duration, str, &scale);
        printf("NOTE: %d%s%d\n",duration,str,scale);
      }
  }
  for (;;)
  {

    char *s1="d";
    playNote(s1,1,3000);

    char *s2="e";
    playNote(s2,1,3000);


    char *s3 ="f";
    playNote(s3,1,3000);

  }
  return 0 ;
}
