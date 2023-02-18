////////////////////////////////////////////////
//  2023 Prùcha Filip <prucha.fil@gmail.com>  //
////////////////////////////////////////////////

#pragma once
#include "pch.h"
#include "config.h"
#include <unordered_map>
#include <memory>
#include "game.h"

class tab : public wxFrame {
	game game_;
	wxButton*** buttons_;
	wxTimer timer_;
	wxStaticText* time_;
	uint64_t seconds_;

public:
	tab();
	~tab();
private:
	void uncover_mindfield(wxEvent&);
	void select_mindfield(wxEvent&);
	void dig_around(int32_t, int32_t);
	void tick(wxTimerEvent&);
};