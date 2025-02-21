#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 🔹 1️⃣ Você - Cadastro e Login
void cadastrarUsuario();  
int fazerLogin();  

// 🔹 2️⃣ Amigo 1 - Cadastro de filmes e Registrar filmes assistidos
void cadastrarFilme();  
void registrarFilmeAssistido();  

// 🔹 3️⃣ Amigo 2 - Listar filmes assistidos e Estatísticas
void listarFilmesAssistidos();  
void mostrarEstatisticas();  

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
