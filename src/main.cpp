#include "menuwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/runner_icon.ico"));
    MenuWindow menu;
    menu.show();
    return QApplication::exec();
}