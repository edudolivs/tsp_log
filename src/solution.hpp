#ifndef SOLUTION
#define SOLUTION
#include <cstdlib>
#include <vector>
using std::vector;

typedef struct {
  vector<int> sequence;
  double cost;
} Solution;

typedef struct {
  int noInserido;
  int arestaRemovida;
  double custo;
} IsertionInfo;

vector<IsertionInfo> calcularCustoInsercao(Solution &s, const vector<int> &CL,
                                           double **c);

vector<int> escolherSemRepeticao(int n, int q);

vector<int> escolher3NosAleatorios(int n);

vector<int> nosRestantes(int n, const vector<int> &escolhidos);

void ordenarEmOrdemCrescente(vector<IsertionInfo> &custoInsercao);

void inserirNaSolucao(Solution &s, const IsertionInfo &info);

Solution Construcao(int n, double **c);

bool bestImprovementSwap(Solution *s, double **c);

bool bestImprovement2Opt(Solution *s, double **c);

bool bestImprovementOrOpt(Solution *s, double **c, int length);

void BuscaLocal(Solution *s, double **c);

Solution Perturbacao(Solution s, double **c, int n);

Solution ILS(int maxIter, int maxIterIls, double **c, int n);

#endif
