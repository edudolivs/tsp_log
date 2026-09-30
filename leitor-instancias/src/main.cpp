#include "Data.h"
#include "solution.cpp"
#include <chrono>
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char **argv) {

  for (int i = 1; i < argc; i++) {
    auto data = Data(2, argv[i]);
    data.read();
    size_t n = data.getDimension();
    auto c = data.getMatrixCost();

    cout << data.getInstanceName() << ":\t\t";
    if (n >= 100) {
      cout << "TOO BIG" << endl;
      continue;
    }

    auto ini = chrono::high_resolution_clock::now();

    Solution s = ILS(10, 10, c, n);

    auto fim = chrono::high_resolution_clock::now();

    auto t = chrono::duration_cast<chrono::nanoseconds>(fim - ini);

    cout << t.count() << "ns\t\t" << s.cost << endl;
  }

  return 0;
}
