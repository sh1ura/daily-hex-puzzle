#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "puzzlelib.c"

#define DEBUG 0
#define FIND_ALL_SOLUTIONS 0
#define SHOW_FIELD 1

typedef struct { // data structure for recursive search
  Field f; // current pattern of the field
  int used[PN]; // table of used pieces
} Sdata;

Sdata sd = {
  {{{0}}},
  {0}
};

// recursive search of placing pieces
int placeCheck(Sdata *sd, int pn) {
  int x, y, i, pose, result = 0;

#if DEBUG
  for(i = 0; i < PN; i++) {
    printf(" %d", sd->used[i]);
  }
  printf("\n");
  showField(sd->f);
#endif
  
  if(pn == PN) { // solution is found
#if SHOW_FIELD
    showField(sd->f);
#if FIND_ALL_SOLUTIONS
    printf("\n");
#endif
#endif
    return 1;
  }
  // find top-left empty cell
  for(y = 0; y < F_SIZE; y++) {
    for(x = 0; x < F_SIZE; x++) {
      if(sd->f.p[y][x] == 0) {
	goto OUT;
      }
    }
  }
 OUT:
  for(i = 0; i < PN; i++) {
    if(sd->used[i]) { // select unsed piece
      continue;
    }
    for(pose = 0; pose < poseNum[i]; pose++) {
      for(int py = 0; py < piece[i][pose].y; py++) {
	for(int px = 0; px < piece[i][pose].x; px++) {
	  int posx = x - px;
	  int posy = y - py;
	  if(posx < 0 || posy < 0 ||
	     posx + piece[i][pose].x > F_SIZE ||
	     posy + piece[i][pose].y > F_SIZE) {
	    continue; // piece is not in the field
	  }
	  if(piece[i][pose].p[py][px] <= 0) {
	    continue; // the piece must fill (y, x)
	  }
	  if(isPlaceable(&(sd->f), &piece[i][pose], posx, posy)) {
	    Sdata nsd = *sd;
	    nsd.used[i] = 1;
	    doPlace(&(nsd.f), &piece[i][pose], posx, posy);
	    result += placeCheck(&nsd, pn + 1);
#if !FIND_ALL_SOLUTIONS
	    if(result) {
	      return 1;
	    }
#endif
	  }
	}
      }
    }
  }
  return result;
}

char *mtext[] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

char *dtext[] = {
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

#if 0 // old version
char *map[F_SIZE][F_SIZE] = {
  {                        "Apr", "May", "Jun", "Jul", "Aug",  ""  ,  ""  ,  ""  ,  ""   },
  {                     "Mar", "Sun", "Mon", "Tue", "Wed", "Sep",  ""  ,  ""  ,  ""   },
  {                  "Feb", "NON", "Thu", "Fri", "Sat", "NON", "Oct",  ""  ,  ""   },
  {               "Jan",  "1" ,  "2" ,  "3" ,  "4" ,  "5" ,  "6" , "Nov",  ""   },
  {            "NON",  "7" ,  "8" ,  "9" , "10" , "11" , "12" , "13" , "Dec" },
  {          ""  , "NON", "14" , "15" , "16" , "17" , "18" , "19" , "NON" },
  {       ""  ,  ""  , "NON", "20" , "21" , "22" , "23" , "24" , "NON" },
  {    ""  ,  ""  ,  ""  , "NON", "25" , "26" , "27" , "28" , "NON" },
  { ""  ,  ""  ,  ""  ,  ""  , "NON", "29" , "30" , "31" , "NON" }
};
#else // improved version with better symmetry
char *map[F_SIZE][F_SIZE] = {
  {                        "May", "Jun",  ""  , "Jul", "Aug",  ""  ,  ""  ,  ""  ,  ""   },
  {                     "Apr", "Sun", "Mon", "Tue", "Wed", "Sep",  ""  ,  ""  ,  ""   },
  {                  "Mar", "NON", "Thu", "Fri", "Sat", "NON", "Oct",  ""  ,  ""   },
  {               "Feb",  "1" ,  "2" ,  "3" ,  "4" ,  "5" ,  "6" , "Nov",  ""   },
  {            "Jan",  "7" ,  "8" ,  "9" , "10" , "11" , "12" , "13" , "Dec" },
  {          ""  , "NON", "14" , "15" , "16" , "17" , "18" , "19" , "NON" },
  {       ""  ,  ""  , "NON", "20" , "21" , "22" , "23" , "24" , "NON" },
  {    ""  ,  ""  ,  ""  , "NON", "25" , "26" , "27" , "28" , "NON" },
  { ""  ,  ""  ,  ""  ,  ""  , "NON", "29" , "30" , "31" , "NON" }
};
#endif

void setMark(Field *f, char *txt, int val) {
  int x, y;

  for(y = 0; y < F_SIZE; y++) {
    for(x = 0; x < F_SIZE; x++) {
      if(strcmp(map[y][x], txt) == 0) {
	f->p[y][x] = val;
	return;
      }
    }
  }
  printf("%s is not found.\n", txt);
  exit(-1);
}
    
int main(int argc, char *argv[]) {
  int minSol = INT_MAX, cntSol;

  initPuzzle(); // generate mirrored and rotated patterns of pieces

#if DEBUG
  for(int pn = 0; pn < PN; pn++) {
    for(int i = 0; i < poseNum[pn]; i++) {
      printf("parts=%d, pose=%d\n", pn, i);
      printf("(%d, %d)\n", piece[pn][i].x, piece[pn][i].y);
      showField(piece[pn][i]);
    }
  }
#endif

  sd.f = mask(sd.f);
  if(argc == 4) {
    Sdata s = sd;
    setMark(&s.f, argv[1], 'm' - 'A' + 1);
    setMark(&s.f, argv[3], 'w' - 'A' + 1);
    setMark(&s.f, argv[2], 'd' - 'A' + 1);
    cntSol = placeCheck(&s, 0);
#if FIND_ALL_SOLUTIONS
    printf("solution = %d\n", cntSol);
#endif

  }
  else {
    for(int month = 0; month < 12; month++) {
      for(int date = 0; date <31; date++) {
	for(int week = 0; week < 7; week++) {
	  Sdata s = sd;
	  char datetext[10];
	  sprintf(datetext, "%d", date + 1);

	  setMark(&s.f, mtext[month], 'm' - 'A' + 1);
	  setMark(&s.f, dtext[week],  'w' - 'A' + 1);
	  setMark(&s.f, datetext,     'd' - 'A' + 1);
	  printf("%s %d %s\n", mtext[month], date + 1, dtext[week]);
	  cntSol = placeCheck(&s, 0);
#if FIND_ALL_SOLUTIONS
	  printf("Solution = %d\n", cntSol);
#endif
	  if(cntSol == 0) { // search
	    printf("No solution!\n");
	    showField(s.f);	
	    exit(0);
	  }
	  if(minSol > cntSol) {
	    minSol = cntSol;
	  }
	}
      }
    }
#if FIND_ALL_SOLUTIONS
    printf("minimum solution count = %d\n", minSol);
#endif
  }
  return 0;
}
