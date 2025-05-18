/**
 * @file src.c
 * @author Matheus Delgado (a31542@alunos.ipca.pt)
 * @brief Implementacao das funcoes para manipulacao do grafo
 * @version 0.6
 * @date 2025-05-17
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "grafo.h"
#include "struct.h"

 /**
  * @brief Inicializa um grafo vazio
  * @return Apontador para o grafo criado ou NULL em caso de erro
  */
Grafo* inicializarGrafo() {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    if (grafo == NULL) return NULL;
    grafo->primeiro = NULL;
    grafo->numVertices = 0;
    return grafo;
}

/**
 * @brief Liberta a memoria alocada para um grafo
 * @param grafo Apontador para o grafo a ser libertado
 * @return true se liberado com sucesso, false caso contrario
 */
bool libertarGrafo(Grafo* grafo) {
    if (grafo == NULL) return false;
    NoVertice* atual = grafo->primeiro;
    while (atual != NULL) {
        NoVertice* proximo = atual->proximo;
        Aresta* aresta = atual->primeiraAresta;
        while (aresta != NULL) {
            Aresta* proxima = aresta->proxima;
            free(aresta);
            aresta = proxima;
        }
        free(atual);
        atual = proximo;
    }
    free(grafo);
    return true;
}

/**
 * @brief Adiciona um vertice (antena) ao grafo
 * @param grafo Apontador para o grafo
 * @param antena Dados da antena a ser adicionada
 * @return Apontador para o novo vertice ou NULL em caso de erro
 */
NoVertice* adicionarVertice(Grafo* grafo, Antena antena) {
    if (grafo == NULL) return NULL;
    // Verificar coordenadas válidas (exemplo: não negativas)
    if (antena.posicao.x < 0 || antena.posicao.y < 0) {
        return NULL;
    }
    NoVertice* atual = grafo->primeiro;
    while (atual != NULL) {
        if (atual->dados.posicao.x == antena.posicao.x &&
            atual->dados.posicao.y == antena.posicao.y) {
            return atual;
        }
        atual = atual->proximo;
    }
    NoVertice* novo = (NoVertice*)malloc(sizeof(NoVertice));
    if (novo == NULL) return NULL;
    novo->dados = antena;
    novo->primeiraAresta = NULL;
    novo->proximo = grafo->primeiro;
    grafo->primeiro = novo;
    grafo->numVertices++;
    return novo;
}

/**
 * @brief Verifica se existe uma aresta entre dois vertices
 * @param origem Apontador para o vertice de origem
 * @param destino Apontador para o vertice de destino
 * @return true se existe aresta, false caso contrario
 */
bool existeAresta(NoVertice* origem, NoVertice* destino) {
    if (origem == NULL || destino == NULL) return false;
    Aresta* atual = origem->primeiraAresta;
    while (atual != NULL) {
        if (atual->destino == destino) return true;
        atual = atual->proxima;
    }
    return false;
}

/**
 * @brief Adiciona uma aresta entre dois vertices
 * @param grafo Apontador para o grafo
 * @param origem Apontador para o vertice de origem
 * @param destino Apontador para o vertice de destino
 * @return true se a aresta foi adicionada, false caso contrario
 */
bool adicionarAresta(Grafo* grafo, NoVertice* origem, NoVertice* destino) {
    if (grafo == NULL || origem == NULL || destino == NULL) return false;
    if (existeAresta(origem, destino)) return true;
    Aresta* novaAresta = (Aresta*)malloc(sizeof(Aresta));
    if (novaAresta == NULL) return false;
    novaAresta->destino = destino;
    novaAresta->proxima = origem->primeiraAresta;
    origem->primeiraAresta = novaAresta;
    return true;
}

/**
 * @brief Carrega os dados das antenas de um ficheiro para um grafo
 * @param nomeFicheiro Nome do ficheiro a ser lido
 * @return Apontador para o grafo criado ou NULL em caso de erro
 */
