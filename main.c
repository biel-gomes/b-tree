#include <stdlib.h>
#include <stdio.h>
#define M 2
#define MM (M * 2)
#define FALSE 0
#define TRUE 1

typedef long  Chave;
typedef struct Registro
{
     Chave chave;
    /*outros componentes*/
} Registro;
typedef struct Pagina *PtrPagina;
typedef struct Pagina
{
    short n;
    Registro r[MM];
    PtrPagina p[MM + 1];
} Pagina;

void CriaArvore(PtrPagina *raiz)
{
    *raiz = NULL;
}

void Busca(Registro *x, PtrPagina ap)
{
    long i = 1;
    if (ap == NULL)
    {
        printf("Registro nao esta presente na arvore\n");
        return;
    }
    while (i < ap->n && x-> chave > ap->r[i - 1].chave)
        i++;
    if (x-> chave == ap->r[i - 1].chave)
    {
        *x = ap->r[i - 1];
        return;
    }
    if (x-> chave < ap->r[i - 1].chave)
        Busca(x, ap->p[i - 1]);
    else
        Busca(x, ap->p[i]);
}

void InsereNaPagina(PtrPagina ap, Registro reg, PtrPagina apDir)
{
    short naoAchouPosicao;
    int k;
    k = ap->n;
    naoAchouPosicao = (k > 0);
    while (naoAchouPosicao)
    {
        if (reg.chave >= ap->r[k - 1].chave)
        {
            naoAchouPosicao = FALSE;
            break;
        }
        ap->r[k] = ap->r[k - 1];
        ap->p[k + 1] = ap->p[k];
        k--;
        if (k < 1)
            naoAchouPosicao = FALSE;
    }
    ap->r[k] = reg;
    ap->p[k + 1] = apDir;
    ap->n++;
}

void Ins(Registro reg, PtrPagina ap, short *cresceu, Registro *regRetorno, PtrPagina *apRetorno)
{
    long i = 1; // Posição provavel do vetor em que novo indice sera inserido
    long j;
    PtrPagina apTemp;

    if (ap == NULL) // Critério de parada da recursão. Pode ser usado para arvore vazia, arvore com espaço em folhas ou arvore que explode
    {
        *cresceu = TRUE;
        (*regRetorno) = reg;
        (*apRetorno) = NULL;
        return;
    }

    while (i < ap->n && reg.chave > ap->r[i - 1].chave)
        i++; // Percorrendo vetores dos nós para encontrar valor maior (ou igual) que o novo indice

    if (reg.chave == ap->r[i - 1].chave)
    {
        printf(" Erro: Registro ja esta presente\n");
        *cresceu = FALSE;
        return;
    }

    if (reg.chave < ap->r[i - 1].chave)
        i--; // Como o valor nao é igual, só pode ser maior, entao volta-se uma posição

    Ins(reg, ap->p[i], cresceu, regRetorno, apRetorno); // Chama-se recursivo para descer na arvore

    if (!*cresceu)
        return;

    if (ap->n < MM) /* Pagina tem espaco */
    {
        InsereNaPagina(ap, *regRetorno, *apRetorno); // Chama insere na página pq tem esoaço
        *cresceu = FALSE;
        return;
    }

    /* Overflow: Pagina tem que ser dividida */         // Página nao tem espaço suficiente
    apTemp = (PtrPagina)malloc(sizeof(Pagina)); // Cria nova pagina pra ser raiz da subarvore que vem do split
    apTemp->n = 0;
    apTemp->p[0] = NULL; // Ela começa com zero indices e primeiro filho pra NULL
    if (i < M + 1)
    {
        InsereNaPagina(apTemp, ap->r[MM - 1], ap->p[MM]);
        ap->n--;
        InsereNaPagina(ap, *regRetorno, *apRetorno);
    }
    else
        InsereNaPagina(apTemp, *regRetorno, *apRetorno);
    for (j = M + 2; j <= MM; j++)
        InsereNaPagina(apTemp, ap->r[j - 1], ap->p[j]);
    ap->n = M;
    apTemp->p[0] = ap->p[M + 1];
    *regRetorno = ap->r[M];
    *apRetorno = apTemp;
}

