#include "cell.h"

cell_t::cell_t() {
    sprite = new sf::Sprite();
}

cell_t::~cell_t() {
    delete sprite;
}

void cell_t::place_mine() {
    is_mine = true;
}

bool cell_t::reveal() {
    if (is_flagged) return false;

    is_revealed = true;
    return is_mine;
}

bool cell_t::unreveal(){
    if (is_flagged) return false;

    is_revealed = false;
    return is_mine;
}

void cell_t::toggle_flag(int &noMines) {
    if(is_flagged)
        ++noMines;
    else
        --noMines;
    is_flagged = !is_flagged;
}

void cell_t::unflag() {
    is_flagged = false;
}

void cell_t::increment_adjacent_mines() {
    ++adjacent_mines;
}

bool cell_t::auto_reveal() {
    return !is_mine;
}

bool cell_t::auto_reveal_neighbors() {
    return !is_mine && adjacent_mines == 0;
}

char cell_t::get_display_value() {
    if (is_flagged) return 'f';
    if (!is_revealed) return 'n';
    if (is_mine) return 'b';

    return (char) (adjacent_mines + '0');
}

bool cell_t::is_mined()
{
    if (is_mine) return true;
    else
        return false;
}

bool cell_t::is_reveal()
{
    if (is_revealed) return true;
    else
        return false;
}

void cell_t::reset() {
    adjacent_mines = 0;
    is_flagged = false;
    is_revealed = false;
    is_mine = false;
}
