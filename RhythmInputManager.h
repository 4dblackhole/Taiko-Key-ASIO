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
	

	void PlaySound(const SOUND_IDX idx);

private:
	FMOD_RESULT InitSound();
	FMOD_RESULT ReleaseSound();

	void InitKeyBind();

	void ReadIniFile();

private:
	bool keyState[INPUT_MAX]{};

	map<byte, SOUND_IDX> keyMap;
	map<UINT, FMOD::Sound*> soundList;

	UINT keyAmount = 4;
	FMOD::System* targetSystem = nullptr;
};

