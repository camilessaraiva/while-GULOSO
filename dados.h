#pragma once // acho que podemos utilizar isso para evitar mais de uma inclusão do mesmo arquivo lá no main.cpp


const int LINHAS = 30;
const int COLUNAS = 40;


const int TAM_CELULA = 20;
const int PAINEL_LARGURA = 300;
const int LARGURA_JANELA = COLUNAS * TAM_CELULA + PAINEL_LARGURA; 
const int ALTURA_JANELA = LINHAS * TAM_CELULA;                    

// obrigatoriedades de estatisticas.dat
const int VIDAS_INICIAIS = 3;
const int ENERGIA_MAXIMA = 100;
const int NUM_FASES = 3;

// Codigos das celulas (cada digito do arquivo faseN.txt)
// 0 a 6 sao definidos pela professora; 7, 8 e 9 sao nossos.
enum Celula {
    LIVRE = 0,
    PAREDE_FIXA = 1,
    CAMPO_ELETRICO = 2,
    ESTACAO_ENERGIA = 3,
    SAIDA = 4,
    OBS_ESPECIAL = 5,
    PERIGO = 6,
    INICIO = 7,        // pensei em ser a posicao inicial do robo, depois de lida ficaria livre
    OBS_BASICO = 8,
    OBS_INTER = 9
};

enum Direcao { CIMA, BAIXO, ESQUERDA, DIREITA };

enum TipoInimigo { PATRULHEIRO, PERSEGUIDOR, SENTINELA };

struct Guloso {
    int linha;
    int coluna;
    int energia;
    int vidas;
    int pontos;
};

// array só para inimigos, os personagens moveis ficam fora da matriz
struct Inimigo {
    int linha;
    int coluna;
    int direcao;
    int tipo;     // inimigo
    int alcance;  // limite até onde o inimigo vai no mapinha
};

struct Fase {
    int mapa[LINHAS][COLUNAS];
    int inicioLinha;
    int inicioColuna;
};

// arquivo binario 
struct Estatistica {
    char nome[40];
    int partidasJogadas;
    int vitorias;
    int derrotas;
    int maiorPontuacao;
    int fasesConcluidas;
};