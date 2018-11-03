// Copyright 2018 Justin Hu
//
// This file is part of GeneThread.
//
// GeneThread is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// GeneThread is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// GeneThread.  If not, see <https://www.gnu.org/licenses/>.

#include "config.h"
#include "model.h"
#include "util.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <thread>
#include <variant>
#include <vector>

namespace {
using genethread::Config;
using genethread::validateConfig;
using genethread::model::crossover;
using genethread::model::Guess;
using genethread::util::rouletteSelect;
using std::cerr;
using std::cout;
using std::function;
using std::map;
using std::ref;
using std::string;
using std::swap;
using std::thread;
using std::vector;
}  // namespace

int main(int argc, char* argv[]) {
  // setup
  //  - get config file name from command line
  //  - get target number from command line
  //  - read in config file

  if (argc != 2 + 1) {
    cerr << "Fatal error: expected config file and target number, but found "
         << (argc - 1) << " argument" << (argc == 1 + 1 ? "" : "s")
         << " instead.\n";
    exit(EXIT_FAILURE);
  }

  Config config(argv[1]);
  validateConfig(config);

  string targetString(argv[2]);
  vector<uint8_t> number;

  number.reserve(targetString.size());
  for (const auto& c : targetString) {
    if (!isdigit(c)) {
      cerr << "Fatal error: expected target number, but got '" << targetString
           << "'.\n";
      exit(EXIT_FAILURE);
    }

    number.push_back(static_cast<uint8_t>(c - '0'));
  }

#ifndef NDEBUG  // sanity check
  for (const auto& digit : number) assert(digit < 10 && digit >= 0);
#endif

  vector<Guess> currentPopulation;
  vector<Guess> nextPopulation;
  // generate initial population
  for (int i = 0; i < config.getInt(Config::POPULATION_SIZE); i++)
    currentPopulation.push_back(Guess(targetString.size()));

  // run genetic algorithm for configured number of generations.
  size_t generation = 0;
  while (generation < config.getInt(Config::MAX_ITERATIONS)) {
    // compute fitness scores
    vector<thread> fitnessThreads;
    for (auto& guess : currentPopulation)  // parallel fitness computation
      fitnessThreads.push_back(
          thread(&Guess::computeFitness, &guess, ref(number)));
    for (auto& t : fitnessThreads) t.join();  // get all to finish

    // create next generation
    //  - perform crossover
    while (nextPopulation.size() < currentPopulation.size()) {
      vector<Guess> parents;
      for (size_t i = 0; i < config.getInt(Config::NUMBER_PARENTS); i++)
        parents.push_back(rouletteSelect(currentPopulation));
      nextPopulation.push_back(
          crossover(parents, config.getInt(Config::CROSSOVER_POINTS)));
    }
    //  - perform mutation
    vector<thread> mutationThreads;
    for (auto& guess : nextPopulation)  // parallel mutate
      mutationThreads.push_back(thread(
          &Guess::mutate, &guess, config.getDouble(Config::MUTATION_RATE)));
    for (auto& t : mutationThreads) t.join();  // get all to finish

    // check for success
    auto bestGuessIter = currentPopulation.cbegin();
    for (auto currIter = currentPopulation.cbegin();
         currIter != currentPopulation.cend(); ++currIter) {
      if (currIter->getFitness() > bestGuessIter->getFitness())
        bestGuessIter = currIter;
    }
    if (bestGuessIter->getFitness() == number.size() * 10) {
      cout << "Found number!\nGuess #"
           << bestGuessIter - currentPopulation.cbegin() << '\n';
      exit(EXIT_SUCCESS);
    }
    if (config.getInt(Config::VERBOSE) > 0)
      cout << "Generation " << generation << '\n';
    if (config.getInt(Config::VERBOSE) > 1)
      cout << "Current closest match: " << bestGuessIter->operator string()
           << " (fitness: "
           << (bestGuessIter->getFitness() /
               static_cast<double>(number.size() * 10))
           << ")\n\n";

    // end of generation
    generation++;
    swap(currentPopulation, nextPopulation);
    nextPopulation.clear();
  }

  cout << "No match found in " << generation << " generations.\n";

  exit(EXIT_SUCCESS);
}