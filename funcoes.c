/**
 * @file funcoes.c
 * @author Matheus Delgado (a31542 IPCA)
 * @brief Implementacao das funcoes para manipular as antenas e os efeitos nefastos
 * @details Este ficheiro contem as funcoes para criar, inserir, remover e alterar antenas
 * e cacular os efeitos nefastos entre antenas com a mesma frequencia.
 * As funcoes tambem incluem a gravacao e leitura de dados em ficheiros binarios e de texto para representacao grafica
 *
 * @version 0.1
 * @date 2025-03-25
 *
 * @copyright Copyright (c) 2025
 *
 */
#define _CRT_SECURE_NO_WARNINGS //para poder usar fopen sem erro
#include "dados.h"
#include "funcoes.h"



 /**
  * @brief Funcao para procurar uma antena na lista
  *
  * @param h (apontador para o incidio da lista)
  * @param v (id da antena a procurar)
  * @return Antena*
  */
Antena* ProcuraAntena(Antena* h, int v) {
    Antena* aux = h;
    while (aux && aux->id < v) {
        aux = aux->next;
    }
    if (aux && aux->id == v) return aux; //se encontrar vai retornar o apontador da antena
    return NULL;
}
/**
 * @brief Funcao para criar uma nova antena
 *
 * @param linha
 * @param coluna
 * @param id
 * @param frequencia
 * @return Antena*
 */
Antena* criaAntena(int linha, int coluna, int id, char frequencia) {
    Antena* aux;
    aux = (Antena*)malloc(sizeof(Antena));
    if (aux != NULL) {
        aux->id = id;
        aux->linha = linha;
        aux->coluna = coluna;
        aux->frequencia = frequencia;
        aux->next = NULL;
    }
    return aux;
}
/**
 * @brief Insere a nova antena na lista de forma ordenada
 *
 * @param head (inicio da lista)
 * @param novo (apontador para a nova antena)
 * @return Antena*
 */
Antena* inserirOrdenado(Antena* head, Antena* novo) {
    if (novo == NULL) return head;
    if (ProcuraAntena(head, novo->id)) return head;

    // Se a lista estiver vazia ou se o novo elemento for menor que o primeiro, insere no início
    if (head == NULL || (novo->linha < head->linha) ||
        (novo->linha == head->linha && novo->coluna < head->coluna)) {
        novo->next = head;
        head = novo;
        return head;
    }

    // Procura a posição correta para inserir o novo elemento
    Antena* aux = head;
    while (aux->next != NULL &&
        ((aux->next->linha < novo->linha) ||
            (aux->next->linha == novo->linha && aux->next->coluna < novo->coluna))) {
        aux = aux->next;
    }
    novo->next = aux->next;
    aux->next = novo;
    return head;
}

/**
 * @brief Funcao para remover uma antena da lista
 *
 * @param h (apontador para inicio da lista)
 * @param l (linha da antena)
 * @param c (coluna da antena)
 * @return Antena* (retorna a lista atualizada)
 */
Antena* removeAntena(Antena* h, int l, int c) {
    if (h == NULL) return NULL;

    // Se for a primeira antena da lista
    if (h->linha == l && h->coluna == c) {
        Antena* aux = h->next;
        free(h);
        return aux; // Atualiza o inicio da lista
    }

    // Se nao for o primeiro
    Antena* atual = h;
    Antena* anterior = NULL;
    while (atual != NULL && (atual->linha != l || atual->coluna != c)) {
        anterior = atual;
        atual = atual->next;
    }

    if (atual != NULL) { // se encontrou
        anterior->next = atual->next;
        free(atual);
    }
    return h;
}


/**
 * @brief Funcao para alterar os dados de uma antena
 *
 * @param h (apontador para inicio da lista)
 * @param id
 * @param l
 * @param c
 * @param f
 * @return Antena*
 */
Antena* alteraAntena(Antena* h, int id, int l, int c, char f) {
    Antena* aux = ProcuraAntena(h, id);
    if (aux != NULL) {
        aux->linha = l;
        aux->coluna = c;
        aux->frequencia = f;
        aux->id = id;
        return h;
    }
    else
        return h;
}

/**
 * @brief Funcao que vai inserir os efeitos nefastos apos serem calcualdos
 *
 * @param head (apontador para o inicio da lista de efeitos)
 * @param l
 * @param c
 * @param id1 (id de uma das antenas que estao a causar o efeito)
 * @param id2 (id da outra antena que estao a causar o efeito)
 * @return Nefasto*
 */
