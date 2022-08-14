#include <stdio.h>

#include <stdlib.h>


struct noAVL {
 	 int dado;
	 int bal;
	 struct noAVL *esq;
	 struct noAVL *dir;
};

typedef struct noAVL* AVL;

//===========================================================================
AVL* cria_ArvAvl(){
    AVL* raiz = (AVL*) malloc(sizeof(AVL));
    if(raiz != NULL)
        *raiz = NULL;
    return raiz;
}

void libera_NO(struct noAVL* no){
    if(no == NULL)
        return;
    libera_NO(no->esq);
    libera_NO(no->dir);
    free(no);
    no = NULL;
}

void libera_ArvAVL(AVL* raiz){
    if(raiz == NULL)
        return;
    libera_NO(*raiz);//libera cada nó
    free(raiz);//libera a raiz
}

//===========================================================================
int altura_NO(struct noAVL* no){
    if(no == NULL)
        return -1;
    else
      return no->bal;
}

//===========================================================================
void RotacaoLR(AVL *A){//LR
    printf("\n Rotação Dupla a Direita (LR).\n");
    RotacaoSimplesAEsquerda(&(*A)->esq);
    RotacaoSimplesADireita(A);
}

void RotacaoRL(AVL *A){//RL
    printf("\n Rotação Dupla a Esquerda (RL).\n");
    RotacaoSimplesADireita(&(*A)->dir);
    RotacaoSimplesAEsquerda(A);
}

int maior(int x, int y){
    if(x > y)
        return x;
    else
        return y;
}

int fatorBalanceamento_NO(struct noAVL* no){
    return labs(altura_NO(no->esq) - altura_NO(no->dir));
}

/*=======================================================================*/

