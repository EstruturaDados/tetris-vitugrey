// =============================================================================
// PROJETO: Tetris Stack Avançado - Trocas Estratégicas
// DESCRIÇÃO: Simula o gerenciamento de peças de Tetris usando uma Fila
//            Circular e uma Pilha, com operações avançadas de troca.
// CONCEITOS: Fila Circular, Pilha, Modularização, Troca de Dados.
// =============================================================================

// 1. Bibliotecas Necessárias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- DEFINIÇÕES GLOBAIS E ESTRUTURAS ---

// 2. Constantes de Tamanho
#define TAMANHO_FILA 5
#define CAPACIDADE_PILHA 3

// 3. Estrutura da Peça (Peca)
struct Peca {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador numérico único
};

// 4. Estrutura da Fila Circular (FilaCircular)
struct FilaCircular {
    struct Peca pecas[TAMANHO_FILA];
    int inicio; // Índice do primeiro elemento
    int fim;    // Índice do último elemento
};

// 5. Estrutura da Pilha (Pilha)
struct Pilha {
    struct Peca pecas[CAPACIDADE_PILHA];
    int topo; // Índice do elemento no topo (-1 se vazia)
};

// 6. Contador Global de IDs
int idContador = 0;

// --- PROTÓTIPOS DAS FUNÇÕES ---

// Funções Auxiliares
struct Peca gerarPeca();

// Funções da Fila
void inicializarFila(struct FilaCircular *f);
struct Peca dequeueFila(struct FilaCircular *f);
void adicionarNovaPecaFila(struct FilaCircular *f); // Renomeada de enqueueFila

// Funções da Pilha
void inicializarPilha(struct Pilha *p);
int isPilhaVazia(struct Pilha *p);
int isPilhaCheia(struct Pilha *p);
void pushPilha(struct Pilha *p, struct Peca peca);
struct Peca popPilha(struct Pilha *p);

// Funções de Interface do Jogo
void exibirEstado(struct FilaCircular *f, struct Pilha *p);
void exibirMenu();

// --- NOVAS FUNÇÕES (DESAFIO ATUAL) ---
void trocarPecaAtual(struct FilaCircular *f, struct Pilha *p);
void trocaMultipla(struct FilaCircular *f, struct Pilha *p);


