#pragma once
#include "stdafx.h"
#include "InputManager.h"
#include "Engine.h"


InputManager::InputManager(void)
{
	m_MouseX = 0;
	m_MouseY = 0;

	m_ScreenHeight = Engine::Instance()->GetHeight();
	m_ScreenWidth = Engine::Instance()->GetWidth();

	m_pDirectInput = NULL;
	DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	
	// KEYBOARD
	m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyboard->SetCooperativeLevel(0, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	
	// MOUSE
	m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(0, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}


InputManager::~InputManager(void)
{
	delete m_pDirectInput;
	delete m_pKeyboard;
	delete m_pMouse;
}

bool InputManager::IsKeyDown(int p_Key)
{
	if (m_KeyboardState[p_Key]/128)
		return true;

	return false;
}

bool InputManager::IsMouseButtonDown(int p_Button)
{
	if(m_MouseState.rgbButtons[p_Button]/128)
		return true;

	return false;
}

void InputManager::GetMouseLocation(int& p_MouseX, int& p_MouseY)
{
	p_MouseX = m_MouseX;
	p_MouseY = m_MouseY;
}

void InputManager::GetMouseMove(int& p_dMouseX, int& p_dMouseY)
{
	p_dMouseX = m_dMouseX;
	p_dMouseY = m_dMouseY;
}

void InputManager::Update()
{
	HRESULT hr = 0;
	hr = m_pKeyboard->Acquire();
	if (FAILED(hr))
		MessageBox(NULL, L"Keyboard acquisition error.", L"Error", MB_OK);

	hr = m_pKeyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);

	if (FAILED(hr))
		MessageBox(NULL, L"Keyboard state error.", L"Error", MB_OK);

	hr = m_pMouse->Acquire();

	if (FAILED(hr))
		MessageBox(NULL, L"Mouse acquisition error.", L"Error", MB_OK);

	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_MouseState);

	if (FAILED(hr))
		MessageBox(NULL, L"Mouse state error.", L"Error", MB_OK);

	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_dMouseX = m_MouseState.lX;
	m_dMouseY = m_MouseState.lY;

	m_MouseX += m_dMouseX;
	m_MouseY += m_dMouseY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if(m_MouseX < 0)
	{
		m_MouseX = 0;
	}
	
	if(m_MouseY < 0)
	{
		m_MouseY = 0;
	}
	
	if(m_MouseX > m_ScreenWidth)  { m_MouseX = m_ScreenWidth; }
	if(m_MouseY > m_ScreenHeight) { m_MouseY = m_ScreenHeight; }
	
}

InputManager* InputManager::Instance()
{
	if (m_Instance == NULL)
	{
		m_Instance = new InputManager();
	}

	return m_Instance;
}

InputManager* InputManager::m_Instance = NULL;
