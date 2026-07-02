#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =========================================================
// ESTRUTURAS
// =========================================================
typedef struct Aluno {
    char nome[50];
    char matricula[15];
    float n1;
    float n2;
    struct Aluno *prox;
} Aluno;

// =========================================================
// FUNÇÕES DO SISTEMA E DOS TESTES
// =========================================================

Aluno* cadastrarAlunoInterativo(Aluno *lista) {
    Aluno *novo = (Aluno*) malloc(sizeof(Aluno));
    if (novo == NULL) {
        printf("Erro: Memoria cheia!\n");
        return lista;
    }
    printf("Digite o nome: ");
    scanf(" %[^\n]", novo->nome);
    printf("Digite a matricula: ");
    scanf(" %[^\n]", novo->matricula);
    printf("Digite a Nota 1: ");
    scanf("%f", &novo->n1);
    printf("Digite a Nota 2: ");
    scanf("%f", &novo->n2);
    novo->prox = lista;
    return novo;
}

Aluno* cadastrarAlunoProgramatico(Aluno *lista, int idx, float n1, float n2) {
    Aluno *novo = (Aluno*) malloc(sizeof(Aluno));
    if (novo == NULL) return lista;
    snprintf(novo->nome, 50, "Aluno_%d", idx);
    snprintf(novo->matricula, 15, "RA%08d", idx);
    novo->n1 = n1;
    novo->n2 = n2;
    novo->prox = lista;
    return novo;
}

void listarAlunos(Aluno *lista) {
    Aluno *aux = lista;
    if (aux == NULL) { printf("Nenhum aluno cadastrado ainda!\n"); return; }
    while (aux != NULL) {
        printf("Nome: %s | Matricula: %s | Nota 1: %.2f | Nota 2: %.2f\n",
               aux->nome, aux->matricula, aux->n1, aux->n2);
        aux = aux->prox;
    }
}

void buscarEEditar(Aluno *lista, const char *nomeBuscado, float novaN1, float novaN2) {
    Aluno *aux = lista;
    while (aux != NULL) {
        if (strcmp(aux->nome, nomeBuscado) == 0) {
            aux->n1 = novaN1;
            aux->n2 = novaN2;
            printf("Notas do aluno '%s' atualizadas!\n", aux->nome);
            return;
        }
        aux = aux->prox;
    }
    printf("Aluno nao encontrado.\n");
}

void liberarLista(Aluno *lista) {
    Aluno *aux = lista;
    while (aux != NULL) {
        Aluno *proximo = aux->prox;
        free(aux);
        aux = proximo;
    }
}

void ordenarBubbleSort(Aluno *lista) {
    if (lista == NULL) return;
    int trocou;
    Aluno *aux;
    do {
        trocou = 0;
        aux = lista;
        while (aux->prox != NULL) {
            if (aux->n1 < aux->prox->n1) {
                float tempN1 = aux->n1;
                float tempN2 = aux->n2;
                char tempNome[50];
                char tempMatricula[15];
                strcpy(tempNome, aux->nome);
                strcpy(tempMatricula, aux->matricula);

                aux->n1 = aux->prox->n1;
                aux->n2 = aux->prox->n2;
                strcpy(aux->nome, aux->prox->nome);
                strcpy(aux->matricula, aux->prox->matricula);

                aux->prox->n1 = tempN1;
                aux->prox->n2 = tempN2;
                strcpy(aux->prox->nome, tempNome);
                strcpy(aux->prox->matricula, tempMatricula);

                trocou = 1;
            }
            aux = aux->prox;
        }
    } while (trocou);
}

int contarRecursivo(Aluno *atual) {
    if (atual == NULL) return 0;
    return 1 + contarRecursivo(atual->prox);
}

int listaEstaOrdenadaDecrescente(Aluno *lista) {
    Aluno *aux = lista;
    while (aux != NULL && aux->prox != NULL) {
        if (aux->n1 < aux->prox->n1) return 0;
        aux = aux->prox;
    }
    return 1;
}

