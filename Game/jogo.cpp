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
        Rectangle caixaTexto = {350, 210, 400, 45};

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
            if (CheckCollisionPointRec(mouse, (Rectangle){450, 285, 200, 38})) {
                tocarClick(jogo);
                if (jogo.nomeJogador.empty()) jogo.nomeJogador = "Jogador";
                jogo.estado = ESTADO_PERGUNTA_PRIMEIRA_VEZ;
            } else if (CheckCollisionPointRec(mouse, (Rectangle){450, 335, 200, 38})) {
                tocarClick(jogo);
                jogo.estado = ESTADO_CONFIGURACOES;
            } else if (CheckCollisionPointRec(mouse, (Rectangle){450, 385, 200, 38})) {
                tocarClick(jogo);
                jogo.estado = ESTADO_REGRAS;
            } else if (CheckCollisionPointRec(mouse, (Rectangle){450, 435, 200, 38})) {
                tocarClick(jogo);
                jogo.estado = ESTADO_RANKING;
            } else if (CheckCollisionPointRec(mouse, (Rectangle){450, 485, 200, 38})) {
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

        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){370, 205, 360, 42})) {
                jogo.arrastandoMusica = true;
                float v = (mouse.x - 380.0f) / 340.0f;
                if (v < 0.0f) v = 0.0f;
                if (v > 1.0f) v = 1.0f;
                jogo.volumeMusica = v;
                if (jogo.musicaCarregada) SetMusicVolume(jogo.musicaMenu, jogo.volumeMusica);
            } else if (CheckCollisionPointRec(mouse, (Rectangle){370, 305, 360, 42})) {
                jogo.arrastandoEfeitos = true;
                float v = (mouse.x - 380.0f) / 340.0f;
                if (v < 0.0f) v = 0.0f;
                if (v > 1.0f) v = 1.0f;
                jogo.volumeEfeitos = v;
                if (jogo.somCarregado) SetSoundVolume(jogo.somClick, jogo.volumeEfeitos);
            } else if (CheckCollisionPointRec(mouse, (Rectangle){450, 430, 200, 42})) {
                tocarClick(jogo);
                jogo.arrastandoMusica = false;
                jogo.arrastandoEfeitos = false;
                jogo.estado = ESTADO_MENU;
                return;
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (jogo.arrastandoMusica) {
                float v = (mouse.x - 380.0f) / 340.0f;
                if (v < 0.0f) v = 0.0f;
                if (v > 1.0f) v = 1.0f;
                jogo.volumeMusica = v;
                if (jogo.musicaCarregada) SetMusicVolume(jogo.musicaMenu, jogo.volumeMusica);
            } else if (jogo.arrastandoEfeitos) {
                float v = (mouse.x - 380.0f) / 340.0f;
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
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){380, 320, 140, 50})) {
                tocarClick(jogo);
                jogo.indiceCutscene = 0;
                jogo.estado = ESTADO_CUTSCENE;
            } else if (CheckCollisionPointRec(mouse, (Rectangle){580, 320, 140, 50})) {
                tocarClick(jogo);
                iniciarFase(jogo, 1);
            }
        }
        return;
    }

    if (jogo.estado == ESTADO_CUTSCENE) {
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){960, 520, 90, 50})) {
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
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){450, 500, 200, 50})) {
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
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){450, 530, 200, 40})) {
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

        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){250, 310, 140, 45})) {
                tocarClick(jogo);
                iniciarFase(jogo, jogo.numeroFase);
            } else if (CheckCollisionPointRec(mouse, (Rectangle){420, 310, 140, 45})) {
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

        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mouse, (Rectangle){440, 240, 220, 45})) {
                tocarClick(jogo);
                jogo.estado = ESTADO_JOGANDO;
            } else if (CheckCollisionPointRec(mouse, (Rectangle){440, 300, 220, 45})) {
                tocarClick(jogo);
                iniciarFase(jogo, jogo.numeroFase);
            } else if (CheckCollisionPointRec(mouse, (Rectangle){440, 360, 220, 45})) {
                tocarClick(jogo);
                jogo.estado = ESTADO_MENU;
            } else if (CheckCollisionPointRec(mouse, (Rectangle){440, 420, 220, 45})) {
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
        DrawText("WHILE (GULOSO)", 340, 75, 50, (Color){255, 182, 193, 255});
        DrawText("Evite o fim do mundo e coma todos os doces malvados!", 255, 140, 21, (Color){255, 205, 220, 255});

        DrawRectangle(350, 210, 400, 45, (Color){50, 35, 55, 255});
        Color corBordaCaixa = jogo.digitandoNome ? (Color){255, 182, 193, 255} : (Color){200, 150, 180, 255};
        DrawRectangleLines(350, 210, 400, 45, corBordaCaixa);
        DrawText("Nome do Jogador: (clique para digitar)", 350, 185, 16, (Color){245, 220, 235, 255});

        string textoExibido = jogo.nomeJogador;
        if (jogo.digitandoNome && ((int)(GetTime() * 2) % 2 == 0)) {
            textoExibido += "|";
        }
        DrawText(textoExibido.c_str(), 365, 222, 22, (Color){255, 240, 180, 255});

        DrawRectangle(450, 285, 200, 38, (Color){115, 185, 135, 255});
        DrawRectangleLines(450, 285, 200, 38, (Color){220, 245, 230, 255});
        DrawText("INICIAR", 512, 294, 19, WHITE);

        DrawRectangle(450, 335, 200, 38, (Color){160, 140, 200, 255});
        DrawRectangleLines(450, 335, 200, 38, (Color){235, 225, 255, 255});
        DrawText("CONFIGURACOES", 475, 344, 17, WHITE);

        DrawRectangle(450, 385, 200, 38, (Color){110, 150, 200, 255});
        DrawRectangleLines(450, 385, 200, 38, (Color){220, 235, 250, 255});
        DrawText("REGRAS", 514, 394, 19, WHITE);

        DrawRectangle(450, 435, 200, 38, (Color){215, 160, 100, 255});
        DrawRectangleLines(450, 435, 200, 38, (Color){255, 240, 220, 255});
        DrawText("RANKING", 508, 444, 19, WHITE);

        DrawRectangle(450, 485, 200, 38, (Color){215, 110, 110, 255});
        DrawRectangleLines(450, 485, 200, 38, (Color){255, 225, 225, 255});
        DrawText("SAIR", 525, 494, 19, WHITE);

        if (!jogo.mensagemErro.empty()) {
            DrawText(jogo.mensagemErro.c_str(), 200, 540, 16, RED);
        }
        return;
    }

    if (jogo.estado == ESTADO_CONFIGURACOES) {
        ClearBackground((Color){25, 20, 35, 255});
        DrawText("CONFIGURACOES", 415, 70, 32, (Color){255, 182, 193, 255});

        DrawRectangle(250, 140, 600, 360, (Color){45, 32, 50, 255});
        DrawRectangleLines(250, 140, 600, 360, (Color){200, 160, 210, 255});

        DrawText("Volume da Musica:", 380, 180, 20, WHITE);
        DrawRectangle(380, 215, 340, 22, (Color){70, 55, 80, 255});
        DrawRectangle(380, 215, (int)(340.0f * jogo.volumeMusica), 22, (Color){255, 182, 193, 255});
        DrawRectangleLines(380, 215, 340, 22, WHITE);
        DrawCircle(380 + (int)(340.0f * jogo.volumeMusica), 226, 12, (Color){255, 220, 235, 255});
        DrawText(TextFormat("%d%%", (int)(jogo.volumeMusica * 100.0f + 0.5f)), 735, 216, 19, (Color){255, 240, 180, 255});

        DrawText("Volume dos Efeitos:", 380, 280, 20, WHITE);
        DrawRectangle(380, 315, 340, 22, (Color){70, 55, 80, 255});
        DrawRectangle(380, 315, (int)(340.0f * jogo.volumeEfeitos), 22, (Color){160, 200, 255, 255});
        DrawRectangleLines(380, 315, 340, 22, WHITE);
        DrawCircle(380 + (int)(340.0f * jogo.volumeEfeitos), 326, 12, (Color){220, 235, 255, 255});
        DrawText(TextFormat("%d%%", (int)(jogo.volumeEfeitos * 100.0f + 0.5f)), 735, 316, 19, (Color){255, 240, 180, 255});

        DrawRectangle(450, 430, 200, 42, (Color){110, 150, 200, 255});
        DrawRectangleLines(450, 430, 200, 42, (Color){220, 235, 250, 255});
        DrawText("VOLTAR", 512, 441, 19, WHITE);
        return;
    }

    if (jogo.estado == ESTADO_PERGUNTA_PRIMEIRA_VEZ) {
        ClearBackground((Color){35, 25, 40, 255});
        DrawRectangle(250, 160, 600, 280, (Color){50, 35, 55, 255});
        DrawRectangleLines(250, 160, 600, 280, (Color){255, 182, 193, 255});

        DrawText("E a sua primeira vez jogando?", 330, 220, 28, WHITE);

        DrawRectangle(380, 320, 140, 50, (Color){115, 185, 135, 255});
        DrawRectangleLines(380, 320, 140, 50, (Color){220, 245, 230, 255});
        DrawText("SIM!", 425, 332, 24, WHITE);

        DrawRectangle(580, 320, 140, 50, (Color){215, 110, 110, 255});
        DrawRectangleLines(580, 320, 140, 50, (Color){255, 225, 225, 255});
        DrawText("NAO!", 620, 332, 24, WHITE);
        return;
    }

    if (jogo.estado == ESTADO_CUTSCENE) {
        ClearBackground((Color){25, 18, 30, 255});

        DrawRectangle(100, 70, 900, 430, (Color){45, 32, 50, 255});
        DrawRectangleLines(100, 70, 900, 430, (Color){200, 160, 210, 255});

        DrawText(TextFormat("HISTORIA - PARTE %d DE %d", jogo.indiceCutscene + 1, jogo.totalCutscenes), 380, 100, 24, (Color){255, 182, 193, 255});

        if (jogo.indiceCutscene == 0) {
            DrawText("Oiee essa e a demo!", 160, 180, 22, WHITE);
            DrawText("Esse e o Guloso,", 160, 220, 22, WHITE);
            DrawText("ele nao e fofinho?", 160, 260, 22, WHITE);
            DrawCircle(550, 380, 40, (Color){255, 215, 0, 255});
            DrawCircle(535, 370, 5, BLACK);
            DrawCircle(565, 370, 5, BLACK);
            DrawCircleSector((Vector2){550.0f, 385.0f}, 20.0f, 0, 180, 10, RED);
        } else if (jogo.indiceCutscene == 1) {
            DrawText("Voce tem que catar todos os docinhos", 160, 180, 22, WHITE);
            DrawText("para sobreviver e manter suas energias. O Guloso tem que", 160, 220, 22, WHITE);
            DrawText("se alimentar constantemente dos docinhos pelo caminho.", 160, 260, 22, WHITE);
            DrawText("Cuidado: se a fome zerar, ele perde uma de suas 3 vidas!", 160, 300, 22, (Color){255, 140, 140, 255});
        } else {
            DrawText("A chave para escapar de cada setor e o raro DOCE DOURADO.", 160, 180, 22, WHITE);
            DrawText("Colete a cota necessaria de doces para faze-lo surgir,", 160, 220, 22, WHITE);
            DrawText("pegue o Doce Dourado e corra para o portal da Saida (S)!", 160, 260, 22, WHITE);
            DrawCircle(550, 370, 30, (Color){255, 215, 0, 255});
            DrawCircleLines(550, 370, 30, WHITE);
        }

        DrawRectangle(960, 520, 90, 50, (Color){115, 185, 135, 255});
        DrawRectangleLines(960, 520, 90, 50, (Color){220, 245, 230, 255});
        DrawText("->", 995, 532, 26, WHITE);
        return;
    }

    if (jogo.estado == ESTADO_TUTORIAL) {
        ClearBackground((Color){25, 20, 35, 255});
        DrawRectangle(150, 60, 800, 480, (Color){45, 35, 55, 255});
        DrawRectangleLines(150, 60, 800, 480, (Color){255, 182, 193, 255});

        DrawText("TUTORIAL & CONTROLES", 400, 90, 26, (Color){255, 182, 193, 255});
        DrawText("• Teclas W, A, S, D ou SETAS: Movimentam o Guloso", 200, 160, 19, WHITE);
        DrawText("• Fome & Energia: A barra diminui com o tempo!", 200, 200, 19, (Color){255, 200, 120, 255});
        DrawText("• Doces Normais: Balas, Chocolates, Rosquinhas, Cupcakes e Bolos", 200, 240, 18, WHITE);
        DrawText("• Piso de Caramelo: Deixa lento e drena a fome aos poucos", 200, 280, 18, (Color){255, 160, 90, 255});
        DrawText("• Pirulito: Recupera 100% da fome (uso unico)", 200, 320, 18, (Color){255, 140, 180, 255});
        DrawText("• Inimigos & Sentinela: Tiram 1 VIDA direta se atingirem voce", 200, 360, 18, (Color){255, 120, 120, 255});
        DrawText("• Meta: Pegue o Doce Dourado e va para a Saida (S)", 200, 400, 18, (Color){140, 235, 160, 255});

        DrawRectangle(450, 470, 200, 45, (Color){115, 185, 135, 255});
        DrawRectangleLines(450, 470, 200, 45, (Color){220, 245, 230, 255});
        DrawText("JOGAR AGORA", 475, 482, 20, WHITE);
        return;
    }

    if (jogo.estado == ESTADO_REGRAS) {
        ClearBackground((Color){25, 20, 35, 255});
        DrawText("REGRAS DO JOGO", 430, 40, 28, (Color){255, 182, 193, 255});

        DrawRectangle(100, 85, 900, 420, (Color){40, 30, 45, 255});
        DrawRectangleLines(100, 85, 900, 420, (Color){160, 120, 150, 255});

        DrawRectangle(450, 530, 200, 40, (Color){110, 150, 200, 255});
        DrawRectangleLines(450, 530, 200, 40, (Color){220, 235, 250, 255});
        DrawText("VOLTAR", 510, 540, 18, WHITE);
        return;
    }

    if (jogo.estado == ESTADO_RANKING) {
        ClearBackground((Color){25, 20, 35, 255});
        DrawText("RANKING DE JOGADORES", 380, 40, 28, (Color){255, 182, 193, 255});

        vector<Estatistica> ranking = obterRankingEstatisticas();
        DrawRectangle(100, 85, 900, 420, (Color){40, 30, 45, 255});
        DrawRectangleLines(100, 85, 900, 420, (Color){160, 120, 150, 255});

        DrawText("POS", 130, 105, 16, (Color){255, 182, 193, 255});
        DrawText("NOME", 200, 105, 16, (Color){255, 182, 193, 255});
        DrawText("PARTIDAS", 420, 105, 16, (Color){255, 182, 193, 255});
        DrawText("VITORIAS", 540, 105, 16, (Color){255, 182, 193, 255});
        DrawText("DERROTAS", 660, 105, 16, (Color){255, 182, 193, 255});
        DrawText("MAIOR PTS", 780, 105, 16, (Color){255, 182, 193, 255});
        DrawText("FASES", 900, 105, 16, (Color){255, 182, 193, 255});

        DrawLine(120, 130, 970, 130, (Color){120, 90, 110, 255});

        int maxItens = min((int)ranking.size(), 10);
        for (int i = 0; i < maxItens; i++) {
            int y = 145 + i * 32;
            DrawText(TextFormat("%d.", i + 1), 130, y, 16, WHITE);
            DrawText(ranking[i].nome, 200, y, 16, WHITE);
            DrawText(TextFormat("%d", ranking[i].partidasJogadas), 450, y, 16, WHITE);
            DrawText(TextFormat("%d", ranking[i].vitorias), 570, y, 16, (Color){140, 235, 160, 255});
            DrawText(TextFormat("%d", ranking[i].derrotas), 690, y, 16, (Color){255, 130, 130, 255});
            DrawText(TextFormat("%d", ranking[i].maiorPontuacao), 800, y, 16, (Color){255, 240, 180, 255});
            DrawText(TextFormat("%d", ranking[i].fasesConcluidas), 920, y, 16, WHITE);
        }

        DrawRectangle(450, 530, 200, 40, (Color){110, 150, 200, 255});
        DrawRectangleLines(450, 530, 200, 40, (Color){220, 235, 250, 255});
        DrawText("VOLTAR", 510, 540, 18, WHITE);
        return;
    }

    desenharCenario(jogo.faseAtual, jogo.portaAberta, jogo.boloSprite);
    desenharDoces(jogo.doces, jogo.balaSprite, jogo.chocolateSprite, jogo.donutSprite, jogo.jujubaSprite, jogo.pirulitoSprite, jogo.DoceDouradoSprite);
    desenharInimigos(jogo.inimigos, jogo.cupcakeInimigoSprite);
    desenharProjeteis(jogo.projeteis);
    desenharGuloso(jogo.guloso, jogo.gulosoSprite);
    desenharPainel(jogo.guloso, jogo.numeroFase, jogo.tempoFase, jogo.faseAtual.metaDoces);

    if (jogo.estado == ESTADO_PAUSA) {
        DrawRectangle(0, 0, LARGURA_JANELA, ALTURA_JANELA, (Color){15, 10, 20, 180});

        DrawRectangle(380, 150, 340, 350, (Color){45, 30, 45, 245});
        DrawRectangleLines(380, 150, 340, 350, (Color){255, 182, 193, 255});
        DrawText("JOGO PAUSADO", 450, 180, 26, (Color){255, 182, 193, 255});

        DrawRectangle(440, 240, 220, 45, (Color){115, 185, 135, 255});
        DrawRectangleLines(440, 240, 220, 45, (Color){220, 245, 230, 255});
        DrawText("CONTINUAR", 495, 253, 18, WHITE);

        DrawRectangle(440, 300, 220, 45, (Color){110, 150, 200, 255});
        DrawRectangleLines(440, 300, 220, 45, (Color){220, 235, 250, 255});
        DrawText("REINICIAR FASE", 480, 313, 18, WHITE);

        DrawRectangle(440, 360, 220, 45, (Color){215, 160, 100, 255});
        DrawRectangleLines(440, 360, 220, 45, (Color){255, 240, 220, 255});
        DrawText("VOLTAR AO MENU", 470, 373, 18, WHITE);

        DrawRectangle(440, 420, 220, 45, (Color){215, 110, 110, 255});
        DrawRectangleLines(440, 420, 220, 45, (Color){255, 225, 225, 255});
        DrawText("SAIR DO JOGO", 485, 433, 18, WHITE);
    }

    if (jogo.estado == ESTADO_VITORIA) {
        desenharMensagem("PARABENS", "Voce comeu todos os doces e evitou o caos!", (Color){60, 240, 90, 255});
        DrawRectangle(250, 310, 140, 45, (Color){50, 160, 70, 255});
        DrawRectangleLines(250, 310, 140, 45, WHITE);
        DrawText("REINICIAR", 270, 323, 17, WHITE);

        DrawRectangle(420, 310, 140, 45, (Color){60, 100, 180, 255});
        DrawRectangleLines(420, 310, 140, 45, WHITE);
        DrawText("RANKING", 445, 323, 17, WHITE);
    } else if (jogo.estado == ESTADO_GAME_OVER) {
        desenharMensagem("GAME OVER!", "Suas vidas acabaram!", (Color){255, 60, 60, 255});
        DrawRectangle(250, 310, 140, 45, (Color){50, 160, 70, 255});
        DrawRectangleLines(250, 310, 140, 45, WHITE);
        DrawText("REINICIAR", 270, 323, 17, WHITE);

        DrawRectangle(420, 310, 140, 45, (Color){180, 50, 50, 255});
        DrawRectangleLines(420, 310, 140, 45, WHITE);
        DrawText("RANKING", 445, 323, 17, WHITE);
    }
}