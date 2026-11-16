#pragma once
#include <vector>
#include "dados.h"

using namespace std;

void inicializarInimigos(int numeroFase, vector<Inimigo> &inimigos);

void atualizarInimigos(vector<Inimigo> &inimigos, const Fase &fase, const Guloso &guloso, float dt);

void inicializarSentinelasEProjeteis(const Fase &fase, vector<Projetil> &projeteis);

void atualizarSentinelasEProjeteis(const Fase &fase, const Guloso &guloso, vector<Projetil> &projeteis, float dt, float &timerSentinela);

bool aplicarObstaculos(Guloso &guloso, Fase &fase, int direcaoUltimoMovimento, bool portaAberta);

bool verificarColisoesInimigos(Guloso &guloso, const vector<Inimigo> &inimigos, vector<Projetil> &projeteis);
