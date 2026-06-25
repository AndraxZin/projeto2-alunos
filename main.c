#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//a ficha de escrição
typedef struct Aluno {
    char nome [50];
    char matrícula [15];
    float n1;
    float n2;
    struct Aluno *prox; //ainda não sei mas isso aqui é um ponteiro que vai apontar para algo 
} Aluno;

int main () {
    int opcao;

    do {
        printf("\n===== SISTEMA DE ALUNOS ======\n");
        printf("1 - Cadastrar aluno\n");
        printf("2 - listar aluno\n");
        printf("3 - buscar aluno\n");
        printf("4 - ordenar por nota aluno\n");
        printf("5 - Desfazer ultima operação\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                /* code */
                break;
            default:
                break;
        }
    } while (opcao != 0);

    return 0;
}
