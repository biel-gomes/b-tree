#include <stdlib.h>
#include <stdio.h>

#include "functions.h"

int main()
{
    printf("############################################################\n");
    printf("Bem vindo ao terceiro tp de AEDS!\n");
    printf("Digite as  chaves de entrada, ou 0 para comecar a remocao\n");
    printf("############################################################\n");
    Registro x;
    Pagina *D;
    CriaArvore(&D);
    printf(" chave: ");
    scanf("%ld%*[^\n]", &x.chave);
    getchar();
    while (x.chave != 0)
    {
        Insere(x, &D);
        Imprime(D);
        printf("\n");
        printf(" chave: ");
        scanf("%ld%*[^\n]", &x.chave);
        getchar();
    }
    Testa(D);
    printf("############################################################\n");
    printf("Digite as  chaves para a remocao, ou 0 para sair do programa\n");
    printf("############################################################\n");
    printf(" chave: ");
    scanf("%ld%*[^\n]", &x.chave);
    getchar();
    while (x.chave != 0)
    {
        Retira(x.chave, &D);
        Imprime(D);
        printf(" chave: ");
        scanf("%ld%*[^\n]", &x.chave);
        getchar();
    }
    Testa(D);
    return 0;
}