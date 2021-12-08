#pragma once

class Peca;

const int TAMANHO   = 10;	
const int NUM_PECAS = 20;

// Representa os jogadores
enum Jogador
{
	Nenhum,
	Humano,
	Computador
};

enum TipoPeca 
{
	Comum,
	Dama
};

const char MARCA_PECA_HUMANO = 'o';
const char MARCA_DAMA_HUMANO = 'O';
const char MARCA_PECA_AI     = 'x';
const char MARCA_DAMA_AI     = 'X';



