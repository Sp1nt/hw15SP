#include "mainDlg.h"

#define _CRT_SECURE_NO_WARNINGS

using namespace std;


TaskTimer* TaskTimer::ptr = NULL;



struct ThreadParams {
	int index;

};


TaskTimer::TaskTimer(void)
{
	ptr = this;
	pNID = new NOTIFYICONDATA;
}

TaskTimer::~TaskTimer(void)
{
	delete pNID;
}

void TaskTimer::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}


BOOL TaskTimer::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{


	hTasks = GetDlgItem(hwnd, IDC_LIST1);
	hDate = GetDlgItem(hwnd, IDC_LIST3);
	hStatus = GetDlgItem(hwnd, IDC_LIST2);


	hBAddTask = GetDlgItem(hwnd, IDC_BUTTON1);
	hBEditTask = GetDlgItem(hwnd, IDC_BUTTON2);
	hBDeleteTask = GetDlgItem(hwnd, IDC_BUTTON3);
	hBFinishTask = GetDlgItem(hwnd, IDC_BUTTON4);


	hEditTask = GetDlgItem(hwnd, IDC_EDIT1);
	hEditDateH = GetDlgItem(hwnd, IDC_EDIT2);
	hEditDateM = GetDlgItem(hwnd, IDC_EDIT3);
	hEditDateS = GetDlgItem(hwnd, IDC_EDIT4);



	hDialog = hwnd;

	return TRUE;
}






DWORD WINAPI Thread(LPVOID lp)
{
	TaskTimer* p = (TaskTimer*)lp;
	int index = p->currentTaskIndex;
	HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	TCHAR buf[10];
	int hours, minutes, seconds;
	GetWindowText(p->hEditDateH, buf, 10);
	hours = _tstoi(buf);
	GetWindowText(p->hEditDateM, buf, 10);
	minutes = _tstoi(buf);
	GetWindowText(p->hEditDateS, buf, 10);
	seconds = _tstoi(buf);
	SYSTEMTIME st;
	GetLocalTime(&st);
	if (st.wHour > hours || st.wHour == hours && st.wMinute > minutes ||
		st.wHour == hours && st.wMinute == minutes && st.wSecond > seconds)
	{
		return 0;
	}


	TCHAR time[100];
	wsprintf(time, TEXT("%d:%d:%d"), hours, minutes, seconds);
	SendMessage(p->hDate, LB_ADDSTRING, 0, LPARAM(time));


	st.wHour = hours;
	st.wMinute = minutes;
	st.wSecond = seconds;
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	LocalFileTimeToFileTime(&ft, &ft);
	SetWaitableTimer(hTimer, (LARGE_INTEGER*)&ft, 0, NULL, NULL, FALSE);


	SetWindowText(p->hEditDateH, TEXT(""));
	SetWindowText(p->hEditDateM, TEXT(""));
	SetWindowText(p->hEditDateS, TEXT(""));

	if (WaitForSingleObject(hTimer, INFINITE) == WAIT_OBJECT_0) {
		MessageBox(0, _T("Failed!"), 0, 0);


		SendMessage(p->hStatus, LB_DELETESTRING, index, 0);
		SendMessage(p->hStatus, LB_INSERTSTRING, index, LPARAM(_T("Time is up!")));
	}

	return 0;
}


void TaskTimer::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	TCHAR buffTask[256];

	if (id == IDC_BUTTON1)
	{

		
		GetWindowText(hEditTask, buffTask, 256);


		TCHAR bufHour[10], bufMinute[10], bufSecond[10];
		GetWindowText(hEditDateH, bufHour, 10);
		GetWindowText(hEditDateM, bufMinute, 10);
		GetWindowText(hEditDateS, bufSecond, 10);

		int hours = _tstoi(bufHour);
		int minutes = _tstoi(bufMinute);
		int seconds = _tstoi(bufSecond);
		if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59)
		{
			MessageBox(hwnd, TEXT("Введено неправильное время!"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
			return;
		}


		if (_tcslen(bufHour) == 0 || _tcslen(bufMinute) == 0 || _tcslen(bufSecond) == 0 || _tcslen(buffTask) == 0)
		{
			MessageBox(hwnd, TEXT("Поля пусты!"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
			return; 
		}


		int index = SendMessage(hTasks, LB_ADDSTRING, 0, LPARAM(buffTask));

		currentTaskIndex = index;

		SendMessage(hStatus, LB_ADDSTRING, 0, LPARAM(TEXT("in process")));

		SetWindowText(hEditTask, TEXT(""));

		HANDLE hThread = CreateThread(NULL, 0, Thread, this, 0, NULL);
		if (hThread == NULL)
		{

			MessageBox(hwnd, TEXT("Ошибка при создании потока"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		}

	}

	if (id == IDC_BUTTON2)
	{
		int index = SendMessage(hTasks, LB_GETCURSEL, 0, 0);
		if (index != LB_ERR)
		{
			int length = SendMessage(hEditTask, WM_GETTEXTLENGTH, 0, 0);
			TCHAR* pBuffer = new TCHAR[length + 1];
			GetWindowText(hEditTask, pBuffer, length + 1);
			SendMessage(hTasks, LB_DELETESTRING, index, 0);
			SendMessage(hTasks, LB_INSERTSTRING, index, LPARAM(pBuffer));

			SetWindowText(hEditDateH, TEXT(""));
			SetWindowText(hEditDateM, TEXT(""));
			SetWindowText(hEditDateS, TEXT(""));

		}
		else
		{
			MessageBox(hwnd, TEXT("Поле не выбрано!"), TEXT("Ошибка"), MB_OK | MB_ICONSTOP);
		}
	}

	else if (id == IDC_BUTTON3) {

		int index = SendMessage(hTasks, LB_GETCURSEL, 0, 0);

		SendMessage(hTasks, LB_DELETESTRING, index, 0);
		SendMessage(hDate, LB_DELETESTRING, index, 0);
		SendMessage(hStatus, LB_DELETESTRING, index, 0);
	}

	else if (id == IDC_BUTTON4)
	{
		int index = SendMessage(hTasks, LB_GETCURSEL, 0, 0);
		SendMessage(hStatus, LB_DELETESTRING, index, 0);
		SendMessage(hStatus, LB_INSERTSTRING, index, LPARAM(_T("Done!")));
	}


}

BOOL CALLBACK TaskTimer::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);

	}
	return FALSE;
}
