#include "gamewidget.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QPainterPath>

GameWidget::GameWidget(QWidget *parent)
    : QWidget{parent}, dir(Direction::Right)
{
    //创建游戏本体窗口
    setFixedSize(cols*cellsize, rows*cellsize);

    gameOver = false;
    basespeed = 400;
    boostspeed = 100;
    currentspeed = basespeed;
    isBoosting = false;
    effectCounter = 0;

    snake.append(QPoint(10,10));
    snake.append(QPoint(9,10));
    snake.append(QPoint(8,10));

    connect(&timer, &QTimer::timeout, this, &GameWidget::moveSnake);

    score = 0;
    level = 1;
    elapsedTime = 0;
    timeCounter = 0;

    //初始化食物统计
    scoreFoodCount = 0;
    speedUpFoodCount = 0;
    slowDownFoodCount = 0;

    while (foods.size() < maxFoodCountForLevel(level)) {
        generateFood();
    }
    //窗口焦点
    setFocusPolicy(Qt::StrongFocus);

    // BGM初始化
    audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(0.4f);

    bgmPlayer = new QMediaPlayer(this);
    bgmPlayer->setAudioOutput(audioOutput);

    isMuted = false;
}

void GameWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//抗锯齿

    painter.fillRect(rect(), QColor(140, 190, 220, 90));

    // 绘制网格
    painter.setPen(QPen(QColor(160, 180, 200, 90), 2));
    for(int i = 0; i <= cols; i++) {
        painter.drawLine(i * cellsize, 0, i * cellsize, rows * cellsize);
    }
    for(int i = 0; i <= rows; i++) {
        painter.drawLine(0, i * cellsize, cols * cellsize, i * cellsize);
    }
    painter.setPen(Qt::NoPen);

    //画蛇，不同状态颜色
    QColor snakeColor;
    if (isBoosting) {
        snakeColor = QColor(255, 180, 120);//空格
    } else if (currentspeed < basespeed) {
        snakeColor = QColor(255, 180, 120);//食物加速
    } else if (currentspeed > basespeed) {
        snakeColor = QColor(120, 180, 255);//食物减速
    } else {
        snakeColor = QColor(255, 140, 0);
    }

    painter.setBrush(snakeColor);

    for(const QPoint &p : snake) {
        QRect cell(
            p.x()*cellsize,
            p.y()*cellsize,
            cellsize,
            cellsize
            );
        painter.drawRoundedRect(cell, 3, 3);
    }

    // 绘制食物
    for (const Food &f : foods) {
        QColor color;
        QColor pencolor;
        int foodSize = cellsize;

        if (f.type == FoodType::Score) {
            color = QColor(255, 215, 0);
            foodSize = cellsize - 8;
            pencolor = QColor(139, 69, 19);
        }
        else if (f.type == FoodType::SpeedUp) {
            color = QColor(50, 205, 50);
            foodSize = cellsize / 2;
            pencolor = QColor(0, 100, 0);
        }
        else {
            color = QColor(255, 69, 0);
            foodSize = cellsize / 2;
            pencolor = QColor(139, 0, 0);
        }

        painter.setBrush(color);
        painter.setPen(QPen(pencolor, 1));

        QRect rect(
            f.pos.x() * cellsize + (cellsize - foodSize) / 2,
            f.pos.y() * cellsize + (cellsize - foodSize) / 2,
            foodSize,
            foodSize
            );
        painter.drawRoundedRect(rect, 3, 3);

        // 高光效果
        QRect highlight(
            rect.x() + 4,
            rect.y() + 4,
            foodSize / 3,
            foodSize / 3
            );
        painter.setBrush(QColor(255, 255, 255, 100));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(highlight, 6, 6);
    }

    // 游戏结束画面
    if(gameOver) {
        painter.fillRect(rect(), QColor(0, 0, 0, 150));
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 18));
        painter.drawText(rect(), Qt::AlignCenter,
                         QString("得分: %1  等级: %2\n长度: %3\n\n点击 [重新开始] 再来一局")
                             .arg(score).arg(level).arg(snake.size()));
    }

    // 暂停画面
    if (!timer.isActive() && !gameOver) {
        painter.fillRect(rect(), QColor(0, 0, 0, 120));
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20));
        painter.drawText(rect(), Qt::AlignCenter, "点击 [开始] 开始游戏");
    }
}

