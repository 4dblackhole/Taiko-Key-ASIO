#include "framework.h"

#include "RhythmInputManager.h"

RhythmInputManager::RhythmInputManager()
{
}

RhythmInputManager::~RhythmInputManager()
{
}

void RhythmInputManager::OnKeyUp(UINT key)
{
    switch (key)
    {
    case keymap[0]: keybind[0] = false; break;
    case keymap[1]: keybind[1] = false; break;
    case keymap[2]: keybind[2] = false; break;
    case keymap[3]: keybind[3] = false; break;
    default: break;
    }
}

void RhythmInputManager::OnKeyDown(UINT key)
{
    switch (key)
    {
    case keymap[0]:
        if (!keybind[0])
        {
            keybind[0] = true;
            action[0]();
        }
        break;
    case keymap[1]:
        if (!keybind[1])
        {
            keybind[1] = true;
            action[1]();
        }
        break;

    case keymap[2]:
        if (!keybind[2])
        {
            keybind[2] = true;
            action[2]();
        }
        break;

    case keymap[3]:
        if (!keybind[3])
        {
            keybind[3] = true;
            action[3]();
        }
        break;
    default:
        break;
    }
}

void RhythmInputManager::RegisterAction(UINT id, void(*pf)(void))
{
    if (id >= keyAmount) return;
    action[id] = pf;
}
