#include <stdio.h>
#include <stdlib.h>
#define Vazio -2

//para cada estado
typedef struct Estado 
{
	int transicoes[100]; // armazena todas as transi��es para cada estado
	int auxet;           //auxiliar de quantas transi��es existem no estado
	int estadosBase[50]; // quais estados geraram esse
	int auxeb;           // quantos geraram
}estado;

int estadoTvazio = -1;
int transicaoTvazio = -1;
int pos[50];
int posTrasi[50];

//para cada transi��o
typedef struct Transicao 
{
	int estadoPara; //para qual estado
	int simbolo;    //qual o s�mbolo
}transicao;

void GerarEstado(int countY);
void novaTransicao(int De, int para, int simbolo);
void UnirTrasicoesEstados(int estadoRecebe, int estadoDe);
void RemoverVazio();
void RemoverDuplicados();
void Apontar(int countY, int estadoDe);
void RemoverTrasicao(int estado, int trasicao);
void RemoverNaoDeterminismos();
void ApontarparaNovosEstados();
int ContarVazio();
void imprimirTabela(void);

int auxe, auxt, auxs; //auxiliares de quantos estados, transi��es e s�mbolos
transicao transicoes[200];
estado estados[100];
char simbolos[50];    //dicionario de s�mbolos


int main(int argc, char* argv[]) {
	int atual = 0, para = 0, opc = 0;
	char continuar;
	auxs =0;
	auxt=0;
	auxe=0;
	char aux;
	
	//recebe valores para alimentar o sistema
	while(continuar != 'n')
	{
		system("CLS");
		
		printf("Insira as transicoes \n\n");
		
		printf("Estado Atual: ");
		scanf("%i", &atual);
		
		fflush(stdin);
		
		printf("\nSimbolo: ");
		scanf("%c", &aux);
		
		fflush(stdin);
		
		printf("\nProximo Estado: ");
		scanf("%i", &para);
		
		fflush(stdin);
		
		printf("\n\nDeseja continiar? s/n");
		scanf("%c", &continuar);
		
		int i;
		opc = 0;
		//percorre o dicionario
		for(i = 0; i < auxs; i++)
		{
			//se o s�mbolo j� existir no dicionario
			if(simbolos[i] == aux)
			{
				novaTransicao(atual, para, i);
				opc++;
				break;
			}
		}
		//se o simbolo recebido for vazio
		if(aux == '\n' || aux == 'E')
			novaTransicao(atual, para, Vazio);
		else if(opc == 0)
		{
			novaTransicao(atual, para, auxs);
			simbolos[auxs] = aux;
			auxs++;
		}
	}
	RemoverVazio();
	RemoverNaoDeterminismos();
	ApontarparaNovosEstados();
	imprimirTabela();
}

void imprimirTabela(void)
{
	system("CLS");
	printf("\n\n");
	int i, x, z, y;
	//array bidimencional onde ser� a tabela modular
	int array[auxe][auxs];
	
	//print da linha dos elementos
	for(i = 0; i < auxs; i++)
		printf("   %c", simbolos[i]);
	
	//inicializando o array com o valor -1
	for(i = 0; i < auxe; i++)
	{
		for(z = 0; z < auxs; z++)
		{
			array[i][z] = -1;
		}
	}	
	
	//alimentando a tabela modular
	
	//percorre em base na quantidade de estados
	for(i = 0; i < auxe; i++)
	{
		//percorre em base na quantidade de transi��es para cada estado
		for(z = 0; z < estados[i].auxet; z++)
		{
			//percorre todos os simbolos
			for(y = 0; y < auxs; y++)
			{	
				//se a transi��o for igual ao estado
				if(y == transicoes[estados[i].transicoes[z]].simbolo)
				{
					//a tabela modular receber� a transi��o em seu respectivo estado e simbolo
					array[i][y] = transicoes[estados[i].transicoes[z]].estadoPara;
					break;
				}
			}
		}
	}
	
	
	//imprimindo a tabela modular
	
	//percorre em base na quantidade de estados
	for(i = 0; i < auxe; i++)
	{
		//se o estado possuir transi��es
		if(estados[i].auxet > 0)
		{
			//imprimi a linha com o respectivo estado
			if(i > 9)
				printf("\n%i ", i);
			else
				printf("\n%i  ", i);
				
			//percorre todos os simbolos
			for(z = 0; z < auxs; z++)
			{
				//se possuir transi��es na tabela modular
				if(array[i][z] != -1)
				{
					//imprimi a transi��o em seu respectivo estado e simbolo
					if(array[i][z] > 9)
						printf("%i  ", array[i][z]);
					else
						printf("%i   ", array[i][z]);	
				}
				//caso o simbolo n�o tenha transi��es
				else
					printf("    ");
			}
		}
	}	
}

