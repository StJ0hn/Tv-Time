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
int total_de_filmes = 0;
int total_filmes_assistidos = 0;
usuario_comum *usuario_logado = NULL;

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
    for (int i = 0; i < total_de_filmes; i++) {
        fprintf(arquivoo, "%s,%d,%s,%d\n", 
              filmes[i].nome, 
              filmes[i].duracao_do_filme,
              filmes[i].genero, 
              filmes[i].ano);
    }
    fclose(arquivoo);
}

void carregar_filmes_cadastrados() {
    FILE *arquivoo = fopen("filmes.txt", "r");
    if (!arquivoo){
        return;
    }    
    
    while(fscanf(arquivoo, "%99[^,],%d,%49[^,],%d\n", 
           filmes[total_de_filmes].nome,
           &filmes[total_de_filmes].duracao_do_filme,
           filmes[total_de_filmes].genero,
           &filmes[total_de_filmes].ano) == 4){
        total_de_filmes++;
        if (total_de_filmes >= MAX_FILMES){
            break;
        } 
    }
    fclose(arquivoo);
}

void salvar_filmes_assistidos() {
    FILE *arquivoo = fopen("assistidos.txt", "w");
    if (arquivoo == NULL) {
        printf("Erro ao salvar histórico de filmes assistidos.\n");
        return;
    }
    for (int i = 0; i < total_filmes_assistidos; i++) { //percorrer todos os filmes designados como assistidos
        fprintf(arquivoo, "%s,%s,%s,%s\n", 
              assistidos[i].login_do_usuario_comum,
              assistidos[i].nome_filme_assistido,
              assistidos[i].plataforma,
              assistidos[i].data_da_visualização_filme);
    }
    fclose(arquivoo);
}

void carregar_filmes_assistidos() {
    FILE *arquivoo = fopen("assistidos.txt", "r");
    if (arquivoo == NULL){
        return;
    } 
    
    while (fscanf(arquivoo, "%49[^,],%99[^,],%49[^,],%10s\n",
           assistidos[total_filmes_assistidos].login_do_usuario_comum,
           assistidos[total_filmes_assistidos].nome_filme_assistido,
           assistidos[total_filmes_assistidos].plataforma,
           assistidos[total_filmes_assistidos].data_da_visualização_filme) == 4) {
        total_filmes_assistidos++;
        if (total_filmes_assistidos >= MAX_ASSISTIDOS) break;
    }
    fclose(arquivoo);
}

// ===================== FUNÇÕES DO SISTEMA =====================
void seguranca_de_senha(char *senha) {
    for(int i = 0; senha[i]; i++) {
        char temp = toupper((unsigned char)senha[i]) + 3;
        if (temp > 255){ //tatar erro de conversão.
            temp -= 256;
        }  
        senha[i] = temp;
    }
}

bool comfirmar_data(const char *data) {
    int dia;
    int mes;
    int ano;
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3){ //verificar se a dta foi inserida no formato desejado
        return false;
    } 

    if (ano < 1900 || ano > 2100){ //verificar se o ano inserido está no intervalo desejado
        return false;
    }

    if (mes < 1 || mes > 12){ //verificando se o mês inserido está no formato desejado
        return false;
    }
    if (dia < 1 || dia > 31){ //verificando se os dias estão de acordo com o calendario normal 
        return false;
    }
    
    return true;
}

