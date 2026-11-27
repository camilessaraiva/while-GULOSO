#include "desenho_cenario.h"
#include <cstdio>
#include <cmath>

using namespace std;

void desenharCenario(const Fase &fase, bool portaAberta, Texture2D boloSprite, Texture2D sorveteInimigoSprite) {
    ClearBackground((Color){245, 240, 235, 255});

    for (int l = 0; l < LINHAS; l++) {
        for (int c = 0; c < COLUNAS; c++) {
            int x = c * TAM_CELULA;
            int y = l * TAM_CELULA;
            int cel = fase.mapa[l][c];

            DrawRectangleLines(x, y, TAM_CELULA, TAM_CELULA, (Color){230, 220, 215, 255});

            if (cel == PAREDE_FIXA) {
                DrawRectangle(x, y, TAM_CELULA, TAM_CELULA, (Color){150, 75, 45, 255});
                DrawRectangleLines(x, y, TAM_CELULA, TAM_CELULA, (Color){110, 50, 30, 255});
                DrawLine(x, y + TAM_CELULA / 2, x + TAM_CELULA, y + TAM_CELULA / 2, (Color){110, 50, 30, 255});
                DrawLine(x + TAM_CELULA / 2, y, x + TAM_CELULA / 2, y + TAM_CELULA / 2, (Color){110, 50, 30, 255});
            } else if (cel == CAMPO_ELETRICO) {
                DrawRectangle(x, y, TAM_CELULA, TAM_CELULA, (Color){210, 130, 40, 220});
                DrawCircle(x + TAM_CELULA / 2, y + TAM_CELULA / 2, (float)TAM_CELULA * 0.15f, (Color){170, 95, 25, 255});
            } else if (cel == ESTACAO_ENERGIA) {
                Rectangle origem = { 
                    0.0f, 0.0f, 
                    (float)boloSprite.width, 
                    (float)boloSprite.height 
                };
                Rectangle destino = { 
                    (float)x, (float)y, 
                    (float)TAM_CELULA, (float)TAM_CELULA 
                };
                Vector2 pivo = { 0.0f, 0.0f };
                DrawTexturePro(boloSprite, origem, destino, pivo, 0.0f, WHITE);
            } else if (cel == SAIDA) {
                Color corSaida = (Color){60, 180, 75, 255};
                DrawRectangle(x, y, TAM_CELULA, TAM_CELULA, corSaida);
                DrawRectangleLines(x + 2, y + 2, TAM_CELULA - 4, TAM_CELULA - 4, WHITE);
                int tamFonte = (int)(TAM_CELULA * 0.6f);
                int largTexto = MeasureText("S", tamFonte);
                DrawText("S", x + (TAM_CELULA - largTexto) / 2, y + (TAM_CELULA - tamFonte) / 2, tamFonte, WHITE);
            } else if (cel == OBS_BASICO) {
                DrawRectangle(x, y, TAM_CELULA, TAM_CELULA, (Color){170, 230, 255, 200});
                DrawLine(x + (int)(TAM_CELULA * 0.15f), y + (int)(TAM_CELULA * 0.75f),
                         x + (int)(TAM_CELULA * 0.85f), y + (int)(TAM_CELULA * 0.25f), WHITE);
            } else if (cel == OBS_INTER) {
                if (portaAberta) {
                    DrawRectangle(x, y, TAM_CELULA, TAM_CELULA, (Color){130, 220, 130, 160});
                    DrawRectangleLines(x + 2, y + 2, TAM_CELULA - 4, TAM_CELULA - 4, DARKGREEN);
                } else {
                    DrawRectangle(x, y, TAM_CELULA, TAM_CELULA, (Color){180, 40, 40, 230});
                    DrawRectangleLines(x, y, TAM_CELULA, TAM_CELULA, MAROON);
                    DrawLine(x + 2, y + 2, x + TAM_CELULA - 2, y + TAM_CELULA - 2, WHITE);
                }
            } else if (cel == OBS_ESPECIAL) {
                Rectangle origem = { 
                    0.0f, 0.0f, 
                    (float)sorveteInimigoSprite.width, 
                    (float)sorveteInimigoSprite.height 
                };
                Rectangle destino = { 
                    (float)x, (float)y, 
                    (float)TAM_CELULA, (float)TAM_CELULA 
                };
                Vector2 pivo = { 0.0f, 0.0f };
                DrawTexturePro(sorveteInimigoSprite, origem, destino, pivo, 0.0f, WHITE);
            } else if (cel == PERIGO) {
                DrawRectangle(x, y, TAM_CELULA, TAM_CELULA, (Color){240, 100, 100, 180});
            }
        }
    }
}

