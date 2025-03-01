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
    for(int i = 0; senha[i]; i++) {
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
        if (strcmp(usuarios[i].login, login) == 0 && 
            strcmp(usuarios[i].senha, senhaHash) == 0) {
            return &usuarios[i];
        }
    }
    return NULL;
}

int cadastrarUsuario(char *login, char *senha, char *nome) {
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
    if (!usuarioLogado || usuarioLogado->isAdmin) {
        printf("\nAcesso restrito a usuarios comuns!\n");
        return;
    }
    
    printf("\n=== Filmes Disponiveis ===\n");
    for (int i = 0; i < totalFilmes; i++) {
        printf("%d. %s\n", i+1, filmes[i].nome);
    }
    
    int escolha;
    do {
        printf("\nEscolha o filme (1-%d): ", totalFilmes);
        scanf("%d", &escolha);
        limparBuffer();
    } while (escolha < 1 || escolha > totalFilmes);
    
    FilmeAssistido novo;
    strcpy(novo.usuarioLogin, usuarioLogado->login);
    strcpy(novo.filmeNome, filmes[escolha-1].nome);
    
    printf("Onde assistiu (plataforma): ");
    scanf(" %49[^\n]", novo.onde);
    limparBuffer();
    
    do {
        printf("Quando assistiu (DD/MM/AAAA): ");
        scanf(" %10s", novo.quando);
        limparBuffer();
    } while (!validarData(novo.quando));
    
    assistidos[totalAssistidos++] = novo;
    salvarAssistidos();
    printf("\nRegistro salvo com sucesso!\n");
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
    int opcao;
    while (1) {
        printf("\n=== TV Time ===\n");
        printf("1. Cadastrar\n2. Login\n3. Sair\nEscolha: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida!\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1: {
                char login[50], senha[50], nome[50];
                printf("\n--- Novo Cadastro ---\n");
                printf("Login: ");
                scanf("%49s", login);
                printf("Senha: ");
                scanf("%49s", senha);
                printf("Nome: ");
                scanf("%49s", nome);
                limparBuffer();
                
                int res = cadastrarUsuario(login, senha, nome);
                if (res == 1) {
                    printf("\nCadastro realizado! ");
                    if (totalUsuarios == 1) printf("(Voce e o administrador)");
                    printf("\n");
                } else if (res == 0) {
                    printf("\nLogin ja existe!\n");
                } else {
                    printf("\nLimite de usuarios atingido!\n");
                }
                break;
            }
            case 2: {
                char login[50], senha[50];
                printf("\n--- Login ---\n");
                printf("Login: ");
                scanf("%49s", login);
                printf("Senha: ");
                scanf("%49s", senha);
                limparBuffer();
                
                usuarioLogado = fazerLogin(login, senha);
                if (usuarioLogado) {
                    printf("\nBem-vindo, %s!\n", usuarioLogado->nome);
                    if (usuarioLogado->isAdmin) menuAdmin();
                    else menuUsuario();
                } else {
                    printf("\nCredenciais invalidas!\n");
                }
                break;
            }
            case 3:
                printf("\nSaindo...\n");
                exit(0);
            default:
                printf("\nOpção invalida!\n");
        }
    }
}

void menuAdmin() {
    int opcao;
    while (1) {
        printf("\n=== Menu Admin ===\n");
        printf("1. Cadastrar filme\n2. Logout\nEscolha: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida!\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (opcao == 1) cadastrarFilme();
        else if (opcao == 2) {
            usuarioLogado = NULL;
            return;
        }
        else printf("Opção invalida!\n");
    }
}

void menuUsuario() {
    int opcao;
    while (1) {
        printf("\n=== Menu Usuario ===\n");
        printf("1. Registrar filme assistido\n");
        printf("2. Listar filmes assistidos\n");
        printf("3. Estatisticas\n");
        printf("4. Logout\nEscolha: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida!\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1: assistirFilme(); break;
            case 2: listarAssistidos(); break;
            case 3: estatisticas(); break;
            case 4:
                usuarioLogado = NULL;
                return;
            default:
                printf("Opção invalida!\n");
        }
    }
}

int main() {
    carregarUsuarios();
    carregarFilmes();
    carregarAssistidos();
    menuInicial();
    return 0;
}