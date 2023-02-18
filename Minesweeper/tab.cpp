////////////////////////////////////////////////
//  2023 Prùcha Filip <prucha.fil@gmail.com>  //
////////////////////////////////////////////////

#include "pch.h"
#include "tab.h"
#include <algorithm>
#include <functional>
#include <random>
#include <queue>
#include <sstream>

tab::tab() :
    wxFrame(nullptr, wxID_ANY, "minesweeper"),
    game_(game()),
    buttons_(new wxButton** [g_field_size_y]),
    timer_(wxTimer(this, wxID_ANY)),
    time_(new wxStaticText(this, wxID_ANY, "0")),
    seconds_(0ull)
{
    wxBoxSizer* b_sizer = new wxBoxSizer(wxVERTICAL);
    wxGridSizer* gr_sizer = new wxGridSizer(g_field_size_y, g_field_size_x, -1, -1);

    this->Bind(wxEVT_TIMER , &tab::tick, this);
    timer_.Start(1000, wxTIMER_CONTINUOUS);

    wxFont f;
    f.SetFamily(wxFontFamily::wxFONTFAMILY_MODERN);
    f.SetPointSize(16);
    f.MakeBold();

    std::ostringstream oss;
    oss << g_field_size_x << " x " << g_field_size_y << '\n' << g_bombs_count << " bombs";

    wxStaticText* text = new wxStaticText(this, wxID_ANY, oss.str());
    time_->SetFont(f);
    text->SetFont(f);

    for (size_t i = 0ull; i < g_field_size_y; ++i) {
        buttons_[i] = new wxButton * [g_field_size_x];
        for (size_t j = 0ull; j < g_field_size_x; ++j) {
            wxButton* b = new wxButton(this, (g_field_size_y * i) + j, wxEmptyString, wxDefaultPosition, wxSize(20, 20));
            b->SetForegroundColour(g_colors_map.at(game_[i][j]));
            b->Bind(wxEVT_LEFT_DOWN, &tab::uncover_mindfield, this);
            b->Bind(wxEVT_RIGHT_DOWN, &tab::select_mindfield, this);
            b->SetFont(f);
            gr_sizer->Add(b, 1, wxALL | wxEXPAND);
            buttons_[i][j] = b;
        }
    }

    b_sizer->Add(time_, 0, wxALIGN_CENTER);
    b_sizer->Add(text, 0, wxALIGN_CENTER);
    b_sizer->Add(gr_sizer, 1, wxSHAPED | wxALIGN_CENTER);

    this->SetSizerAndFit(b_sizer);
}

tab::~tab() {
    for (size_t i = 0ull; i < g_field_size_y; ++i) {
        delete[] buttons_[i];
    }
    delete[] buttons_;
}

void tab::uncover_mindfield(wxEvent& _Evt) {
    int32_t id = _Evt.GetId();
    uint32_t y = id / g_field_size_y, x = id % g_field_size_y;
    uint16_t num = game_.uncover_minefield(x, y);

    if (game_.game_ended()) { // player lost
        buttons_[y][x]->SetLabel('*');
        int i = wxMessageBox("BOOM!", "You lose!", wxICON_WARNING | wxOK);
        game_.reset();

        for (size_t i = 0ull; i < g_field_size_y; ++i){
            for (size_t j = 0ull; j < g_field_size_x; ++j) {
                buttons_[i][j]->SetLabel("");
                buttons_[i][j]->SetBackgroundColour(g_button_color);
                buttons_[i][j]->SetForegroundColour(g_colors_map.at(game_[i][j]));
            }
        }

        seconds_ = 0ull;
        timer_.Start(1000, wxTIMER_CONTINUOUS);
        return;
    }
    if (num == 0) { // we will dig around until we find 1-8
        dig_around(x,y);
        return;
    }
    buttons_[y][x]->SetLabel(std::to_string(num));
}

void tab::select_mindfield(wxEvent& _Evt) {
    _Evt.Skip();
    int32_t id = _Evt.GetId();
    uint32_t y = id / g_field_size_y, x = id % g_field_size_y;

    if (buttons_[y][x]->GetLabel() != "") // can not select exposed box
        return;

    wxColour c = buttons_[y][x]->GetBackgroundColour();
    bool won{};
    if (c == g_button_color) {
        buttons_[y][x]->SetBackgroundColour(*wxRED);
        won = game_.select_bomb(x, y);
    } else {
        buttons_[y][x]->SetBackgroundColour(g_button_color);
        won = game_.undo_select_bomb(x, y);
    }

    if (won)
        wxMessageBox("You won!", "Cogratulations!", wxICON_INFORMATION | wxOK);
}

void tab::dig_around(int32_t _X, int32_t _Y) {
    if(
        _X < 0 || _X >= g_field_size_x ||
        _Y < 0 || _Y >= g_field_size_y ||
        buttons_[_Y][_X]->GetLabel() != "" // is not exposed
    )
        return;

    std::string str = std::to_string(game_[_Y][_X]);
    buttons_[_Y][_X]->SetLabel(str);

    if (str != "0")
        return;

    // look around
    dig_around(_X + 1, _Y);
    dig_around(_X - 1, _Y);
    dig_around(_X, _Y + 1);
    dig_around(_X, _Y - 1);
    dig_around(_X + 1, _Y + 1);
    dig_around(_X + 1, _Y - 1);
    dig_around(_X - 1, _Y - 1);
    dig_around(_X - 1, _Y + 1);
}

void tab::tick(wxTimerEvent& _Evt) {
    time_->SetLabel(std::to_string(seconds_++));
}