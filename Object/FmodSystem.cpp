#include "FmodSystem.h"

FmodSystem::FmodSystem()
{
}

FmodSystem::~FmodSystem()
{
}

FMOD_RESULT FmodSystem::Init()
{
    FMOD_RESULT result = FMOD_OK;
    //FMOD Init
    result = FMOD::System_Create(&_system);
    if (result != FMOD_OK) return result;

    result = _system->getVersion(&version);
    if (result != FMOD_OK) return result;

    result = _system->setOutput(FMOD_OUTPUTTYPE_ASIO);
    if (result != FMOD_OK) return result;

    _system->getNumDrivers(&driveCount);
    if (driveCount == 0)
    {
        if (MessageBox(NULL, _T("No ASIO driver detected."), _T("Buy your own Audio Interface."), MB_YESNO) == IDNO)
        {
            MessageBox(NULL, _T("Buy your own Audio Interface."), _T("Buy your own Audio Interface."), MB_OK);
        }
        return result;
    }

    result = _system->setDSPBufferSize(64, 4);

    result = _system->init(256, FMOD_INIT_NORMAL, extradriverdata);
    if (result != FMOD_OK) return result;

    result = _system->getDriver(&currentDriveIdx);

    return result;
}

FMOD_RESULT FmodSystem::Release()
{
    FMOD_RESULT result = FMOD_OK;
    result = _system->close();
    result = _system->release();

    return result;
}

FMOD_RESULT FmodSystem::ChangeDrive(int idx)
{
    FMOD_RESULT result = FMOD_OK;
    result = _system->setDriver(idx);
    
    return result;
}

void FmodSystem::EnumDriverListToComboBox(HWND comboWnd)
{
    char tempDriveName[256];
    for (int i = 0; i < driveCount; ++i)
    {
        _system->getDriverInfo(i, (char*)tempDriveName, sizeof(tempDriveName), 0, 0, 0, 0);

        WCHAR uni[256]{};
        int nLen = MultiByteToWideChar(CP_UTF8, 0, tempDriveName, (int)strlen(tempDriveName), NULL, NULL);
        MultiByteToWideChar(CP_UTF8, 0, tempDriveName, (int)strlen(tempDriveName), uni, nLen);
        SendMessage(comboWnd, CB_ADDSTRING, 0, (LPARAM)uni);
    }
}
