/*
 * ActivatableTabuleiroGame sample:
 * This is an example that demonstrates the use of Activatable interactors in the context of a Tabuleiro game.
 *
 * Copyright 2013 Tobii Technology AB. All rights reserved.
 */

#include "stdafx.h"
#include "Tabuleiro.h"
#include <cassert>
#include "Observer.h"

Tabuleiro::Tabuleiro(int tamanho): _tamanho(tamanho), _pecaSelecionada(nullptr)
{
	_caminhoFoco = std::vector<Posicao*>();
	NovoJogo();
}

Tabuleiro::~Tabuleiro()
{
	// TODO: Desalocar todas as posiçoes 
}

/* Posiciona as pecas em seus devidos lugares, durante o início do jogo */
void Tabuleiro::IniciaTabuleiro()
{
	/* Inicia todas as posicoes do tabuleiro como vazias e define quais posicoes sao jogaveis */
	for (int i = 0; i < TAMANHO; i++)	
	{
		for (int j = 0; j < TAMANHO; j++)
		{		
			IniciaPosicao(i,j);
			if (((i%2==0) && (j%2==1))||((i%2==1) && (j%2==0)))
				MarcaJogavel(i,j);			
			else 
				MarcaJogavel(i,j, false);
		}
	}

		/* Posiciona as pecas do jogador AI */
		for (int i = 0; i < (TAMANHO/2 - 1); i++)	
		{
			for (int j = 0; j < TAMANHO; j++)
			{				
				if (((i%2==0) && (j%2==1))||((i%2==1) && (j%2==0)))
				{
					Peca* p = new Peca(Jogador::Computador);
					PoePecaEm(p,i,j);
				}				
			}
		}

		/* Posiciona as pecas do jogador Humano */
		for (int i = (TAMANHO/2 + 1); i < TAMANHO; i++)
		{
			for (int j = 0; j < TAMANHO; j++)
			{				
				if (((i%2==0) && (j%2==1))||((i%2==1) && (j%2==0)))
				{
					Peca* p = new Peca(Jogador::Humano);	
					PoePecaEm(p,i,j);
				}				
			}
		}
		
}	

void Tabuleiro::IniciaTabuleiro(int pecas[10][10])
{
	/* Inicia todas as posicoes do tabuleiro como vazias e define quais posicoes sao jogaveis */
	for (int i = 0; i < TAMANHO; i++)	
	{
		for (int j = 0; j < TAMANHO; j++)
		{		
			IniciaPosicao(i,j);
			if (((i%2==0) && (j%2==1))||((i%2==1) && (j%2==0)))
				MarcaJogavel(i,j);			
			else 
				MarcaJogavel(i,j, false);
		}
	}

		/* Posiciona as pecas do jogador AI */
		for (int i = 0; i < TAMANHO; i++)	
		{
			for (int j = 0; j < TAMANHO; j++)
			{				
				if (((i%2==0) && (j%2==1))||((i%2==1) && (j%2==0)))
				{					
					Peca* p = new Peca(Jogador::Computador);
					if (pecas[i][j] == 3)
						p->promove();
					if ((pecas[i][j] == 1)||(pecas[i][j] == 3))
						PoePecaEm(p,i,j);
				}				
			}
		}

		/* Posiciona as pecas do jogador Humano */
		for (int i = 0; i < TAMANHO; i++)
		{
			for (int j = 0; j < TAMANHO; j++)
			{				
				if (((i%2==0) && (j%2==1))||((i%2==1) && (j%2==0)))
				{
					Peca* p = new Peca(Jogador::Humano);	
					if (pecas[i][j] == 4)
						p->promove();
					if ((pecas[i][j] == 2)||(pecas[i][j] == 4))
						PoePecaEm(p,i,j);					
				}				
			}
		}
		
}

// Inicia uma posicao
void Tabuleiro::IniciaPosicao(int linha, int coluna)
{
	SetPosicao(linha,coluna);
	EsvaziaPosicao(linha,coluna);
	_posicao[linha][coluna].selecionada = false;	
}

// Retorna uma ponteiro para a peça na posição passada
Peca* Tabuleiro::GetPecaEm(int linha, int coluna)
{
	assert(PosicaoValida(linha,coluna));

	return (_posicao[linha][coluna].peca);
}

// Retorna um ponteiro para uma determinada posição (struct) do tabuleiro
Tabuleiro::Posicao* Tabuleiro::GetPosicao(int linha, int coluna)
{
	assert(PosicaoValida(linha,coluna));

	return (&_posicao[linha][coluna]);
}

// Retorna a posicao inicial da ultima jogada realizada
Tabuleiro::Posicao* Tabuleiro::GetPosicaoInicial()
{
	if (_posicaoInicial != nullptr)
		return (_posicaoFinal);
	else
		return (nullptr);
}

// Retorna a posicao final da ultima jogada realizada
Tabuleiro::Posicao* Tabuleiro::GetPosicaoFinal()
{
	if (_posicaoFinal != nullptr)
		return (_posicaoFinal);
	else
		return (nullptr);
}

// Verifica se a posicao passada é inicial ou final da ultima jogada executada
bool Tabuleiro::PosFinalouInicial(int linha, int coluna)
{
	Posicao* posInicial = GetPosicaoInicial();
	Posicao* posFinal = GetPosicaoFinal();	

	if ((posInicial == nullptr) || (posFinal == nullptr))
		return (false);

	if (((linha == posInicial->linha) && (coluna == posInicial->coluna)) || ((linha == posFinal->linha) && (coluna == posFinal->coluna)))
		return (true);
	else 
		return (false);
}

// Zera os indicadores da ultima jogada executada
void Tabuleiro::ZeraPosicoes()
{
	_posicaoInicial = nullptr;
	_posicaoFinal   = nullptr;
}

// Seta as variáveis de indicação da linha e coluna da struct com a sua posição no tabuleiro
void Tabuleiro::SetPosicao(int linha, int coluna)
{
	assert(PosicaoValida(linha,coluna));

	_posicao[linha][coluna].linha  = linha;
	_posicao[linha][coluna].coluna = coluna;
}

// Verifica se o turno de jogar é do humano
bool Tabuleiro::TurnoHumano()
{
	if (_jogadorTurno == Jogador::Humano) 
		return (true);
	else if (_jogadorTurno == Jogador::Computador)
		return (false);

}

// Poe uma peca numa determinada posição
void Tabuleiro::PoePecaEm(Peca* peca, int linha, int coluna)
{
	assert(PosicaoValida(linha,coluna));

	_posicao[linha][coluna].peca    = peca;
	_posicao[linha][coluna].ocupada = true;
	peca->SetPosicao(linha,coluna);
	
	NotifyObserver();
}

// Retira uma peca de determinada posição
void Tabuleiro::RetiraPecaDe(Peca* peca, int linha, int coluna)
{
	assert(PosicaoValida(linha,coluna));		

	EsvaziaPosicao(linha,coluna);

	NotifyObserver();
}

