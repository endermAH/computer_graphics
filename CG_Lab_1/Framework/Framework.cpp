#include "Framework.h"
#include <Windows.h>

Framework::Framework() :
		m_wnd(nullptr),
		m_render(nullptr),
		m_input(nullptr),
		m_init(false)
{
}

Framework::~Framework()
{
}

void Framework::AddInputListener(InputListener *listener)
{
	if (m_input)
		m_input->AddListener(listener);
}

void Framework::Run()
{
	if (m_init)
		while(m_frame());
}

bool Framework::Init()
{
	m_wnd = new Window();
	m_input = new InputManager();

	m_input->Init();
	
	DescWindow desc;			
	if ( !m_wnd->Create(desc) )
	{
		Log::LogError("Failed to create a window!");
		return false;
	}
	m_wnd->SetInputMgr(m_input);

	if ( !m_render->Init(m_wnd->GetHWND()) )
	{
		Log::LogError("Failed to create a render!");
		return false;
	}

	// Add default listeners
	Keyboard = new KeyboardController();
	AddInputListener(Keyboard);
	m_init = true;
	return true;
}

bool Framework::m_frame()
{

	m_wnd->RunEvent();
	if (!m_wnd->IsActive())
		return true;
	
	if (m_wnd->IsExit())
		return false;
	
	if (m_wnd->IsResize())
	{
	}

	if (!m_render->Draw())
		return false;

	return true;
}

void Framework::Close()
{
	m_init = false;
	m_render->Close();
	m_wnd->Close();
	m_input->Close();
}