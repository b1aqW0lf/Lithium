#include "app_version.h"
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
    MainWindow window;

    window.setWindowTitle("Lithium v"+compiled_date+"-"+git_hash_str);
    window.setWindowIcon(QIcon(":/images/resources/Logo1.png"));
    app.setOrganizationName("The Lithium Team");
    app.setApplicationName("Lithium");
    app.setDesktopFileName("Lithium");
    app.setApplicationVersion(compiled_date+"-"+git_hash_str);

    //get process ID for the application
    QString app_pid{};
    app_pid.setNum(QCoreApplication::applicationPid());

    window.show();
    return app.exec();
}