//生成食物
void GameWidget::generateFood() {
    Food newFood;

    do {
        int x = QRandomGenerator::global()->bounded(cols);
        int y = QRandomGenerator::global()->bounded(rows);
        newFood.pos = QPoint(x, y);
    } while(snake.contains(newFood.pos));//防止食物生成在蛇身上

    //食物概率
    int scoreChance = 80 - level * 5;
    if (scoreChance < 60)
        scoreChance = 60;

    int r = QRandomGenerator::global()->bounded(100);
    if (r < scoreChance)
        newFood.type = FoodType::Score;
    else if (r < scoreChance + 15)
        newFood.type = FoodType::SpeedUp;
    else
        newFood.type = FoodType::SlowDown;

    foods.append(newFood);
}

//按键输入
void GameWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space && !event->isAutoRepeat() && !isBoosting) {
        //判断加速键，防止重复触发
        isBoosting = true;
        currentspeed = boostspeed;
        timer.start(currentspeed);
        return;
    }

    switch(event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        if(dir != Direction::Down)
            dir = Direction::Up;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        if(dir != Direction::Up)
            dir = Direction::Down;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        if(dir != Direction::Right)
            dir = Direction::Left;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        if(dir != Direction::Left)
            dir = Direction::Right;
        break;
    }
}
//持续按键，加速
void GameWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space && !event->isAutoRepeat() && isBoosting) {
        isBoosting = false;
        currentspeed = basespeed;
        timer.start(currentspeed);
    }
}

//槽函数（蛇的移动）
//整体逻辑：记录当前状态；计算新状态；更新；
void GameWidget::moveSnake() {
    if (gameOver)
        return;

    timeCounter += currentspeed;
    if (timeCounter >= 1000) {
        elapsedTime++;
        emit timeChanged(elapsedTime);
        timeCounter = 0;
    }

    QPoint head = snake.first();
    QPoint newHead = head;

    switch(dir) {
    case Direction::Up:
        newHead.ry() -= 1;
        break;
    case Direction::Down:
        newHead.ry() += 1;
        break;
    case Direction::Left:
        newHead.rx() -= 1;
        break;
    case Direction::Right:
        newHead.rx() += 1;
        break;
    }

    //死亡判断- 撞墙
    if(newHead.x() < 0 || newHead.x() >= cols || newHead.y() < 0 || newHead.y() >= rows) {
        gameOver = true;
        timer.stop();
        bgmPlayer->stop();
        update();
        return;
    }

    //死亡判断- 撞到自己
    for(int i = 1; i < snake.size(); ++i) {
        if(newHead == snake[i]) {
            gameOver = true;
            timer.stop();
            bgmPlayer->stop();
            update();
            return;
        }
    }
    //.prepend()表示把元素插到最前面，保持运动
    snake.prepend(newHead);

    //一次吃食物判断
    bool ateFood = false;
    for (int i = 0; i < foods.size(); ++i) {
        if (newHead == foods[i].pos) {
            if (foods[i].type == FoodType::Score) {//加分
                score += 10;
                emit scoreChanged(score);
                scoreFoodCount++;//统计
                emit foodStatsChanged(scoreFoodCount, speedUpFoodCount, slowDownFoodCount);

                updateBGM();

                int newLevel = score / 30 + 1;
                if (newLevel != level) {
                    level = newLevel;
                    emit levelChanged(level);

                    basespeed = baseSpeedForLevel(level);
                    if(!isBoosting) {
                        currentspeed = basespeed;
                        timer.start(currentspeed);
                    }
                }

                ateFood = true;
            }
            else if (foods[i].type == FoodType::SpeedUp) {//加速
                //统计
                speedUpFoodCount++;
                emit foodStatsChanged(scoreFoodCount, speedUpFoodCount, slowDownFoodCount);

                currentspeed = boostspeed;
                effectCounter = 10;
                timer.start(currentspeed);
            }
            else if (foods[i].type == FoodType::SlowDown) {//减速
                //统计
                slowDownFoodCount++;
                emit foodStatsChanged(scoreFoodCount, speedUpFoodCount, slowDownFoodCount);

                currentspeed = basespeed + 200;
                effectCounter = 10;
                timer.start(currentspeed);
            }
            //删除食物
            foods.removeAt(i);

            while (foods.size() < maxFoodCountForLevel(level)) {
                generateFood();
            }

            break;
        }
    }

    if (!ateFood) {
        snake.removeLast();//删去最后一个身体
    }

    //速度特效计数器
    if(effectCounter > 0) {
        effectCounter--;
        if(effectCounter == 0 && !isBoosting) {
            currentspeed = basespeed;
            timer.start(currentspeed);
        }
    }

    update();
}

