#pragma once

#include "header.h"

class TaskTimer
{
public:
	TaskTimer(void);
	~TaskTimer(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static TaskTimer* ptr;
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	HWND hAdd, hDateH, hDateM, hDateS;
	HWND hStatus, hDate, hTasks;
	HWND hCreate, hEdit, hDelete, hFinished;
	int currentTaskIndex = -1;
	HWND hDialog;
	PNOTIFYICONDATA pNID;
};
