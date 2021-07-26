/*
 * ActivatableTabuleiroGame sample:
 * This is an example that demonstrates the use of Activatable interactors in the context of a Tabuleiro game.
 *
 * Copyright 2013 Tobii Technology AB. All rights reserved.
 */

#include "stdafx.h"
#include "TabuleiroWindow.h"
#include "Tabuleiro.h"
#include "resource.h"
#include "Strsafe.h"


// window messages used for notifications from the EyeXHost.
#define WM_EYEX_HOST_STATUS_CHANGED     WM_USER + 0
#define WM_FOCUSED_REGION_CHANGED       WM_USER + 1
#define WM_REGION_ACTIVATED             WM_USER + 2
#define WM_GAZE                         WM_USER + 3
#define WM_REGION_GOT_ACTIVATION_FOCUS  WM_USER + 4

// ID timmer foco
#define ID_TIMMERFOCO 101
#define ID_TIMMERTEMPO 102

// color scheme.
const Gdiplus::Color TabuleiroWindow::BackgroundGazeInteractionFunctionalColor = Gdiplus::Color(134, 152, 172);      // blue
const Gdiplus::Color TabuleiroWindow::BackgroundMouseOnlyColor                 = Gdiplus::Color(164, 156, 153);      // grey
const Gdiplus::Color TabuleiroWindow::GridColor                                = Gdiplus::Color(110, 57, 88, 109);   // dark blue
const Gdiplus::Color TabuleiroWindow::MarkerColor                              = Gdiplus::Color(242, 233, 216);      // off white
const Gdiplus::Color TabuleiroWindow::CorMarcaHumano                           = Gdiplus::Color(0, 0, 0);    
const Gdiplus::Color TabuleiroWindow::CorBordaHumano                           = Gdiplus::Color(205,92,92); 
const Gdiplus::Color TabuleiroWindow::CorBordaAI                               = Gdiplus::Color(128,128,128); 
const Gdiplus::Color TabuleiroWindow::CorMarcaAI                               = Gdiplus::Color(139,0,0);      
const Gdiplus::Color TabuleiroWindow::HighlightColor                           = Gdiplus::Color(175,238,238);  // azulzinho frufru
const Gdiplus::Color TabuleiroWindow::CorFoco                                  = Gdiplus::Color(124,252,0);  // azulzinho frufru
const Gdiplus::Color TabuleiroWindow::WonMessageColor                          = Gdiplus::Color(34,139,34);    
const Gdiplus::Color TabuleiroWindow::LostMessageColor                         = Gdiplus::Color(255,99,71); 
const Gdiplus::Color TabuleiroWindow::CorPosNaoJogavel                         = Gdiplus::Color(139,69,19);		 
const Gdiplus::Color TabuleiroWindow::CorPosJogavel                            = Gdiplus::Color(255,235,205);			 
const Gdiplus::Color TabuleiroWindow::CorPontoCentral                          = Gdiplus::Color(255, 255, 255);		 // branco
const Gdiplus::Color TabuleiroWindow::CorSelecao                               = Gdiplus::Color(255,255,51);			// amarelo brilhante


// constants.
const int TabuleiroWindow::SquareSize = 70;
const int TabuleiroWindow::MarkerMargin = 10;
const int TabuleiroWindow::MarkerDiameter = SquareSize - MarkerMargin * 2;
const TCHAR* TabuleiroWindow::WindowClassName = _T("PiscaDama");

// enum utilizada para identificação da barra de status
enum {
  IDC_STATUSBAR=200,
};