// Move uma peca da sua posicao atual para uma nova posição
void Tabuleiro::MovePecaPara(Peca* peca, int linha, int coluna)
{
	RetiraPecaDe(peca, peca->GetLinha(), peca->GetColuna());
	PoePecaEm(peca, linha, coluna);
}

// Verifica se a jogada a ser exeucutada é válida
// Basicamente as jogadas que podem ser executadas, são:
//   movimento simples - a peca anda uma casa na diagonal
//   movimento de captura simples - a peca anda duas casas na diagonal capturando uma pecao inimiga
//   movimento de captura multipla - a peca percorre um caminho determinado pelas posições selecionadas
//   _posSelecionadas, capturando as pecas inimigas no caminho
// Quando se tratar de uma peça comum, as distâncias entre cada posição será de no máximo duas casas
// Quando se tratar de uma peça dama, as distâncias poderão ter qualquer tamanho
bool Tabuleiro::JogadaValida()
{
	//assert(PecaSelecionada());
	if (PecaSelecionada() && ((getTamanhoCaminho() >= 2) || (_caminhoFoco.size() > 1)))		// sem peça selecionada não há jogada, sem uma segunda posição também não; com um caminho focado há uma possibilidade
	{
		  Peca* peca = GetPecaSelecionada();
		   int linha = peca->GetLinha();
		  int coluna = peca->GetColuna();
		Posicao* pos = GetPosicao(linha, coluna);
	
		if (peca->GetTipo() == TipoPeca::Comum)
		{
			if ((getTamanhoCaminho() == 2) && (Distancia(pos, _posSelecionadas.at(1)) == 1))	// movimento simples
			{
				Sentido sentido = GetSentido(pos, _posSelecionadas.at(1));
				if ((sentido == Sentido::EsqTras) || (sentido == Sentido::DirTras))
					return (false);
				else
					return (true);
			}
			else if (_caminhoFoco.size() > 1)
			{
				return (true);
			}
			else    // movimento de captura (multiplo ou simples)
			{
				Posicao* pos_atual = _posSelecionadas.at(0);
				Posicao* prox_pos; 
				int tamanho = _posSelecionadas.size();
				for(int i=1; i < tamanho; i++) 
				{
					prox_pos = _posSelecionadas.at(i);

					// Verifica se a proxima posicao está na mesma linha da atual
					int dif_linha  = (prox_pos->linha - pos_atual->linha);
					int dif_coluna = (prox_pos->coluna - pos_atual->coluna);
					if (abs(dif_linha) == abs(dif_coluna))
					{		
						// Verifica se há uma peça inimiga entre as duas posições
						int m_linha  = pos_atual->linha  + (prox_pos->linha - pos_atual->linha)/2;
						int m_coluna = pos_atual->coluna + (prox_pos->coluna - pos_atual->coluna)/2;
						Peca* captura = GetPecaEm(m_linha, m_coluna);
										
						if (Distancia(pos_atual, prox_pos) != 2 || (captura->GetDono() == _jogadorTurno) || PosicaoVazia(m_linha, m_coluna))  
							return (false);
					}
					else
						return (false);			// posicao selecionada não está na mesma linha da posicao atual

					pos_atual = prox_pos;					
				}
			}
		}
		else    // a peça é uma dama (ou king na versão inglesa)
		{
			if (getTamanhoCaminho() == 2)	// movimento simples da dama (com distancia maior que 2)
			{
				Posicao* pos_atual = _posSelecionadas.at(0);
				Posicao* prox_pos = _posSelecionadas.at(1); 

				int linha  = pos_atual->linha;
				int coluna = pos_atual->coluna;

				int dif_linha  = (prox_pos->linha - pos_atual->linha);
				int dif_coluna = (prox_pos->coluna - pos_atual->coluna);
				int incr_linha = dif_linha/abs(dif_linha);			// dependendo da posicao da posicao final o incremento poderá ser +1 ou -1
				int incr_coluna = dif_coluna/abs(dif_coluna);		// dependendo da posicao da posicao final o incremento poderá ser +1 ou -1
				int distancia = Distancia(pos_atual, prox_pos);

				// Verifica se há alguma peça inimiga na diagonal de movimentacao				
				for (int i=0; i < distancia; i++)
				{
					linha  += incr_linha;
					coluna += incr_coluna;
					if (!PosicaoVazia(linha, coluna))
					{
						Peca* p = GetPecaEm(linha, coluna);
						if (p->GetDono() == _jogadorTurno) // jogador não pode "pular" suas próprias peças
							return (false);
						else  	// Se há uma peça inimiga a dama tem que ser movimentada para a próxima posição adjacente
						{
							if (i == distancia-2)   // uma casa antes da casa final
								return (true);
							else
								return (false);
						}
					}
				}
				return (true);
			}
		}	
	}
	else
	{
		return (false);
	}
}


// Marca um posicao como jogavel ou não jogável
void Tabuleiro::MarcaJogavel(int linha, int coluna, bool jogavel)
{
		_posicao[linha][coluna].jogavel = jogavel;
		return;
}

// Testa se existe alguma captura a ser feita
bool Tabuleiro::TemCaptura()
{
	int  linha = _pecaSelecionada->GetLinha();
	int coluna = _pecaSelecionada->GetColuna();

	Posicao* sel = &(_posicao[linha][coluna]);

	_temCaptura  = false;
	
	// Busca todas as capturas possíveis
	for (int i = 0; i < _tamanho; i++)
	{
		for (int j = 0; j < _tamanho; j++)
		{
			// Procura por uma peca do jogador
			if (PosicaoJogavel(i,j) && (!PosicaoVazia(i,j)))   
			{
				Peca* p = GetPecaEm(i,j);
				if (p->GetDono() == _jogadorTurno)
				{
					BuscaCapturas(p);					
					if (_movsPossiveis.size() > 0)  // tem capturas possiveis no caminho retornado?	
					{
						_temCaptura = true;	
						return (true);
					}
				}
			}		
		}
	}

	return(false);
}

// Testa se existe alguma captura a ser feita para uma determinada peça
bool Tabuleiro::TemCaptura(Peca* p)
{	
	_temCaptura  = false;		
	BuscaCapturas(p);
					
	if (_movsPossiveis.size() > 0)  // tem capturas possiveis no caminho retornado?	
	{
		_temCaptura = true;	
		return (true);
	}
	
	return(false);
}


// Testa se uma posição passada é jogável ou não
bool Tabuleiro::PosicaoJogavel(int linha, int coluna)
{
	//return (_posicao[linha][coluna].jogavel);
	return (((linha%2==0) && (coluna%2==1))||((linha%2==1) && (coluna%2==0)));
}