void GameWidget::resetGame() {
    snake.clear();

    snake.append(QPoint(10,10));
    snake.append(QPoint(9,10));
    snake.append(QPoint(8,10));

    dir = Direction::Right;
    score = 0;
    elapsedTime = 0;
    level = 1;
    timeCounter = 0;
    gameOver = false;
    isBoosting = false;
    effectCounter = 0;
    basespeed = 400;
    currentspeed = basespeed;

    //重置食物统计
    scoreFoodCount = 0;
    speedUpFoodCount = 0;
    slowDownFoodCount = 0;
    foods.clear();

    while(foods.size() < maxFoodCountForLevel(level)) {
        generateFood();
    }

    emit scoreChanged(score);
    emit levelChanged(level);
    emit timeChanged(elapsedTime);
    emit foodStatsChanged(scoreFoodCount, speedUpFoodCount, slowDownFoodCount);

    update();
}

void GameWidget::startGame() {
    //死亡后不允许开始
    if(gameOver) {
        return;
    }

    timer.start(currentspeed);
    timeCounter = 0;

    if(!isMuted) {
        if(!Path.isEmpty()) {
            //音乐已加载，检查是否暂停
            if(bgmPlayer->playbackState() == QMediaPlayer::PausedState) {
                bgmPlayer->play();
            }
        } else {
            updateBGM();
        }
    }
}

void GameWidget::pauseGame() {
    if(gameOver) {
        return;
    }

    timer.stop();
    bgmPlayer->pause();
}

void GameWidget::restartGame() {
    timer.stop();
    bgmPlayer->pause();
    resetGame();
    timer.start(currentspeed);

    if(!isMuted) {
        Path = "";
        updateBGM();
    }
}

//速度-等级计算
int GameWidget::baseSpeedForLevel(int lvl) {
    int speed = 400 - (lvl - 1) * 40;
    if(speed < 120) speed = 120;
    return speed;
}
//等级-食物数量换算
int GameWidget::maxFoodCountForLevel(int lvl) {
    if(lvl <= 2) return 2;
    if(lvl <= 4) return 3;
    if(lvl <= 6) return 4;
    return 5;
}
//分数-切歌实现
void GameWidget::updateBGM() {
    QString musicPath;
    if(score < 60)
        musicPath = "/music/bgm1.mp3";
    else if(score < 120)
        musicPath = "/music/bgm2.mp3";
    else
        musicPath = "/music/bgm3.mp3";

    if (Path == musicPath)
        return;

    Path = musicPath;

    bgmPlayer->stop();
    bgmPlayer->setSource(QUrl::fromLocalFile(musicPath));
    bgmPlayer->setLoops(QMediaPlayer::Infinite);

    if(!isMuted) {
        bgmPlayer->play();
    }
}