TabuleiroWindow::TabuleiroWindow(Tabuleiro& tabuleiro, HINSTANCE hInstance, int nCmdShow)
	: _tabuleiro(tabuleiro), _hInstance(hInstance), _hWnd(0), _hMenu(0), _leftMargin(0), _upperMargin(0)
{
	// create the window instance.
	_hWnd = CreateWindow(
		WindowClassName, 
		_T("DAMA DOS OLHOS"), 
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		(_tabuleiro.Tamanho() + 1) * SquareSize, 
		(_tabuleiro.Tamanho() + 1) * SquareSize + 120,
		NULL, 
		NULL, 
		hInstance, 
		NULL);

	if (!_hWnd)
	{
		throw new std::runtime_error("Could not create main window.");
	}

	_hMenu = GetMenu(_hWnd);

	// Cria barra de status
/*	_hBarraStatus = DoCreateStatusBar(_hWnd, IDC_STATUSBAR, hInstance, 1);
	int Statwidths[] = {800, 810, -1};
	SendMessage(_hBarraStatus, SB_SETPARTS, (WPARAM)(sizeof(Statwidths)/sizeof(int)), (LPARAM)Statwidths);	
	TabuleiroWindow::MsgStatus((LPSTR) TEXT("Hello"));
*/
	// write the "this" pointer to the extra window memory so that we can reference back to this 
	// object from the window function.
	SetWindowLongPtr(_hWnd, 0, (LONG_PTR)this);
	
	ShowWindow(_hWnd, nCmdShow);
	UpdateWindow(_hWnd);

	_tabuleiro.RegisterTabuleiroChangedObserver(this);
	
	// initialize the EyeX host and the activatable regions.
	_eyeXHost.Init(_hWnd, WM_EYEX_HOST_STATUS_CHANGED, WM_FOCUSED_REGION_CHANGED, WM_REGION_ACTIVATED, WM_GAZE);
	UpdateActivatableRegions();

	// Configura o timer de verificação do foco
	_timmerFoco = SetTimer( _hWnd , ID_TIMMERFOCO , 200 , (TIMERPROC) NULL);
	_timerTempo = SetTimer( _hWnd, ID_TIMMERTEMPO, 1000, (TIMERPROC) NULL);
	if (_eyeXHost.IsFunctional())
	{
		_regiaoFocoID = _eyeXHost.GetFocusedRegionId();
	}
		
	_tempoJogadas = std::vector<DWORD>();
	_tempoJogada = 0;
	_tempoSelecao = 0;
}

const void TabuleiroWindow::MsgStatus(LPSTR textString)
{
	SendMessage((HWND) _hBarraStatus, (UINT) SB_SETTEXT, (WPARAM)(INT) 0 | 0, (LPARAM) textString);
}

TabuleiroWindow::~TabuleiroWindow()
{
}

void TabuleiroWindow::RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= sizeof(void*); // make room for the "this" pointer.
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GOBANG));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= 0;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GOBANG);
	wcex.lpszClassName	= WindowClassName;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);
}

// Callback function invoked when the Tabuleiro has changed.
void TabuleiroWindow::SubjectChanged()
{
	InvalidateRect(_hWnd, NULL, FALSE);
	UpdateActivatableRegions();
}