// Testa se é possível selecionar uma posição
// Verificando se ela está inserida na lista de possíveis movimentos
bool Tabuleiro::PodeSelecionar(int linha, int coluna)
{
	for(std::vector<Posicao*>::iterator it = _movsPossiveis.begin(); it != _movsPossiveis.end(); ++it) 
	{
		Posicao* p = *it;
		if (p == &(_posicao[linha][coluna])) return (true);		
	}
	return (false);
}

// Seleciona uma posicao e caso haja peça na posição, seleciona a peça também
// Caso haja alguma peça selecionada e uma nova posição contendo uma peça for selecionada,
// a peça em questão se torna a peça selecionada e a lista de posições selecionadas é
// zerada
void Tabuleiro::SelecionaPosicao(int linha, int coluna)
{
	if (PosicaoSelecionada(linha,coluna))	// selecionar uma posicao já selecionada, desmarca ela
	{		
		   DeselecionaPosicao(linha,coluna);
	}
	else if (PosicaoVazia(linha,coluna))
	{		
		if (PecaSelecionada() && PodeSelecionar(linha, coluna))	// só permite selecionar posicoes vazias se já houver uma peca selecionada
		{
			Posicao* posAnterior = _posSelecionadas.back();
			Peca* pecaSel = GetPecaSelecionada();
			_posicao[linha][coluna].selecionada = true;
			_posSelecionadas.push_back(&(_posicao[linha][coluna]));			
			Posicao* posAtual = &(_posicao[linha][coluna]);
			if (Distancia(posAtual, posAnterior) == 2)
				BuscaMovimentos(linha, coluna);			
			else
				if ((_posSelecionadas.size() == 2) && (pecaSel->GetTipo() != TipoPeca::Dama))  // é um movimento simples
					ZeraMovsPossiveis();
				else 
					if ((pecaSel->GetTipo() == TipoPeca::Dama) && _temCaptura)
						BuscaCapturasDama(linha, coluna, _movsPossiveis);
		}
	}
	else
	{
		Peca* p = GetPecaEm(linha,coluna);			

		if (p->GetDono() == _jogadorTurno)   // o jogador só pode selecionar as suas peças
		{
			if (PecaSelecionada()) 
			{
				Peca* o = GetPecaSelecionada();				
				DeselecionaPosicao(o->GetLinha(), o->GetColuna());		// desmarca a peca que estava selecionada anteriormente e a sua posicao
			}
			_posicao[linha][coluna].selecionada = true;			
			_posSelecionadas.push_back(&(_posicao[linha][coluna]));		// inclui a posição na lista de posições selecionadas
			p->seleciona();  
			_pecaSelecionada = p;

			TemCaptura();			// verifica se há capturas possíveis de serem efetuadas
			BuscaMovimentos(p);		// busca os movimentos possíveis para aquela peça. Caso haja capturas, ignora movimentações
		}
	}
}

// Seleciona todas as posicoes do caminho passado
void Tabuleiro::SelecionaCaminho(std::vector<Posicao*> caminho)
{
	DesmarcaSelecionadas();
	for(std::vector<Posicao*>::iterator it = caminho.begin(); it != caminho.end(); ++it) 
	{
		Posicao* p = *it;
		SelecionaPosicao(p->linha, p->coluna);					
	}
	ZeraMovsPossiveis(); 
}

// Deseleciona uma posicao e cajo haja peça na posição, deseleciona a peça também
// Caso a posicao deselecionada possua uma peca, a lista de posições selecionadas 
// é zerada
void Tabuleiro::DeselecionaPosicao(int linha, int coluna)
{
	_posicao[linha][coluna].selecionada = false;		// deseleciona a posicao
	
	if (!PosicaoVazia(linha,coluna))
	{
		Peca* p = GetPecaEm(linha,coluna);			
		p->deSeleciona();			// deseleciona a peça que está na posição passada
		_pecaSelecionada = nullptr;	// limpa o indicador da peça selecionada atual		
		DesmarcaSelecionadas();		// desmarca todas as posições que estão selecionadas
		_posSelecionadas.clear();	// limpa a lista de posicoes selecionadas
		_movsPossiveis.clear();		// zera a lista de movimentos possíveis para a peça
	}
	else
	{
		RetiraPosSelecionada(linha, coluna);
		if (_posSelecionadas.size() > 1)
		{			
			Posicao* p = _posSelecionadas.back();
			BuscaMovimentos(p->linha, p->coluna);		// atualiza a lista de movimentos possiveis para a (nova) ultima posicao possível
		}
		else
			BuscaMovimentos(_pecaSelecionada);
	}
}

// Testa se uma posicao esta selecionada
bool Tabuleiro::PosicaoSelecionada(int linha, int coluna)
{
	return(_posicao[linha][coluna].selecionada);
}

// Retira uma posicao da lista de posições selecionadas e todas as posicoes subsequentes do caminho
void Tabuleiro::RetiraPosSelecionada(int linha, int coluna)
{
	bool achouPosicao=false;
	int posicao;
	int tamanho = _posSelecionadas.size();

	for(int i=0; i < tamanho; i++)			// itera o vector
    {
		if ((_posSelecionadas[i]->linha == linha) && (_posSelecionadas[i]->coluna == coluna))   //verifica se a posicao passada esta no vector de selecionadas
		{
			achouPosicao = true;
			posicao = i;
		}

		if (achouPosicao)	// deseleciona as posicoes a partir da posicao encontrada
		{			
			_posSelecionadas[i]->selecionada = false;								
		}
    }
	if (achouPosicao)	// exclui da lista de posicoes selecionadas todos as posições a partir da posição passada (inclusive)
	{		
			_posSelecionadas.erase(_posSelecionadas.begin()+posicao, _posSelecionadas.end());
	}
}

// Itera a lista de posicoes selecionadas e desmarca todas elas
void Tabuleiro::DesmarcaSelecionadas()
{
	int tamanho = _posSelecionadas.size();

	for(int i=0; i < tamanho; i++) 
    {
		_posSelecionadas[i]->selecionada = false;		
    }

	_posSelecionadas.clear();
}

// Testa se há uma pecao selecionada
bool Tabuleiro::PecaSelecionada()
{
	if (_pecaSelecionada != nullptr)
		return (true);
	else
		return (false);
}

// Retorna a peca selecionada (caso haja uma)
Peca* Tabuleiro::GetPecaSelecionada()
{
	if (PecaSelecionada())
		return (_pecaSelecionada);
	else
		return (nullptr);
}

// Retorna o tamanho do caminho a ser percorrido (lista de posições selecionadas)
int Tabuleiro::getTamanhoCaminho()
{
	return (_posSelecionadas.size());
}

