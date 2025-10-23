// =============================================================================
// PROJETO: Tetris Stack - Gerenciamento de Peças
// DESCRIÇÃO: Simula o gerenciamento de peças de Tetris usando uma Fila
//            Circular (próximas peças) e uma Pilha (reserva/hold).
// CONCEITOS: Structs, Fila Circular, Pilha, Modularização, Funções.
// =============================================================================

// 1. Bibliotecas Necessárias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- DEFINIÇÕES GLOBAIS E ESTRUTURAS ---

// 2. Constantes de Tamanho
// Definindo os tamanhos fixos para nossas estruturas.
#define TAMANHO_FILA 5
#define CAPACIDADE_PILHA 3

// 3. Estrutura da Peça (Peca)
// Define o modelo de dados para cada peça do jogo.
struct Peca {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador numérico único
};

// 4. Estrutura da Fila Circular (FilaCircular)
// Armazena as próximas peças.
struct FilaCircular {
    struct Peca pecas[TAMANHO_FILA];
    int inicio; // Índice do primeiro elemento
    int fim;    // Índice do último elemento
};

// 5. Estrutura da Pilha (Pilha)
// Armazena as peças reservadas.
struct Pilha {
    struct Peca pecas[CAPACIDADE_PILHA];
    int topo; // Índice do elemento no topo (-1 se vazia)
};

// 6. Contador Global de IDs
// Garante que cada peça gerada tenha um ID único.
int idContador = 0;

// --- PROTÓTIPOS DAS FUNÇÕES ---

// Funções Auxiliares
struct Peca gerarPeca();

// Funções da Fila
void inicializarFila(struct FilaCircular *f);
struct Peca dequeueFila(struct FilaCircular *f);
void enqueueFila(struct FilaCircular *f);

// Funções da Pilha
void inicializarPilha(struct Pilha *p);
int isPilhaVazia(struct Pilha *p);
int isPilhaCheia(struct Pilha *p);
void pushPilha(struct Pilha *p, struct Peca peca);
struct Peca popPilha(struct Pilha *p);

// Funções de Interface do Jogo
void exibirEstado(struct FilaCircular *f, struct Pilha *p);
void exibirMenu();


