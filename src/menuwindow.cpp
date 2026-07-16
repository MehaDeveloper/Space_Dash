#include "menuwindow.h"
#include "ui_menuwindow.h"
#include "mainwindow.h"

MenuWindow::MenuWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuWindow)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    setWindowTitle("Space dash++");
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::on_pushButton_2_clicked()
{
    this->hide();
    MainWindow *gameWindow = new MainWindow();
    gameWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(gameWindow, &MainWindow::destroyed, this, &MenuWindow::show);
    gameWindow->show();
}

void MenuWindow::on_pushButton_clicked()
{
    QApplication::quit();
}