void novaTransicao(int de, int para, int simbolo) {
	//transi��es recebe o simbolo e o estado futuro
	transicoes[auxt].estadoPara = para;
	transicoes[auxt].simbolo = simbolo;
	
	//caso o estado n�o exista
	if(estados[de].auxet <= 0)
		//conta mais um no auxiliar de estados
		auxe++;
		
	//o estado recebe o posicionamento do array onde est� o seu estado futuro laceado com o simbolo
	estados[de].transicoes[estados[de].auxet] = auxt;
	//contador da quantidade de transi��es recebe mais 1
	estados[de].auxet++;

	//auxiliar de transi��es recebe mais 1
	auxt++;
}
void RemoverNaoDeterminismos() {

	int i = 0, j = 0, k = 0, l = 0, m = 0;
	for (k = 0; k < auxe; k++) {
		for (j = 0; j < auxs; j++) {
			int countZ = 0;
			for (l = 0; l < estados[k].auxet; l++) {
				if (transicoes[estados[k].transicoes[l]].simbolo == j) {
					pos[countZ] = transicoes[estados[k].transicoes[l]].estadoPara;
					countZ++;
				}

			}
			if (countZ > 1) {
				GerarEstado(countZ);
			}
		}
	}
}
void ApontarparaNovosEstados() {

	int i = 0, j = 0, k = 0, l = 0, m = 0;
	for (k = 0; k < auxe; k++) {
		for (j = 0; j < auxs; j++) {
			int countW = 0;
			for (l = 0; l < estados[k].auxet; l++) {
				if (transicoes[estados[k].transicoes[l]].simbolo == j) {
					pos[countW] = transicoes[estados[k].transicoes[l]].estadoPara;
					posTrasi[countW] = l;
					countW++;
				}

			}
			if (countW > 1) {
				Apontar(countW,k);
			}
		}
	}
}
void Apontar(int countY, int estadoDe) {
	int i = 0;
	int aux= 0;
	int estado = -1;
	int j = 0;
	for (j = 1; j < countY; j++) {
		for (i = 0; i < countY - 1; i++) {
			if (posTrasi[i] < posTrasi[i + 1]) {
				aux = posTrasi[i];
				posTrasi[i] = posTrasi[i + 1];
				posTrasi[i + 1] = aux;
			}
		}
	}
	for (i = 0; i < auxe; i++) {
		if (estados[i].auxeb == countY) {
			int j, k;
			int estadosIguais = 0;
			for (j = 0; j < countY; j++) {
				for (k = 0; k < estados[i].auxeb; k++) {
					if (pos[j] == estados[i].estadosBase[k]) {
						estadosIguais++;
					}
				}
			}
			if (estadosIguais == countY) {
				estado = i;
			}
		}
	}
	int simbolo = 0;
	for (i = 0; i < countY; i++) {
		simbolo = transicoes[estados[estadoDe].transicoes[posTrasi[i]]].simbolo;
		RemoverTrasicao(estadoDe, posTrasi[i]);
	}
	novaTransicao(estadoDe, estado, simbolo);
}