void desenharGuloso(const Guloso &guloso, Texture2D textura) {
    if (guloso.tempoInvulneravel > 0.0f) {
        if ((int)(guloso.tempoInvulneravel * 10) % 2 == 0) return; //pisca qd invulneravel
    }
    float x = (float)(guloso.coluna * TAM_CELULA);
    float y = (float)(guloso.linha * TAM_CELULA);


    Rectangle origem = { 
        0.0f, 0.0f, 
        (float)textura.width, 
        (float)textura.height 
    };

    Rectangle destino = { 
        x, y, 
        (float)TAM_CELULA, (float)TAM_CELULA 
    };

    Vector2 pivo = { 0.0f, 0.0f };

    DrawTexturePro(textura, origem, destino, pivo, 0.0f, WHITE);
}

void desenharInimigos(const vector<Inimigo> &inimigos, Texture2D cupcakeInimigoSprite, Texture2D rocamboleInimigoSprite) {
    for (const auto &ini : inimigos) {
        if (ini.tipo == PATRULHEIRO) {
            float x = (float)(ini.coluna * TAM_CELULA);
            float y = (float)(ini.linha * TAM_CELULA);
            Rectangle origem = { 
                0.0f, 0.0f, 
                (float)cupcakeInimigoSprite.width, 
                (float)cupcakeInimigoSprite.height 
            };
            Rectangle destino = { 
                x, y, 
                (float)TAM_CELULA, (float)TAM_CELULA 
            };
            Vector2 pivo = { 0.0f, 0.0f };
            DrawTexturePro(cupcakeInimigoSprite, origem, destino, pivo, 0.0f, WHITE);
        } else if (ini.tipo == PERSEGUIDOR) {
            float x = (float)(ini.coluna * TAM_CELULA);
            float y = (float)(ini.linha * TAM_CELULA);
            Rectangle origem = { 
                0.0f, 0.0f, 
                (float)rocamboleInimigoSprite.width, 
                (float)rocamboleInimigoSprite.height 
            };
            Rectangle destino = { 
                x, y, 
                (float)TAM_CELULA, (float)TAM_CELULA 
            };
            Vector2 pivo = { 0.0f, 0.0f };
            DrawTexturePro(rocamboleInimigoSprite, origem, destino, pivo, 0.0f, WHITE);
        }
    }
}

void desenharProjeteis(const vector<Projetil> &projeteis, Texture2D geloSprite) {
    for (const auto &p : projeteis) {
        if (!p.ativo) continue;
        Rectangle origem = { 
            0.0f, 0.0f, 
            (float)geloSprite.width, 
            (float)geloSprite.height 
        };
        Rectangle destino = { 
            p.x, p.y, 
            (float)TAM_CELULA, (float)TAM_CELULA 
        };
        Vector2 pivo = { (float)TAM_CELULA / 2.0f, (float)TAM_CELULA / 2.0f };
        DrawTexturePro(geloSprite, origem, destino, pivo, 0.0f, WHITE);
    }
}

