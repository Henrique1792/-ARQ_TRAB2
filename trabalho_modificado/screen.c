#include <stdio.h> //função de gravação e leitura
#include <stdlib.h> // funções padrões (malloc por ex)
#include <unistd.h> // utilizado para fazer o system(clear);
#include <string.h> //fgets
#include <ctype.h> // tolower

#include "booktag.h"
#include "debug.h"
#include "screen.h"

// defines
#define DATAFILE_PATH "data.bin"// nome do arquivo de dados utilizado
#define DEBUG 1 //flag de depuracao

/*
   Trabalho de Organizacao de Arquivos - Trabalho 2

   Integrantes:

   Marcos Vinicius Barros L. Andrade Junqueira     numero USP: 8922393
   Rita Raad                                       numero USP: 8061452
   Henrique Fernandes de Matos Freitas             numero USP: 8937225
   Gustavo Santiago                                numero USP: 8937416

   Descricao do arquivo screen.c: implementa as funcoes definidas no arquivo screen.h. Possui as funcoes relativas a
   a interface grafica, manipulacao das opcoes nela e tratamentos.

*/

//
// funcao que deixa a string para lower
// modifica por referencia
//
static void lower_case(char *string) {

    int i;
    int tam = strlen(string);
    for(i = 0; i < tam; i++) {
        string[i] = (char)tolower(string[i]);
    }
}



/**
   Função start_screen() comeca a tela inicial do programa
 **/
