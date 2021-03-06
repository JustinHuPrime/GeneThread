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

#ifndef GENETHREAD_MODEL_H_
#define GENETHREAD_MODEL_H_

#include <cstdint>
#include <string>
#include <vector>

namespace genethread::model {
constexpr auto MAX_DIFF = 'z' - '0';

class Guess {
 public:
  explicit Guess(size_t) noexcept;
  Guess(const Guess&) = default;

  Guess& operator=(const Guess&) = default;

  operator std::string() const noexcept;

  size_t getFitness() const noexcept;

  void mutate(double) noexcept;
  void computeFitness(const std::string&) noexcept;

  friend Guess crossover(const std::vector<Guess>&, size_t, double) noexcept;

 private:
  Guess() noexcept;

  std::string chromosome;
  size_t fitness;
};

Guess crossover(const std::vector<Guess>&, size_t, double) noexcept;
}  // namespace genethread::model

#endif  // GENETHREAD_MODEL_H_