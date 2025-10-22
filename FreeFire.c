#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

// Tamanho máximo das strings e capacidade da mochila
#define MAX_STR_LEN 30
#define TAM_MOCHILA 10

// Estrutura de um nó da lista encadeada
typedef struct No {
    char* nome;          // Nome do item
    char* tipo;          // Tipo do item
    int* quantidade;     // Quantidade do item
    int* prioridade;     // Prioridade do item
    struct No* proximo;  // Ponteiro para o próximo nó da lista
} No;

// Definição de Mochila como ponteiro para Nó
typedef No* Mochila;

// Declaração das funções utilizadas
void limparBufferEntrada();
void inicializarMochila(Mochila *lista);
void inserirItem(Mochila *lista, int *qtd_item);
void removerItem(Mochila *lista, int *qtd_item);
void buscarItem(Mochila *lista);
void listarMochila(const Mochila lista);
void menuOrdenacao(Mochila *lista, int *ordenacaopornome);
void OrdenacaoNome(Mochila *lista, int *qtd_comparacoes);
void OrdenacaoPrioridade(Mochila *lista, int *qtd_comparacoes);
void OrdenacaoTipo(Mochila *lista, int *qtd_comparacoes);
void TrocaDados(No* a, No* b);
void liberarMochila(Mochila *lista);

