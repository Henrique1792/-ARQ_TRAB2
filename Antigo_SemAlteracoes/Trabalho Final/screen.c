#include <stdio.h> //função de gravação e leitura
#include <stdlib.h> // funções padrões (malloc por ex)
#include <unistd.h> // utilizado para fazer o system(clear);
#include <assert.h> //utilizamos o assert para guarantir que alguns parametros sejam corretos
#include <string.h> //fgets
#include "screen.h"

//arquivos nossos
#include "booktag.h"
#include "debug.h"


/*
   Trabalho de Organizacao de Arquivos - Trabalho 1

   Integrantes:

   Marcos Vinicius Barros L. Andrade Junqueira     numero USP: 8922393
   Rita Raad                                       numero USP: 8061452
   Henrique Fernandes de Matos Freitas             numero USP: 8937225
   Gustavo                                         numero USP: 8937416

   Descricao do arquivo screen.c: implementa as funcoes definidas no arquivo screen.h. Possui as funcoes relativas a
   a interface grafica, manipulacao das opcoes nela e tratamentos.

*/


/**
   Função start_screen() comeca a tela inicial do programa
 **/
void start_screen() {

    //loop principal do programa
    while(1) {
        system("clear"); //limpamos a telax
        printf("---------------------- Tela Inicial - Escolha as opções ----------------------"
               "\n"
               "\n"
               "\n"
               "Menu de opções: \n"
               "\n"
               "1) Menu de Cadastro " "\n\n" "2) Menu de Remoção" "\n\n"
               "3) Menu de Relatorio " "\n\n" "4) Sair"
               "\n"
               "\n"
               "Digite a opção desejada:  "
            );

        int op; // operação do usuário
        scanf("%d", &op);

        switch(op) {
        case 1: // tela de cadastro
            //limpamos e chamamos a tela de inserção
            system("clear");
            insert_screen();

            break;
        case 2: // tela de remocao
            //limpamos e chamamos a tela de remoçãox
            system("clear");
            remove_screen();

            break;
        case 3: // tela de relatorio
            //limpamos e chamamos a tela de relatório/busca
            system("clear");
            booktag_search_screen();

            break;
        case 4: // tela de sair
            //sai e faco a limpeza/checagem do arquivo
            printf("\nSaindo...\n");
            diskrem_booktag(DATAFILE_PATH);
            return;
        default:
            printf("\n Opção incorreta, digite novamente\n");
            break;
        }
    }
}


/**
   Função printf_booktag() que imprime uma booktag

   @param booktag a ser impressa
 **/
void printf_booktag(BOOKTAG_T *booktag) {
    //verificamos se o paramẽtro está ok
    if (booktag == NULL) {
        printf("[ERRO] booktag nula, precisa de paramêtro para impressão");
        return;
    }

    //vemos se o tamanho é zero, se for saimos porque naõ é um registro válido
    if (strlen(booktag->title) == 0) return;

    if(booktag->title[0] == '*'){
        printf("\n----------------------------------------------\n");
        printf("Registro removido");
        printf("\n----------------------------------------------\n");
        sleep(TIME_PRINTF);
        return;
    }
    //imprimos todo o conteúdo da estrutura na tela
    printf("\n----------------------------------------------\n");
    printf("Titulo: %s\n", booktag->title);
    printf("Autor: %s\n", booktag->author);
    printf("Editora: %s\n", booktag->publisher);
    printf("Ano: %d\nPáginas: %d\nPreço: %.2f\n",
           booktag->year, booktag->pages, booktag->price);
    printf("-----------------------------------------------\n");
    sleep(TIME_PRINTF);
}


/**
   Função get_input() pega o input, e alocado a memória assim que necessário
   @return BOOKTAG_T booktag criada com as informações de input
 **/
