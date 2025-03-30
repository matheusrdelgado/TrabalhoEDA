/**

    @file      funcoes.h
	@brief     Assinaturas das funcoes para manipular as antenas e os efeitos nefastos
    @author    Matheus Delgado (a31542 IPCA)
    @date      25.03.2025
    @copyright © Matheus Delgado, 2025. All right reserved.

**/
#pragma once

#pragma once
#include "dados.h"


Antena* ProcuraAntena(Antena* h, int v);
Antena* criaAntena(int linha, int coluna, int id, char frequencia);
Antena* inserirOrdenado(Antena* head, Antena* novo);
Antena* removeAntena(Antena* h, int l, int c);
Antena* alteraAntena(Antena* h, int id, int l, int c, char f);
Nefasto* inserirEfeito(Nefasto* head, int l, int c, int id1, int id2);
Nefasto* atualizaEfeito(Antena* h);
void mostraLista(Antena* antenas, Nefasto* efeitos);
bool gravarFicheiroB(Antena* h, const char* nomeFicheiro, Nefasto* efeitos);
bool gravarMatrizTxt(Antena* h, Nefasto* efeitos, const char* ficheiro);
Antena* lerFicheirobinario(char* nomeFicheiro);
Antena* DestroiListaAntenas(Antena* h);
Nefasto* DestroiListaEfeitos(Nefasto* h);