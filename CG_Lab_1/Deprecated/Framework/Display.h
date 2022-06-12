#pragma once

#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>

class Display
{
public:
	LPCWSTR window_name_;
	int window_width_;
	int window_height_;
	HINSTANCE hInstance_;
	HWND hWnd_;
public:
	Display(LPCWSTR window_name, int window_width, int window_height);
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};