Nefasto* inserirEfeito(Nefasto* head, int l, int c, int id1, int id2) {
    Nefasto* novo = (Nefasto*)malloc(sizeof(Nefasto));
    if (!novo) return head;

    novo->linha = l;
    novo->coluna = c;
    novo->idAntena1 = id1;
    novo->idAntena2 = id2;
    novo->next = NULL;

    // Se lista for nula ou se for menor que o primeiro, insere a frente
    if (head == NULL ||
        (l < head->linha) || (l == head->linha && c < head->coluna)) {
        novo->next = head;
        return novo;
    }

    // se nao, percorre ate achar a posicao
    Nefasto* aux = head;
    while (aux->next != NULL &&
        (aux->next->linha < l ||
            (aux->next->linha == l && aux->next->coluna < c))) {
        aux = aux->next;
    }

    // Insere o novo no meio ou no final
    novo->next = aux->next;
    aux->next = novo;

    return head;
}


/**
 * @brief A função percorre a lista de antenas e calcula os efeitos entre antenas com a mesma frequencia
 * e atualiza a lista removendo efeitos se a antena for removida ou alterada
 * @param h
 * @return Nefasto*
 */
Nefasto* atualizaEfeito(Antena* listaAntenas) {
    // lista de efeitos nova 
    Nefasto* listaEfeitos = NULL;

    // Percorre todas as antenas
    for (Antena* a1 = listaAntenas; a1 != NULL; a1 = a1->next) {
        for (Antena* a2 = a1->next; a2 != NULL; a2 = a2->next){
            // Se a frequencia for diferente, ignora
            if (a1->frequencia != a2->frequencia)
                continue;

            // calculo dos pontos de efeito
            int efeitoX1 = 2 * a1->linha - a2->linha;
            int efeitoY1 = 2 * a1->coluna - a2->coluna;
            int efeitoX2 = 2 * a2->linha - a1->linha;
            int efeitoY2 = 2 * a2->coluna - a1->coluna;

            // Insere os dois efeitos na lista
            listaEfeitos = inserirEfeito(listaEfeitos, efeitoX1, efeitoY1, a1->id, a2->id);
            listaEfeitos = inserirEfeito(listaEfeitos, efeitoX2, efeitoY2, a2->id, a1->id);
        }
    }

    return listaEfeitos;
}


/**
 * @brief Mosta a lista de antenas e efeitos nefastos de forma tabular
 *
 * (apontadores para incio das listas)
 * @param antenas
 * @param efeitos
 */
void mostraLista(Antena* antenas, Nefasto* efeitos) {
    printf("Lista de Antenas:\n\n");
    printf("|  ID  | Frequencia | Linha | Coluna |\n");
    for (Antena* aux = antenas; aux != NULL; aux = aux->next) {
        printf("| %4d |     %c      | %5d | %6d |\n",
            aux->id, aux->frequencia, aux->linha, aux->coluna);
    }

    printf("Lista de Efeitos Nefastos:\n\n");
    printf("| Linha | Coluna | Antena 1 | Antena 2 |\n");
    for (Nefasto* aux = efeitos; aux != NULL; aux = aux->next) {
        printf("| %5d | %6d | %8d | %8d |\n",
            aux->linha, aux->coluna, aux->idAntena1, aux->idAntena2);
    }
}
/**
 * @brief Liberta a memoria da lista de Antenas
 *
 * @param h Ponteiro para o inicio da lista de antenas.
 * @return Antena* Retorna NULL após liberar todos antenas.
 */
Antena* DestroiListaAntenas(Antena* h) {
    Antena* aux;
    while (h != NULL) {
        aux = h;
        h = h->next;
        free(aux);
    }
    return NULL;
}

/**
 * @brief Liberta a memoria da lista de Efeitos Nefastos
 *
 * @param h Ponteiro para o inicio da lista de efeitos nefastos.
 * @return Nefasto* Retorna NULL após liberar todos efeitos nefastos.
 */
Nefasto* DestroiListaEfeitos(Nefasto* h) {
    Nefasto* aux;
    while (h != NULL) {
        aux = h;
        h = h->next;
        free(aux);
    }
    return NULL;
}   

#pragma region FICHEIROS

/**
 * @brief Funcao booleana para gravas os dados das antenas e efeitos nefastos num ficheiro binario
 * se o ficheiro ja existir, os dados serao sobrescritos, se nao existir, o ficheiro sera criado
 *
 * @param h (apontador para o inicio da lista de antenas)
 * @param nomeFicheiro (o nome do ficheiro passado como argumento)
 * @param efeitos (apontador para inicio da lista de efeitos nefastos)
 * @return true
 * @return false
 */
