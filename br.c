/* --------------------------------------------------------------------
 Biorhythmus (c) by Frank Kaefer 1987-2000 V5.0
 History:
     V1.0  ZX-80 Basic (ZX-80 with 16 kB RAM)
     V2.0  TRS-80 Basic (Video Genie)
     V2.1  C-64 Basic
     V2.2  OS-9 Basic (OS-9/68000 on Atari ST)
     V3.0  GW-Basic
     V3.0a VMS-Basic             (brennan@cchs.su.edu.au 02-Feb-1994)
     V4.0  C-language conversion (brennan@cchs.su.edu.au 04-Feb-1994)
     V5.0  Y2K and leapyear problem fixed (fkk)
*/

/*
  Compilation:
        SunOS 5.X and
        SunOS 4.X     % cc  br br.c -lm
                 or   % gcc br br.c -lm

        VMS 5.5-2     $ cc/Standard=PORTABLE/Object=br.obj br.c
                 or   $ gcc/Warning/Standard=PORTABLE/Object=br.obj br.c

                (ld)  $ LINK br,Sys$Input:/OPTION
                      Sys$Share:VaxCrtl.exe/Share
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifdef VMS
#include <ssdef.h>
#define SUCCESS  SS$_NORMAL
#define ERROR    SS$_BADPARAM
#else
#define SUCCESS  0
#define ERROR    1
#endif

#define YES (1 == 1)
#define NO  (1 == 0)

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define PRINTFILE "biorythm.txt"
#define MAXBUF 20
#define MAXSTRING 1000
#define MAXGRAPH 51

time_t now;
static struct tm *presenttime;
int t_dd,t_mm,t_yy;

FILE *fd;

int DRUCKER;                        /* PRINTER */

char dtmp[MAXBUF];
char ausgabe[MAXBUF];
char ausgabeart[MAXBUF];        /* which type of display */
char Cktage[MAXBUF];

int t,m,j;
int th,mh,jh;
int tg,mg,jg;
int ta,tah,tag,gtag,tage;
int phys,emot,geist;
int p,e,g;

int ktage;
int pl,cl,z;

char graph[MAXGRAPH];

/* define the 'clone' routines of the BASIC functions/gosubs from original */
void INPUT();
char *LEFT();
char *MID();
char *RIGHT();
void Vergangene_Tage();
void Kopf();

/* --------------------------------------------------------------------
   Daten aus den Datazeilen lesen [read data from data lines (aka BASIC)]
   -------------------------------------------------------------------- */
static char *WOCHENTAG[] =
{
  "Sunday","Monday","Tuesday","Wednesday","Thursday",
    "Friday","Saturday"
  };

/* static */ int MONATSTAG[12] =
{
  31,28,31,30,31,30,31,31,30,31,30,31
  };



