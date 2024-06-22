#include "framework.h"

#include "RhythmInputManager.h"

void RhythmInputManager::PlaySound(const SOUND_IDX idx)
{
    static FMOD::Channel* localChannel[(int)SOUND_IDX::MAX];
    targetSystem->playSound(soundList[(int)idx], 0, false, &localChannel[(int)idx]);
}

RhythmInputManager::RhythmInputManager()
{
    InitKeyBind();
}

RhythmInputManager::~RhythmInputManager()
{
}

void RhythmInputManager::OnKeyUp(UINT key)
{
    keyState[key] = false;
}

void RhythmInputManager::OnKeyDown(UINT key)
{
    if (!keyState[key]) PlaySound(keyMap[key]);
    keyState[key] = true;
}

FMOD_RESULT RhythmInputManager::Init(FMOD::System* ptr)
{
    targetSystem = ptr;
    return InitSound();
}

FMOD_RESULT RhythmInputManager::Release()
{
    return ReleaseSound();
}

FMOD_RESULT RhythmInputManager::InitSound()
{
    FMOD_RESULT result = FMOD_OK;
    result = targetSystem->createStream("HitSounds/don.wav", FMOD_LOOP_OFF | FMOD_CREATESAMPLE, nullptr, &soundList[(int)SOUND_IDX::DON]);
    result = targetSystem->createStream("HitSounds/kat.wav", FMOD_LOOP_OFF | FMOD_CREATESAMPLE, nullptr, &soundList[(int)SOUND_IDX::KAT]);

    return result;
}

FMOD_RESULT RhythmInputManager::ReleaseSound()
{
    FMOD_RESULT result = FMOD_OK;
    for (FMOD::Sound*& it : soundList) result = it->release();
    return result;
}

void RhythmInputManager::InitKeyBind()
{
    //TODO: read key bind information from ini file
    keyMap.insert(make_pair('Z', SOUND_IDX::KAT));
    keyMap.insert(make_pair('X', SOUND_IDX::DON));
    keyMap.insert(make_pair(VK_OEM_PERIOD, SOUND_IDX::DON));
    keyMap.insert(make_pair(VK_OEM_2, SOUND_IDX::KAT));
}
