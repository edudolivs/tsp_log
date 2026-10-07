#include "Data.h"
#include "mlpils.h"
#include "tspils.h"
#include <chrono>

void tsp(int argc, char **argv) {
  printf("%-12s %12s %12s\n", "instancia", "tempo", "custo");
  printf("======================================\n");
  for (int i = 2; i < argc; i++) {
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
    fprintf(stderr, "%-12s%02d/%02d done\n", name.c_str(), i - 1, argc - 2);
  }
}

void mlp(int argc, char **argv) {
  printf("%-12s %12s %12s\n", "instancia", "tempo", "custo");
  printf("======================================\n");
  for (int i = 2; i < argc; i++) {
    auto data = Data(2, argv[i]);
    data.read();
    size_t n = data.getDimension();

    int maxIter = 10;
    int maxIterIls = std::min(100, (int)n);

    auto c = data.getMatrixCost();

    string name = data.getInstanceName();

    double total_time = 0;
    double total_cost = 0;

    for (int j = 0; j < 10; j++) {
      auto ini = chrono::high_resolution_clock::now();

      Solution s = MLP(maxIter, maxIterIls, c, n);

      auto fim = chrono::high_resolution_clock::now();

      auto t = chrono::duration_cast<chrono::nanoseconds>(fim - ini);
      total_time += t.count() * 1e-9;
      total_cost += s.cost;
    }
    total_time /= 10;
    total_cost /= 10;

    printf("%-12s %12.4lf %12.1lf\n", name.c_str(), total_time, total_cost);
    fprintf(stderr, "%-12s%02d/%02d done\n", name.c_str(), i - 1, argc - 2);
  }
}

int main(int argc, char **argv) {
  if (argc < 3 || (strcmp(argv[1], "tsp") && strcmp(argv[1], "mlp"))) {
    fprintf(stderr, "usage: ./tsp [tsp|mlp] instance\n");
  }
  if (!strcmp(argv[1], "tsp"))
    tsp(argc, argv);
  else if (!strcmp(argv[1], "mlp"))
    mlp(argc, argv);
}
