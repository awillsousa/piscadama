#include "stdafx.h"
#include "Peca.h"


// Construtor
Peca::Peca(Jogador dono): _dono(dono), _tipo(TipoPeca::Comum), _selecionada(false)
{
	//_tipo = tipo;
	if (dono == Jogador::Humano)
		_marca = MARCA_PECA_HUMANO;
	else if (dono == Jogador::Computador)
		_marca = MARCA_PECA_AI;
}

// Seta o dono da peca
void Peca::SetDono(Jogador jogador)
{
	_dono = jogador;
}

// Retorna o dono da peca
Jogador Peca::GetDono()
{
	return (_dono);
}

// Retorna o tipo da peca
TipoPeca Peca::GetTipo()
{
	return (_tipo);
}

// Seleciona a peca
void Peca::seleciona()
{
	_selecionada = true;
}

// Deseleciona a peca
void Peca::deSeleciona()
{
	_selecionada = false;
}

// Seta o valor de x da peca
void Peca::SetPosicao(int x, int y)
{
	_x = x;
	_y = y;
}

// Retorna o valor de X da peca
int Peca::GetLinha()
{
	return(_x);
}

// Retorna o valor de Y da peca
int Peca::GetColuna()
{
	return(_y);
}

// Promove a peca para dama (king)
void Peca::promove()
{	
	if (_tipo == TipoPeca::Comum)
		_tipo = TipoPeca::Dama;	
}

bool Peca::eDama()
{
	if (_tipo == TipoPeca::Dama) return (true);
	else return (false);
}

