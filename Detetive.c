#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma sala (nó da árvore binária)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ---------------------------------------------------------------------
// Função: criarSala()
// Objetivo: Cria uma nova sala dinamicamente e define seus caminhos.
// ---------------------------------------------------------------------
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ---------------------------------------------------------------------
// Função: explorarSalas()
// Objetivo: Permite que o jogador explore a mansão a partir de uma sala.
//            Ele pode escolher ir à esquerda, à direita ou sair do jogo.
// ---------------------------------------------------------------------
void explorarSalas(Sala *atual) {
    char opcao;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Verifica se chegou a um nó folha (sem caminhos)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Não há mais caminhos a seguir... Você chegou ao fim da exploração!\n");
            break;
        }

        printf("Escolha seu caminho:\n");
        if (atual->esquerda != NULL) printf("  (e) Ir para %s\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf("  (d) Ir para %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("→ ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda != NULL)
                atual = atual->esquerda;
            else
                printf("Caminho à esquerda inexistente!\n");
        } 
        else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita != NULL)
                atual = atual->direita;
            else
                printf("Caminho à direita inexistente!\n");
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

// ---------------------------------------------------------------------
// Função: liberarArvore()
// Objetivo: Libera toda a memória alocada para a árvore binária.
// ---------------------------------------------------------------------
void liberarArvore(Sala *raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

// ---------------------------------------------------------------------
// Função: main()
// Objetivo: Monta o mapa da mansão e inicia a exploração.
// ---------------------------------------------------------------------
int main() {
    // Criação automática da estrutura da mansão (árvore binária fixa)
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *adega = criarSala("Adega");
    Sala *escritorio = criarSala("Escritório Secreto");

    // Montagem da estrutura da árvore (mapa da mansão)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->esquerda = adega;
    cozinha->direita = escritorio;

    // Início da exploração
    printf("=== Detective Quest: Exploração da Mansão Enigma ===\n");
    printf("Bem-vindo(a), detetive! Sua missão é explorar a mansão em busca de pistas.\n");
    printf("Começando pelo Hall de Entrada...\n");

    explorarSalas(hall);

    // Libera memória antes de encerrar
    liberarArvore(hall);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}
