// =============================================================================
// PROJETO: Simulador de Fila do Tetris (Fila Circular)
// DESCRIÇÃO: Este programa simula a fila de próximas peças de um jogo
//            como o Tetris, usando uma estrutura de dados de fila circular
//            para gerenciar as peças.
// CONCEITOS: Fila Circular, Structs, Arrays, Modularização.
// =============================================================================

// 1. Bibliotecas Necessárias
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- DEFINIÇÕES GLOBAIS E STRUCTS ---

// Define o tamanho fixo da nossa fila de peças
#define CAPACIDADE_FILA 5

/**
 * @brief Estrutura para representar uma única peça do jogo.
 */
struct Peca {
    char nome; // 'I', 'O', 'T', 'L'
    int id;
};

/**
 * @brief Estrutura da Fila Circular.
 * Contém o vetor de peças e os índices de controle.
 */
struct Fila {
    struct Peca pecas[CAPACIDADE_FILA]; // O vetor que armazena as peças
    int inicio;   // Índice do primeiro elemento (frente da fila)
    int fim;      // Índice da próxima posição livre (final da fila)
    int tamanho;  // Número atual de elementos na fila
};

// --- PROTÓTIPOS DAS FUNÇÕES ---
void inicializarFila(struct Fila *f);
struct Peca gerarPeca();
int estaVazia(struct Fila *f);
int estaCheia(struct Fila *f);
void enqueue(struct Fila *f); // Inserir nova peça
void dequeue(struct Fila *f); // Jogar peça
void exibirFila(struct Fila *f);
void exibirMenu();

// --- FUNÇÃO PRINCIPAL ---
int main() {
    int opcao;

    // Inicializa o gerador de números aleatórios (essencial para gerarPeca)
    srand(time(NULL));

    // 1. Cria e inicializa a fila
    struct Fila filaDePecas;
    inicializarFila(&filaDePecas);

    // 2. Preenche a fila inicial com 5 peças, conforme solicitado
    printf("Inicializando a fila com %d pecas...\n", CAPACIDADE_FILA);
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enqueue(&filaDePecas);
    }

    // 3. Loop principal do menu
    do {
        // Mostra o estado atual da fila ANTES de cada ação
        exibirFila(&filaDePecas);
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                dequeue(&filaDePecas); // Jogar peça
                break;
            case 2:
                enqueue(&filaDePecas); // Inserir nova peça
                break;
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\j.\n");
        }
        printf("\n"); // Adiciona um espaço para legibilidade

    } while (opcao != 0);

    return 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES AUXILIARES ---

/**
 * @brief Configura a fila para um estado inicial vazio.
 */
void inicializarFila(struct Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
}

/**
 * @brief Gera uma nova peça aleatória.
 * Utiliza uma variável estática para garantir que o ID seja sempre único.
 * @return Uma nova struct Peca.
 */
struct Peca gerarPeca() {
    static int proximoId = 0; // 'static' mantém o valor entre as chamadas
    char tiposDePeca[] = {'I', 'O', 'T', 'L'};
    
    struct Peca novaPeca;
    novaPeca.nome = tiposDePeca[rand() % 4]; // Sorteia um dos 4 tipos
    novaPeca.id = proximoId;
    
    proximoId++; // Incrementa o ID para a próxima peça
    return novaPeca;
}

/**
 * @brief Verifica se a fila está vazia.
 * @return 1 (verdadeiro) se a fila estiver vazia, 0 (falso) caso contrário.
 */
int estaVazia(struct Fila *f) {
    return (f->tamanho == 0);
}

/**
 * @brief Verifica se a fila está cheia.
 * @return 1 (verdadeiro) se a fila estiver cheia, 0 (falso) caso contrário.
 */
int estaCheia(struct Fila *f) {
    return (f->tamanho == CAPACIDADE_FILA);
}

/**
 * @brief Insere uma nova peça no final da fila (Enqueue).
 * Só insere se a fila não estiver cheia.
 */
void enqueue(struct Fila *f) {
    if (estaCheia(f)) {
        printf("Acao falhou: A fila de pecas esta cheia! (Jogue uma peca primeiro)\n");
    } else {
        // Gera uma nova peça
        struct Peca novaPeca = gerarPeca();
        
        // Adiciona a peça na posição 'fim'
        f->pecas[f->fim] = novaPeca;
        
        // Atualiza o índice 'fim' de forma circular
        // O operador '%' (módulo) faz com que o índice "dê a volta" para 0
        // se ele atingir o final do vetor.
        f->fim = (f->fim + 1) % CAPACIDADE_FILA;
        
        // Incrementa o contador de tamanho
        f->tamanho++;
        
        printf("Nova peca [%c %d] inserida no final da fila.\n", novaPeca.nome, novaPeca.id);
    }
}

/**
 * @brief Remove a peça da frente da fila (Dequeue).
 * Só remove se a fila não estiver vazia.
 */
void dequeue(struct Fila *f) {
    if (estaVazia(f)) {
        printf("Acao falhou: A fila de pecas esta vazia! (Insira uma peca primeiro)\n");
    } else {
        // Pega a peça que está na posição 'inicio'
        struct Peca pecaJogada = f->pecas[f->inicio];
        
        // Atualiza o índice 'inicio' de forma circular
        f->inicio = (f->inicio + 1) % CAPACIDADE_FILA;
        
        // Decrementa o contador de tamanho
        f->tamanho--;
        
        printf("Peca [%c %d] foi jogada!\n", pecaJogada.nome, pecaJogada.id);
    }
}

/**
 * @brief Exibe o estado atual de todas as peças na fila.
 */
void exibirFila(struct Fila *f) {
    printf("Fila de pecas: ");
    if (estaVazia(f)) {
        printf("[Fila Vazia]\n");
    } else {
        // Percorre a fila de forma circular
        // 'i' é o contador de quantos elementos exibimos
        // 'j' é o índice real no vetor, começando em 'inicio'
        int i, j;
        for (i = 0, j = f->inicio; i < f->tamanho; i++) {
            // Imprime a peça na posição 'j'
            printf("[%c %d] ", f->pecas[j].nome, f->pecas[j].id);
            
            // Avança 'j' de forma circular
            j = (j + 1) % CAPACIDADE_FILA;
        }
        printf("\n");
    }
}

/**
 * @brief Exibe o menu de opções para o jogador.
 */
void exibirMenu() {
    printf("\nOpcoes de acao:\n");
    printf("1. Jogar peca (dequeue)\n");
    printf("2. Inserir nova peca (enqueue)\n");
    printf("0. Sair\n");
    printf("Escolha sua acao: ");
}