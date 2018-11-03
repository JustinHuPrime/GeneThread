# Gene Thread

A proof-of-concept genetic algorithm using C++'s threads to achieve parallelism.

This genetic algorithm will guess an alphanumeric string. (For example, CPE1704TKS.)

## Config Options

* max-generations: maximum number of generations that will be run
* population-size: number of guesses per generation
* mutation-rate: chance that a digit in a guess will be mutated randomly
* number-parents: number of parents per child
* crossover-points: number of crossover points per child
* display-mode: display mode:
  * 0: only show final results
  * 1: show generation number
  * 2: show generation number, current best guess, and its fitness
* crossover-rate: frequency at which crossovers, instead of straight survival, happens.