#pragma once
/**
 * @file struct.h
 * @author Matheus Delgado (a31542@alunos.ipca.pt)
 * @brief Definicao das estruturas de dados para o grafo
 * @version 0.3
 * @date 2025-05-17
 * @copyright Copyright (c) 2025
 */
#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

 /**
  * @brief Estrutura para representar as coordenadas de uma antena
  */
typedef struct {
    int x;  // coordenada x na matriz
    int y;  // coordenada y na matriz
} Coordenada;

/**
 * @brief Estrutura para representar uma antena
 */
typedef struct {
    char frequencia;    
    Coordenada posicao; 
} Antena;

/**
 * @brief Estrutura para representar um no de vertice no grafo
 */
typedef struct NoVertice {
    Antena dados;           
    struct Aresta* primeiraAresta; // lista de arestas
    struct NoVertice* proximo; 
} NoVertice;

/**
 * @brief Estrutura para representar uma aresta no grafo
 */
typedef struct Aresta {
    NoVertice* destino;     
    struct Aresta* proxima; // proxima aresta na lista
} Aresta;

/**
 * @brief Estrutura para representar um grafo
 */
typedef struct Grafo{
    NoVertice* primeiro;    // primeiro vertice da lista
    int numVertices;        // numero de vertices
} Grafo;

/**
 * @brief Estrutura para representar um elemento da fila para busca em largura
 */
typedef struct ElementoFila {
    NoVertice* valor;      
    struct ElementoFila* proximo; // proximo elemento na fila
} ElementoFila;

/**
 * @brief Estrutura para representar uma fila
 */
typedef struct Fila{
    ElementoFila* frente;   // elemento na frente da fila
    ElementoFila* tras;     // elemento no final da fila
} Fila;

/**
 * @brief Estrutura para armazenar um vertice em um caminho
 */
typedef struct ElementoCaminho {
    NoVertice* vertice;     
    struct ElementoCaminho* proximo; // proximo vertice no caminho
} ElementoCaminho;

/**
 * @brief Estrutura para armazenar uma lista de caminhos
 */
typedef struct ListaCaminho {
    ElementoCaminho* caminho; // um caminho completo
    struct ListaCaminho* proximo; // proximo caminho na lista
} ListaCaminho;

/**
 * @brief Estrutura para representar uma intersecao entre antenas
 */
typedef struct Intersecao{
    Coordenada antenaA;     // coordenadas da antena com frequencia A
    Coordenada antenaB;     // coordenadas da antena com frequencia B
} Intersecao;

#endif // ESTRUTURAS_H