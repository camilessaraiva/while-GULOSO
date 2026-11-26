#pragma once
#include <string>
#include <vector>
#include "raylib.h"
#include "dados.h"

using namespace std;

enum EstadoJogo {
    ESTADO_MENU,
    ESTADO_PERGUNTA_PRIMEIRA_VEZ,
    ESTADO_CUTSCENE,
    ESTADO_TUTORIAL,
    ESTADO_JOGANDO,
    ESTADO_PAUSA,
    ESTADO_CONFIGURACOES,
    ESTADO_REGRAS,
    ESTADO_RANKING,
    ESTADO_VITORIA,
    ESTADO_DERROTA,
    ESTADO_GAME_OVER
};

struct Jogo {
    EstadoJogo estado;
    int numeroFase;
    Fase faseAtual;
    Guloso guloso;
    Texture2D gulosoSprite;
    Texture2D DoceDouradoSprite;
    Texture2D pirulitoSprite;
    Texture2D balaSprite;
    Texture2D chocolateSprite;
    Texture2D jujubaSprite;
    Texture2D donutSprite;
    Texture2D boloSprite;
    Texture2D cupcakeInimigoSprite;
    vector<Inimigo> inimigos;
    vector<Projetil> projeteis;
    vector<DoceItem> doces;
    float tempoFase;
    float timerPorta;
    bool portaAberta;
    float timerSentinela;
    float timerSpawnDoces;
    int indiceCutscene;
    int totalCutscenes;
    string nomeJogador;
    bool digitandoNome;
    string mensagemErro;
    bool deveFechar;
    Sound somClick;
    bool somCarregado;
    Music musicaMenu;
    bool musicaCarregada;
    float volumeMusica;
    float volumeEfeitos;
    float tempoPressionadoBackspace;
    float tempoRepeticaoBackspace;
    bool arrastandoMusica;
    bool arrastandoEfeitos;
};

void inicializarJogo(Jogo &jogo);

void iniciarFase(Jogo &jogo, int faseNum);

void atualizarJogo(Jogo &jogo, float dt);

void desenharJogo(const Jogo &jogo);

void finalizarJogo(Jogo &jogo);
