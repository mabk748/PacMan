#include <ctime>
#include "mainwindow.h"

#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    srand(time(0));

    PacmanWindow wnd;
    wnd.show();
    return app.exec();
}
