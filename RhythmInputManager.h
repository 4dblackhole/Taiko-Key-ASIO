#pragma once
#include "framework.h"

constexpr size_t INPUT_MAX = 256;

enum class SOUND_IDX
{
	DON=0,
	KAT,
	MAX
};

class RhythmInputManager
{
public:
	RhythmInputManager();
	~RhythmInputManager();

	void OnKeyUp(UINT key);
	void OnKeyDown(UINT key);

	FMOD_RESULT Init(FMOD::System* ptr);
	FMOD_RESULT Release();
	

	void PlaySound(const UINT idx);

private:
	void InitSound(const wstring& iniFile);
	FMOD_RESULT ReleaseSound();
	void InitKeyBind(const wstring& iniFile);

	void ReadIniFile();

private:
	bool keyState[INPUT_MAX]{};

	map<byte, UINT> keyMap;
	map<UINT, FMOD::Sound*> soundList;

	FMOD::System* targetSystem = nullptr;
};

