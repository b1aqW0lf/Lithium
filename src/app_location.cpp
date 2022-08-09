#include "app_location.h"


QT_BEGIN_NAMESPACE
namespace AppPath{


ExePath::ExePath(QObject *parent) : QObject(parent)
{

    //location/path of Elysium install directory - the application/root directory
    application_path = QCoreApplication::applicationDirPath();
    application_dir = QDir(application_path).absolutePath();

}

ExePath::~ExePath(){}

}
QT_END_NAMESPACE
