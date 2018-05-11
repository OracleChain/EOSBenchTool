#include "datamanager.h"

#include <QMutexLocker>

DataManager::DataManager()
{
    begin_block_num = 0;
    end_block_num = 0;
}

DataManager &DataManager::instance()
{
    static DataManager dataManager;
    return dataManager;
}

void DataManager::setBeginBlockNum(int block_num)
{
    begin_block_num = block_num;
}

int DataManager::beginBlockNum()
{
    return begin_block_num;
}

void DataManager::setEndBlockNum(int block_num)
{
    end_block_num = block_num;
}

int DataManager::endBlockNum()
{
    return end_block_num;
}
