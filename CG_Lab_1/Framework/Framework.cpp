#include "Framework.h"
#include <Windows.h>

Framework::Framework() :
		window_(nullptr),
		render_(nullptr),
		input_(nullptr),
		init_(false)
{
}

Framework::~Framework()
{
}

void Framework::AddInputListener(InputListener *listener)
{
	if (input_)
		input_->AddListener(listener);
}

void Framework::Run()
{
	if (init_)
		while(frame_());
}

bool Framework::Init()
{
	window_ = new Window();
	input_ = new InputManager();

	input_->Init();
	
	DescWindow desc;			
	if ( !window_->Create(desc) )
	{
		Log::LogError("Failed to create a window!");
		return false;
	}
	window_->SetInputMgr(input_);

	if ( !render_->Init(window_->GetHWND()) )
	{
		Log::LogError("Failed to create a render!");
		return false;
	}

	// Add default listeners
	Keyboard = new KeyboardController();
	AddInputListener(Keyboard);
	init_ = true;
	return true;
}

bool Framework::frame_()
{

	window_->RunEvent();
	if (!window_->IsActive())
		return true;
	
	if (window_->IsExit())
		return false;
	
	if (window_->IsResize())
	{
	}

	if (!render_->Draw())
		return false;

	return true;
}

void Framework::Close()
{
	init_ = false;
	render_->Close();
	window_->Close();
	input_->Close();
}