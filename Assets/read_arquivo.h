#pragma once
#include <string>
#include <vector>
#include "dados.h"

using namespace std;

bool carregarFase(const string &nome, Fase &fase, string &erro);

bool chegouSaida(const Fase &fase);

bool lerTextoArquivo(const string &nome, string &conteudo);

bool carregarEstatisticas(vector<Estatistica> &lista);

bool salvarEstatisticas(const vector<Estatistica> &lista);

bool registrarResultadoJogador(const string &nome, bool venceu, int pontos, int fasesConcluidas);

vector<Estatistica> obterRankingEstatisticas();