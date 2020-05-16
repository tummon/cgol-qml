#ifndef BOARD_H
#define BOARD_H

#include <QTimer>
#include <QAbstractTableModel>
#include <vector>

namespace board {
    constexpr int board_size = 25;
}

class Board : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit Board(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Q_INVOKABLE void tick();
    Q_INVOKABLE void randomize_buffer();
    Q_INVOKABLE void clear();


private:
    std::vector<bool> get_neighbours(int self_row, int self_col) const;
    int neighbour_score(int row, int col) const;

    int m_size;
    std::vector< std::vector<bool> > buffer;

signals:
    void cleared();

};

#endif // BOARD_H
