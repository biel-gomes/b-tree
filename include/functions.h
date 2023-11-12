#ifndef FUNCOES_H
#define FUNCOES_H

#define M 2
#define MM (M * 2)
#define FALSE 0
#define TRUE 1

typedef long Chave;
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

void CriaArvore(PtrPagina *raiz);
void Busca(Registro *x, PtrPagina ap);
void InsereNaPagina(PtrPagina ap, Registro reg, PtrPagina apDir);
void Ins(Registro reg, PtrPagina ap, short *cresceu, Registro *regRetorno, PtrPagina *apRetorno);
void Insere(Registro reg, PtrPagina *ap);
void Reconstitui(PtrPagina apPag, PtrPagina apPai, int posPai, short *diminuiu);
void Antecessor(PtrPagina ap, int ind, PtrPagina apPai, short *diminuiu);
void Ret(Chave ch, PtrPagina *ap, short *diminuiu);
void Retira(Chave ch, PtrPagina *ap);
void ImprimeI(PtrPagina p, int nivel);
void Imprime(PtrPagina p);
void TestaI(PtrPagina p, int pai, short direita);
void Testa(PtrPagina p);

#endif