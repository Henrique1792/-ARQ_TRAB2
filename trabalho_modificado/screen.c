#include <stdio.h> //fun��o de grava��o e leitura
#include <stdlib.h> // fun��es padr�es (malloc por ex)
#include <unistd.h> // utilizado para fazer o system(clear);
#include <string.h> //fgets
#include <ctype.h> // tolower

#include "booktag.h"
#include "debug.h"
#include "screen.h"
#include "idc.h"

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

void write_topstack(int *topstack){
	FILE *f = fopen(TOPSTACK_PATH,"r+");

	fwrite(topstack,sizeof(int),1,f);

	fclose(f);
}

void load_topstack(int *topstack){
	FILE *f = fopen(TOPSTACK_PATH,"r");

	if(!f){
		f = fopen(TOPSTACK_PATH,"w+");
		fclose(f);
		write_topstack(topstack);
		return;
	}

	fread(topstack,sizeof(int),1,f);

	fclose(f);
}


/**
   Fun��o start_screen() comeca a tela inicial do programa
 **/
void start_screen() {

    //loop principal do programa
    while(1) {
        system("clear"); //limpamos a telax
        printf_colorful("----------------------", ANSI_WHITE);
        printf_colorful(" Tela Inicial - Escolha as op��es ", ANSI_WHITE);
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

        int op; // opera��o do usu�rio
        scanf("%d", &op);

        switch(op) {
        case 1: // tela de cadastro
            //limpamos e chamamos a tela de inser��o
            system("clear");
            insert_screen();

            break;
        case 2: // tela de remocao
            //limpamos e chamamos a tela de remo��ox
            system("clear");
            remove_screen();

            break;
        case 3: // tela de relatorio
            //limpamos e chamamos a tela de relat�rio/busca
            system("clear");
            booktag_search_screen();

            break;
        case 4: // tela de sair
            //sai e faco a limpeza/checagem do arquivo
            printf("\nSaindo...\n");
			int a = verify_index(DATAFILE_PATH);
			if(a == 0) create_index(DATAFILE_PATH);
            return;
        default:
            printf("\n Op��o incorreta, digite novamente\n");
            break;
        }
    }
}



/**
   Fun��o get_input() pega o input, e alocado a mem�ria assim que necess�rio
   @return BOOKTAG_T booktag criada com as informa��es de input
 **/
BOOKTAG_T *screen_get_input() {

    //estrutura tempor�ria para guardar o input do usu�rio
    BOOKTAG_T *booktag = create_booktag();

    char *BUFFER = malloc(sizeof(char) * BUFFER_MAX);


    fflush(stdin);

    //pegando o titulo do livro
    printf_colorful("\n\nDigite o Titulo do livro ", ANSI_CYAN);
    printf_colorful("(caracteres alfanumericos [A-Z/0-9]): ", ANSI_RED);
//    BUFFER = getline_input(); //fgets_log(BUFFER, sizeof BUFFER, stdin);
    scanf(" %[^\n]s",BUFFER);

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

    scanf(" %[^\n]s",BUFFER);
//    fgets(BUFFER, sizeof BUFFER, stdin);

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
    scanf(" %[^\n]s",BUFFER);
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
    scanf(" %[^\n]s",BUFFER);

    if (DEBUG) {
        printf_debug("\tDEBUG: digitado ");
        printf_debug(BUFFER);
    }

    booktag->language = malloc(sizeof(char) * strlen(BUFFER));
    lower_case(BUFFER);
    strcpy(booktag->language, BUFFER);

    fflush(stdin);
    getchar();

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
            printf_error("\nAs paginas n�o podem ser negativas, digite novamente: ");
        } else
            break;
    }
    booktag->pages = atof(BUFFER);
    if (DEBUG) printf_debug("\tDEBUG: digitado %d\n", booktag->pages);


    fflush(stdin);

    // pegamos o pre�o e o validamos
    printf_colorful("\nDigite o preco do livro:  ", ANSI_CYAN);
    printf_colorful("(caracteres numericos e ponto flutuante[0.0-9.9]): ", ANSI_RED);
    while (1) {
        fgets(BUFFER, sizeof BUFFER, stdin);
        if (atof(BUFFER) < 0) { //preco negativo
            printf_error("O preco n�o podem ser negativo, digite novamente: ");
        } else
            break;
    }
    booktag->price = atof(BUFFER);
    if (DEBUG) printf_debug("\tDEBUG: digitado %d\n", booktag->price);

    //liberamos espa�o
    //e retornamos a booktag com as informca��es
    free(BUFFER);
    return booktag;
}


/**
   Fun��o insert_screen() tela de cadastrado
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

        printf_debug("\nInforma��es cadastradas foram: ");
        printf_booktag(booktag_temp);
        printf_debug("\nDigite 1 para inserir mais um cadastro: ");
        scanf("%d", &cont);
        printf("\n\n");
    }

    free(booktag_temp);
    return;
}

/**
   Func�o remove_screen() tela de remocao
 **/
