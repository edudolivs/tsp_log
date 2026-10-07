#include "mlpils.h"
#include "structs.h"
#include <vector>
using std::vector;

namespace {

inline void concat(Subsequence &s1, const Subsequence &s2, double **t) {
  double tmp = t[s1.last][s2.first];
  s1.W = s1.W + s2.W;
  s1.C = s1.C + s2.W * (s1.T + tmp) + s2.C;
  s1.T = s1.T + tmp + s2.T;
  s1.first = s1.first;
  s1.last = s2.last;
}

void UpdateAllSubseq(Solution *s, vector<vector<Subsequence>> &subseq_matrix,
                     double **t) {
  int n = s->sequence.size() - 1;

  for (int i = 0; i <= n; i++) {
    subseq_matrix[i][i].W = 1;
    subseq_matrix[i][i].C = 0;
    subseq_matrix[i][i].T = 0;
    subseq_matrix[i][i].first = s->sequence[i];
    subseq_matrix[i][i].last = s->sequence[i];
  }

  for (int i = 0; i <= n; i++) {
    for (int j = i + 1; j <= n; j++) {
      subseq_matrix[i][j] = subseq_matrix[i][j - 1];
      concat(subseq_matrix[i][j], subseq_matrix[j][j], t);
    }
  }

  // for (int i = n; i >= 0; i--) {
  //   for (int j = i - 1; j >= 0; j--) {
  //     subseq_matrix[i][j] = subseq_matrix[i][j + 1];
  //     concat(subseq_matrix[i][j], subseq_matrix[j][j], t);
  //   }
  // }
}

void UpdateInterSubseq(Solution *s, vector<vector<Subsequence>> &subseq_matrix,
                       double **t, int i, int j) {
  int n = s->sequence.size() - 1;
  int low = std::min(i, j);
  int high = std::max(i, j);

  for (int k = low; k <= high; k++) {
    subseq_matrix[k][k].first = s->sequence[k];
    subseq_matrix[k][k].last = s->sequence[k];
  }

  for (int k = 0; k <= high; k++) {
    for (int l = std::max(low, k + 1); l <= n; l++) {
      subseq_matrix[k][l] = subseq_matrix[k][l - 1];
      concat(subseq_matrix[k][l], subseq_matrix[l][l], t);
    }
  }

  // for (int k = n; k >= low; k--) {
  //   for (int l = std::min(high, k - 1); l >= 0; l--) {
  //     subseq_matrix[k][l] = subseq_matrix[k][l + 1];
  //     concat(subseq_matrix[k][l], subseq_matrix[l][l], t);
  //   }
  // }
}

vector<int> escolherSemRepeticao(int n, int q) {
  vector<int> escolhidos;
  escolhidos.reserve(q);
  if (q > n)
    return escolhidos;

  for (int i = 0; i < q; i++) {
    int x = rand() % (n - i);
    int j = 0;
    while (j < escolhidos.size() && x >= escolhidos[j]) {
      x++;
      j++;
    }
    escolhidos.insert(escolhidos.begin() + j, x);
  }
  return escolhidos;
}

vector<int> nosRestantes(int n) {
  vector<int> restantes;
  restantes.reserve(n - 1);
  for (int i = 1; i < n; i++) {
    restantes.push_back(i);
  }
  return restantes;
}

void ordenarEmOrdemCrescente(vector<int> &CL, double **t, int r) {
  auto cmp = [t, r](const auto &c1, const auto &c2) {
    return t[r][c1] < t[r][c2];
  };
  sort(CL.begin(), CL.end(), cmp);
}

Solution Construcao(int n, double **t) {
  double alpha = ((double)rand() / RAND_MAX) / 4;
  if (alpha == 0)
    alpha = 0.25;
  Solution s;
  int r = 0;
  s.cost = 0;
  s.sequence.push_back(0);
  vector<int> CL = nosRestantes(n);
  while (!CL.empty()) {
    ordenarEmOrdemCrescente(CL, t, r);
    int selecionado = rand() % ((int)ceil(alpha * CL.size()));
    int r = CL[selecionado];
    s.sequence.push_back(r);
    CL.erase(CL.begin() + selecionado);
  }
  s.sequence.push_back(0);
  for (int i = 0; i < n; i++) {
    s.cost += (n - i) * t[s.sequence[i]][s.sequence[i + 1]];
  }
  return s;
}

bool bestImprovementSwap(Solution *s,
                         vector<vector<Subsequence>> &subseq_matrix,
                         double **t) {
  int n = s->sequence.size() - 1;
  double lowest = s->cost;
  int best_i = -1, best_j = -1;
  for (int i = 1; i < n - 2; i++) {
    for (int j = i + 3; j < n; j++) {
      Subsequence sub = subseq_matrix[0][i - 1];
      concat(sub, subseq_matrix[j][j], t);
      concat(sub, subseq_matrix[i + 1][j - 1], t);
      concat(sub, subseq_matrix[i][i], t);
      concat(sub, subseq_matrix[j + 1][n], t);
      if (sub.C < lowest) {
        lowest = sub.C;
        best_i = i;
        best_j = j;
      }
    }
  }

  if (best_i != -1) {
    std::swap(s->sequence[best_i], s->sequence[best_j]);
    s->cost = lowest;
    UpdateInterSubseq(s, subseq_matrix, t, best_i, best_j);
    return true;
  }
  return false;
}

bool bestImprovement2Opt(Solution *s,
                         vector<vector<Subsequence>> &subseq_matrix,
                         double **t) {
  int n = s->sequence.size() - 1;
  double lowest = s->cost;
  int best_i = -1, best_j = -1;

  for (int i = 1; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      Subsequence sub = subseq_matrix[0][i - 1];
      concat(sub, -subseq_matrix[i][j], t);
      concat(sub, subseq_matrix[j + 1][n], t);
      if (sub.C < lowest) {
        lowest = sub.C;
        best_i = i;
        best_j = j;
      }
    }
  }

