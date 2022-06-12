#pragma once

#include <string>
#include <Windows.h>

#include "InputSystem/InputManager.h"

static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

struct WindorDescriptor
{
	WindorDescriptor() : 
		caption(L""),
		width(640),
		height(480),
		pos_x(200),
		pos_y(20),
		resizing(true)
	{}

	int pos_x;
	int pos_y;
	std::wstring caption;	
	int width;				
	int height;				
	bool resizing;
};

class Window
{
public:
	Window();

	static Window* Get(){return this_window_;}
	
	bool Create(const WindorDescriptor &desc);
	void RunEvent();
	void Close();

	void SetInputMgr(InputManager *inputmgr);
		
	HWND GetHWND() const {return hwnd_;}
	int GetLeft() const {return descrioptor_.pos_x;}
	int GetTop() const {return descrioptor_.pos_y;}
	int GetWidth() const {return descrioptor_.width;}
	int GetHeight() const {return descrioptor_.height;}

	const std::wstring& GetCaption() const {return descrioptor_.caption;}
	
	bool IsExit() const {return is_exit_;}
	bool IsActive() const {return is_active_;}
	
	bool IsResize() 
	{
		bool ret = is_isresize_;
		is_isresize_ = false;
		return ret;
	}
	
	LRESULT WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	
private:
	static Window *this_window_;

	WindorDescriptor descrioptor_;
	InputManager *input_manager_;
	HWND hwnd_;		
	bool is_exit_;		
	bool is_active_;		
	bool is_minimized_;
	bool is_maximized_;
	bool is_isresize_;
	
private:
	void UpdateWindowState();
};