void remove_screen() {
    //limpamos a tela e imprimos a tela de remo��o
    system("clear");
    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful(" Tela de Remocao - Escolha as op��es ", ANSI_WHITE);
    printf_colorful("----------------------", ANSI_WHITE);

    printf_colorful("\n\n1)", ANSI_WHITE);
    printf_colorful(" Remover Registro", ANSI_BLUE);
    printf_colorful("\n\n2)", ANSI_WHITE);
    printf_colorful(" Menu de Relatorios", ANSI_BLUE);
    printf_colorful("\n\n3)", ANSI_WHITE);
    printf_colorful(" Voltar",ANSI_BLUE);
    printf_colorful("\n\nDigite a opcao desejada: ", ANSI_YELLOW);

    //pegamos a op�a� digitada
    int op;
    scanf("%d", &op);

    //verificamos qual a op��o digitada
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
        printf_error("\n Op��o incorreta");
        break;
    }
}

/**
   Fun��o booktag_search_all_screen() Tela  de impressa� de todas booktags
 **/

void booktag_search_all_screen() {
    int opt;
    system("clear");
    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful(" Tela de Listagem - Escolha as op��es ", ANSI_WHITE);
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
        system("clear");
        break;
    case 2: // opcao para aver um registor por vez
        booktag_search_list_one();
        break;
    case 3: // opcao para sair
        return;
    }

}

/**
   Fun��o booktag_search_screen() Tela de procura/pesquisa de booktags
 **/
void booktag_search_screen() {
    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful(" Tela de Listagem/Busca - Escolha as op��es ", ANSI_WHITE);
    printf_colorful("----------------------", ANSI_WHITE);

    printf_colorful("\n\n1)", ANSI_WHITE);
    printf_colorful(" Mostra todos os registros", ANSI_BLUE);

    printf_colorful("\n\n2)", ANSI_WHITE);
    printf_colorful(" Buscar por Editora", ANSI_BLUE);

    printf_colorful("\n\n3)", ANSI_WHITE);
    printf_colorful(" Buscar por Autor",ANSI_BLUE);

    printf_colorful("\n\n4)", ANSI_WHITE);
    printf_colorful(" Buscar por Autor E Editora(NAO IMPLEMENTADO)",ANSI_BLUE);

    printf_colorful("\n\n5)", ANSI_WHITE);
    printf_colorful(" Buscar por Autor OU Editora(NAO IMPLEMENTADO)",ANSI_BLUE);

    printf_colorful("\n\n6)", ANSI_WHITE);
    printf_colorful(" Buscar por Ano",ANSI_BLUE);

    printf_colorful("\n\n7)", ANSI_WHITE);
    printf_colorful(" Mostrar conte�do das listas invertidas",ANSI_BLUE);

    printf_colorful("\n\n8)", ANSI_WHITE);
    printf_colorful(" Mostrar conte�do dos �ndices secund�rios (Autor e Editora)",ANSI_BLUE);

    printf_colorful("\n\n9)", ANSI_WHITE);
    printf_colorful(" Voltar",ANSI_BLUE);

    printf_colorful("\n\nDigite a opcao desejada: ", ANSI_YELLOW);



    int op;
    scanf("%d", &op);

    switch(op) {
    case 1: // lista todos os registros
        system("clear");
        booktag_search_all_screen();

        return;
        break;
    case 2: // busca por editora
        system("clear");
        booktag_search_publisher();

        break;
    case 3: //busca por autor
        system("clear");
        booktag_search_author();

        break;
    case 4: // busca por autor E editora
        system("clear");
        booktag_search_publisher_author(1);

        break;
    case 5: // busca por autor OU editora
        system("clear");
        booktag_search_publisher_author(2);

        break;
    case 6: // pesquisar por ano
    	system("clear");

        return;

    case 7: // mostrar conte�do das listas
    	system("clear");
    	booktag_show_lists();
        return;

    case 8: // mostrar conte�do dos �ndices
    	system("clear");
    	booktag_show_index();
        return;

    case 9: // sair
        return;

    default:
        printf_error("\nOpc�o inv�lida\n");
        break;
    }
}
/**
 * Tela de busca pelo campo secundario autor
 **/
void booktag_search_author() {
    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful(" Tela de Busca por Autor - Escolha as op��es ", ANSI_WHITE);
    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful("\n\n", ANSI_WHITE);
    printf_colorful("Digite o autor que deseja procurar", ANSI_CYAN);
    printf_error("(Caracteres alfanumericos [0-9/A-z]): ");

    char author[BUFFER_MAX];
    scanf("%s", author);

    lower_case(author);
    //index_search_author(author);

    printf_colorful("\nDigite 1 para voltar ou 2 para fazer outra pesquisa: ", ANSI_YELLOW);

    int op;
    scanf("%d",&op);
    switch(op){
    	case 1:
    		break;
    	case 2:
    		system("clear");
    		booktag_search_publisher();
    		break;
    	default:
    		break;
    }
}



