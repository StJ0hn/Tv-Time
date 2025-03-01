# Tv-Time
EN: Application for managing preferences about movies.

PT-BR: Aplicação para gerenciamento de preferências sobre filmes.

## Estrutura do Código
### Estrutura do main.c
🔹 1. Estrutura de Dados
📌 Usuários:
Nome
Senha (PIN de 8 dígitos, numérica)
Indicador de administrador (1 para admin, 0 para usuário comum)
📌 Filmes:
Nome
Duração
Gênero
Ano de lançamento
📌 Filmes assistidos:
Nome do filme
Onde assistiu
Data de visualização

🔹 2. Fluxo do Programa
📌 Cadastro e Login

✅ Sem mudanças, apenas considerar possíveis dados extras no cadastro.

📌 Cadastro de Filmes (Somente para Admins)

1️⃣ Pedir nome, duração, gênero e ano de lançamento.

2️⃣ Adicionar o filme ao arquivo de filmes.

📌 Registrar Filme Assistido

1️⃣ O usuário escolhe um filme da lista.

2️⃣ Ele informa onde assistiu e a data.

3️⃣ O filme é salvo na lista de assistidos.

📌 Listar Meus Filmes Assistidos

1️⃣ O usuário pode ver todos os filmes que assistiu, com data e onde assistiu.

📌 Estatísticas

1️⃣ O programa exibe o tempo total assistido pelo usuário.

3️⃣ Novas Funções Necessárias

✅ cadastrarFilme() → Permite que o admin cadastre um filme.

✅ listarFilmes() → Mostra os filmes cadastrados.

✅ registrarFilmeAssistido() → Permite que um usuário registre um filme assistido.

✅ listarFilmesAssistidos() → Exibe os filmes assistidos.

✅ calcularEstatisticas() → Calcula o tempo total de filmes assistidos.

