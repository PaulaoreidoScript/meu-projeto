#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define TAM 100

// Remove o Enter do final do texto
void limparTexto(char texto[]) {
    texto[strcspn(texto, "\n")] = '\0';
}

// Carrega os produtos do arquivo
int carregarEstoque(char nomes[][TAM], int quantidades[]) {
    FILE *arquivo;
    int total = 0;

    arquivo = fopen("inventory.txt", "r");

    if (arquivo == NULL) {
        return 0;
    }

    while (total < MAX && fgets(nomes[total], TAM, arquivo) != NULL) {

        limparTexto(nomes[total]);

        if (fscanf(arquivo, "%d", &quantidades[total]) != 1) {
            break;
        }

        // Consome o Enter depois da quantidade
        fgetc(arquivo);

        total++;
    }

    fclose(arquivo);

    return total;
}

// Salva os produtos no arquivo
void salvarEstoque(char nomes[][TAM], int quantidades[], int total) {
    FILE *arquivo;

    arquivo = fopen("inventory.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao salvar o estoque!\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        fprintf(arquivo, "%s\n", nomes[i]);
        fprintf(arquivo, "%d\n", quantidades[i]);
    }

    fclose(arquivo);
}

// Adiciona um produto
void adicionarItem(char nomes[][TAM], int quantidades[], int *total) {
    char nome[TAM];
    int quantidade;

    printf("Digite o nome do item: ");
    fgets(nome, TAM, stdin);
    limparTexto(nome);

    printf("Digite a quantidade: ");
    scanf("%d", &quantidade);
    getchar();

    if (strlen(nome) == 0 || quantidade <= 0) {
        printf("Dados invalidos!\n");
        return;
    }

    // Verifica se o produto ja existe
    for (int i = 0; i < *total; i++) {

        if (strcmp(nomes[i], nome) == 0) {

            quantidades[i] += quantidade;

            salvarEstoque(nomes, quantidades, *total);

            printf("Item adicionado com sucesso!\n");
            return;
        }
    }

    if (*total >= MAX) {
        printf("Limite de produtos atingido!\n");
        return;
    }

    strcpy(nomes[*total], nome);
    quantidades[*total] = quantidade;

    (*total)++;

    salvarEstoque(nomes, quantidades, *total);

    printf("Item adicionado com sucesso!\n");
}

// Remove um produto ou parte dele
void removerItem(char nomes[][TAM], int quantidades[], int *total) {
    char nome[TAM];
    int quantidade;

    printf("Digite o nome do item: ");
    fgets(nome, TAM, stdin);
    limparTexto(nome);

    printf("Digite a quantidade a ser removida: ");
    scanf("%d", &quantidade);
    getchar();

    if (quantidade <= 0) {
        printf("Quantidade invalida!\n");
        return;
    }

    for (int i = 0; i < *total; i++) {

        if (strcmp(nomes[i], nome) == 0) {

            if (quantidade > quantidades[i]) {
                printf("Estoque insuficiente. Quantidade disponivel: %d\n",
                       quantidades[i]);
                return;
            }

            quantidades[i] -= quantidade;

            // Se a quantidade chegar a zero, remove o produto
            if (quantidades[i] == 0) {

                for (int j = i; j < *total - 1; j++) {
                    strcpy(nomes[j], nomes[j + 1]);
                    quantidades[j] = quantidades[j + 1];
                }

                (*total)--;

                salvarEstoque(nomes, quantidades, *total);

                printf("Item removido do estoque!\n");

            } else {

                salvarEstoque(nomes, quantidades, *total);

                printf("Quantidade atualizada com sucesso!\n");
            }

            return;
        }
    }

    printf("Item nao encontrado.\n");
}

// Lista os produtos cadastrados
void listarEstoque(char nomes[][TAM], int quantidades[], int total) {

    printf("=============================\n");
    printf("        Estoque Atual\n");
    printf("=============================\n");

    if (total == 0) {
        printf("O estoque esta vazio.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        printf("Nome: %s\n", nomes[i]);
        printf("Quantidade: %d\n\n", quantidades[i]);
    }
}

// Funcao principal
int main() {

    char nomes[MAX][TAM];
    int quantidades[MAX];

    char resposta[10];
    int opcao;
    int total;

    total = carregarEstoque(nomes, quantidades);

    while (1) {

        printf("\n==================================\n");
        printf("       Controle de Estoque\n");
        printf("==================================\n");

        printf("1. Adicionar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Estoque\n");
        printf("4. Sair\n");

        printf("Opcao: ");

        if (fgets(resposta, sizeof(resposta), stdin) == NULL) {
            break;
        }

        opcao = atoi(resposta);

        // Verifica se a opcao e valida
        if (opcao < 1 || opcao > 4) {
            printf("Opcao invalida! Tente novamente.\n");
            continue;
        }

        if (opcao == 1) {

            adicionarItem(nomes, quantidades, &total);

        } else if (opcao == 2) {

            removerItem(nomes, quantidades, &total);

        } else if (opcao == 3) {

            listarEstoque(nomes, quantidades, total);

        } else if (opcao == 4) {

            break;
        }

        printf("\nDeseja realizar outra operacao? (s/n): ");

        if (fgets(resposta, sizeof(resposta), stdin) == NULL) {
            break;
        }

        if (resposta[0] != 's' && resposta[0] != 'S') {
            break;
        }
    }

    printf("\nObrigado por usar o Controle de Estoques! Ate a proxima.\n");

    return 0;
}
