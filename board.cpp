#include "board.h"
#include "random.h"
#include <iostream>

Board::Board(QObject *parent) :
    QAbstractTableModel(parent),
    m_size{board::board_size},
    buffer{std::vector<std::vector<bool>>(m_size, std::vector<bool>(m_size, true))} {
        randomize_buffer();
    }

int Board::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return m_size;
}

int Board::columnCount(const QModelIndex &parent) const{
    if (parent.isValid())
        return 0;
    return m_size;
}

QVariant Board::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    return QVariant(buffer[index.row()][index.column()]);
}

Qt::ItemFlags Board::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

bool Board::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::DisplayRole || data(index, role) == value)
        return false;

    buffer[index.row()][index.column()] = value.toBool();

    emit dataChanged(index, index, {role});

    return true;
}

void Board::randomize_buffer() {
    for (int i=0; i<m_size; i++){
        for (int j=0; j<m_size; j++) {
            buffer[i][j] = Random::randint(0,10) <= 4;
        }
    }
    emit dataChanged(index(0, 0), index(m_size - 1, m_size - 1));
}

void Board::clear()
{
    for (int i=0; i<m_size; i++){
        for (int j=0; j<m_size; j++) {
            buffer[i][j] = false;
        }
    }

    emit cleared();
    emit dataChanged(index(0, 0), index(m_size - 1, m_size - 1));
}

std::vector<bool> Board::get_neighbours(int self_row,int self_col) const {
    std::vector<bool> result;

    std::vector<int> rows = {self_row-1, self_row, self_row+1};
    std::vector<int> cols = {self_col-1, self_col, self_col+1};

    for (auto row: rows) {
        for (auto col: cols) {
            bool is_self = (row == self_row && col == self_col);
            bool outside_board = col < 0 || row <0 || col >= m_size || row >= m_size;

            if (is_self || outside_board)
                continue;

            result.push_back(buffer[row][col]);
        }
    }

    return result;
}

int Board::neighbour_score(int row, int col) const {
    int score = 0;
    auto neighbours = get_neighbours(row, col);

    for (const auto& i: neighbours) {
        score += i ? 1 : 0;
    }

    return score;
}


void Board::tick() {
    auto back_buffer = std::vector<std::vector<bool>>(m_size, std::vector<bool>(m_size, false));

    bool empty_board = true;
    for (int row=0; row<m_size; row++) {
        for(int col=0; col<m_size; col++) {

            auto score = neighbour_score(row, col);

            bool live_cell = buffer[row][col];
            bool two_live_neighbours = score == 2;
            bool three_live_neighbours = score == 3;

            if (live_cell && (two_live_neighbours || three_live_neighbours)) {
                // Survive
                back_buffer[row][col] = true;
                empty_board = false;
            } else if (!live_cell && three_live_neighbours){
                // Revive
                back_buffer[row][col] = true;
                empty_board = false;
            } else if (live_cell) {
                // Die
                back_buffer[row][col] = false;
            }
        }
    }

    buffer = move(back_buffer);

    if (empty_board) {
        emit cleared();
    }

    emit dataChanged(index(0, 0), index(m_size - 1, m_size - 1));
}