void Insere(Registro reg, PtrPagina *ap)
{
    short cresceu;
    Registro regRetorno;
    Pagina *apRetorno, *apTemp;
    Ins(reg, *ap, &cresceu, &regRetorno, &apRetorno); // Chamando funcao insere auxiliar
    if (cresceu)                                      /* Arvore cresce na altura pela raiz */
    {
        apTemp = (Pagina *)malloc(sizeof(Pagina)); // Criando nova raiz
        apTemp->n = 1;                                     // Definindo que a nova raiz só terá um índice
        apTemp->r[0] = regRetorno;                         // Definindo valor do unico indice da raiz
        apTemp->p[1] = apRetorno;                          // Definindo filho da direita da raiz, resultado do split que causou explosao no tamanho
        apTemp->p[0] = *ap;                                // Definindo filho da esquerda da raiz, resultado do split que causou explosao no tamanho
        *ap = apTemp;                                      // Enderençando nova raiz
    }
}

void Reconstitui(PtrPagina apPag, PtrPagina apPai, int posPai, short *diminuiu)
{
    Pagina *aux;
    long dispAux, j;
    if (posPai < apPai->n) /* aux = Pagina a direita de apPag */
    {
        aux = apPai->p[posPai + 1];
        dispAux = (aux->n - M + 1) / 2;
        apPag->r[apPag->n] = apPai->r[posPai];
        apPag->p[apPag->n + 1] = aux->p[0];
        apPag->n++;
        if (dispAux > 0) /* Existe folga: transfere de aux para apPag */
        {
            for (j = 1; j < dispAux; j++)
                InsereNaPagina(apPag, aux->r[j - 1], aux->p[j]);
            apPai->r[posPai] = aux->r[dispAux - 1];
            aux->n -= dispAux;
            for (j = 0; j < aux->n; j++)
                aux->r[j] = aux->r[j + dispAux];
            for (j = 0; j <= aux->n; j++)
                aux->p[j] = aux->p[j + dispAux];
            *diminuiu = FALSE;
        }
        else /* Fusao: intercala aux em apPag e libera aux */
        {
            for (j = 1; j <= M; j++)
                InsereNaPagina(apPag, aux->r[j - 1], aux->p[j]);
            free(aux);
            for (j = posPai + 1; j < apPai->n; j++)
            {
                apPai->r[j - 1] = apPai->r[j];
                apPai->p[j] = apPai->p[j + 1];
            }
            apPai->n--;
            if (apPai->n >= M)
                *diminuiu = FALSE;
        }
    }
    else /* aux = Pagina a esquerda de apPag */
    {
        aux = apPai->p[posPai - 1];
        dispAux = (aux->n - M + 1) / 2;
        for (j = apPag->n; j >= 1; j--)
            apPag->r[j] = apPag->r[j - 1];
        apPag->r[0] = apPai->r[posPai - 1];
        for (j = apPag->n; j >= 0; j--)
            apPag->p[j + 1] = apPag->p[j];
        apPag->n++;
        if (dispAux > 0) /* Existe folga: transf. de aux para apPag */
        {
            for (j = 1; j < dispAux; j++)
                InsereNaPagina(apPag, aux->r[aux->n - j],
                               aux->p[aux->n - j + 1]);
            apPag->p[0] = aux->p[aux->n - dispAux + 1];
            apPai->r[posPai - 1] = aux->r[aux->n - dispAux];
            aux->n -= dispAux;
            *diminuiu = FALSE;
        }
        else /* Fusao: intercala apPag em aux e libera apPag */
        {
            for (j = 1; j <= M; j++)
                InsereNaPagina(aux, apPag->r[j - 1], apPag->p[j]);
            free(apPag);
            apPai->n--;
            if (apPai->n >= M)
                *diminuiu = FALSE;
        }
    }
}

