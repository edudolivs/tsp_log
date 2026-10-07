#ifndef STRUCTS
#define STRUCTS
#include <vector>

typedef struct Subsequence {
  double T, C;
  int W;
  int first, last;
  Subsequence operator-() {
    Subsequence s = {
        .T = T, .C = W * T - C, .W = W, .first = last, .last = first};
    return s;
  }
} Subsequence;

typedef struct {
  std::vector<int> sequence;
  double cost;
} Solution;

typedef struct {
  int noInserido;
  int arestaRemovida;
  double custo;
} IsertionInfo;

#endif
