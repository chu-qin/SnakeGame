#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QTimer>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QAudioOutput>

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);

public slots:
    void startGame();
    void pauseGame();
    void restartGame();

private:
    const int cellsize = 25;
    const int rows = 28;
    const int cols = 28;

    //用snake容器对象存储“蛇”的身体节点
    //QVector是Qt的动态数组容器
    //<QPoint>存储点坐标类
    QVector<QPoint> snake;

    //食物类枚举
    enum class FoodType{
        Score,
        SpeedUp,
        SlowDown
    };

    struct Food {
        QPoint pos;
        FoodType type;
    };
    QVector<Food> foods;

    int score;
    int elapsedTime;
    int level;
    int timeCounter;
    int effectCounter;

    //食物统计
    int scoreFoodCount;
    int speedUpFoodCount;
    int slowDownFoodCount;

    QTimer timer;
    int basespeed;
    int boostspeed;
    int currentspeed;
    bool isBoosting;

    void resetGame();

    //方向枚举
    enum class Direction {
        Up,
        Down,
        Left,
        Right
    };
    Direction dir;

    void moveSnake();
    void generateFood();
    int baseSpeedForLevel(int lvl);
    int maxFoodCountForLevel(int lvl);

    bool gameOver;

    // BGM相关
    QMediaPlayer *bgmPlayer;
    QAudioOutput *audioOutput;
    QString Path;
    bool isMuted;
    void updateBGM();

protected:
    void paintEvent(QPaintEvent *event) ;
    void keyPressEvent(QKeyEvent *event) ;
    void keyReleaseEvent(QKeyEvent *event) ;

signals:
    //信息信号
    void scoreChanged(int newScore);
    void timeChanged(int seconds);
    void levelChanged(int newLevel);

    //食物统计信号
    void foodStatsChanged(int scoreFood, int speedUpFood, int slowDownFood);
};

#endif
