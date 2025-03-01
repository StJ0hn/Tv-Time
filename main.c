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
   FILE *file = fopen("usuarios.txt", "w");
    if (!file) {
        printf("Erro ao salvar usuários!\n");
        return;
    }
    for (int i = 0; i < totalUsuarios; i++) {
        fprintf(file, "%s,%s,%s,%d\n", 
              usuarios[i].login, 
              usuarios[i].senha,
              usuarios[i].nome,
              usuarios[i].isAdmin);
    }
    fclose(file);
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
    
}

void estatisticas() {
    
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