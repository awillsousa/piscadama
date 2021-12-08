/*
 * BoardWindow class: Responsible for the game window and the GDI+ drawing.
 * Listens to notifications from the Board.
 * Owns the EyeX host.
 *
 * Copyright 2013 Tobii Technology AB. All rights reserved.
 */

#pragma once
//#define _WIN32_IE 0x0400

#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <commctrl.h>
#include <tchar.h>
#include "Observer.h"
#include "EyeXHost.h"

class Tabuleiro;

class TabuleiroWindow : Observer
{
public:
	TabuleiroWindow(Tabuleiro& tabuleiro, HINSTANCE hInstance, int nCmdShow);
	virtual ~TabuleiroWindow();

	// registers the window class; must be called once at application initialization.
	static void RegisterWindowClass(HINSTANCE hInstance);

	// Exibe um texto na barra de status
	const void MsgStatus(LPSTR textString);

	// From Observer
	void SubjectChanged();

private:
	// window procedure for the main window.
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	// window procedure for the About dialog.
	static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

	// Procedimento disparado na criação da window
	HWND DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts);
	void StartCommonControls(DWORD flags);
	//int OnCreate(HWND hwnd,CREATESTRUCT *cs);	
	//HWND CreateStatusbar(const HWND hParent,const HINSTANCE hInst,DWORD dwStyle,const RECT& rc,const int id);

	// GDI+ drawing methods.
	void OnDraw(HDC hdc);
	void DrawGrid(Gdiplus::Graphics *graphics);
	void DrawHighlight(Gdiplus::Graphics* graphics, int linha, int coluna);
	void DrawXMarker(Gdiplus::Graphics* graphics, Gdiplus::Pen* pen, int linha, int coluna, bool eDama=false);
	void DrawOMarker(Gdiplus::Graphics* graphics, Gdiplus::Pen* pen, int linha, int coluna, bool eDama=false);
	void DrawGameOver(Gdiplus::Graphics* graphics, WCHAR* message, const Gdiplus::Color& messageColor);

	// coordinate/board position mapping methods.
	int	TabuleiroToClientAreaX(int coluna);
	int	TabuleiroToClientAreaY(int linha);
	int	ClientAreaToTabuleiroColumn(int x);
	int	ClientAreaToTabuleiroRow(int y);

	// region id/board position mapping methods.
	int	TabuleiroPositionToRegionId(int linha, int coluna);
	int	RegionIdToTabuleiroRow(int regionId);
	int	RegionIdToTabuleiroColumn(int regionId);

	// re-calculates the margins based on the size of the window.
	void UpdateMargins();

	// reports all vacant board positions as activatable regions to the EyeX host.
	void UpdateActivatableRegions();

	// Realiza uma jogada do jogador humano
	bool JogadaHumano();
	//Realiza uma jogada do jogador AI
	void JogadaAI();

	// seleciona uma posicao (highlight)
	void SelecionaPosicao(int linha, int coluna);
	
	// Verifica se o foco mudou
	bool FocoMudou(UINT regionId);
	
	// usado para converter numeros para TCHAR*
	int PrintNumber(__in int nNumber);

	// event handlers.
	void OnSquareClicked(POINTS point);
	void OnSquareActivated(UINT regionId);
	void OnNewGameClicked();
	void OnFoco(UINT regionId);

	static const Gdiplus::Color BackgroundGazeInteractionFunctionalColor;
	static const Gdiplus::Color BackgroundMouseOnlyColor;
	static const Gdiplus::Color GridColor;
	static const Gdiplus::Color MarkerColor;
	static const Gdiplus::Color CorBordaHumano; 
	static const Gdiplus::Color CorBordaAI;
	static const Gdiplus::Color CorMarcaHumano;  
	static const Gdiplus::Color CorMarcaAI;
	static const Gdiplus::Color HighlightColor;
	static const Gdiplus::Color WonMessageColor;
	static const Gdiplus::Color LostMessageColor;
	static const Gdiplus::Color CorPosJogavel;
	static const Gdiplus::Color CorPosNaoJogavel;
	static const Gdiplus::Color CorPontoCentral;  
	static const Gdiplus::Color CorSelecao; 
	static const Gdiplus::Color CorFoco;  

	static const int SquareSize;
	static const int MarkerMargin;
	static const int MarkerDiameter;
	static const TCHAR* WindowClassName;

	Tabuleiro& _tabuleiro;
	EyeXHost _eyeXHost;
	HINSTANCE _hInstance;
	HWND _hWnd;
	HMENU _hMenu;
	HWND _hBarraStatus;
	int _leftMargin;
	int _upperMargin;
	UINT _idFocoAnt;
	UINT _timmerFoco;
	UINT _timerTempo;
	int _regiaoFocoID;
	DWORD _tempoSelecao;		// momento da seleção da peça
	DWORD _tempoJogada;			// momento da realização da jogada
	std::vector<DWORD> _tempoJogadas;

	// private copy constructor and operator making the class non-copyable (declared but not implemented).
	TabuleiroWindow(const TabuleiroWindow&);
	TabuleiroWindow& operator = (const TabuleiroWindow&);
};
