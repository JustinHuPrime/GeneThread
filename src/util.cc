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

#include "util.h"

#include <algorithm>
#include <functional>
#include <random>

namespace genethread::util {
namespace {
using model::Guess;
using std::numeric_limits;
using std::random_device;
using std::vector;
}  // namespace

uint8_t randomDigit() noexcept { return random_device()() % 10; }
char randomChar() noexcept {
  char c;
  random_device rd;
  do {
    c = rd() % numeric_limits<char>().max();
  } while (!isalnum(c));

  return c;
}
bool randomChance(double chance) noexcept {
  random_device rd;
  return rd() < rd.max() * chance;
}
size_t randomTo(size_t max) noexcept { return random_device()() % max; }

Guess rouletteSelect(const vector<Guess>& possibles) noexcept {
  size_t totalFitness = std::accumulate(
      possibles.cbegin(), possibles.cend(), 0UL,
      [](size_t curr, const Guess& g) { return curr + g.getFitness(); });

  size_t selectedFitness = randomTo(totalFitness);
  for (const auto& guess : possibles) {
    if (selectedFitness <= guess.getFitness()) return guess;
    selectedFitness -= guess.getFitness();
  }

  abort();
}
}  // namespace genethread::util