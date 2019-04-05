#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

#define Q_Simbolos_especiais 20
char reservadas[15][10] = {{'p', 'r', 'o', 'g', 'r', 'a', 'm', '\0'},{'v', 'a', 'r', '\0'},{'p','r','o','c','e','d','u','r','e', '\0'},{'f','u','n','c','t','i','o','n', '\0'},{'b','e','g','i','n', '\0'},{'e','n','d', '\0'},{'i','f','\0'},{'t','h','e','n', '\0'},{'e','l','s','e', '\0'},{'w','h','i','l','e', '\0'},{'d','o', '\0'},{'o','r','\0'},{'a','n','d', '\0'},{'d','i','v', '\0'},{'n','o','t', '\0'}};
char ignorar[20] = {' ', '\t', '\b', 10, -1};
char especiais[20] = {'.', ';', ',', '(', ')', ':','=', '<', '>', '+', '-', '*', '/', '[',']'};
char alfabeto[52] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm','n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'x', 'w','y', 'z','A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M','N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'X', 'W','Y', 'Z'};
char digitos[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char proximo = 'a';
char stringAuxiliar[30];

void gotoxy( int x, int y )
{
   COORD coord;
   coord.X = (short)x;
   coord.Y = (short)y;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


// Criando uma lista encadeada para colocar os lexemas gerados
typedef struct reg *no;
struct reg {
	char lexema[20];
	struct reg *prox; 
};

no simbolo;

void cria_lista (no *lista) {
 	*lista=NULL;
}

void inclui_final (no *lista, char lexema[]){
	
	 no p = (no) malloc(sizeof(struct reg));
	 strcpy (p->lexema,lexema);
	 p->prox=NULL;
	 if (*lista==NULL)
	 	*lista = p;
	 else {
		 no q = *lista;
		 while (q->prox)
		 	q = q->prox;
		 q->prox = p;
	 }
}

void mostra_lista (no lista) {
	 no p = lista;
	 printf ("\nElementos da lista: ");
	 while (p) {
	 	printf ("  %s  ",p->lexema);
	 	p = p->prox;
	 }
}

void Proximo(int *linha, FILE *program) {
	char c = getc(program); 
	if(c == '\n'){ // fim da linha 
		(*linha)++;		
	}
	proximo = c;
}

void Codigo(int tipo, char s[]) {
	switch (tipo)
	{
		case 0: 
			// Infelizmente tive que fazer esse monte de if, else com o strcmp
			strcpy(stringAuxiliar,"c_"); 
			if(!strcmp(s,".")){
				strcat(stringAuxiliar,"ponto"); 
			}else if(!strcmp(s,";")){
				strcat(stringAuxiliar,"pontoEvirgula"); 
			}else if(!strcmp(s,",")){
				strcat(stringAuxiliar,"virgula"); 
			}else if(!strcmp(s,"(")){
				strcat(stringAuxiliar,"abreParanteses"); 
			}else if(!strcmp(s,")")){
				strcat(stringAuxiliar,"fechaParanteses"); 
			}else if(!strcmp(s,":")){
				strcat(stringAuxiliar,"doisPontos"); 
			}else if(!strcmp(s,"=")){
				strcat(stringAuxiliar,"igual"); 
			}else if(!strcmp(s,"<")){
				strcat(stringAuxiliar,"menor"); 
			}else if(!strcmp(s,">")){
				strcat(stringAuxiliar,"maior"); 
			}else if(!strcmp(s,"+")){
				strcat(stringAuxiliar,"adicao"); 
			}else if(!strcmp(s,"-")){
				strcat(stringAuxiliar,"subtracao"); 
			}else if(!strcmp(s,"*")){
				strcat(stringAuxiliar,"multiplicacao"); 
			}else if(!strcmp(s,":=")){
				strcat(stringAuxiliar,"atribui"); 
			}else if(!strcmp(s,"..")){
				strcat(stringAuxiliar,"pontoPonto"); 
			}else if(!strcmp(s,"/")){
				strcat(stringAuxiliar,"divisao"); 
			}else if(!strcmp(s,"[")){
				strcat(stringAuxiliar,"abreColchete"); 
			}else if(!strcmp(s,"]")){
				strcat(stringAuxiliar,"fechaColchete"); 
			}
			break;
		case 1:
			strcpy(stringAuxiliar, "c_"); 
			strcat(stringAuxiliar, s);
			break;
		case 2: 
			strcpy(stringAuxiliar, "id_"); 
			strcat(stringAuxiliar, s);
			break;
		case 3: 
			strcpy(stringAuxiliar, "c_inteiro_"); 
			strcat(stringAuxiliar, s);
			break; 	
		default:
			break;
	}
}

void Erro(int tipoErro, int *erros, int linha, char car) {
	printf("\n\t\t ERRO -> Erro econtrado na linha %d\n",(linha)+1);	
	printf("\n\t\t");
	if(tipoErro == 1){
		printf(" Simbolo desconhecido!: %c", car);
	}else {
		printf("Valor inválido!");
	}
	(*erros)++; 
	//(*quantidade)++;
}

int Especial(char c, char vetor[]){
	for (int i = 0; i < Q_Simbolos_especiais; i++ ) {
		if(c == vetor[i]){
			return 1; 
		}
	}
	return 0;
}

int Letra(char c, char vetor[]){
	for (int i = 0; i < 52; i++ ) {
		if(c == vetor[i]){
			return 1; 
		}
	}
	return 0;
}

int Digito(char c, char vetor[]){

	for (int i = 0; i < 10; i++ ) {
		if(c == vetor[i]){
			return 1; 
		}
	}
	return 0;
}

int PalavraReservada(char palavra[], char palavras[][10]){
	for(int i=0; i<15; i++){
		if(!strcmp(palavra, palavras[i])){
			return 1;
		}
	}
	return 0;
}

int Ignorar(char c, char ig[]){
	for (int i = 0; i < 5; i++ ) {
		if(c == ig[i]){
			return 1; 
		}
	}
	return 0;
}

void EscreveLexemaNoArquivo(FILE *result, no lista){
	no p = lista;
	int i = 0; // contador de lexemas por linha
	 while (p) {
		fputs(p->lexema, result); 
		fputs(", ",result);
	 	if(i == 5){
			 putc('\n',result);
			 i = 0; 
		 }
		 	
	 	p = p->prox;
		 i++; 
	 }	
}

int menu(){
	system("cls");
	int opcao = 0;
	printf("\n\t\t Trabalho de Compiladores - Analisador Lexico"); 
	printf("\n\t\tDesenvolvido por Leonardo Oliveira - 171025903");
	printf("\n\n\tEscolha uma opcao: "); 
	printf("\n\n\t1 - Rodar o arquivo exemplo (mesma pasta que esse programa)"); 
	printf("\n\t2 - Digitar um nome de outro arquivo (OBS: O novo arquivo tem que estar na mesma pasta que o .exe)"); 
	printf("\n\t3 - Fechar o programa"); 
	printf("\n\n\tOpcao: "); 

	
	do{
		gotoxy(20, 10);
		printf("   ");
		gotoxy(20, 10);
		scanf("%d",&opcao); 
	}while(opcao < 1 || opcao > 3);
	return opcao; 
}

void AnalisadorLexico(int *linha, FILE *program, no *simbolos, int *erros){
	char atomo[30];		// atomo encontrado 
	int foilido = 0;
	int fimDoWhile = 1; 
	//Proximo(&(*linha), program);
	while(proximo != EOF && fimDoWhile){ //
		if(foilido == 0) { 
			Proximo(&(*linha), program);
		}else {	
			foilido = 0; 
		} 		
		if(Ignorar(proximo, ignorar)){	// Espaço, tab, \n são ignorados 
			//Proximo(&(*linha), program);
			continue; 
		}else {
			//Proximo(&(*linha), program);
			if(Especial(proximo, especiais)){
				char s[2] = {' ','\0'};
				s[0] = proximo; 
				Proximo(&(*linha), program);
				switch (s[0]) {
					case ':':
						if(proximo == '=') {
							strcpy(s, ":=");
							foilido = 0;	// já foi lido e pode chamar o proximo 
						}
						break;
					case '.': 
						if(proximo == '.'){
							strcpy(s, ".."); 
							foilido = 0; // já foi lido e pode chamar o proximo 
						} else {
							proximo = '.';
							fimDoWhile = 0; 	//Encerra o loop pois encontrou um ponto sozinho
							continue;
						}
						break; 	
					case '/': 
						if(proximo == '/'){
							while(proximo != '\n'){
								Proximo(&(*linha), program); 
							}
							foilido = 0; 
							continue;
						}else foilido = 1; // já foi lido e pode chamar o proximo 	
						break; 
					case '(': 
						if(proximo == '*'){
							while(proximo != '\n');{
								Proximo(&(*linha), program); 
							}
							foilido = 0; 
							continue; 
						} else foilido = 1; // já foi lido e pode chamar o proximo 						
						break; 
					default:
						foilido = 1;
						break;
				}
				// Adiciona na lista encadeada o s 
				Codigo(0, s);
				inclui_final(simbolos, stringAuxiliar);
			} else {
				if(Letra(proximo, alfabeto)){
					strcpy(atomo, "\0");
					while(Letra(proximo, alfabeto) || Digito(proximo, digitos)){
						strcat(atomo, &proximo);
						Proximo(&(*linha), program);
					}
					if(PalavraReservada(atomo, reservadas)){
						Codigo(1, atomo); 
						inclui_final(&(*simbolos), stringAuxiliar); 
					}else {
						Codigo(2, atomo); 
						inclui_final(&(*simbolos), stringAuxiliar);
					};
					foilido=1; 
				} else {
					if(Digito(proximo, digitos)){
						strcpy(atomo, "\0"); 
						while(Digito(proximo, digitos) == 1){
							strcat(atomo, &proximo); 
							Proximo(&(*linha), program);
						}
						if(Letra(proximo, alfabeto)){
							Erro(0,&(*erros), *linha, proximo); //&(*erros), (*linha), proximo); 
							//break;
						}
						foilido = 1; 
						Codigo(3, atomo); 
						inclui_final(&(*simbolos), stringAuxiliar);
					} else {
						Erro(1, &(*erros), *linha, proximo); //&(*erros), (*linha), proximo);
						//break;
					}					
				} 
			}
		}
	}
}

void TrataErros(int erros, no lexemas, FILE *res){

	if(erros == 0){
		printf("\n\t\tNenhum erro encontrado!!! ");
		if((res = fopen("lexemas.txt","wt")) == NULL){
			printf("\nfalha ao criar o arquivo de resultado");
			exit(1); 
		}

		EscreveLexemaNoArquivo(res, lexemas);
		printf("\n\n\t\t --> Lexemas gravados em Lexemas.txt\n");
		fclose(res);
		
	} else {
		if(erros == 1){
			printf("\n\n\t\tHa um erro no programa!"); 
		}else printf("\n\n\t\tForam encontrados %d erros no programa!", erros); 
		// Limpa o arquivo lexemas 
		if((res = fopen("lexemas.txt","wt")) == NULL){
			printf("\nfalha ao criar o arquivo de resultado");
			exit(1); 
		}
		fclose(res);
	}

}

void Janela2(char nomeArq[]) {

	system("cls"); 
	printf("\n\n\tDigite o nome do novo arquivo: "); 
	printf("\n\tObs: Digite sem a extensao (.txt) e o arquivo deve ser um txt"); 
	gotoxy(40, 2); 
	fflush(stdin); 
	gets(nomeArq); 
}

int remove_inicio (no *lista) {
	if (!*lista)
		return 0;
	no p = *lista;
	*lista = p->prox;
	free (p);
	return 1;
}

int ApagarLista(no *lista){
	if(!*lista)
		return 0; 
	while((*lista) != NULL){
		remove_inicio(&(*lista));
	}
	return 1; 
}

int main () {

	setlocale(LC_ALL, "");	
	FILE *program;		// Arquivo a ser lido
	FILE *resultado;	// Arquivo com os lexemas 
	no simbolos; 		// Lista en cadeada com os lexemas 
	int linha = 0; 		// Contador da linha 	
	int erros = 0; 		// Quantidade de erros encontrados 
	int opcao;			// Opção selecionada no menu
	char nomeArq[30]; 	// Nome do novo arquivo

	// Reprocessamento é possível 
	do{
		// Zerando as variaveis sempre no inicio de um novo reprocessamento 
		linha = 0; 
		erros = 0; 

		opcao = menu();		
		switch (opcao) {
			case 1:	//Exemplo pronto
				cria_lista(&simbolos);
				if((program = fopen("ex1.txt","r+t")) == NULL){
					printf("\n\n\tArquivo ex1.txt do exemplo não encontrado!!!");
					printf("\n\tTente a opção 2 e digitbe o nome do arquivo!");
				} else {
					printf("\n\n\tArquivo aberto...\n\n");
					AnalisadorLexico(&linha, program, &simbolos, &erros);
					//mostra_lista(simbolos);
					TrataErros(erros, simbolos, resultado);
					ApagarLista(&simbolos);
					fclose(program);
					program = NULL;
					resultado = NULL;
					printf("\n\t\t"); 				
				}
				break; 
			case 2: 
				cria_lista(&simbolos);
				Janela2(nomeArq); 
				strcat(nomeArq, ".txt"); 
				if((program = fopen(nomeArq, "r+t")) == NULL){
					printf("\n\n\t\tArquivo nao encontrado!!!\n\t\t"); 
				} else {
					printf("\n\n\tArquivo aberto...\n\n");
					AnalisadorLexico(&linha, program, &simbolos, &erros);
					TrataErros(erros, simbolos, resultado);
					fclose(program);
					program = NULL;
					resultado = NULL;
					printf("\n\t\t"); 	
				}
				break; 
			default:
				printf("\n\n\tObrigado!\n\t"); 
				break;
		}
		system("pause"); 
	}while(opcao != 3);
}