  if (best_i != -1) {
    std::reverse(s->sequence.begin() + best_i,
                 s->sequence.begin() + best_j + 1);
    s->cost = lowest;
    UpdateInterSubseq(s, subseq_matrix, t, best_i, best_j);
    return true;
  }
  return false;
}

bool bestImprovementOrOpt(Solution *s,
                          vector<vector<Subsequence>> &subseq_matrix,
                          double **t, int length) {
  int n = s->sequence.size() - 1;
  double lowest = s->cost;
  int best_i = -1, best_j = -1;

  for (int i = 1; i < n - length; i++) {
    for (int j = 0; j < n; j++) {
      if (j >= i - 2 && j <= i + length)
        continue;
      Subsequence sub;
      if (i > j) {
        sub = subseq_matrix[0][j];
        concat(sub, subseq_matrix[i][i + length - 1], t);
        concat(sub, subseq_matrix[j + 1][i - 1], t);
        concat(sub, subseq_matrix[i + length][n], t);
      } else {
        sub = subseq_matrix[0][i - 1];
        concat(sub, subseq_matrix[i + length][j], t);
        concat(sub, subseq_matrix[i][i + length - 1], t);
        concat(sub, subseq_matrix[j + 1][n], t);
      }
      if (sub.C < lowest) {
        lowest = sub.C;
        best_i = i;
        best_j = j;
      }
    }
  }

  if (best_i != -1) {
    if (best_i < best_j) {
      std::rotate(s->sequence.begin() + best_i,
                  s->sequence.begin() + best_i + length,
                  s->sequence.begin() + best_j + 1);
    } else {
      std::rotate(s->sequence.begin() + best_j + 1,
                  s->sequence.begin() + best_i,
                  s->sequence.begin() + best_i + length);
    }
    s->cost = lowest;
    UpdateInterSubseq(s, subseq_matrix, t, best_i, best_j);
    return true;
  }
  return false;
}

void BuscaLocal(Solution *s, vector<vector<Subsequence>> &subseq_matrix,
                double **t) {
  static int count = 0;
  vector<int> NL = {1, 2, 3, 4, 5};
  bool improved = false;

  while (!NL.empty()) {
    int n = rand() % NL.size();
    switch (NL[n]) {
    case 1:
      improved = bestImprovementSwap(s, subseq_matrix, t);
      break;
    case 2:
      improved = bestImprovement2Opt(s, subseq_matrix, t);
      break;
    case 3:
      improved = bestImprovementOrOpt(s, subseq_matrix, t, 1);
      break;
    case 4:
      improved = bestImprovementOrOpt(s, subseq_matrix, t, 2);
      break;
    case 5:
      improved = bestImprovementOrOpt(s, subseq_matrix, t, 3);
      break;
    }

    if (improved)
      NL = {1, 2, 3, 4, 5};
    else
      NL.erase(NL.begin() + n);
  }
}

Solution Perturbacao(Solution s, vector<vector<Subsequence>> &subseq_matrix,
                     double **t) {
  int n = s.sequence.size() - 1;
  if (n < 6)
    return s;
  int max_len = (int)ceil((double)n / 10);
  int len1, len0;
  if (max_len >= 3) {
    len0 = 2 + rand() % (max_len - 1);
    len1 = 2 + rand() % (max_len - 1);
  } else {
    len0 = 2;
    len1 = 2;
  }
  vector<int> i = escolherSemRepeticao(n - len0 - len1, 2);
  i[0] += 1;
  i[1] += len0 + 1;

  Subsequence sub = subseq_matrix[0][i[0] - 1];
  concat(sub, subseq_matrix[i[1]][i[1] + len1 - 1], t);
  concat(sub, subseq_matrix[i[0] + len0][i[1] - 1], t);
  concat(sub, subseq_matrix[i[0]][i[0] + len0 - 1], t);
  concat(sub, subseq_matrix[i[1] + len1][n], t);

  std::rotate(s.sequence.begin() + i[0], s.sequence.begin() + i[0] + len0,
              s.sequence.begin() + i[1]);
  std::rotate(s.sequence.begin() + i[0], s.sequence.begin() + i[1],
              s.sequence.begin() + i[1] + len1);

  UpdateInterSubseq(&s, subseq_matrix, t, i[0], i[1] + len1 - 1);

  s.cost = sub.C;

  return s;
}
} // namespace

Solution MLP(int maxIter, int maxIterIls, double **t, int n) {
  Solution bestOfAll;
  bestOfAll.cost = INFINITY;
  for (int i = 0; i < maxIter; i++) {
    Solution s = Construcao(n, t);
    vector<vector<Subsequence>> subseq_matrix(n + 1,
                                              vector<Subsequence>(n + 1));
    UpdateAllSubseq(&s, subseq_matrix, t);
    Solution best = s;

    int iterIls = 0;

    while (iterIls <= maxIterIls) {
      BuscaLocal(&s, subseq_matrix, t);
      s.cost = 0;
      for (int j = 0; j < n; j++) {
        s.cost += (n - j) * t[s.sequence[j]][s.sequence[j + 1]];
      }
      if (s.cost < best.cost) {
        best = s;
        iterIls = 0;
      }
      s = Perturbacao(best, subseq_matrix, t);
      iterIls++;
    }
    if (best.cost < bestOfAll.cost)
      bestOfAll = best;
  }

  return bestOfAll;
}