// Executa a captura de peças no caminho a ser percorrido (lista de posições selecionadas)
void Tabuleiro::CapturaPecas()
{
	Posicao* pos_atual = _posSelecionadas.at(0);
	Posicao* prox_pos; 
	Posicao* pos;
	Peca* captura;

	int tamanho = _posSelecionadas.size();
	for(int i=1; i < tamanho; i++) 
	{
		int linha1;
		int coluna1;
		int linha2;
		int coluna2;
		int incrLinha;
		int incrColuna;

		prox_pos = _posSelecionadas.at(i);

		 linha1 = pos_atual->linha;
		coluna1 = pos_atual->coluna;
		 linha2 = prox_pos->linha;
		coluna2 = prox_pos->coluna;
		 incrLinha = (linha2 - linha1)/abs(linha2 - linha1);
		incrColuna = (coluna2 - coluna1)/abs(coluna2 - coluna1);
		/*
		// Verifica se há uma peça inimiga entre as duas posições
		int m_linha  = pos_atual->linha  + (prox_pos->linha - pos_atual->linha)/2;
		int m_coluna = pos_atual->coluna + (prox_pos->coluna - pos_atual->coluna)/2;
		*/
		while ((linha1 != linha2) && (coluna1 != coluna2))
		{			
			linha1  += incrLinha;
			coluna1 += incrColuna;
			
			pos = GetPosicao(linha1, coluna1);
			if (pos->ocupada)
			{
				captura = GetPecaEm(linha1, coluna1);	
				if (captura->GetDono() != _pecaSelecionada->GetDono())
					CapturaPeca(captura);
			}
		}
					
		pos_atual = prox_pos;					
	}
	
}



// Executa a jogada do jogador Humano
void Tabuleiro::JogadaHumano()
{	
	if (_jogadorTurno == Jogador::Humano && !JogoAcabou())  // Jogador só pode jogar no seu turno ou enquanto o jogo não acabou
	{	
		if ((_caminhoFoco.size() > 1) && !(_posSelecionadas.size() > 1))	// se tiver um caminho focado e não tiver selecionado um caminho
		{
			SelecionaCaminho(_caminhoFoco);
		}    
		Jogada();
		
		if (!JogoAcabou())		// Realiza a jogada do jogador AI		
			_jogadorTurno = Jogador::Computador;					
	}
}

// Dada uma posição e um sentido, retorna a posição possível de realizar um movimento (ou captura)
Tabuleiro::Posicao* Tabuleiro::BuscaMovimento(Peca* peca, Sentido sentido, bool somenteCaptura)
{
	int linha  = peca->GetLinha();
	int coluna = peca->GetColuna();
	Posicao* posAdjacente = PosicaoAdjacente(linha, coluna, sentido, _jogadorTurno);

	if (peca->GetTipo() == TipoPeca::Comum)
	{
		if (posAdjacente != nullptr)
		{
			if (!posAdjacente->ocupada)		// posicao está vazia, é um movimento simples
			{
				if ((!somenteCaptura) && (!_temCaptura))			
					return (posAdjacente);			
			}
			else       // posição ocupada, pode ser uma captura
			{
					Peca* outraPeca = posAdjacente->peca;
				Posicao* posCaptura = PosicaoAdjacente(posAdjacente->linha, posAdjacente->coluna, sentido, _jogadorTurno);

				if ((outraPeca->GetDono() != peca->GetDono()) // Se for uma peça inimiga					 
					&& (posCaptura != nullptr))		
				{					
					if (!posCaptura->ocupada)
						return (posCaptura);
				}			
			}
		}
	}
	else if (peca->GetTipo() == TipoPeca::Dama)
	{

	}

	return (nullptr);
}

// Busca os movimentos possíveis para uma peça
void Tabuleiro::BuscaMovimentos(Peca* peca) 
{
	int linha  = peca->GetLinha();
	int coluna = peca->GetColuna();
	int contaDir = 0;
	
	_movsPossiveis.clear();

	if (peca->GetTipo() == TipoPeca::Comum)	// peca comum só movimenta para frente e nas diagonais 1 casa por vez
	{
		Posicao* pos=nullptr;
		
		while (contaDir < 4)
		{
			switch (contaDir)
			{
				case 0:
					pos = BuscaMovimento(peca, Sentido::DirFrente);		// busca movimento e captura
				break;
				case 1:
					pos = BuscaMovimento(peca, Sentido::EsqFrente);		// busca movimento e captura		
				break;
				case 2:
					pos = BuscaMovimento(peca, Sentido::DirTras, true);	// busca somente capturas
				break;
				case 3:
					pos = BuscaMovimento(peca, Sentido::EsqTras, true);	// busca somente capturas
				break;
			}			
			contaDir++;		
			if (pos != nullptr) _movsPossiveis.push_back(pos);					
		}
	}
	else if (peca->GetTipo() == TipoPeca::Dama)		// dama movimenta para onde quiser, desde que não haja uma peça no caminho
	{
		if (_temCaptura)
			BuscaMovimentosDama(peca, _movsPossiveis, true);
		else
			BuscaMovimentosDama(peca, _movsPossiveis);	
	}
}

// Busca movimentos possiveis para uma peca dama
void Tabuleiro::BuscaMovimentosDama(Peca* dama, std::vector<Posicao*> & movimentos, bool somenteCaptura)
{	
	int n = 0;
	int incrLinha  = -1;
	int incrColuna = -1;	
	int incremLinha = true;
	int linha;
	int coluna;

	// Procura em cada uma das diagonais por movimentos
	do
	{
		bool buscaPos = true;
		Posicao* proxPos = nullptr; 
		linha  = dama->GetLinha();
		coluna = dama->GetColuna();
		linha  += incrLinha;
		coluna += incrColuna;

		while (buscaPos && PosicaoValida(linha, coluna))
		{
			if (PosicaoValida(linha, coluna))
			{
				proxPos = GetPosicao(linha, coluna);

				if (PosicaoVazia(linha, coluna) && !somenteCaptura)
				{				
					movimentos.push_back(proxPos);						
					buscaPos=true;
				}
				else
				{
					//Sentido sentido = GetSentido(GetPosicao(dama->GetLinha(), dama->GetColuna()), proxPos, this->_jogadorTurno);
				
					if (proxPos != nullptr)
					{					
						Peca* outraPeca = proxPos->peca;
						Posicao* posCaptura = GetPosicao(proxPos->linha+incrLinha, proxPos->coluna+incrColuna);//PosicaoAdjacente(proxPos->linha, proxPos->coluna, sentido, this->_jogadorTurno);

						if ((outraPeca->GetDono() != dama->GetDono()) // Se for uma peça inimiga					 
							&& (posCaptura != nullptr))		
						{					
							if (!posCaptura->ocupada)
							{
								_temCaptura = true;
								movimentos.push_back(posCaptura);
							}
						}
						buscaPos=false;
					}
				}
			}

			linha  += incrLinha;
			coluna += incrColuna;
		}

		
		if (incremLinha)
			incrLinha = incrLinha*(-1);	
		else		
			incrColuna = incrColuna*(-1);
		
		incremLinha = !incremLinha;
		n++;
	} while (n < 4); 


}

