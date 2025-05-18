#include <stdio.h>
#include <stdlib.h>
#include "dados.h"
#include "funcoes.h"

#define FICHEIRO_MATRIZ "matriz.txt"

int main() {
    Antena* listaAntenas = NULL;
    Nefasto* listaEfeitos = NULL;

    Antena* antena1 = criaAntena(1, 2, 101, 'A');
    Antena* antena2 = criaAntena(3, 4, 102, 'B');
    Antena* antena3 = criaAntena(1, 4, 103, 'A');
    Antena* antena4 = criaAntena(4, 4, 104, 'B');
    Antena* antena5 = criaAntena(10, 2, 105, 'C');

    listaAntenas = inserirOrdenado(listaAntenas, antena1);
    listaAntenas = inserirOrdenado(listaAntenas, antena2);
    listaAntenas = inserirOrdenado(listaAntenas, antena3);
	listaAntenas = inserirOrdenado(listaAntenas, antena4);
	listaAntenas = inserirOrdenado(listaAntenas, antena5);

    listaEfeitos = atualizaEfeito(listaAntenas);

    mostraLista(listaAntenas, listaEfeitos);

    if (gravarMatrizTxt(listaAntenas, listaEfeitos, FICHEIRO_MATRIZ)) {
        printf("\nMatriz gravada com sucesso em '%s'\n", FICHEIRO_MATRIZ);
    }
    else {
        printf("\nErro ao gravar a matriz.\n");
    }

    if (gravarFicheiroB(listaAntenas, "lista.bin", listaEfeitos)) {
        printf("\nLista de antenas gravada com sucesso em 'lista.bin'\n");
    }
    else {
        printf("\nErro ao gravar a lista de antenas.\n");
    }

    // Remove uma antena
    listaAntenas = removeAntena(listaAntenas, 3, 4);

    listaEfeitos = atualizaEfeito(listaAntenas);

    printf("\nDepois de remover uma antena:\n");
    mostraLista(listaAntenas, listaEfeitos);

    // liberta a memoria
    listaAntenas = DestroiListaAntenas(listaAntenas);
    listaEfeitos = DestroiListaEfeitos(listaEfeitos);

    return 0;
}
