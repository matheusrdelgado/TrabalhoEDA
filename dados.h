/**
 * @file dados.h
 * @author Matheus Delgado (a31542 IPCA)
 * @brief Definicao das estruturas de antenas e dos efeitos nefastos
 * @version 0.1
 * @date 2025-03-25
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>    
#include <string.h>

typedef struct Antena {
    int linha;
    int coluna;
    char frequencia;
    int id;
    struct Antena* next;
}Antena;

typedef struct Nefasto {
    int linha;
    int coluna;
    int idAntena1;
    int idAntena2;
    struct Nefasto* next;
} Nefasto;