bool gravarFicheiroB(Antena* h, const char* nomeFicheiro, Nefasto* efeitos) {
    FILE* fp;
    if (h == NULL) return false; //verificacao
    if ((fp = fopen(nomeFicheiro, "wb")) == NULL) return false;
    Antena* aux = h;
    while (aux != NULL) {
        //grava os dados no ficheiro sem gravar o next para que possa ser reconstruida depois
        fwrite(&aux->linha, sizeof(int), 1, fp);
        fwrite(&aux->coluna, sizeof(int), 1, fp);
        fwrite(&aux->frequencia, sizeof(char), 1, fp);
        fwrite(&aux->id, sizeof(int), 1, fp);
        aux = aux->next;
    }
    Nefasto* aux2 = efeitos;
    while (aux2 != NULL) {
        fwrite(&aux2->linha, sizeof(int), 1, fp);
        fwrite(&aux2->coluna, sizeof(int), 1, fp);
        fwrite(&aux2->idAntena1, sizeof(int), 1, fp);
        fwrite(&aux2->idAntena2, sizeof(int), 1, fp);
        aux2 = aux2->next;
    }
    fclose(fp);
    return true;
}

/**
 * @brief A funcao grava uma matriz num ficheiro de texto para uma representacao grafica
 * as antenas sao representadas pela letra da sua frequencia
 * os efeitos nefastos sao representados por "#"
 *
 * retorna true se a matriz for gravada com sucesso, false se houver erro
 *
 * @param h (apontador para o inicio da lista de antenas)
 * @param efeitos (apontador para o inicio da lista de efeitos nefastos)
 * @param ficheiro (o nome do ficheiro passado como argumento)
 * @return true
 * @return false
 */
bool gravarMatrizTxt(Antena* h, Nefasto* efeitos, const char* ficheiro) {
    if (h == NULL) return false;

    int maxLinha, maxColuna;
    maxLinha = 0;
    maxColuna = 0;

    //dimensoes da matriz
    for (Antena* aux = h; aux != NULL; aux = aux->next) {
        if (aux->linha > maxLinha) maxLinha = aux->linha;
        if (aux->coluna > maxColuna) maxColuna = aux->coluna;
    }
    for (Nefasto* aux = efeitos; aux != NULL; aux = aux->next) {
        if (aux->linha > maxLinha) maxLinha = aux->linha;
        if (aux->coluna > maxColuna) maxColuna = aux->coluna;
    }

    FILE* fp = fopen(ficheiro, "w");
    if (fp == NULL) return false;

    for (int i = 1; i <= maxLinha; i++) {
        for (int j = 1; j <= maxColuna; j++) {
            char elemento = '.';

            for (Antena* aux = h; aux != NULL; aux = aux->next) {
                if (aux->linha == i && aux->coluna == j) {
                    elemento = aux->frequencia;
                    break; //busca a antena para adicionar como elemento da matriz
                }
            }
            if (elemento == '.') {
                for (Nefasto* aux = efeitos; aux != NULL; aux = aux->next) {
                    if (aux->linha == i && aux->coluna == j) {
                        elemento = '#'; //verifica se existe efeito nefasto na posicao
                        break;
                    }
                }
            }
            fprintf(fp, "%c ", elemento);
        }
        fprintf(fp, "\n"); //adiciona nova linha
    }
    fclose(fp);
    return true;
}

/**
 * @brief Funcao para ler o ficheiro binario e recriar a lista de antenas
 *
 * @param nomeFicheiro
 * @return Antena*
 */
Antena* lerFicheirobinario(char* nomeFicheiro) {
    FILE* fp;
    if ((fp = fopen(nomeFicheiro, "rb")) == NULL) return NULL; //verificacao
    Antena* head = NULL;
    Antena* aux = NULL; //inicializado em null para evitar erros
    int l, c, id;
    char f;
    while (fread(&l, sizeof(int), 1, fp) == 1 && fread(&c, sizeof(int), 1, fp) == 1 && fread(&f, sizeof(char), 1, fp) == 1 && fread(&id, sizeof(int), 1, fp) == 1) { //le cada dado de cada antena e depois usa as funcoes de criar e inserir para recriar a lista
        aux = criaAntena(l, c, id, f);
        head = inserirOrdenado(head, aux); //passa NULL para efeitos, pois nao e necessario
    }
    Nefasto* efeitos = NULL;
    int l2, c2, id1, id2;
    while (fread(&l2, sizeof(int), 1, fp) == 1 && fread(&c2, sizeof(int), 1, fp) == 1 && fread(&id1, sizeof(int), 1, fp) == 1 && fread(&id2, sizeof(int), 1, fp) == 1) {

        efeitos = inserirEfeito(efeitos, l2, c2, id1, id2);

    }
    fclose(fp);
    return head;
}
#pragma endregion
