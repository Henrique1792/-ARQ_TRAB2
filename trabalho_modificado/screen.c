#include <stdio.h> //função de gravação e leitura
#include <stdlib.h> // funções padrões (malloc por ex)
#include <unistd.h> // utilizado para fazer o system(clear);
#include <string.h> //fgets
#include <ctype.h> // tolower

#include "booktag.h"
#include "debug.h"
#include "screen.h"
#include "idc.h"

// defines
#define DATAFILE_PATH "data.bin"// nome do arquivo de dados utilizado
#define DEBUG 0 //flag de depuracao

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
			int a = verify_index(DATAFILE_PATH);
			if(a == 0) create_index(DATAFILE_PATH);
            return;
        default:
            printf("\n Opção incorreta, digite novamente\n");
            break;
        }
    }
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

    //pegando o titulo do livro
    printf_colorful("\n\nDigite o Titulo do livro ", ANSI_CYAN);
    printf_colorful("(caracteres alfanumericos [A-Z/0-9]): ", ANSI_RED);
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

    printf_colorful("------------------", ANSI_WHITE);
    printf_colorful(" Tela de Cadastro - Digite as informacoes: ", ANSI_WHITE);
    printf_colorful("------------------", ANSI_WHITE);

    //booktag temporaria e contador
    BOOKTAG_T *booktag_temp; //temporario para guardar os dados do input e guardar o usuario
    int cont = 1; //variavel para continuar

    //loop de operacao de cadastro
    while (cont == 1)    {
        booktag_temp = screen_get_input();
        //substituir aqui
        write_booktags(booktag_temp, DATAFILE_PATH);

        int a = verify_index(DATAFILE_PATH);
        if(a == 0){
            FILE *data = fopen(DATAFILE_PATH,"r");
            INDICES_T *idx = (INDICES_T *) malloc(sizeof(INDICES_T));

            idx->author = fopen(IDXSECAUT_PATH,"r+");
            idx->publisher = fopen(IDXSECPUB_PATH,"r+");
            idx->list_aut = fopen(IDXLISTAUT_PATH,"r+");
            idx->list_pub = fopen(IDXLISTPUB_PATH,"r+");

            insert_to_index(idx, booktag_temp, get_offset(data));
        }

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
    printf_colorful("---------------------", ANSI_WHITE);
    printf_colorful(" Tela de Remocao - Escolha as opções ", ANSI_WHITE);
    printf_colorful("---------------------", ANSI_WHITE);

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
        booktag_remove_author();

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
void booktag_search_all_screen() {
    int opt;
    system("clear");
    printf_colorful("---------------------", ANSI_WHITE);
    printf_colorful(" Tela de Listagem - Escolha as opções ", ANSI_WHITE);
    printf_colorful("--------------------", ANSI_WHITE);
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
   Função booktag_search_screen() Tela de procura/pesquisa de booktags
 **/
void booktag_search_screen() {
    printf_colorful("-----------------", ANSI_WHITE);
    printf_colorful(" Tela de Listagem/Busca - Escolha as opções ", ANSI_WHITE);
    printf_colorful("-----------------", ANSI_WHITE);

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
    printf_colorful(" Mostrar conteúdo das listas invertidas",ANSI_BLUE);

    printf_colorful("\n\n8)", ANSI_WHITE);
    printf_colorful(" Mostrar conteúdo dos índices secundários (Autor e Editora)",ANSI_BLUE);

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
        booktag_search_year_screen();

        return;

    case 7: // mostrar conteúdo das listas
    	system("clear");
    	booktag_show_lists();
        return;

    case 8: // mostrar conteúdo dos índices
    	system("clear");
    	booktag_show_index();
        return;

    case 9: // sair
        return;

    default:
        printf_error("\nOpcão inválida\n");
        break;
    }
}


/**
 * Tela de busca pelo campo secundario autor
 **/
void booktag_search_author() {
    printf_colorful("-----------------", ANSI_WHITE);
    printf_colorful(" Tela de Busca por Autor - Escolha as opções ", ANSI_WHITE);
    printf_colorful("-----------------", ANSI_WHITE);
    printf_colorful("\n\n", ANSI_WHITE);
    printf_colorful("Digite o autor que deseja procurar", ANSI_CYAN);
    printf_error("(Caracteres alfanumericos [0-9/A-z]): ");

    char author[BUFFER_MAX];
    getchar();
    scanf("%[^\n]s",author);

    lower_case(author);
    index_search_author(author);

    printf_colorful("\nDigite 1 para voltar ou 2 para fazer outra pesquisa: ", ANSI_YELLOW);

    int op;
    scanf("%d",&op);
    switch(op){
    	case 1:
    		break;
    	case 2:
    		system("clear");
    		booktag_search_author();
    		break;
    	default:
    		break;
    }
}


/**
 * Tela de busca pelo campo secundario Editora
 **/
void booktag_search_publisher() {
    printf_colorful("-----------------", ANSI_WHITE);
    printf_colorful(" Tela de Busca por Editora - Escolha as opções ", ANSI_WHITE);
    printf_colorful("---------------", ANSI_WHITE);
    printf_colorful("\n\n", ANSI_WHITE);
    printf_colorful("Digite a Editora que deseja procurar", ANSI_CYAN);
    printf_error("(Caracteres alfanumericos [0-9/A-z]): ");

    char publisher[BUFFER_MAX];
    getchar();
    scanf("%[^\n]s",publisher);

    lower_case(publisher);
    index_search_publisher(publisher);

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
        printf_colorful(" Tela de Busca por Editora E Editora - Escolha as opções ", ANSI_WHITE);
    else
        printf_colorful(" Tela de Busca por Editora OU Editora - Escolha as opções ", ANSI_WHITE);

    printf_colorful("----------------------", ANSI_WHITE);
    printf_colorful("\n\n", ANSI_WHITE);

    printf_colorful("Digite a Editora que deseja procurar", ANSI_CYAN);
    printf_error("(Caracteres alfanumericos [0-9/A-z]): ");

    char publisher[BUFFER_MAX];
    getchar();
    scanf("%[^\n]s",publisher);

    printf_colorful("Digite o Autor que deseja procurar", ANSI_CYAN);
    printf_error("(Caracteres alfanumericos [0-9/A-z]): ");
    char author[BUFFER_MAX];
    getchar();
    scanf("%[^\n]s",author);



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
	printf_colorful("---------------------", ANSI_WHITE);
    printf_colorful(" Tela de Mostrar Índices Secundários ", ANSI_WHITE);
    printf_colorful("---------------------\n\n", ANSI_WHITE);

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


/**
   Função remove_def_screen() Tela de remoção definitiva de um registro
 **/
void booktag_remove_author() {

    system("clear");
    printf("---------------------- Tela De Remoção - Digite o Autor ----------------------"
           "\n"
           "\n"
           "\n"
        );
    printf_colorful("Digite o autor que deseja procurar para remoção", ANSI_CYAN);
    printf_error("(Caracteres alfanumericos [0-9/A-z]): ");

    char author[BUFFER_MAX];
    getchar();
    scanf("%[^\n]s",author);

    lower_case(author);
    index_remove_author(author);

    printf_colorful("\nDigite 1 para voltar ou 2 para fazer outra remoção: ", ANSI_YELLOW);

    int op;
    scanf("%d",&op);
    switch(op){
        case 1:
            break;
        case 2:
            system("clear");
            booktag_remove_author();
            break;
        default:
            break;
    }

}


/**
   Função booktag_search_year_screen Tela de procura de pesquisa de booktag pelo ano
 **/
void booktag_search_year_screen() {
    printf_colorful("------------------", ANSI_WHITE);
    printf_colorful(" Tela De Busca Por Ano - Digite o ano ", ANSI_WHITE);
    printf_colorful("------------------\n\n", ANSI_WHITE);

    printf_colorful("Digite o ano a ser buscado: ", ANSI_CYAN);

    int year;
    scanf("%d", &year);

    recover_year(DATAFILE_PATH, year);

    printf("\n");
}
