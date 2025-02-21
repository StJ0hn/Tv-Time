# Tv-Time
EN: Application for managing preferences about movies.
PT-BR: Aplicação para gerenciamento de preferências sobre filmes.

## Estrutura do Código
### Estrutura do main.c
#### 1 - Cadastro e Login
- Função para cadastrar usuário → Escreve no arquivo usuarios.txt;
- Função para fazer login → Lê o arquivo usuarios.txt e verifica credenciais.
___________________________________________________________________________________________________________
#### 2 - Cadastro de filmes e Registrar filmes assistidos
- Função para cadastrar filme (Administrador) → Escreve no arquivo filmes.txt;
- Função para registrar filme assistido (Usuário comum) → Escreve no arquivo assistidos.txt.
___________________________________________________________________________________________________________
#### 3 - Listar filmes assistidos e Estatísticas
- Função para listar filmes assistidos → Lê assistidos.txt e exibe os dados;
- Função para gerar estatísticas → Calcula tempo total e outras métricas a partir de assistidos.txt.
___________________________________________________________________________________________________________
#### Organização dos Arquivos
- usuarios.txt → Armazena login e senha dos usuários;
- filmes.txt → Armazena os filmes cadastrados pelo admin;
- assistidos.txt → Registra os filmes que os usuários assistiram.
___________________________________________________________________________________________________________
##### Exemplo de Como deverá ser o arquivo ```usuarios.txt```
- Cada linha representa um usuário com:
- ```Login | Senha | Nome```
___________________________________________________________________________________________________________
##### Exemplo de como deverá ser o arquivo ```filmes.txt```
- Cada linha representa um filme cadastrado pelo administrador com:
- ```Nome | Duração (min) | Gênero | Ano de Lançamento```
___________________________________________________________________________________________________________
##### Exemplo de como deverá ser o arquivo ```assistidos.txt```
- Cada linha representa um filme assistido por um usuário com:
- ```Usuário | Nome do Filme | Plataforma | Data```
