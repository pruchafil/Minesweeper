////////////////////////////////////////////////
//  2023 Prùcha Filip <prucha.fil@gmail.com>  //
////////////////////////////////////////////////

#include "pch.h"
#include "game.h"
#include <unordered_set>
#include <set>

game::game() : field_(new uint16_t* [g_field_size_y]), good_counter_(0), bad_counter_(0), game_ended_(false) {
    generate_bombs();
}

game::~game() {
    for (size_t i = 0ull; i < g_field_size_y; ++i) delete[] field_[i]; delete[] field_;
}

/// <summary>
/// how many bombs is around, 9 = you picked a bomb
/// </summary>
uint16_t game::uncover_minefield(uint32_t _X, uint32_t _Y) {
    if (_X >= g_field_size_x || _X < 0)
        throw std::invalid_argument("x index is invalid");
    if (_Y >= g_field_size_y || _Y < 0)
        throw std::invalid_argument("y index is invalid");

    if (field_[_Y][_X] == 9)
        game_ended_ = true;

    return field_[_Y][_X];
}

/// <summary>
/// player's guess where the bomb lies
/// </summary>
/// <returns>you won</returns>
bool game::select_bomb(uint32_t _X, uint32_t _Y) {
    if (field_[_Y][_X] == 9)
        return ++good_counter_ == g_bombs_count && bad_counter_ == 0;

    ++bad_counter_;
    return false;
}

/// <summary>
/// player's undo of guess where the bomb lies
/// </summary>
/// <returns>you won</returns>
bool game::undo_select_bomb(uint32_t _X, uint32_t _Y) {
    if (field_[_Y][_X] == 9) {
        --good_counter_;
        return false;
    }

    return good_counter_ == g_bombs_count && --bad_counter_ == 0;
}

void game::reset() {
    good_counter_ = 0;
    bad_counter_ = 0;
    game_ended_ = false;
    for (size_t i = 0ull; i < g_field_size_y; ++i)
        for (size_t j = 0ull; j < g_field_size_x; ++j)
            field_[i][j] = 0;
    generate_bombs(false);
}

void game::generate_bombs(bool _Allocate) {
    if (_Allocate)
    for (size_t i = 0ull; i < g_field_size_y; ++i) {
        field_[i] = new uint16_t[g_field_size_x];

        for (size_t j = 0ull; j < g_field_size_x; ++j)
            field_[i][j] = 0;
    }

    std::mt19937 mt((std::random_device()()));
    std::uniform_int_distribution<std::mt19937::result_type> uid_x(0ull, g_field_size_x - 1ull);
    std::uniform_int_distribution<std::mt19937::result_type> uid_y(0ull, g_field_size_y - 1ull);

    std::set<std::pair<int,int>> set;

    while (set.size() != g_bombs_count)
        set.emplace(uid_x(mt), uid_y(mt));

    for (const auto&[x,y] : set) {
        field_[y][x] = 9; // bomb

        const static std::function<void(const size_t&, const size_t&)> incr_if_exists = [&](const size_t& y, const size_t& x) {
            if (y < 0ull || y >= g_field_size_y) return;
            if (x < 0ull || x >= g_field_size_x) return;
            if (field_[y][x] == 9) return;

            ++field_[y][x];
        };

        incr_if_exists(y + 1ull, x);
        incr_if_exists(y - 1ull, x);
        incr_if_exists(y, x + 1ull);
        incr_if_exists(y, x - 1ull);
        incr_if_exists(y + 1ull, x + 1ull);
        incr_if_exists(y - 1ull, x - 1ull);
        incr_if_exists(y + 1ull, x - 1ull);
        incr_if_exists(y - 1ull, x + 1ull);
    }
}

bool game::game_ended() const { return game_ended_; }

const uint16_t* game::operator[](size_t i) const { return field_[i]; }