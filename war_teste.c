#include <stdio.h>   
#include <stdlib.h>  
#include <string.h>  
#include <time.h>
#include <stdbool.h>

// Definição de constantes para o tamanho das strings
#define TAM_NOME 30
#define TAM_COR 10
#define TAM_MISSAO 100

// Struct que representa um território
typedef struct {
    char nome[TAM_NOME];       // Nome do território
    char cor_exercito[TAM_COR]; // Cor do exército que domina o território
    int numero_tropas;          // Quantidade de tropas presentes
} territorio;

// ================= FUNÇÕES ===================

// funcao pra liberar memoria
void liberarMemoria(territorio* mapa) {
    free(mapa);
}

// funcao para limpar o buffer
void limparbufferentrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// funcao para gerar rolagem do dado (1 a 6)
int rolar_dado() {
    return (rand() % 6) + 1;
}

//funcao para exibir menu
int exibir_menu() {
    int opcao;
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1. Exibir territórios\n");
    printf("2. Atacar\n");
    printf("3. Verificar missão\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    limparbufferentrada();
    return opcao;
}

// funcao para cadastrar territorios
territorio* cadastrar_territorios(int quantidade) {
    territorio *cadastros = calloc(quantidade, sizeof(territorio));
    if (cadastros == NULL) {
        printf("Erro na alocação!\n");
        exit(1);
    }

    for (int i = 0; i < quantidade; i++) {
        printf("\n--- Cadastrando Território %d ---\n", i + 1);

        printf("Digite o nome do Território: ");
        fgets(cadastros[i].nome, TAM_NOME, stdin);
        cadastros[i].nome[strcspn(cadastros[i].nome, "\n")] = '\0';

        printf("Digite a cor do Exército (ex: Azul, Amarelo, Vermelho): ");
        fgets(cadastros[i].cor_exercito, TAM_COR, stdin);
        cadastros[i].cor_exercito[strcspn(cadastros[i].cor_exercito, "\n")] = '\0';

        printf("Digite o numero de tropas: ");
        scanf("%d", &cadastros[i].numero_tropas);
        limparbufferentrada();
    }
    return cadastros;
}

// funcao para exibir os territórios
void exibir_territorios(territorio *territorios, int quantidade) {
    printf("\n=== TERRITÓRIOS ===\n");
    for (int i = 0; i < quantidade; i++) {
        printf("\nTerritório %d\n", i + 1);
        printf("- Nome: %s\n", territorios[i].nome);
        printf("- Dominado por: Exército %s\n", territorios[i].cor_exercito);
        printf("- Tropas: %d\n", territorios[i].numero_tropas);
    }
}

// funcao para escolher territorios para ataque
void escolher_territorio(territorio *territorios, int quantidade, int *atacante, int *defensor){
    printf("\n--- FASE DE ATAQUE ---\n");
    
    // Escolha do atacante
    do {
        printf("\nEscolha o território atacante (1 a %d, 0 para sair): ", quantidade);
        scanf("%d", atacante);
        limparbufferentrada();

        if (*atacante == 0) {
            printf("Saindo...\n");
            return;
        }

        if (*atacante < 1 || *atacante > quantidade) {
            printf("Erro. Entrada inválida.\n");
        }
    } while (*atacante < 1 || *atacante > quantidade);

    // Escolha do defensor
    do {
        printf("Escolha o território defensor (1 a %d, 0 para sair): ", quantidade);
        scanf("%d", defensor);
        limparbufferentrada();

        if (*defensor == 0) {
            printf("Saindo da fase de ataque...\n");
            *atacante = 0; // garante que o main saiba que é pra parar
            return;
        }

        if (*defensor < 1 || *defensor > quantidade) {
            printf("Erro. Entrada inválida.\n");
            continue;
        }

        if (*defensor == *atacante) {
            printf("Erro. Não pode atacar o mesmo território.\n");
            continue;
        }

        if (strcmp(territorios[*defensor - 1].cor_exercito,
                territorios[*atacante - 1].cor_exercito) == 0) {
            printf("Erro. Não pode atacar um território da mesma cor de exército.\n");
            continue;
        }
        break;

    } while (1);
}

//funcao para simular um ataque
void simular_ataque(territorio *atacante, territorio *defensor) {
    //Declaracao dos dados
    int dado_atacante = rolar_dado();
    int dado_defensor = rolar_dado();

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s tirou no dado: %d\n", atacante->nome, dado_atacante);
    printf("O defensor %s tirou no dado: %d\n", defensor->nome, dado_defensor);

    if (dado_atacante > dado_defensor) { // atacante venceu
        printf("VITORIA DO ATACANTE! O defensor perdeu 1 tropa.\n");
        defensor->numero_tropas -= 1;// quando o defensor perder
        
        if (defensor->numero_tropas <= 0){ // Se defensor ficar sem tropas, conquista
            printf("CONQUISTA! O territorio %s foi dominado pelo %s!", defensor->nome, atacante->cor_exercito);
            
            // Transferência de cor do exército
            strncpy(defensor->cor_exercito, atacante->cor_exercito, TAM_COR);
            defensor->cor_exercito[TAM_COR - 1] = '\0';

            // Transferência de tropas
            int tropas_transferidas = atacante->numero_tropas / 2;
            if (tropas_transferidas < 1) tropas_transferidas = 1;          // sempre pelo menos 1
            defensor->numero_tropas = tropas_transferidas;                 // transfere as tropas para o conquistado
            atacante->numero_tropas -= tropas_transferidas;                // tira metada das tropas do atacante
            if (atacante->numero_tropas < 1) atacante->numero_tropas = 1;  // sempre pelo menos 1
        }
        
        
    } else if (dado_defensor > dado_atacante) {
        printf("%s Resistiu ao ataque!\n", defensor->nome);

         // Atacante perde 1 tropa
        atacante->numero_tropas -= 1;
        if (atacante->numero_tropas < 1) {
            atacante->numero_tropas = 1; // sempre pelo menos 1
            printf("%s foi derrotado por falta de tropas.\n", atacante->nome);
            // Transferência de cor do exército
            strncpy(atacante->cor_exercito, defensor->cor_exercito, TAM_COR);
            atacante->cor_exercito[TAM_COR - 1] = '\0';
        }
    } else {
        printf("Empate!\n");   
    }
}

// funcao para sortear missao
int sortear_missao(int totalMissoes) {
    if (totalMissoes <= 0) return -1;
    return rand() % totalMissoes;
}

// funcao exibir a missao
void exibir_missao(const char *missao) {
    if (missao == NULL) return;
    printf("\n=== MISSÃO DO JOGADOR ===\n");
    printf("%s\n", missao);
}

//funcao para atribuir missao
void atribuir_missao(char** destino, char* missoes[], int total) {
    int indice = sortear_missao(total);
    if (indice == -1) return;

    *destino = malloc((strlen(missoes[indice]) + 1) * sizeof(char));
    if (*destino != NULL) {
        strcpy(*destino, missoes[indice]);
    }
}

// funcao para verificar a missao
int verificar_missao(char *missao, territorio *mapa, int tamanho) {
    if (!missao || !mapa || tamanho <= 0) return 0;

    int tropasNecessarias = 0;
    if (strstr(missao, "10 tropas")) tropasNecessarias = 10;
    else if (strstr(missao, "6 tropas")) tropasNecessarias = 6;
    else if (strstr(missao, "5 tropas")) tropasNecessarias = 5;
    else if (strstr(missao, "2 tropas")) tropasNecessarias = 2;
    else if (strstr(missao, "1 tropas")) tropasNecessarias = 1;

    for (int i = 0; i < tamanho; i++) {
        if (mapa[i].numero_tropas >= tropasNecessarias)
            return 1;
    }
    return 0;
}

// funcao para verificr e exibir missão cumprida
void checar_missao(territorio *mapa, int tamanho, char **missao, bool acao_realizada) {
    if (*missao == NULL) return; // missão já cumprida
    if (!acao_realizada) return;

    if (verificar_missao(*missao, mapa, tamanho)) {
        printf("\nPARABÉNS! Você cumpriu a missão!\n");
        exibir_missao(*missao);

        // Libera memória e marca como NULL
        free(*missao);
        *missao = NULL;
    }
}

// ============== CODIGO PRINCIPAL ===================
int main() {
    // Variáveis
    int quantidade_territorios, escolha_atacante, escolha_defensor, escolha_menu;
    srand(time(NULL)); // inicializa o rand para os dados e sorteio de missão
    bool acao_realizada = false; // flag, falso-positivo

    // Pergunta quantos territórios deseja cadastrar
    printf("Quantos territórios deseja cadastrar (ex: 4): ");
    scanf("%d", &quantidade_territorios);
    limparbufferentrada();

    // Definição das missões possíveis
    char *missoes[] = {
        "Ter pelo menos 10 tropas em um território",
        "Ter pelo menos 6 tropas em um território",
        "Ter pelo menos 5 tropas em um território",
        "Ter pelo menos 2 tropas em um território",
        "Ter pelo menos 1 tropas em um território"
    };

    // Cadastro dos territórios
    territorio *cadastros = cadastrar_territorios(quantidade_territorios);

    // Sorteia uma missão para o jogador
    char *missao_jogador = NULL;
    atribuir_missao(&missao_jogador, missoes, 5);
    exibir_missao(missao_jogador);

    // Menu principal
    while (1) {
        escolha_menu = exibir_menu();

        switch(escolha_menu) {
            case 1: // Exibir territórios
                exibir_territorios(cadastros, quantidade_territorios);
                break;

            case 2: // Ataque
                exibir_territorios(cadastros, quantidade_territorios);
                escolher_territorio(cadastros, quantidade_territorios, &escolha_atacante, &escolha_defensor);

                if (escolha_atacante != 0) {
                    simular_ataque(&cadastros[escolha_atacante - 1], &cadastros[escolha_defensor - 1]);
                    
                    // Atualiza territórios após batalha
                    exibir_territorios(cadastros, quantidade_territorios);

                    acao_realizada = true; // jogador realizou uma ação válida

                    // Verifica imediatamente se a missão foi cumprida
                    checar_missao(cadastros, quantidade_territorios, &missao_jogador, acao_realizada);
                }
                break;

            case 3: // Verificar missão manualmente
                if (missao_jogador == NULL)
                    printf("\nVocê já cumpriu sua missão!\n");
                else
                    printf("\nA missão ainda não foi cumprida.\n");
                break;

            case 0: // Sair
                printf("Saindo do jogo...\n");
                liberarMemoria(cadastros);
                free(missao_jogador);
                exit(0);

            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }

    return 0;
}

