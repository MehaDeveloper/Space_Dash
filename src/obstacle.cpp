#include "obstacle.h"
#include <cstdlib>
#include <QVector>

Obstacle::Obstacle(int windowWidth, double initialSpeed)
{
    // Размер препятствия
    width = 30 + rand() % 70;
    height = 30 + rand() % 40;
    x = rand() % (windowWidth - width);
    y = -height;
    // Скорость падения
    speed = initialSpeed;
    QVector<QColor> colors = {
        QColor("#D4266D"),  // Малиновый
        QColor("#F34723"),  // Гранатовый
        QColor("#6495ED"),  // Васильковый
        QColor("#50C878"),  // Изумрудный
        QColor("#FFD700"),  // Золотой
        QColor("#DA70D6"),  // Орхидея
        QColor("#8A2BE2"),  // Сине-лиловый
        QColor("#FF4040"),  // Глубокий коралловый
        QColor("#036C56"),
    };
    color = colors[rand() % colors.size()];
}
// ОБНОВЛЕНИЕ - вызывается каждый кадр
void Obstacle::update()
{
    y += speed;
}
//Возврашаем прямоугольник
QRect Obstacle::getRect() const
{
    // Создаем и возвращаем прямоугольник с текущими координатами
    return QRect(x, y, width, height);
}
// Проверка выхода за экран
bool Obstacle::isOffScreen() const
{
    // Если Y координата больше высоты окна (600) → упало ниже
    return y > 600;
}
