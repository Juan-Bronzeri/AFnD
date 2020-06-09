#include <stdio.h>
#include <stdlib.h>
#define Vazio -2

//para cada estado
typedef struct Estado 
{
	int transicoes[100]; // armazena todas as transições para cada estado
	int auxet;           //auxiliar de quantas transições existem no estado
	int estadosBase[50]; // quais estados geraram esse
	int auxeb;           // quantos geraram
}estado;

int estadoTvazio = -1;
int transicaoTvazio = -1;
int pos[50];
int posTrasi[50];

//para cada transição
typedef struct Transicao 
{
	int estadoPara; //para qual estado
	int simbolo;    //qual o símbolo
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

int auxe, auxt, auxs; //auxiliares de quantos estados, transições e símbolos
transicao transicoes[200];
estado estados[100];
char simbolos[50];    //dicionario de símbolos


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
			//se o símbolo já existir no dicionario
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
	//array bidimencional onde será a tabela modular
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
		//percorre em base na quantidade de transições para cada estado
		for(z = 0; z < estados[i].auxet; z++)
		{
			//percorre todos os simbolos
			for(y = 0; y < auxs; y++)
			{	
				//se a transição for igual ao estado
				if(y == transicoes[estados[i].transicoes[z]].simbolo)
				{
					//a tabela modular receberá a transição em seu respectivo estado e simbolo
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
		//se o estado possuir transições
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
				//se possuir transições na tabela modular
				if(array[i][z] != -1)
				{
					//imprimi a transição em seu respectivo estado e simbolo
					if(array[i][z] > 9)
						printf("%i  ", array[i][z]);
					else
						printf("%i   ", array[i][z]);	
				}
				//caso o simbolo não tenha transições
				else
					printf("    ");
			}
		}
	}	
}

void novaTransicao(int de, int para, int simbolo) {
	//transições recebe o simbolo e o estado futuro
	transicoes[auxt].estadoPara = para;
	transicoes[auxt].simbolo = simbolo;
	
	//caso o estado não exista
	if(estados[de].auxet <= 0)
		//conta mais um no auxiliar de estados
		auxe++;
		
	//o estado recebe o posicionamento do array onde está o seu estado futuro laceado com o simbolo
	estados[de].transicoes[estados[de].auxet] = auxt;
	//contador da quantidade de transições recebe mais 1
	estados[de].auxet++;

	//auxiliar de transições recebe mais 1
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
	//auxiliar da quantidade de transições por estado perde 1
	estados[estado].auxet--;
	
	//percorre todas as transições posteriores onde se encontra a transição em vazio do estado respectivo
	for (transicao = transicao; transicao < estados[estado].auxet; transicao++) 
	{
		//o array de transicoes para o estado recebe a sua transição posterior para cada uma a sua frente
		//isso irá remover a transição em vazio
		estados[estado].transicoes[transicao] = estados[estado].transicoes[transicao + 1];
	}
}

void RemoverVazio() 
{
	//verifica se tem trasição em vazio
	while (ContarVazio() != 0) 
	{
		//RemoverTransicao recebe o estado e a transicao onde o símbolo é vazio
		RemoverTrasicao(estadoTvazio, transicaoTvazio);
		//UnirTransicoesEstados recebe o estado e a transição em vazio do respectivo estado
		UnirTrasicoesEstados(estadoTvazio, transicoes[estados[estadoTvazio].transicoes[transicaoTvazio]].estadoPara);
	}
}
int ContarVazio() 
{
	int i, j;
	//percorre por todos os estados
	for (i = 0; i < auxe; i++) 
	{
		//percorre todas as trancições para cada estado
		for (j = 0; j < estados[i].auxet; j++) 
		{
			//se o simbolo da transicoes, onde a transicoes é um array que está na posição do estado ponto transicoes é vazio
			if (transicoes[estados[i].transicoes[j]].simbolo == Vazio)
			{
				//transicaoTvazio recebe a transição que recebe vazio
				transicaoTvazio = j;
				//estadoTvazio recebe o estado onde a transicao em vazio foi localizada
				estadoTvazio = i;
				//retorna verdadeiro pois achou uma transição em vazio
				return 1;
			}
		}
	}
	//retorna falso para transições em vazio
	return 0;
}
void UnirTrasicoesEstados(int estadoDe, int estadoPara) 
{
	int i = 0;
	//se a transição for diferente do próprio estado
	if (estadoDe != estadoPara) 
	{
		//percorre todas as transições do estado que a transição se encontrava em vazio
		for (i = 0; i < estados[estadoPara].auxet; i++) 
		{
			//o estado recebe todas as transições do estado onde a transição foi encontrada em vazio
			estados[estadoDe].transicoes[estados[estadoDe].auxet] = estados[estadoPara].transicoes[i];
			//o estado recebe mais 1 no auxiliar de estados
			estados[estadoDe].auxet++;
		}
		//remove todas as transições duplicadas do estado
		RemoverDuplicados(estadoDe);
	}
}
void RemoverDuplicados(int estadoDe)
{
	int i;
	//percorre por todas as transições do estado
	for(i = 0; i < estados[estadoDe].auxet; i++)
	{		
		int x;
		//x recebe o i + 1, pois o i é a posição do array de transicoes do estado
		//percorre por todas as transições posteriores de i
		for(x = i + 1; x < estados[estadoDe].auxet; x++)
		{
			//se a transição do estado for igual a outra transição no mesmo estado
			if(transicoes[estados[estadoDe].transicoes[i]].estadoPara == transicoes[estados[estadoDe].transicoes[x]].estadoPara && transicoes[estados[estadoDe].transicoes[i]].simbolo == transicoes[estados[estadoDe].transicoes[x]].simbolo)
			{
				//remove a transição duplicada
				RemoverTrasicao(estadoDe, x);
			}
		}
	}
}
