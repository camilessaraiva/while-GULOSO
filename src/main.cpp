#include "raylib.h"
#include "dados.h"
#include "jogo.h"

int main() {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "WHILE (GULOSO)");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    InitAudioDevice();

    Jogo jogo;
    inicializarJogo(jogo);

    while (!WindowShouldClose() && !jogo.deveFechar) {
        float dt = GetFrameTime();
        if (dt > 0.1f) dt = 0.1f;

        atualizarJogo(jogo, dt);

        BeginDrawing();
        desenharJogo(jogo);
        EndDrawing();
    }

    finalizarJogo(jogo);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