AVL procuraMenor(struct noAVL* atual){
    struct noAVL *no1 = atual;
    struct noAVL *no2 = atual->esq;
    while(no2 != NULL){
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

/*=======================================================================*/
int remove_ArvAVL(AVL *raiz, int valor){
	if(*raiz == NULL){// valor não existe
	    printf("valor não existe!!\n");
	    return 0;
	}

    int res;
	if(valor < (*raiz)->dado){
	    if((res = remove_ArvAVL(&(*raiz)->esq,valor)) == 1){
            if(fatorBalanceamento_NO(*raiz) >= 2){
                if(altura_NO((*raiz)->dir->esq) <= altura_NO((*raiz)->dir->dir))
                    RotacaoSimplesAEsquerda(raiz);
                else
                    RotacaoRL(raiz); //RotacaoDuplaEsquerda
            }
	    }
	}

	if((*raiz)->dado < valor){
	    if((res = remove_ArvAVL(&(*raiz)->dir, valor)) == 1){
	        if(fatorBalanceamento_NO(*raiz) >= 2){
                if(altura_NO((*raiz)->esq->dir) <= altura_NO((*raiz)->esq->esq) )
                    RotacaoSimplesADireita(raiz);
                else
                    RotacaoLR(raiz); //RotacaoDuplaDireita
            }
	    }
	}

	if((*raiz)->dado == valor){
	    if(((*raiz)->esq == NULL || (*raiz)->dir == NULL)){// nó tem 1 filho ou nenhum
			struct noAVL* oldNode = (*raiz);
			if((*raiz)->esq != NULL)
                *raiz = (*raiz)->esq;
            else if((*raiz)->dir != NULL)
                    *raiz = (*raiz)->dir;
                 else *raiz = NULL;
			free(oldNode);
		}else { // nó tem 2 filhos
			struct noAVL* temp = procuraMenor((*raiz)->dir);
			(*raiz)->dado = temp->dado;
			remove_ArvAVL(&(*raiz)->dir, (*raiz)->dado);
            if(fatorBalanceamento_NO(*raiz) >= 2){
				if(altura_NO((*raiz)->esq->dir) <= altura_NO((*raiz)->esq->esq))
					RotacaoSimplesADireita(raiz);
				else
					RotacaoLR(raiz);  //RotacaoDuplaDireita
			}
		}
		if (*raiz != NULL)
            (*raiz)->bal = maior(altura_NO((*raiz)->esq),altura_NO((*raiz)->dir)) + 1;
		return 1;
	}

	(*raiz)->bal = maior(altura_NO((*raiz)->esq),altura_NO((*raiz)->dir)) + 1;

	return res;
}

/*======================================================================*/
void RotacaoSimplesADireita(AVL *raiz)
{
	printf("\n Rotação Simples a Direita(LL).\n");
	struct noAVL *ne = (*raiz)->esq;
	(*raiz)->esq = ne->dir;
	ne->dir = (*raiz);
	(*raiz)->bal = maior(altura_NO((*raiz)->esq),altura_NO((*raiz)->dir)) + 1;
    ne->bal = maior(altura_NO(ne->esq),(*raiz)->bal) + 1;
	*raiz = ne;
}

void RotacaoSimplesAEsquerda(AVL *raiz)
{
    printf("\n Rotação Simples a Esquerda(RR).\n");
	struct noAVL *nd = (*raiz)->dir;
	(*raiz)->dir = nd->esq;
	nd->esq = (*raiz);
	(*raiz)->bal = maior(altura_NO((*raiz)->esq),altura_NO((*raiz)->dir)) + 1;
    nd->bal = maior(altura_NO(nd->dir),(*raiz)->bal) + 1;
	(*raiz) = nd;
}

/*===============================================================================*/
int Inserir(int val, AVL *raiz)
{
       if (*raiz == NULL){
	   struct noAVL *novo;
	   novo = (AVL) malloc(sizeof(struct noAVL));
	   novo->dado = val;
	   novo->bal = 0;
	   novo->esq = NULL;
	   novo->dir = NULL;
	   *raiz=novo;
	   return 0;
	}
         struct noAVL *atual = *raiz;
         if (val < atual->dado){
            if (Inserir(val, &(atual->esq))==0){
                if(fatorBalanceamento_NO(atual) >= 2){
                    if(val < (*raiz)->esq->dado){
                        RotacaoSimplesADireita(raiz);
                    }else{
                        RotacaoLR(raiz);
                    }
                }
             } }

        else if (val > atual->dado){
               if (Inserir(val, &(atual->dir))==0){
                  if(fatorBalanceamento_NO(atual) >= 2){
                    if(val > (*raiz)->dir->dado){
                        RotacaoSimplesAEsquerda(raiz);
                    }else{
                       RotacaoRL(raiz); }
                } //Se do fatordebalanceamento
               }
             }
     atual->bal = maior(altura_NO(atual->esq),altura_NO(atual->dir)) + 1;
     return 0;
}


/*========================================================================*/
void mostraPre(AVL *raiz)
{
  if(raiz==NULL)
  	  return;
  if (*raiz != NULL){
  (*raiz)->bal = fatorBalanceamento_NO(*raiz);
  printf("valor do no: %d ( Bal: %d )\n",(*raiz)->dado,(*raiz)->bal);
  printf("End Esq:%p - End. no:%p - End dir:%p\n\n", (*raiz)->esq,(*raiz),(*raiz)->dir);
  mostraPre(&((*raiz)->esq));
  mostraPre(&((*raiz)->dir));
  }
}

/*========================================================================*/
void mostraIn(AVL *raiz)
{
  if(raiz==NULL)
  	  return;
  if (*raiz != NULL){
  (*raiz)->bal = fatorBalanceamento_NO(*raiz);
  mostraIn(&((*raiz)->esq));
  printf("valor do no: %d ( Bal: %d )\n",(*raiz)->dado,(*raiz)->bal);
  printf("End Esq:%p - End. no:%p - End dir:%p\n\n", (*raiz)->esq,(*raiz),(*raiz)->dir);
  mostraIn(&((*raiz)->dir));
  }
}

/*========================================================================*/
void mostraPos(AVL *raiz)
{
  if(raiz==NULL)
  	  return;
  if (*raiz != NULL){
  (*raiz)->bal = fatorBalanceamento_NO(*raiz);
  mostraPos(&((*raiz)->esq));
  mostraPos(&((*raiz)->dir));
  printf("valor do no: %d ( Bal: %d )\n",(*raiz)->dado,(*raiz)->bal);
  printf("End Esq:%p - End. no:%p - End dir:%p\n\n", (*raiz)->esq,(*raiz),(*raiz)->dir);
  }
}

/*========================================================================*/

void mostrarMenor(AVL *raiz){

    if((*raiz)->esq != NULL){
        mostrarMenor(&((*raiz)->esq));
    } else{
        printf("menor numero da arvore: %d\n", (*raiz)->dado);
    }

}

/*========================================================================*/

void mostrarNosDeUmNivel(AVL *raiz, int nivel){

    if(0 < nivel){
        if((*raiz)->esq != NULL){
            mostrarNosDeUmNivel(&((*raiz)->esq), nivel-1);
        }
        if((*raiz)->dir != NULL){
            mostrarNosDeUmNivel(&((*raiz)->dir), nivel-1);
        }
    } else if((*raiz) != NULL){
        printf("\n%d\n", (*raiz)->dado);
    }

}

/*========================================================================*/

void somaNosNiveisImpares(AVL *raiz, int *soma, int nv){

    if(*raiz == NULL){
        return 0;
    } else if(nv % 2 == 0 && (*raiz)->esq != NULL){
        //printf("recebeu: %d\n", (*raiz)->esq->dado); mostrar os valores recebidos
        *soma += (*raiz)->esq->dado;
    }
    if((*raiz)->dir != NULL && nv % 2 == 0){
        //printf("recebeu: %d\n", (*raiz)->dir->dado); mostrar os valores recebidos
        *soma += (*raiz)->dir->dado;
    }

    if((*raiz)->esq != NULL){
        somaNosNiveisImpares(&((*raiz)->esq), soma, nv+1);
    }

    if((*raiz)->dir != NULL){
    somaNosNiveisImpares(&((*raiz)->dir), soma, nv+1);
    }

}

int main()
{
	AVL* raiz1;
	raiz1 = cria_ArvAvl();
	int nivel, opcao, valor=0;
	int *x;
	x=0;

	do{
	   printf("\nDigite um numero para insercao na arvore ou 0 para parar: ");
	   scanf("%d",&valor);
	   if(valor!=0)
	      Inserir(valor,raiz1);
	}while(valor!=0);


	do{
        printf("\n1. mostrar o menor numero na arvore, 2. mostrar todos os nos de um nivel escolhido, 3. impressao da arvore dos 3 modos, 4. soma dos nos presentes nos niveis impares, 0. encerrar\n");
        scanf("%d", &opcao);
        switch(opcao){
        case 1:
           mostrarMenor(raiz1);
           break;
        case 2:
            printf("Digite o nivel que deseja verificar: ");
            scanf("%d", &valor);
            mostrarNosDeUmNivel(raiz1, valor);
            break;
        case 3:
            printf("1. pre-ordem, 2. in-ordem, 3. pos-ordem: \n");
            scanf("%d", &valor);
            switch(valor){
            case 1:
                mostraPre(raiz1);
                break;
            case 2:
                mostraIn(raiz1);
                break;
            case 3:
                mostraPos(raiz1);
                break;
            default:
                printf("opcao invalida");
            }
            break;
            case 4:
                somaNosNiveisImpares(raiz1, &x, 0);
                printf("\nsoma: %d\n", x);
                x = 0;
            break;
        }

	}while(opcao != 0);





	return 0;
}
