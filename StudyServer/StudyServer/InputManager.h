#pragma once

#include <windows.h>
#include "InputEnum.h"

class InputManager
{
	struct KeyInfomation
	{
		KeyState state = KeyState::None;
		bool prevPush = false;
	};

public:
	InputManager();
	~InputManager();

	void Initalize(HWND main);
	void Update(float dt);

	void Finalize();

	// 키보드 키의 현재상태를 반환
	KeyState GetKeyState(Key key) const;
	bool IsKeyState(const Key& _key, const KeyState& _state) const;

	// 이번 프레임 마우스 위치
	POINT GetMousePosition() const
	{
		return currentMousePosition;
	}
	// 이번 프레임 마우스 변화량
	POINT GetDeltaMousePosition() const
	{
		return deltaMousePosition;
	}

	bool IsActive() const
	{
		return _isActive;
	}
	void SetActive(bool isActive)
	{
		_isActive = isActive;
	}

	RECT GetScreenRect() const;

private:
	HWND _hwnd; // 메인 윈도우 핸들
	KeyInfomation _keyInfomations[static_cast<size_t>(Key::Last)]; // 키보드정보를 담은 배열

	const int _matchVK[static_cast<size_t>(Key::Last)]; // KEY 값을 가상키값과 매칭시켜주는 배열

	POINT currentMousePosition; // 현재 프레임 마우스 위치
	POINT prevMousePosition;    // 이전 프레임 마우스 위치
	POINT deltaMousePosition;   // 이번프레임 마우스 변화량

	bool _isActive; // 현재 키입력을 받는지
};
