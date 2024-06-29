#pragma once

#include "resource.h"

enum class HwndID
{
	COMBO_DRIVER = 1,
	STATIC_COMBO_DRIVER,
	STATIC_VOLUME,
	BUTTON_RELOAD_INI,
	TRACKBAR_VOLUME,
};

constexpr int DriverComboY = 10;
constexpr int ReloadButtonY = 165;