int main() {
    setlocale(LC_ALL, "Portuguese");
    int opcao;
    int qtd_items = 0;
    Mochila lista;
    inicializarMochila(&lista); // Inicializa a mochila (lista vazia)
    int ordenacaopornome = 0;

    do {
        // Menu principal
        printf("========================================\n");
        printf("  PLANO DE FUGA - CÓDIGO DA ILHA  \n");
        printf("========================================\n");
        printf("Itens na Mochila: %d/%d\n", qtd_items, TAM_MOCHILA);
        printf("Status da Ordenação por Nome: ");
        printf(ordenacaopornome == 1 ? "ATIVADA\n" : "DESATIVADA\n");

        printf("\n1. Adicionar Item (Loot)\n");
        printf("2. Remover Item \n");
        printf("3. Listar Itens na Mochila \n");
        printf("4. Ordenar lista\n");
        printf("5. Buscar item (obs: ORDENAÇÃO POR NOME NECESSÁRIA)\n");
        printf("0. Sair\n");
        printf("----------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        // Escolha do usuário
        switch(opcao) {
            case 1:
                inserirItem(&lista, &qtd_items);
                break;
            case 2:
                removerItem(&lista, &qtd_items);
                break;
            case 3:
                listarMochila(lista);
                break;
            case 4:
                if (qtd_items == 0){
                    printf("A mochila está vazia!");
                    break;
                }
                menuOrdenacao(&lista, &ordenacaopornome);
                break;
            case 5:
                if (ordenacaopornome == 1){
                    buscarItem(&lista);
                } else {
                    printf("Ordenação por nome necessária!\n");
                    getchar();
                }
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    // Libera memória antes de encerrar
    liberarMochila(&lista);
    return 0;
}

/*---------------------------------------------
 Função: limparBufferEntrada()
 Remove caracteres extras deixados no buffer
 após uso de scanf().
---------------------------------------------*/
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*---------------------------------------------
 Função: inicializarMochila()
 Define a mochila como vazia (ponteiro nulo).
---------------------------------------------*/
void inicializarMochila(Mochila *lista) {
    *lista = NULL;
}

/*---------------------------------------------
 Função: inserirItem()
 Adiciona um novo item no início da lista encadeada.
---------------------------------------------*/
void inserirItem(Mochila *lista, int *qtd_item) {
    if (*qtd_item >= TAM_MOCHILA) {
        printf("Mochila cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    char nome_item[MAX_STR_LEN];
    char tipo_item[MAX_STR_LEN - 10];
    int quantidade_do_item;
    int prioridade_do_item;

    // Coleta de dados do usuário
    printf("Digite o nome do item: ");
    fgets(nome_item, MAX_STR_LEN, stdin);
    nome_item[strcspn(nome_item, "\n")] = '\0';

    printf("Digite o tipo do item: ");
    fgets(tipo_item, MAX_STR_LEN - 10, stdin);
    tipo_item[strcspn(tipo_item, "\n")] = '\0';

    printf("Digite a quantidade do item: ");
    scanf("%d", &quantidade_do_item);
    limparBufferEntrada();

    printf("Digite a prioridade do item: ");
    scanf("%d", &prioridade_do_item);
    limparBufferEntrada();

    // Alocação dinâmica de memória para o novo nó
    No* novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro na alocação do nó.\n");
        return;
    }

    // Cópia dos dados
    novoNo->nome = strdup(nome_item);
    novoNo->tipo = strdup(tipo_item);
    novoNo->quantidade = (int*) malloc(sizeof(int));
    novoNo->prioridade = (int*) malloc(sizeof(int));

    if (novoNo->nome == NULL || novoNo->tipo == NULL || novoNo->quantidade == NULL) {
        printf("Erro na alocação dos dados do item.\n");
        free(novoNo->nome);
        free(novoNo->tipo);
        free(novoNo->quantidade);
        free(novoNo->prioridade);
        free(novoNo);
        return;
    }

    // Atribuição dos valores
    *(novoNo->prioridade) = prioridade_do_item;
    *(novoNo->quantidade) = quantidade_do_item;
    novoNo->proximo = *lista; // Insere no início da lista
    *lista = novoNo;
    (*qtd_item)++;

    printf("Item [%s] inserido com sucesso!\n", nome_item);
    printf("Pressione ENTER para continuar\n");
    getchar();
}

/*---------------------------------------------
 Função: removerItem()
 Remove um item da lista pelo nome.
---------------------------------------------*/
void removerItem(Mochila *lista, int *qtd_item) {
    if (*lista == NULL) {
        printf("A mochila está vazia.\n");
        return;
    }

    char nome_item[MAX_STR_LEN];
    printf("Digite o nome do item a ser removido: ");
    fgets(nome_item, MAX_STR_LEN, stdin);
    nome_item[strcspn(nome_item, "\n")] = '\0';

    No *atual = *lista;
    No *anterior = NULL;

    // Busca sequencial do item
    while (atual != NULL && strcmp(atual->nome, nome_item) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("ERRO: Item [%s] não encontrado.\n", nome_item);
        return;
    }

    // Ajuste dos ponteiros
    if (anterior == NULL) {
        *lista = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    // Liberação de memória
    free(atual->nome);
    free(atual->tipo);
    free(atual->quantidade);
    free(atual);
    (*qtd_item)--;

    printf("Item [%s] removido com sucesso!\n", nome_item);
    printf("Pressione ENTER para continuar\n");
    getchar();
}

/*---------------------------------------------
 Função: listarMochila()
 Exibe todos os itens da mochila em formato de tabela.
---------------------------------------------*/
void listarMochila(const Mochila lista) {
    No *temp = lista;
    if (temp == NULL) {
        printf("Você não possui itens na mochila.\n");
        printf("Aperte ENTER para continuar");
        getchar();
        return;
    }

    printf("--------------------------------------------------------------\n");
    printf("NOME          | TIPO         | QUANTIDADE    | PRIORIDADE\n");
    printf("--------------------------------------------------------------\n");

    while(temp != NULL) {
        printf("%-13s | %-12s | %-12d | %d\n",
            temp->nome, temp->tipo, *(temp->quantidade), *(temp->prioridade));
        temp = temp->proximo;
    }

    printf("----------------------------------------\n");
    printf("Aperte ENTER para continuar\n");
    getchar();
}

/*---------------------------------------------
 Função: menuOrdenacao()
 Exibe um menu de escolha de ordenação e chama
 a função correspondente.
---------------------------------------------*/
void menuOrdenacao(Mochila *lista, int *ordenacaopornome){
    int opcao = 0;
    int qtd_comparacoes = 0;

    printf("----------------------------------------\n");
    printf("          MENU DE ORDENAÇÕES     \n");
    printf("----------------------------------------\n");
    printf("1.        Ordenação por NOME\n");
    printf("2.        Ordenação por PRIORIDADE\n");
    printf("3.        Ordenação por TIPO\n");
    printf("Qual ordenação você deseja? ");
    scanf("%d", &opcao);
    limparBufferEntrada();

    switch (opcao) {
        case 1:
            OrdenacaoNome(lista, &qtd_comparacoes); 
            (*ordenacaopornome) = 1; // Ativa o sinalizador de ordenação por nome
            break;
        case 2:
            OrdenacaoPrioridade(lista, &qtd_comparacoes);
            break;
        case 3:
            OrdenacaoTipo(lista, &qtd_comparacoes);
            break;
        default:
            printf("Opção inválida!\n");
            printf("Aperte ENTER para continuar...\n");
            getchar();
            break;
    }
}

/*---------------------------------------------
 Função: TrocaDados()
 Troca os dados de dois nós (sem trocar os ponteiros).
 Usado para simular "troca de posições" na lista.
---------------------------------------------*/
void TrocaDados(No* a, No* b) {
    char* tempNome = a->nome;
    char* tempTipo = a->tipo;
    int* tempQuantidade = a->quantidade;
    int* tempPrioridade = a->prioridade;

    a->nome = b->nome;
    a->tipo = b->tipo;
    a->quantidade = b->quantidade;
    a->prioridade = b->prioridade;

    b->nome = tempNome;
    b->tipo = tempTipo;
    b->quantidade = tempQuantidade;
    b->prioridade = tempPrioridade;
}

/*---------------------------------------------
 Função: OrdenacaoNome()
 Ordena a lista em ordem alfabética crescente pelo nome.
 Algoritmo de seleção (selection sort).
---------------------------------------------*/
void OrdenacaoNome(Mochila *lista, int *qtd_comparacoes){
    if (*lista == NULL || (*lista)->proximo == NULL) {
        printf("A lista está vazia ou tem apenas um item. Nada para ordenar.\n");
        return;
    }

    No* i = *lista;
    while (i != NULL) {
        No* min = i;
        No* j = i->proximo;

        while (j != NULL) {
            if (strcmp(j->nome, min->nome) < 0) {
                min = j;
            }
            j = j->proximo;
            (*qtd_comparacoes)++;
        }

        if (min != i) {
            TrocaDados(i, min);
        }

        i = i->proximo;
    }

    printf("\n\nMochila ordenada por NOME\n");
    printf("Foram exatas %d comparações necessárias.\n", *qtd_comparacoes);
    listarMochila(*lista);
}

/*---------------------------------------------
 Função: OrdenacaoPrioridade()
 Ordena a lista pela prioridade do item (menor para maior).
---------------------------------------------*/
void OrdenacaoPrioridade(Mochila *lista, int *qtd_comparacoes){
    if (*lista == NULL || (*lista)->proximo == NULL) {
        printf("A lista está vazia ou tem apenas um item. Nada para ordenar.\n");
        return;
    }

    No* i = *lista;
    while (i != NULL) {
        No* min = i;
        No* j = i->proximo;

        while (j != NULL) {
            if (*(j->prioridade) < *(min->prioridade)) {
                min = j;
            }
            j = j->proximo;
            (*qtd_comparacoes)++;
        }

        if (min != i) {
            TrocaDados(i, min);
        }

        i = i->proximo;
    }

    printf("\n\nMochila ordenada por PRIORIDADE\n");
    printf("Foram exatas %d comparações necessárias.\n", *qtd_comparacoes);
    listarMochila(*lista);
}

/*---------------------------------------------
 Função: OrdenacaoTipo()
 Ordena a lista em ordem alfabética crescente pelo tipo do item.
---------------------------------------------*/
void OrdenacaoTipo(Mochila *lista, int *qtd_comparacoes){
    if (*lista == NULL || (*lista)->proximo == NULL) {
        printf("A lista está vazia ou tem apenas um item. Nada para ordenar.\n");
        return;
    }

    No* i = *lista;
    while (i != NULL) {
        No* min = i;
        No* j = i->proximo;

        while (j != NULL) {
            if (strcmp(j->tipo, min->tipo) < 0) {
                min = j;
            }
            j = j->proximo;
        }

        if (min != i) {
            TrocaDados(i, min);
            (*qtd_comparacoes)++;
        }

        i = i->proximo;
    }

    printf("\n\nMochila ordenada por TIPO\n");
    printf("Foram exatas %d comparações necessárias.\n", *qtd_comparacoes);
    listarMochila(*lista);
}

/*---------------------------------------------
 Função: encontrarMeio()
 Retorna o ponteiro do nó do meio entre dois nós.
 Usado para busca binária na lista.
---------------------------------------------*/
No* encontrarMeio(No* inicio, No* fim) {
    if (inicio == NULL)
        return NULL;

    No* slow = inicio;
    No* fast = inicio->proximo;

    while (fast != fim) {
        fast = fast->proximo;
        if (fast != fim) {
            slow = slow->proximo;
            fast = fast->proximo;
        }
    }

    return slow;  
}

/*---------------------------------------------
 Função: buscarItem()
 Busca um item pelo nome usando busca binária.
 Só funciona se a lista estiver ordenada por nome.
---------------------------------------------*/
void buscarItem(Mochila *lista) {
    if (*lista == NULL) {
        printf("A mochila está vazia.\n");
        return;
    }

    char nome_item[MAX_STR_LEN];
    printf("Digite o nome do item que está buscando: ");
    fgets(nome_item, MAX_STR_LEN, stdin);
    nome_item[strcspn(nome_item, "\n")] = '\0';

    No* inicio = *lista;
    No* fim = NULL;

    // Busca binária em lista encadeada
    while (inicio != fim) {
        No* meio = encontrarMeio(inicio, fim);

        if (meio == NULL)
            break;

        int cmp = strcmp(nome_item, meio->nome);

        if (cmp == 0) {
            printf("-------------------- ITEM ENCONTRADO --------------------\n");
            printf("%-13s | %-12s | %d | Prioridade: %d\n",
                meio->nome,
                meio->tipo,
                *(meio->quantidade),
                *(meio->prioridade));
            printf("---------------------------------------------------------\n");
            printf("Aperte ENTER para continuar...");
            getchar();
            return;
        } else if (cmp < 0) {
            fim = meio;
        } else {
            inicio = meio->proximo;
        }
    }

    printf("O item [%s] não está na mochila!\n", nome_item);
    printf("Aperte ENTER para continuar...");
    getchar();
}

/*---------------------------------------------
 Função: liberarMochila()
 Libera toda a memória alocada dinamicamente pela lista.
---------------------------------------------*/
void liberarMochila(Mochila *lista) {
    No* atual = *lista;
    while (atual != NULL) {
        No* temp = atual;
        atual = atual->proximo;
        free(temp->nome);
        free(temp->tipo);
        free(temp->quantidade);
        free(temp->prioridade);
        free(temp);
    }
    *lista = NULL;
}

