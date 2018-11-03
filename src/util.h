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

#ifndef GENETHREAD_UTIL_H_
#define GENETHREAD_UTIL_H_

#include "model.h"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace genethread::util {
uint8_t randomDigit() noexcept;
bool randomChance(double) noexcept;
size_t randomTo(size_t max) noexcept;
model::Guess rouletteSelect(const std::vector<model::Guess>&) noexcept;
}  // namespace genethread::util

#endif  // GENETHREAD_UTIL_H_