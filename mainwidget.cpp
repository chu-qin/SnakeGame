#include "mainwidget.h"
#include "gamewidget.h"
#include "helpdialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

MainWidget::MainWidget(QWidget *parent)
    : QWidget{parent}
{
    //窗口整体背景色
    setStyleSheet("QWidget#MainWidget { background-color: #4A5D6E; }");
    setObjectName("MainWidget");

    //横向排列的布局管理器，从左向右排
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    //左侧游戏区
    mainLayout->setSpacing(15);//留空
    mainLayout->setContentsMargins(15, 15, 15, 15);//与边界间隔
    //游戏本体
    GameWidget *gameWidget = new GameWidget(this);

    // 右侧信息面板
    QWidget *infoPanel = new QWidget(this);
    infoPanel->setFixedWidth(250);//固定宽度
    //信息面板样式
    infoPanel->setStyleSheet(
        "QWidget {"
        "   background-color: #3A4D5E;"
        "   border: 3px solid #5A7D9E;"
        "   border-radius: 12px;"
        "}"
        );
    //右侧信息区
    QVBoxLayout *infoLayout = new QVBoxLayout(infoPanel);
    infoLayout->setSpacing(10);
    infoLayout->setContentsMargins(15, 15, 15, 15);

    //标题样式
    QLabel *titleLabel = new QLabel("🐍 贪吃蛇", infoPanel);
    titleLabel->setStyleSheet(
        "QLabel {"
        "   color: #00FFF0;"
        "   font-size: 26px;"
        "   font-weight: bold;"
        "   padding: 8px;"
        "   border: none;"
        "   background: transparent;"
        "}"
        );
    titleLabel->setAlignment(Qt::AlignCenter);//居中

    //分数显示
    QLabel *scoreLabel = new QLabel("000000000000", infoPanel);
    scoreLabel->setStyleSheet(
        "QLabel {"
        "   background-color: #1A1A2E;"
        "   color: #00FFF0;"
        "   font-family: 'Courier New', monospace;"
        "   font-size: 22px;"
        "   font-weight: bold;"
        "   padding: 12px 8px;"
        "   border: 2px solid #2A2A4E;"
        "   border-radius: 8px;"
        "   letter-spacing: 2px;"
        "}"
        );
    scoreLabel->setAlignment(Qt::AlignCenter);

    //时间和等级
    QLabel *timeLabel = new QLabel("⏱ 时间: 0 秒", infoPanel);
    QLabel *levelLabel = new QLabel("⭐ 等级: 1", infoPanel);

    QString infoStyle =
        "QLabel {"
        "   background-color: #2E3D4E;"
        "   color: #FFFFFF;"
        "   font-size: 15px;"
        "   font-weight: bold;"
        "   padding: 8px;"
        "   border-radius: 6px;"
        "   border: none;"
        "}";

    timeLabel->setStyleSheet(infoStyle);
    levelLabel->setStyleSheet(infoStyle);
    timeLabel->setAlignment(Qt::AlignCenter);
    levelLabel->setAlignment(Qt::AlignCenter);

    // 连接信号
    connect(gameWidget, &GameWidget::scoreChanged, this, [=](int s) {
        scoreLabel->setText(QString("%1").arg(s, 12, 10, QChar('0')));//将分数s格式化为12位10进制数字，不足位用0补齐
    });

    connect(gameWidget, &GameWidget::timeChanged, this, [=](int t) {
        timeLabel->setText(QString("⏱ 时间: %1 秒").arg(t));
    });

    connect(gameWidget, &GameWidget::levelChanged, this, [=](int lv) {
        levelLabel->setText(QString("⭐ 等级: %1").arg(lv));
    });

    //食物统计区
    QLabel *statsTitle = new QLabel("📊 食物统计", infoPanel);
    statsTitle->setStyleSheet(
        "QLabel {"
        "   color: #3498DB;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   background: transparent;"
        "   border: none;"
        "   padding: 5px 0;"
        "}"
        );

    QLabel *goldFoodLabel = new QLabel("🟡 金色: 0", infoPanel);
    QLabel *greenFoodLabel = new QLabel("🟢 绿色: 0", infoPanel);
    QLabel *redFoodLabel = new QLabel("🔴 红色: 0", infoPanel);

    QString statsStyle =
        "QLabel {"
        "   background-color: #2C3E50;"
        "   color: #ECF0F1;"
        "   font-size: 13px;"
        "   padding: 6px;"
        "   border: 1px solid #3498DB;"
        "   border-radius: 4px;"
        "}";

    goldFoodLabel->setStyleSheet(statsStyle);
    greenFoodLabel->setStyleSheet(statsStyle);
    redFoodLabel->setStyleSheet(statsStyle);

    //连接食物统计信号
    connect(gameWidget, &GameWidget::foodStatsChanged,
            this, [=](int gold, int green, int red) {
                goldFoodLabel->setText(QString("🟡 金色: %1").arg(gold));
                greenFoodLabel->setText(QString("🟢 绿色: %1").arg(green));
                redFoodLabel->setText(QString("🔴 红色: %1").arg(red));
            });

    //控制按钮
    QPushButton *startBtn = new QPushButton("▶ 开始", infoPanel);
    QPushButton *pauseBtn = new QPushButton("⏸ 暂停", infoPanel);
    QPushButton *restartBtn = new QPushButton("🔄 重新开始", infoPanel);
    //按钮统一样式1
    QString btnStyle =
        "QPushButton {"
        "   background-color: #3A8FB7;"
        "   color: white;"
        "   font-size: 15px;"
        "   font-weight: bold;"
        "   padding: 10px;"
        "   border: none;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #4A9FC7;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #2A7FA7;"
        "}";

    startBtn->setStyleSheet(btnStyle);
    pauseBtn->setStyleSheet(btnStyle);
    //按钮统一样式2
    QString restartStyle =
        "QPushButton {"
        "   background-color: #E74C3C;"
        "   color: white;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   padding: 10px;"
        "   border: none;"
        "   border-radius: 6px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #EC7063;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #C0392B;"
        "}";
    restartBtn->setStyleSheet(restartStyle);

    //连接按钮信号
    connect(startBtn, &QPushButton::clicked, this, [=]() {
        gameWidget->startGame();
        gameWidget->setFocus();//焦点回归游戏
    });
    connect(pauseBtn, &QPushButton::clicked, this, [=]() {
        gameWidget->pauseGame();
        gameWidget->setFocus();
    });
    connect(restartBtn, &QPushButton::clicked, this, [=]() {
        gameWidget->restartGame();
        gameWidget->setFocus();
    });

    // 分割线
    QFrame *line1 = new QFrame(infoPanel);
    line1->setFrameShape(QFrame::HLine);
    line1->setStyleSheet("background-color: #3498DB; border: none;");
    line1->setFixedHeight(2);

    //帮助按钮
    QPushButton *helpBtn = new QPushButton("点击查看游戏说明", infoPanel);
    helpBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #95A5A6;"
        "   color: white;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   padding: 10px;"
        "   border: none;"
        "   border-radius: 6px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ABB7B7;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #7F8C8D;"
        "}"
        );

    connect(helpBtn, &QPushButton::clicked, this, [=]() {
        HelpDialog dialog(this);
        dialog.exec();//弹出弹窗
    });

    //简短提示
    QLabel *tipLabel = new QLabel(
        "<p style='text-align:center; margin:4px 0;'>"
        "<b>WASD / 方向键</b> 移动<br>"
        "<b>空格键</b> 加速<br>"
        "点击 [游戏说明] 查看详情"
        "</p>",
        infoPanel
        );
    tipLabel->setStyleSheet(
        "QLabel {"
        "   color: #BDC3C7;"
        "   font-size: 12px;"
        "   padding: 8px;"
        "   background-color: #2C3E50;"
        "   border: 1px solid #3498DB;"
        "   border-radius: 6px;"
        "}"
        );
    tipLabel->setWordWrap(true);

    //组装布局
    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(scoreLabel);
    infoLayout->addWidget(timeLabel);
    infoLayout->addWidget(levelLabel);
    infoLayout->addWidget(statsTitle);
    infoLayout->addWidget(goldFoodLabel);
    infoLayout->addWidget(greenFoodLabel);
    infoLayout->addWidget(redFoodLabel);
    infoLayout->addWidget(startBtn);
    infoLayout->addWidget(pauseBtn);
    infoLayout->addWidget(restartBtn);
    infoLayout->addWidget(line1);
    infoLayout->addWidget(helpBtn);
    infoLayout->addWidget(tipLabel);
    infoLayout->addStretch();//底部自动填充
    //依次放入两个“小盒”
    mainLayout->addWidget(gameWidget);
    mainLayout->addWidget(infoPanel);

    setLayout(mainLayout);
    setWindowTitle("动感贪吃蛇");

    //游戏区域宽度+，高度一样
    //即限定最小窗口大小
    resize(gameWidget->width() + 280, gameWidget->height() + 30);
}
