#ifndef DETECT_STORAGE_H
#define DETECT_STORAGE_H

#include <QObject>
#include <QStorageInfo>


class DetectStorage : public QObject
{
    Q_OBJECT

public:
    explicit DetectStorage(QObject *parent = nullptr);
    ~DetectStorage();

    //creating the variables that hold values for available disk space
    //total disc space, and their comparison
private:
    QStorageInfo storage{};

public:
    QString stor_avail{};//available disk space
    QString stor_total{};//total disk space
    QString stor_data{};//available disk space/total disk space
};

#endif // DETECT_STORAGE_H
