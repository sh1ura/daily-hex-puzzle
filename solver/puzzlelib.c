Field fieldMask = {{{0}}};
int poseNum[PN]; // effective number of poses

Field simpleRot(Field f) { // just rotate the pattern CCW 60deg
  Field nf;
  int x, y;
  
  for(y = 0; y < F_SIZE; y++) {
    for(x = 0; x < F_SIZE; x++) {
      int x2, y2;
      x2 = y;
      y2 = F_CENTER - x + y;
      if(0 <= x2 && x2 < F_SIZE && 0 <= y2 && y2 < F_SIZE) {
	nf.p[y][x] = f.p[y2][x2];
      }
      else {
	nf.p[y][x] = -1;
      }
    }
  }
  return nf;
}

Field mask(Field f) { // mask outside of the pattern
  int x, y;
  
  for(y = 0; y < F_SIZE; y++) {
    for(x = 0; x < F_SIZE; x++) {
      int x2, y2;
      x2 = y;
      y2 = F_CENTER - x + y;
      if(0 <= x2 && x2 < F_SIZE && 0 <= y2 && y2 < F_SIZE) {
      }
      else {
	f.p[y][x] = -1;
      }
    }
  }
  return f;
}

int outofMask(Field f) { // check if the pattern is on the mask
  int x, y;
  
  for(y = 0; y < F_SIZE; y++) {
    for(x = 0; x < F_SIZE; x++) {
      if(fieldMask.p[y][x] < 0 && f.p[y][x] > 0) {
	return 1;
      }
    }
  }
  return 0;
}

Field transpose(Field f) { // mirror
  Field nf;
  int x, y;
  
  for(y = 0; y < F_SIZE; y++) {
    for(x = 0; x < F_SIZE; x++) {
      nf.p[y][x] = f.p[x][y];
    }
  }
  return nf;
}		       

Field shiftUp(Field f) { // delete empty line
  int x, y, i;
  int sum;
  Field f2;
  
  for(i = 0; i < F_SIZE; i++) {
    f2 = f;
    sum = 0;
    for(x = 0; x < F_SIZE; x++) {
      if(f2.p[0][x] > 0) {
	sum++;
      }
    }
    if(sum == 0) {
      for(y = 1; y < F_SIZE; y++) {
	for(x = 0; x < F_SIZE; x++) {
	  if(f2.p[y][x] >= 0) {
	    f2.p[y-1][x] = f2.p[y][x];
	  }
	  else {
	    f2.p[y-1][x] = 0;
	  }
	}
      }
      for(x = F_CENTER; x < F_SIZE; x++) {
	f2.p[F_SIZE-1][x] = 0;
      }
      if(outofMask(f2)) {
	return f;
      }
      f = mask(f2);
    }
  }
  return f;
}

Field shiftXY(Field f) { // shift the pattern to upper-left
  f = shiftUp(f);
  f = transpose(f);
  f = shiftUp(f);
  f = transpose(f);
  return f;
}

Field shiftRot(Field f) { // rotate and shift to upper-left
  return shiftXY(simpleRot(f));
}

void showField(Field f) { // show the field to the display
  int x, y;

  for(y = 0; y < F_SIZE; y++) {
    for(x = 0; x < F_SIZE - y; x++) {
      printf(" ");
    }
    for(x = 0; x < F_SIZE; x++) {
      if(f.p[y][x] < 0) {
	printf("  ");
      }
      else if(f.p[y][x] != 0) {
	printf(" %c", 'A' + f.p[y][x] - 1);
      }
      else if(f.p[y][x] == 0) {
	printf(" .");
      }
    }
    printf("\n");
  }
}

int isSame(Field a, Field b) { // compare two patterns
  int x, y;

  for(y = 0; y < F_SIZE; y++) {
    for(x = 0; x < F_SIZE; x++) {
      if(a.p[y][x] != b.p[y][x]) {
	return 0;
      }
    }
  }
  return 1;
}

void genPose(void) { // generate rotated and mirrored patterns of pieces
  int pn, pose, i, j;
  Field f;

  for(pn = 0; pn < PN; pn++) {
    // generating rotated pattern
    piece[pn][0] = mask(piece[pn][0]);
    f = piece[pn][0];
    for(i = 0; i < 6; i++) {
      f = shiftRot(f);
      for(j = 0; j < poseNum[pn]; j++) {
	if(isSame(piece[pn][j], f)) {
	  break; // delete redundant pattern
	}
      }
      if(j == poseNum[pn]) {
	piece[pn][j] = f;
	poseNum[pn]++;
      }
    }
#if FLIP_PIECE
    // generating mirrored and rotated pattern
    f = transpose(piece[pn][0]);
    for(i = 0; i < 6; i++) {
      f = shiftRot(f);
      for(j = 0; j < poseNum[pn]; j++) {
	if(isSame(piece[pn][j], f)) {
	  break; // delete redundant pattern
	}
      }
      if(j == poseNum[pn]) {
	piece[pn][j] = f;
	poseNum[pn]++;
      }
    }
#endif
  }
  // calculate the effective pattern size
  for(pn = 0; pn < PN; pn++) {
    for(pose = 0; pose < poseNum[pn]; pose++) {
      int x, y;

      piece[pn][pose].x = 0;
      piece[pn][pose].y = 0;
      for(y = 0; y < F_SIZE; y++) {
	for(x = 0; x < F_SIZE; x++) {
	  if(piece[pn][pose].p[y][x] > 0) {
	    if(piece[pn][pose].x <= x) {
	      piece[pn][pose].x = x + 1;
	    }
	    if(piece[pn][pose].y <= y) {
	      piece[pn][pose].y = y + 1;
	    }
	  }
	}
      }
    }
  }      
}

// check if the pattern is placeable at (sx, sy)
int isPlaceable(Field *f, Field *p, int sx, int sy) {
  int x, y;
  
  for(y = 0; y < p->y; y++) {
    for(x = 0; x < p->x; x++) {
      if(p->p[y][x]) {
	if(f->p[y + sy][x + sx] != 0) { // collision
	  return 0;
	}
      }
    }
  }
  return 1;
}

// place the pattern at (sx, sy)
void doPlace(Field *f, Field *p, int sx, int sy) {
  int x, y;

  for(y = 0; y < p->y; y++) {
    for(x = 0; x < p->x; x++) {
      if(p->p[y][x]) {
	f->p[y + sy][x + sx] = p->p[y][x];
      }
    }
  }
}

void initPuzzle(void) {
  int i;
  Field f;
  
  for(i = 0; i < PN; i++) {
    poseNum[i] = 1;
  }

  fieldMask = mask(fieldMask);
  genPose();
}  