void start_screen() {

    //loop principal do programa
    while(1) {
        system("clear"); //limpamos a telax
        printf_colorful("----------------------", ANSI_WHITE);
        printf_colorful(" Tela Inicial - Escolha as opções ", ANSI_WHITE);
        printf_colorful("----------------------", ANSI_WHITE);

        printf_colorful("\n\n1)", ANSI_WHITE);
        printf_colorful(" Menu de Cadastro", ANSI_BLUE);
        printf_colorful("\n\n2)", ANSI_WHITE);
        printf_colorful(" Menu de Remocao", ANSI_BLUE);
        printf_colorful("\n\n3)", ANSI_WHITE);
        printf_colorful(" Menu de Relatorio",ANSI_BLUE);
        printf_colorful("\n\n4)", ANSI_WHITE);
        printf_colorful(" Sair", ANSI_BLUE);
        printf_colorful("\n\nDigite a opcao desejada: ", ANSI_YELLOW);

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


    if (DEBUG) {
        printf_debug("\n\tDEBUG: imprimindo a estrutura: \n");
    }


    if (booktag == NULL) {
        printf_error("[ERRO] booktag nula, precisa de paramêtro para impressão");
        return;
    }

    //vemos se o tamanho é zero, se for saimos porque naõ é um registro válido
    if (strlen(booktag->title) == 0 || booktag->title == NULL) return;
    if (booktag->price == 0 || booktag->pages == 0 || booktag->price == 0) return;

    if(booktag->title[0] == '*'){
        printf_separator();
        printf_debug("Registro removido");
        printf_separator();
        printf_separator();
        return;
    }
    printf_separator();

    printf_colorful("Titulo: ", ANSI_CYAN);
    printf("%s", booktag->title);

    printf_colorful("Autor: ", ANSI_CYAN);
    printf("%s", booktag->author);

    printf_colorful("Editora: ", ANSI_CYAN);
    printf("%s\n", booktag->publisher);

    printf_colorful("Idioma: ", ANSI_CYAN);
    printf("%s", booktag->language);

    printf_colorful("Ano: ", ANSI_CYAN);
    printf("%d\n", booktag->year);

    printf_colorful("Paginas: ", ANSI_CYAN);
    printf("%d\n", booktag->pages);

    printf_colorful("Preco:: ", ANSI_CYAN);
    if (booktag->price != 0) printf("%0.2f\n", booktag->price);

    printf_separator();
}


/**
   Função get_input() pega o input, e alocado a memória assim que necessário
   @return BOOKTAG_T booktag criada com as informações de input
 **/
BOOKTAG_T *screen_get_input() {

    //estrutura temporária para guardar o input do usuário
    BOOKTAG_T *booktag = create_booktag();

    char *BUFFER = malloc(sizeof(char) * BUFFER_MAX);

    fflush(stdin);
    getchar();

    //pegando o titulo do livro
    printf_colorful("\n\nDigite o Titulo do livro ", ANSI_CYAN);
    printf_colorful("(caracteres alfanumericos [A-Z/0-9]): ", ANSI_RED);
    fgets(BUFFER, sizeof BUFFER, stdin);
    if (DEBUG) {
        printf_debug("\tDEBUG: digitado ");
        printf_debug(BUFFER);
    }

    booktag->title = malloc(sizeof(char) * strlen(BUFFER));
    lower_case(BUFFER);
    strcpy(booktag->title, BUFFER);
    fflush(stdin);

    //pegamos o autor
    printf_colorful("\nDigite o Autor do livro ", ANSI_CYAN);
    printf_colorful("(caracteres alfanumericos [A-Z/0-9]): ", ANSI_RED);

    fgets(BUFFER, sizeof BUFFER, stdin);
    if (DEBUG) {
        printf_debug("\tDEBUG: digitado ");
        printf_debug(BUFFER);
    }

    booktag->author = malloc(sizeof(char) * strlen(BUFFER));
    lower_case(BUFFER);
    strcpy(booktag->author, BUFFER);


    fflush(stdin);

    //pegamos o autor
    printf_colorful("\nDigite a Editora do livro ", ANSI_CYAN);
    printf_colorful("(caracteres alfanumericos [A-Z/0-9]): ", ANSI_RED);
    fgets(BUFFER, sizeof BUFFER, stdin);
    if (DEBUG) {
        printf_debug("\tDEBUG: digitado ");
        printf_debug(BUFFER);
    }

    booktag->publisher = malloc(sizeof(char) * strlen(BUFFER));
    lower_case(BUFFER);
    strcpy(booktag->publisher, BUFFER);

    fflush(stdin);
    getchar();
    //pegamos o autor
    printf_colorful("\n\nDigite o Idioma do livro ", ANSI_CYAN);
    printf_colorful("(caracteres alfanumericos [A-Z/0-9]): ", ANSI_RED);

    fgets(BUFFER, sizeof BUFFER, stdin);
    if (DEBUG) {
        printf_debug("\tDEBUG: digitado ");
        printf_debug(BUFFER);
    }

    booktag->language = malloc(sizeof(char) * strlen(BUFFER));
    lower_case(BUFFER);
    strcpy(booktag->language, BUFFER);

    //validamos o ano (sem negativos) e o guardamos
    printf_colorful("\nDigite o ano do livro ", ANSI_CYAN);
    printf_colorful("(caracteres numericos e inteiros[0-9]): ", ANSI_RED);
    while (1) {
        fgets(BUFFER, sizeof BUFFER, stdin);
        if (atoi(BUFFER) < 0) {
            printf_error("\nO ano nao pode ser negativo, digite novamente: \n");
        } else
            break;
    }
    booktag->year = atoi(BUFFER);
    if (DEBUG) printf_debug("\tDEBUG: digitado %d\n", booktag->year);
    fflush(stdin);

    // pegamos a qtd paginas
    printf_colorful("\nDigite o numero de paginas do livro ", ANSI_CYAN);
    printf_colorful("(caracteres numericos e inteiros[0-9]): ", ANSI_RED);
    while(1)    {
        fgets(BUFFER, sizeof BUFFER, stdin);
        if (atof(BUFFER) < 0) {
            printf_error("\nAs paginas não podem ser negativas, digite novamente: ");
        } else
            break;
    }
    booktag->pages = atof(BUFFER);
    if (DEBUG) printf_debug("\tDEBUG: digitado %d\n", booktag->pages);


    fflush(stdin);

    // pegamos o preço e o validamos
    printf_colorful("\nDigite o preco do livro:  ", ANSI_CYAN);
    printf_colorful("(caracteres numericos e ponto flutuante[0.0-9.9]): ", ANSI_RED);
    while (1) {
        fgets(BUFFER, sizeof BUFFER, stdin);
        if (atof(BUFFER) < 0) { //preco negativo
            printf_error("O preco não podem ser negativo, digite novamente: ");
        } else
            break;
    }
    booktag->price = atof(BUFFER);
    if (DEBUG) printf_debug("\tDEBUG: digitado %d\n", booktag->price);

    //liberamos espaço
    //e retornamos a booktag com as informcações
    free(BUFFER);
    return booktag;
}


/**
   Função insert_screen() tela de cadastrado
 **/
void insert_screen() {
    //limpamos a tela
    system("clear");

    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful(" Tela de Cadastro - Digite as informacoes: ", ANSI_WHITE);
    printf_colorful("----------------------", ANSI_WHITE);

    //booktag temporaria e contador
    BOOKTAG_T *booktag_temp; //temporario para guardar os dados do input e guardar o usuario
    int cont = 1; //variavel para continuar

    //loop de operacao de cadastro
    while (cont == 1)    {
        booktag_temp = screen_get_input();
        //substituir aqui
        write_booktags(booktag_temp, DATAFILE_PATH);

        printf_debug("\nInformações cadastradas foram: ");
        printf_booktag(booktag_temp);
        printf_debug("\nDigite 1 para inserir mais um cadastro: ");
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
    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful(" Tela de Remocao - Escolha as opções ", ANSI_WHITE);
    printf_colorful("----------------------", ANSI_WHITE);

    printf_colorful("\n\n1)", ANSI_WHITE);
    printf_colorful(" Remover Registro", ANSI_BLUE);
    printf_colorful("\n\n2)", ANSI_WHITE);
    printf_colorful(" Menu de Relatorios", ANSI_BLUE);
    printf_colorful("\n\n3)", ANSI_WHITE);
    printf_colorful(" Voltar",ANSI_BLUE);
    printf_colorful("\n\nDigite a opcao desejada: ", ANSI_YELLOW);

    //pegamos a opçaõ digitada
    int op;
    scanf("%d", &op);

    //verificamos qual a opção digitada
    switch(op) {
    case 1:
        system("clear");
        remove_def_screen();

        break;
    case 2:
        system("clear");
        booktag_search_screen();

        break;
    case 3: // sai
        return;

    default:
        printf_error("\n Opção incorreta");
        break;
    }
}

/**
   Função booktag_search_all_screen() Tela  de impressaõ de todas booktags
 **/




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// Funcoes que precisam ser alteradas /////////// /////////// /////////// /////////// /////////// /////////// /////////// //////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
   Função booktag_search_screen() Tela de procura/pesquisa de booktags
 **/
//
// TODO: ver as buscas que estao sendo pedidas no pdf e fazer as as telas para elas
//
void booktag_search_screen() {
    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful(" Tela de Listagem/Busca - Escolha as opções ", ANSI_WHITE);
    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful("\n\n1)", ANSI_WHITE);
    printf_colorful(" Mostra todos os registros", ANSI_BLUE);
    printf_colorful("\n\n2)", ANSI_WHITE);
    printf_colorful(" Buscar por Editora", ANSI_BLUE);
    printf_colorful("\n\n3)", ANSI_WHITE);
    printf_colorful(" Buscar por Autor",ANSI_BLUE);
    printf_colorful("\n\n4)", ANSI_WHITE);
    printf_colorful(" Voltar",ANSI_BLUE);

    printf_colorful("\n\nDigite a opcao desejada: ", ANSI_YELLOW);

    int op;
    scanf("%d", &op);

    switch(op) {
    case 1: // lista todos os registros
        system("clear");
        booktag_search_all_screen();
        printf_error("oi");
        return;
        break;
    case 2: // chama busca por ano
        system("clear");
        booktag_search_year_screen();

        break;
    case 3: //chama busca por rrn

        break;
    case 4: // sia
        return;

    default:
        printf_error("\nOpcao errada\n");
        break;
    }
}
void booktag_search_all_screen() {
    int opt;
    system("clear");
    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful(" Tela de Listagem - Escolha as opções ", ANSI_WHITE);
    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful("\n\n1)", ANSI_WHITE);
    printf_colorful(" Ver todos Registros", ANSI_BLUE);
    printf_colorful("\n\n2)", ANSI_WHITE);
    printf_colorful(" Ver um registro por vez", ANSI_BLUE);
    printf_colorful("\n\n3)", ANSI_WHITE);
    printf_colorful(" Voltar",ANSI_BLUE);
    printf_colorful("\n\nDigite a opcao desejada: ", ANSI_YELLOW);

    scanf("%d", &opt);

    switch(opt) {
    case 1: // opcao para ver todos os registros
        read_booktag_list(DATAFILE_PATH);
        break;
    case 2: // opcao para aver um registor por vez
        //
        // TODO:  inserir nova funcao que imprime na tela um registro por vez
        //

        break;
    case 3: // opcao para sair
        return;
    }

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
    //
    // TODO:
    // adicoinar funcao de recuperar ano nova
    //
    //    recover_year(DATAFILE_PATH, year);
}
