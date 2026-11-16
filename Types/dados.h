#pragma once

const int LINHAS = 30;
const int COLUNAS = 40;

const int TAM_CELULA = 20;
const int PAINEL_LARGURA = 300;
const int LARGURA_JANELA = COLUNAS * TAM_CELULA + PAINEL_LARGURA; 
const int ALTURA_JANELA = LINHAS * TAM_CELULA;                    

const int VIDAS_INICIAIS = 3;
const int ENERGIA_MAXIMA = 100;
const int NUM_FASES = 3;
const float TEMPO_FASE_SEGUNDOS = 300.0f;

enum Celula {
    LIVRE = 0,
    PAREDE_FIXA = 1,
    CAMPO_ELETRICO = 2,
    ESTACAO_ENERGIA = 3,
    SAIDA = 4,
    OBS_ESPECIAL = 5,
    PERIGO = 6,
    INICIO = 7,
    OBS_BASICO = 8,
    OBS_INTER = 9
};

enum Direcao { CIMA, BAIXO, ESQUERDA, DIREITA };

enum TipoInimigo { PATRULHEIRO, PERSEGUIDOR, SENTINELA };

enum TipoDoce {
    DOCE_NENHUM = 0,
    DOCE_BALA,
    DOCE_CHOCOLATE,
    DOCE_ROSQUINHA,
    DOCE_CUPCAKE,
    DOCE_BOLO,
    DOCE_DOURADO
};

struct DoceItem {
    int linha;
    int coluna;
    TipoDoce tipo;
    bool ativo;
};

struct Projetil {
    float x;
    float y;
    float vx;
    float vy;
    bool ativo;
};

struct Guloso {
    int linha;
    int coluna;
    float energia;
    int vidas;
    int pontos;
    int docesColetados;
    bool temDoceDourado;
    float tempoInvulneravel;
    float tempoNoCaramelo;
    float delayMovimento;
};

struct Inimigo {
    int linha;
    int coluna;
    int direcao;
    int tipo;
    int alcance;
    int minLinha;
    int maxLinha;
    int minColuna;
    int maxColuna;
    float timerMovimento;
};

struct Fase {
    int mapa[LINHAS][COLUNAS];
    int inicioLinha;
    int inicioColuna;
    int saidaLinha;
    int saidaColuna;
    int metaDoces;
};

struct Estatistica {
    char nome[40];
    int partidasJogadas;
    int vitorias;
    int derrotas;
    int maiorPontuacao;
    int fasesConcluidas;
};