BOOKTAG_T *screen_get_input() {

    //estrutura temporária para guardar o input do usuário
    BOOKTAG_T *booktag = create_booktag();

    char *BUFFER = NULL; // nossa variavel 'buffer' ou no caso temporária para o input

    // pegamos esse caracter pois alguma sujeira esta vindo do stdin
    // que o fflush(stdin) não consegue pegar.
    getchar();

    // limpamos a string na segunda utilização para garantir que não haja sujeiras
    if (BUFFER) memset(BUFFER,0,strlen(BUFFER));

    //DEBUG: vendo conteudo se o buffer existir

    if(DEBUG && (BUFFER))    printf("TAMANHO: %lu\n", strlen(BUFFER));

    //pegando o titulo do livro
    fprintf(stdout, "Digite o título do livro: ");

    //utilizamos o fgets dentro de uma rotina de logging e checamos o tamanho do input
    do {
        BUFFER = fgets_log(BUFFER, sizeof(BUFFER), stdin);

        if (strlen(BUFFER) > TITLE_MAX) { //verificamos se ele digitou coisa mais
            printf("\nUltrapassou a quantidade máxima de caracteres %d, digite novamente: ", TITLE_MAX);
        } else
            break; //validado

    } while (1);
    // em caso de depuração
    /*if (DEBUG)
        printf("TAMANHO: %lu\n", strlen(booktag->title));*/

    //completamos o restante do espaço que sobrou
    string_complete(BUFFER, 1);

    //copiamos o input
    strncpy(booktag->title, BUFFER, strlen(BUFFER));

    //DEBUG:
    /*if(DEBUG) {
        printf("TAMANHO: %lu\n", strlen(BUFFER));
        printf("TAMANHO: %lu\n", strlen(booktag->title));
    }*/

    //pegamos o autor
    fprintf(stdout, "Digite o autor do livro: ");
    do {
        BUFFER = fgets_log(BUFFER, sizeof(BUFFER), stdin);

        if (strlen(BUFFER) > AUTHOR_MAX) { //verificamos se ele digitou coisa mais
            printf("\nUltrapassou a quantidade máxima de caracteres %d, digite novamente: ", AUTHOR_MAX);
        } else
            break; //validado
    } while (1);

    //completamos o restante do espaço vago no buffer
    string_complete(BUFFER, 2);

    //guardamos
    strncpy(booktag->author, BUFFER, strlen(BUFFER));

    //pegando editora
    fprintf(stdout, "Digite a editora do livro: ");

    do { //validamos
        BUFFER = fgets_log(BUFFER, sizeof(BUFFER), stdin);

        if (strlen(BUFFER) > PUB_MAX) { //verificamos se ele digitou coisa mais
            printf("\nUltrapassou a quantidade máxima de caracteres %d, digite novamente: ", PUB_MAX);
        } else
            break; //validado

    } while(1);

    //completamos o espaço em branco
    string_complete(BUFFER, 3);

    //copiamos para estrutura
    strncpy(booktag->publisher, BUFFER, strlen(BUFFER));

    //pegamos idioma
    fprintf(stdout, "Digite o idioma do livro: ");

    do { //validamos
        BUFFER = fgets_log(BUFFER, sizeof(BUFFER), stdin);

        if (strlen(BUFFER) > LANG_MAX) { //verificamos se ele digitou coisa mais
            printf("\nUltrapassou a quantidade máxima de caracteres %d, digite novamente: ", LANG_MAX);
        } else
            break; //validado
    } while (1);

    //completamos o espaço que sobrou
    string_complete(BUFFER, 4);
    //copiamos
    strncpy(booktag->language, BUFFER, strlen(BUFFER));


    //validamos o ano (sem negativos) e o guardamos
    printf("Digite o ano do livro: ");
    while (1) {
        fgets(BUFFER, sizeof(BUFFER), stdin);

        if (atoi(BUFFER) < 0) {
            printf("O ano não pode ser negativo, digite novamente: ");
        } else
            break;

    }
    booktag->year = atoi(BUFFER);

    // pegamos a qtd paginas
    printf("Digite a quantidade de paginas do livro: ");
    while(1)    {
        fgets(BUFFER, sizeof(BUFFER), stdin);
        if (atoi(BUFFER) < 0) {
            printf("As paginas não podem ser negativo, digite novamente: ");
        } else
            break;
    }
    booktag->pages = atoi(BUFFER);

    // pegamos o preço e o validamos
    printf("Digite o preço: ");

    while (1) {
        fgets(BUFFER, sizeof(BUFFER), stdin);
        if (atoi(BUFFER) < 0) { //preco negativo
            printf("O preco não podem ser negativo, digite novamente: ");
        } else
            break;
    }

    booktag->price = atof(BUFFER);

    //liberamos espaço
    //e retornamos a booktag com as informações
    free(BUFFER);
    BUFFER = NULL;
    return booktag;
}


/**
   Função insert_screen() tela de cadastrado
 **/
void insert_screen() {
    //limpamos a tela
    system("clear");

    //imprimos menu
    printf("---------------------- Tela De Cadastro - Cadastre uma booktag ----------------------"
           "\n"
           "\n"
           "\n"
           "Digite as informações: \n\n"
        );
    //booktag temporaria e contador
    BOOKTAG_T *booktag_temp; //temporario para guardar os dados do input e guardar o usuario
    int cont = 1; //variavel para continuar

    //loop de operacao de cadastro
    while (cont == 1)    {
        booktag_temp = screen_get_input();
        //substituir aqui
        write_booktags(booktag_temp, DATAFILE_PATH, 1);
        printf("\nInformações cadastradas foram: ");
        printf_booktag(booktag_temp);
        printf("\nDigite 1 para inserir mais um cadastro: ");
        scanf("%d", &cont);
        printf("\n\n");
    }

    free(booktag_temp);
    return;
}

/**
   Funcão remove_screen() tela de remocao
 **/
