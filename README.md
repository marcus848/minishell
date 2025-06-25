# Minishell - Meu projeto com nota máxima (125/125)

Este repositório apresenta a minha implementação do Minishell, um dos projetos mais desafiadores da 42 School.

O objetivo do projeto é criar um shell simplificado em C, replicando grande parte do comportamento do bash. Esta versão do minishell foi desenvolvida com atenção especial a cada detalhe e recebeu a **nota máxima de 125 pontos na avaliação oficial.**

Apesar de enxuto em proposta, o projeto envolve conceitos avançados como gerenciamento de processos, manipulação de arquivos, tratamento de sinais, expansão de variáveis, pipes, redirecionamentos e a construção de um pipeline completo de tokenização, parsing e execução.

Esse projeto foi desenvolvido em parceria com o colega [caide-so](https://github.com/Caio-de-Souza-Marinho), cuja contribuição foi fundamental em todo o processo. 🙌

## Objetivo Geral

O propósito principal é entender em detalhes como um shell funciona: ler a entrada do usuário, interpretar essa entrada respeitando regras de quoting e expansão, construir uma representação interna (AST) e executar os comandos, lidando corretamente com sinais e códigos de saída. O projeto foi avaliado com nota alta e é um dos mais desafiadores do Common Core.

## Principais Funcionalidades

- **Parsing de entrada** com suporte a aspas simples e duplas
- **Expansão de variáveis de ambiente** (`$VAR`)
- **Expansão de wildcards** (`*`)
- **Redirecionamentos** `>`, `>>`, `<`, `<<` (heredoc)
- **Pipes** (`|`)
- **Operadores lógicos** `&&` e `||`
- **Comandos builtin**: `cd`, `echo`, `pwd`, `exit`, `export`, `unset`, `env`
- **Tratamento de sinais** para `SIGINT` e `SIGQUIT`
- **Gestão de códigos de saída** seguindo o comportamento do bash
- **Suporte a subshells** via parênteses `( )` *(bônus)*

## Estrutura Interna

1. **Tokenizer** – converte a linha digitada em uma lista de tokens, respeitando operadores, palavras e diferentes tipos de aspas.
2. **Parser / AST** – organiza os tokens em uma árvore de sintaxe abstrata (AST). Essa árvore representa comandos simples, pipelines, operadores lógicos e subshells.
3. **Executor** – percorre a AST realizando redirecionamentos, criando pipes e chamando os programas ou builtins apropriados.
4. **Gerenciamento de variáveis de ambiente** – armazena as variáveis em uma lista ligada, permitindo consulta e modificação via `export`, `unset` e leitura durante a expansão.
5. **Heredoc** – arquivos temporários são utilizados para armazenar o conteúdo de `<<`. Durante a leitura, sinais são tratados para cancelar o heredoc corretamente ao receber `CTRL+C`.

### Fluxo de Execução

1. **Leitura da linha** através de `readline`.
2. **Tokenização** da string de entrada.
3. **Parsing** dos tokens, construindo a AST.
4. **Execução** de cada nó da árvore:
   - Criação de processos e pipes quando necessário.
   - Configuração de redirecionamentos.
   - Chamada de comandos externos ou funções builtin.
5. **Atualização do status de saída** para que comandos seguintes possam utilizar `$?`.

Esse fluxo imita o comportamento de um shell real, permitindo combinações como:

```bash
ls -l | grep txt && echo "encontrado" || echo "nada"
```

## Compilação

É necessário ter a biblioteca **readline** instalada. Compile com:

```bash
make
```

O binário gerado será `minishell`.

## Como Executar

Inicie o shell executando:

```bash
./minishell
```

Alguns comandos de exemplo dentro do minishell:

```bash
export PATH=$PATH:/meus/bin
mkdir teste && cd teste
echo "arquivo" > file.txt
cat *.txt | wc -l
```

## Limitações Conhecidas

- O comportamento de algumas expansões pode diferir em casos extremos do bash.
- Não há suporte a recursos avançados como redirecionamentos de arquivo‑descritor (`2>&1`).

## Testes e Valgrind

O projeto foi desenvolvido com foco em ausência de memory leaks. Utilize:

```bash
make leak
```

para rodar o `valgrind` diretamente no binário.

## Contribuidores

Projeto realizado por **Marcus Gomes** e **Caio de Souza Marinho**.

## Licença

Distribuído sob a [MIT License](LICENSE).

