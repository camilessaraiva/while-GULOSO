#include "jogo.h"
#include "read_arquivo.h"
#include "desenho_cenario.h"
#include "guloso.h"
#include "obstaculos.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

static void tentarSpawnDoce(Jogo &jogo) {
    int ativos = 0;
    for (const auto &d : jogo.doces) {
        if (d.ativo) ativos++;
    }
    if (ativos >= 15) return;

    for (int tent = 0; tent < 30; tent++) {
        int l = 1 + rand() % (LINHAS - 2);
        int c = 1 + rand() % (COLUNAS - 2);

        if (jogo.faseAtual.mapa[l][c] == LIVRE && (l != jogo.guloso.linha || c != jogo.guloso.coluna)) {
            bool celOcupada = false;
            for (const auto &d : jogo.doces) {
                if (d.ativo && d.linha == l && d.coluna == c) {
                    celOcupada = true;
                    break;
                }
            }
            if (!celOcupada) {
                TipoDoce tipo = DOCE_BALA;
                if (!jogo.guloso.temDoceDourado && (jogo.guloso.docesColetados >= jogo.faseAtual.metaDoces || jogo.tempoFase < 240.0f)) {
                    bool jaTemDouradoNoMapa = false;
                    for (const auto &d : jogo.doces) {
                        if (d.ativo && d.tipo == DOCE_DOURADO) {
                            jaTemDouradoNoMapa = true;
                            break;
                        }
                    }
                    if (!jaTemDouradoNoMapa) {
                        tipo = DOCE_DOURADO;
                        if (jogo.numeroFase == 2) {
                            l = 6;
                            c = 22;
                        }
                    }
                }

                if (tipo != DOCE_DOURADO) {
                    int r = rand() % 100;
                    if (r < 45) tipo = DOCE_BALA;
                    else if (r < 70) tipo = DOCE_CHOCOLATE;
                    else if (r < 85) tipo = DOCE_ROSQUINHA;
                    else if (r < 95) tipo = DOCE_JUJUBA;
                    else tipo = DOCE_PIRULITO;
                }

                DoceItem novo;
                novo.linha = l;
                novo.coluna = c;
                novo.tipo = tipo;
                novo.ativo = true;
                jogo.doces.push_back(novo);
                break;
            }
        }
    }
}

void inicializarJogo(Jogo &jogo) {
    jogo.estado = ESTADO_MENU;
    jogo.numeroFase = 1;
    jogo.tempoFase = TEMPO_FASE_SEGUNDOS;
    jogo.timerPorta = 0.0f;
    jogo.portaAberta = true;
    jogo.timerSentinela = 0.0f;
    jogo.timerSpawnDoces = 0.0f;
    jogo.indiceCutscene = 0;
    jogo.totalCutscenes = 3;
    jogo.nomeJogador = "Jogador";
    jogo.digitandoNome = false;
    jogo.mensagemErro = "";
    jogo.deveFechar = false;
    jogo.volumeMusica = 0.35f;
    jogo.volumeEfeitos = 0.60f;
    jogo.tempoPressionadoBackspace = 0.0f;
    jogo.tempoRepeticaoBackspace = 0.0f;
    jogo.arrastandoMusica = false;
    jogo.arrastandoEfeitos = false;
    jogo.gulosoSprite = LoadTexture("Assets/images/guloso.png");
    GenTextureMipmaps(&jogo.gulosoSprite);
    SetTextureFilter(jogo.gulosoSprite, TEXTURE_FILTER_TRILINEAR); //tira o efeito pixelizado da imagem
    jogo.DoceDouradoSprite = LoadTexture("Assets/images/dourado.png");
    GenTextureMipmaps(&jogo.DoceDouradoSprite);
    SetTextureFilter(jogo.DoceDouradoSprite, TEXTURE_FILTER_TRILINEAR);
    jogo.pirulitoSprite = LoadTexture("Assets/images/pirulito.png");
    GenTextureMipmaps(&jogo.pirulitoSprite);
    SetTextureFilter(jogo.pirulitoSprite, TEXTURE_FILTER_TRILINEAR);
    jogo.balaSprite = LoadTexture("Assets/images/bala.png");
    GenTextureMipmaps(&jogo.balaSprite);
    SetTextureFilter(jogo.balaSprite, TEXTURE_FILTER_TRILINEAR);
    jogo.chocolateSprite = LoadTexture("Assets/images/choco.png");
    GenTextureMipmaps(&jogo.chocolateSprite);
    SetTextureFilter(jogo.chocolateSprite, TEXTURE_FILTER_TRILINEAR);
    jogo.jujubaSprite = LoadTexture("Assets/images/jujuba.png");
    GenTextureMipmaps(&jogo.jujubaSprite);
    SetTextureFilter(jogo.jujubaSprite, TEXTURE_FILTER_TRILINEAR);
    jogo.donutSprite = LoadTexture("Assets/images/donut.png");
    GenTextureMipmaps(&jogo.donutSprite);
    SetTextureFilter(jogo.donutSprite, TEXTURE_FILTER_TRILINEAR);
    jogo.boloSprite = LoadTexture("Assets/images/bolo.png");
    GenTextureMipmaps(&jogo.boloSprite);
    SetTextureFilter(jogo.boloSprite, TEXTURE_FILTER_TRILINEAR);
    jogo.cupcakeInimigoSprite = LoadTexture("Assets/images/cupcakedomal.png");
    GenTextureMipmaps(&jogo.cupcakeInimigoSprite);
    SetTextureFilter(jogo.cupcakeInimigoSprite, TEXTURE_FILTER_TRILINEAR);
    jogo.sorveteInimigoSprite = LoadTexture("Assets/images/sorveteinimigo.png");
    GenTextureMipmaps(&jogo.sorveteInimigoSprite);
    SetTextureFilter(jogo.sorveteInimigoSprite, TEXTURE_FILTER_TRILINEAR);
    jogo.rocamboleInimigoSprite = LoadTexture("Assets/images/rocamboledomal.png");
    GenTextureMipmaps(&jogo.rocamboleInimigoSprite);
    SetTextureFilter(jogo.rocamboleInimigoSprite, TEXTURE_FILTER_TRILINEAR);
    jogo.geloSprite = LoadTexture("Assets/images/gelo.png");
    GenTextureMipmaps(&jogo.geloSprite);
    SetTextureFilter(jogo.geloSprite, TEXTURE_FILTER_TRILINEAR);


    if (FileExists("Assets/sounds/click.wav")) {
        jogo.somClick = LoadSound("Assets/sounds/click.wav");
        SetSoundVolume(jogo.somClick, jogo.volumeEfeitos);
        jogo.somCarregado = true;
    } else if (FileExists("click.wav")) {
        jogo.somClick = LoadSound("click.wav");
        SetSoundVolume(jogo.somClick, jogo.volumeEfeitos);
        jogo.somCarregado = true;
    }

    if (FileExists("Assets/sounds/musica_menu.wav")) {
        jogo.musicaMenu = LoadMusicStream("Assets/sounds/musica_menu.wav");
        SetMusicVolume(jogo.musicaMenu, jogo.volumeMusica);
        jogo.musicaCarregada = true;
    } else if (FileExists("musica_menu.wav")) {
        jogo.musicaMenu = LoadMusicStream("musica_menu.wav");
        SetMusicVolume(jogo.musicaMenu, jogo.volumeMusica);
        jogo.musicaCarregada = true;
    }

    srand((unsigned int)time(NULL));
}