// Busca movimentos possiveis para uma peca dama, a partir de uma dada posição
void Tabuleiro::BuscaCapturasDama(int linhaBase, int colunaBase, std::vector<Posicao*> & movimentos)
{	
	int n = 0;
	int incrLinha  = -1;
	int incrColuna = -1;	 
	int incremLinha = true;
	int linha;
	int coluna;
	Peca* dama = this->GetPecaSelecionada();
	int posPenult = _posSelecionadas.size()-2;
	Posicao* penultPos = _posSelecionadas.at(posPenult);

	Sentido sentido = GetSentido(GetPosicao(linhaBase, colunaBase), penultPos);

	ZeraMovsPossiveis();
	// Procura em cada uma das diagonais por movimentos
	do
	{
		bool buscaPos = true; 
		Posicao* proxPos = nullptr; 
		linha  = linhaBase;
		coluna = colunaBase;
		linha  += incrLinha;
		coluna += incrColuna;

		while (buscaPos && PosicaoValida(linha, coluna))
		{
			//if (PosicaoValida(linha, coluna))
			//{
				proxPos = GetPosicao(linha, coluna);
				if (GetSentido(GetPosicao(linhaBase, colunaBase), proxPos) == sentido)
					break;

				if (PosicaoVazia(linha, coluna))
				{				
					//movimentos.push_back(proxPos);						
					buscaPos=true;
				}
				else
				{					
					if (proxPos != nullptr)
					{					
						Peca* outraPeca = proxPos->peca;
						Posicao* posCaptura = GetPosicao(proxPos->linha+incrLinha, proxPos->coluna+incrColuna);//PosicaoAdjacente(proxPos->linha, proxPos->coluna, sentido, this->_jogadorTurno);

						if ((outraPeca->GetDono() != dama->GetDono()) // Se for uma peça inimiga					 
							&& (posCaptura != nullptr) && PosicaoJogavel(posCaptura->linha, posCaptura->coluna))		
						{					
							if (!posCaptura->ocupada)
							{
								_temCaptura = true;
								movimentos.push_back(posCaptura);
							}
						}
						buscaPos=false;
					}
				}
			//}

			linha  += incrLinha;
			coluna += incrColuna;
		}

		
		if (incremLinha)
			incrLinha = incrLinha*(-1);	
		else		
			incrColuna = incrColuna*(-1);
		
		incremLinha = !incremLinha;		// controle para incrementar a linha e a coluna alternadamente
		n++;
	} while (n < 4); 


}

// Busca capturas possíveis para uma peça
void Tabuleiro::BuscaCapturas(Peca* peca)
{
	int contaDir = 0;		

	ZeraMovsPossiveis();
	//if (peca->GetTipo() == TipoPeca::Comum)
	//{
		Posicao* pos=nullptr;
		
		while (contaDir < 4)
		{
			switch (contaDir)
			{
				case 0:
					pos = BuscaMovimento(peca, Sentido::DirFrente, true);		// busca somente capturas
				break;
				case 1:
					pos = BuscaMovimento(peca, Sentido::EsqFrente, true);		// busca somente capturas		
				break;
				case 2:
					pos = BuscaMovimento(peca, Sentido::DirTras, true);	// busca somente capturas
				break;
				case 3:
					pos = BuscaMovimento(peca, Sentido::EsqTras, true);	// busca somente capturas
				break;
			}			
			contaDir++;		
			if (pos != nullptr) _movsPossiveis.push_back(pos);					
		}
	//}
		
}

// Dada uma posição e um sentido, retorna a posição possível de realizar movimento ou captura
Tabuleiro::Posicao* Tabuleiro::BuscaMovimento(int linha, int coluna, Sentido sentido, bool somenteCaptura)
{
	Peca* peca = GetPecaSelecionada();
	Posicao* posAnterior  = _posSelecionadas.size() == 1 ? nullptr : _posSelecionadas.at(_posSelecionadas.size()-2);			// penultima posicao selecionada
	Posicao* posAdjacente = PosicaoAdjacente(linha, coluna, sentido, _jogadorTurno);

	if ((posAdjacente != nullptr) && PosicaoValida(posAdjacente->linha, posAdjacente->coluna))
	{
		if (!posAdjacente->ocupada)		// posicao está vazia, é um movimento simples
		{
			if (!somenteCaptura)			
				return (posAdjacente);			
		}
		else       // posição ocupada, pode ser uma captura
		{
			    Peca* outraPeca = posAdjacente->peca;
			Posicao* posCaptura = PosicaoAdjacente(posAdjacente->linha, posAdjacente->coluna, sentido, _jogadorTurno);
			if ((outraPeca->GetDono() != peca->GetDono())			// Se for uma peça inimiga		
				&& (posCaptura != posAnterior)	   // Não pode selecionar a posição anteriormente selecionada
				&& (posCaptura != nullptr))			
			{	
				if ((!posCaptura->ocupada)||
					(posCaptura->ocupada && (posCaptura->linha == peca->GetLinha() && posCaptura->coluna == peca->GetColuna())))  // caso passe mais de uma vez pela mesma posicao durante uma captura
					return (posCaptura);
			}			
		}
	}

	return (nullptr);
}

// Busca os movimentos que podem ser executados a partir de uma determinada posição
// Considera movimentos que a peça correntemente selecionada pode fazer
void Tabuleiro::BuscaMovimentos(int linha, int coluna) 
{
	_movsPossiveis.clear();
	Peca* peca = GetPecaSelecionada();

	if (peca->GetTipo() == TipoPeca::Comum)
	{
		Posicao* pos;
		
		pos = BuscaMovimento(linha, coluna, Sentido::DirFrente, true);
		if (pos != nullptr) _movsPossiveis.push_back(pos);
		pos = BuscaMovimento(linha, coluna, Sentido::EsqFrente, true);
		if (pos != nullptr) _movsPossiveis.push_back(pos);
		pos = BuscaMovimento(linha, coluna, Sentido::DirTras, true);
		if (pos != nullptr) _movsPossiveis.push_back(pos);
		pos = BuscaMovimento(linha, coluna, Sentido::EsqTras, true);
		if (pos != nullptr) _movsPossiveis.push_back(pos);
	}
	else
	{
		BuscaCapturasDama(linha, coluna, _movsPossiveis);
	}
}


