#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 🔹 1️⃣ Estrutura de dados do usuário
typedef struct usuario_comum {
    char nome[100];
    char senha[9];
    int admin;
} Usuario_comum;

// 🔹 2️⃣ Função para cadastrar usuário
void cadastrarUsuario() {
    // Implementar cadastro de usuário
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