// Callback function invoked from the message loop to process a Windows message.
LRESULT CALLBACK TabuleiroWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	// read the "this" pointer from extra window memory.
	TabuleiroWindow* instance = reinterpret_cast<TabuleiroWindow*>(GetWindowLongPtr(hWnd, 0));

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_NEWGAME:
			instance->OnNewGameClicked();
			break;

		case IDM_ABOUT:
			DialogBox(instance->_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			// mouse button pressed; possibly on a Tabuleiro square.
			auto point = MAKEPOINTS(lParam);
			instance->OnSquareClicked(point);
			instance->UpdateMargins();
			hdc = GetDC(hWnd);
				instance->OnDraw(hdc);
			ReleaseDC(hWnd, hdc); 
		}
		break;

	case WM_RBUTTONDOWN:
		{
			// botão direito do mouse pressionado, executa jogada (Humano)
			auto point = MAKEPOINTS(lParam);			
			bool humanoJogou = instance->JogadaHumano();
			if (humanoJogou)
			{
				instance->UpdateMargins();
				hdc = GetDC(hWnd);
					instance->OnDraw(hdc);
				ReleaseDC(hWnd, hdc);
				
				instance->JogadaAI();				
				hdc = GetDC(hWnd);								
					Sleep(500);				
					instance->UpdateMargins();				
					instance->OnDraw(hdc);
				ReleaseDC(hWnd, hdc);
			}
		}
		break;

    case WM_KEYUP:
		if (VK_SPACE == wParam)
		{
			instance->JogadaHumano();
			instance->UpdateMargins();
			hdc = GetDC(hWnd);
				instance->OnDraw(hdc);
			ReleaseDC(hWnd, hdc); 
		}
		break;

	case WM_PAINT:
		instance->UpdateMargins();
		hdc = BeginPaint(hWnd, &ps);
			instance->OnDraw(hdc);
		EndPaint(hWnd, &ps);
		break;
	
	case WM_MOUSEMOVE:
		{
		  TRACKMOUSEEVENT tme;
		  tme.cbSize = sizeof(TRACKMOUSEEVENT);
		  tme.dwFlags = TME_HOVER;
		  tme.dwHoverTime = 100;
		  tme.hwndTrack = hWnd;
		  TrackMouseEvent(&tme);
		}
      break;

	case WM_MBUTTONDOWN:
		{
			auto point = MAKEPOINTS(lParam);			
			instance->UpdateMargins();
			hdc = BeginPaint(hWnd, &ps);
				instance->OnDraw(hdc);
			EndPaint(hWnd, &ps);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

	case WM_ERASEBKGND:
		// no background erasing needed since our OnDraw method draws the entire window.
		return TRUE;

	case WM_EYEX_HOST_STATUS_CHANGED:
		// the background color of the Tabuleiro indicates whether we have a working connection to the engine or not.
		// so, when the status changes, we force a redraw of the window.
		InvalidateRect(hWnd, 0, FALSE);
		break;

	case WM_FOCUSED_REGION_CHANGED:
			InvalidateRect(hWnd, 0, FALSE);
		break;

	case WM_REGION_ACTIVATED:
		instance->OnSquareActivated((UINT)wParam);
		break;
			
	case WM_WINDOWPOSCHANGED:
		// the window was moved, so we need to refresh the screen bounds of our activatable regions.
		instance->UpdateActivatableRegions();
		break;
		
	case WM_GAZE:
		{
		
		}
		break;

	case WM_TIMER:
		{
			if (instance->_eyeXHost.IsFunctional())
			{
				int regiaoFocoID = instance->_eyeXHost.GetFocusedRegionId();
				
				if (regiaoFocoID >= 0)
				{
					if (instance->_regiaoFocoID == regiaoFocoID)     // foco permanece
					{					
						instance->OnFoco((UINT)regiaoFocoID);
					}
					else // foco mudou
					{
						instance->_regiaoFocoID = regiaoFocoID;
						int linha = instance->RegionIdToTabuleiroRow(regiaoFocoID);
						int coluna = instance->RegionIdToTabuleiroColumn(regiaoFocoID);
						instance->SelecionaPosicao(linha, coluna);
					}
				}

				instance->UpdateMargins();
				hdc = BeginPaint(hWnd, &ps);
					instance->OnDraw(hdc);
				EndPaint(hWnd, &ps);
				InvalidateRect(hWnd, NULL, FALSE);

				if (!instance->_tabuleiro.TurnoHumano())
				{
					instance->JogadaAI();				
					hdc = GetDC(hWnd);								
						Sleep(500);				
						instance->UpdateMargins();				
						instance->OnDraw(hdc);
					ReleaseDC(hWnd, hdc);
				}
			}
		}
		break;

	case WM_DESTROY:
		KillTimer(hWnd, ID_TIMMERFOCO);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}



