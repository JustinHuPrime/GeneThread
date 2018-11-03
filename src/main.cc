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
#include <list>
#include <map>
#include <thread>
#include <variant>
#include <vector>

namespace {
using genethread::Config;
using genethread::validateConfig;
using genethread::model::crossover;
using genethread::model::Guess;
using genethread::model::MAX_DIFF;
using genethread::util::rouletteSelect;
using std::cerr;
using std::cout;
using std::function;
using std::list;
using std::map;
using std::min;
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
    cerr << "Fatal error: expected config file and target string, but found "
         << (argc - 1) << " argument" << (argc == 1 + 1 ? "" : "s")
         << " instead.\n";
    exit(EXIT_FAILURE);
  }

  Config config(argv[1]);
  validateConfig(config);

  string target(argv[2]);
  for (const auto& c : target) {
    if (!isalnum(c)) {
      cerr << "Fatal error: expected alphanumeric string, but found '" << target
           << "'.\n";
      exit(EXIT_FAILURE);
    }
  }

  vector<Guess> currentPopulation;
  vector<Guess> nextPopulation;
  // generate initial population. Not parallel due container access w/ resize
  for (size_t i = 0; i < config.getInt(Config::POPULATION_SIZE); i++)
    currentPopulation.push_back(Guess(target.size()));

  // run genetic algorithm for configured number of generations.
  size_t generation = 0;
  size_t maxThreads = min(1U, thread::hardware_concurrency() - 1);
  while (generation < config.getInt(Config::MAX_ITERATIONS)) {
    // compute fitness scores
    list<thread> fitnessThreads;
    for (auto& guess :
         currentPopulation) {  // parallel fitness computation using max
                               // hardware threads minus one (or one)
      if (fitnessThreads.size() >= maxThreads) {
        fitnessThreads.front().join();
        fitnessThreads.pop_front();
      }
      fitnessThreads.push_back(
          thread(&Guess::computeFitness, &guess, ref(target)));
    }
    for (auto& t : fitnessThreads) t.join();  // get all to finish

    // create next generation
    //  - perform crossover
    // not parallel due to container access w/ resize
    while (nextPopulation.size() < currentPopulation.size()) {
      vector<Guess> parents;
      for (size_t i = 0; i < config.getInt(Config::NUMBER_PARENTS); i++)
        parents.push_back(rouletteSelect(currentPopulation));
      nextPopulation.push_back(
          crossover(parents, config.getInt(Config::CROSSOVER_POINTS),
                    config.getDouble(Config::CROSSOVER_RATE)));
    }
    //  - perform mutation

    list<thread> mutationThreads;
    for (auto& guess :
         currentPopulation) {  // parallel mutate, capped number of threads
      if (mutationThreads.size() >= maxThreads) {
        mutationThreads.front().join();
        mutationThreads.pop_front();
      }
      mutationThreads.push_back(thread(
          &Guess::mutate, &guess, config.getDouble(Config::MUTATION_RATE)));
    }
    for (auto& t : mutationThreads) t.join();  // get all to finish

    // check for success
    auto bestGuessIter = currentPopulation.cbegin();
    for (auto currIter = currentPopulation.cbegin();
         currIter != currentPopulation.cend(); ++currIter) {
      if (currIter->getFitness() > bestGuessIter->getFitness())
        bestGuessIter = currIter;
    }
    if (bestGuessIter->getFitness() == target.size() * MAX_DIFF) {
      cout << "Found number!\nGuess #"
           << bestGuessIter - currentPopulation.cbegin() << '\n';
      cout.flush();
      exit(EXIT_SUCCESS);
    }
    if (config.getInt(Config::VERBOSE) > 0) {
      cout << "Generation " << generation << '\n';
      cout.flush();
    }
    if (config.getInt(Config::VERBOSE) > 1) {
      cout << "Current closest match: " << bestGuessIter->operator string()
           << " (fitness: "
           << (bestGuessIter->getFitness() /
               static_cast<double>(target.size() * MAX_DIFF))
           << ")\n\n";
      cout.flush();
    }

    // end of generation
    generation++;
    swap(currentPopulation, nextPopulation);
    nextPopulation.clear();
  }

  cout << "No match found in " << generation << " generations.\n";

  exit(EXIT_SUCCESS);
}