// Busca um caminho de captura a partir de uma determinada posicao
// Busca capturas na quatro direcoes possiveis e recursivamente vai buscando novas capturas, a partir das posicoes de captura adjacentes
// em todas as direcoes possiveis
void Tabuleiro::BuscaCaminho(int linha, int coluna, TipoPeca tipoPeca, const std::vector<Posicao*> & caminhoAnterior, std::vector<Posicao*> & melhorCaminho) 
{		
	int contaDir  = 0;
	std::vector<Posicao*> caminho(caminhoAnterior);

	if (caminho.size() == 0)	// primeira chamada da função para a primeira peca
		caminho.push_back(_posSelecionadas.at(0));   // insere a peca selecionada no inicio do caminho
	
	if (tipoPeca == TipoPeca::Comum)	// peca comum só captura saltando uma casa por vez
	{
		Posicao* pos=nullptr;

		while (contaDir < 4)
		{
			switch (contaDir)
			{
				case 0:
					pos = BuscaMovimento(linha, coluna, Sentido::DirFrente, true);	// busca somente capturas
				break;
				case 1:
					pos = BuscaMovimento(linha, coluna, Sentido::EsqFrente, true);	// busca somente capturas
				break;
				case 2:
					pos = BuscaMovimento(linha, coluna, Sentido::DirTras, true);	// busca somente capturas
				break;
				case 3:
					pos = BuscaMovimento(linha, coluna, Sentido::EsqTras, true);	// busca somente capturas
				break;
			}
			contaDir++;

			if (pos != nullptr)
			{
				Posicao* ultima_pos = nullptr;
				Posicao* penult_pos = nullptr;
				int tam = caminho.size();

				if (tam > 1)
					penult_pos = caminho.at(tam-2);											
				if (tam > 0)				
					ultima_pos = caminho.at(tam-1);
				
				if ((pos != penult_pos) && !(PosJaVisitada(pos, caminho) && PosJaVisitada(ultima_pos, caminho)))
				{
					caminho.push_back(pos);

					if (CaminhoMelhor(caminho, melhorCaminho))		// se o caminho atual for melhor que O MELHOR, torne-o O MELHOR
						melhorCaminho = caminho;
				
					BuscaCaminho(pos->linha, pos->coluna, tipoPeca, caminho, melhorCaminho);
					caminho.pop_back();
				}
			}			
		}		
	}
}

// Busca um caminho que termine numa posição passada
void Tabuleiro::BuscaCaminho(int linha, int coluna, Posicao* posFinal, TipoPeca tipoPeca, const std::vector<Posicao*> & caminhoAnterior, std::vector<Posicao*> & melhorCaminho) 
{		
	int contaDir  = 0;
	std::vector<Posicao*> caminho(caminhoAnterior);

	if (caminho.size() == 0)	// primeira chamada da função para a primeira peca
		caminho.push_back(_posSelecionadas.at(0));   // insere a peca selecionada no inicio do caminho
	
	if (tipoPeca == TipoPeca::Comum)	// peca comum só captura saltando uma casa por vez
	{
		Posicao* pos=nullptr;				
		while (contaDir < 4)
		{
			switch (contaDir)
			{
				case 0:
					pos = BuscaMovimento(linha, coluna, Sentido::DirFrente, true);	// busca somente capturas
				break;
				case 1:
					pos = BuscaMovimento(linha, coluna, Sentido::EsqFrente, true);	// busca somente capturas
				break;
				case 2:
					pos = BuscaMovimento(linha, coluna, Sentido::DirTras, true);	// busca somente capturas
				break;
				case 3:
					pos = BuscaMovimento(linha, coluna, Sentido::EsqTras, true);	// busca somente capturas
				break;
			}
			contaDir++;

			if (pos != nullptr)
			{
				Posicao* ultima_pos = nullptr;
				Posicao* penult_pos = nullptr;
				    int tam = caminho.size();
				bool iguais = false;
			
				if (tam > 1)
					penult_pos = caminho.at(tam-2);											
				if (tam > 0)
				{
					ultima_pos = caminho.at(tam-1);
					iguais = PosicoesIguais(posFinal, ultima_pos);			// somente será igual em uma passagem específica antes de começar a desempilhar (subir no nível de recursão)
					if (iguais)
						melhorCaminho = caminho;							// esse caminho é o melhor para o que precisamos (posição focada)
				}
						
				if (melhorCaminho.size() > 0)
					iguais = PosicoesIguais(posFinal, melhorCaminho.back());   // para evitar chamadas recursivas após encontrar o caminho procurado 

				if ((pos != penult_pos) && !(PosJaVisitada(pos, caminho) && PosJaVisitada(ultima_pos, caminho)) && !iguais)
				{
					caminho.push_back(pos);

					if (CaminhoMelhor(caminho, melhorCaminho))		// se o caminho atual for melhor que O MELHOR, torne-o O MELHOR
						melhorCaminho = caminho;
				
					BuscaCaminho(pos->linha, pos->coluna, posFinal, tipoPeca, caminho, melhorCaminho);
					caminho.pop_back();
				}
			}			
		}		
	}
}

// Busca um caminho que termine na posicao focada
void Tabuleiro::BuscaCaminhoFoco(int linha, int coluna)
{
	std::vector<Posicao*> caminho;
	Posicao* posFoco = GetPosicao(linha, coluna);
	Peca* p = _pecaSelecionada;

	if (_caminhoFoco.size() > 0)
	{
		if (PosicoesIguais(posFoco, _caminhoFoco.back()))		// focando a última posição do caminho já traçado
		{
			return;
		}		
		else 
		{
			_caminhoFoco.clear();
		}
	}

	if (PosicaoJogavel(linha, coluna) && PecaSelecionada())
	{
		// Procura pelo caminho que termine na posição focada 
		BuscaCaminho(p->GetLinha(), p->GetColuna(), posFoco, p->GetTipo(), caminho, caminho);	// passa uma copia do caminho atual e o próprio que será utilizado para comparar com o melhor caminho global

		if (PodeSelecionar(posFoco->linha, posFoco->coluna))
		{			
			_caminhoFoco.push_back(_posSelecionadas.at(0));
			_caminhoFoco.push_back(posFoco);
		}

		if (caminho.size() > 0)
		{
			if (PosicoesIguais(posFoco, caminho.back()))			
				_caminhoFoco = caminho;			
		}	
	}
}

// Testa se a posicao passada já está inserida em um determinado caminho
bool Tabuleiro::PosJaVisitada(Posicao* pos, std::vector<Posicao*> & caminho)
{
	if (!caminho.empty())
	{
		std::vector<Posicao*>::iterator inicio = caminho.begin();
		std::vector<Posicao*>::iterator    fim = caminho.end();
		--fim;
		for(std::vector<Posicao*>::iterator it = inicio; it != fim; ++it) 
		{
			if (pos == *it) return (true);		
		}
	}
	return (false);
}

// Testa se o caminho1 é melhor que o caminho2
// Por enquanto avalia apenas se o tamanho do caminho é maior
// Melhor se avaliasse a vantagem da posicao final 
bool Tabuleiro::CaminhoMelhor(const std::vector<Tabuleiro::Posicao*> caminho1, const std::vector<Tabuleiro::Posicao*> caminho2)
{
	if (caminho1.size() > caminho2.size())
		return (true);
	else
		return (false);
}