// Cria a barra de status
HWND TabuleiroWindow::DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts)
{
    HWND hwndStatus;
    RECT rcClient;
    HLOCAL hloc;
    PINT paParts;
    int i, nWidth;

    // Ensure that the common control DLL is loaded.
    InitCommonControls();

    // Create the status bar.
    hwndStatus = CreateWindowEx(
        0,                       // no extended styles
        STATUSCLASSNAME,         // name of status bar class
        (PCTSTR) NULL,           // no text when first created
        SBARS_SIZEGRIP |         // includes a sizing grip
        WS_CHILD | WS_VISIBLE,   // creates a visible child window
        0, 0, 0, 0,              // ignores size and position
        hwndParent,              // handle to parent window
        (HMENU) idStatus,       // child window identifier
        hinst,                   // handle to application instance
        NULL);                   // no window creation data

    // Get the coordinates of the parent window's client area.
    GetClientRect(hwndParent, &rcClient);

    // Allocate an array for holding the right edge coordinates.
    hloc = LocalAlloc(LHND, sizeof(int) * cParts);
    paParts = (PINT) LocalLock(hloc);

    // Calculate the right edge coordinate for each part, and
    // copy the coordinates to the array.
    nWidth = rcClient.right / cParts;
    int rightEdge = nWidth;
    for (i = 0; i < cParts; i++) { 
       paParts[i] = rightEdge;
       rightEdge += nWidth;
    }

    // Tell the status bar to create the window parts.
    SendMessage(hwndStatus, SB_SETPARTS, (WPARAM) cParts, (LPARAM)
               paParts);

    // Free the array, and return.
    LocalUnlock(hloc);
    LocalFree(hloc);
    return hwndStatus;
} 

void TabuleiroWindow::StartCommonControls(DWORD flags)
{
//load the common controls dll, specifying the type of control(s) required 
INITCOMMONCONTROLSEX iccx;
iccx.dwSize=sizeof(INITCOMMONCONTROLSEX);
iccx.dwICC=flags;
InitCommonControlsEx(&iccx);
}


