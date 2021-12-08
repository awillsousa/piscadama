/*
 * Tabuleiro class: Implements the game logic and the not-so-clever AI player for the exciting Gobang game.
 *
 * Copyright 2013 Tobii Technology AB. All rights reserved.
 */

#pragma once
#include <random>
#include <vector>
#include "Peca.h"

class Observer;

const int TAMANHO   = 10;	
const int NUM_PECAS = 20;

class Tabuleiro
{
public:
	// Representa uma posicao do tabuleiro
struct Posicao
{
	int linha;
	int coluna;		
	bool jogavel;
	bool ocupada;
	bool selecionada;
	Peca* peca;

	Posicao(int paramRow, int paramColumn, bool paramJogavel=false, bool paramSelecionada=false) : linha(paramRow), coluna(paramColumn), jogavel(paramJogavel), selecionada(paramSelecionada)   { }
	Posicao() { }
};

	int Tabuleiro::GetPecasAI();

	Tabuleiro(int tamanho); 
	virtual ~Tabuleiro(); 

	// Retorna o tamanho do tabuleiro
	int Tamanho() const { return _tamanho; }

	// Indica quando o jogo acabou
	bool JogoAcabou() const { return _jogadorTurno == Jogador::Nenhum; } 

	// Retorna o vencedor
	Jogador BuscaVencedor() const { return _vencedor; }

	// Retorna a peca que está em determinada posição
	Peca* GetPecaEm(int linha, int coluna);

	// Retorna um ponteiro para a posicao passada
	Posicao* GetPosicao(int linha, int coluna); 

	// Retorna a posicao inicial da ultima jogada executada
	Posicao* GetPosicaoInicial();

	// Retorna a posicao final da ultima jogada executada
	Posicao* GetPosicaoFinal();

	// Verifica se a posicao passada é inicial ou final da ultima jogada executada
	bool PosFinalouInicial(int linha, int coluna);

	// Zera os indicadores da ultima jogada executada
	void ZeraPosicoes();

	// Seta a linha e coluna de uma posicao
	void SetPosicao(int linha, int coluna);

	// Testa se a posicao passada eh valida
	bool PosicaoValida(int linha, int coluna) const;

	// Testa se a posicao passada está vazia
	bool PosicaoVazia(int linha, int coluna);
	
	// Testa se duas posições são iguais
	bool PosicoesIguais(Posicao* pos1, Posicao* pos2) const { return (pos1->linha == pos2->linha && pos1->coluna == pos2->coluna); }

	// Seleciona uma posicao
	void SelecionaPosicao(int linha, int coluna);

	// Seleciona todas as posicoes do caminho passado
	void SelecionaCaminho(std::vector<Posicao*> caminho);

	// De(s)seleciona uma posicao
	void DeselecionaPosicao(int linha, int coluna);

	// testa se uma posicao esta selecionada
	bool PosicaoSelecionada(int linha, int coluna);
	
	// Busca os movimentos possíveis para uma peça
	void BuscaMovimentos(Peca* peca);

	// Busca movimentos possiveis para uma peca dama
	void BuscaMovimentosDama(Peca* dama, std::vector<Posicao*> & movimentos, bool somenteCaptura=false);

	// Busca movimentos possiveis para uma peca dama, a partir de uma dada posição
	void BuscaCapturasDama(int linha, int coluna, std::vector<Posicao*> & movimentos);

	// Busca um caminho que termine na posicao focada
	void BuscaCaminhoFoco(int linha, int coluna);

	// Captura as pecas inimigas que estejam no caminho determinado pelas posições selecionadas
	void CapturaPecas();
	
	// Reinicia o jogo
	void NovoJogo();

	// registers an observer that is notified when the Tabuleiro has changed.
	void RegisterTabuleiroChangedObserver(Observer* TabuleiroChangedObserver);
	
	// Torna uma posicao vaga
	void EsvaziaPosicao(int linha, int coluna);

	// Retorna a distancia entre duas posicoes
	int Distancia(Posicao* pos1, Posicao* pos2);

	// Captura uma peca (captura simples)	
	void CapturaPeca(Peca* peca);

	// Verifica a jogada a ser executada é válida
	bool JogadaValida();

	// Faz uma jogada e verifica se houve vencedor ou se houve um empate
	void Jogada();

	// Faz uma jogada do player AI
	void JogadaAIPlayer();	
		
	// Realiza uma jogada do Jogador Humano (e dispara automaticamente um movimento do Jogador AI)
	void JogadaHumano();

	// Verifica se a vez de jogar é do humano
	bool TurnoHumano();

	// Poe uma peca numa dada posicao
	void PoePecaEm(Peca* peca, int linha, int coluna);

	// Retira uma peca de uma dada posicao
	void RetiraPecaDe(Peca* peca, int linha, int coluna);

	// Move uma peca para uma determinada posicao
	void MovePecaPara(Peca* peca, int linha, int coluna);
	
