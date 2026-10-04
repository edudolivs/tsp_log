#include "Data.h"
#include "solution.hpp"
#include <chrono>

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

    string name = data.getInstanceName();

    double total_time = 0;
    double total_cost = 0;

    for (int j = 0; j < 10; j++) {
      auto ini = chrono::high_resolution_clock::now();

      Solution s = ILS(maxIter, maxIterIls, c, n);

      auto fim = chrono::high_resolution_clock::now();

      auto t = chrono::duration_cast<chrono::nanoseconds>(fim - ini);
      total_time += t.count() * 1e-9;
      total_cost += s.cost;
    }
    total_time /= 10;
    total_cost /= 10;

    printf("%-12s %12.4lf %12.1lf\n", name.c_str(), total_time, total_cost);
    fprintf(stderr, "%-12s%02d/%02d done\n", name.c_str(), i, argc - 1);
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
