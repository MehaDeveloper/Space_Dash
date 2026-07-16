#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <ctime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    setWindowTitle("Space dash++");
    setFocusPolicy(Qt::StrongFocus);

    // Параметры игрока
    playerX = 400;
    playerY = 480;
    playerSize = 50;
    playerSpeed = 5;

    // Флаги движения
    leftPressed = false;
    rightPressed = false;
    spacePressed = false;

    // Игровая статистика
    score = 0;
    globalSpeed = 5;
    bestScoreValue = 0;
    gameOver = false;

    // Надписи счёта и подсказка
    scoreLabel = new QLabel(this);
    scoreLabel->setGeometry(20, 20, 200, 40);
    scoreLabel->setStyleSheet("color: white; font-size: 20px; font-weight: bold; font-family: Century Gothic");
    scoreLabel->setText("Score: 0");

    speedLabel = new QLabel(this);
    speedLabel->setGeometry(20, 50, 200, 40);
    speedLabel->setStyleSheet("color: white; font-size: 20px; font-weight: bold; font-family: Century Gothic");
    speedLabel->setText("Speed: 1");
    speedLabelValue = 1;

    bestScoreLabel = new QLabel(this);
    bestScoreLabel->setGeometry(20, 80, 200, 40);
    bestScoreLabel->setStyleSheet("color: white; font-size: 20px; font-weight: bold; font-family: Century Gothic");
    bestScoreLabel->setText("Best: 0");

    hintLabel = new QLabel(this);
    hintLabel->setGeometry(20, height() - 50, 300, 40);
    hintLabel->setStyleSheet("color: #a6a6a6; font-size: 14px; font-weight: bold; font-family: Century Gothic");
    hintLabel->setText("Press space to speed up");

    // Создание таёмера
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::updateGame);
    gameTimer->start(16);

    // Таймер создания препятсивий
    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &MainWindow::spawnObstacle);
    spawnTimerValue = 500;
    spawnTimer->start(spawnTimerValue);
    initStars();
}

MainWindow::~MainWindow()
{
    qDeleteAll(obstacles);
    obstacles.clear();
}

void MainWindow::initStars()
{
    for (int i = 0; i < 100; i++) {
        stars.append({rand() % width(), rand() % height(), rand() % 256});
    }
}

void MainWindow::clearStars()
{
    stars.clear();
}

