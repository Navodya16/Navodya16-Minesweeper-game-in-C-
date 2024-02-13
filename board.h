#include <SFML/Graphics.hpp>
#include "image_manager.h"
#include "cell.h"

using std::vector;
using std::pair;
using std::fstream;

struct board_t {
private:
    int width;
    int height;
    cell_t ***cells;

public:
    board_t(int W, int H);

    void reset();

    void place_mines(int count);

    void place_mine(unsigned int row, unsigned int col);

    void reveal_cell(unsigned int row, unsigned int col);

    void reveal_all_cells();

    void reveal_all_mines();

    void unreveal_all_mines();

    int countNonMines();

    void toggle_flag_cell(unsigned int row, unsigned int col, int &noMines);

    void draw(sf::RenderWindow* window, ImageManager *ImageManager, int cell_size);

    vector<pair<int, int> > get_adjacent_positions(unsigned int row, unsigned int col);

    ~board_t();
};
