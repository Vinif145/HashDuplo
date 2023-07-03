#ifndef __HASH__
#define __HASH__

#define SEED    0x12345678

typedef struct {
     uintptr_t * table;
     int size;
     int max;
     uintptr_t deleted;
     char * (*get_key)(void *);
}thash;


uint32_t hashf(const char* str, uint32_t h);

int hash_constroi(thash * h,int nbuckets, char * (*get_key)(void *) );

int hash_insere(thash * h, void * bucket);

void * hash_busca(thash h, const char * key);

int hash_remove(thash * h, const char * key);

void hash_apaga(thash *h);


#endif