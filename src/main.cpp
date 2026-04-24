#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_LOGGING_RULES", "qt.multimedia.ffmpeg=false");
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Spectral Canvas");
    w.show();
    return a.exec();
}
