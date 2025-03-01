#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_USUARIOS 100
#define MAX_FILMES 100
#define MAX_ASSISTIDOS 1000

typedef struct {
    char login[50];
    char senha[50];
    char nome[50];
    int isAdmin;
} Usuario;

typedef struct {
    char nome[100];
    int duracaoMinutos;
    char genero[50];
    int ano;
} Filme;

typedef struct {
    char usuarioLogin[50];
    char filmeNome[100];
    char onde[50];
    char quando[11];
} FilmeAssistido;

// Variáveis globais
Usuario usuarios[MAX_USUARIOS];
Filme filmes[MAX_FILMES];
FilmeAssistido assistidos[MAX_ASSISTIDOS];
int totalUsuarios = 0;
int totalFilmes = 0;
int totalAssistidos = 0;
Usuario *usuarioLogado = NULL;

// Protótipos de funções
void hashSenha(char *senha);
bool validarData(const char *data);
void limparBuffer();

// ===================== FUNÇÕES DE ARQUIVO =====================
void salvarUsuarios() {
   
}

void carregarUsuarios() {
  
}

void salvarFilmes() {
   
}

void carregarFilmes() {
    
}

void salvarAssistidos() {
    
}

void carregarAssistidos() {
   
}

// ===================== FUNÇÕES DO SISTEMA =====================
void hashSenha(char *senha) {
   
}

bool validarData(const char *data) {
  
}

void limparBuffer() {
    
}

Usuario* fazerLogin(char *login, char *senha) {

}

int cadastrarUsuario(char *login, char *senha, char *nome) {
    
}

void cadastrarFilme() {
    
}

void assistirFilme() {
   
}

void listarAssistidos() {
     if (!usuarioLogado) {
        printf("\nFaça login primeiro!\n");
        return;
    }
    
    printf("\n=== Filmes Assistidos ===\n");
    int contador = 0;
    for (int i = 0; i < totalAssistidos; i++) {
        if (strcmp(assistidos[i].usuarioLogin, usuarioLogado->login) == 0) {
            for (int j = 0; j < totalFilmes; j++) {
                if (strcmp(filmes[j].nome, assistidos[i].filmeNome) == 0) {
                    int h = filmes[j].duracaoMinutos / 60;
                    int m = filmes[j].duracaoMinutos % 60;
                    printf("%d. %s (%dh%02dmin) - Assistido em %s via %s\n",
                          ++contador,
                          filmes[j].nome, 
                          h, m,
                          assistidos[i].quando,
                          assistidos[i].onde);
                    break;
                }
            }
        }
    }
    if (contador == 0) printf("Nenhum filme assistido ainda!\n");
}

void estatisticas() {
    if (!usuarioLogado || usuarioLogado->isAdmin) {
        printf("\nAcesso restrito a usuarios comuns!\n");
        return;
    }
    
    int totalMinutos = 0;
    for (int i = 0; i < totalAssistidos; i++) {
        if (strcmp(assistidos[i].usuarioLogin, usuarioLogado->login) == 0) {
            for (int j = 0; j < totalFilmes; j++) {
                if (strcmp(filmes[j].nome, assistidos[i].filmeNome) == 0) {
                    totalMinutos += filmes[j].duracaoMinutos;
                    break;
                }
            }
        }
    }
    
    printf("\n=== Estatisticas ===\n");
    printf("Total de filmes assistidos: %d\n", totalAssistidos);
    printf("Tempo total: %dh%02dmin\n\n", 
          totalMinutos / 60, 
          totalMinutos % 60); 
}

// ===================== MENUS =====================
void menuAdmin();
void menuUsuario();

void menuInicial() {
    
}

void menuAdmin() {

}



int main() {
    carregarUsuarios();
    carregarFilmes();
    carregarAssistidos();
    menuInicial();
    return 0;
}