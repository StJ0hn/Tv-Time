#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_USUARIOS 100
#define MAX_FILMES 100
#define MAX_ASSISTIDOS 1000

//struct de usuario comum
typedef struct usuario_comum{
    char login[50];
    char senha[50];
    char nome[50];
    int eh_admin;
}usuario_comum;

//struct de cadastro de novos filmes feito pelo admin (primeiro usuario cadastrado no sistema)
typedef struct filme{
    char nome[100];
    int duracao_do_filme;
    char genero[50];
    int ano;
}filme;

//struct para registrar os filmes assistido pelo usuario comum
typedef struct filmes_assistidos{
    char login_do_usuario_comum[50];
    char nome_filme_assistido[100];
    char plataforma[50];
    char data_da_visualização_filme[11];
}filmes_assistidos;

//inicialização de variáveis globais
usuario_comum usuarios[MAX_USUARIOS];
filme filmes[MAX_FILMES];
filmes_assistidos assistidos[MAX_ASSISTIDOS];
int total_usuarios_cadastrados = 0;
int totalFilmes = 0;
int totalAssistidos = 0;
usuario_comum *usuarioLogado = NULL;

//funções para validar senhas, data e uma para limpar buffer de entrada 
void seguranca_de_senha(char *senha);
bool comfirmar_data(const char *data);
void limpador_de_buffer();

//__________funções para manipular os arquivoss____________
void guardar_usuario_cadastrados() {
    FILE *arquivoo = fopen("usuarios.txt", "w");
    if (arquivoo == NULL) {
        printf("Erro ao salvar usuários!\n");
        return;
    }
    for (int i = 0; i < total_usuarios_cadastrados; i++) {
        fprintf(arquivoo, "%s,%s,%s,%d\n", 
              usuarios[i].login, 
              usuarios[i].senha,
              usuarios[i].nome,
              usuarios[i].eh_admin);
    }
    fclose(arquivoo);
}

void pegar_lista_dados_usuarios() {
    FILE *arquivoo = fopen("usuarios.txt", "r");
    if (arquivoo == NULL) return;
    
    while (fscanf(arquivoo, "%49[^,],%49[^,],%49[^,],%d\n", 
           usuarios[total_usuarios_cadastrados].login,
           usuarios[total_usuarios_cadastrados].senha,
           usuarios[total_usuarios_cadastrados].nome,
           &usuarios[total_usuarios_cadastrados].eh_admin) == 4) {
        total_usuarios_cadastrados++;
        if (total_usuarios_cadastrados >= MAX_USUARIOS) break;
    }
    fclose(arquivoo);
}

void guardar_nome_filmes() {
    FILE *arquivoo = fopen("filmes.txt", "w");
    if (!arquivoo) {
        printf("Erro ao salvar filmes!\n");
        return;
    }
    for (int i = 0; i < totalFilmes; i++) {
        fprintf(arquivoo, "%s,%d,%s,%d\n", 
              filmes[i].nome, 
              filmes[i].duracao_do_filme,
              filmes[i].genero, 
              filmes[i].ano);
    }
    fclose(arquivoo);
}

void carregarFilmes() {
    FILE *file = fopen("filmes.txt", "r");
    if (!file) return;
    
    while (fscanf(file, "%99[^,],%d,%49[^,],%d\n",
           filmes[totalFilmes].nome,
           &filmes[totalFilmes].duracao_do_filme,
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
              assistidos[i].login_do_usuario_comum,
              assistidos[i].nome_filme_assistido,
              assistidos[i].plataforma,
              assistidos[i].data_da_visualização_filme);
    }
    fclose(file);
}

void carregarAssistidos() {
    FILE *file = fopen("assistidos.txt", "r");
    if (!file) return;
    
    while (fscanf(file, "%49[^,],%99[^,],%49[^,],%10s\n",
           assistidos[totalAssistidos].login_do_usuario_comum,
           assistidos[totalAssistidos].nome_filme_assistido,
           assistidos[totalAssistidos].plataforma,
           assistidos[totalAssistidos].data_da_visualização_filme) == 4) {
        totalAssistidos++;
        if (totalAssistidos >= MAX_ASSISTIDOS) break;
    }
    fclose(file);
}

// ===================== FUNÇÕES DO SISTEMA =====================
void seguranca_de_senha(char *senha) {
    for(int i = 0; senha[i]; i++) {
        senha[i] = toupper(senha[i]) + 3;
    }
}

bool comfirmar_data(const char *data) {
    int dia, mes, ano;
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) return false;
    
    if (ano < 1900 || ano > 2100) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;
    
    return true;
}

void limpador_de_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

usuario_comum* fazerLogin(char *login, char *senha) {
    char senhaHash[50];
    strcpy(senhaHash, senha);
    seguranca_de_senha(senhaHash);
    
    for (int i = 0; i < total_usuarios_cadastrados; i++) {
        if (strcmp(usuarios[i].login, login) == 0 && 
            strcmp(usuarios[i].senha, senhaHash) == 0) {
            return &usuarios[i];
        }
    }
    return NULL;
}

