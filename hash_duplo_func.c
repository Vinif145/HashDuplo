#include <stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hash.h"


uint32_t hashf(const char* str, uint32_t h){
    /* One-byte-at-a-time Murmur hash 
    Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp */
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}


int hash_constroi(thash * h,int nbuckets, char * (*get_key)(void *) ){
    h->table = calloc(sizeof(uintptr_t),nbuckets +1);
    if (h->table == NULL){
        return EXIT_FAILURE;
    }
    h->max = nbuckets +1;
    h->size = 0;
    h->deleted = (uintptr_t) & (h->size);
    h->get_key = get_key;
    return EXIT_SUCCESS;
}

int hash_insere(thash * h, void * bucket){
    uint32_t hash = hashf(h->get_key(bucket), SEED);
    int pos = hash % (h->max); //h
    int k = 0;

    if (h->max == (h->size+1)){
        free(bucket);
        return EXIT_FAILURE;
    } else {
        while (h->table[pos] != 0) { //Está preenchida 
            if (h->table[pos] == h->deleted) //É diferente de 0 mas está livre (removida)
                break;

            k = (hash % (h->max - 1)) + 1;  // hash duplo -> h2
            pos = (pos + k) % h->max;  //Normalizando no espaço da nossa Hash
           

        }
        h->table[pos] = (uintptr_t) bucket;
        h->size += 1;
    }
    return EXIT_SUCCESS;
}


void* hash_busca(thash h, const char* key) {
    int pos = hashf(key, SEED) % h.max;
    int k = 0;

    while (h.table[pos] != 0) {
        if (strcmp(h.get_key((void*)h.table[pos]), key) == 0)
            return (void*)h.table[pos];
        else {
            
            k = (hashf(key, SEED) % (h.max - 1)) + 1;
            pos = (pos + k) % h.max;
        }
    }

    return NULL;
}


int hash_remove(thash* h, const char* key) {
    int pos = hashf(key, SEED) % h->max;
    int k = 0;

    while (h->table[pos] != 0) {
        if (strcmp(h->get_key((void*)h->table[pos]), key) == 0) {
            free((void*)h->table[pos]);
            h->table[pos] = h->deleted;
            h->size -= 1;
            return EXIT_SUCCESS;
        } else {
           
            k = (hashf(key, SEED) % (h->max - 1)) + 1;
            pos = (pos + k) % h->max;
        }
    }

    return EXIT_FAILURE;
}

void hash_apaga(thash *h){
    int pos;
    for(pos =0;pos< h->max;pos++){
        if (h->table[pos] != 0){
            if (h->table[pos]!=h->deleted){ //!= quer dizer que não foi apagada
                free((void *)h->table[pos]);
            }
        }
    }
    free(h->table);
}