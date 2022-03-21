# Solver of the Daily Hexagonal Puzzle

The program is written in standard C. To compile it, just run `make` in UNIX environment.

## Usage

* Solve the puzzle on specified day
```
./solver Sep 1 Fri
```

* Solve the puzzle for the all month/date/day combinations
```
./solver
```

## Configuration

```
#define FIND_ALL_SOLUTIONS 0
#define SHOW_FIELD 1
```

* `FIND_ALL_SOLUTIONS` : switch to show the all solutions for each day(or just one solution)
* `SHOW_FIELD` : Show the solution (or just count the number of solutions)
