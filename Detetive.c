#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// ======================================================================
// Estruturas de Dados
// ======================================================================

// Estrutura da sala (árvore binária principal)
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nó da árvore de pistas (BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Estrutura para tabela hash (associação pista → suspeito)
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

// ======================================================================
// Função: criarSala()
// Objetivo: Cria dinamicamente uma sala com nome e pista associada.
// ======================================================================
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

// ======================================================================
// Função: inserirPista()
// Objetivo: Insere uma nova pista na árvore BST em ordem alfabética.
// ======================================================================
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

// ======================================================================
// Função: funcaoHash()
// Objetivo: Gera o índice da tabela hash a partir do texto da pista.
// ======================================================================
int funcaoHash(const char *pista) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++)
        soma += pista[i];
    return soma % TAM_HASH;
}

// ======================================================================
// Função: inserirNaHash()
// Objetivo: Associa uma pista a um suspeito e insere na tabela hash.
// ======================================================================
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito) {
    int indice = funcaoHash(pista);
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[indice];
    tabela[indice] = novo;
}

// ======================================================================
// Função: encontrarSuspeito()
// Objetivo: Retorna o suspeito correspondente a uma pista.
// ======================================================================
char* encontrarSuspeito(HashNode *tabela[], const char *pista) {
    int indice = funcaoHash(pista);
    HashNode *atual = tabela[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// ======================================================================
// Função: explorarSalas()
// Objetivo: Controla a navegação pela mansão e coleta de pistas.
// ======================================================================
void explorarSalas(Sala *atual, PistaNode **arvorePistas, HashNode *tabela[]) {
    char opcao;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Mostra e coleta pista, se existir
        if (strlen(atual->pista) > 0) {
            printf("🔎 Pista encontrada: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista encontrada aqui.\n");
        }

        // Exibe suspeito associado à pista (se houver)
        char *suspeito = encontrarSuspeito(tabela, atual->pista);
        if (suspeito != NULL)
            printf("Esta pista parece apontar para: %s\n", suspeito);

        // Opções de caminho
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

// ======================================================================
// Função: contarPistasPorSuspeito()
// Objetivo: Conta quantas pistas coletadas pertencem a um suspeito.
// ======================================================================
int contarPistasPorSuspeito(PistaNode *raiz, HashNode *tabela[], const char *suspeito) {
    if (raiz == NULL) return 0;
    int cont = 0;
    char *s = encontrarSuspeito(tabela, raiz->pista);
    if (s != NULL && strcmp(s, suspeito) == 0)
        cont++;
    cont += contarPistasPorSuspeito(raiz->esquerda, tabela, suspeito);
    cont += contarPistasPorSuspeito(raiz->direita, tabela, suspeito);
    return cont;
}

// ======================================================================
// Função: exibirPistas()
// Objetivo: Exibe todas as pistas coletadas (em ordem alfabética).
// ======================================================================
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf("🕵️  %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// ======================================================================
// Função: verificarSuspeitoFinal()
// Objetivo: Avalia o julgamento final e confirma (ou não) a acusação.
// ======================================================================
void verificarSuspeitoFinal(PistaNode *raiz, HashNode *tabela[]) {
    char suspeitoFinal[50];
    printf("\n===== JULGAMENTO FINAL =====\n");
    printf("Com base nas pistas, quem você acredita ser o culpado?\n→ ");
    scanf(" %[^\n]", suspeitoFinal);

    int qtd = contarPistasPorSuspeito(raiz, tabela, suspeitoFinal);

    printf("\n===== RESULTADO =====\n");
    if (qtd >= 2)
        printf("✅ Acusação consistente! %s foi considerado CULPADO com %d pistas.\n", suspeitoFinal, qtd);
    else
        printf("❌ Acusação inconsistente. Apenas %d pista(s) apontam para %s.\n", qtd, suspeitoFinal);
}

// ======================================================================
// Funções auxiliares de liberação
// ======================================================================
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

void liberarHash(HashNode *tabela[]) {
    for (int i = 0; i < TAM_HASH; i++) {
        HashNode *atual = tabela[i];
        while (atual != NULL) {
            HashNode *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
}

// ======================================================================
// Função principal: main()
// ======================================================================
int main() {
    // Criação da mansão (árvore binária fixa)
    Sala *hall = criarSala("Hall de Entrada", "Pegadas recentes próximas à porta.");
    Sala *salaEstar = criarSala("Sala de Estar", "Um copo quebrado sobre a mesa.");
    Sala *cozinha = criarSala("Cozinha", "Uma luva ensanguentada caída no chão.");
    Sala *biblioteca = criarSala("Biblioteca", "Um livro antigo aberto com uma anotação suspeita.");
    Sala *jardim = criarSala("Jardim", "");
    Sala *adega = criarSala("Adega", "Uma garrafa com impressão digital.");
    Sala *escritorio = criarSala("Escritório Secreto", "Um documento que revela a identidade do culpado!");

    // Estrutura da árvore
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->esquerda = adega;
    cozinha->direita = escritorio;

    // Criação da tabela hash (pistas → suspeitos)
    HashNode *tabela[TAM_HASH] = {NULL};
    inserirNaHash(tabela, "Pegadas recentes próximas à porta.", "Sr. Black");
    inserirNaHash(tabela, "Um copo quebrado sobre a mesa.", "Sra. White");
    inserirNaHash(tabela, "Uma luva ensanguentada caída no chão.", "Coronel Mustard");
    inserirNaHash(tabela, "Um livro antigo aberto com uma anotação suspeita.", "Prof. Plum");
    inserirNaHash(tabela, "Uma garrafa com impressão digital.", "Coronel Mustard");
    inserirNaHash(tabela, "Um documento que revela a identidade do culpado!", "Sr. Black");

    // Ponteiro para árvore de pistas coletadas
    PistaNode *arvorePistas = NULL;

    // Introdução
    printf("=== Detective Quest: O Julgamento Final ===\n");
    printf("Bem-vindo(a), detetive! Explore a mansão, colete pistas e acuse o verdadeiro culpado.\n");
    printf("Você começa no Hall de Entrada.\n");

    // Exploração
    explorarSalas(hall, &arvorePistas, tabela);

    // Mostra pistas coletadas
    printf("\n===== PISTAS COLETADAS =====\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(arvorePistas);

    // Julgamento final
    verificarSuspeitoFinal(arvorePistas, tabela);

    // Libera memória
    liberarArvoreSalas(hall);
    liberarArvorePistas(arvorePistas);
    liberarHash(tabela);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}
