#pragma once
#include "dados.h"

void iniciarGuloso(Guloso &guloso, const Fase &fase);

void reiniciarPosicao(Guloso &guloso, const Fase &fase);

bool moverRobo(Guloso &guloso, const Fase &fase, int direcao);