void remove_screen() {
    //limpamos a tela e imprimos a tela de remoção
    system("clear");

    printf("---------------------- Tela De Remoção - Escolha as opções ----------------------"
           "\n"
           "\n"
           "\n"
           "Menu de opcoes: \n"
           "\n"
           "1) Deseja a ver relatorios/busca?  " "\n\n" "2) Continuar com a remoção?\n"
           "\n3) Voltar"
           "\n\n"
           "Digite a opção desejada: " );
    //pegamos a opçaõ digitada
    int op;
    scanf("%d", &op);

    //verificamos qual a opção digitada
    switch(op) {
    case 1: // chama tela de relatorios
        system("clear");
        booktag_search_screen();

        break;
    case 2: // continua com a remocao
        system("clear");
        remove_def_screen();
        break;

    case 3: // sai
        return;

    default:
        printf("\n Opção incorreta");
        break;
    }
}

/**
   Função remove_def_screen() Tela de remoção definitiva de um registro
 **/
void remove_def_screen() {

    system("clear");
    printf("---------------------- Tela De Remoção - Digite o RRN ----------------------"
           "\n"
           "\n"
           "\n"
           " Digite o RRN que deseja remover:"
        );
    fflush(stdin);
    int op;
    scanf("%d", &op); //pegamos rrn do stdin

    printf("\nRRN DIGITADO %d\n", op);

    //chamamos a função de remoção;
    int r = markrem_booktag(DATAFILE_PATH, op);

    if (!r) {
        printf("Registro não encontrado\n");
    }
	
    sleep(10);
}

/**
   Função booktag_search_screen() Tela de procura de pesquisa de booktag
 **/
void booktag_search_screen() {
    printf("---------------------- Tela De Listagem/Busca - Escolha as opcões ----------------------"
           "\n"
           "\n"
           "\n"
           "1) Mostrar todos os registros\n\n"
           "2) Buscar por ano\n\n"
           "3) Buscar por RRN\n\n"
           "4) Voltar\n\n"
           "Digite a opção desejada: "
       );

    int op;
    scanf("%d", &op);

    switch(op) {
    case 1: // lista todos os registros
        system("clear");
        booktag_search_all_screen();
        return;
        break;
    case 2: // chama busca por ano
        system("clear");
        booktag_search_year_screen();

        break;
    case 3: //chama busca por rrn
        system("clear");
        booktag_search_rrn_screen();

        break;
    case 4:
        return;

    default:
        printf("\nOpção errada");
        break;
    }
}

/**
   Função booktag_search_screen() Tela de procura de pesquisa de booktag
 **/
void booktag_search_rrn_screen() {

    printf("---------------------- Tela De Busca Por RRN- Digite a RRN ----------------------"
           "\n"
           "\n"
           "\n\n"
           "Digite a RRN à ser buscado: "
        );
    int rrn;
    scanf("%d", &rrn);
    recover_rrn(DATAFILE_PATH, rrn);
sleep(10);
}

/**
   Função booktag_search_year_screen Tela de procura de pesquisa de booktag pelo ano
 **/
void booktag_search_year_screen() {
    printf("---------------------- Tela De Busca Por Ano - Digite o ano ----------------------"
           "\n"
           "\n"
           "\n\n"
           "Digite o ano à ser buscado: "
        );
    int year;
    scanf("%d", &year);
    recover_year(DATAFILE_PATH, year);
}

/**
   Função booktag_search_all_screen() Tela  de impressaõ de todas booktags
 **/
void booktag_search_all_screen() {
    int opt;

    printf("---------------------- Tela De Listagem - Digite uma opção ----------------------"
           "\n"
           "\n"
           "\n"
           "1) Ver todos os registros\n"
           "2) Ver um registro por vez\n"
           "3) Voltar\n\n"
           "Digite a opção desejada: "
        );
    scanf("%d", &opt);

    switch(opt) {
    case 1: // opcao para ver todos os registros
        read_booktag_list(DATAFILE_PATH);
        break;
    case 2: // opcao para aver um registor por vez
        read_booktag_list_one(DATAFILE_PATH);
        break;
    case 3: // opcao para sair
        return;
    }

}


/**
   Função interna string_complete() que preenche o restante da string para evitar
   problemas de diferenças nos tamanhos

   @param char string[] que será preenchida
   @param int n qual campo será preencido (1 para titulo, para autor, 3 para editora e 4 para idioma)
 **/
void string_complete(char string[], int n) {

    //pegamos o tamanho dela e o contador
    int size = strlen(string);
    int i;

    switch(n) {
    case 1: // titulo
        for(i = size+1; i < TITLE_MAX; i++) { //completamos para o tamanho do título
            string[i] = ' ';
        }
        break;
    case 2: // autor
        for(i = size+1; i < AUTHOR_MAX; i++) { // para o autor
            string[i] = ' ';
        }

        break;
    case 3://editora
        for(i = size+1; i < PUB_MAX; i++) { //para o editor
            string[i] = ' ';
        }
        break;
    case 4://idioma
        for(i = size+1; i < LANG_MAX; i++) { // para o idioma
            string[i] = ' ';
        }
        break;
    default:
        break;
    }

}
