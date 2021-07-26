/*
 * Classe Peca: Implementa uma peca do jogo
 * 
 */

#pragma once

enum TipoPeca 
{
	Comum,
	Dama
};

const char MARCA_PECA_HUMANO = 'o';
const char MARCA_DAMA_HUMANO = 'O';
const char MARCA_PECA_AI     = 'x';
const char MARCA_DAMA_AI     = 'X';

// Representa os jogadores
enum Jogador
{
	Nenhum,
	Humano,
	Computador
};

class Peca
{
public:
	Peca(Jogador dono);
	TipoPeca GetTipo();
	void SetDono(Jogador jogador);
	Jogador GetDono();
	void seleciona();
	void deSeleciona();
	void promove();
	void marcaCapturada();
	void SetPosicao(int x, int y);
	int GetLinha();
	int GetColuna();
	bool eDama();	
private:	
	int _x;
	int _y;
	char _marca;	
	bool _selecionada;	
	TipoPeca _tipo;
	Jogador _dono;

	// private copy constructor and operator making the class non-copyable (declared but not implemented).
	Peca(const Peca&);
	Peca& operator = (const Peca&);
};