void MainWindow::updateGame()
{
    //Движение игрока
    if (gameOver) {
        return;
    }

    int newDelay = 500 - (score * 3);
    if (newDelay < 150) newDelay = 150;

    if (newDelay != spawnTimerValue) {
        spawnTimerValue = newDelay;
        spawnTimer->start(spawnTimerValue);
    }

    int currentSpeed = playerSpeed;
    if (spacePressed) {
        currentSpeed *= 2;
    }
    // Движение влево
    if (leftPressed && playerX > 0) {
        playerX -= currentSpeed;
        if (playerX < 0) playerX = 0;
    }

    // Движение вправо
    if (rightPressed && playerX < width() - playerSize) {
        playerX += currentSpeed;
        if (playerX > width() - playerSize) playerX = width() - playerSize;
    }

    // Обновление препятствий
    for (int i = 0; i < obstacles.size(); i++) {
        // Падаем вниз
        obstacles[i]->update();
        // Если препятствие упало ниже экрана - удаляем и увеличиваем счет
        if (obstacles[i]->isOffScreen()) {
            delete obstacles[i];
            obstacles.removeAt(i);
            score++;

            if (score % 5 == 0) {
                globalSpeed += 0.3;
                speedLabelValue++;
                for (Obstacle *obs : obstacles) {
                    obs->setSpeed(globalSpeed);
                }
                qDebug() << "Speed increased! Score:" << score;
                speedLabel->setText("Speed: " + QString::number(speedLabelValue));
            }

            scoreLabel->setText("Score: " + QString::number(score));
            if (score > bestScoreValue) {
                bestScoreLabel->setText("Best: " + QString::number(score));
            }
            i--;
        }
    }

    //Добавляем выхлоп
    Particle p;
    p.x = playerX + playerSize / 2 + (rand() % 8 - 4);
    p.y = playerY + playerSize + 35;
    p.life = 30;
    particles.append(p);

    // Обновляем частицы
    for (int i = 0; i < particles.size(); i++) {
        particles[i].y += 2;
        particles[i].life--;
        if (particles[i].life <= 0 || particles[i].y > height()) {
            particles.removeAt(i);
            i--;
        }
    }

    // Ограничиваем количество частиц
    if (particles.size() > 100) {
        particles.removeFirst();
    }

    //Проверка столкновений
    //Создаем прямоугольник игрока
    QRect playerRect(playerX, playerY, 50, 85);
    // Проверяем каждое препятствие
    for (Obstacle *obs : obstacles) {
        if (playerRect.intersects(obs->getRect())) {
            gameOver = true;
            gameTimer->stop();
            spawnTimer->stop();
            update();
            qDebug() << "============================";
            qDebug() << "GAME OVER! Final Score:" << score;
            qDebug() << "============================";
            return;
        }
    }
    //Перерисовка экрана
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor("#000000"));
    gradient.setColorAt(1, QColor("#1a1a1a"));
    painter.fillRect(rect(), gradient);
    for (const Star &star : stars) {
        painter.setPen(QColor(star.brightness, star.brightness, star.brightness));
        painter.drawPoint(star.x, star.y);
    }
    // Рисуем частицы выхлопа
    for (const Particle &p : particles) {
        int lightValue = 100 + (15 - p.life) * 6;
        // Выбираем цвет в зависимости от нажатия пробела
        QColor exhaustColor;
        if (spacePressed) {
            exhaustColor = QColor("#FF0000");  // Красный при ускорении
        } else {
            exhaustColor = QColor("#FF7F50");  // Оранжевый обычно
        }
        painter.setBrush(exhaustColor.lighter(lightValue));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(p.x - 3, p.y - 3, 6, 6);
    }
    // Рисуем препятствия

    for (Obstacle *obs : obstacles) {
        QColor color = obs->getColor();
        painter.setBrush(QBrush(color));
        painter.setPen(QPen(color.darker(200), 4));
        painter.drawRoundedRect(obs->getRect(), 10, 10);
    }

    int centerX = playerX + playerSize / 2;
    int topY = playerY;
    int bottomY = playerY + playerSize;

    QLinearGradient shipGradient(centerX, topY, centerX, bottomY);
    shipGradient.setColorAt(0, QColor("#E8E8E8"));
    shipGradient.setColorAt(0.3, QColor("#A0A0A0"));
    shipGradient.setColorAt(0.7, QColor("#606060"));
    painter.setPen(QPen(QColor("#FEFEFE"), 2));
    painter.setBrush(shipGradient);

    QPolygon ship;
    int k = 5;
    ship << QPoint(centerX, topY)
         << QPoint(centerX - 2 * k, topY + 13 * k)
         << QPoint(centerX - 4 * k, topY + 12.5 * k)
         << QPoint(centerX - 4.5 * k, topY + 8 * k)
         << QPoint(centerX - 5 * k, topY + 13 * k)
         << QPoint(centerX - 5 * k, topY + 17 * k)
         << QPoint(centerX - 4 * k, topY + 17 * k)
         << QPoint(centerX - 4 * k, topY + 15 * k)
         << QPoint(centerX, topY + 17 * k)
         << QPoint(centerX + 4 * k, topY + 15 * k)
         << QPoint(centerX + 4 * k, topY + 17 * k)
         << QPoint(centerX + 5 * k, topY + 17 * k)
         << QPoint(centerX + 5 * k, topY + 13 * k)
         << QPoint(centerX + 4.5 * k, topY + 8 * k)
         << QPoint(centerX + 4 * k, topY + 12.5 * k)
         << QPoint(centerX + 2 * k, topY + 13 * k);
    painter.drawPolygon(ship);
    // Сообщения
    if (gameOver) {
        painter.fillRect(rect(), QColor(0, 0, 0, 180));
        // Центр окна
        int centerY = height() / 2;
        painter.setRenderHint(QPainter::TextAntialiasing, true);
        // Строка 1: GAME OVER
        QRect titleRect(0, centerY - 90, width(), 60);
        painter.setFont(QFont("Century Gothic", 48, QFont::Bold));
        painter.setPen(QColor(255, 80, 80));
        painter.drawText(titleRect, Qt::AlignCenter, "GAME OVER");
        // Строка 2: Score: 30
        QRect scoreRect(0, centerY - 20, width(), 50);
        painter.setFont(QFont("Century Gothic", 32));
        painter.setPen(QColor(255, 200, 50));
        painter.drawText(scoreRect, Qt::AlignCenter, "Score: " + QString::number(score));
        // Строка 3: Press R to restart
        QRect hintRect(0, centerY + 40, width(), 40);
        painter.setFont(QFont("Century Gothic", 20));
        painter.setPen(QColor(150, 200, 255));
        painter.drawText(hintRect, Qt::AlignCenter, "Press R to restart");
        // Строка 4: Press M to return menu
        QRect returnRect(0, centerY + 80, width(), 40);
        painter.setFont(QFont("Century Gothic", 20));
        painter.setPen(QColor(Qt::white)); //
        painter.drawText(returnRect, Qt::AlignCenter, "Press M to return to menu");

        if (score > bestScoreValue) {
            QRect bestRect(0, centerY + 120, width(), 40);
            painter.setFont(QFont("Century Gothic", 24));
            painter.setPen(QColor("#448061"));
            painter.drawText(bestRect, Qt::AlignCenter, "New Best score!");
        }
    }
}

