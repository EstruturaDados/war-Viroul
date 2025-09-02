#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição de constantes para o tamanho das strings
#define TAM_NOME 30
#define TAM_COR 10

// Struct que representa um território
typedef struct
{
    char nome[TAM_NOME];            // Nome do território
    char cor_exercito[TAM_COR];     // Cor do exército que domina o território
    int numero_tropas;              // Quantidade de tropas presentes
} territorio;

// Função para limpar o buffer
void limparbufferentrada(){

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

}

int main () {

    // Vetor de 5 territórios para cadastro
    territorio cadastros [5];

    // Loop para cadastrar os territorios
    for(int i = 0; i < 5; i++) {
        printf("\n--- Cadastrando Territorio %d ---\n", i + 1);

        // Entrada do nome do territorio
        printf("Digite o nome do Territorio: ");
        fgets(cadastros[i].nome, TAM_NOME, stdin);
        // Remover o '\n' que o fgets adiciona
        cadastros[i].nome[strcspn(cadastros[i].nome, "\n")] = '\0';
        
        // Entrada da cor do exercito
        printf("Digite a cor do Exercito (ex: Azul, Amarelo, Vermelho): ");
        fgets(cadastros[i].cor_exercito, TAM_COR, stdin);
        // Remover o '\n' que o fgets adiciona
        cadastros[i].cor_exercito[strcspn(cadastros[i].cor_exercito, "\n")] = '\0';

        // Entrada do numero de tropas
        printf("Digite o numero de tropas: ");
        scanf("%d", &cadastros[i].numero_tropas);
        limparbufferentrada(); // Limpar buffer
    }

    // Loop para exibir os territorios cadastrados
    for(int i = 0; i < 5; i++) {
        printf("\nTERRITORIO %d \n", i + 1);
        printf("- Nome: %s", cadastros[i].nome);
        printf("- Dominado por: Exercito %s",cadastros[i].cor_exercito);
        printf("- Tropas %d\n",cadastros[i].numero_tropas);
    }

    return 0;
}