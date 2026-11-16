#include "obstaculos.h"
#include <cmath>
#include <cstdlib>

using namespace std;

void inicializarInimigos(int numeroFase, vector<Inimigo> &inimigos) {
    inimigos.clear();

    if (numeroFase == 2) {
        Inimigo p1;
        p1.linha = 5;
        p1.coluna = 10;
        p1.direcao = DIREITA;
        p1.tipo = PATRULHEIRO;
        p1.alcance = 15;
        p1.minLinha = 5;
        p1.maxLinha = 5;
        p1.minColuna = 5;
        p1.maxColuna = 28;
        p1.timerMovimento = 0.0f;
        inimigos.push_back(p1);

        Inimigo p2;
        p2.linha = 18;
        p2.coluna = 30;
        p2.direcao = ESQUERDA;
        p2.tipo = PATRULHEIRO;
        p2.alcance = 15;
        p2.minLinha = 18;
        p2.maxLinha = 18;
        p2.minColuna = 10;
        p2.maxColuna = 35;
        p2.timerMovimento = 0.0f;
        inimigos.push_back(p2);

        Inimigo p3;
        p3.linha = 12;
        p3.coluna = 20;
        p3.direcao = BAIXO;
        p3.tipo = PATRULHEIRO;
        p3.alcance = 10;
        p3.minLinha = 8;
        p3.maxLinha = 22;
        p3.minColuna = 20;
        p3.maxColuna = 20;
        p3.timerMovimento = 0.0f;
        inimigos.push_back(p3);
    } else if (numeroFase == 3) {
        Inimigo p1;
        p1.linha = 8;
        p1.coluna = 12;
        p1.direcao = BAIXO;
        p1.tipo = PATRULHEIRO;
        p1.alcance = 14;
        p1.minLinha = 4;
        p1.maxLinha = 22;
        p1.minColuna = 12;
        p1.maxColuna = 12;
        p1.timerMovimento = 0.0f;
        inimigos.push_back(p1);

        Inimigo p2;
        p2.linha = 15;
        p2.coluna = 26;
        p2.direcao = DIREITA;
        p2.tipo = PATRULHEIRO;
        p2.alcance = 16;
        p2.minLinha = 15;
        p2.maxLinha = 15;
        p2.minColuna = 10;
        p2.maxColuna = 34;
        p2.timerMovimento = 0.0f;
        inimigos.push_back(p2);

        Inimigo per1;
        per1.linha = 22;
        per1.coluna = 32;
        per1.direcao = ESQUERDA;
        per1.tipo = PERSEGUIDOR;
        per1.alcance = 11;
        per1.minLinha = 0;
        per1.maxLinha = LINHAS - 1;
        per1.minColuna = 0;
        per1.maxColuna = COLUNAS - 1;
        per1.timerMovimento = 0.0f;
        inimigos.push_back(per1);

        Inimigo per2;
        per2.linha = 6;
        per2.coluna = 30;
        per2.direcao = ESQUERDA;
        per2.tipo = PERSEGUIDOR;
        per2.alcance = 10;
        per2.minLinha = 0;
        per2.maxLinha = LINHAS - 1;
        per2.minColuna = 0;
        per2.maxColuna = COLUNAS - 1;
        per2.timerMovimento = 0.0f;
        inimigos.push_back(per2);
    }
}