static void tocarClick(const Jogo &jogo) {
    if (jogo.somCarregado) {
        SetSoundVolume(jogo.somClick, jogo.volumeEfeitos);
        PlaySound(jogo.somClick);
    }
}

void finalizarJogo(Jogo &jogo) {
    if (jogo.somCarregado) {
        UnloadSound(jogo.somClick);
        jogo.somCarregado = false;
    }
    if (jogo.musicaCarregada) {
        UnloadMusicStream(jogo.musicaMenu);
        jogo.musicaCarregada = false;
    }
    UnloadTexture(jogo.gulosoSprite);
    UnloadTexture(jogo.DoceDouradoSprite);
    UnloadTexture(jogo.pirulitoSprite);
    UnloadTexture(jogo.balaSprite);
    UnloadTexture(jogo.chocolateSprite);
    UnloadTexture(jogo.jujubaSprite);
    UnloadTexture(jogo.donutSprite);
    UnloadTexture(jogo.boloSprite);
    UnloadTexture(jogo.cupcakeInimigoSprite);
    UnloadTexture(jogo.sorveteInimigoSprite);
    UnloadTexture(jogo.rocamboleInimigoSprite);
    UnloadTexture(jogo.geloSprite);
}

void iniciarFase(Jogo &jogo, int faseNum) {
    jogo.numeroFase = faseNum;
    string arq = "fase" + to_string(faseNum) + ".txt";
    string erro;

    if (!carregarFase(arq, jogo.faseAtual, erro)) {
        jogo.mensagemErro = erro;
        jogo.estado = ESTADO_MENU;
        return;
    }

    if (faseNum == 1) {
        jogo.faseAtual.metaDoces = 15;
        iniciarGuloso(jogo.guloso, jogo.faseAtual);
    } else {
        reiniciarPosicao(jogo.guloso, jogo.faseAtual);
        jogo.guloso.docesColetados = 0;
        jogo.guloso.temDoceDourado = false;
        if (faseNum == 2) jogo.faseAtual.metaDoces = 22;
        else if (faseNum == 3) jogo.faseAtual.metaDoces = 30;
    }

    jogo.tempoFase = TEMPO_FASE_SEGUNDOS;
    jogo.timerPorta = 0.0f;
    jogo.portaAberta = true;
    jogo.timerSentinela = 0.0f;
    jogo.timerSpawnDoces = 0.0f;
    jogo.doces.clear();

    inicializarInimigos(faseNum, jogo.inimigos);
    inicializarSentinelasEProjeteis(jogo.faseAtual, jogo.projeteis);

    for (int i = 0; i < 8; i++) {
        tentarSpawnDoce(jogo);
    }

    jogo.estado = ESTADO_JOGANDO;
}

