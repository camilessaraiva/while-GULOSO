#pragma once
#include "raylib.h"
#include "dados.h"

void desenharCenario(const Fase &fase);

void desenharGuloso(const Guloso &guloso);

void desenharPainel(const Guloso &guloso, int numeroFase);

void desenharMensagem(const char *titulo, const char *subtitulo, Color cor);

int lerDirecao();