void TabuleiroWindow::OnDraw(HDC hdc)
{
	// use double-buffer drawing to speed up the drawing and reduce flicker.
	// (the drawing is performed on a bitmap, which is then blitted to the screen in one fell swoop.)

	RECT rect;
	GetClientRect(_hWnd, &rect);
	Gdiplus::Bitmap bitmap(rect.right - rect.left, rect.bottom - rect.top);
	Gdiplus::Graphics graphics(&bitmap);

	// draw the background in a color which depends on the state of the connection to the engine.
	// blue if the connection is functional, gray otherwise.
	Gdiplus::Color backgroundColor(BackgroundGazeInteractionFunctionalColor);
	if (!_eyeXHost.IsFunctional())
	{
		backgroundColor = BackgroundMouseOnlyColor;
	}
	Gdiplus::SolidBrush backgroundBrush(backgroundColor);
	graphics.FillRectangle(&backgroundBrush, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);

	// draw the grid
	DrawGrid(&graphics);

	// draw the markers		
	Gdiplus::Pen bordaHumano(CorBordaHumano, 3.0F);
	Gdiplus::Pen bordaAI(CorBordaAI, 3.0F);
		

	Gdiplus::SolidBrush pontoBrush(CorPontoCentral);	
	Gdiplus::SolidBrush destaqueBrush(CorSelecao);
	Gdiplus::SolidBrush highLightBrush(HighlightColor);
	Gdiplus::SolidBrush focoBrush(CorFoco);
	
	// draw the focus highlight
	int focusedRegionId = _eyeXHost.GetFocusedRegionId();
	if (focusedRegionId >= 0)
	{
		//DrawHighlight(&graphics, RegionIdToTabuleiroRow(focusedRegionId), RegionIdToTabuleiroColumn(focusedRegionId));
		int linha = RegionIdToTabuleiroRow(focusedRegionId);
		int coluna = RegionIdToTabuleiroColumn(focusedRegionId);
		int x = TabuleiroToClientAreaX(coluna) + 1;
		int y = TabuleiroToClientAreaY(linha) + 1;
		graphics.FillRectangle(&focoBrush, x, y, SquareSize - 1, SquareSize - 1);
	}

	// Desenha o retangulo que marca as posições selecionadas
	for (int linha = 0; linha < _tabuleiro.Tamanho(); linha++)
	{
		for (int col = 0; col < _tabuleiro.Tamanho(); col++)
		{
			if (_tabuleiro.PosicaoSelecionada(linha, col) || _tabuleiro.PosFinalouInicial(linha, col))
			{
				Gdiplus::Pen penSelecao(CorSelecao, 2.0F);				
				penSelecao.SetDashStyle(Gdiplus::DashStyle::DashStyleSolid);
				int x = TabuleiroToClientAreaX(col) + 1;
				int y = TabuleiroToClientAreaY(linha) + 1;
				graphics.DrawRectangle(&penSelecao, x, y, SquareSize, SquareSize);				
			}
						
			if (!_tabuleiro.PosicaoVazia(linha, col))
			{
				Peca* p = _tabuleiro.GetPecaEm(linha, col);				
				switch (p->GetDono())
				{
				case Jogador::Humano:					
					DrawXMarker(&graphics, &bordaHumano, linha, col, p->eDama());
					break;

				case Jogador::Computador:					
					DrawOMarker(&graphics, &bordaAI, linha, col, p->eDama());					
					break;
				}				
			}			
		}
	}

	// Desenha os retangulos preenchido que destaca os movimentos possíveis
	for (int i=0; i < (int) _tabuleiro._movsPossiveis.size(); i++)
	{
		Tabuleiro::Posicao* p = _tabuleiro._movsPossiveis.at(i);
		int x = TabuleiroToClientAreaX(p->coluna) + 1;
		int y = TabuleiroToClientAreaY(p->linha) + 1;
		graphics.FillRectangle(&highLightBrush, x, y, SquareSize - 1, SquareSize - 1);
	}
	// Tentativa de exibir informações da peça selecionada na barra de status
	/*
	Peca* peca = _tabuleiro.GetPecaSelecionada();
	
	TCHAR msg[100]; 
	size_t cchDest = sizeof(msg);

	LPCTSTR msgFormat = TEXT("%s %d ,%d ");
	TCHAR* msgTxt = TEXT("Peca: ");
	HRESULT hr = StringCchPrintf(msg, cchDest, msgFormat, msgTxt, peca->GetLinha(), peca->GetColuna());

	TabuleiroWindow::MsgStatus((LPSTR) msg);
*/
	
	// destaca o caminho determinado entre a posição focada e a posição selecionada
	for (int i=1; i < (int) _tabuleiro._caminhoFoco.size(); i++)
	{
		Tabuleiro::Posicao* p = _tabuleiro._caminhoFoco.at(i);
		int x = TabuleiroToClientAreaX(p->coluna) + 1;
		int y = TabuleiroToClientAreaY(p->linha) + 1;
		DrawHighlight(&graphics, p->linha, p->coluna);
		graphics.FillRectangle(&focoBrush, x, y, SquareSize - 1, SquareSize - 1);
	}
	
	for (int linha = 0; linha < _tabuleiro.Tamanho(); linha++)
		for (int col = 0; col < _tabuleiro.Tamanho(); col++)
		{
			// Desenha um ponto no centro da peca para (auxiliar a) guiar o olhar
			if (((linha%2==0) && (col%2==1))||((linha%2==1) && (col%2==0)))
			{
				auto x = _leftMargin + (col + 0.5) * SquareSize;
				auto y = _upperMargin + (linha + 0.5) * SquareSize;
				graphics.FillEllipse(&pontoBrush, (Gdiplus::REAL)(x - 1), (Gdiplus::REAL)(y - 1), (Gdiplus::REAL)3, (Gdiplus::REAL)3);
			}
		}

		
	// draw the game over message
	if (_tabuleiro.JogoAcabou())
	{
		WCHAR* message;
		Gdiplus::Color messageColor(0,0,0);
		switch (_tabuleiro.BuscaVencedor())
		{
		case Jogador::Humano:
			message = L"VOCÊ GANHOU :D";
			messageColor = WonMessageColor;
			break;

		case Jogador::Computador:
			message = L"VOCÊ PERDEU :(";
			messageColor = LostMessageColor;
			break;

		default:
			message = L"EMPATE";
			messageColor = LostMessageColor;
			break;
		}

		DrawGameOver(&graphics, message, messageColor);
	}

	Gdiplus::Graphics screen(hdc);
	screen.DrawImage(&bitmap, 0, 0);
}

