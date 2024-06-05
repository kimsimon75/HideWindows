#pragma once
#include <windows.h>
#include<stack>
#include<map>
#include <psapi.h>
#include "º¼·ý Á¶Àý.h"

using namespace std;

wstring PN;
stack<HWND> st = {};
map<wstring, DWORD> m;

bool func()
{
	return (PN == L"Explorer.EXE") ? 0 : 1;
}

std::wstring GetProcessNameByPID(DWORD processID) {

	char processName[MAX_PATH] = "<unknown>";

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

	if (hProcess != NULL) {

		HMODULE hMod = NULL;

		DWORD cbNeeded = -1;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {

			GetModuleBaseNameA(hProcess, hMod, processName, sizeof(processName) / sizeof(char));

		}

	}

	CloseHandle(hProcess);

	std::string temp(processName);

	return std::wstring(temp.begin(), temp.end());

}

void HideWindow()
{
	HWND hWnd;
	POINT point;

	GetCursorPos(&point);
	hWnd = WindowFromPoint(point);

	DWORD processId;
	GetWindowThreadProcessId(hWnd, &processId);
	PN = GetProcessNameByPID(processId);

	HWND parent = GetParent(hWnd), parent2;
	while (1) {
		parent2 = GetParent(parent);
		if (parent2 == 0)
			break;
		parent = parent2;
	}

	if (parent != 0)
		hWnd = parent;
	
	if (func())
	{
		st.push(hWnd);
		ShowWindow(hWnd, SW_HIDE);
		SetApplicationVolume(m[PN], 0.0f);
	}
}
void ShowWindow()
{
	if (!st.empty())
	{
		DWORD processId;
		GetWindowThreadProcessId(st.top(), &processId);
		PN = GetProcessNameByPID(processId);
		SetApplicationVolume(m[PN], 1.0f);
		
		ShowWindow(st.top(), SW_SHOW);
		st.pop();
	}
}

void ShowWindowAll()
{
	while (!st.empty())
	{
		ShowWindow();
	}
}

