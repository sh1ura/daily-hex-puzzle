#define F_SIZE 9
#define F_CENTER 4

#define POSE 12 // maximum cases of pose (mirror + rotation

typedef struct {
  int p[F_SIZE][F_SIZE]; // pattern
  int x, y;
} Field;

#define PN 11 // number of pieces

Field piece[PN][POSE] = { // pattern of pieces  
  {{{{  1, 1, 1 },
     { 1, 1, 1 }}}},

  {{{{  2, 2, 2 },
     { 0, 2, 2 },
     {0, 0, 2}}}},
  
  {{{{ 3, 3, 3 },
     {0, 0, 3, 3, 3 }}}},

  {{{{ 4, 4, 4, 4, 4 }}}},

  {{{{  5, 5 },
     { 5, 5, 5 }}}},

  {{{{ 6, 6, 6, 6 },
     { 6 }}}},

  {{{{ 7, 7, 7 },
     {0, 0, 7, 7 }}}},

  {{{{  8, 8, 8 },
     { 8 },
     {8 }}}},

  {{{{  0, 0, 9 },
     { 9, 9, 9 },
     {9 }}}},

  {{{{  0,10,10,10 },
     {10,10 }}}},
  
  {{{{   0,11, 0 },
     { 11,11,11 },
     { 0,11, 0 }}}},
};


