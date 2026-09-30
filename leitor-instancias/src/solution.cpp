#include "solution.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
using std::vector;

vector<IsertionInfo> calcularCustoInsercao(Solution &s, const vector<int> &CL,
                                           double **c) {
  vector<IsertionInfo> custoInsercao =
      vector<IsertionInfo>((s.sequence.size() - 1) * CL.size());
  int l = 0;
  for (int a = 0; a < s.sequence.size() - 1; a++) {
    int i = s.sequence[a];
    int j = s.sequence[a + 1];
    for (auto k : CL) {
      custoInsercao[l].custo = c[i][k] + c[j][k] - c[i][j];
      custoInsercao[l].noInserido = k;
      custoInsercao[l].arestaRemovida = a;
      l++;
    }
  }
  return custoInsercao;
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

vector<int> escolher3NosAleatorios(int n) {
  vector<int> escolhidos = escolherSemRepeticao(n, 3);
  escolhidos.reserve(n);
  return escolhidos;
}

vector<int> nosRestantes(int n, const vector<int> &escolhidos) {
  vector<int> restantes;
  restantes.reserve(n - 3);
  for (int i = 0; i < n; i++) {
    if (i != escolhidos[0] && i != escolhidos[1] && i != escolhidos[2]) {
      restantes.push_back(i);
    }
  }

  return restantes;
}

void ordenarEmOrdemCrescente(vector<IsertionInfo> &custoInsercao) {
  auto cmp = [](const auto &c1, const auto &c2) { return c1.custo < c2.custo; };
  sort(custoInsercao.begin(), custoInsercao.end(), cmp);
}

void inserirNaSolucao(Solution &s, const IsertionInfo &info) {
  s.sequence.insert(s.sequence.begin() + info.arestaRemovida + 1,
                    info.noInserido);
  s.cost += info.custo;
}

Solution Construcao(int n, double **c) {
  Solution s;
  s.cost = 0;
  s.sequence = escolher3NosAleatorios(n);
  s.sequence.push_back(s.sequence[0]);
  for (int i = 0; i < 3; i++) {
    s.cost += c[s.sequence[i]][s.sequence[i + 1]];
  }
  vector<int> CL = nosRestantes(n, s.sequence);
  while (!CL.empty()) {
    vector<IsertionInfo> custoInsercao = calcularCustoInsercao(s, CL, c);
    ordenarEmOrdemCrescente(custoInsercao);
    double alpha = (double)rand() / RAND_MAX;
    int selecionado = rand() % ((int)ceil(alpha * custoInsercao.size()));
    auto it =
        std::find(CL.begin(), CL.end(), custoInsercao[selecionado].noInserido);
    CL.erase(it);
    inserirNaSolucao(s, custoInsercao[selecionado]);
  }

  return s;
}

bool bestImprovementSwap(Solution *s, double **c) {
  double bestDelta = -1e-7;
  int best_i = -1, best_j = -1;
  for (int i = 1; i < s->sequence.size() - 3; i++) {
    int vi = s->sequence[i];
    int vi_next = s->sequence[i + 1];
    int vi_prev = s->sequence[i - 1];
    for (int j = i + 3; j < s->sequence.size() - 1; j++) {
      int vj = s->sequence[j];
      int vj_next = s->sequence[j + 1];
      int vj_prev = s->sequence[j - 1];
      double delta =
          (-c[vi_prev][vi] - c[vi][vi_next] + c[vi_prev][vj] + c[vj][vi_next] -
           c[vj_prev][vj] - c[vj][vj_next] + c[vj_prev][vi] + c[vi][vj_next]);
      if (delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
  }

  if (best_i != -1) {
    std::swap(s->sequence[best_i], s->sequence[best_j]);
    s->cost += bestDelta;
    return true;
  }
  return false;
}

bool bestImprovement2Opt(Solution *s, double **c) {
  static int v = 0;
  double bestDelta = -1e-7;
  int best_i = -1, best_j = -1;

  for (int i = 1; i < s->sequence.size() - 2; i++) {
    int vi = s->sequence[i];
    int vi_prev = s->sequence[i - 1];
    for (int j = i + 1; j < s->sequence.size() - 1; j++) {
      int vj = s->sequence[j];
      int vj_next = s->sequence[j + 1];
      double delta =
          (c[vi_prev][vj] + c[vi][vj_next] - c[vi_prev][vi] - c[vj][vj_next]);
      if (delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
  }

  if (best_i != -1) {
    std::reverse(s->sequence.begin() + best_i,
                 s->sequence.begin() + best_j + 1);
    s->cost += bestDelta;
    return true;
  }
  return false;
}

bool bestImprovementOrOpt(Solution *s, double **c, int length) {
  double bestDelta = -1e-7;
  int best_i = -1, best_j = -1;

  for (int i = 1; i < s->sequence.size() - length; i++) {
    int vi = s->sequence[i];
    int vi_prev = s->sequence[i - 1];
    int vf = s->sequence[i + length - 1];
    int vf_next = s->sequence[i + length];

    for (int j = 0; j < s->sequence.size() - 1; j++) {
      if ((j >= i - 1 && j < i + length) || j == i - 2 || j == i + length)
        continue;
      int vj_prev = s->sequence[j];
      int vj_next = s->sequence[j + 1];
      double delta = (c[vj_prev][vi] + c[vf][vj_next] + c[vi_prev][vf_next] -
                      c[vi_prev][vi] - c[vf][vf_next] - c[vj_prev][vj_next]);
      if (delta < bestDelta) {
        bestDelta = delta;
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
    s->cost += bestDelta;
    return true;
  }
  return false;
}

void BuscaLocal(Solution *s, double **c) {
  vector<int> NL = {1, 2, 3, 4, 5};
  bool improved = false;

  while (!NL.empty()) {
    int n = rand() % NL.size();
    switch (NL[n]) {
    case 1:
      improved = bestImprovementSwap(s, c);
      break;
    case 2:
      improved = bestImprovement2Opt(s, c);
      break;
    case 3:
      improved = bestImprovementOrOpt(s, c, 1);
      break;
    case 4:
      improved = bestImprovementOrOpt(s, c, 2);
      break;
    case 5:
      improved = bestImprovementOrOpt(s, c, 3);
      break;
    }

    if (improved)
      NL = {1, 2, 3, 4, 5};
    else
      NL.erase(NL.begin() + n);
  }
}

Solution Perturbacao(Solution s, double **c, int n) {
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

  int v0i_prev = s.sequence[i[0] - 1], v0i = s.sequence[i[0]];
  int v0f = s.sequence[i[0] + len0 - 1], v0f_next = s.sequence[i[0] + len0];
  int v1i_prev = s.sequence[i[1] - 1], v1i = s.sequence[i[1]];
  int v1f = s.sequence[i[1] + len1 - 1], v1f_next = s.sequence[i[1] + len1];

  double delta = (c[v0i_prev][v1i] + c[v1f][v0f_next] + c[v1i_prev][v0i] +
                  c[v0f][v1f_next] - c[v0i_prev][v0i] - c[v0f][v0f_next] -
                  c[v1i_prev][v1i] - c[v1f][v1f_next]);

  std::rotate(s.sequence.begin() + i[0], s.sequence.begin() + i[0] + len0,
              s.sequence.begin() + i[1]);
  std::rotate(s.sequence.begin() + i[0], s.sequence.begin() + i[1],
              s.sequence.begin() + i[1] + len1);

  s.cost += delta;

  return s;
}

Solution ILS(int maxIter, int maxIterIls, double **c, int n) {
  Solution bestOfAll;
  bestOfAll.cost = INFINITY;
  for (int i = 0; i < maxIter; i++) {
    Solution s = Construcao(n, c);
    Solution best = s;

    int iterIls = 0;

    while (iterIls <= maxIterIls) {
      BuscaLocal(&s, c);
      if (s.cost < best.cost) {
        best = s;
        iterIls = 0;
      }
      s = Perturbacao(best, c, n);
      iterIls++;
    }
    if (best.cost < bestOfAll.cost)
      bestOfAll = best;
  }

  return bestOfAll;
}
