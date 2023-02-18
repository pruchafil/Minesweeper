#pragma once
#include <cstdint>
inline const uint32_t g_field_size_x = 20;
inline const uint32_t g_field_size_y = 20;
inline const uint32_t g_bombs_count = 50;
inline const wxColour g_button_color = wxColour(240, 240, 240);
inline const std::unordered_map<int, wxColour> g_colors_map = {
	{0, *wxBLACK },
	{1, *wxBLUE },
	{2, *wxGREEN },
	{3, *wxRED },
	{4, wxColour(0, 0, 102) },
	{5, wxColour(153, 0, 0) },
	{6, wxColour(0, 153, 153) },
	{7, *wxBLACK },
	{8, wxColour(102, 102, 153)  },
	{9, *wxRED },
};