#include "Data.h"
#include "solution.hpp"
#include <chrono>
// #include <iostream>

// using std::cout;
// using std::endl;

int main(int argc, char **argv) {

  printf("%-12s %12s %12s\n", "instancia", "tempo", "custo");
  printf("======================================\n");
  for (int i = 1; i < argc; i++) {
    auto data = Data(2, argv[i]);
    data.read();
    size_t n = data.getDimension();

    int maxIter = 50;
    int maxIterIls;
    if (n >= 150)
      maxIterIls = n / 2;
    else
      maxIterIls = n;

    auto c = data.getMatrixCost();

    // c = (double **)realloc(c, (n + 1) * sizeof(double *));
    // for (int i = n; i > 0; i--) {
    //   c[i - 1] = (double *)realloc(c[i - 1], (n + 1) * sizeof(double));
    //   c[i] = c[i - 1];
    //   for (int j = n; j > 0; j--) {
    //     c[i][j] = c[i][j - 1];
    //   }
    //   c[i][0] = (double)NULL;
    // }
    // c[0] = (double *)NULL;

    string name = data.getInstanceName();

    auto ini = chrono::high_resolution_clock::now();

    Solution s = ILS(maxIter, maxIterIls, c, n);

    auto fim = chrono::high_resolution_clock::now();

    auto t = chrono::duration_cast<chrono::nanoseconds>(fim - ini);

    printf("%-12s %12.4lf %12.0lf\n", name.c_str(), t.count() / 1e9, s.cost);
    // cout << t.count() / 1e9 << "s\t" << s.cost << endl;
    // double true_cost = 0;
    // for (int i = 0; i < n; i++) {
    //   true_cost += c[s.sequence[i]][s.sequence[i + 1]];
    //   cout << s.sequence[i] << " (" << c[s.sequence[i]][s.sequence[i + 1]] <<
    //   ") ";
    // }
    // cout << s.sequence[n] << endl;
    // cout << "true_cost: " << true_cost << endl;
  }

  return 0;
}
