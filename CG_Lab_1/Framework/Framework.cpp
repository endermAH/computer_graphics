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

bool Framework::Init(const FrameworkDescriptor &desc)
{
	render_ = desc.render;
	window_ = new Window();
	input_ = new InputManager();

	input_->Init();
	
	if ( !window_->Create(desc.window_descriptor) )
	{
		Log::LogError("Failed to create a window!");
		return false;
	}
	window_->SetInputMgr(input_);
	
	if (!render_->CreateDevice(window_->GetHWND()))
	{
		Log::LogError("Failed to create a render!");
		return false;
	}

	// Add default listeners
	init_ = true;
	return true;
}

/*bool Framework::frame_()
{

	window_->RunEvent();
	if (!window_->IsActive())
		return true;
	
	if (window_->IsExit())
		return false;
	
	if (window_->IsResize())
	{
	}
	//
	render_->BeginFrame();
	if (!render_->Draw())
		return false;
	render_->EndFrame();
	
	return true;
}*/

void Framework::Close()
{
	init_ = false;
	render_->Shutdown();
	delete render_;
	window_->Close();
	input_->Close();
}