void TabuleiroWindow::DrawGrid(Gdiplus::Graphics *graphics)
{
	const int lineMargin = 0;
		
	Gdiplus::Pen gridPen(GridColor);
	gridPen.SetDashStyle(Gdiplus::DashStyleDash);
	Gdiplus::SolidBrush PosJogavelBrush(CorPosJogavel);
	Gdiplus::SolidBrush PosNaoJogavelBrush(CorPosNaoJogavel);

	// horizontal lines
	auto x0 = _leftMargin - lineMargin;
	auto x1 = _leftMargin + _tabuleiro.Tamanho() * SquareSize + lineMargin;
	for (int i = 0; i <= _tabuleiro.Tamanho(); i++)
	{		
		auto y = _upperMargin + i * SquareSize;
		graphics->DrawLine(&gridPen, x0, y, x1, y);
	}

	// vertical lines
	auto y0 = _upperMargin - lineMargin;
	auto y1 = _upperMargin + _tabuleiro.Tamanho() * SquareSize + lineMargin;
	for (int j = 0; j <= _tabuleiro.Tamanho(); j++)
	{
		auto x = _leftMargin + j * SquareSize;
		graphics->DrawLine(&gridPen, x, y0, x, y1);
	}

	for (int i = 0; i <= _tabuleiro.Tamanho(); i++)
	{
		for (int j = 0; j <= _tabuleiro.Tamanho(); j++)
		{			
			int x = TabuleiroToClientAreaX(i) + 1;
			int y = TabuleiroToClientAreaY(j) + 1;
			
			if ((j < _tabuleiro.Tamanho()) && (i < _tabuleiro.Tamanho()))
			{
				if (((i%2==0) && (j%2==1)) || ((i%2==1) && (j%2==0)))  // Posicoes jogaveis do tabuleiro
				{
					graphics->FillRectangle(&PosJogavelBrush, x, y, SquareSize - 1, SquareSize - 1);					
				}
				else	// Posicoes nao jogaveis
				{					
					graphics->FillRectangle(&PosNaoJogavelBrush, x, y, SquareSize - 1, SquareSize - 1);
				}	
			}
		}
	}

	// dots that guide the eye to the center of the square
	Gdiplus::SolidBrush gridBrush(GridColor);
	for (int i = 0; i <= _tabuleiro.Tamanho(); i++)
	{
		for (int j = 0; j <= _tabuleiro.Tamanho(); j++)
		{
			auto x = _leftMargin + (j + 0.5) * SquareSize;
			auto y = _upperMargin + (i + 0.5) * SquareSize;
			graphics->FillEllipse(&gridBrush, (Gdiplus::REAL)(x - 1), (Gdiplus::REAL)(y - 1), (Gdiplus::REAL)3, (Gdiplus::REAL)3);
		}
	}
}

void TabuleiroWindow::DrawHighlight(Gdiplus::Graphics *graphics, INT linha, INT coluna)
{
	Gdiplus::SolidBrush highlightBrush(HighlightColor);

	int x = TabuleiroToClientAreaX(coluna) + 1;
	int y = TabuleiroToClientAreaY(linha) + 1;
		
	graphics->FillRectangle(&highlightBrush, x, y, SquareSize - 1, SquareSize - 1);
}

void TabuleiroWindow::DrawOMarker(Gdiplus::Graphics *graphics, Gdiplus::Pen *markerPen, INT linha, INT coluna, bool eDama)
{
	Gdiplus::SolidBrush brushHumano(CorMarcaHumano);	
	int x = TabuleiroToClientAreaX(coluna) + MarkerMargin;
	int y = TabuleiroToClientAreaY(linha) + MarkerMargin;		
	graphics->FillEllipse(&brushHumano, x, y, MarkerDiameter-1, MarkerDiameter-1);
	graphics->DrawEllipse(markerPen, x, y, MarkerDiameter, MarkerDiameter);	
	if (eDama) 
	{
		Gdiplus::SolidBrush brushAI(Gdiplus::Color::Green);
		graphics->FillEllipse(&brushAI, x+MarkerDiameter/4, y+MarkerDiameter/4, MarkerDiameter/2, MarkerDiameter/2);
	}

}