int main()
{

  /* Start
     -------------------------------------------------------------------- */
  (void) printf("\nBiorhythm (c) 1987-2000 V5.0\n");

  /* --------------------------------------------------------------------
     Eingabe des heutigen Datums [enter todays date]
     -------------------------------------------------------------------- */
  (void) printf("\nDate     [<RETURN> for Systemdate] (DD.MM.YYYY) : ");

  INPUT(dtmp);

  if (strlen(dtmp) != 10)
  {                                /* get TODAY's date */
    now = time((time_t *) NULL);
    presenttime = localtime(&now);
    t_dd = presenttime->tm_mday;
    t_mm = presenttime->tm_mon + 1;
    t_yy = presenttime->tm_year;
    /* if (90 < t_yy && t_yy < 100) */
    /*   t_yy += 1900;              */  /* correct for century */
    /* if (t_yy < 90)               */
    /*   t_yy += 2000;              */
    t_yy += 1900;        /* localtime years = years since 1900 */
    (void) sprintf(dtmp,"%02d.%02d.%04d",t_dd,t_mm,t_yy);
  }

  t = atoi(LEFT(dtmp,2));
  m = atoi(MID(dtmp,4,2));
  j = atoi(RIGHT(dtmp,7));
  th = t;
  mh = m;
  jh = j;

  /* leapyear ? */
  if (t_yy % 4 == 0)
    MONATSTAG[1]=29;

  /* --------------------------------------------------------------------
     Eingabe des Geburtstages  [enter the birthdate]
     -------------------------------------------------------------------- */
  (void) printf("Birthday [<RETURN> for 04.12.1965] (DD.MM.YYYY) : ");

  INPUT(dtmp);

  if (strlen(dtmp) != 10)
    (void) strcpy(dtmp,"04.12.1965");

  t = atoi(LEFT(dtmp,2));
  m = atoi(MID(dtmp,4,2));
  j = atoi(RIGHT(dtmp,7));
  tg = t;
  mg = m;
  jg = j;

  Vergangene_Tage();                /*  GOSUB 1170  */

  ta -= ((double) (ta / 7)) * 7;
  gtag = ta;

  /* --------------------------------------------------------------------
     Abfrage, ob und wo Kurve oder Tageswerte ausgegeben werden sollen
     [ask if they want the curve or the day values and where to display it]
     -------------------------------------------------------------------- */
  (void) printf("\nOutput to screen or printer [<RETURN> for Screen] (s/p) ");

  INPUT(ausgabe);

  if (ausgabe[0] == 'p' || ausgabe[0] == 'P')
    DRUCKER = YES;
  else
    DRUCKER = NO;

  if (DRUCKER)
  {
    fd = fopen(PRINTFILE,"w");
    if (!fd)
    {
      perror(PRINTFILE);
      (void) exit(ERROR);
    }
  }

  (void) printf("Values for today or Graph   [<RETURN> for Graph]  (v/g) ");

  INPUT(ausgabeart);

  if (ausgabeart[0] == 'v' || ausgabeart[0] == 'V')
  {
    /* --------------------------------------------------------------------
       Berechnung der vergangenen Tage  [calc the past days]
       -------------------------------------------------------------------- */
    j = jg;
    m = mg;
    t = tg;

    Vergangene_Tage();                /*  GOSUB 1170  */

    tag = ta;
    j = jh;
    m = mh;
    t = th;

    Vergangene_Tage();                /*  GOSUB 1170  */

    tah = ta;
    tage = tah - tag;

    Kopf();                        /* GOSUB 2110 */
    if (!DRUCKER)
    {
      (void) printf("\nThe birthday %02d.%02d.%04d is a %s\n",tg,mg,jg,WOCHENTAG[gtag]);
      (void) printf("Age in days: %d\n",tage);
    }
    else
    {
      (void) fprintf(fd,"\nThe birthday %02d.%02d.%04d is a %s\n",tg,mg,jg,WOCHENTAG[gtag]);
      (void) fprintf(fd,"Age in days: %d\n",tage);
    }

    /* --------------------------------------------------------------------
       Berechnung der drei Werte fuer das angegebene Datum
       [calculate the 3 values for the given date]
       -------------------------------------------------------------------- */
    phys = 50 * (1 + sin((double) (((double) tage / 23) - (tage / 23)) * 360 * PI / 180));
    emot = 50 * (1 + sin((double) (((double) tage / 28) - (tage / 28)) * 360 * PI / 180));
    geist = 50 * (1 + sin((double) (((double) tage / 33) - (tage / 33)) * 360 * PI / 180));

    /* --------------------------------------------------------------------
       Ausgabe der Werte [display the values]
       -------------------------------------------------------------------- */
    if (!DRUCKER)
    {
      (void) printf("Biorhythm    %02d.%02d.%04d\n",th,mh,jh);
      (void) printf("Physical:    %d%%\n",phys);
      (void) printf("Emotional:   %d%%\n",emot);
      (void) printf("Mental:      %d%%\n\n",geist);
    }
    else
    {
      (void) fprintf(fd,"Biorhythm    %02d.%02d.%04d\n",th,mh,jh);
      (void) fprintf(fd,"Physical:    %d%%\n",phys);
      (void) fprintf(fd,"Emotional:   %d%%\n",emot);
      (void) fprintf(fd,"Mental:      %d%%\n\n",geist);
    }
  }
  else
  {
    /* --------------------------------------------------------------------
       Ausgabe einer Kurve  [display the curves]
       -------------------------------------------------------------------- */
    (void) printf("\nGraph for how many days : ");

    INPUT(Cktage);
    ktage = atoi(Cktage);
    ktage -= 1;

    /* --------------------------------------------------------------------
       Berechnung der vergangenen Tage  [calc the past days]
       -------------------------------------------------------------------- */
    j = jg;
    m = mg;
    t = tg;
    Vergangene_Tage();                /*  GOSUB 1170 */

    tag = ta;
    j = jh;
    m = mh;
    t = th;
    Vergangene_Tage();                /*  GOSUB 1170 */

    tah = ta;
    tage = tah - tag;
    Kopf();                        /* heading *//* GOSUB 2110 */


    if (!DRUCKER)
    {
      (void) printf("\nThe birthday %02d.%02d.%04d is a %s\n",tg,mg,jg,WOCHENTAG[gtag]);
      (void) printf("Age in days: %d\n",tage);
      (void) printf("\n               P=Physical, E=Emotional, M=Mental\n");
      (void) printf("\n               bad condition");

      (void) printf("                         good condition\n");
    }
    else
    {
      (void) fprintf(fd,"\nThe birthday %02d.%02d.%04d is a %s\n",tg,mg,jg,WOCHENTAG[gtag]);
      (void) fprintf(fd,"Age in days: %d\n",tage);
      (void) fprintf(fd,"\n               P=Physical, E=Emotional, M=Mental\n");
      (void) fprintf(fd,"\n               bad condition");
      (void) fprintf(fd,"                         good condition\n");
    }

    /* --------------------------------------------------------------------
       Schleife fuer die Ausgabe der Kurve
       [loop for the display of the curves]
       -------------------------------------------------------------------- */

    for (z = tage; z <= (tage + ktage); z++)
    {
      phys = 50 * (1 + sin((double) (((double) z / 23) - (z / 23)) * 360 * PI / 180));
      emot = 50 * (1 + sin((double) (((double) z / 28) - (z / 28)) * 360 * PI / 180));
      geist = 50 * (1 + sin((double) (((double) z / 33) - (z / 33)) * 360 * PI / 180));

      /* --------------------------------------------------------------------
         Ausgabe der Werte als Kurve [display the values as a curve]
         ------------------------------------------------------------------ */
      if (!DRUCKER)
      {
        (void) printf("%02d %02d %04d",th,mh,jh);
        (void) printf("     ");
      }
      else
      {
        (void) fprintf(fd,"%02d %02d %04d",th,mh,jh);
        (void) fprintf(fd,"     ");
      }
      phys = ((double) ((double) phys / 2) + .5);
      emot = ((double) ((double) emot / 2) + .5);
      geist = ((double) ((double) geist / 2) + .5);
      p = phys + 1;
      e = emot + 1;
      g = geist + 1;

      /* ------------------------------------------------------------------ */

      for (cl = 0; cl <= MAXGRAPH; cl++)
      {
        graph[cl] = '.';
      }

      graph[p] = 'P';
      graph[e] = 'E';
      graph[g] = 'M';
      graph[MAXGRAPH / 2] = '|';

      for (pl = 0; pl <= MAXGRAPH; pl++)
      {
        if (!DRUCKER)
          (void) printf("%c",graph[pl]);
        else
          (void) fprintf(fd,"%c",graph[pl]);
      }

      if (!DRUCKER)
        (void) printf("\n");
      else
        (void) fprintf(fd,"\n");

      /* ------------------------------------------------------------------ */
      th += 1;
      if (th > MONATSTAG[mh - 1])
      {
        mh += 1;
        th = 1;
      }
      if (mh > 12)
      {
        jh += 1;
        mh = 1;
        /* leapyear ? */
        if (jh % 4 == 0)
          MONATSTAG[1]=29;
        else
          MONATSTAG[1]=28;
      }
    }                                /* next Z */
  }
  if (!DRUCKER)
    (void) printf("\n");
  else
    (void) fclose(fd);

  return (SUCCESS);                /* END */
}