void atualizarInimigos(vector<Inimigo> &inimigos, const Fase &fase, const Guloso &guloso, float dt) {
    for (auto &ini : inimigos) {
        ini.timerMovimento += dt;

        if (ini.tipo == PATRULHEIRO) {
            if (ini.timerMovimento >= 0.28f) {
                ini.timerMovimento = 0.0f;
                int nl = ini.linha;
                int nc = ini.coluna;

                if (ini.direcao == DIREITA) nc++;
                else if (ini.direcao == ESQUERDA) nc--;
                else if (ini.direcao == BAIXO) nl++;
                else if (ini.direcao == CIMA) nl--;

                bool inverte = false;
                if (nl < ini.minLinha || nl > ini.maxLinha || nc < ini.minColuna || nc > ini.maxColuna) {
                    inverte = true;
                } else if (nl < 0 || nl >= LINHAS || nc < 0 || nc >= COLUNAS) {
                    inverte = true;
                } else if (fase.mapa[nl][nc] == PAREDE_FIXA) {
                    inverte = true;
                }

                if (inverte) {
                    if (ini.direcao == DIREITA) ini.direcao = ESQUERDA;
                    else if (ini.direcao == ESQUERDA) ini.direcao = DIREITA;
                    else if (ini.direcao == CIMA) ini.direcao = BAIXO;
                    else if (ini.direcao == BAIXO) ini.direcao = CIMA;
                } else {
                    ini.linha = nl;
                    ini.coluna = nc;
                }
            }
        } else if (ini.tipo == PERSEGUIDOR) {
            if (ini.timerMovimento >= 0.28f) {
                ini.timerMovimento = 0.0f;
                int distL = abs(guloso.linha - ini.linha);
                int distC = abs(guloso.coluna - ini.coluna);

                bool temVisao = false;
                if (distL + distC <= ini.alcance) {
                    bool bloqueado = false;
                    int passoL = (guloso.linha > ini.linha) ? 1 : ((guloso.linha < ini.linha) ? -1 : 0);
                    int passoC = (guloso.coluna > ini.coluna) ? 1 : ((guloso.coluna < ini.coluna) ? -1 : 0);

                    int checL = ini.linha + passoL;
                    int checC = ini.coluna + passoC;
                    while (checL != guloso.linha || checC != guloso.coluna) {
                        if (checL >= 0 && checL < LINHAS && checC >= 0 && checC < COLUNAS) {
                            if (fase.mapa[checL][checC] == PAREDE_FIXA) {
                                bloqueado = true;
                                break;
                            }
                        }
                        if (checL != guloso.linha) checL += passoL;
                        if (checC != guloso.coluna) checC += passoC;
                    }
                    if (!bloqueado) temVisao = true;
                }

                if (temVisao) {
                    int tentL = ini.linha;
                    int tentC = ini.coluna;

                    if (distC >= distL) {
                        if (guloso.coluna > ini.coluna) tentC++;
                        else if (guloso.coluna < ini.coluna) tentC--;
                        if (tentC < 0 || tentC >= COLUNAS || fase.mapa[tentL][tentC] == PAREDE_FIXA) {
                            tentC = ini.coluna;
                            if (guloso.linha > ini.linha) tentL++;
                            else if (guloso.linha < ini.linha) tentL--;
                        }
                    } else {
                        if (guloso.linha > ini.linha) tentL++;
                        else if (guloso.linha < ini.linha) tentL--;
                        if (tentL < 0 || tentL >= LINHAS || fase.mapa[tentL][tentC] == PAREDE_FIXA) {
                            tentL = ini.linha;
                            if (guloso.coluna > ini.coluna) tentC++;
                            else if (guloso.coluna < ini.coluna) tentC--;
                        }
                    }

                    if (tentL >= 0 && tentL < LINHAS && tentC >= 0 && tentC < COLUNAS) {
                        if (fase.mapa[tentL][tentC] != PAREDE_FIXA) {
                            ini.linha = tentL;
                            ini.coluna = tentC;
                        }
                    }
                }
            }
        }
    }
}

void inicializarSentinelasEProjeteis(const Fase &fase, vector<Projetil> &projeteis) {
    (void)fase;
    projeteis.clear();
}

