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

#ifndef GENETHREAD_CONFIG_H_
#define GENETHREAD_CONFIG_H_

#include <map>
#include <string>

namespace genethread {
class Config {
 public:
  static constexpr auto MAX_ITERATIONS = "max-generations";
  static constexpr auto POPULATION_SIZE = "population-size";
  static constexpr auto MUTATION_RATE = "mutation-rate";
  static constexpr auto NUMBER_PARENTS = "number-parents";
  static constexpr auto CROSSOVER_POINTS = "crossover-points";
  static constexpr auto VERBOSE = "verbose";

  Config(const std::string&) noexcept;
  Config(const Config&) = delete;
  Config(Config&&) = delete;

  Config& operator=(const Config&) = delete;
  Config& operator=(Config&&) = delete;

  size_t getInt(const std::string&) const;
  double getDouble(const std::string&) const;

 private:
  std::map<std::string, size_t> ints;
  std::map<std::string, double> doubles;
};

void validateConfig(const Config&);
}  // namespace genethread

#endif  // GENETHREAD_CONFIG_H_