int cadastrarUsuario(char *login, char *senha, char *nome) {
    for (int i = 0; i < total_usuarios_cadastrados; i++) {
        if (strcmp(usuarios[i].login, login) == 0) return 0;
    }
    
    if (total_usuarios_cadastrados >= MAX_USUARIOS) return -1;
    
    usuario_comum novo;
    strcpy(novo.login, login);
    strcpy(novo.senha, senha);
    seguranca_de_senha(novo.senha);
    strcpy(novo.nome, nome);
    novo.eh_admin = (total_usuarios_cadastrados == 0) ? 1 : 0;
    
    usuarios[total_usuarios_cadastrados++] = novo;
    guardar_usuario_cadastrados();
    return 1;
}

void cadastrarFilme() {
    if (!usuarioLogado || !usuarioLogado->eh_admin) {
        printf("\nAcesso restrito a administradores!\n");
        return;
    }
    
    filme novo;
    printf("\n--- Cadastro de Filme ---\n");
    
    // Nome
    printf("Nome: ");
    scanf(" %99[^\n]", novo.nome);
    limpador_de_buffer();
    
    // Duração
    int horas, minutos;
    do {
        printf("Duracao (h:mm): ");
        if (scanf("%d:%d", &horas, &minutos) != 2) {
            printf("Formato invalido! Use h:mm\n");
            limpador_de_buffer();
        } else if (horas < 0 || minutos < 0 || minutos >= 60) {
            printf("Valores invalidos! Ex: 2:16\n");
        }
    } while (horas < 0 || minutos < 0 || minutos >= 60);
    novo.duracao_do_filme = horas * 60 + minutos;
    limpador_de_buffer();
    
    // Gênero
    printf("Genero: ");
    scanf(" %49[^\n]", novo.genero);
    limpador_de_buffer();
    
    // Ano
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    do {
        printf("Ano (1888-%d): ", tm.tm_year + 1900);
        scanf("%d", &novo.ano);
        limpador_de_buffer();
    } while(novo.ano < 1888 || novo.ano > (tm.tm_year + 1900));
    
    filmes[totalFilmes++] = novo;
    guardar_nome_filmes();
    
    printf("\nFilme cadastrado com sucesso:\n");
    printf("Nome: %s\nDuracao: %dh%02dm\nGenero: %s\nAno: %d\n\n",
          novo.nome, 
          novo.duracao_do_filme / 60,
          novo.duracao_do_filme % 60,
          novo.genero, 
          novo.ano);
}

void assistirFilme() {
    if (!usuarioLogado || usuarioLogado->eh_admin) {
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
        limpador_de_buffer();
    } while (escolha < 1 || escolha > totalFilmes);
    
    filmes_assistidos novo;
    strcpy(novo.login_do_usuario_comum, usuarioLogado->login);
    strcpy(novo.nome_filme_assistido, filmes[escolha-1].nome);
    
    printf("Onde assistiu (plataforma): ");
    scanf(" %49[^\n]", novo.plataforma);
    limpador_de_buffer();
    
    do {
        printf("Quando assistiu (DD/MM/AAAA): ");
        scanf(" %10s", novo.data_da_visualização_filme);
        limpador_de_buffer();
    } while (!comfirmar_data(novo.data_da_visualização_filme));
    
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
        if (strcmp(assistidos[i].login_do_usuario_comum, usuarioLogado->login) == 0) {
            for (int j = 0; j < totalFilmes; j++) {
                if (strcmp(filmes[j].nome, assistidos[i].nome_filme_assistido) == 0) {
                    int h = filmes[j].duracao_do_filme / 60;
                    int m = filmes[j].duracao_do_filme % 60;
                    printf("%d. %s (%dh%02dmin) - Assistido em %s via %s\n",
                          ++contador,
                          filmes[j].nome, 
                          h, m,
                          assistidos[i].data_da_visualização_filme,
                          assistidos[i].plataforma);
                    break;
                }
            }
        }
    }
    if (contador == 0) printf("Nenhum filme assistido ainda!\n");
}

void estatisticas() {
    if (!usuarioLogado || usuarioLogado->eh_admin) {
        printf("\nAcesso restrito a usuarios comuns!\n");
        return;
    }
    
    int totalMinutos = 0;
    for (int i = 0; i < totalAssistidos; i++) {
        if (strcmp(assistidos[i].login_do_usuario_comum, usuarioLogado->login) == 0) {
            for (int j = 0; j < totalFilmes; j++) {
                if (strcmp(filmes[j].nome, assistidos[i].nome_filme_assistido) == 0) {
                    totalMinutos += filmes[j].duracao_do_filme;
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
            limpador_de_buffer();
            continue;
        }
        limpador_de_buffer();

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
                limpador_de_buffer();
                
                int res = cadastrarUsuario(login, senha, nome);
                if (res == 1) {
                    printf("\nCadastro realizado! ");
                    if (total_usuarios_cadastrados == 1) printf("(Voce e o administrador)");
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
                limpador_de_buffer();
                
                usuarioLogado = fazerLogin(login, senha);
                if (usuarioLogado) {
                    printf("\nBem-vindo, %s!\n", usuarioLogado->nome);
                    if (usuarioLogado->eh_admin) menuAdmin();
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
            limpador_de_buffer();
            continue;
        }
        limpador_de_buffer();

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
            limpador_de_buffer();
            continue;
        }
        limpador_de_buffer();

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
    pegar_lista_dados_usuarios();
    carregarFilmes();
    carregarAssistidos();
    menuInicial();
    return 0;
}