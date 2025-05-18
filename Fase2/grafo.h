#pragma once
/**
 * @file grafo.h
 * @author Matheus Delgado (a31542@alunos.ipca.pt)
 * @brief Definicoes das funcoes para manipulacao do grafo
 * @version 0.4
 * @date 2025-05-17
 * @copyright Copyright (c) 2025
 */
#ifndef GRAFO_H
#define GRAFO_H

#include <stdbool.h>
#include "struct.h"

 /**
  * @brief Inicializa um grafo vazio
  * @return Apontador para o grafo criado ou NULL em caso de erro
  */
Grafo* inicializarGrafo();

/**
 * @brief Liberta a memoria alocada para um grafo
 * @param grafo Apontador para o grafo a ser libertado
 * @return true se liberado com sucesso, false caso contrario
 */
bool libertarGrafo(Grafo* grafo);

/**
 * @brief Adiciona um vertice (antena) ao grafo
 * @param grafo Apontador para o grafo
 * @param antena Dados da antena a ser adicionada
 * @return Apontador para o novo vertice ou NULL em caso de erro
 */
NoVertice* adicionarVertice(Grafo* grafo, Antena antena);

/**
 * @brief Verifica se existe uma aresta entre dois vertices
 * @param origem Apontador para o vertice de origem
 * @param destino Apontador para o vertice de destino
 * @return true se existe aresta, false caso contrario
 */
bool existeAresta(NoVertice* origem, NoVertice* destino);

/**
 * @brief Adiciona uma aresta entre dois vertices
 * @param grafo Apontador para o grafo
 * @param origem Apontador para o vertice de origem
 * @param destino Apontador para o vertice de destino
 * @return true se a aresta foi adicionada, false caso contrario
 */
bool adicionarAresta(Grafo* grafo, NoVertice* origem, NoVertice* destino);

/**
 * @brief Carrega os dados das antenas de um ficheiro para um grafo
 * @param nomeFicheiro Nome do ficheiro a ser lido
 * @return Apontador para o grafo criado ou NULL em caso de erro
 */
Grafo* carregarDadosGrafo(const char* nomeFicheiro);

/**
 * @brief Busca em profundidade a partir de um vertice
 * @param grafo Apontador para o grafo
 * @param verticeInicial Apontador para o vertice inicial
 * @param visitados Array para marcar os vertices visitados
 * @param resultado Lista para armazenar os vertices visitados
 * @param tamanhoResultado Apontador para o tamanho do resultado
 * @return true se a busca foi realizada, false caso contrario
 */
bool buscaEmProfundidade(Grafo* grafo, NoVertice* verticeInicial, bool* visitados, NoVertice*** resultado, int* tamanhoResultado);

/**
 * @brief Busca em largura a partir de um vertice
 * @param grafo Apontador para o grafo
 * @param verticeInicial Apontador para o vertice inicial
 * @param visitados Array para marcar os vertices visitados
 * @param resultado Lista para armazenar os vertices visitados
 * @param tamanhoResultado Apontador para o tamanho do resultado
 * @return true se a busca foi realizada, false caso contrario
 */
bool buscaEmLargura(Grafo* grafo, NoVertice* verticeInicial, bool* visitados, NoVertice*** resultado, int* tamanhoResultado);

/**
 * @brief Inicializa uma fila vazia
 * @return Apontador para a fila criada
 */
Fila* inicializarFila();

/**
 * @brief Verifica se uma fila está vazia
 * @param fila Apontador para a fila
 * @return true se a fila está vazia, false caso contrario
 */
bool filaVazia(Fila* fila);

/**
 * @brief Adiciona um vertice ao final da fila
 * @param fila Apontador para a fila
 * @param valor Apontador para o vertice a ser adicionado
 * @return true se adicionado com sucesso, false caso contrario
 */
bool enfileirar(Fila* fila, NoVertice* valor);

/**
 * @brief Remove o vertice da frente da fila
 * @param fila Apontador para a fila
 * @return Apontador para o vertice removido ou NULL se vazia
 */