// --- FUNÇÃO PRINCIPAL (main) ---
int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Declara e inicializa as estruturas do jogo
    struct FilaCircular fila;
    struct Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opcao;

    // Loop principal do jogo
    do {
        // Mostra o estado atual e o menu
        exibirEstado(&fila, &pilha);
        exibirMenu();
        scanf("%d", &opcao);
        while (getchar() != '\n'); // Limpa o buffer de entrada

        switch (opcao) {
            // Caso 1: Jogar Peça
            case 1: {
                // 1. Remove a peça da frente da fila
                struct Peca jogada = dequeueFila(&fila);
                printf("\n>>> Peça [ %c %d ] jogada.\n", jogada.nome, jogada.id);
                // 2. Adiciona uma nova peça ao final da fila
                enqueueFila(&fila);
                break;
            }

            // Caso 2: Reservar Peça
            case 2:
                // Verifica se a pilha de reserva não está cheia
                if (isPilhaCheia(&pilha)) {
                    printf("\n>>> ERRO: Pilha de reserva cheia! Jogue a peça reservada primeiro.\n");
                } else {
                    // 1. Remove a peça da frente da fila
                    struct Peca reservada = dequeueFila(&fila);
                    // 2. Adiciona a peça removida à pilha
                    pushPilha(&pilha, reservada);
                    printf("\n>>> Peça [ %c %d ] reservada.\n", reservada.nome, reservada.id);
                    // 3. Adiciona uma nova peça ao final da fila
                    enqueueFila(&fila);
                }
                break;

            // Caso 3: Usar Peça Reservada
            case 3:
                // Verifica se a pilha de reserva não está vazia
                if (isPilhaVazia(&pilha)) {
                    printf("\n>>> ERRO: Pilha de reserva vazia! Nao ha pecas para usar.\n");
                } else {
                    // 1. Remove a peça do topo da pilha
                    struct Peca usada = popPilha(&pilha);
                    printf("\n>>> Peça [ %c %d ] usada da reserva.\n", usada.nome, usada.id);
                    // (Neste jogo, usar a reserva não afeta a fila)
                }
                break;

            // Caso 0: Sair
            case 0:
                printf("Saindo do Tetris Stack...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }

        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar(); // Pausa para o usuário ler a saída
        }

    } while (opcao != 0);

    return 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES AUXILIARES E ESTRUTURAS ---

/**
 * @brief Gera uma nova peça com tipo aleatório e ID único.
 * @return A struct Peca recém-criada.
 */
struct Peca gerarPeca() {
    // Tipos de peças disponíveis
    static const char tiposDePeca[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    
    struct Peca novaPeca;
    // Sorteia um tipo de peça
    novaPeca.nome = tiposDePeca[rand() % 7];
    // Atribui e incrementa o ID global
    novaPeca.id = idContador++;
    
    return novaPeca;
}

/**
 * @brief Preenche a fila com as peças iniciais.
 * @param f Ponteiro para a FilaCircular.
 */
void inicializarFila(struct FilaCircular *f) {
    f->inicio = 0;
    for (int i = 0; i < TAMANHO_FILA; i++) {
        f->pecas[i] = gerarPeca();
    }
    // Em uma fila cheia de N itens, o 'fim' aponta para o último item.
    f->fim = TAMANHO_FILA - 1;
}

/**
 * @brief Remove e retorna a peça da frente da fila.
 * @param f Ponteiro para a FilaCircular.
 * @return A peça que estava na frente.
 */
struct Peca dequeueFila(struct FilaCircular *f) {
    // Pega a peça do início
    struct Peca pecaRemovida = f->pecas[f->inicio];
    
    // Avança o 'inicio' de forma circular
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    
    return pecaRemovida;
}

/**
 * @brief Adiciona uma nova peça (gerada) ao final da fila.
 * @param f Ponteiro para a FilaCircular.
 */
void enqueueFila(struct FilaCircular *f) {
    // Avança o 'fim' de forma circular
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    
    // Gera e insere a nova peça na posição 'fim'
    // (Em uma fila circular cheia, a nova peça sobrescreve
    // a posição que acabou de ser "liberada" pelo início,
    // mantendo a fila sempre cheia.)
    f->pecas[f->fim] = gerarPeca();
}

/**
 * @brief Inicializa a pilha como vazia.
 * @param p Ponteiro para a Pilha.
 */
void inicializarPilha(struct Pilha *p) {
    // Uma pilha vazia é representada por topo = -1
    p->topo = -1;
}

/**
 * @brief Verifica se a pilha está vazia.
 * @return 1 (verdadeiro) se vazia, 0 (falso) caso contrário.
 */
int isPilhaVazia(struct Pilha *p) {
    return (p->topo == -1);
}

/**
 * @brief Verifica se a pilha está cheia.
 * @return 1 (verdadeiro) se cheia, 0 (falso) caso contrário.
 */
int isPilhaCheia(struct Pilha *p) {
    return (p->topo == CAPACIDADE_PILHA - 1);
}

/**
 * @brief Adiciona uma peça ao topo da pilha (push).
 * @param p Ponteiro para a Pilha.
 * @param peca A peça a ser adicionada.
 */
void pushPilha(struct Pilha *p, struct Peca peca) {
    if (isPilhaCheia(p)) {
        // Esta verificação já é feita no main, mas é uma boa prática
        printf("ERRO: Tentativa de push em pilha cheia!\n");
    } else {
        p->topo++;
        p->pecas[p->topo] = peca;
    }
}

/**
 * @brief Remove e retorna a peça do topo da pilha (pop).
 * @param p Ponteiro para a Pilha.
 * @return A peça que estava no topo.
 */
struct Peca popPilha(struct Pilha *p) {
    if (isPilhaVazia(p)) {
        // Esta verificação já é feita no main
        printf("ERRO: Tentativa de pop em pilha vazia!\n");
        // Retorna uma peça "vazia" em caso de erro
        struct Peca erroPeca = {' ', -1};
        return erroPeca;
    } else {
        struct Peca pecaRemovida = p->pecas[p->topo];
        p->topo--;
        return pecaRemovida;
    }
}

/**
 * @brief Exibe o estado atual da Fila e da Pilha.
 * @param f Ponteiro para a FilaCircular.
 * @param p Ponteiro para a Pilha.
 */
void exibirEstado(struct FilaCircular *f, struct Pilha *p) {
    system("clear || cls"); // Limpa a tela (Linux/Mac || Windows)
    printf("================ TETRIS STACK =================\n");
    
    // 1. Exibir Fila Circular
    printf("Fila de peças (Proxima -> ...): \n");
    int i = f->inicio;
    for (int count = 0; count < TAMANHO_FILA; count++) {
        printf("[ %c %d ] ", f->pecas[i].nome, f->pecas[i].id);
        i = (i + 1) % TAMANHO_FILA;
    }
    printf("\n\n");

    // 2. Exibir Pilha
    printf("Pilha de reserva (Topo -> Base): \n");
    if (isPilhaVazia(p)) {
        printf("[ Vazia ]");
    } else {
        // Imprime do topo para a base
        for (int j = p->topo; j >= 0; j--) {
            printf("[ %c %d ] ", p->pecas[j].nome, p->pecas[j].id);
        }
    }
    printf("\n===============================================\n");
}

/**
 * @brief Exibe o menu de opções para o jogador.
 */
void exibirMenu() {
    printf("\n--- Opções de Ação ---\n");
    printf("1. Jogar peça (Remove da fila)\n");
    printf("2. Reservar peça (Fila -> Pilha)\n");
    printf("3. Usar peça reservada (Pilha -> Jogo)\n");
    printf("0. Sair\n");
    printf("Opção: ");
}