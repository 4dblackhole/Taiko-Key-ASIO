#include "framework.h"

#include "RhythmInputManager.h"

void RhythmInputManager::PlaySound(const UINT idx)
{
    if (soundList.find(idx) != soundList.cend())
    {
        targetSystem->playSound(soundList[idx], 0, false, &localChannel[idx]);
        localChannel[idx]->setVolume(volume);
    }
}

void RhythmInputManager::ChangeVolume(float val)
{
    volume = val;
    
}

RhythmInputManager::RhythmInputManager()
{
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
    if (!ReadIniFile())
    {
        if (WriteDefaultIniFile())
        {
            ReadIniFile();
            MessageBox(NULL, _T("Default ini file has created"), _T("alert"), MB_OK);
        }
        else
        {
            MessageBox(NULL, _T("Default ini file creation failed"), _T("alert"), MB_OK);
            return FMOD_RESULT_FORCEINT;
        }
    }
    return FMOD_OK;
}

FMOD_RESULT RhythmInputManager::Release()
{
    return ReleaseSound();
}

FMOD_RESULT RhythmInputManager::ReleaseSound()
{
    FMOD_RESULT result = FMOD_OK;
    for (pair<const UINT, FMOD::Sound*>&it : soundList) result = it.second->release();
    return result;
}

static constexpr LPCTSTR iniFileName = _T("KeyBind.ini");
static const wstring soundSetIdc = L"[Sound Set]";
static const wstring keyBindIdc = L"[KeyBind]";

static bool ColumnSeparate(const wstring& source, wstring& first, wstring& second)
{
    size_t columnPos = source.find(L':'); // key: filename
    if (columnPos == wstring::npos) return false; //not valid desc
    
    first = source.substr(0, columnPos);

    size_t secondStartPos = columnPos;
    while (source[++secondStartPos] == L' ');
    second = source.substr(secondStartPos);
}

static bool GetSoundInfo(const wstring& desc, UINT& key, string& fileName)
{
    wstringstream wss;

    size_t colonmPos = desc.find(L':'); // key: filename
    if (colonmPos == wstring::npos) return false; //not valid desc

    wstring soundKey = desc.substr(0, colonmPos);
    wss << soundKey;
    wss >> key;

    size_t wavFilePos = colonmPos;
    while (desc[++wavFilePos] == L' ');
    wstring wavFileWstr = desc.substr(wavFilePos);
    fileName.assign(wavFileWstr.begin(), wavFileWstr.end());

    return true;
}

static bool GetKeyBindInfo(const wstring& desc, byte& key, UINT& soundKey)
{
    wstringstream wss;

    size_t colonmPos = desc.find(L':'); // key: filename
    if (colonmPos == wstring::npos) return false; //not valid desc

    wstring keyStr = desc.substr(0, colonmPos);
    wss << keyStr;
    wss >> keyStr;
    tstring keyTstr;
    keyTstr.assign(keyStr.begin(), keyStr.end());
    auto dd = STR_TO_VK_CONVERT(keyTstr);
    key = dd;
    wss.clear();

    size_t soundKeyPos = colonmPos;
    while (desc[++soundKeyPos] == L' ');
    wstring soundKeyStr = desc.substr(soundKeyPos);
    wss << soundKeyStr;
    wss >> soundKey;

    return true;
}

bool RhythmInputManager::InitSound(const wstring& iniFile)
{
    // pos below the line of [Sound Set]
    if (iniFile.find(soundSetIdc) == wstring::npos) return false;
    size_t soundSetPos = iniFile.find(soundSetIdc) + soundSetIdc.size();
    while (iniFile[soundSetPos++] != L'\n');

    // pos below the line of [KeyBind]
    if (iniFile.find(keyBindIdc) == wstring::npos) return false;
    size_t keyBindPos = iniFile.find(keyBindIdc, soundSetPos) + keyBindIdc.size();
    while (iniFile[keyBindPos++] != L'\n');

    ReleaseSound();
    soundList.clear();

    size_t startPos = soundSetPos;
    while (startPos < keyBindPos)
    {
        size_t endPos = iniFile.find(L'\n', startPos); //location of \n
        wstring currentSoundDesc = iniFile.substr(startPos, endPos - startPos); //read on line

        UINT key;
        string wavFile;
        if (GetSoundInfo(currentSoundDesc, key, wavFile) == true)
        {
            FMOD::Sound* tempSound;
            FMOD_RESULT result = targetSystem->createStream(wavFile.c_str(), FMOD_LOOP_OFF | FMOD_CREATESAMPLE, nullptr, &tempSound);
            if (result == FMOD_OK) soundList.insert(make_pair(key, tempSound));
            else MessageBox(NULL, _T("Wrong sound file."), _T("Error"), MB_OK);
        }
        startPos = endPos + 1;
    }
    return true;
}

bool RhythmInputManager::InitKeyBind(const wstring& iniFile)
{
    // pos below the line of [KeyBind]
    if (iniFile.find(keyBindIdc) == wstring::npos) return false;
    size_t keyBindPos = iniFile.find(keyBindIdc) + keyBindIdc.size();
    while (iniFile[keyBindPos++] != L'\n');

    for (byte& it : keyMap) it = 0;

    size_t endOfFilePos = iniFile.length();
    size_t startPos = keyBindPos;
    while (startPos < endOfFilePos)
    {
        size_t endPos = iniFile.find(L'\n', startPos); //location of \n
        wstring currentSoundDesc = iniFile.substr(startPos, endPos - startPos); //read on line

        byte vkey;
        UINT soundKey;
        if (GetKeyBindInfo(currentSoundDesc, vkey, soundKey) == true)
        {
            keyMap[vkey] = soundKey;
        }

        if (endPos == wstring::npos)startPos = endPos;
        else startPos = endPos + 1;
    }
    return true;
}

bool RhythmInputManager::ReadIniFile()
{
    bool result = true;
    ifstream in(iniFileName);
    string utf8Str;
    if (in.is_open())
    {
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        utf8Str.resize(size);
        in.seekg(0, std::ios::beg);
        in.read(&utf8Str[0], size);

        wstring iniFile = UTF8ToWstring(utf8Str);

        if (InitSound(iniFile) == false)
        {
            result = false;
            MessageBox(NULL, _T("No sound descrptions"), _T("ini error"), MB_OK);
        }
        if (InitKeyBind(iniFile) == false)
        {
            result = false;
            MessageBox(NULL, _T("No key bind descrptions"), _T("ini error"), MB_OK);
        }
        in.close();
        return result;
    }
    else return false;
    
}

bool RhythmInputManager::WriteDefaultIniFile()
{
    ofstream out(iniFileName);
    if (out.is_open())
    {
        out << WstringToUTF8(L"// https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes\n");
        out << WstringToUTF8(soundSetIdc) << endl;
        out << WstringToUTF8(L"1: HitSounds/don.wav\n");
        out << WstringToUTF8(L"2: HitSounds/kat.wav\n\n");
        out << WstringToUTF8(keyBindIdc) << endl;
        out << WstringToUTF8(L"Z: 2\n");
        out << WstringToUTF8(L"X: 1\n");
        out << WstringToUTF8(L"VK_OEM_PERIOD: 1\n");
        out << WstringToUTF8(L"VK_OEM_2: 2\n");

        out.close();
        return true;
    }
    return false;
}
