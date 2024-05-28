#pragma once
#include "framework.h"

class RhythmInputManager
{
public:
	RhythmInputManager();
	~RhythmInputManager();

	void OnKeyUp(UINT key);
	void OnKeyDown(UINT key);

	void RegisterAction(UINT id, void (*pf)(void));

private:
	static constexpr UINT keyAmount = 4;
	static constexpr unsigned char keymap[keyAmount] = { 'Z', 'X', VK_OEM_PERIOD, VK_OEM_2 };

	bool keybind[keyAmount]{};
	void (*action[keyAmount])(void) = {};

};