void desenharDoces(const vector<DoceItem> &doces, Texture2D doceBala, Texture2D doceChocolate, Texture2D doceRosquinha, Texture2D doceJujuba, Texture2D docePirulito, Texture2D doceDourado) {
    for (const auto &d : doces) {
        if (!d.ativo) continue;

        Texture2D textura;
        switch (d.tipo) {
            case DOCE_BALA:
                textura = doceBala;
                break;
            case DOCE_CHOCOLATE:
                textura = doceChocolate;
                break;
            case DOCE_ROSQUINHA:
                textura = doceRosquinha;
                break;
            case DOCE_JUJUBA:
                textura = doceJujuba;
                break;
            case DOCE_PIRULITO:
                textura = docePirulito;
                break;
            case DOCE_DOURADO:
                textura = doceDourado;
                break;
            default:
                continue;
        }

        float x = (float)(d.coluna * TAM_CELULA);
        float y = (float)(d.linha * TAM_CELULA);

        Rectangle origem = { 
            0.0f, 0.0f, 
            (float)textura.width, 
            (float)textura.height 
        };

        Rectangle destino = { 
            x, y, 
            (float)TAM_CELULA, (float)TAM_CELULA 
        };

        Vector2 pivo = { 0.0f, 0.0f };

        DrawTexturePro(textura, origem, destino, pivo, 0.0f, WHITE);
    }
}

