#include "detect_storage.h"


DetectStorage::DetectStorage(QObject *parent) : QObject(parent)
{
    storage = QStorageInfo::root();
    stor_avail = stor_avail.setNum(storage.bytesAvailable()/1000/1000/1000.00) + " GB";
    stor_total = stor_total.setNum(storage.bytesTotal()/1000/1000/1000.00) + " GB";
    stor_data = stor_avail + "/" + stor_total;

}

DetectStorage::~DetectStorage(){}
