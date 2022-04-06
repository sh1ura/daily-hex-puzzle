# Solver of the Daily Hexagonal Puzzle

The program is written in standard C. To compile it, just run `make` in UNIX environment.

## Solutions

* The solutions for smaller versions (`solutions43-noflip.txt` and `solutions43-2-noflip.txt`) are solved without flipping pieces.

## Usage

* Solve the puzzle on specified day
```
./solver43 Sep 1
```
or 
```
./solver61 Sep 1 Fri
```

* Solve the puzzle for the all month/date/day combinations (NN = 43 or 61)
```
./solverNN
```

## Configuration

in `solver.c`, some config parameters there.

```
#define FIND_ALL_SOLUTIONS 0
#define SHOW_FIELD 1
#define FLIP_PIECE 0
```

* `FIND_ALL_SOLUTIONS` : switch to show the all solutions for each day(or just one solution)
* `SHOW_FIELD` : Show the solution (or just count the number of solutions)
* `FLIP_PIECE` : Solve the puzzle with or without flipping pieces

## How to compile

* In the UNIX like environment, just run `make` in the folder.
* Here I do not use separate compilation / link scheme but just direct #include -ing of several C sources for simplicity.

## Algorithm

### Representation

* Hexagonal map is represented by usual 2-dimensional array.
* Some elements (at top-right and bottom-left) are outside of the map.
* 60deg rotation of the map is possible by linear transformation.
* By the combination of 60deg rotation and mirror transformation (transpose of matrix) make it possible to generate 12way poses of the pieces.

![スライド1](https://user-images.githubusercontent.com/86639425/159246601-a743e4ad-03d5-4c60-bbd7-1d96e4da48be.jpeg)

### Search algorithm

Blute-force search is done by recursive algorithm as follows.

1. Search empty cell from top left.
2. Try to fill the empty cell by using all possible poses and positions of all remaining (unused) pieces.
3. If the empty cell can be filled by the piece without collisions, place the piece there.
4. If there is still unused piece, repeat ( = recursive call of) the process from 1.