/**
 * Tela de busca pelo campo secundario Editora
 **/
void booktag_search_publisher() {
    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful(" Tela de Busca por Editora - Escolha as op��es ", ANSI_WHITE);
    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful("\n\n", ANSI_WHITE);
    printf_colorful("Digite a Editora que deseja procurar", ANSI_CYAN);
    printf_error("(Caracteres alfanumericos [0-9/A-z]): ");

    char publisher[BUFFER_MAX];
    scanf("%s", publisher);

    lower_case(publisher);
//    index_search_publisher(publisher);

    printf_colorful("\nDigite 1 para voltar ou 2 para fazer outra pesquisa: ", ANSI_YELLOW);

    int op;
    scanf("%d",&op);
    switch(op){
    	case 1:
    		break;
    	case 2:
    		system("clear");
    		booktag_search_publisher();
    		break;
    	default:
    		break;
    }
}


void booktag_search_publisher_author(int i) {
    printf_colorful("----------------------", ANSI_WHITE);

    if (i == 1)
        printf_colorful(" Tela de Busca por Editora E Editora - Escolha as op��es ", ANSI_WHITE);
    else
        printf_colorful(" Tela de Busca por Editora OU Editora - Escolha as op��es ", ANSI_WHITE);

    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful("\n\n", ANSI_WHITE);

    printf_colorful("Digite a Editora que deseja procurar", ANSI_CYAN);
    printf_error("(Caracteres alfanumericos [0-9/A-z]): ");
    char publisher[BUFFER_MAX];
    scanf("%s", publisher);

    printf_colorful("Digite o Autor que deseja procurar", ANSI_CYAN);
    printf_error("(Caracteres alfanumericos [0-9/A-z]): ");
    char author[BUFFER_MAX];
    scanf("%s", author);



    //
    // TODO: adicionar funcao que busca por editora e/ou digitado
    // olha o indice secundario e mostra o que tiver com o autor digitado
    //

    //verifica qual o valor do parametro i e chama as funcoes correspondentes
    //(1 para matching ou 2 para merging)
}

void booktag_show_lists() {
	printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful(" Tela de Mostrar Listas Invertidas ", ANSI_WHITE);
    printf_colorful("----------------------\n\n", ANSI_WHITE);

	index_show_lists();

	printf_colorful("\nDigite 1 para voltar ou 2 para mostrar de novo: ", ANSI_YELLOW);

    int op;
    scanf("%d",&op);
    switch(op){
    	case 1:
    		break;
    	case 2:
    		system("clear");
    		booktag_show_lists();
    		break;
    	default:
    		break;
    }
}

void booktag_show_index() {
	printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful(" Tela de Mostrar �ndices Secund�rios ", ANSI_WHITE);
    printf_colorful("----------------------\n\n", ANSI_WHITE);

	index_show_index();

	printf_colorful("\nDigite 1 para voltar ou 2 para mostrar de novo: ", ANSI_YELLOW);

    int op;
    scanf("%d",&op);
    switch(op){
    	case 1:
    		break;
    	case 2:
    		system("clear");
    		booktag_show_index();
    		break;
    	default:
    		break;
    }
}

/**
 * Funcao booktag_search_list_one() que mostra um registro por vez
 **/
void booktag_search_list_one() {

    read_booktag_list_one(DATAFILE_PATH);

}

////////////////////////////////////////// Funcoes que precisam ser alteradas /////////// /////////// /////////// /////////// /////////// /////////// /////////// //////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
   Fun��o remove_def_screen() Tela de remo��o definitiva de um registro
 **/
void remove_def_screen() {
	static int topstack = -1; //inicializa e mant�m o valor de topstack para cada itera��o
	load_topstack(&topstack);

    system("clear");
    printf("---------------------- Tela De Remo��o - Digite o Autor ----------------------"
           "\n"
           "\n"
           "\n"
           " Digite o nome do autor:"
        );
    char *input;

    //
    // inserir rotina de ler o input
    //

    //chamamos a fun��o de remo��o;
    int r = markrem_booktag(DATAFILE_PATH, input, topstack);

    if (!r) {
        printf("Registro n�o encontrado\n");
    }

//    sleep(10);
}




/**
   Fun��o booktag_search_year_screen Tela de procura de pesquisa de booktag pelo ano
 **/
void booktag_search_year_screen() {
    printf("---------------------- Tela De Busca Por Ano - Digite o ano ----------------------"
           "\n"
           "\n"
           "\n\n"
           "Digite o ano a ser buscado: "
        );
    int year;
    scanf("%d", &year);
    //
    // TODO:
    // adicoinar funcao de recuperar ano nova
    //
    //    recover_year(DATAFILE_PATH, year);
}
