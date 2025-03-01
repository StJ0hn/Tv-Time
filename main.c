#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMO_DE_USUARIOS 100
#define MAXIMO_DE_FILMES 100
#define MAX_FILMES_ASSISTIDOS 100


//Struct para dados dos usuários
typedef struct usuario_comum {
    char nome[100];
    char senha[9];
    int admin;
} usuario_comum;

//Struct para dados dos filmes
typedef struct {
    char nome[100];
    int duracaoMinutos;
    char genero[50];
    int ano;
} Filme;

//Struct para filmes já assistidos
typedef struct {
    char usuarioLogin[50];
    char filmeNome[100];
    char onde[50];
    char quando[11];
} FilmeAssistido;


//Função para cadastrar usuário
void cadastrarUsuario() {
    //Alocar memoria para usuario
    usuario_comum *Usuario = malloc(sizeof(usuario_comum));
    //Pedir nome para cadastro:
    printf("Cadastre seu nome: ");
    scanf("%s", Usuario->nome);
    printf("Cadastre uma senha (apenas números): ");
    scanf("%s", Usuario->senha);

    FILE *arquivo = fopen("usuarios.txt", "r+");
}

// 🔹 3️⃣ Função para login do usuário
int fazerLogin() {
    // Implementar login do usuário
    return 0; // Retornar status de login
}

// 🔹 4️⃣ Função para cadastro de filmes (Apenas Admin)
void cadastrarFilme() {
    // Implementar cadastro de filmes
}

// 🔹 5️⃣ Função para registrar filme assistido
void registrarFilmeAssistido() {
    // Implementar registro de filmes assistidos
}

// 🔹 6️⃣ Função para listar filmes assistidos
void listarFilmesAssistidos() {
    // Implementar listagem de filmes assistidos
}

// 🔹 7️⃣ Função para mostrar estatísticas de filmes assistidos
void mostrarEstatisticas() {
    // Implementar estatísticas de filmes assistidos
}

// 🔹 8️⃣ Função principal
int main() {
    int opcao, logado = 0;

    do {
        printf("\n===== TV TIME =====\n");
        printf("1 - Cadastrar Usuário\n");
        printf("2 - Login\n");
        printf("3 - Cadastrar Filme (Admin)\n");
        printf("4 - Registrar Filme Assistido\n");
        printf("5 - Listar Filmes Assistidos\n");
        printf("6 - Ver Estatísticas\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                cadastrarUsuario();
                break;
            case 2:
                logado = fazerLogin();
                break;
            case 3:
                if (logado) cadastrarFilme();
                else printf("Você precisa estar logado!\n");
                break;
            case 4:
                if (logado) registrarFilmeAssistido();
                else printf("Você precisa estar logado!\n");
                break;
            case 5:
                if (logado) listarFilmesAssistidos();
                else printf("Você precisa estar logado!\n");
                break;
            case 6:
                if (logado) mostrarEstatisticas();
                else printf("Você precisa estar logado!\n");
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
