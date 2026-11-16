#include "guloso.h"

void iniciarGuloso(Guloso &guloso, const Fase &fase) {
    guloso.linha = fase.inicioLinha;
    guloso.coluna = fase.inicioColuna;
    guloso.energia = (float)ENERGIA_MAXIMA;
    guloso.vidas = VIDAS_INICIAIS;
    guloso.pontos = 0;
    guloso.docesColetados = 0;
    guloso.temDoceDourado = false;
    guloso.tempoInvulneravel = 0.0f;
    guloso.tempoNoCaramelo = 0.0f;
    guloso.delayMovimento = 0.0f;
}

void reiniciarPosicao(Guloso &guloso, const Fase &fase) {
    guloso.linha = fase.inicioLinha;
    guloso.coluna = fase.inicioColuna;
    guloso.energia = (float)ENERGIA_MAXIMA;
    guloso.tempoNoCaramelo = 0.0f;
    guloso.delayMovimento = 0.0f;
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

    if (nl < 0 || nl >= LINHAS || nc < 0 || nc >= COLUNAS) return false;
    if (fase.mapa[nl][nc] == PAREDE_FIXA) return false;

    guloso.linha = nl;
    guloso.coluna = nc;
    return true;
}

bool moverRobo(Guloso &guloso, const Fase &fase, int direcao) {
    return moverGuloso(guloso, fase, direcao);
}