void Kopf()                        /* heading */
{
  /* 2110 subroutine */
  /* --------------------------------------------------------------------
     Kopf BIORHYTHMUS
     ------------------------------------------------------------------ */
  if (DRUCKER)
  {
    (void) fprintf(fd,"\n");
    (void) fprintf(fd,"                                BIORHYTHM\n");
    (void) fprintf(fd,"                                =========\n");
    (void) fprintf(fd,"\n                       (c) 1987-2000 by Frank Kaefer\n");
  }
  else
  {
    (void) printf("\n");
    (void) printf("                                BIORHYTHM\n");
    (void) printf("                                =========\n");
    (void) printf("\n                       (c) 1987-2000 by Frank Kaefer\n");
  }

  return;

}


void Vergangene_Tage()
{
  /* 1170 subroutine */
  /* --------------------------------------------------------------------
     Unterprogramm zur Berechnung der vergangenen Tage
     [subroutine to calculate the past days]
     -------------------------------------------------------------------- */
  ta = 0;
  if (m <= 2)
  {
    ta = (double) (m - 1) * 31;
  }
  else
  {
    if ((j % 4 == 0 && j % 100 != 0) || j % 400 == 0)
      ta = 1;
    else
      ta = 0;
    ta += ((double) (306 * m - 324) / 10);
  }
  ta += (double) (j - 1) * 365 + ((j - 1) / 4);
  ta -= (double) ((j - 1) / 100) - (double) ((j - 1) / 400);
  ta += t;

  return;
}

/* clone the BASIC functions LEFT$, MID$, RIGHT$ */

char *LEFT(source,numchars)
  char source[];
  int numchars;
{
  char result[MAXSTRING];
  int i;

  if (numchars > strlen(source))
    return (source);

  for (i = 0; i < numchars; i++)
    result[i] = source[i];

  result[i] = '\0';
  return (result);
}

char *MID(source,startpos,numchars)
  char source[];
  int startpos,numchars;
{
  char result[MAXSTRING];
  int i,j;

  if ((startpos + numchars) > strlen(source))
    return (source);

  for (i = startpos - 1,j = 0; i < (startpos + numchars) - 1; i++,j++)
    result[j] = source[i];

  result[j] = '\0';
  return (result);
}

char *RIGHT(source,startpos)
  char source[];
  int startpos;
{
  char result[MAXSTRING];
  int i,j;

  if (startpos > strlen(source))
    return (source);

  for (i = startpos - 1,j = 0; i < strlen(source); i++,j++)
    result[j] = source[i];

  result[j] = '\0';
  return (result);
}

void INPUT(var)
  char var[];
{
  int c;
  int i;

  for (i = 0; (c = getchar()) != EOF && c != '\n'; i++)
    var[i] = c;

  var[i + 1] = '\0';
  return;
}
