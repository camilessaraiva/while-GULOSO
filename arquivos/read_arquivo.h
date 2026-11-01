#pragma once
#include <string>
#include "dados.h"

using namespace std;

bool carregarFase(const string &nome, Fase &fase, string &erro);

bool chegouSaida(const Fase &fase);

bool lerTextoArquivo(const string &nome, string &conteudo);