void atualizarJogo(Jogo &jogo, float dt) {
    if (jogo.musicaCarregada) {
        bool noMenuGeral = (jogo.estado == ESTADO_MENU || jogo.estado == ESTADO_REGRAS || jogo.estado == ESTADO_RANKING || jogo.estado == ESTADO_CONFIGURACOES);
        if (noMenuGeral) {
            if (!IsMusicStreamPlaying(jogo.musicaMenu)) {
                PlayMusicStream(jogo.musicaMenu);
            }
            UpdateMusicStream(jogo.musicaMenu);
        } else {
            if (IsMusicStreamPlaying(jogo.musicaMenu)) {
                StopMusicStream(jogo.musicaMenu);
            }
        }
    }

    if (jogo.estado == ESTADO_MENU) {
        Vector2 mouse = GetMousePosition();
        int cx = (LARGURA_JANELA - 400) / 2;
        int bx = (LARGURA_JANELA - 200) / 2;
        Rectangle caixaTexto = {(float)cx, 210.0f, 400.0f, 45.0f};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, caixaTexto)) {
                jogo.digitandoNome = true;
                if (jogo.nomeJogador == "Jogador") {
                    jogo.nomeJogador = "";
                }
            } else {
                jogo.digitandoNome = false;
            }
        }

        if (jogo.digitandoNome) {
            bool teclaApagar = IsKeyDown(KEY_BACKSPACE) || IsKeyDown(KEY_DELETE);
            if (teclaApagar) {
                if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_DELETE)) {
                    if (!jogo.nomeJogador.empty()) jogo.nomeJogador.pop_back();
                    jogo.tempoPressionadoBackspace = 0.0f;
                    jogo.tempoRepeticaoBackspace = 0.0f;
                } else {
                    jogo.tempoPressionadoBackspace += dt;
                    if (jogo.tempoPressionadoBackspace >= 0.35f) {
                        jogo.tempoRepeticaoBackspace += dt;
                        if (jogo.tempoRepeticaoBackspace >= 0.04f) {
                            if (!jogo.nomeJogador.empty()) jogo.nomeJogador.pop_back();
                            jogo.tempoRepeticaoBackspace = 0.0f;
                        }
                    }
                }
            } else {
                jogo.tempoPressionadoBackspace = 0.0f;
                jogo.tempoRepeticaoBackspace = 0.0f;
            }

            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (jogo.nomeJogador.size() < 25)) {
                    jogo.nomeJogador += (char)key;
                }
                key = GetCharPressed();
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){(float)bx, 285.0f, 200.0f, 38.0f})) {
                tocarClick(jogo);
                if (jogo.nomeJogador.empty()) jogo.nomeJogador = "Jogador";
                jogo.estado = ESTADO_PERGUNTA_PRIMEIRA_VEZ;
            } else if (CheckCollisionPointRec(mouse, (Rectangle){(float)bx, 335.0f, 200.0f, 38.0f})) {
                tocarClick(jogo);
                jogo.estado = ESTADO_CONFIGURACOES;
            } else if (CheckCollisionPointRec(mouse, (Rectangle){(float)bx, 385.0f, 200.0f, 38.0f})) {
                tocarClick(jogo);
                jogo.estado = ESTADO_REGRAS;
            } else if (CheckCollisionPointRec(mouse, (Rectangle){(float)bx, 435.0f, 200.0f, 38.0f})) {
                tocarClick(jogo);
                jogo.estado = ESTADO_RANKING;
            } else if (CheckCollisionPointRec(mouse, (Rectangle){(float)bx, 485.0f, 200.0f, 38.0f})) {
                tocarClick(jogo);
                jogo.deveFechar = true;
            }
        }
        return;
    }

    if (jogo.estado == ESTADO_CONFIGURACOES) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            tocarClick(jogo);
            jogo.arrastandoMusica = false;
            jogo.arrastandoEfeitos = false;
            jogo.estado = ESTADO_MENU;
            return;
        }

        int cx = (LARGURA_JANELA - 600) / 2;
        int cy = 140;
        int sx = cx + 80;
        int bx = (LARGURA_JANELA - 200) / 2;

        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){(float)sx - 10, (float)cy + 65, 360, 42})) {
                jogo.arrastandoMusica = true;
                float v = (mouse.x - sx) / 340.0f;
                if (v < 0.0f) v = 0.0f;
                if (v > 1.0f) v = 1.0f;
                jogo.volumeMusica = v;
                if (jogo.musicaCarregada) SetMusicVolume(jogo.musicaMenu, jogo.volumeMusica);
            } else if (CheckCollisionPointRec(mouse, (Rectangle){(float)sx - 10, (float)cy + 165, 360, 42})) {
                jogo.arrastandoEfeitos = true;
                float v = (mouse.x - sx) / 340.0f;
                if (v < 0.0f) v = 0.0f;
                if (v > 1.0f) v = 1.0f;
                jogo.volumeEfeitos = v;
                if (jogo.somCarregado) SetSoundVolume(jogo.somClick, jogo.volumeEfeitos);
            } else if (CheckCollisionPointRec(mouse, (Rectangle){(float)bx, (float)cy + 290, 200, 42})) {
                tocarClick(jogo);
                jogo.arrastandoMusica = false;
                jogo.arrastandoEfeitos = false;
                jogo.estado = ESTADO_MENU;
                return;
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (jogo.arrastandoMusica) {
                float v = (mouse.x - sx) / 340.0f;
                if (v < 0.0f) v = 0.0f;
                if (v > 1.0f) v = 1.0f;
                jogo.volumeMusica = v;
                if (jogo.musicaCarregada) SetMusicVolume(jogo.musicaMenu, jogo.volumeMusica);
            } else if (jogo.arrastandoEfeitos) {
                float v = (mouse.x - sx) / 340.0f;
                if (v < 0.0f) v = 0.0f;
                if (v > 1.0f) v = 1.0f;
                jogo.volumeEfeitos = v;
                if (jogo.somCarregado) SetSoundVolume(jogo.somClick, jogo.volumeEfeitos);
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            jogo.arrastandoMusica = false;
            jogo.arrastandoEfeitos = false;
        }
        return;
    }

    if (jogo.estado == ESTADO_PERGUNTA_PRIMEIRA_VEZ) {
        int bSimX = (LARGURA_JANELA - 340) / 2;
        int bNaoX = bSimX + 200;
        int by = 320;

        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){(float)bSimX, (float)by, 140, 50})) {
                tocarClick(jogo);
                jogo.indiceCutscene = 0;
                jogo.estado = ESTADO_CUTSCENE;
            } else if (CheckCollisionPointRec(mouse, (Rectangle){(float)bNaoX, (float)by, 140, 50})) {
                tocarClick(jogo);
                iniciarFase(jogo, 1);
            }
        }
        return;
    }

    if (jogo.estado == ESTADO_CUTSCENE) {
        int cx = (LARGURA_JANELA - 900) / 2;
        int cy = 70;
        int bx = cx + 900 - 90 - 40;
        int by = cy + 430 - 50 - 20;

        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){(float)bx, (float)by, 90, 50})) {
                tocarClick(jogo);
                jogo.indiceCutscene++;
                if (jogo.indiceCutscene >= jogo.totalCutscenes) {
                    jogo.estado = ESTADO_TUTORIAL;
                }
            }
        }
        return;
    }

    if (jogo.estado == ESTADO_TUTORIAL) {
        int cy = 60;
        int bx = (LARGURA_JANELA - 200) / 2;
        int by = cy + 410;

        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){(float)bx, (float)by, 200, 45})) {
                tocarClick(jogo);
                iniciarFase(jogo, 1);
            }
        }
        return;
    }

    if (jogo.estado == ESTADO_REGRAS || jogo.estado == ESTADO_RANKING) {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
            tocarClick(jogo);
            jogo.estado = ESTADO_MENU;
        }
        int bx = (LARGURA_JANELA - 200) / 2;
        int by = 530;
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){(float)bx, (float)by, 200, 40})) {
                tocarClick(jogo);
                jogo.estado = ESTADO_MENU;
            }
        }
        return;
    }

    if (jogo.estado == ESTADO_VITORIA || jogo.estado == ESTADO_GAME_OVER) {
        if (IsKeyPressed(KEY_R)) {
            tocarClick(jogo);
            iniciarFase(jogo, jogo.numeroFase);
            return;
        }
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
            tocarClick(jogo);
            jogo.estado = ESTADO_RANKING;
            return;
        }

        int boardW = COLUNAS * TAM_CELULA;
        int b1X = (boardW - 310) / 2;
        int b2X = b1X + 170;
        int by = (ALTURA_JANELA - 180) / 2 + 115;

        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){(float)b1X, (float)by, 140, 45})) {
                tocarClick(jogo);
                iniciarFase(jogo, jogo.numeroFase);
            } else if (CheckCollisionPointRec(mouse, (Rectangle){(float)b2X, (float)by, 140, 45})) {
                tocarClick(jogo);
                jogo.estado = ESTADO_RANKING;
            }
        }
        return;
    }

    if (jogo.estado == ESTADO_PAUSA) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            tocarClick(jogo);
            jogo.estado = ESTADO_JOGANDO;
            return;
        }

        int bx = (LARGURA_JANELA - 220) / 2;
        int cy = (ALTURA_JANELA - 350) / 2;

        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){(float)bx, (float)cy + 90, 220, 45})) {
                tocarClick(jogo);
                jogo.estado = ESTADO_JOGANDO;
            } else if (CheckCollisionPointRec(mouse, (Rectangle){(float)bx, (float)cy + 150, 220, 45})) {
                tocarClick(jogo);
                iniciarFase(jogo, jogo.numeroFase);
            } else if (CheckCollisionPointRec(mouse, (Rectangle){(float)bx, (float)cy + 210, 220, 45})) {
                tocarClick(jogo);
                jogo.estado = ESTADO_MENU;
            } else if (CheckCollisionPointRec(mouse, (Rectangle){(float)bx, (float)cy + 270, 220, 45})) {
                tocarClick(jogo);
                jogo.deveFechar = true;
            }
        }
        return;
    }

    if (jogo.estado != ESTADO_JOGANDO) return;

    if (IsKeyPressed(KEY_ESCAPE)) {
        jogo.estado = ESTADO_PAUSA;
        return;
    }

    jogo.tempoFase -= dt;
    if (jogo.tempoFase <= 0.0f) {
        jogo.guloso.vidas--;
        if (jogo.guloso.vidas <= 0) {
            registrarResultadoJogador(jogo.nomeJogador, false, jogo.guloso.pontos, jogo.numeroFase - 1);
            jogo.estado = ESTADO_GAME_OVER;
            return;
        } else {
            jogo.tempoFase = TEMPO_FASE_SEGUNDOS;
            reiniciarPosicao(jogo.guloso, jogo.faseAtual);
        }
    }

    jogo.timerPorta += dt;
    if (jogo.timerPorta >= 4.0f) {
        jogo.timerPorta = 0.0f;
        jogo.portaAberta = !jogo.portaAberta;
    }

    bool noCaramelo = (jogo.faseAtual.mapa[jogo.guloso.linha][jogo.guloso.coluna] == CAMPO_ELETRICO);
    if (noCaramelo) {
        jogo.guloso.tempoNoCaramelo += dt;
        float taxaDreno = 6.0f + (jogo.guloso.tempoNoCaramelo * 3.5f);
        if (taxaDreno > 20.0f) taxaDreno = 20.0f;
        jogo.guloso.energia -= taxaDreno * dt;
    } else {
        jogo.guloso.tempoNoCaramelo = 0.0f;
        jogo.guloso.energia -= 1.8f * dt;
    }

    if (jogo.guloso.energia <= 0.0f) {
        jogo.guloso.vidas--;
        jogo.guloso.energia = (float)ENERGIA_MAXIMA;
        jogo.guloso.tempoInvulneravel = 1.6f;
        if (jogo.guloso.vidas <= 0) {
            registrarResultadoJogador(jogo.nomeJogador, false, jogo.guloso.pontos, jogo.numeroFase - 1);
            jogo.estado = ESTADO_GAME_OVER;
            return;
        }
    }

    if (jogo.guloso.tempoInvulneravel > 0.0f) {
        jogo.guloso.tempoInvulneravel -= dt;
    }

    if (jogo.guloso.delayMovimento > 0.0f) {
        jogo.guloso.delayMovimento -= dt;
    }

    int dir = lerDirecaoTeclado();
    if (dir != -1 && jogo.guloso.delayMovimento <= 0.0f) {
        int lAnt = jogo.guloso.linha;
        int cAnt = jogo.guloso.coluna;

        if (moverGuloso(jogo.guloso, jogo.faseAtual, dir)) {
            if (jogo.faseAtual.mapa[jogo.guloso.linha][jogo.guloso.coluna] == OBS_INTER && !jogo.portaAberta) {
                jogo.guloso.linha = lAnt;
                jogo.guloso.coluna = cAnt;
            } else {
                aplicarObstaculos(jogo.guloso, jogo.faseAtual, dir, jogo.portaAberta);
                jogo.guloso.delayMovimento = noCaramelo ? 0.22f : 0.09f;
            }
        }
    }

    for (auto &d : jogo.doces) {
        if (!d.ativo) continue;
        if (d.linha == jogo.guloso.linha && d.coluna == jogo.guloso.coluna) {
            d.ativo = false;
            jogo.guloso.docesColetados++;

            if (d.tipo == DOCE_BALA) {
                jogo.guloso.pontos += 2;
                jogo.guloso.energia = min((float)ENERGIA_MAXIMA, jogo.guloso.energia + 5.0f);
            } else if (d.tipo == DOCE_CHOCOLATE) {
                jogo.guloso.pontos += 5;
                jogo.guloso.energia = min((float)ENERGIA_MAXIMA, jogo.guloso.energia + 10.0f);
            } else if (d.tipo == DOCE_ROSQUINHA) {
                jogo.guloso.pontos += 10;
                jogo.guloso.energia = min((float)ENERGIA_MAXIMA, jogo.guloso.energia + 15.0f);
            } else if (d.tipo == DOCE_JUJUBA) {
                jogo.guloso.pontos += 25;
                jogo.guloso.energia = min((float)ENERGIA_MAXIMA, jogo.guloso.energia + 25.0f);
            } else if (d.tipo == DOCE_PIRULITO) {
                jogo.guloso.pontos += 50;
                jogo.guloso.energia = min((float)ENERGIA_MAXIMA, jogo.guloso.energia + 50.0f);
            } else if (d.tipo == DOCE_DOURADO) {
                jogo.guloso.pontos += 100;
                jogo.guloso.energia = (float)ENERGIA_MAXIMA;
                jogo.guloso.temDoceDourado = true;
            }
        }
    }

    jogo.timerSpawnDoces += dt;
    if (jogo.timerSpawnDoces >= 2.2f) {
        jogo.timerSpawnDoces = 0.0f;
        tentarSpawnDoce(jogo);
    }

    atualizarInimigos(jogo.inimigos, jogo.faseAtual, jogo.guloso, dt);
    atualizarSentinelasEProjeteis(jogo.faseAtual, jogo.guloso, jogo.projeteis, dt, jogo.timerSentinela);

    if (verificarColisoesInimigos(jogo.guloso, jogo.inimigos, jogo.projeteis)) {
        if (jogo.guloso.vidas <= 0) {
            registrarResultadoJogador(jogo.nomeJogador, false, jogo.guloso.pontos, jogo.numeroFase - 1);
            jogo.estado = ESTADO_GAME_OVER;
            return;
        }
    }

    if (jogo.faseAtual.mapa[jogo.guloso.linha][jogo.guloso.coluna] == SAIDA) {
        if (jogo.guloso.temDoceDourado) {
            if (jogo.numeroFase < NUM_FASES) {
                iniciarFase(jogo, jogo.numeroFase + 1);
            } else {
                registrarResultadoJogador(jogo.nomeJogador, true, jogo.guloso.pontos, NUM_FASES);
                jogo.estado = ESTADO_VITORIA;
            }
        }
    }
}

