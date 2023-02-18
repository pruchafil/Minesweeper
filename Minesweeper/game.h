////////////////////////////////////////////////
//  2023 Prùcha Filip <prucha.fil@gmail.com>  //
////////////////////////////////////////////////

#pragma once
#include "pch.h"
#include "config.h"
#include <cstdint>
#include <random>

class game {
	uint16_t** field_;
    int64_t good_counter_;
    int64_t bad_counter_;
    bool game_ended_;

public:

    game();
    ~game();
    game(game&&) = delete;
    bool game_ended() const;

    const uint16_t* operator[](size_t i) const;

    uint16_t uncover_minefield(uint32_t _X, uint32_t _Y);
    bool select_bomb(uint32_t _X, uint32_t _Y);
    bool undo_select_bomb(uint32_t _X, uint32_t _Y);
    void reset();

private:
    void generate_bombs(bool _Allocate = true);
};