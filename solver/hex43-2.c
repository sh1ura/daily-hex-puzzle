#define F_SIZE 9
#define F_CENTER 4

#define POSE 12 // maximum cases of pose (mirror + rotation

typedef struct {
  int p[F_SIZE][F_SIZE]; // pattern
  int x, y;
} Field;

#define PN 10 // number of pieces

Field piece[PN][POSE] = { // pattern of pieces  
  {{{{  1, 1, 0 },
     { 1, 1, 1 }}}},

  {{{{  2, 0, 0 },
     { 2, 2, 2 }}}},
  
  {{{{  0, 3, 0 },
     { 3, 3, 3 }}}},

  {{{{ 4, 4, 4, 4 }}}},

  {{{{  0, 0, 5 },
     { 5, 5, 5 }}}},

  {{{{  6, 0, 0, 0 },
     { 0, 6, 6, 6 }}}},

  {{{{  0, 7, 0 },
     { 7, 7, 0 },
     {0, 0, 7 }}}},

  {{{{  8, 8, 0 },
     { 8, 0, 8}}}},

  {{{{  0, 9, 9 },
     { 9, 9, 0 }}}},

  {{{{ 10,10 },
     {10,10 }}}},
};


