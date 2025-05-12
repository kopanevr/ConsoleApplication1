#include <iostream>
#include <Windows.h>
#include <glad.h>

LRESULT CALLBACK WinProc(
	HWND	hWnd,
	UINT	uMsg,
	WPARAM	wParam,
	LPARAM	lParam)					// WindowProc (или WndProc) - это функция обработки сообщений окна (Window Procedure) в Windows API.
{
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPreviousInstance,

	LPSTR		lpCmdLine,			// Этот параметр содержит параметры командой строки, переданные при запуске приложения.
	int			nCmdShow)			// Этот параметр указывает, как изначально должно отображаться окно приложения.
{
	const char CLASS_NAME[] = "SampleWindowClass";

	WNDCLASSA wc = {};
	wc.lpfnWndProc		= WinProc;
	wc.hInstance		= hInstance;
	wc.lpszClassName	= CLASS_NAME;

	RegisterClassA(&wc);

	return 0;
}