void atualizarSentinelasEProjeteis(const Fase &fase, const Guloso &guloso, vector<Projetil> &projeteis, float dt, float &timerSentinela) {
    timerSentinela += dt;

    if (timerSentinela >= 0.9f) {
        timerSentinela = 0.0f;

        for (int l = 0; l < LINHAS; l++) {
            for (int c = 0; c < COLUNAS; c++) {
                if (fase.mapa[l][c] == OBS_ESPECIAL) {
                    if (l == guloso.linha && abs(c - guloso.coluna) <= 12) {
                        Projetil p;
                        p.x = c * TAM_CELULA + TAM_CELULA / 2.0f;
                        p.y = l * TAM_CELULA + TAM_CELULA / 2.0f;
                        p.vx = (guloso.coluna > c) ? 220.0f : -220.0f;
                        p.vy = 0.0f;
                        p.ativo = true;
                        projeteis.push_back(p);
                    } else if (c == guloso.coluna && abs(l - guloso.linha) <= 12) {
                        Projetil p;
                        p.x = c * TAM_CELULA + TAM_CELULA / 2.0f;
                        p.y = l * TAM_CELULA + TAM_CELULA / 2.0f;
                        p.vx = 0.0f;
                        p.vy = (guloso.linha > l) ? 220.0f : -220.0f;
                        p.ativo = true;
                        projeteis.push_back(p);
                    }
                }
            }
        }
    }

    for (auto &p : projeteis) {
        if (!p.ativo) continue;
        p.x += p.vx * dt;
        p.y += p.vy * dt;

        int celL = (int)(p.y / TAM_CELULA);
        int celC = (int)(p.x / TAM_CELULA);

        if (celL < 0 || celL >= LINHAS || celC < 0 || celC >= COLUNAS) {
            p.ativo = false;
        } else if (fase.mapa[celL][celC] == PAREDE_FIXA) {
            p.ativo = false;
        }
    }
}

bool aplicarObstaculos(Guloso &guloso, Fase &fase, int direcaoUltimoMovimento, bool portaAberta) {
    int cel = fase.mapa[guloso.linha][guloso.coluna];

    if (cel == OBS_INTER && !portaAberta) {
        return false;
    }

    if (cel == OBS_BASICO) {
        int dl = 0, dc = 0;
        if (direcaoUltimoMovimento == CIMA) dl = -1;
        else if (direcaoUltimoMovimento == BAIXO) dl = 1;
        else if (direcaoUltimoMovimento == ESQUERDA) dc = -1;
        else if (direcaoUltimoMovimento == DIREITA) dc = 1;

        for (int i = 0; i < 2; i++) {
            int nl = guloso.linha + dl;
            int nc = guloso.coluna + dc;
            if (nl >= 0 && nl < LINHAS && nc >= 0 && nc < COLUNAS) {
                if (fase.mapa[nl][nc] != PAREDE_FIXA && (fase.mapa[nl][nc] != OBS_INTER || portaAberta)) {
                    guloso.linha = nl;
                    guloso.coluna = nc;
                } else break;
            } else break;
        }
    }

    if (cel == ESTACAO_ENERGIA) {
        guloso.energia = (float)ENERGIA_MAXIMA;
        fase.mapa[guloso.linha][guloso.coluna] = LIVRE;
    }

    return true;
}

bool verificarColisoesInimigos(Guloso &guloso, const vector<Inimigo> &inimigos, vector<Projetil> &projeteis) {
    if (guloso.tempoInvulneravel > 0.0f) return false;

    for (const auto &ini : inimigos) {
        if (ini.linha == guloso.linha && ini.coluna == guloso.coluna) {
            guloso.vidas--;
            guloso.tempoInvulneravel = 1.6f;
            return true;
        }
    }

    float gx = guloso.coluna * TAM_CELULA + TAM_CELULA / 2.0f;
    float gy = guloso.linha * TAM_CELULA + TAM_CELULA / 2.0f;

    for (auto &p : projeteis) {
        if (!p.ativo) continue;
        float dx = p.x - gx;
        float dy = p.y - gy;
        if (dx * dx + dy * dy <= (TAM_CELULA * 0.5f) * (TAM_CELULA * 0.5f)) {
            p.ativo = false;
            guloso.vidas--;
            guloso.tempoInvulneravel = 1.6f;
            return true;
        }
    }

    return false;
}