// Testa se o caminho passado é o melhor
// Por enquanto avalia apenas se o tamanho do caminho é maior
// Melhor se avaliasse a vantagem da posicao final 
bool Tabuleiro::CaminhoOtimo(std::vector<Tabuleiro::Posicao*> caminho)
{
	if (caminho.size() > _melhorCaminho.size())
		return (true);
	else
		return (false);
}


// Inicia novo jogo
void Tabuleiro::NovoJogo()
{
	int tab[10][10] = { 0,2,0,2,0,2,0,2,0,2,
						0,0,1,0,1,0,1,0,1,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,1,0,1,0,2,0,1,0,2,
						0,0,2,0,2,0,2,0,1,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,1,0,0,0,0,0,0,0,
						0,2,0,1,0,1,0,1,0,0,
						0,0,2,0,2,0,2,0,0,0};

	/* COMENTADO ENQUANTO ESTIVER TESTANDO POSICIONAMENTOS ESPECÍFICOS NO TABULEIRO - lembrar de descomentar ao gerar versao final
	_qtPecasAI = ((TAMANHO/2)*((TAMANHO/2)-1));
	_qtPecasHumano = ((TAMANHO/2)*((TAMANHO/2)-1));

	*/ 
	_qtPecasAI = 12;
	_qtPecasHumano = 7;
	_temCaptura = false;
	_jogadorTurno = Jogador::Humano; 
	_vencedor     = Jogador::Nenhum;
	IniciaTabuleiro(tab);	 
}

int Tabuleiro::GetPecasAI()
{
	return (_qtPecasAI);
}

// Verifica se a posicao passada nao ultrapassa os limites do tabuleiro
bool Tabuleiro::PosicaoValida(int linha, int coluna) const
{
	return (0 <= linha && linha < TAMANHO && 0 <= coluna && coluna < TAMANHO);
}

// Verifica se a posicao passada esta vazia
bool Tabuleiro::PosicaoVazia(int linha, int coluna)
{	
	return(_posicao[linha][coluna].peca == nullptr?true:false);
}

//Executa a jogada do jogador AI
void Tabuleiro::JogadaAIPlayer()
{	
	// o codigo a seguir esta apenas para testes e deve ser apagado no futuro
	// ele faz com o jogadorAI simplesmente passe a vez e não efetue jogada
	if (!JogoAcabou())
				_jogadorTurno = Jogador::Humano;
	return;

	//bool achouMelhor = false;
	bool temCaptura  = false;
	std::vector<Peca*> pecasAI;	

	_melhorCaminho.clear();
	// Busca todas as capturas possíveis
	for (int i = 0; i < _tamanho; i++)
	{
		for (int j = 0; j < _tamanho; j++)
		{
			// Procura por uma peca do jogador AI
			if (PosicaoJogavel(i,j) && (!PosicaoVazia(i,j)))    
			{
				Peca* p = GetPecaEm(i,j);
				std::vector<Posicao*> caminho;
				if (p->GetDono() == Jogador::Computador)
				{
					pecasAI.push_back(p);
					SelecionaPosicao(i, j);   // seleciona a posição da peça

					// Procura pelo melhor caminho
					BuscaCaminho(i,j, p->GetTipo(), caminho, caminho);	// passa uma copia do caminho atual e o próprio que será utilizado para comparar com o melhor caminho global

					if (caminho.size() > 1)  // tem capturas possiveis no caminho retornado?
					{
						temCaptura = true;		
						if (CaminhoOtimo(caminho)) _melhorCaminho = caminho;
					}					
				}
			}		
		}
	}
	
	// Se tem uma captura obriga a fazer (a de maior alcance)
	if (temCaptura)
	{
		SelecionaCaminho(_melhorCaminho);				
		if (JogadaValida())		
			Jogada();		
	}
	else
	{
		bool temMovs = false;
		DesmarcaSelecionadas();	
		// Busca movimentos possíveis (iterando toda a lista de pecas do jogador AI montada anteriormente)
		for (int i=0; i < (int) pecasAI.size(); i++)	
		{
			Peca* p = pecasAI.at(i);
			SelecionaPosicao(p->GetLinha(), p->GetColuna());
			_movsPossiveis.clear();						
			BuscaMovimentos(p); 
			if (_movsPossiveis.size() > 0) 
			{
				Posicao* posMover = _movsPossiveis.at(0);
				SelecionaPosicao(posMover->linha, posMover->coluna);
				if (JogadaValida())
				{
					Jogada();
					break;					
				}
				else
					DeselecionaPosicao(p->GetLinha(), p->GetColuna());
			}			
		}

		// TODO: por enquanto o código acima faz a primeira jogada que encontra. Depois é preciso criar um método de valoração dos movimentos
		//       para que possa ser escolhido o melhor dos movimentos a serem feitos
		/*if (!temMovs)
		{
			_jogadorTurno = Jogador::Humano;
			if (TemCaptura())   // TODO: incluir aqui a verificação se há movimentos que podem ser feitos, além das capturas
			{					
				_vencedor = Jogador::Humano;
			}
			else
				_vencedor = Jogador::Nenhum;

			_jogadorTurno = Jogador::Nenhum;
		}*/
	}
	if (!JogoAcabou())
				_jogadorTurno = Jogador::Humano;
}

// Realiza a captura de uma peça
void Tabuleiro::CapturaPeca(Peca* peca)
{
	EsvaziaPosicao(peca->GetLinha(), peca->GetColuna());
	if (peca->GetDono() == Jogador::Humano)
		_qtPecasHumano--;
	else if (peca->GetDono() == Jogador::Computador) 
		_qtPecasAI--;
}


// Executa uma jogada, realizando a movimentação da peça da posição inicial para a posição final e nos 
// casos onde tem captura, exeucta a captura das peças que se encontram no caminho
void Tabuleiro::Jogada()
{	
	Peca* peca = GetPecaSelecionada();		// peca selecionada para a jogada
	 int linha = peca->GetLinha();			
	int coluna = peca->GetColuna();
	Posicao* pos = GetPosicao(linha, coluna);		// posicao da peça selecionada
	Posicao* pos_final = nullptr;					// ponteiro para a posicao final da jogada (nao conhecido a priori)		


	if (peca->GetTipo() == TipoPeca::Comum)
	{
		if (getTamanhoCaminho() == 2)  // jogada com apenas um movimento (captura ou movimento simples)
		{	
			pos_final = _posSelecionadas.at(1);				
			if ((getTamanhoCaminho() == 2) && (Distancia(pos, pos_final) == 1))  // jogada com apenas um movimento simples
			{
				MovePecaPara(peca, pos_final->linha, pos_final->coluna);
			}
			else   // jogada com uma única captura de peça
			{
				pos_final = _posSelecionadas.back();
				CapturaPecas();
				MovePecaPara(peca, pos_final->linha, pos_final->coluna);
			}			
		}
		else     // jogada com multiplas capturas
		{						
			pos_final = _posSelecionadas.back();
			CapturaPecas();
			MovePecaPara(peca, pos_final->linha, pos_final->coluna);
		}
	}
	else   // é uma dama
	{
		if (_temCaptura)
		{
			pos_final = _posSelecionadas.back();
			CapturaPecas();
			MovePecaPara(peca, pos_final->linha, pos_final->coluna);
		}
		else    // apenas movimentação
		{
			pos_final = _posSelecionadas.at(1);	
			MovePecaPara(peca, pos_final->linha, pos_final->coluna);
		}
	}
	
	_posicaoInicial = pos;
	_posicaoFinal   = pos_final;
	
	if ((((_jogadorTurno == Jogador::Humano)&&(pos_final->linha == 0)) ||
	   ((_jogadorTurno == Jogador::Computador )&&(pos_final->linha == 9))) &&	
	   (peca->GetTipo() != TipoPeca::Dama))			// não pode promover dama a dama
	{
		peca->promove();
	}

	DeselecionaPosicao(peca->GetLinha(), peca->GetColuna());
	//ZeraMovsPossiveis();	// Zera a lista de movimentos possíveis	
	DetectaVencedor();
	//DetectaEmpate();	
	_temCaptura = false;      // reseta o flag indicador de capturas
}

