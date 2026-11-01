#include "guloso.h"

void iniciarGuloso(Guloso &guloso, const Fase &fase) {
    guloso.linha = fase.inicioLinha;
    guloso.coluna = fase.inicioColuna;
    guloso.energia = ENERGIA_MAXIMA;
    guloso.vidas = VIDAS_INICIAIS;
    guloso.pontos = 0;
}

void reiniciarPosicao(Guloso &guloso, const Fase &fase) {
    guloso.linha = fase.inicioLinha;
    guloso.coluna = fase.inicioColuna;
    guloso.energia = ENERGIA_MAXIMA;
}

bool moverGuloso(Guloso &guloso, const Fase &fase, int direcao) {
    int nl = guloso.linha;
    int nc = guloso.coluna;

    switch (direcao) {
        case CIMA:     nl--; break;
        case BAIXO:    nl++; break;
        case ESQUERDA: nc--; break;
        case DIREITA:  nc++; break;
    }

    // nao pode sair da matriz
    if (nl < 0 || nl >= LINHAS || nc < 0 || nc >= COLUNAS) return false;
    // nao pode atravessar parede
    if (fase.mapa[nl][nc] == PAREDE_FIXA) return false;

    guloso.linha = nl;
    guloso.coluna = nc;
    return true;
}