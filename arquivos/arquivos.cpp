#include "readarquivo.h"

#include <fstream>
#include <queue>
#include <sstream>

using namespace std;

bool carregarFase(const string &nome, Fase &fase, string &erro) {
    ifstream arq(nome);
    if (!arq) {
        erro = "Nao foi possivel abrir o arquivo " + nome;
        return false;
    }

    string linha;
    int l = 0, inicios = 0, saidas = 0;

    while (getline(arq, linha)) {
        if (!linha.empty() && linha.back() == '\r') linha.pop_back(); // isso é pra caso rode no windows

        if (l >= LINHAS) {
            erro = nome + ": linhas demais (o esperado e " + to_string(LINHAS) + ")";
            return false;
        }
        if ((int)linha.size() != COLUNAS) {
            erro = nome + ": linha " + to_string(l + 1) + " tem " +
                   to_string(linha.size()) + " caracteres (o esperado e " +
                   to_string(COLUNAS) + ")";
            return false;
        }

        for (int c = 0; c < COLUNAS; c++) {
            char ch = linha[c];
            if (ch < '0' || ch > '9') {
                erro = nome + ": caractere invalido '" + string(1, ch) +
                       "' na linha " + to_string(l + 1) + ", coluna " + to_string(c + 1);
                return false;
            }
            int v = ch - '0';
            fase.mapa[l][c] = v;

            if (v == INICIO) {
                inicios++;
                fase.inicioLinha = l;
                fase.inicioColuna = c;
            }
            if (v == SAIDA) saidas++;
        }
        l++;
    }

    if (l != LINHAS) {
        erro = nome + ": tem " + to_string(l) + " linhas (o esperado e " + to_string(LINHAS) + ")";
        return false;
    }
    if (inicios != 1) {
        erro = nome + ": precisa de exatamente 1 posicao inicial (7), possui " + to_string(inicios);
        return false;
    }
    if (saidas < 1) {
        erro = nome + ": nao existe nenhuma saida (4)";
        return false;
    }
    if (!chegouSaida(fase)) {
        erro = nome + ": nao da para chegar ate a saida a partir do inicio";
        return false;
    }

    // a posicao inicial vira caminho livre e dai o robo fica fora da matriz
    fase.mapa[fase.inicioLinha][fase.inicioColuna] = LIVRE;
    return true;
}

bool chegouSaida(const Fase &fase) {
    bool visitado[LINHAS][COLUNAS] = {};
    queue<pair<int, int>> fila;

    fila.push({fase.inicioLinha, fase.inicioColuna});
    visitado[fase.inicioLinha][fase.inicioColuna] = true;

    const int dl[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!fila.empty()) {
        pair<int, int> atual = fila.front();
        fila.pop();

        if (fase.mapa[atual.first][atual.second] == SAIDA) return true;

        for (int i = 0; i < 4; i++) {
            int nl = atual.first + dl[i];
            int nc = atual.second + dc[i];
            if (nl < 0 || nl >= LINHAS || nc < 0 || nc >= COLUNAS) continue;
            if (visitado[nl][nc]) continue;
            if (fase.mapa[nl][nc] == PAREDE_FIXA) continue;
            visitado[nl][nc] = true;
            fila.push({nl, nc});
        }
    }
    return false;
}

bool lerTextoArquivo(const string &nome, string &conteudo) {
    ifstream arq(nome);
    if (!arq) return false;
    stringstream buffer;
    buffer << arq.rdbuf();
    conteudo = buffer.str();
    return true;
}