#include "Window.h"
#include "Log.h"

Window *Window::this_window_ = nullptr;

Window::Window(void) :
		input_manager_(nullptr),
		hwnd_(0),
		is_exit_(false),
		is_active_(true),
		is_minimized_(false),
		is_maximized_(false),
		is_isresize_(false)
{
	if (!this_window_)
		this_window_ = this;
	else
		Log::LogError("Window already was created!");
}

bool Window::Create(const WindowDescriptor &desc)
{
	Log::LogDebug("Creating window...");
	descrioptor_ = desc;

	WNDCLASSEXW wnd;

	wnd.cbSize = sizeof(WNDCLASSEXW);
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = StaticWndProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = 0;
	wnd.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wnd.hIconSm = wnd.hIcon;
	wnd.hCursor = LoadCursor(0, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = L"D3D11F";
	wnd.cbSize = sizeof(WNDCLASSEX);

	if( !RegisterClassEx( &wnd ) )
	{
		Log::LogError("Can not register window!");
		return false;
	}

	RECT rect = {0, 0, descrioptor_.width, descrioptor_.height};
	SetCursorPos(rect.right + rect.left / 2, rect.top + rect.bottom / 2);
	
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

	long lwidth = rect.right - rect.left;
	long lheight = rect.bottom - rect.top;

	long lleft = (long)descrioptor_.pos_x;	
	long ltop = (long)descrioptor_.pos_y;

	
	
	hwnd_ = CreateWindowEx(NULL, L"D3D11F", descrioptor_.caption.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,  lleft, ltop, lwidth, lheight, NULL, NULL, NULL, NULL);

	if( !hwnd_ )
	{
		Log::LogError("Can not create window :(");
		return false;
	}

	ShowWindow(hwnd_, SW_SHOW);
	UpdateWindow(hwnd_);

	return true;
}

void Window::RunEvent()
{
	MSG msg;

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::Close()
{
	if (hwnd_)
		DestroyWindow(hwnd_);
	hwnd_ = nullptr;

	Log::LogDebug("Window was closed.");
}

LRESULT Window::WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch(nMsg)
	{
	case WM_CREATE:
		return 0;
	case WM_CLOSE:
		is_exit_ = true;
		return 0;
	case WM_ACTIVATE:
		if (LOWORD(wParam) != WA_INACTIVE)
			is_active_ = true;
		else
			is_active_ = false;
		return 0;
	case WM_MOVE:
		descrioptor_.pos_x = LOWORD(lParam);
		descrioptor_.pos_y = HIWORD(lParam);
		UpdateWindowState();
		return 0;
	case WM_SIZE:
		if (!descrioptor_.resizing)
			return 0;
		descrioptor_.width = LOWORD(lParam);
		descrioptor_.height = HIWORD(lParam);
		is_isresize_ = true;
		if( wParam == SIZE_MINIMIZED )
		{
			is_active_ = false;
			is_minimized_ = true;
			is_maximized_ = false;
		}
		else if( wParam == SIZE_MAXIMIZED )
		{
			is_active_ = true;
			is_minimized_ = false;
			is_maximized_ = true;
		}
		else if( wParam == SIZE_RESTORED )
		{
			if( is_minimized_ )
			{
				is_active_ = true;
				is_minimized_ = false;
			}
			else if( is_maximized_ )
			{
				is_active_ = true;
				is_maximized_ = false;
			}
		}
		UpdateWindowState();
		return 0;
	case WM_MOUSEMOVE: case WM_LBUTTONUP: case WM_LBUTTONDOWN: case WM_MBUTTONUP: case WM_MBUTTONDOWN: case WM_RBUTTONUP: case WM_RBUTTONDOWN: case WM_MOUSEWHEEL: case WM_KEYDOWN: case WM_KEYUP:
		if (input_manager_)
			input_manager_->Run(nMsg ,wParam, lParam);
		return 0;
	}

	return DefWindowProcW( hwnd, nMsg, wParam, lParam);
}

void Window::SetInputMgr(InputManager *inputmgr) 
{
	input_manager_ = inputmgr;
	UpdateWindowState();	
}

void Window::UpdateWindowState()
{
	RECT ClientRect;
	ClientRect.left = descrioptor_.pos_x;
	ClientRect.top = descrioptor_.pos_y;
	ClientRect.right = descrioptor_.width;
	ClientRect.bottom = descrioptor_.height;
	if (input_manager_)
		input_manager_->SetWinRect(ClientRect);
}

LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return Window::Get()->WndProc( hwnd, nMsg, wParam, lParam );
}