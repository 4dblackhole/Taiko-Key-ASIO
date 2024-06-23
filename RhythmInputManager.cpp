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
    ReadIniFile();
    return InitSound();
}

FMOD_RESULT RhythmInputManager::Release()
{
    return ReleaseSound();
}

FMOD_RESULT RhythmInputManager::InitSound()
{
    FMOD_RESULT result = FMOD_OK;
    FMOD::Sound* tempSound;
    result = targetSystem->createStream("HitSounds/don.wav", FMOD_LOOP_OFF | FMOD_CREATESAMPLE, nullptr, &tempSound);
    soundList.insert(make_pair((UINT)SOUND_IDX::DON, tempSound));
    result = targetSystem->createStream("HitSounds/kat.wav", FMOD_LOOP_OFF | FMOD_CREATESAMPLE, nullptr, &tempSound);
    soundList.insert(make_pair((UINT)SOUND_IDX::KAT, tempSound));

    return result;
}

FMOD_RESULT RhythmInputManager::ReleaseSound()
{
    FMOD_RESULT result = FMOD_OK;
    for (pair<const UINT, FMOD::Sound*>&it : soundList) result = it.second->release();
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

static constexpr LPCTSTR iniFileName = _T("KeyBind.ini");
static const wstring soundSetIdc = L"[Sound Set]";
static const wstring keyBindIdc = L"[KeyBind]";

void GetSoundInfo(const wstring& desc, UINT& key, string& fileName)
{
    wstringstream wss;

    size_t colonmPos = desc.find(L':'); // key: filename
    wstring soundKey = desc.substr(0, colonmPos);
    wss << soundKey;
    wss >> key;

    size_t wavFilePos = colonmPos;
    while (desc[++wavFilePos] == L' ');
    wstring wavFileWstr = desc.substr(wavFilePos);
    fileName.assign(wavFileWstr.begin(), wavFileWstr.end());
}

void RhythmInputManager::ReadIniFile()
{
    ifstream in(iniFileName);
    string utf8Str;
    if (in.is_open()) {
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        utf8Str.resize(size);
        in.seekg(0, std::ios::beg);
        in.read(&utf8Str[0], size);

        wstring iniFile = UTF8ToWstring(utf8Str.c_str(), (int)size);

        // pos below the line of [Sound Set]
        size_t soundSetPos = iniFile.find(soundSetIdc) + soundSetIdc.size();
        while (iniFile[soundSetPos++] != L'\n');

        // pos below the line of [KeyBind]
        size_t keyBindPos = iniFile.find(keyBindIdc, soundSetPos) + keyBindIdc.size();
        while (iniFile[keyBindPos++] != L'\n');

        size_t startPos = soundSetPos;
        while (startPos <= keyBindPos)
        {
            size_t endPos = iniFile.find(L'\n', startPos); //location of \n
            wstring currentSoundDesc = iniFile.substr(startPos, endPos - startPos); //read on line

            UINT key;
            string wavFile;
            GetSoundInfo(currentSoundDesc, key, wavFile);
        }

    }
    else {
        std::cout << "파일을 찾을 수 없습니다!" << std::endl;
    }

}
