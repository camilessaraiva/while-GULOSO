#pragma once
#include <vector>
#include "raylib.h"
#include "dados.h"

using namespace std;

void desenharCenario(const Fase &fase, bool portaAberta);

void desenharGuloso(const Guloso &guloso);

void desenharInimigos(const vector<Inimigo> &inimigos);

void desenharProjeteis(const vector<Projetil> &projeteis);

void desenharDoces(const vector<DoceItem> &doces);

void desenharPainel(const Guloso &guloso, int numeroFase, float tempoRestante, int metaDoces);

void desenharMensagem(const char *titulo, const char *subtitulo, Color cor);

int lerDirecaoTeclado();