NoVertice* desenfileirar(Fila* fila);

/**
 * @brief Liberta a memoria alocada para uma fila
 * @param fila Apontador para a fila a ser libertada
 * @return true se liberada com sucesso, false caso contrario
 */
bool libertarFila(Fila* fila);

/**
 * @brief Encontra todos os caminhos entre dois vertices
 * @param grafo Apontador para o grafo
 * @param origem Apontador para o vertice de origem
 * @param destino Apontador para o vertice de destino
 * @param caminhos Apontador para a lista de caminhos
 * @return true se pelo menos um caminho foi encontrado, false caso contrario
 */
bool encontrarCaminhos(Grafo* grafo, NoVertice* origem, NoVertice* destino, ListaCaminho** caminhos);

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
bool encontrarCaminhosRecursivo(Grafo* grafo, NoVertice* atual, NoVertice* destino, bool* visitados, ElementoCaminho* caminho, ListaCaminho** caminhos);

/**
 * @brief Adiciona um vertice a um caminho
 * @param caminho Apontador para o caminho
 * @param vertice Apontador para o vertice a ser adicionado
 * @return Apontador para o novo caminho
 */
ElementoCaminho* adicionarVerticeCaminho(ElementoCaminho* caminho, NoVertice* vertice);

/**
 * @brief Copia um caminho
 * @param caminho Apontador para o caminho a ser copiado
 * @return Apontador para o novo caminho
 */
ElementoCaminho* copiarCaminho(ElementoCaminho* caminho);

/**
 * @brief Adiciona um caminho à lista de caminhos
 * @param caminhos Apontador para a lista de caminhos
 * @param caminho Apontador para o caminho a ser adicionado
 * @return true se adicionado com sucesso, false caso contrario
 */
bool adicionarCaminho(ListaCaminho** caminhos, ElementoCaminho* caminho);

/**
 * @brief Liberta a memoria alocada para um caminho
 * @param caminho Apontador para o caminho a ser libertado
 * @return true se liberado com sucesso, false caso contrario
 */
bool libertarCaminho(ElementoCaminho* caminho);

/**
 * @brief Liberta a memoria alocada para uma lista de caminhos
 * @param caminhos Apontador para a lista de caminhos a ser libertada
 * @return true se liberada com sucesso, false caso contrario
 */
bool libertarCaminhos(ListaCaminho* caminhos);

/**
 * @brief Encontra intersecoes entre antenas de frequencias diferentes
 * @param grafo Apontador para o grafo
 * @param frequenciaA Frequencia A
 * @param frequenciaB Frequencia B
 * @param intersecoes Array para armazenar as intersecoes
 * @param tamanhoIntersecoes Apontador para o tamanho do array de intersecoes
 * @return true se pelo menos uma intersecao foi encontrada, false caso contrario
 */
bool encontrarIntersecoes(Grafo* grafo, char frequenciaA, char frequenciaB, Intersecao* intersecoes, int* tamanhoIntersecoes, int maxIntersecoes);

/**
 * @brief Encontra o vertice com as coordenadas especificadas
 * @param grafo Apontador para o grafo
 * @param x Coordenada x
 * @param y Coordenada y
 * @return Apontador para o vertice ou NULL se nao encontrado
 */
NoVertice* encontrarVerticePorCoordenadas(Grafo* grafo, int x, int y);

/**
 * @brief Encontra todos os vertices com uma determinada frequencia
 * @param grafo Apontador para o grafo
 * @param frequencia Frequencia a ser procurada
 * @param vertices Array para armazenar os vertices encontrados
 * @param tamanhoVertices Apontador para o tamanho do array de vertices
 * @return true se pelo menos um vertice foi encontrado, false caso contrario
 */
bool encontrarVerticesPorFrequencia(Grafo* grafo, char frequencia, NoVertice** vertices, int* tamanhoVertices, int maxVertices);;

#endif // GRAFO_H