// --- FUNÇÃO PRINCIPAL (main) ---
int main() {
    srand(time(NULL));
    struct FilaCircular fila;
    struct Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opcao;

    do {
        exibirEstado(&fila, &pilha);
        exibirMenu();
        scanf("%d", &opcao);
        while (getchar() != '\n'); 

        switch (opcao) {
            // Caso 1: Jogar Peça (Remove da fila, adiciona nova)
            case 1: {
                struct Peca jogada = dequeueFila(&fila);
                printf("\n>>> Peça [ %c %d ] jogada.\n", jogada.nome, jogada.id);
                adicionarNovaPecaFila(&fila);
                break;
            }

            // Caso 2: Reservar Peça (Fila -> Pilha, adiciona nova)
            case 2:
                if (isPilhaCheia(&pilha)) {
                    printf("\n>>> ERRO: Pilha de reserva cheia!\n");
                } else {
                    struct Peca reservada = dequeueFila(&fila);
                    pushPilha(&pilha, reservada);
                    printf("\n>>> Peça [ %c %d ] reservada.\n", reservada.nome, reservada.id);
                    adicionarNovaPecaFila(&fila);
                }
                break;

            // Caso 3: Usar Peça Reservada (Remove da pilha)
            case 3:
                if (isPilhaVazia(&pilha)) {
                    printf("\n>>> ERRO: Pilha de reserva vazia!\n");
                } else {
                    struct Peca usada = popPilha(&pilha);
                    printf("\n>>> Peça [ %c %d ] usada da reserva.\n", usada.nome, usada.id);
                }
                break;
            
            // --- NOVOS CASOS ---

            // Caso 4: Trocar peça da frente da fila com o topo da pilha
            case 4:
                trocarPecaAtual(&fila, &pilha);
                break;

            // Caso 5: Trocar os 3 primeiros da fila com os 3 da pilha
            case 5:
                trocaMultipla(&fila, &pilha);
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
            getchar(); 
        }

    } while (opcao != 0);

    return 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

/**
 * @brief Gera uma nova peça com tipo aleatório e ID único.
 */
struct Peca gerarPeca() {
    static const char tiposDePeca[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    struct Peca novaPeca;
    novaPeca.nome = tiposDePeca[rand() % 7];
    novaPeca.id = idContador++;
    return novaPeca;
}

/**
 * @brief Preenche a fila com as peças iniciais.
 */
void inicializarFila(struct FilaCircular *f) {
    f->inicio = 0;
    for (int i = 0; i < TAMANHO_FILA; i++) {
        f->pecas[i] = gerarPeca();
    }
    f->fim = TAMANHO_FILA - 1;
}

/**
 * @brief Remove e retorna a peça da frente da fila.
 */
struct Peca dequeueFila(struct FilaCircular *f) {
    struct Peca pecaRemovida = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    return pecaRemovida;
}

/**
 * @brief Adiciona uma nova peça (gerada) ao final da fila.
 */
void adicionarNovaPecaFila(struct FilaCircular *f) {
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->pecas[f->fim] = gerarPeca();
}

/**
 * @brief Inicializa a pilha como vazia.
 */
void inicializarPilha(struct Pilha *p) {
    p->topo = -1;
}

/**
 * @brief Verifica se a pilha está vazia.
 */
int isPilhaVazia(struct Pilha *p) {
    return (p->topo == -1);
}

/**
 * @brief Verifica se a pilha está cheia.
 */
int isPilhaCheia(struct Pilha *p) {
    return (p->topo == CAPACIDADE_PILHA - 1);
}

/**
 * @brief Adiciona uma peça ao topo da pilha (push).
 */
void pushPilha(struct Pilha *p, struct Peca peca) {
    if (!isPilhaCheia(p)) {
        p->topo++;
        p->pecas[p->topo] = peca;
    }
}

/**
 * @brief Remove e retorna a peça do topo da pilha (pop).
 */
struct Peca popPilha(struct Pilha *p) {
    if (!isPilhaVazia(p)) {
        return p->pecas[p->topo--];
    }
    // Retorna peça "vazia" em caso de erro
    struct Peca erroPeca = {' ', -1};
    return erroPeca;
}

/**
 * @brief Exibe o estado atual da Fila e da Pilha.
 */
void exibirEstado(struct FilaCircular *f, struct Pilha *p) {
    system("clear || cls"); 
    printf("================ TETRIS STACK (Avançado) ================\n");
    
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
        for (int j = p->topo; j >= 0; j--) {
            printf("[ %c %d ] ", p->pecas[j].nome, p->pecas[j].id);
        }
    }
    printf("\n========================================================\n");
}

/**
 * @brief Exibe o menu de opções para o jogador.
 */
void exibirMenu() {
    printf("\n--- Opções Disponíveis ---\n");
    printf("1. Jogar peça da frente da fila\n");
    printf("2. Enviar peça da fila para a pilha de reserva\n");
    printf("3. Usar peça da pilha de reserva\n");
    printf("4. Trocar peça da frente da fila com o topo da pilha\n");
    printf("5. Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
    printf("0. Sair\n");
    printf("Opção escolhida: ");
}

// --- IMPLEMENTAÇÃO DAS NOVAS FUNÇÕES ---

/**
 * @brief Troca a peça da frente da fila com a peça do topo da pilha.
 * @param f Ponteiro para a FilaCircular.
 * @param p Ponteiro para a Pilha.
 */
void trocarPecaAtual(struct FilaCircular *f, struct Pilha *p) {
    // Validação: A pilha não pode estar vazia para uma troca.
    if (isPilhaVazia(p)) {
        printf("\n>>> ERRO: Impossivel trocar. A pilha de reserva esta vazia.\n");
        return;
    }

    // A fila está sempre cheia, então não precisamos validar.
    
    // 1. Cria uma variável temporária para a troca
    struct Peca temp;

    // 2. Armazena a peça da frente da fila
    temp = f->pecas[f->inicio];

    // 3. Coloca a peça do topo da pilha na frente da fila
    f->pecas[f->inicio] = p->pecas[p->topo];

    // 4. Coloca a peça (antiga da fila) no topo da pilha
    p->pecas[p->topo] = temp;

    printf("\n>>> Ação: Troca 1x1 realizada com sucesso!\n");
}

/**
 * @brief Troca as 3 primeiras peças da fila com as 3 peças da pilha.
 * @param f Ponteiro para a FilaCircular.
 * @param p Ponteiro para a Pilha.
 */
void trocaMultipla(struct FilaCircular *f, struct Pilha *p) {
    // Validação: A pilha DEVE estar cheia (com 3 peças).
    if (!isPilhaCheia(p)) {
        printf("\n>>> ERRO: Troca multipla exige 3 pecas na pilha de reserva!\n");
        return;
    }

    // A fila está sempre cheia (5 peças), então podemos trocar as 3 primeiras.
    printf("\n>>> Ação: Realizando troca 3x3...\n");

    // Vamos trocar em bloco:
    // Fila[inicio]   <-> Pilha[topo]
    // Fila[inicio+1] <-> Pilha[topo-1]
    // Fila[inicio+2] <-> Pilha[topo-2]
    
    for (int i = 0; i < 3; i++) {
        // 1. Calcula o índice correto da fila (circular)
        int indiceFila = (f->inicio + i) % TAMANHO_FILA;
        
        // 2. Calcula o índice correto da pilha (de cima para baixo)
        int indicePilha = p->topo - i;

        // 3. Realiza a troca
        struct Peca temp = f->pecas[indiceFila];
        f->pecas[indiceFila] = p->pecas[indicePilha];
        p->pecas[indicePilha] = temp;
    }

    printf("Troca multipla concluida com sucesso!\n");
}