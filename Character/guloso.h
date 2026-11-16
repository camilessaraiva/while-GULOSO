#pragma once
#include "dados.h"

void iniciarGuloso(Guloso &guloso, const Fase &fase);

void reiniciarPosicao(Guloso &guloso, const Fase &fase);

bool moverGuloso(Guloso &guloso, const Fase &fase, int direcao);

bool moverRobo(Guloso &guloso, const Fase &fase, int direcao);