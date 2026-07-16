#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QLabel>
#include "obstacle.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    int playerX;      // X координата игрока
    int playerY;      // Y координата игрока
    int playerSize;   // Размер игрока
    int playerSpeed;  // Скорость игрока
    double globalSpeed; // Скорость препятствий
    int speedLabelValue;
    int bestScoreValue;

    // ФЛАГИ ДВИЖЕНИЯ
    bool leftPressed;   // true если левая стрелка зажата
    bool rightPressed;  // true если правая стрелка зажата
    bool spacePressed;
    int spawnTimerValue;

    // Звезды
    struct Star {
        int x, y;
        int brightness;
    };
    QVector<Star> stars;
    void initStars();
    void clearStars();

    struct Particle {
        int x, y;
        int life;
    };
    QVector<Particle> particles;

    // ===== ПРЕПЯТСТВИЯ =====
    QVector<Obstacle*> obstacles;  // Список всех препятствий
    QTimer *spawnTimer;            // Таймер для создания новых

    // ===== ИГРОВАЯ СТАТИСТИКА =====
    int score;                     // Счет (сколько препятствий избежал)
    bool gameOver;                 // Флаг окончания игры

    // ===== ТАЙМЕР ИГРОВОГО ЦИКЛА =====
    QTimer *gameTimer;

    // ===== UI ЭЛЕМЕНТЫ =====
    QLabel *scoreLabel;
    QLabel *speedLabel;
    QLabel *hintLabel;
    QLabel *bestScoreLabel;

    void restartGame();
    void returnToMenu();


private slots:  //Слоты для таймера
    void updateGame();      // Игровой цикл (движение, столкновения)
    void spawnObstacle();   // Создание нового препятствия
protected:  // protected означает "доступно только мне и наследникам"
    void paintEvent(QPaintEvent *event) override;  // ОБЯЗАТЕЛЬНО такое же имя и параметры
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};
#endif // MAINWINDOW_H
