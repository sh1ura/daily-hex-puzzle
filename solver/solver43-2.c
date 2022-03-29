#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "hex43-2.c"
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

char *map[F_SIZE][F_SIZE] = {
  {                         "NON","NON","Jan","NON","NON",  "" ,  "" ,  "" ,  ""  },
  {                      "NON","Sep","Jun","Apr","Feb","NON",  "" ,  "" ,  ""  },
  {                    "8" , "4" , "1" ,"Oct","Jul","May","Mar",  "" ,  ""  },
  {                "NON", "13", "9" , "5" , "2" ,"Nov","Aug","NON",  ""  },
  {             "NON", "20", "17", "14", "10", "6" , "3" ,"Dec","NON" },
  {            "" ,"NON", "24", "21", "18", "15", "11", "7" ,"NON" },
  {         "" ,  "" , "29", "27", "25", "22", "19", "16", "12" },
  {      "" ,  "" ,  "" ,"NON", "30", "28", "26", "23","NON" },
  {   "" ,  "" ,  "" ,  "" ,"NON","NON", "31","NON","NON" }
};

void setMark(Field *f, char *txt, int val) {
  int x, y;

  for(y = 0; y < F_SIZE; y++) {
    for(x = 0; x < F_SIZE; x++) {
      if(strcmp(map[y][x], txt) == 0) {
	f->p[y][x] = val;
      }
    }
  }
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
  if(argc == 3) {
    Sdata s = sd;
    setMark(&s.f, "NON", '.' - 'A' + 1);
    setMark(&s.f, argv[1], 'm' - 'A' + 1);
    setMark(&s.f, argv[2], 'd' - 'A' + 1);
    cntSol = placeCheck(&s, 0);
#if FIND_ALL_SOLUTIONS
    printf("solution = %d\n", cntSol);
#endif

  }
  else {
    for(int month = 0; month < 12; month++) {
      for(int date = 0; date <31; date++) {
	  Sdata s = sd;
	  char datetext[10];
	  sprintf(datetext, "%d", date + 1);

	  setMark(&s.f, "NON", '.' - 'A' + 1);
	  setMark(&s.f, mtext[month], 'm' - 'A' + 1);
	  setMark(&s.f, datetext,     'd' - 'A' + 1);
	  printf("%s %d\n", mtext[month], date + 1);
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
#if FIND_ALL_SOLUTIONS
    printf("minimum solution count = %d\n", minSol);
#endif
  }
  return 0;
}
