#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class MenuWindow;
}

class MenuWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::MenuWindow *ui;
    MainWindow *gameWindow = nullptr;
};

#endif

//#include <QWidget>	Базовый класс для окна
//#include "mainwindow.h"	Чтобы создать игровое окно
//Q_OBJECT	Включить сигналы/слоты
//explicit MenuWindow(...)	Конструктор
//~MenuWindow()	Деструктор
//private slots:	Слоты для кнопок
//Ui::MenuWindow *ui	Интерфейс из Qt Designer
//MainWindow *gameWindow	Указатель на игровое окно