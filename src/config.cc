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

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace genethread {
namespace {
using std::cerr;
using std::ifstream;
using std::pair;
using std::stod;
using std::stoul;
using std::string;
}  // namespace
Config::Config(const string& filename) noexcept {
  ifstream fin;
  fin.open(filename);

  if (!fin.is_open()) {
    cerr << "Fatal error: cannot open config file '" << filename << "'.\n";
    exit(EXIT_FAILURE);
  }

  string buffer;
  while (getline(fin, buffer)) {
    string key = buffer.substr(0, buffer.find(" = "));
    string value = buffer.substr(buffer.find(" = ") + 3);

    try {
      size_t eidx;
      int i = stoul(value, &eidx);
      if (eidx != value.size()) throw std::invalid_argument("stoi");
      ints.insert(pair<string, int>(key, i));
    } catch (const std::invalid_argument&) {
      try {
        size_t eidx;
        double d = stod(value, &eidx);
        if (eidx != value.size()) throw std::invalid_argument("stod");
        doubles.insert(pair<string, double>(key, d));
      } catch (const std::invalid_argument&) {
        cerr << "Fatal error: could not parse line '" << buffer << "'.\n";
        exit(EXIT_FAILURE);
      }
    }
  }
}

size_t Config::getInt(const string& fieldname) const {
  auto iter = ints.find(fieldname);
  if (iter == ints.end()) throw std::invalid_argument("Config::getInt");
  return iter->second;
}
double Config::getDouble(const string& fieldname) const {
  auto iter = doubles.find(fieldname);
  if (iter == doubles.end()) throw std::invalid_argument("Config::getDouble");
  return iter->second;
}

void validateConfig(const Config& c) {
  try {
    c.getInt(Config::MAX_ITERATIONS);
  } catch (const std::invalid_argument&) {
    cerr << "Fatal error: config does not have integer '"
         << Config::MAX_ITERATIONS << "'.\n";
    exit(EXIT_FAILURE);
  }
  try {
    c.getInt(Config::POPULATION_SIZE);
  } catch (const std::invalid_argument&) {
    cerr << "Fatal error: config does not have integer '"
         << Config::POPULATION_SIZE << "'.\n";
    exit(EXIT_FAILURE);
  }
  try {
    c.getDouble(Config::MUTATION_RATE);
  } catch (const std::invalid_argument&) {
    cerr << "Fatal error: config does not have real '" << Config::MUTATION_RATE
         << "'.\n";
    exit(EXIT_FAILURE);
  }
  try {
    c.getInt(Config::NUMBER_PARENTS);
  } catch (const std::invalid_argument&) {
    cerr << "Fatal error: config does not have integer '"
         << Config::NUMBER_PARENTS << "'.\n";
    exit(EXIT_FAILURE);
  }
  try {
    c.getInt(Config::CROSSOVER_POINTS);
  } catch (const std::invalid_argument&) {
    cerr << "Fatal error: config does not have integer '"
         << Config::CROSSOVER_POINTS << "'.\n";
    exit(EXIT_FAILURE);
  }
  try {
    c.getInt(Config::VERBOSE);
  } catch (const std::invalid_argument&) {
    cerr << "Fatal error: config does not have integer '" << Config::VERBOSE
         << "'.\n";
    exit(EXIT_FAILURE);
  }
  try {
    c.getDouble(Config::CROSSOVER_RATE);
  } catch (const std::invalid_argument&) {
    cerr << "Fatal error: config does not have real '" << Config::CROSSOVER_RATE
         << "'.\n";
    exit(EXIT_FAILURE);
  }
}
}  // namespace genethread