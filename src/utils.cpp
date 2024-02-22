#include "../include/utils.hpp"

#include <cstdlib>

int random_range(int min, int max) {
  return (std::rand() % (max - min + 1)) + min;
}