void desenharJogo(const Jogo &jogo) {
    if (jogo.estado == ESTADO_MENU) {
        ClearBackground((Color){30, 20, 35, 255});
        int titW = MeasureText("WHILE (GULOSO)", 50);
        DrawText("WHILE (GULOSO)", (LARGURA_JANELA - titW) / 2, 75, 50, (Color){255, 182, 193, 255});
        int subW = MeasureText("Evite o fim do mundo e coma todos os doces malvados!", 21);
        DrawText("Evite o fim do mundo e coma todos os doces malvados!", (LARGURA_JANELA - subW) / 2, 140, 21, (Color){255, 205, 220, 255});

        int cx = (LARGURA_JANELA - 400) / 2;
        int bx = (LARGURA_JANELA - 200) / 2;

        DrawRectangle(cx, 210, 400, 45, (Color){50, 35, 55, 255});
        Color corBordaCaixa = jogo.digitandoNome ? (Color){255, 182, 193, 255} : (Color){200, 150, 180, 255};
        DrawRectangleLines(cx, 210, 400, 45, corBordaCaixa);
        DrawText("Nome do Jogador: (clique para digitar)", cx, 185, 16, (Color){245, 220, 235, 255});

        string textoExibido = jogo.nomeJogador;
        if (jogo.digitandoNome && ((int)(GetTime() * 2) % 2 == 0)) {
            textoExibido += "|";
        }
        DrawText(textoExibido.c_str(), cx + 15, 222, 22, (Color){255, 240, 180, 255});

        DrawRectangle(bx, 285, 200, 38, (Color){115, 185, 135, 255});
        DrawRectangleLines(bx, 285, 200, 38, (Color){220, 245, 230, 255});
        int tIni = MeasureText("INICIAR", 19);
        DrawText("INICIAR", bx + (200 - tIni) / 2, 294, 19, WHITE);

        DrawRectangle(bx, 335, 200, 38, (Color){160, 140, 200, 255});
        DrawRectangleLines(bx, 335, 200, 38, (Color){235, 225, 255, 255});
        int tCfg = MeasureText("CONFIGURACOES", 17);
        DrawText("CONFIGURACOES", bx + (200 - tCfg) / 2, 344, 17, WHITE);

        DrawRectangle(bx, 385, 200, 38, (Color){110, 150, 200, 255});
        DrawRectangleLines(bx, 385, 200, 38, (Color){220, 235, 250, 255});
        int tReg = MeasureText("REGRAS", 19);
        DrawText("REGRAS", bx + (200 - tReg) / 2, 394, 19, WHITE);

        DrawRectangle(bx, 435, 200, 38, (Color){215, 160, 100, 255});
        DrawRectangleLines(bx, 435, 200, 38, (Color){255, 240, 220, 255});
        int tRnk = MeasureText("RANKING", 19);
        DrawText("RANKING", bx + (200 - tRnk) / 2, 444, 19, WHITE);

        DrawRectangle(bx, 485, 200, 38, (Color){215, 110, 110, 255});
        DrawRectangleLines(bx, 485, 200, 38, (Color){255, 225, 225, 255});
        int tSai = MeasureText("SAIR", 19);
        DrawText("SAIR", bx + (200 - tSai) / 2, 494, 19, WHITE);

        if (!jogo.mensagemErro.empty()) {
            int errW = MeasureText(jogo.mensagemErro.c_str(), 16);
            DrawText(jogo.mensagemErro.c_str(), (LARGURA_JANELA - errW) / 2, 540, 16, RED);
        }
        return;
    }

    if (jogo.estado == ESTADO_CONFIGURACOES) {
        ClearBackground((Color){25, 20, 35, 255});
        int titW = MeasureText("CONFIGURACOES", 32);
        DrawText("CONFIGURACOES", (LARGURA_JANELA - titW) / 2, 70, 32, (Color){255, 182, 193, 255});

        int cx = (LARGURA_JANELA - 600) / 2;
        int cy = 140;
        int sx = cx + 80;
        int bx = (LARGURA_JANELA - 200) / 2;

        DrawRectangle(cx, cy, 600, 360, (Color){45, 32, 50, 255});
        DrawRectangleLines(cx, cy, 600, 360, (Color){200, 160, 210, 255});

        DrawText("Volume da Musica:", sx, cy + 40, 20, WHITE);
        DrawRectangle(sx, cy + 75, 340, 22, (Color){70, 55, 80, 255});
        DrawRectangle(sx, cy + 75, (int)(340.0f * jogo.volumeMusica), 22, (Color){255, 182, 193, 255});
        DrawRectangleLines(sx, cy + 75, 340, 22, WHITE);
        DrawCircle(sx + (int)(340.0f * jogo.volumeMusica), cy + 86, 12, (Color){255, 220, 235, 255});
        DrawText(TextFormat("%d%%", (int)(jogo.volumeMusica * 100.0f + 0.5f)), sx + 355, cy + 76, 19, (Color){255, 240, 180, 255});

        DrawText("Volume dos Efeitos:", sx, cy + 140, 20, WHITE);
        DrawRectangle(sx, cy + 175, 340, 22, (Color){70, 55, 80, 255});
        DrawRectangle(sx, cy + 175, (int)(340.0f * jogo.volumeEfeitos), 22, (Color){160, 200, 255, 255});
        DrawRectangleLines(sx, cy + 175, 340, 22, WHITE);
        DrawCircle(sx + (int)(340.0f * jogo.volumeEfeitos), cy + 186, 12, (Color){220, 235, 255, 255});
        DrawText(TextFormat("%d%%", (int)(jogo.volumeEfeitos * 100.0f + 0.5f)), sx + 355, cy + 176, 19, (Color){255, 240, 180, 255});

        DrawRectangle(bx, cy + 290, 200, 42, (Color){110, 150, 200, 255});
        DrawRectangleLines(bx, cy + 290, 200, 42, (Color){220, 235, 250, 255});
        int tVoltar = MeasureText("VOLTAR", 19);
        DrawText("VOLTAR", bx + (200 - tVoltar) / 2, cy + 301, 19, WHITE);
        return;
    }

    if (jogo.estado == ESTADO_PERGUNTA_PRIMEIRA_VEZ) {
        ClearBackground((Color){35, 25, 40, 255});
        int cx = (LARGURA_JANELA - 600) / 2;
        int cy = 160;
        int bSimX = (LARGURA_JANELA - 340) / 2;
        int bNaoX = bSimX + 200;
        int by = cy + 160;

        DrawRectangle(cx, cy, 600, 280, (Color){50, 35, 55, 255});
        DrawRectangleLines(cx, cy, 600, 280, (Color){255, 182, 193, 255});

        int tw = MeasureText("E a sua primeira vez jogando?", 28);
        DrawText("E a sua primeira vez jogando?", (LARGURA_JANELA - tw) / 2, cy + 60, 28, WHITE);

        DrawRectangle(bSimX, by, 140, 50, (Color){115, 185, 135, 255});
        DrawRectangleLines(bSimX, by, 140, 50, (Color){220, 245, 230, 255});
        int tSim = MeasureText("SIM!", 24);
        DrawText("SIM!", bSimX + (140 - tSim) / 2, by + 12, 24, WHITE);

        DrawRectangle(bNaoX, by, 140, 50, (Color){215, 110, 110, 255});
        DrawRectangleLines(bNaoX, by, 140, 50, (Color){255, 225, 225, 255});
        int tNao = MeasureText("NAO!", 24);
        DrawText("NAO!", bNaoX + (140 - tNao) / 2, by + 12, 24, WHITE);
        return;
    }

    if (jogo.estado == ESTADO_CUTSCENE) {
        ClearBackground((Color){25, 18, 30, 255});

        int cx = (LARGURA_JANELA - 900) / 2;
        int cy = 70;
        int bx = cx + 900 - 90 - 40;
        int by = cy + 430 - 50 - 20;
        int centerX = LARGURA_JANELA / 2;

        DrawRectangle(cx, cy, 900, 430, (Color){45, 32, 50, 255});
        DrawRectangleLines(cx, cy, 900, 430, (Color){200, 160, 210, 255});

        const char *tit = TextFormat("HISTORIA - PARTE %d DE %d", jogo.indiceCutscene + 1, jogo.totalCutscenes);
        int titW = MeasureText(tit, 24);
        DrawText(tit, (LARGURA_JANELA - titW) / 2, cy + 30, 24, (Color){255, 182, 193, 255});

        if (jogo.indiceCutscene == 0) {
            DrawText("Oiee essa e a demo!", cx + 60, cy + 110, 22, WHITE);
            DrawText("Esse e o Guloso,", cx + 60, cy + 150, 22, WHITE);
            DrawText("ele nao e fofinho?", cx + 60, cy + 190, 22, WHITE);
            DrawCircle(centerX, cy + 310, 40, (Color){255, 215, 0, 255});
            DrawCircle(centerX - 15, cy + 300, 5, BLACK);
            DrawCircle(centerX + 15, cy + 300, 5, BLACK);
            DrawCircleSector((Vector2){(float)centerX, (float)cy + 315}, 20.0f, 0, 180, 10, RED);
        } else if (jogo.indiceCutscene == 1) {
            DrawText("Voce tem que catar todos os docinhos", cx + 60, cy + 110, 22, WHITE);
            DrawText("para sobreviver e manter suas energias. O Guloso tem que", cx + 60, cy + 150, 22, WHITE);
            DrawText("se alimentar constantemente dos docinhos pelo caminho.", cx + 60, cy + 190, 22, WHITE);
            DrawText("Cuidado: se a fome zerar, ele perde uma de suas 3 vidas!", cx + 60, cy + 230, 22, (Color){255, 140, 140, 255});
        } else {
            DrawText("A chave para escapar de cada setor e o raro DOCE DOURADO.", cx + 60, cy + 110, 22, WHITE);
            DrawText("Colete a cota necessaria de doces para faze-lo surgir,", cx + 60, cy + 150, 22, WHITE);
            DrawText("pegue o Doce Dourado e corra para o portal da Saida (S)!", cx + 60, cy + 190, 22, WHITE);
            DrawCircle(centerX, cy + 300, 30, (Color){255, 215, 0, 255});
            DrawCircleLines(centerX, cy + 300, 30, WHITE);
        }

        DrawRectangle(bx, by, 90, 50, (Color){115, 185, 135, 255});
        DrawRectangleLines(bx, by, 90, 50, (Color){220, 245, 230, 255});
        int tAv = MeasureText("->", 26);
        DrawText("->", bx + (90 - tAv) / 2, by + 12, 26, WHITE);
        return;
    }

    if (jogo.estado == ESTADO_TUTORIAL) {
        ClearBackground((Color){25, 20, 35, 255});
        int cx = (LARGURA_JANELA - 800) / 2;
        int cy = 60;
        int bx = (LARGURA_JANELA - 200) / 2;
        int by = cy + 410;

        DrawRectangle(cx, cy, 800, 480, (Color){45, 35, 55, 255});
        DrawRectangleLines(cx, cy, 800, 480, (Color){255, 182, 193, 255});

        int titW = MeasureText("TUTORIAL & CONTROLES", 26);
        DrawText("TUTORIAL & CONTROLES", (LARGURA_JANELA - titW) / 2, cy + 30, 26, (Color){255, 182, 193, 255});
        DrawText("• Teclas W, A, S, D ou SETAS: Movimentam o Guloso", cx + 50, cy + 100, 19, WHITE);
        DrawText("• Fome & Energia: A barra diminui com o tempo!", cx + 50, cy + 140, 19, (Color){255, 200, 120, 255});
        DrawText("• Doces Normais: Balas, Chocolates, Rosquinhas, Cupcakes e Bolos", cx + 50, cy + 180, 18, WHITE);
        DrawText("• Piso de Caramelo: Deixa lento e drena a fome aos poucos", cx + 50, cy + 220, 18, (Color){255, 160, 90, 255});
        DrawText("• Pirulito: Recupera 100% da fome (uso unico)", cx + 50, cy + 260, 18, (Color){255, 140, 180, 255});
        DrawText("• Inimigos & Sentinela: Tiram 1 VIDA direta se atingirem voce", cx + 50, cy + 300, 18, (Color){255, 120, 120, 255});
        DrawText("• Meta: Pegue o Doce Dourado e va para a Saida (S)", cx + 50, cy + 340, 18, (Color){140, 235, 160, 255});

        DrawRectangle(bx, by, 200, 45, (Color){115, 185, 135, 255});
        DrawRectangleLines(bx, by, 200, 45, (Color){220, 245, 230, 255});
        int tJogar = MeasureText("JOGAR AGORA", 20);
        DrawText("JOGAR AGORA", bx + (200 - tJogar) / 2, by + 12, 20, WHITE);
        return;
    }

    if (jogo.estado == ESTADO_REGRAS) {
        ClearBackground((Color){25, 20, 35, 255});
        int titW = MeasureText("REGRAS DO JOGO", 28);
        DrawText("REGRAS DO JOGO", (LARGURA_JANELA - titW) / 2, 40, 28, (Color){255, 182, 193, 255});

        int cx = (LARGURA_JANELA - 900) / 2;
        int cy = 85;
        int bx = (LARGURA_JANELA - 200) / 2;
        int by = 530;

        DrawRectangle(cx, cy, 900, 420, (Color){40, 30, 45, 255});
        DrawRectangleLines(cx, cy, 900, 420, (Color){160, 120, 150, 255});

        DrawRectangle(bx, by, 200, 40, (Color){110, 150, 200, 255});
        DrawRectangleLines(bx, by, 200, 40, (Color){220, 235, 250, 255});
        int tVoltar = MeasureText("VOLTAR", 18);
        DrawText("VOLTAR", bx + (200 - tVoltar) / 2, by + 10, 18, WHITE);
        return;
    }

    if (jogo.estado == ESTADO_RANKING) {
        ClearBackground((Color){25, 20, 35, 255});
        int titW = MeasureText("RANKING DE JOGADORES", 28);
        DrawText("RANKING DE JOGADORES", (LARGURA_JANELA - titW) / 2, 40, 28, (Color){255, 182, 193, 255});

        vector<Estatistica> ranking = obterRankingEstatisticas();
        int cx = (LARGURA_JANELA - 900) / 2;
        int cy = 85;
        int bx = (LARGURA_JANELA - 200) / 2;
        int by = 530;

        DrawRectangle(cx, cy, 900, 420, (Color){40, 30, 45, 255});
        DrawRectangleLines(cx, cy, 900, 420, (Color){160, 120, 150, 255});

        DrawText("POS", cx + 30, cy + 20, 16, (Color){255, 182, 193, 255});
        DrawText("NOME", cx + 100, cy + 20, 16, (Color){255, 182, 193, 255});
        DrawText("PARTIDAS", cx + 320, cy + 20, 16, (Color){255, 182, 193, 255});
        DrawText("VITORIAS", cx + 440, cy + 20, 16, (Color){140, 235, 160, 255});
        DrawText("DERROTAS", cx + 560, cy + 20, 16, (Color){255, 130, 130, 255});
        DrawText("MAIOR PTS", cx + 680, cy + 20, 16, (Color){255, 240, 180, 255});
        DrawText("FASES", cx + 800, cy + 20, 16, (Color){255, 182, 193, 255});

        DrawLine(cx + 20, cy + 45, cx + 870, cy + 45, (Color){120, 90, 110, 255});

        int maxItens = min((int)ranking.size(), 10);
        for (int i = 0; i < maxItens; i++) {
            int y = cy + 60 + i * 32;
            DrawText(TextFormat("%d.", i + 1), cx + 30, y, 16, WHITE);
            DrawText(ranking[i].nome, cx + 100, y, 16, WHITE);
            DrawText(TextFormat("%d", ranking[i].partidasJogadas), cx + 350, y, 16, WHITE);
            DrawText(TextFormat("%d", ranking[i].vitorias), cx + 470, y, 16, (Color){140, 235, 160, 255});
            DrawText(TextFormat("%d", ranking[i].derrotas), cx + 590, y, 16, (Color){255, 130, 130, 255});
            DrawText(TextFormat("%d", ranking[i].maiorPontuacao), cx + 700, y, 16, (Color){255, 240, 180, 255});
            DrawText(TextFormat("%d", ranking[i].fasesConcluidas), cx + 820, y, 16, WHITE);
        }

        DrawRectangle(bx, by, 200, 40, (Color){110, 150, 200, 255});
        DrawRectangleLines(bx, by, 200, 40, (Color){220, 235, 250, 255});
        int tVoltar = MeasureText("VOLTAR", 18);
        DrawText("VOLTAR", bx + (200 - tVoltar) / 2, by + 10, 18, WHITE);
        return;
    }

    desenharCenario(jogo.faseAtual, jogo.portaAberta, jogo.boloSprite, jogo.sorveteInimigoSprite);
    desenharDoces(jogo.doces, jogo.balaSprite, jogo.chocolateSprite, jogo.donutSprite, jogo.jujubaSprite, jogo.pirulitoSprite, jogo.DoceDouradoSprite);
    desenharInimigos(jogo.inimigos, jogo.cupcakeInimigoSprite, jogo.rocamboleInimigoSprite);
    desenharProjeteis(jogo.projeteis, jogo.geloSprite);
    desenharGuloso(jogo.guloso, jogo.gulosoSprite);
    desenharPainel(jogo.guloso, jogo.numeroFase, jogo.tempoFase, jogo.faseAtual.metaDoces);

    if (jogo.estado == ESTADO_PAUSA) {
        DrawRectangle(0, 0, LARGURA_JANELA, ALTURA_JANELA, (Color){15, 10, 20, 180});

        int cx = (LARGURA_JANELA - 340) / 2;
        int cy = (ALTURA_JANELA - 350) / 2;
        int bx = (LARGURA_JANELA - 220) / 2;

        DrawRectangle(cx, cy, 340, 350, (Color){45, 30, 45, 245});
        DrawRectangleLines(cx, cy, 340, 350, (Color){255, 182, 193, 255});
        int tw = MeasureText("JOGO PAUSADO", 26);
        DrawText("JOGO PAUSADO", (LARGURA_JANELA - tw) / 2, cy + 30, 26, (Color){255, 182, 193, 255});

        DrawRectangle(bx, cy + 90, 220, 45, (Color){115, 185, 135, 255});
        DrawRectangleLines(bx, cy + 90, 220, 45, (Color){220, 245, 230, 255});
        int t1 = MeasureText("CONTINUAR", 18);
        DrawText("CONTINUAR", bx + (220 - t1) / 2, cy + 103, 18, WHITE);

        DrawRectangle(bx, cy + 150, 220, 45, (Color){110, 150, 200, 255});
        DrawRectangleLines(bx, cy + 150, 220, 45, (Color){220, 235, 250, 255});
        int t2 = MeasureText("REINICIAR FASE", 18);
        DrawText("REINICIAR FASE", bx + (220 - t2) / 2, cy + 163, 18, WHITE);

        DrawRectangle(bx, cy + 210, 220, 45, (Color){215, 160, 100, 255});
        DrawRectangleLines(bx, cy + 210, 220, 45, (Color){255, 240, 220, 255});
        int t3 = MeasureText("VOLTAR AO MENU", 18);
        DrawText("VOLTAR AO MENU", bx + (220 - t3) / 2, cy + 223, 18, WHITE);

        DrawRectangle(bx, cy + 270, 220, 45, (Color){215, 110, 110, 255});
        DrawRectangleLines(bx, cy + 270, 220, 45, (Color){255, 225, 225, 255});
        int t4 = MeasureText("SAIR DO JOGO", 18);
        DrawText("SAIR DO JOGO", bx + (220 - t4) / 2, cy + 283, 18, WHITE);
    }

    if (jogo.estado == ESTADO_VITORIA) {
        desenharMensagem("PARABENS", "Voce comeu todos os doces e evitou o caos!", (Color){60, 240, 90, 255});
        int boardW = COLUNAS * TAM_CELULA;
        int b1X = (boardW - 310) / 2;
        int b2X = b1X + 170;
        int by = (ALTURA_JANELA - 180) / 2 + 115;

        DrawRectangle(b1X, by, 140, 45, (Color){50, 160, 70, 255});
        DrawRectangleLines(b1X, by, 140, 45, WHITE);
        int tRei = MeasureText("REINICIAR", 17);
        DrawText("REINICIAR", b1X + (140 - tRei) / 2, by + 13, 17, WHITE);

        DrawRectangle(b2X, by, 140, 45, (Color){60, 100, 180, 255});
        DrawRectangleLines(b2X, by, 140, 45, WHITE);
        int tRnk = MeasureText("RANKING", 17);
        DrawText("RANKING", b2X + (140 - tRnk) / 2, by + 13, 17, WHITE);
    } else if (jogo.estado == ESTADO_GAME_OVER) {
        desenharMensagem("GAME OVER!", "Suas vidas acabaram!", (Color){255, 60, 60, 255});
        int boardW = COLUNAS * TAM_CELULA;
        int b1X = (boardW - 310) / 2;
        int b2X = b1X + 170;
        int by = (ALTURA_JANELA - 180) / 2 + 115;

        DrawRectangle(b1X, by, 140, 45, (Color){50, 160, 70, 255});
        DrawRectangleLines(b1X, by, 140, 45, WHITE);
        int tRei = MeasureText("REINICIAR", 17);
        DrawText("REINICIAR", b1X + (140 - tRei) / 2, by + 13, 17, WHITE);

        DrawRectangle(b2X, by, 140, 45, (Color){180, 50, 50, 255});
        DrawRectangleLines(b2X, by, 140, 45, WHITE);
        int tRnk = MeasureText("RANKING", 17);
        DrawText("RANKING", b2X + (140 - tRnk) / 2, by + 13, 17, WHITE);
    }
}