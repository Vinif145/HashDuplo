#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hash.h"
#define SEED    0x12345678

typedef struct _municipios{
   float x;
   float y;
   char nome[300];
   char codigo[8];
   int capital;
   int codigo_uf;
   int siafi_id;
   int ddd;
   char fuso[200];

}cidade;


void * aloca_reg(float x, float y, char *nome, char *codigo, int capital, int codigo_uf, int siafi_id, int ddd, char *fuso){
    cidade * pitem;
    pitem = malloc(sizeof(cidade));
    pitem->x = x;
    pitem->y = y;
    strcpy(pitem->nome, nome);
    //pitem->codigo = codigo;
    strcpy(pitem->codigo, codigo);
    pitem->capital = capital;
    pitem->codigo_uf = codigo_uf;
    pitem->siafi_id = siafi_id;
    pitem->ddd = ddd;
    strcpy(pitem->fuso, fuso);
    return pitem;
}

char * get_key(void * reg){
    return (*(cidade *)reg).codigo;
    
}


void test_hash(){
    thash h;
    int nbuckets = 11149;
    hash_constroi(&h,nbuckets,get_key);

    cidade * found;

    assert(hash_insere(&h,aloca_reg(-16.7573,-49.4412,"Abadia de Goiais","5200050",0,52,1050,62,"America/Sao_Paulo"))==EXIT_SUCCESS);

    found = hash_busca(h,"5200050");
   // printf("%f -- %f ", found->x, found->y);

    hash_remove(&h, "5200050");
    assert(hash_busca(h, "5200050") == NULL);

    hash_apaga(&h);

}

void menu(){

    printf("\nOpcao 1 -> Buscar por um municipio \n");
    printf("Opcao 2 -> Remover um municipio \n");
}



int main(int argc, char* argv[]){

    test_hash();

    thash h;
    int nbuckets = 11148; //nbuckets+1 11.1149 (total * 2) -> Primo mais próximo
    hash_constroi(&h,nbuckets,get_key);
    
    
    FILE *arq1;

           arq1 = fopen("cidades.txt", "r");

           if(arq1 == NULL){

                printf("Erro na abertura do arquivo\n");

           }else{      
                       char lixo[200]; //Aqui não
                       int numero_lixo;
                       float x = 0;
                       float y = 0;
                       char nome[300];
                       int codigoNum;
                       char codigo[8];
                       int capital;
                       int codigo_uf;
                       int siafi_id;
                       int ddd;
                       char fuso[200];

              fscanf(arq1, " %[^\n]", lixo  );

             //printf("%s \n", lixo);


                while(feof (arq1) == 0){

                    fscanf(arq1, "%d[^,]", &codigoNum );

                    sprintf(codigo, "%d", codigoNum); //Mantendo a mesma hashf

                    fscanf(arq1, " %[^0-9]", nome  );

                    fscanf(arq1, " %f[^,]", &x );

                    fscanf(arq1, " ,%f[^,]", &y );

                    fscanf(arq1, " ,%d[^,]", &capital );

                    fscanf(arq1, " ,%d[^,]", &codigo_uf );

                    fscanf(arq1, " ,%d[^,]", &siafi_id );

                    fscanf(arq1, " ,%d[^,]", &ddd );

                    fscanf(arq1, " %[^\n]", fuso );

                    assert(hash_insere(&h,aloca_reg(x, y, nome, codigo, capital, codigo_uf, siafi_id, ddd, fuso ))==EXIT_SUCCESS);



                    //printf("%s --%s-- numeero: %f (%f) %d %d %d %d --%s-- \n", codigo, nome, x, y,capital, codigo_uf, siafi_id, ddd, fuso);

                    

               }

        }

        //Menu para buscar, remover um Item. Inserção já é feita na leitura; 
         
        int op;
        menu();
        scanf(" %d", &op);

        do {

            

            if (op == 1 ){

                cidade * found;

                char chaveIBGE[8];

                printf("Digite o codigo do IBGE: ");
                scanf(" %s", chaveIBGE );
                
                found = hash_busca(h, chaveIBGE);

                if(found != NULL){
                    
                    printf("%s --%s-- numeero: %f (%f) %d %d %d %d --%s-- \n", found->codigo, found->nome, found->x, found->y, 
                        found->capital, found->codigo_uf, found->siafi_id, found->ddd, found->fuso);

                }else {
                  
                  printf("Nao foi possivel localizar esta cidade \n");
                }


            }else if (op == 2){

                char chaveIBGE[8];

                printf("Digite o codigo do IBGE: ");
                scanf(" %s", chaveIBGE );

                cidade * remove = hash_busca(h, chaveIBGE);
                
                if(hash_remove(&h, chaveIBGE) == EXIT_SUCCESS){

                    printf(" %s removida com sucesso \n", remove->nome);


                }else {

                    printf("Nao foi possivel remover a cidade com chave: %s \n", chaveIBGE);
                }

            }else{

                printf("Opcao nao reconhecida pelo Menu\n");
            }

            printf("\n");
            menu();
            printf("Digite outra opcao ou saia do menu -> 0\n");
            scanf(" %d", &op);
        
        }while(op != 0);

        hash_apaga(&h);
}

