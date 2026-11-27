#pragma once
#include <vector>
#include "raylib.h"
#include "dados.h"

using namespace std;

void desenharCenario(const Fase &fase, bool portaAberta, Texture2D boloSprite, Texture2D sorveteInimigoSprite);

void desenharGuloso(const Guloso &guloso, Texture2D textura);

void desenharInimigos(const vector<Inimigo> &inimigos, Texture2D cupcakeInimigoSprite, Texture2D rocamboleInimigoSprite);

void desenharProjeteis(const vector<Projetil> &projeteis, Texture2D geloSprite);

void desenharDoces(const vector<DoceItem> &doces, Texture2D doceBala, Texture2D doceChocolate, Texture2D doceRosquinha, Texture2D doceJujuba, Texture2D docePirulito, Texture2D doceDourado);

void desenharPainel(const Guloso &guloso, int numeroFase, float tempoRestante, int metaDoces);

void desenharMensagem(const char *titulo, const char *subtitulo, Color cor);

int lerDirecaoTeclado();