#ifndef APP_LOCATION_H
#define APP_LOCATION_H


#include <QCoreApplication>
#include <QDir>
#include <QObject>


QT_BEGIN_NAMESPACE
namespace AppPath{
    Q_NAMESPACE

class ExePath : public QObject{
    Q_OBJECT

public:
    explicit ExePath (QObject *parent = nullptr);
    ~ExePath();

public:
    //install/location directory path of Barium executable
    QString application_path{};
    QString application_dir{};

};

}
QT_END_NAMESPACE
#endif // APP_LOCATION_H