void GerarEstado(int countY) {
	int i = 0;
	int existe = 0;
	for (i = 0; i < auxe; i++) {
		if (estados[i].auxeb == countY) {
			int j, k;
			int estadosIguais = 0;
			for (j = 0; j < countY; j++) {
				for (k = 0; k < estados[i].auxeb; k++) {
					if (pos[j] == estados[i].estadosBase[k]) {
						estadosIguais++;
					}
				}
			}
			if (estadosIguais == countY) {
				existe = 1;
			}
		}
	}
	if (existe == 0) {
		estados[auxe].auxet = 0;
		for (i = 0; i < countY; i++) {
			estados[auxe].estadosBase[estados[auxe].auxeb] = pos[i];
			estados[auxe].auxeb++;
		}
		auxe++;
		for (i = 0; i < countY; i++) {
			UnirTrasicoesEstados(auxe - 1, pos[i]);
		}
	}
}
void RemoverTrasicao(int estado, int transicao) 
{
	//auxiliar da quantidade de transi��es por estado perde 1
	estados[estado].auxet--;
	
	//percorre todas as transi��es posteriores onde se encontra a transi��o em vazio do estado respectivo
	for (transicao = transicao; transicao < estados[estado].auxet; transicao++) 
	{
		//o array de transicoes para o estado recebe a sua transi��o posterior para cada uma a sua frente
		//isso ir� remover a transi��o em vazio
		estados[estado].transicoes[transicao] = estados[estado].transicoes[transicao + 1];
	}
}

void RemoverVazio() 
{
	//verifica se tem trasi��o em vazio
	while (ContarVazio() != 0) 
	{
		//RemoverTransicao recebe o estado e a transicao onde o s�mbolo � vazio
		RemoverTrasicao(estadoTvazio, transicaoTvazio);
		//UnirTransicoesEstados recebe o estado e a transi��o em vazio do respectivo estado
		UnirTrasicoesEstados(estadoTvazio, transicoes[estados[estadoTvazio].transicoes[transicaoTvazio]].estadoPara);
	}
}
int ContarVazio() 
{
	int i, j;
	//percorre por todos os estados
	for (i = 0; i < auxe; i++) 
	{
		//percorre todas as tranci��es para cada estado
		for (j = 0; j < estados[i].auxet; j++) 
		{
			//se o simbolo da transicoes, onde a transicoes � um array que est� na posi��o do estado ponto transicoes � vazio
			if (transicoes[estados[i].transicoes[j]].simbolo == Vazio)
			{
				//transicaoTvazio recebe a transi��o que recebe vazio
				transicaoTvazio = j;
				//estadoTvazio recebe o estado onde a transicao em vazio foi localizada
				estadoTvazio = i;
				//retorna verdadeiro pois achou uma transi��o em vazio
				return 1;
			}
		}
	}
	//retorna falso para transi��es em vazio
	return 0;
}
void UnirTrasicoesEstados(int estadoDe, int estadoPara) 
{
	int i = 0;
	//se a transi��o for diferente do pr�prio estado
	if (estadoDe != estadoPara) 
	{
		//percorre todas as transi��es do estado que a transi��o se encontrava em vazio
		for (i = 0; i < estados[estadoPara].auxet; i++) 
		{
			//o estado recebe todas as transi��es do estado onde a transi��o foi encontrada em vazio
			estados[estadoDe].transicoes[estados[estadoDe].auxet] = estados[estadoPara].transicoes[i];
			//o estado recebe mais 1 no auxiliar de estados
			estados[estadoDe].auxet++;
		}
		//remove todas as transi��es duplicadas do estado
		RemoverDuplicados(estadoDe);
	}
}
void RemoverDuplicados(int estadoDe)
{
	int i;
	//percorre por todas as transi��es do estado
	for(i = 0; i < estados[estadoDe].auxet; i++)
	{		
		int x;
		//x recebe o i + 1, pois o i � a posi��o do array de transicoes do estado
		//percorre por todas as transi��es posteriores de i
		for(x = i + 1; x < estados[estadoDe].auxet; x++)
		{
			//se a transi��o do estado for igual a outra transi��o no mesmo estado
			if(transicoes[estados[estadoDe].transicoes[i]].estadoPara == transicoes[estados[estadoDe].transicoes[x]].estadoPara && transicoes[estados[estadoDe].transicoes[i]].simbolo == transicoes[estados[estadoDe].transicoes[x]].simbolo)
			{
				//remove a transi��o duplicada
				RemoverTrasicao(estadoDe, x);
			}
		}
	}
}
