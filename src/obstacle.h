#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QRect>
#include <QColor>
class Obstacle
{
public:
    // КОНСТРУКТОР - создает препятствие
    // windowWidth: ширина окна (нужна, чтобы случайно определить X)
    Obstacle(int windowWidth, double initialSpeed);

    // ОБНОВЛЕНИЕ - вызывается каждый кадр
    void update();

    // ПОЛУЧИТЬ ПРЯМОУГОЛЬНИК - для проверки столкновений и рисования
    QRect getRect() const;

    // ПРОВЕРКА - упало ли препятствие ниже экрана
    bool isOffScreen() const;

    // ГЕТТЕРЫ - получить координаты
    int getX() const { return x; }
    int getY() const { return y; }
    QColor getColor() { return color; }
    void setSpeed(double newSpeed) { speed = newSpeed; }

private:
    int x;        // Координата X (где появится, случайная)
    int y;        // Координата Y (начинается с 0, сверху)
    int width;    // Ширина препятствия
    int height;   // Высота препятствия
    double speed;    // Скорость падения (пикселей за кадр)
    QColor color;    // Цвет
};
#endif // OBSTACLE_H