void desenharPainel(const Guloso &guloso, int numeroFase, float tempoRestante, int metaDoces) {
    int px = COLUNAS * TAM_CELULA;
    DrawRectangle(px, 0, PAINEL_LARGURA, ALTURA_JANELA, (Color){45, 30, 40, 255});
    DrawLine(px, 0, px, ALTURA_JANELA, (Color){85, 60, 75, 255});

    DrawText("WHILE (GULOSO)", px + 40, 25, 24, (Color){255, 182, 193, 255});
    DrawText(TextFormat("FASE %d DE %d", numeroFase, NUM_FASES), px + 80, 58, 16, (Color){230, 210, 220, 255});

    DrawText("VIDAS:", px + 25, 95, 15, WHITE);
    for (int i = 0; i < VIDAS_INICIAIS; i++) {
        int hx = px + 100 + i * 26;
        int hy = 102;
        if (i < guloso.vidas) {
            DrawCircle(hx - 4, hy - 2, 5, RED);
            DrawCircle(hx + 4, hy - 2, 5, RED);
            DrawTriangle((Vector2){(float)hx - 9, (float)hy}, (Vector2){(float)hx + 9, (float)hy}, (Vector2){(float)hx, (float)hy + 9}, RED);
        } else {
            DrawCircle(hx - 4, hy - 2, 5, (Color){90, 70, 80, 255});
            DrawCircle(hx + 4, hy - 2, 5, (Color){90, 70, 80, 255});
            DrawTriangle((Vector2){(float)hx - 9, (float)hy}, (Vector2){(float)hx + 9, (float)hy}, (Vector2){(float)hx, (float)hy + 9}, (Color){90, 70, 80, 255});
        }
    }

    DrawText("FOME / SACIEDADE:", px + 25, 130, 15, WHITE);

    float pct = guloso.energia / (float)ENERGIA_MAXIMA;
    if (pct < 0.0f) pct = 0.0f;
    if (pct > 1.0f) pct = 1.0f;

    int fx = px + 60;
    int fy = 160;

    Color corFrango = (pct > 0.6f) ? (Color){205, 115, 45, 255} : ((pct > 0.3f) ? (Color){215, 150, 60, 255} : (Color){180, 70, 40, 255});
    Color corCinza = (Color){80, 70, 75, 220};

    Color corFinalCarne = (pct <= 0.05f) ? corCinza : corFrango;
    Color corFinalOsso = (pct <= 0.05f) ? (Color){100, 95, 100, 255} : (Color){245, 240, 230, 255};

    DrawRectangle(fx + 25, fy - 3, 20, 6, corFinalOsso);
    DrawCircle(fx + 45, fy - 5, 4, corFinalOsso);
    DrawCircle(fx + 45, fy + 5, 4, corFinalOsso);

    DrawCircle(fx - 10, fy, 16, corFinalCarne);
    DrawCircle(fx + 10, fy, 13, corFinalCarne);
    DrawRectangle(fx - 10, fy - 12, 20, 24, corFinalCarne);

    if (pct < 0.95f) {
        float alturaVazia = 32.0f * (1.0f - pct);
        DrawRectangle(fx - 28, fy - 16, 48, (int)alturaVazia, (Color){45, 30, 40, 200});
        DrawCircle(fx - 10, fy, 16, (Color){80, 70, 75, (unsigned char)(180 * (1.0f - pct))});
        DrawCircle(fx + 10, fy, 13, (Color){80, 70, 75, (unsigned char)(180 * (1.0f - pct))});
    }

    DrawRectangle(px + 130, 152, 135, 16, (Color){25, 15, 20, 255});
    DrawRectangleLines(px + 130, 152, 135, 16, (Color){140, 110, 125, 255});
    Color corBarraAux = (pct > 0.5f) ? (Color){210, 130, 50, 255} : ((pct > 0.25f) ? (Color){230, 170, 40, 255} : RED);
    DrawRectangle(px + 132, 154, (int)(131 * pct), 12, corBarraAux);
    DrawText(TextFormat("%d%%", (int)(pct * 100)), px + 175, 153, 13, WHITE);

    int min = (int)tempoRestante / 60;
    int seg = (int)tempoRestante % 60;
    DrawText("TEMPO RESTANTE:", px + 25, 190, 15, WHITE);
    Color corTempo = (tempoRestante < 60.0f) ? RED : (Color){180, 240, 255, 255};
    DrawText(TextFormat("%02d:%02d", min, seg), px + 180, 190, 18, corTempo);

    DrawLine(px + 25, 225, px + 265, 225, (Color){90, 70, 85, 255});

    DrawText("INVENTARIO & DOCES", px + 45, 240, 17, (Color){255, 200, 100, 255});
    DrawText(TextFormat("Doces: %d / %d", guloso.docesColetados, metaDoces), px + 25, 275, 16, WHITE);
    DrawText(TextFormat("Pontuacao: %d pts", guloso.pontos), px + 25, 305, 16, (Color){255, 230, 120, 255});

    DrawText("Doce Dourado:", px + 25, 345, 15, WHITE);
    int dx = px + 180;
    int dy = 352;
    if (guloso.temDoceDourado) {
        DrawCircle(dx, dy, 12, (Color){255, 215, 0, 255});
        DrawCircleLines(dx, dy, 12, WHITE);
        DrawCircle(dx, dy, 6, (Color){255, 245, 140, 255});
        DrawText("OBTIDO!", px + 205, 345, 14, (Color){100, 255, 120, 255});
    } else {
        DrawCircle(dx, dy, 12, (Color){90, 80, 85, 140});
        DrawCircleLines(dx, dy, 12, (Color){120, 110, 115, 140});
        DrawCircle(dx, dy, 6, (Color){100, 95, 100, 140});
        DrawText("PENDENTE", px + 200, 345, 13, (Color){160, 145, 150, 180});
    }

    DrawLine(px + 25, 395, px + 265, 395, (Color){90, 70, 85, 255});

    DrawText("CONTROLES:", px + 25, 415, 15, (Color){200, 190, 200, 255});
    DrawText("• Mover: Setas ou W A S D", px + 25, 440, 13, (Color){180, 175, 180, 255});
    DrawText("• R: Ver regras da fase", px + 25, 465, 13, (Color){180, 175, 180, 255});
    DrawText("• ESC: Menu principal", px + 25, 490, 13, (Color){180, 175, 180, 255});
}

void desenharMensagem(const char *titulo, const char *subtitulo, Color cor) {
    int larg = 500;
    int alt = 180;
    int x = (COLUNAS * TAM_CELULA - larg) / 2;
    int y = (ALTURA_JANELA - alt) / 2;
    DrawRectangle(x, y, larg, alt, (Color){25, 20, 30, 235});
    DrawRectangleLines(x, y, larg, alt, cor);
    int tLargura = MeasureText(titulo, 28);
    DrawText(titulo, x + (larg - tLargura) / 2, y + 35, 28, cor);
    int sLargura = MeasureText(subtitulo, 16);
    DrawText(subtitulo, x + (larg - sLargura) / 2, y + 95, 16, WHITE);
}

int lerDirecaoTeclado() {
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) return CIMA;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) return BAIXO;
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) return ESQUERDA;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) return DIREITA;
    return -1;
}
