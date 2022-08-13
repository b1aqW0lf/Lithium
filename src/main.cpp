#include "app_version.h"
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif

    QApplication app(argc, argv);
    MainWindow window;

    window.setWindowTitle("Lithium v"+compiled_date+"-"+git_hash_str);
    window.setWindowIcon(QIcon(":/images/resources/Logo1.png"));
    app.setOrganizationName("The Lithium Team");
    app.setApplicationName("Lithium");
    app.setDesktopFileName("Lithium");
    app.setApplicationVersion(compiled_date+"-"+git_hash_str);

    window.show();
    return app.exec();
}