Grafo* carregarDadosGrafo(const char* nomeFicheiro) {
    FILE* ficheiro = fopen(nomeFicheiro, "r");
    if (ficheiro == NULL) {
        perror("Erro ao abrir ficheiro");
        return NULL;
    }
    Grafo* grafo = inicializarGrafo();
    if (grafo == NULL) {
        fclose(ficheiro);
        return NULL;
    }
    char linha[256];
    int y = 0;
    while (fgets(linha, sizeof(linha), ficheiro) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        size_t tamanho = strlen(linha);
        for (int x = 0; linha[x] != '\0'; x++) {
            char c = linha[x];
            if (c != '.' && c != ' ') {
                Antena antena = { c, {x, y} };
                if (adicionarVertice(grafo, antena) == NULL) {
                    libertarGrafo(grafo);
                    fclose(ficheiro);
                    return NULL;
                }
            }
        }
        y++;
    }
    fclose(ficheiro);
    NoVertice* v1 = grafo->primeiro;
    while (v1 != NULL) {
        NoVertice* v2 = v1->proximo;
        while (v2 != NULL) {
            if (v1->dados.frequencia == v2->dados.frequencia) {
                adicionarAresta(grafo, v1, v2);
                adicionarAresta(grafo, v2, v1);
            }
            v2 = v2->proximo;
        }
        v1 = v1->proximo;
    }
    return grafo;
}

/**
 * @brief Busca em profundidade a partir de um vertice
 * @param grafo Apontador para o grafo
 * @param verticeInicial Apontador para o vertice inicial
 * @param visitados Array para marcar os vertices visitados
 * @param resultado Lista para armazenar os vertices visitados
 * @param tamanhoResultado Apontador para o tamanho do resultado
 * @return true se a busca foi realizada, false caso contrario
 */
bool buscaEmProfundidade(Grafo* grafo, NoVertice* verticeInicial, bool* visitados, NoVertice*** resultado, int* tamanhoResultado) {
  
    if (grafo == NULL || verticeInicial == NULL || visitados == NULL || resultado == NULL || tamanhoResultado == NULL)
        return false;

    if (*resultado == NULL) {
        *resultado = (NoVertice**)calloc(grafo->numVertices, sizeof(NoVertice*));
        if (*resultado == NULL) return false;
        *tamanhoResultado = 0; 
    }

    int indice = 0;
    NoVertice* atual = grafo->primeiro;
    while (atual != NULL) {
        if (atual == verticeInicial) break;
        indice++;
        atual = atual->proximo;
    }

    if (indice >= grafo->numVertices) {
        return false;
    }

    if (!visitados[indice]) {
        visitados[indice] = true;
        if (*tamanhoResultado < grafo->numVertices) {
            (*resultado)[*tamanhoResultado] = verticeInicial;
            (*tamanhoResultado)++;
        }

        Aresta* aresta = verticeInicial->primeiraAresta;
        while (aresta != NULL) {
            if (!buscaEmProfundidade(grafo, aresta->destino, visitados, resultado, tamanhoResultado)) {
                return false;
            }
            aresta = aresta->proxima;
        }
    }

    return true;
}

/**
 * @brief Inicializa uma fila vazia
 * @return Apontador para a fila criada
 */
Fila* inicializarFila() {
    Fila* fila = (Fila*)malloc(sizeof(Fila));
    if (fila == NULL) return NULL;
    fila->frente = NULL;
    fila->tras = NULL;
    return fila;
}

/**
 * @brief Verifica se uma fila está vazia
 * @param fila Apontador para a fila
 * @return true se a fila está vazia, false caso contrario
 */
bool filaVazia(Fila* fila) {
    return (fila == NULL || fila->frente == NULL);
}

/**
 * @brief Adiciona um vertice ao final da fila
 * @param fila Apontador para a fila
 * @param valor Apontador para o vertice a ser adicionado
 * @return true se adicionado com sucesso, false caso contrario
 */
bool enfileirar(Fila* fila, NoVertice* valor) {
    if (fila == NULL || valor == NULL) return false;
    ElementoFila* novoElemento = (ElementoFila*)malloc(sizeof(ElementoFila));
    if (novoElemento == NULL) return false;
    novoElemento->valor = valor;
    novoElemento->proximo = NULL;
    if (filaVazia(fila)) {
        fila->frente = novoElemento;
    }
    else {
        fila->tras->proximo = novoElemento;
    }
    fila->tras = novoElemento;
    return true;
}

/**
 * @brief Remove o vertice da frente da fila
 * @param fila Apontador para a fila
 * @return Apontador para o vertice removido ou NULL se vazia
 */
NoVertice* desenfileirar(Fila* fila) {
    if (filaVazia(fila)) return NULL;
    ElementoFila* elementoRemovido = fila->frente;
    NoVertice* valor = elementoRemovido->valor;
    fila->frente = elementoRemovido->proximo;
    if (fila->frente == NULL) fila->tras = NULL;
    free(elementoRemovido);
    return valor;
}

