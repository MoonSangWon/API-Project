#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<string.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("����ܾ��");

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
		//���� ������ ������ �ѹ� �߻�...
		//�ʱ�ȭ���� �ڵ�..
		srand((unsigned int)time(NULL)); //seed
		GetClientRect(hWnd, &rect);	// ������ ������ ũ��
		SetTimer(hWnd, 1, 800, NULL);

		//	SendMessage(hWnd, WM_TIMER, 1, 0);
		j = rand() % 16;
		for (int i = 0; i < 20; i++) {
			str[i] = parament[j][i];
		}
		x = rand() % (rect.right - 40) + 20;	// ���� x ��ġ

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
		if ((TCHAR)wParam == '\b') // �� �����̽��� ���
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
			VARIABLE_PITCH | FF_ROMAN, "����");
		oldFont = (HFONT)SelectObject(hdc, hFont);

		wsprintf(str1, TEXT("��� ���ڼ� : %d"), fcount);//��� ���ڼ� �Է�
		TextOut(hdc, 10, 30, str1, lstrlen(str1));//��� ���ڼ� ���
		SetTextColor(hdc, RGB(255, 0, 0));
		wsprintf(str1, TEXT("���� ���ڼ� : %d"), score);//���� ���ڼ� �Է�
		TextOut(hdc, 10, 60, str1, lstrlen(str1));//���� ���ڼ� ���
		SetTextColor(hdc, RGB(0, 0, 0));
		DeleteObject(hFont);

		hFont = CreateFont(17, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
			VARIABLE_PITCH | FF_ROMAN, "����");
		oldFont = (HFONT)SelectObject(hdc, hFont);
		wsprintf(ch2, TEXT("�Է¹���--> "));//�Է� ���� ���� ��Ÿ����
		TextOut(hdc, 500, 600, ch2, lstrlen(ch2));

		TextOut(hdc, 610, 600, ch1, lstrlen(ch1));//�Է¹��� ���� ���
		TextOut(hdc, x, y, str, lstrlen(str));//���� ��������

		SetTextColor(hdc, RGB(0, 0, 0));
		DeleteObject(hFont);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_CLOSE:
		int check = MessageBox(hWnd, "���α׷��� �����Ͻðڽ��ϱ�?",
			"���� Ȯ��!", MB_ICONQUESTION | MB_OKCANCEL);
		// ����ڰ� ��� ��ư�� �����ٸ� return ���� ����Ͽ� DefWindowProc �Լ��� 
		// ȣ����� �ʵ��� �����ϴ�. 
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

��� ���ڼ� :

���� ���ڼ� :

���� ��������

98������ Ÿ�̸�, 119 WM_SIZE ������ ũ�� Ȱ��

*/