// Zera a lista de movimentos possíveis
void Tabuleiro::ZeraMovsPossiveis()
{
	_movsPossiveis.clear();
	_caminhoFoco.clear();   
}


// Detecta se há um vencedor
void Tabuleiro::DetectaVencedor()
{
	return;
	if ((_qtPecasAI == 0) && (_qtPecasHumano > 0))
		_vencedor = Jogador::Humano;
	else if ((_qtPecasHumano == 0) && (_qtPecasAI > 0))
		_vencedor = Jogador::Computador;

	// Se o jogador imobilizar todas as outras pecas do adversário será o vencedor

	if (_vencedor != Jogador::Nenhum)
		_jogadorTurno = Jogador::Nenhum;

	NotifyObserver();
}

// Detecta se ocorreu um empate
void Tabuleiro::DetectaEmpate()
{
	/*
	// if there is an empty Posicao, the game isn't a draw (yet).
	for (int i = 0; i < TAMANHO; ++i)
	{
		for (int j = 0; j < TAMANHO; ++j)
		{
			if (GetMarcacaoEm(Posicao(i, j)) == Marcacao::Vazio)
			{
				return;
			}
		}
	}
	
	// no empty Posicaos found: it's a draw.
	_playerInTurn = Marcacao::Vazio;*/
	return;
	NotifyObserver();
}

// Retorna a distancia entre duas posicoes dadas
// Cada jogada de uma peca comum deve ter no máximo tamanho 2 para capturas simples
// Podendo ter tamanho diferente caso haja capturas sequenciais
int Tabuleiro::Distancia(Posicao* pos1, Posicao* pos2)
{
	int distLinha  = abs(pos1->linha - pos2->linha);
	int distColuna = abs(pos1->coluna - pos2->coluna);

	int distancia = (int) floor(sqrt(pow(distLinha,2)+pow(distColuna,2)));

	return (distancia);
}

Tabuleiro::Posicao* Tabuleiro::PosicaoAdjacente(int linha, int coluna, Sentido sentido, Jogador jogador)
{
	int incrLinha=0;
	int incrColuna=0;
	bool inverso;
	Posicao* p = nullptr;		// posicao adjacente a ser retornada

	jogador == Jogador::Humano ? inverso=true : inverso=false;
	switch (sentido)
	{
		case Sentido::EsqFrente:
			if (inverso)
			{
				incrLinha  = -1;
				incrColuna = -1; 
			}
			else
			{
				incrLinha  = +1;
				incrColuna = +1;
			}
		break;

		case Sentido::EsqTras:
			if (inverso)
			{
				incrLinha  = +1;
				incrColuna = -1; 
			}
			else
			{
				incrLinha  = -1;
				incrColuna = +1;
			}
		break;

		case Sentido::DirFrente:
			if (inverso)
			{
				incrLinha  = -1;
				incrColuna = +1; 
			}
			else
			{
				incrLinha  = +1;
				incrColuna = -1;
			}
		break;

		case Sentido::DirTras:
			if (inverso)
			{
				incrLinha  = +1;
				incrColuna = +1; 
			}
			else
			{
				incrLinha  = -1;
				incrColuna = -1;
			}
		break;
	}

	if (PosicaoValida(linha+incrLinha, coluna+incrColuna))
		p = GetPosicao( linha+incrLinha, coluna+incrColuna);
	
	return (p);	
}

// Determina o setindo indo de uma posicao para a outra (pos1 ---> pos2)
// Verifica as diferencas entre as duas posicoes e determina a direcao de acordo
// 
Tabuleiro::Sentido Tabuleiro::GetSentido(Posicao* pos1, Posicao* pos2, Jogador jogador)
{
	if (jogador == Jogador::Nenhum) jogador = _jogadorTurno;

	 int linha1 = pos1->linha;
	int coluna1 = pos1->coluna;
	 int linha2 =  pos2->linha;
	int coluna2 = pos2->coluna;

	int incrLinha = (linha2 - linha1)/abs(linha2 - linha1);
	int incrColuna = (coluna2 - coluna1)/abs(coluna2 - coluna1);

	bool inverso;
	jogador == Jogador::Humano ? inverso=true : inverso=false;

	switch (incrLinha)
	{
		case 1:
			switch (incrColuna)
			{
				case 1:	// (1,1)
					if (inverso)
						return (Sentido::DirTras);
					else
						return (Sentido::EsqFrente);
					break;
				case -1:  // (1,-1)
					if (inverso)
						return (Sentido::EsqTras);
					else
						return (Sentido::DirFrente);
					break;				
			}
		case -1:
			switch (incrColuna)
			{
				case 1:  // (-1,1)
					if (inverso)
						return (Sentido::DirFrente);
					else
						return (Sentido::EsqTras);
					break;
				case -1:  // (-1,-1)
					if (inverso)
						return (Sentido::EsqFrente);
					else
						return (Sentido::DirTras);
					break;
			}
	}

}


// Esvazia a posição do tabuleiro, retirando o ponteiro para a posicao e setando o flag de ocupada da posição para
// false
void Tabuleiro::EsvaziaPosicao(int linha, int coluna)
{
	_posicao[linha][coluna].ocupada = false;
	(_posicao[linha][coluna]).peca = nullptr;
}


void Tabuleiro::RegisterTabuleiroChangedObserver(Observer* TabuleiroChangedObserver)
{
	_tabuleiroChangedObserver = TabuleiroChangedObserver;
}

void Tabuleiro::NotifyObserver()
{
	/*if (_tabuleiroChangedObserver != nullptr)
	{
		_tabuleiroChangedObserver->SubjectChanged();
	}  */
}