void TabuleiroWindow::DrawXMarker(Gdiplus::Graphics *graphics, Gdiplus::Pen *markerPen, INT linha, INT coluna, bool eDama)
{
	Gdiplus::SolidBrush brushAI(CorMarcaAI);
	int x = TabuleiroToClientAreaX(coluna) + MarkerMargin;
	int y = TabuleiroToClientAreaY(linha) + MarkerMargin;			
	graphics->FillEllipse(&brushAI, x, y, MarkerDiameter-1, MarkerDiameter-1);
	graphics->DrawEllipse(markerPen, x, y, MarkerDiameter, MarkerDiameter);
	if (eDama) 
	{
		Gdiplus::SolidBrush brushHumano(Gdiplus::Color::Yellow);
		graphics->FillEllipse(&brushHumano, x+MarkerDiameter/4, y+MarkerDiameter/4, MarkerDiameter/2, MarkerDiameter/2);
	}
}

void TabuleiroWindow::DrawGameOver(Gdiplus::Graphics *graphics, WCHAR* message, const Gdiplus::Color& messageColor)
{
   Gdiplus::Font myFont(L"Arial", 72);

   RECT rect;
   GetClientRect(_hWnd, &rect);
   Gdiplus::RectF layoutRect((float)rect.left, (float)rect.top, (float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
   
   Gdiplus::StringFormat format;
   format.SetAlignment(Gdiplus::StringAlignmentCenter);
   format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

   Gdiplus::SolidBrush messageBrush(messageColor);

   graphics->DrawString(
	   message,
	   (INT)wcslen(message),
	   &myFont,
	   layoutRect,
	   &format,
	   &messageBrush);
}

INT TabuleiroWindow::TabuleiroToClientAreaX(INT coluna)
{
	return coluna * SquareSize + _leftMargin;
}

INT TabuleiroWindow::TabuleiroToClientAreaY(INT linha)
{
	return linha * SquareSize + _upperMargin;
}

INT TabuleiroWindow::ClientAreaToTabuleiroColumn(INT x)
{
	//return (x - _upperMargin) / SquareSize;
	return (x - _leftMargin) / SquareSize;
}

INT TabuleiroWindow::ClientAreaToTabuleiroRow(INT y)
{
	//return (y - _leftMargin) / SquareSize;
	return (y - _upperMargin) / SquareSize;
}

int TabuleiroWindow::TabuleiroPositionToRegionId(int linha, int coluna)
{
	return coluna + linha * _tabuleiro.Tamanho();
}

int TabuleiroWindow::RegionIdToTabuleiroRow(int regionId)
{
	return regionId / _tabuleiro.Tamanho();
}

int TabuleiroWindow::RegionIdToTabuleiroColumn(int regionId)
{
	return regionId % _tabuleiro.Tamanho();
}

void TabuleiroWindow::UpdateMargins()
{
	RECT rect;
	GetClientRect(_hWnd, &rect);
	_upperMargin = ((rect.bottom - rect.top) - _tabuleiro.Tamanho() * SquareSize) / 2;
	_leftMargin = ((rect.right - rect.left) - _tabuleiro.Tamanho() * SquareSize) / 2;
}

void TabuleiroWindow::OnSquareClicked(POINTS point)
{
	int linha = ClientAreaToTabuleiroRow(point.y);
	int coluna = ClientAreaToTabuleiroColumn(point.x);
	if (_tabuleiro.PosicaoJogavel(linha, coluna))
		SelecionaPosicao(linha, coluna);	
}

void TabuleiroWindow::OnSquareActivated(UINT regionId)
{	
	int linha = RegionIdToTabuleiroRow(regionId);
	int coluna = RegionIdToTabuleiroColumn(regionId);
	if (_tabuleiro.PosicaoJogavel(linha, coluna))
		SelecionaPosicao(linha, coluna);	
}

bool TabuleiroWindow::JogadaHumano()
{
	
	// Verifica se há uma peca selecionada
	if (_tabuleiro.JogadaValida())
	{		
			_tabuleiro.JogadaHumano();	
			return (true);
	}
	else
	{
		// Verifica se tem peca selecionada
		if (_tabuleiro.PecaSelecionada())
			int i = 0;  // avisa que é preciso selecionar uma peca primeiro para fazer jogada
		else if (_tabuleiro.getTamanhoCaminho() == 1)
			int i = 1; // informar que  tem peca selecionada, mas não tem outra posição selecionada
		else
			int i = 2;	// informar que o erro é de outra natureza 

		return (false);
	}
		
	
	/*Tabuleiro::Posicao posicao(linha, coluna);
	if (_tabuleiro.PodeMoverPara(posicao))
	{
		_tabuleiro.JogadaHumano(posicao);
	}*/
}

void TabuleiroWindow::JogadaAI()
{
	_tabuleiro.JogadaAIPlayer();
}

void TabuleiroWindow::UpdateActivatableRegions()
{
	std::vector<EyeXHost::ActivatableRegion> regions;

	if (!_tabuleiro.JogoAcabou())
	{
		for (int linha = 0; linha < _tabuleiro.Tamanho(); linha++)
		{
			for (int coluna = 0; coluna < _tabuleiro.Tamanho(); coluna++)
			{
				POINT point;
				point.x = TabuleiroToClientAreaX(coluna);
				point.y = TabuleiroToClientAreaY(linha);
				ClientToScreen(_hWnd, &point);

				RECT bounds;
				bounds.left = point.x;
				bounds.top = point.y;
				bounds.right = point.x + SquareSize;
				bounds.bottom = point.y + SquareSize;

				regions.push_back(EyeXHost::ActivatableRegion(TabuleiroPositionToRegionId(linha, coluna), bounds));
			}
		}
	}

	_eyeXHost.SetActivatableRegions(regions);
}

void TabuleiroWindow::OnNewGameClicked()
{
	_tabuleiro.NovoJogo();
	UpdateActivatableRegions();
}

void TabuleiroWindow::SelecionaPosicao(int linha, int coluna)
{	
	Peca* p = nullptr;
	_tabuleiro.ZeraPosicoes();
	if (_tabuleiro.PecaSelecionada())
	{		
		p = _tabuleiro.GetPecaSelecionada();
	}

	_tabuleiro.SelecionaPosicao(linha, coluna);
	if ((_tabuleiro.PecaSelecionada()) && (p != _tabuleiro.GetPecaSelecionada()))
		this->_tempoSelecao = GetTickCount();
}

/*
void TabuleiroWindow::OnFoco(POINTS ponto)
{
	int linha = ClientAreaToTabuleiroRow(ponto.y);
	int coluna = ClientAreaToTabuleiroColumn(ponto.x);

	// Busca um caminho que termine na posicao focada
	// Se houver uma posição selecionada, busca um caminho a partir daquela posição
	if (_tabuleiro.PecaSelecionada())
		_tabuleiro.BuscaCaminhoFoco(linha, coluna);
}*/

bool TabuleiroWindow::FocoMudou(UINT regionId)
{
	return (regionId != _idFocoAnt);
}

void TabuleiroWindow::OnFoco(UINT regionId)
{
	int linha  = RegionIdToTabuleiroRow(regionId);
	int coluna = RegionIdToTabuleiroColumn(regionId);

	// Busca um caminho que termine na posicao focada
	// Se houver uma posição selecionada, busca um caminho a partir daquela posição
	if (_tabuleiro.PecaSelecionada())
	{		
		_tabuleiro.BuscaCaminhoFoco(linha, coluna);
		if (JogadaHumano())
		{
			this->_tempoJogada = GetTickCount();
			this->_tempoJogada -= _tempoSelecao;
			this->_tempoJogadas.push_back(_tempoJogada);
		}

	}
	else
	{
		if (_tabuleiro.PosicaoJogavel(linha, coluna))
		{						
			SelecionaPosicao(linha, coluna);
		}
	}
}

// Message handler for about box.
INT_PTR CALLBACK TabuleiroWindow::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}