void MainWindow::restartGame()
{
    // Очищаем все препятствия
    qDeleteAll(obstacles);
    obstacles.clear();
    // Сбрасываем параметры
    playerX = 400;
    if (score > bestScoreValue) {
        bestScoreValue = score;
    }
    score = 0;
    gameOver = false;
    globalSpeed = 5;
    speedLabelValue = 1;
    // Обновляем надпись со счетом
    scoreLabel->setText("Score: 0");
    speedLabel->setText("Speed: 1");
    // Перезапускаем таймеры
    gameTimer->start(16);
    spawnTimer->start(spawnTimerValue);
    clearStars();
    initStars();
    particles.clear();
    qDebug() << "Игра перезапущена!";
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Стрелки для движения
    if (event->key() == Qt::Key_Left) {
        leftPressed = true;
    }
    else if (event->key() == Qt::Key_Right) {
        rightPressed = true;
    }
    else if (event->key() == Qt::Key_Space) {
        spacePressed = true;
    }
    // Клавиша R для перезапуска
    else if ((event->key() == Qt::Key_R || event->text().toLower() == "к") && gameOver) {
        restartGame();
    }
    // Возврат в меню
    else if ((event->key() == Qt::Key_M || event->text().toLower() == "ь") && gameOver) {
        returnToMenu();
    }
}
void MainWindow::returnToMenu()
{
    // Останавливаем таймеры
    gameTimer->stop();
    spawnTimer->stop();
    // Закрываем игровое окно
    this->close();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left) {
        leftPressed = false;
    }
    else if (event->key() == Qt::Key_Right) {
        rightPressed = false;
    }
    else if(event->key() == Qt::Key_Space) {
        spacePressed = false;
    }
}

void MainWindow::spawnObstacle()
{
    if (gameOver) return;
    Obstacle *obstacle = new Obstacle(width(), globalSpeed);
    obstacles.append(obstacle);
    qDebug() << "Создано препятствие! Всего на экране:" << obstacles.size();
}


