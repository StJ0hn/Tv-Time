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
    FILE *file = fopen("usuarios.txt", "r");
    if (!file) return;
    
    while (fscanf(file, "%49[^,],%49[^,],%49[^,],%d\n", 
           usuarios[totalUsuarios].login,
           usuarios[totalUsuarios].senha,
           usuarios[totalUsuarios].nome,
           &usuarios[totalUsuarios].isAdmin) == 4) {
        totalUsuarios++;
        if (totalUsuarios >= MAX_USUARIOS) break;
    }
    fclose(file);
}

void salvarFilmes() {
    FILE *file = fopen("filmes.txt", "w");
    if (!file) {
        printf("Erro ao salvar filmes!\n");
        return;
    }
    for (int i = 0; i < totalFilmes; i++) {
        fprintf(file, "%s,%d,%s,%d\n", 
              filmes[i].nome, 
              filmes[i].duracaoMinutos,
              filmes[i].genero, 
              filmes[i].ano);
    }
    fclose(file);
}

void carregarFilmes() {
    FILE *file = fopen("filmes.txt", "r");
    if (!file) return;
    
    while (fscanf(file, "%99[^,],%d,%49[^,],%d\n",
           filmes[totalFilmes].nome,
           &filmes[totalFilmes].duracaoMinutos,
           filmes[totalFilmes].genero,
           &filmes[totalFilmes].ano) == 4) {
        totalFilmes++;
        if (totalFilmes >= MAX_FILMES) break;
    }
    fclose(file);
}

void salvarAssistidos() {
    FILE *file = fopen("assistidos.txt", "w");
    if (!file) {
        printf("Erro ao salvar historicos!\n");
        return;
    }
    for (int i = 0; i < totalAssistidos; i++) {
        fprintf(file, "%s,%s,%s,%s\n", 
              assistidos[i].usuarioLogin,
              assistidos[i].filmeNome,
              assistidos[i].onde,
              assistidos[i].quando);
    }
    fclose(file);
}

void carregarAssistidos() {
    FILE *file = fopen("assistidos.txt", "r");
    if (!file) return;
    
    while (fscanf(file, "%49[^,],%99[^,],%49[^,],%10s\n",
           assistidos[totalAssistidos].usuarioLogin,
           assistidos[totalAssistidos].filmeNome,
           assistidos[totalAssistidos].onde,
           assistidos[totalAssistidos].quando) == 4) {
        totalAssistidos++;
        if (totalAssistidos >= MAX_ASSISTIDOS) break;
    }
    fclose(file);
}

// ===================== FUNÇÕES DO SISTEMA =====================
void hashSenha(char *senha) {
    for(int i = 0; senha[i]; i++){
        senha[i] = toupper(senha[i]) + 3;
    }
}

bool validarData(const char *data) {
    int dia, mes, ano;
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) return false;
    
    if (ano < 1900 || ano > 2100) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;
    
    return true;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

Usuario* fazerLogin(char *login, char *senha) {
    char senhaHash[50];
    strcpy(senhaHash, senha);
    hashSenha(senhaHash);
    
    for (int i = 0; i < totalUsuarios; i++) {
        if (strcmp(usuarios[i].login, login) == 0 && strcmp(usuarios[i].senha, senhaHash) == 0) {
            return &usuarios[i];
        }
    }
    return NULL;
}

int cadastrarUsuario(char *login, char *senha, char *nome){
    for (int i = 0; i < totalUsuarios; i++) {
        if (strcmp(usuarios[i].login, login) == 0) return 0;
    }
    
    if (totalUsuarios >= MAX_USUARIOS) return -1;
    
    Usuario novo;
    strcpy(novo.login, login);
    strcpy(novo.senha, senha);
    hashSenha(novo.senha);
    strcpy(novo.nome, nome);
    novo.isAdmin = (totalUsuarios == 0) ? 1 : 0;
    
    usuarios[totalUsuarios++] = novo;
    salvarUsuarios();
    return 1;
}

void cadastrarFilme() {
    if (!usuarioLogado || !usuarioLogado->isAdmin) {
        printf("\nAcesso restrito a administradores!\n");
        return;
    }
    
    Filme novo;
    printf("\n--- Cadastro de Filme ---\n");
    
    // Nome
    printf("Nome: ");
    scanf(" %99[^\n]", novo.nome);
    limparBuffer();
    
    // Duração
    int horas, minutos;
    do {
        printf("Duracao (h:mm): ");
        if (scanf("%d:%d", &horas, &minutos) != 2) {
            printf("Formato invalido! Use h:mm\n");
            limparBuffer();
        } else if (horas < 0 || minutos < 0 || minutos >= 60) {
            printf("Valores invalidos! Ex: 2:16\n");
        }
    } while (horas < 0 || minutos < 0 || minutos >= 60);
    novo.duracaoMinutos = horas * 60 + minutos;
    limparBuffer();
    
    // Gênero
    printf("Genero: ");
    scanf(" %49[^\n]", novo.genero);
    limparBuffer();
    
    // Ano
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    do {
        printf("Ano (1888-%d): ", tm.tm_year + 1900);
        scanf("%d", &novo.ano);
        limparBuffer();
    } while(novo.ano < 1888 || novo.ano > (tm.tm_year + 1900));
    
    filmes[totalFilmes++] = novo;
    salvarFilmes();
    
    printf("\nFilme cadastrado com sucesso:\n");
    printf("Nome: %s\nDuracao: %dh%02dm\nGenero: %s\nAno: %d\n\n",
          novo.nome, 
          novo.duracaoMinutos / 60,
          novo.duracaoMinutos % 60,
          novo.genero, 
          novo.ano);
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