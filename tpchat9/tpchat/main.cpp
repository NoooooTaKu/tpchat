#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QString qss;
//    QFile qssFile("my.qss");
//    qssFile.open(QFile::ReadOnly);
//    if(qssFile.isOpen())
//    {
//        qss = QLatin1String(qssFile.readAll());
//        a.setStyleSheet(qss);
//        qssFile.close();
//    }

    MainWindow w;
    // w.show();

    return a.exec();
}