// =========================================================
// MAIN DE TESTE (BENCHMARK) - Ativado apenas se compilar com -DBENCHMARK
// =========================================================
#ifdef BENCHMARK
#include <time.h>
int main(void) {
    int tamanhos[] = {100, 1000, 5000, 10000, 20000};
    int qtd = sizeof(tamanhos)/sizeof(int);
    srand(42);

    printf("N,tempo_insercao_ms,tempo_ordenacao_ms,tempo_contagem_ms,ordenado_ok,contagem_ok\n");

    for (int t = 0; t < qtd; t++) {
        int N = tamanhos[t];
        Aluno *lista = NULL;
        clock_t inicio, fim;

        inicio = clock();
        for (int i = 0; i < N; i++) {
            float n1 = (rand() % 1001) / 100.0f;
            float n2 = (rand() % 1001) / 100.0f;
            lista = cadastrarAlunoProgramatico(lista, i, n1, n2);
        }
        fim = clock();
        double tempoInsercao = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;

        inicio = clock();
        ordenarBubbleSort(lista);
        fim = clock();
        double tempoOrdenacao = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;

        int ordenadoOk = listaEstaOrdenadaDecrescente(lista);

        inicio = clock();
        int total = contarRecursivo(lista);
        fim = clock();
        double tempoContagem = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;

        int contagemOk = (total == N);

        printf("%d,%.4f,%.4f,%.4f,%d,%d\n", N, tempoInsercao, tempoOrdenacao, tempoContagem, ordenadoOk, contagemOk);

        liberarLista(lista);
    }
    return 0;
}

// =========================================================
// MAIN DE TESTE DE VAZAMENTO DE MEMÓRIA
// =========================================================
#elif defined(LEAKTEST)
int main(void) {
    Aluno *lista = NULL;
    for (int i = 0; i < 500; i++) {
        float n1 = (i % 10) + 0.5f;
        float n2 = (i % 7) + 0.25f;
        lista = cadastrarAlunoProgramatico(lista, i, n1, n2);
    }
    ordenarBubbleSort(lista);
    buscarEEditar(lista, "Aluno_10", 9.5f, 8.0f);
    int total = contarRecursivo(lista);
    printf("Total antes da liberacao: %d\n", total);
    liberarLista(lista);
    printf("Lista liberada.\n");
    return 0;
}

// =========================================================
// MAIN OFICIAL (MENU INTERATIVO) - O que roda por padrão
// =========================================================
#else
int main () {
    int opcao;
    Aluno *lista = NULL; 

    do {
        printf("\n===== SISTEMA DE ALUNOS ======\n");
        printf("1 - Cadastrar aluno (CRUD: Create)\n");
        printf("2 - Listar alunos (CRUD: Read)\n");
        printf("3 - Buscar e Editar aluno (CRUD: Update)\n"); 
        printf("4 - Ordenar por nota (Bubble Sort)\n");          
        printf("5 - Contar alunos (Recursividade)\n"); 
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                lista = cadastrarAlunoInterativo(lista);
                break;
            case 2:
                listarAlunos(lista);
                break;
            case 3:
                {
                    char nome[50];
                    float n1, n2;
                    printf("Digite o nome do aluno que deseja buscar: ");
                    scanf(" %[^\n]", nome);
                    printf("Digite a nova Nota 1: ");
                    scanf("%f", &n1);
                    printf("Digite a nova Nota 2: ");
                    scanf("%f", &n2);
                    
                    // Chama a função nova que não tem scanf dentro dela
                    buscarEEditar(lista, nome, n1, n2);
                }
                break;
            case 4:
                ordenarBubbleSort(lista); 
                break;
            case 5:
                { 
                    int total = contarRecursivo(lista);
                    printf("\nTotal de alunos cadastrados: %d\n", total);
                }
                break;
            case 0:
                printf("Saindo do sistema...\n");
                liberarLista(lista); 
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
#endif
