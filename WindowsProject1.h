#pragma once

#include "resource.h"

enum class HwndID
{
	COMBO_DRIVER = 1,

	STATIC_COMBO_DRIVER,

	BUTTON_RELOAD_INI,
};

constexpr int DriverComboY = 10;
constexpr int ReloadButtonY = 165;