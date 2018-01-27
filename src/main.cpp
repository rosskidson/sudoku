
#include <array>
#include <iostream>
#include <vector>
#include <assert.h>
#include "my_timer.hpp"

template <typename T, uint size>
struct Grid {
  T& operator()(uint x, uint y) { return grid[x + (y * size)]; }

  const T& operator()(uint x, uint y) const { return grid[x + (y * size)]; }

  std::array<T, size * size> grid{};
};

class SodukuGrid {
 public:
  SodukuGrid() {
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        // Initialize to ones.  This means all numbers are possible entries.
        // This is the case for an empty grid.
        constraints_(i, j) = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
      }
    }
  }

  bool Set(const int i, const int j, const int val) {
    if (!constraints_(i, j)[val]) {
      return false;
    }
    // Set the grid number.
    grid_numbers_(i, j) = val;

    // Update constraints.
    for (int n = 0; n < 9; ++n) {
      constraints_(n, j)[val] = 0;
      constraints_(i, n)[val] = 0;
    }
    const int sec_i = i / 3;
    const int sec_j = j / 3;
    for (int ni = sec_i * 3; ni < (sec_i + 1) * 3; ++ni) {
      for (int nj = sec_j * 3; nj < (sec_j + 1) * 3; ++nj) {
        constraints_(ni, nj)[val] = 0;
      }
    }
    return true;
  }

  void PopulateGrid_2() {
    Set(2, 0, 7);
    Set(5, 0, 9);
    Set(8, 0, 1);

    Set(1, 1, 4);
    Set(4, 1, 8);
    Set(7, 1, 3);

    Set(0, 2, 8);
    Set(3, 2, 2);
    Set(6, 2, 7);

    Set(0, 3, 2);
    Set(3, 3, 5);
    Set(6, 3, 1);

    Set(1, 4, 6);
    Set(4, 4, 4);
    Set(7, 4, 7);

    Set(2, 5, 4);
    Set(5, 5, 8);
    Set(8, 5, 9);

    Set(2, 6, 2);
    Set(5, 6, 6);
    Set(8, 6, 5);

    Set(1, 7, 9);
    Set(4, 7, 3);
    Set(7, 7, 6);

    Set(0, 8, 7);
    Set(3, 8, 1);
    Set(6, 8, 3);
  }

  void PopulateGrid_1() {
    Set(3, 0, 2);
    Set(4, 1, 4);
    Set(5, 1, 5);
    Set(8, 1, 3);
    Set(1, 2, 2);
    Set(2, 2, 8);
    Set(3, 2, 1);
    Set(6, 2, 4);
    Set(8, 2, 6);

    Set(0, 3, 9);
    Set(3, 3, 3);
    Set(4, 3, 7);
    Set(7, 3, 5);
    Set(8, 3, 4);

    Set(0, 4, 4);
    Set(2, 4, 3);
    Set(4, 4, 5);
    Set(6, 4, 1);
    Set(8, 4, 2);

    Set(0, 5, 8);
    Set(1, 5, 1);
    Set(4, 5, 2);
    Set(5, 5, 4);
    Set(8, 5, 9);

    Set(0, 6, 2);
    Set(2, 6, 6);
    Set(5, 6, 7);
    Set(6, 6, 5);
    Set(7, 6, 9);

    Set(0, 7, 7);
    Set(3, 7, 8);
    Set(4, 7, 6);

    Set(5, 8, 3);
  }

  void PrintGrid() const {
    std::cout << "#########################" << std::endl;
    for (int j = 0; j < 9; ++j) {
      std::cout << "| ";
      for (int i = 0; i < 9; ++i) {
        std::cout << grid_numbers_(i, j) << " ";
        if (!((i + 1) % 3)) {
          std::cout << "| ";
        }
      }
      if (!((j + 1) % 3)) {
        std::cout << std::endl << "##########################" << std::endl;
      } else {
        std::cout << std::endl;
      }
    }
  }

  void PrintConstraints() const {
    std::cout << "#########################" << std::endl;
    for (int j = 0; j < 9; ++j) {
      for (int i = 0; i < 9; ++i) {
        std::cout << "(" << i << ", " << j << "): ";
        for (int k = 1; k < 10; ++k) {
          std::cout << constraints_(i, j)[k] << " ";
        }
        std::cout << std::endl;
      }
    }
  }

  bool IsPossible(const int i, const int j, const int k) const { return constraints_(i, j)[k]; }

  int GetCell(const int i, const int j) const { return grid_numbers_(i, j); }

 private:
  Grid<int, 9> grid_numbers_;

  // This is an array of possible entries for the cell.
  // The index is the number in question (not zero indexed. Zero is wasted in this case)
  // 0 means not possible and 1 means possible.
  Grid<std::array<uint8_t, 10>, 9> constraints_;
};

bool Solve(const SodukuGrid& grid) {
  // Iterate over the grid.
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      // Only consider empty cells
      if (grid.GetCell(i, j) == 0) {
        // Iterate over values.
        for (int k = 1; k < 10; ++k) {
          if (grid.IsPossible(i, j, k) == 1) {
            // Get a new grid
            SodukuGrid new_grid = grid;
            assert(new_grid.Set(i, j, k));

            bool success = Solve(new_grid);
            if (success) {
              return true;
            }
          }
        }
        // At this point all values for a given cell location
        // have been tried and none were successful.
        return false;
      }
    }
  }

  std::cout << "solved. answer: " << std::endl;
  grid.PrintGrid();

  return true;
}

int main() {
  SodukuGrid grid;
  grid.PopulateGrid_2();
  grid.PrintGrid();

  std::cout << " #####    Solving....  ######" << std::endl;
  if (!Solve(grid)) {
    std::cout << "Failed to find a solution." << std::endl;
  }

  return 0;
}
