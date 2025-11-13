#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =====================================================================
// Estruturas de Dados
// =====================================================================

// Estrutura para representar cada sala (nó da árvore da mansão)
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura para representar as pistas coletadas (nó da árvore BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// =====================================================================
// Função: criarSala()
// Objetivo: Cria uma nova sala com nome e pista opcional.
// =====================================================================
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// =====================================================================
// Função: inserirPista()
// Objetivo: Insere uma nova pista na árvore BST em ordem alfabética.
// =====================================================================
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    // se for igual, não insere duplicata
    return raiz;
}

// =====================================================================
// Função: exibirPistas()
// Objetivo: Exibe todas as pistas em ordem alfabética (in-order).
// =====================================================================
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf("🕵️  %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// =====================================================================
// Função: explorarSalasComPistas()
// Objetivo: Controla a exploração da mansão e coleta automática de pistas.
// =====================================================================
void explorarSalasComPistas(Sala *atual, PistaNode **arvorePistas) {
    char opcao;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Coleta automática de pista (se existir e ainda não coletada)
        if (strlen(atual->pista) > 0) {
            printf("🔎 Você encontrou uma pista: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista encontrada aqui.\n");
        }

        // Mostra opções de caminho
        printf("\nEscolha seu próximo destino:\n");
        if (atual->esquerda != NULL) printf("  (e) Ir para %s\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf("  (d) Ir para %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("→ ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda != NULL)
                atual = atual->esquerda;
            else
                printf("Não há caminho à esquerda!\n");
        } 
        else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita != NULL)
                atual = atual->direita;
            else
                printf("Não há caminho à direita!\n");
        } 
        else if (opcao == 's' || opcao == 'S') {
            printf("\nVocê decidiu encerrar a exploração.\n");
            break;
        } 
        else {
            printf("Opção inválida! Escolha novamente.\n");
        }
    }
}

// =====================================================================
// Função: liberarArvoreSalas() e liberarArvorePistas()
// Objetivo: Liberam a memória alocada dinamicamente.
// =====================================================================
void liberarArvoreSalas(Sala *raiz) {
    if (raiz == NULL) return;
    liberarArvoreSalas(raiz->esquerda);
    liberarArvoreSalas(raiz->direita);
    free(raiz);
}

void liberarArvorePistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

// =====================================================================
// Função: main()
// Objetivo: Monta o mapa da mansão e inicia a exploração com pistas.
// =====================================================================
int main() {
    // Criação das salas e associação de pistas
    Sala *hall = criarSala("Hall de Entrada", "Pegadas recentes próximas à porta.");
    Sala *salaEstar = criarSala("Sala de Estar", "Um copo quebrado sobre a mesa.");
    Sala *cozinha = criarSala("Cozinha", "Uma luva ensanguentada caída no chão.");
    Sala *biblioteca = criarSala("Biblioteca", "Um livro antigo aberto com uma anotação suspeita.");
    Sala *jardim = criarSala("Jardim", "");
    Sala *adega = criarSala("Adega", "Uma garrafa com impressão digital.");
    Sala *escritorio = criarSala("Escritório Secreto", "Um documento que revela a identidade do culpado!");

    // Montagem da estrutura da árvore da mansão
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->esquerda = adega;
    cozinha->direita = escritorio;

    // Ponteiro para a árvore de pistas (inicialmente vazia)
    PistaNode *arvorePistas = NULL;

    // Início do jogo
    printf("=== Detective Quest: Coleta de Pistas ===\n");
    printf("Bem-vindo(a), detetive! Explore a mansão e descubra os segredos ocultos.\n");
    printf("Começando pelo Hall de Entrada...\n");

    explorarSalasComPistas(hall, &arvorePistas);

    // Exibe todas as pistas coletadas
    printf("\n===== PISTAS COLETADAS (em ordem alfabética) =====\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(arvorePistas);

    // Libera memória
    liberarArvoreSalas(hall);
    liberarArvorePistas(arvorePistas);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}
