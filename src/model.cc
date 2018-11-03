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

#include "model.h"

#include "util.h"

#include <algorithm>
#include <cassert>
#include <cmath>

namespace genethread::model {
namespace {
using std::sort;
using std::string;
using std::vector;
using util::randomChance;
using util::randomDigit;
using util::randomTo;
}  // namespace
Guess::Guess(size_t length) noexcept : Guess() {
  for (size_t i = 0; i < length; i++) chromosome.push_back(randomDigit());
}

Guess::operator string() const noexcept {
  string buffer;
  for (const auto& digit : chromosome) buffer += static_cast<char>(digit + '0');
  return buffer;
}

size_t Guess::getFitness() const noexcept { return fitness; }

void Guess::mutate(double rate) noexcept {
  for (auto& gene : chromosome) {
    if (randomChance(rate)) gene = randomDigit();
  }
}

void Guess::computeFitness(const std::vector<uint8_t>& target) noexcept {
  assert(chromosome.size() == target.size());

  // difference between digits cannot possibly be more than 9.
  fitness = target.size() * 10;
  for (size_t i = 0; i < target.size(); i++)
    fitness -= abs(target[i] - chromosome[i]);
}

// Perfoms multi-parent, n-point crossover with randomly selected crossover
// points. Selection of parent follows round-robin order.
Guess crossover(const vector<Guess>& parents, size_t crossoverCount) noexcept {
  Guess g;
  vector<size_t> crossoverPoints;

  assert(!parents.empty());

  size_t chromosomeLength = parents.front().chromosome.size();
#ifndef NDEBUG  // sanity check
  for (const auto& parent : parents)
    assert(parent.chromosome.size() == chromosomeLength);
#endif

  for (size_t i = 0; i < crossoverCount; i++)
    crossoverPoints.push_back(randomTo(chromosomeLength));

  sort(crossoverPoints.begin(), crossoverPoints.end());

  size_t currentPoint = 0;
  for (size_t i = 0; i < chromosomeLength; i++) {
    if (i < crossoverPoints.size() - 1 && i == crossoverPoints[currentPoint])
      currentPoint++;
    g.chromosome.push_back(
        parents[currentPoint % parents.size()].chromosome[i]);
  }

  return g;
}

Guess::Guess() noexcept : fitness{0} {}
}  // namespace genethread::model