/**
 * @brief Liberta a memoria alocada para uma fila
 * @param fila Apontador para a fila a ser libertada
 * @return true se liberada com sucesso, false caso contrario
 */
bool libertarFila(Fila* fila) {
    if (fila == NULL) return false;
    while (!filaVazia(fila)) desenfileirar(fila);
    free(fila);
    return true;
}

/**
 * @brief Busca em largura a partir de um vertice
 * @param grafo Apontador para o grafo
 * @param verticeInicial Apontador para o vertice inicial
 * @param resultado Lista para armazenar os vertices visitados
 * @param tamanhoResultado Apontador para o tamanho do resultado
 * @return true se a busca foi realizada, false caso contrario
 */
bool buscaEmLargura(Grafo* grafo, NoVertice* verticeInicial, bool* visitados, NoVertice*** resultado, int* tamanhoResultado) {
    if (grafo == NULL || verticeInicial == NULL || visitados == NULL || resultado == NULL || tamanhoResultado == NULL) return false;
    if (*resultado != NULL) {
        free(*resultado); // Evitar vazamento se já alocado
        *resultado = NULL;
    }
    *resultado = (NoVertice**)calloc(grafo->numVertices, sizeof(NoVertice*));
    if (*resultado == NULL) return false;
    Fila* fila = inicializarFila();
    if (fila == NULL) {
        free(*resultado);
        *resultado = NULL;
        return false;
    }
    int indiceInicial = 0;
    NoVertice* atual = grafo->primeiro;
    while (atual != NULL) {
        if (atual == verticeInicial) break;
        indiceInicial++;
        atual = atual->proximo;
    }
    if (indiceInicial >= grafo->numVertices) {
        libertarFila(fila);
        free(*resultado);
        *resultado = NULL;
        return false;
    }
    visitados[indiceInicial] = true;
    enfileirar(fila, verticeInicial);
    *tamanhoResultado = 0;
    while (!filaVazia(fila)) {
        NoVertice* verticeAtual = desenfileirar(fila);
        if (*tamanhoResultado >= grafo->numVertices) {
            libertarFila(fila);
            return false;
        }
        NoVertice** arrayResultado = *resultado;
        arrayResultado[*tamanhoResultado] = verticeAtual;
        (*tamanhoResultado)++;
        Aresta* aresta = verticeAtual->primeiraAresta;
        while (aresta != NULL) {
            int destinoIndice = 0;
            atual = grafo->primeiro;
            while (atual != NULL) {
                if (atual == aresta->destino) break;
                destinoIndice++;
                atual = atual->proximo;
            }
            if (destinoIndice < grafo->numVertices && !visitados[destinoIndice]) {
                visitados[destinoIndice] = true;
                enfileirar(fila, aresta->destino);
            }
            aresta = aresta->proxima;
        }
    }
    libertarFila(fila);
    return true;
}
/**
 * @brief Adiciona um vertice a um caminho
 * @param caminho Apontador para o caminho
 * @param vertice Apontador para o vertice a ser adicionado
 * @return Apontador para o novo caminho
 */
ElementoCaminho* adicionarVerticeCaminho(ElementoCaminho* caminho, NoVertice* vertice) {
    ElementoCaminho* novo = (ElementoCaminho*)malloc(sizeof(ElementoCaminho));
    if (novo == NULL) return NULL;
    novo->vertice = vertice;
    novo->proximo = NULL;
    if (caminho == NULL) {
        return novo;
    }
    ElementoCaminho* atual = caminho;
    while (atual != NULL) {
        if (atual->proximo == NULL) break;
        atual = atual->proximo;
    }
    if (atual != NULL) {
        atual->proximo = novo;
    }
    return caminho;
}

/**
 * @brief Copia um caminho
 * @param caminho Apontador para o caminho a ser copiado
 * @return Apontador para o novo caminho
 */
ElementoCaminho* copiarCaminho(ElementoCaminho* caminho) {
    if (caminho == NULL) return NULL;
    ElementoCaminho* novoCaminho = NULL;
    ElementoCaminho* atual = caminho;
    while (atual != NULL) {
        novoCaminho = adicionarVerticeCaminho(novoCaminho, atual->vertice);
        atual = atual->proximo;
    }
    return novoCaminho;
}

/**
 * @brief Adiciona um caminho à lista de caminhos
 * @param caminhos Apontador para a lista de caminhos
 * @param caminho Apontador para o caminho a ser adicionado
 * @return true se adicionado com sucesso, false caso contrario
 */