void Antecessor(PtrPagina ap, int Ind, PtrPagina apPai, short *diminuiu)
{
    if (apPai->p[apPai->n] != NULL)
    {
        Antecessor(ap, Ind, apPai->p[apPai->n], diminuiu);
        if (*diminuiu)
            Reconstitui(apPai->p[apPai->n], apPai, (long)apPai->n, diminuiu);
        return;
    }
    ap->r[Ind - 1] = apPai->r[apPai->n - 1];
    apPai->n--;
    *diminuiu = (apPai->n < M);
}

void Ret(Chave ch, PtrPagina *ap, short *diminuiu)
{
    long j, Ind = 1;
    PtrPagina pag;
    if (*ap == NULL)
    {
        printf("Erro: registro nao esta na arvore\n");
        *diminuiu = FALSE;
        return;
    }
    pag = *ap;
    while (Ind < pag->n && ch > pag->r[Ind - 1].chave)
        Ind++;
    if (ch == pag->r[Ind - 1].chave)
    {
        if (pag->p[Ind - 1] == NULL) /* Pagina folha */
        {
            pag->n--;
            *diminuiu = (pag->n < M);       // Detecta underflow
            for (j = Ind; j <= pag->n; j++) // Deslocando vetor
            {
                pag->r[j - 1] = pag->r[j];
                pag->p[j] = pag->p[j + 1];
            }
            return;
        }
        /* Pagina nao e folha: trocar com antecessor */
        Antecessor(*ap, Ind, pag->p[Ind - 1], diminuiu);
        if (*diminuiu)
            Reconstitui(pag->p[Ind - 1], *ap, Ind - 1, diminuiu);
        return;
    }
    if (ch > pag->r[Ind - 1].chave)
        Ind++;
    Ret(ch, &pag->p[Ind - 1], diminuiu);
    if (*diminuiu)
        Reconstitui(pag->p[Ind - 1], *ap, Ind - 1, diminuiu);
}

void Retira(Chave ch, PtrPagina *ap)
{
    short diminuiu;
    PtrPagina aux;
    Ret(ch, ap, &diminuiu);
    if (diminuiu && (*ap)->n == 0) /* Arvore diminui na altura */
    {
        aux = *ap;
        *ap = aux->p[0]; // Elimina antiga raiz
        free(aux);
    }
}

void ImprimeI(PtrPagina p, int nivel)
{
    long i;
    if (p == NULL)
        return;
    printf("Nivel %d : ", nivel);
    for (i = 0; i < p->n; i++)
        printf("%ld ", (long)p->r[i].chave);
    putchar('\n');
    nivel++;
    for (i = 0; i <= p->n; i++)
        ImprimeI(p->p[i], nivel);
}

void Imprime(PtrPagina p)
{
    int n = 0;
    ImprimeI(p, n);
}

void TestaI(PtrPagina p, int pai, short direita)
{
    int i;
    int antecessor = 0;
    if (p == NULL)
        return;
    if (p->r[0].chave > pai && direita == FALSE)
    {
        printf("Erro: filho %12ld maior que pai %d\n", p->r[0].chave, pai);
        return;
    }
    for (i = 0; i < p->n; i++)
    {
        if (p->r[i].chave <= antecessor)
        {
            printf("Erro: irmao %ld maior que irmao a esquerda %d\n",
                   (long)p->r[i].chave, antecessor);
            return;
        }
        antecessor = p->r[i].chave;
    }
    for (i = 0; i < p->n; i++)
        TestaI(p->p[i], p->r[i].chave, FALSE);
    TestaI(p->p[p->n], p->r[i].chave, TRUE);
}

void Testa(PtrPagina p)
{
    int i;
    if (p == NULL)
        return;
    for (i = 0; i < p->n; i++)
        TestaI(p->p[i], p->r[i].chave, FALSE);
    TestaI(p->p[p->n], p->r[i].chave, TRUE);
}

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