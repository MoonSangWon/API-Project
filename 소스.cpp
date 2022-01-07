#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<string.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("영어단어연습");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance

	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {

		TranslateMessage(&Message);
		DispatchMessage(&Message);

	}
	return (int)Message.wParam;
}
HWND hEdit; HFONT hFont,oldFont;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;
	static TCHAR str[256];
	static TCHAR str1[256];
	static int fcount = 0;
	static int score = 0;
	static char parament[][20] = { "Christmas","Premium","Previous","Question",
		"Experience","Believe","Table","Backspace","IronMan","SpiderMan","December",
		"Captain","Light","Natural","Powerful","Solution"};
	static char ch1[50];
	static char ch2[50];
	static int j = 0;
	static int x, y;
	int len =0;
	switch (iMessage) {
	case WM_CREATE:
		//최초 윈도우 생성시 한번 발생...
		//초기화관련 코드..
		srand((unsigned int)time(NULL)); //seed
		GetClientRect(hWnd, &rect);	// 생성된 윈도우 크기
		SetTimer(hWnd, 1, 800, NULL);

		//	SendMessage(hWnd, WM_TIMER, 1, 0);
		j = rand() % 16;
		for (int i = 0; i < 20; i++) {
			str[i] = parament[j][i];
		}
		x = rand() % (rect.right - 40) + 20;	// 최초 x 위치

		SendMessage(hWnd, WM_TIMER, 1, 0);
		return 0;
	case WM_SIZE:
		GetClientRect(hWnd, &rect);
		return 0;
	case WM_TIMER:
		y += 60;
		if (y >= rect.bottom)
		{
			for (int i = 0; i < 30; i++) {
				str[i] = parament[j][i];
				ch1[i] = 0;
			}
			j = rand() % 16;
			x = rand() % (rect.right - 40) + 20;
			y = 0;
			len = 0;
			fcount++;

		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_CHAR:
		len = lstrlen(ch1);
		if ((TCHAR)wParam == '\b') // 백 스페이스일 경우
			memmove(ch1 + (len - 1), ch1 + len, sizeof(TCHAR));
		else {
			ch1[len] = (TCHAR)wParam;
			ch1[len + 1] = 0;
		}
		if (strcmp(ch1, str) == 0) {
			score += 1;
			fcount += 1;
			for (int i = 0; i < 50; i++) {
				ch1[i] = 0;
				str[i] = parament[j][i];
			}
			len = 0;
			j = rand() % 16;
			x = rand() % (rect.right - 40) + 20;
			y = 0;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetTextColor(hdc, RGB(0, 0, 255));
		hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
			VARIABLE_PITCH | FF_ROMAN, "굴림");
		oldFont = (HFONT)SelectObject(hdc, hFont);

		wsprintf(str1, TEXT("모든 문자수 : %d"), fcount);//모든 문자수 입력
		TextOut(hdc, 10, 30, str1, lstrlen(str1));//모든 문자수 출력
		SetTextColor(hdc, RGB(255, 0, 0));
		wsprintf(str1, TEXT("성공 문자수 : %d"), score);//성공 문자수 입력
		TextOut(hdc, 10, 60, str1, lstrlen(str1));//성공 문자수 출력
		SetTextColor(hdc, RGB(0, 0, 0));
		DeleteObject(hFont);

		hFont = CreateFont(17, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
			VARIABLE_PITCH | FF_ROMAN, "굴림");
		oldFont = (HFONT)SelectObject(hdc, hFont);
		wsprintf(ch2, TEXT("입력문자--> "));//입력 받을 문자 나타내기
		TextOut(hdc, 500, 600, ch2, lstrlen(ch2));

		TextOut(hdc, 610, 600, ch1, lstrlen(ch1));//입력받을 문자 출력
		TextOut(hdc, x, y, str, lstrlen(str));//문자 내려오기

		SetTextColor(hdc, RGB(0, 0, 0));
		DeleteObject(hFont);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_CLOSE:
		int check = MessageBox(hWnd, "프로그램을 종료하시겠습니까?",
			"종료 확인!", MB_ICONQUESTION | MB_OKCANCEL);
		// 사용자가 취소 버튼을 눌렀다면 return 문을 사용하여 DefWindowProc 함수가 
		// 호출되지 않도록 막습니다. 
		if (IDCANCEL == check) {
			score = 0;
			fcount = 0;
			for (int i = 0; i < 50; i++) {
				str[i] = parament[j][i];
			}
			j = rand() % 16;
			x = rand() % (rect.right - 40) + 20;
			y = 0;
			return 0;
		}
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));

}



/*

모든 문자수 :

성공 문자수 :

방향 한쪽으로

98페이지 타이머, 119 WM_SIZE 윈도우 크기 활용

*/