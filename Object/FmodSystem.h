#pragma once
#include "framework.h"

class FmodSystem
{
public:
	FmodSystem();
	~FmodSystem();

	FMOD_RESULT Init();
	FMOD_RESULT Release();

	FMOD_RESULT ChangeDrive(int idx);

	inline int GetDriveIdx() const { return currentDriveIdx; }
	inline void SetDriveIdx(int v) { currentDriveIdx = v; }

	inline int GetDriveCount() const { return driveCount; }

	FMOD::System* System() { return _system; }

	void EnumDriverListToComboBox(HWND comboWnd);

private:
	FMOD::System* _system = nullptr;
	int driveCount = 0, currentDriveIdx = 0;

	unsigned int version = 0;
	void* extradriverdata = nullptr;
};