void limpador_de_buffer() { //função para limpar o buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

usuario_comum *efetuar_login(char *login, char *senha) {
    char assegurar_senha[50];
    strcpy(assegurar_senha, senha);
    seguranca_de_senha(assegurar_senha);
    
    for (int i = 0; i < total_usuarios_cadastrados; i++) {
        if (strcmp(usuarios[i].login, login) == 0 && 
            strcmp(usuarios[i].senha, assegurar_senha) == 0){
            return &usuarios[i];
        }
    }
    return NULL;
}

int fazer_cadastro_usuario(char *login, char *senha, char *nome) {
    for (int i = 0; i < total_usuarios_cadastrados; i++) {
        if (strcmp(usuarios[i].login, login) == 0){
            return 0;
        } 
    }
    
    if (total_usuarios_cadastrados >= MAX_USUARIOS){
        return -1;
    } 
    
    usuario_comum novo;
    strcpy(novo.login, login);
    strcpy(novo.senha, senha);
    seguranca_de_senha(novo.senha);
    strcpy(novo.nome, nome);
    if (total_usuarios_cadastrados == 0) {
        novo.eh_admin = 1;
    } else {
        novo.eh_admin = 0;
    }
    
    usuarios[total_usuarios_cadastrados++] = novo;
    guardar_usuario_cadastrados();
    return 1;
}

void fazer_cadastrar_filme() {
    if (usuario_logado == NULL || !usuario_logado->eh_admin){
        printf("\nAcesso restrito a administradores!\n");
        return;
    }
    
    filme novo;
    printf("\n\033[1;32m--- Cadastro de Filme ---\033[m\n");
    
    printf("Nome: ");
    scanf(" %99[^\n]", novo.nome);
    limpador_de_buffer();
    
    int horas, minutos;

    do {
        printf("Duracao (h:mm): ");
        if (scanf("%d:%d", &horas, &minutos) != 2){
            printf("Formato invalido! Use h:mm\n");
            limpador_de_buffer();
        } 
        else if (horas < 0 || minutos < 0 || minutos >= 60){
            printf("Valores invalidos! Ex: 2:16\n");
        }
    } 
    while (horas < 0 || minutos < 0 || minutos >= 60);
    novo.duracao_do_filme = horas * 60 + minutos;
    limpador_de_buffer();
    
    printf("Genero: ");
    scanf(" %49[^\n]", novo.genero);
    limpador_de_buffer();
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    do {
        printf("Ano (1888-%d): ", tm.tm_year + 1900);
        scanf("%d", &novo.ano);
        limpador_de_buffer();
    }
    while(novo.ano < 1888 || novo.ano > (tm.tm_year + 1900));
    
    filmes[total_de_filmes++] = novo;
    guardar_nome_filmes();
    
    printf("\nFilme cadastrado com sucesso:\n");
    printf("Nome: %s\nDuracao: %dh%02dm\nGenero: %s\nAno: %d\n\n",
          novo.nome, 
          novo.duracao_do_filme / 60,
          novo.duracao_do_filme % 60,
          novo.genero, 
          novo.ano);
}

void registrar_visualizacao_filme() {
    if (!usuario_logado || usuario_logado->eh_admin) {
        printf("\nAcesso restrito a usuarios comuns!\n");
        return;
    }
    
    printf("\n=== Filmes Disponiveis ===\n");
    for (int i = 0; i < total_de_filmes; i++) {
        printf("%d. %s\n", i+1, filmes[i].nome);
    }
    
    int escolha;
    do {
        printf("\nEscolha o filme (1-%d): ", total_de_filmes);
        scanf("%d", &escolha);
        limpador_de_buffer();
    } 
    while (escolha < 1 || escolha > total_de_filmes);
    
    filmes_assistidos novo;
    strcpy(novo.login_do_usuario_comum, usuario_logado->login);
    strcpy(novo.nome_filme_assistido, filmes[escolha-1].nome);
    
    printf("Onde assistiu (plataforma): ");
    scanf(" %49[^\n]", novo.plataforma);
    limpador_de_buffer();
    
    do {
        printf("Quando assistiu (DD/MM/AAAA): ");
        scanf(" %10s", novo.data_da_visualização_filme);
        limpador_de_buffer();
    } while (!comfirmar_data(novo.data_da_visualização_filme));
    
    assistidos[total_filmes_assistidos++] = novo;
    salvar_filmes_assistidos();
    printf("\nRegistro salvo com sucesso!\n");
}

void listarAssistidos() {
    if (!usuario_logado) {
        printf("\nFaça login primeiro!\n");
        return;
    }
    
    printf("\n=== Filmes Assistidos ===\n");
    int contador = 0;
    for (int i = 0; i < total_filmes_assistidos; i++) {
        if (strcmp(assistidos[i].login_do_usuario_comum, usuario_logado->login) == 0) {
            for (int j = 0; j < total_de_filmes; j++) {
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
    if (!usuario_logado || usuario_logado->eh_admin) {
        printf("\nAcesso restrito a usuarios comuns!\n");
        return;
    }
    
    int totalMinutos = 0;
    for (int i = 0; i < total_filmes_assistidos; i++) {
        if (strcmp(assistidos[i].login_do_usuario_comum, usuario_logado->login) == 0) {
            for (int j = 0; j < total_de_filmes; j++) {
                if (strcmp(filmes[j].nome, assistidos[i].nome_filme_assistido) == 0) {
                    totalMinutos += filmes[j].duracao_do_filme;
                    break;
                }
            }
        }
    }
    
    printf("\n=== Estatisticas ===\n");
    printf("Total de filmes assistidos: %d\n", total_filmes_assistidos);
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
                
                int res = fazer_cadastro_usuario(login, senha, nome);
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
                
                usuario_logado = efetuar_login(login, senha);
                if (usuario_logado) {
                    printf("\nBem-vindo, %s!\n", usuario_logado->nome);
                    if (usuario_logado->eh_admin) menuAdmin();
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

        if (opcao == 1) fazer_cadastrar_filme();
        else if (opcao == 2) {
            usuario_logado = NULL;
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
            case 1: registrar_visualizacao_filme(); break;
            case 2: listarAssistidos(); break;
            case 3: estatisticas(); break;
            case 4:
                usuario_logado = NULL;
                return;
            default:
                printf("Opção invalida!\n");
        }
    }
}

int main() {
    pegar_lista_dados_usuarios();
    carregar_filmes_cadastrados();
    carregar_filmes_assistidos();
    menuInicial();
    return 0;
}