bool adicionarCaminho(ListaCaminho** caminhos, ElementoCaminho* caminho) {
    if (caminhos == NULL || caminho == NULL) return false;
    ListaCaminho* novoCaminho = (ListaCaminho*)malloc(sizeof(ListaCaminho));
    if (novoCaminho == NULL) return false;
    novoCaminho->caminho = caminho;
    novoCaminho->proximo = NULL;
    if (*caminhos == NULL) {
        *caminhos = novoCaminho;
    }
    else {
        ListaCaminho* atual = *caminhos;
        while (atual->proximo != NULL) atual = atual->proximo;
        atual->proximo = novoCaminho;
    }
    return true;
}

/**
 * @brief Liberta a memoria alocada para um caminho
 * @param caminho Apontador para o caminho a ser libertado
 * @return true se liberado com sucesso false caso contrario
 */
bool libertarCaminho(ElementoCaminho* caminho) {
    if (caminho == NULL) return false;
    ElementoCaminho* atual = caminho;
    while (atual != NULL) {
        ElementoCaminho* proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    return true;
}

/**
 * @brief Liberta a memoria alocada para uma lista de caminhos
 * @param caminhos Apontador para a lista de caminhos a ser libertada
 * @return true se liberada com sucesso, false caso contrario
 */
bool libertarCaminhos(ListaCaminho* caminhos) {
    if (caminhos == NULL) return true;
    ListaCaminho* atual = caminhos;
    while (atual != NULL) {
        ListaCaminho* proximo = atual->proximo;
        if (atual->caminho != NULL) {
            libertarCaminho(atual->caminho);
        }
        free(atual);
        atual = proximo;
    }
    return true;
}


/**
 * @brief Funcao auxiliar para encontrar caminhos recursivamente
 * @param grafo Apontador para o grafo
 * @param atual Apontador para o vertice atual
 * @param destino Apontador para o vertice de destino
 * @param visitados Array para marcar os vertices visitados
 * @param caminho Caminho atual
 * @param caminhos Apontador para a lista de caminhos
 * @return true se pelo menos um caminho foi encontrado, false caso contrario
 */
bool encontrarCaminhosRecursivo(Grafo* grafo, NoVertice* atual, NoVertice* destino, bool* visitados, ElementoCaminho* caminho, ListaCaminho** caminhos) {
    if (grafo == NULL || atual == NULL || destino == NULL || visitados == NULL || caminhos == NULL) return false;
    int indiceAtual = 0;
    NoVertice* temp = grafo->primeiro;
    while (temp != NULL) {
        if (temp == atual) break;
        indiceAtual++;
        temp = temp->proximo;
    }
    if (indiceAtual >= grafo->numVertices) return false;
    visitados[indiceAtual] = true;
    ElementoCaminho* novoCaminho = adicionarVerticeCaminho(caminho, atual);
    if (novoCaminho == NULL) {
        visitados[indiceAtual] = false;
        return false;
    }
    bool encontrouCaminho = false;
    if (atual == destino) {
        ElementoCaminho* caminhoCopiado = copiarCaminho(novoCaminho);
        if (caminhoCopiado != NULL) {
            encontrouCaminho = adicionarCaminho(caminhos, caminhoCopiado);
        }
    }
    else {
        Aresta* aresta = atual->primeiraAresta;
        while (aresta != NULL) {
            int destinoIndice = 0;
            temp = grafo->primeiro;
            while (temp != NULL) {
                if (temp == aresta->destino) break;
                destinoIndice++;
                temp = temp->proximo;
            }
            if (destinoIndice < grafo->numVertices && !visitados[destinoIndice]) {
                if (encontrarCaminhosRecursivo(grafo, aresta->destino, destino, visitados, novoCaminho, caminhos)) {
                    encontrouCaminho = true;
                }
            }
            aresta = aresta->proxima;
        }
    }
    visitados[indiceAtual] = false;
    return encontrouCaminho;
}

/**
 * @brief Encontra todos os caminhos entre dois vertices
 * @param grafo Apontador para o grafo
 * @param origem Apontador para o vertice de origem
 * @param destino Apontador para o vertice de destino
 * @param caminhos Apontador para a lista de caminhos
 * @return true se pelo menos um caminho foi encontrado, false caso contrario
 */
bool encontrarCaminhos(Grafo* grafo, NoVertice* origem, NoVertice* destino, ListaCaminho** caminhos) {
    if (grafo == NULL || origem == NULL || destino == NULL || caminhos == NULL) return false;
    bool* visitados = (bool*)calloc(grafo->numVertices, sizeof(bool));
    if (visitados == NULL) return false;
    *caminhos = NULL;
    bool resultado = encontrarCaminhosRecursivo(grafo, origem, destino, visitados, NULL, caminhos);
    free(visitados);
    return resultado;
}

/**
 * @brief Encontra o vertice com as coordenadas especificadas
 * @param grafo Apontador para o grafo
 * @param x Coordenada x
 * @param y Coordenada y
 * @return Apontador para o vertice ou NULL se nao encontrado
 */
NoVertice* encontrarVerticePorCoordenadas(Grafo* grafo, int x, int y) {
    if (grafo == NULL) return NULL;
    NoVertice* atual = grafo->primeiro;
    while (atual != NULL) {
        if (atual->dados.posicao.x == x && atual->dados.posicao.y == y) return atual;
        atual = atual->proximo;
    }
    return NULL;
}

/**
 * @brief Encontra todos os vertices com uma determinada frequencia
 * @param grafo Apontador para o grafo
 * @param frequencia Frequencia a ser procurada
 * @param vertices Array para armazenar os vertices encontrados
 * @param tamanhoVertices Apontador para o tamanho do array de vertices
 * @return true se pelo menos um vertice foi encontrado, false caso contrario
 */
bool encontrarVerticesPorFrequencia(Grafo* grafo, char frequencia, NoVertice** vertices, int* tamanhoVertices, int maxVertices) {
    if (grafo == NULL || vertices == NULL || tamanhoVertices == NULL) return false;
    *tamanhoVertices = 0;
    NoVertice* atual = grafo->primeiro;
    while (atual != NULL) {
        if (atual->dados.frequencia == frequencia) {
            if (*tamanhoVertices >= maxVertices) break;
            vertices[*tamanhoVertices] = atual;
            (*tamanhoVertices)++;
        }
        atual = atual->proximo;
    }
    return true;
}

/**
 * @brief Encontra intersecoes entre antenas de frequencias diferentes
 * @param grafo Apontador para o grafo
 * @param frequenciaA Frequencia A
 * @param frequenciaB Frequencia B
 * @param intersecoes Array para armazenar as intersecoes
 * @param tamanhoIntersecoes Apontador para o tamanho do array de intersecoes
 * @return true se pelo menos uma intersecao foi encontrada, false caso contrario
 */
bool encontrarIntersecoes(Grafo* grafo, char frequenciaA, char frequenciaB, Intersecao* intersecoes, int* tamanhoIntersecoes, int maxIntersecoes) {
    if (grafo == NULL || intersecoes == NULL || tamanhoIntersecoes == NULL) return false;
    *tamanhoIntersecoes = 0;
    int maxVertices = grafo->numVertices; // Tamanho máximo dos arrays verticesA e verticesB
    NoVertice** verticesA = (NoVertice**)malloc(maxVertices * sizeof(NoVertice*));
    if (verticesA == NULL) return false;
    int tamanhoVerticesA = 0;
    if (!encontrarVerticesPorFrequencia(grafo, frequenciaA, verticesA, &tamanhoVerticesA, maxVertices) || tamanhoVerticesA == 0) {
        free(verticesA);
        return false;
    }
    NoVertice** verticesB = (NoVertice**)malloc(maxVertices * sizeof(NoVertice*));
    if (verticesB == NULL) {
        free(verticesA);
        return false;
    }
    int tamanhoVerticesB = 0;
    if (!encontrarVerticesPorFrequencia(grafo, frequenciaB, verticesB, &tamanhoVerticesB, maxVertices) || tamanhoVerticesB == 0) {
        free(verticesA);
        free(verticesB);
        return false;
    }
    for (int i = 0; i < tamanhoVerticesA; i++) {
        for (int j = 0; j < tamanhoVerticesB; j++) {
            if (*tamanhoIntersecoes >= maxIntersecoes) break;
            intersecoes[*tamanhoIntersecoes].antenaA = verticesA[i]->dados.posicao;
            intersecoes[*tamanhoIntersecoes].antenaB = verticesB[j]->dados.posicao;
            (*tamanhoIntersecoes)++;
        }
    }
    free(verticesA);
    free(verticesB);
    return (*tamanhoIntersecoes > 0);
}