	// Testa se existe alguma captura a ser feita
	bool TemCaptura();

	// Testa se existe alguma captura a ser feita para uma determinada peça
	bool TemCaptura(Peca* p);

	// Marca a posicao como jogavel
	void MarcaJogavel(int linha, int coluna, bool jogavel=true);

	// Testa se a posicao é jogável ou não
	bool PosicaoJogavel(int linha, int coluna);

	// Testa se tem uma peca selecionada
	bool PecaSelecionada();

	// Retira uma posicao passada da lista de posicoes selecionadas
	void RetiraPosSelecionada(int linha, int coluna);

	// Desmarca todas as posicoes na lista de posicoes selecionadas
	void DesmarcaSelecionadas();

	// Retorna a peca selecionada (caso haja uma)
	Peca* GetPecaSelecionada();
	 
	// Retorna o tamanho do caminho a ser percorrido (posições selecionadas)
	int getTamanhoCaminho();

	void IniciaTabuleiro(int pecas[10][10]);

	std::vector<Posicao*> _movsPossiveis;
	std::vector<Posicao*> _caminhoFoco;

private:
	enum Sentido
	{
		EsqFrente=101,
		DirFrente=102,
		EsqTras=103,
		DirTras=104,
		Frente=105,
		Tras=106,
		Esq=107,
		Dir=108
	};

	// Acha uma posicao adjacente a uma outra num dado sentido
	Posicao* PosicaoAdjacente(int linha, int coluna, Sentido sentido, Jogador jogador=Jogador::Humano);
	
	// Determina de uma posicao para a outra
	Sentido GetSentido(Posicao* pos1, Posicao* pos2, Jogador jogador=Jogador::Nenhum);

	// Dada uma posição e um sentido, retorna a posição possível de realizar um movimento ou captura
	Posicao* BuscaMovimento(Peca* peca, Sentido sentido, bool somenteCaptura=false);

	// Dada uma posição e um sentido, retorna a posição possível de realizar movimento ou captura
	Posicao* BuscaMovimento(int linha, int coluna, Sentido sentido, bool somenteCaptura);

	// Busca os movimentos que podem ser executados a partir de uma determinada posição
	// Considera movimentos que a peça correntemente selecionada pode fazer
	void BuscaMovimentos(int linha, int coluna); 

	// Busca os movimentos possíveis para uma peça e retorna um vector contendo todos os movimentos possíveis
	void BuscaCaminho(int linha, int coluna, TipoPeca tipoPeca, const std::vector<Posicao*> & caminhoAnterior, std::vector<Posicao*> & melhorCaminho);

	// Busca os movimentos possíveis para uma peca terminando em uma determinada posição e retorna um vector contendo todos os movimentos possíveis
	void BuscaCaminho(int linha, int coluna, Posicao* posFinal, TipoPeca tipoPeca, const std::vector<Posicao*> & caminhoAnterior, std::vector<Posicao*> & melhorCaminho);

	// Busca capturas possíveis para uma peça
	void BuscaCapturas(Peca* peca);

	// Testa se é possível selecionar uma posição
	bool PodeSelecionar(int linha, int coluna);

	// Testa se a posicao passada já está inserida no melhor caminho
	bool PosJaVisitada(Posicao* pos, std::vector<Posicao*> & caminho);

	// Testa se o caminho passado é o melhor 
	bool CaminhoOtimo(std::vector<Tabuleiro::Posicao*> caminho);

	// Testa se o caminho1 é melhor que o caminho2
	bool CaminhoMelhor(const std::vector<Tabuleiro::Posicao*> caminho1, const std::vector<Tabuleiro::Posicao*> caminho2);

	// Inicia o Tabuleiro para uma nova partida
	void IniciaTabuleiro();

	// Inicia uma posicao
	void IniciaPosicao(int linha, int coluna);
	
	// Zera a lista de movimentos possíveis
	void ZeraMovsPossiveis();

	// Verifica se ha um vencedor
	void DetectaVencedor();

	// Verifica se aconteceu um empate
	void DetectaEmpate();

	// Notifica o observer que houve alguma mudança no tabuleiro
	void NotifyObserver();
	int _tamanho;
	Posicao  _posicao[TAMANHO][TAMANHO];

	Jogador   _jogadorTurno;
	Jogador   _vencedor;
	int		  _qtPecasHumano;
	int		  _qtPecasAI;	
	bool      _temCaptura;
	Peca*	  _pecaSelecionada;
	Posicao* _posicaoInicial;
	Posicao* _posicaoFinal;
	std::vector<Posicao*> _posSelecionadas;
	std::vector<Posicao*> _melhorCaminho;	
	Observer* _tabuleiroChangedObserver;	 
	
	// private copy constructor and operator making the class non-copyable (declared but not implemented).
	Tabuleiro(const Tabuleiro&);
	Tabuleiro& operator = (const Tabuleiro&);
};
