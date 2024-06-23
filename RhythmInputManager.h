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
	
	void ReadIniFile();

	void PlaySound(const UINT idx);

	void ChangeVolume(float val);

private:
	bool InitSound(const wstring& iniFile);
	FMOD_RESULT ReleaseSound();
	bool InitKeyBind(const wstring& iniFile);

	void WriteDefaultIniFile();
private:
	bool keyState[INPUT_MAX]{};

	map<byte, UINT> keyMap;
	map<UINT, FMOD::Sound*> soundList;
	map<UINT, FMOD::Channel*> localChannel;

	FMOD::System* targetSystem = nullptr;

	float volume = 1.0f;
};

