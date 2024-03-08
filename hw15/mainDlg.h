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
	HWND hBAddTask, hBEditTask, hBDeleteTask, hBFinishTask, hEditTask, hEditDateH, hEditDateM, hEditDateS, hDialog;
	HWND hStatus, hDate, hTasks;
	int currentTaskIndex = -1;
	PNOTIFYICONDATA pNID;
};
