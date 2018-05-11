#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QSet>
#include <QString>
#include <QMutex>

class DataManager
{
public:
    DataManager();
    static DataManager& instance();

    void setBeginBlockNum(int block_num);
    int beginBlockNum();

    void setEndBlockNum(int block_num);
    int endBlockNum();

private:
    int begin_block_num;
    int end_block_num;
};

#endif // DATAMANAGER_H
