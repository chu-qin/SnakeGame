#include "helpdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>

HelpDialog::HelpDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("游戏帮助");
    setMinimumSize(500, 600);

    // 设置对话框样式
    setStyleSheet(
        "QDialog {"
        "   background-color: #2C3E50;"
        "}"
        "QLabel {"
        "   color: #ECF0F1;"
        "}"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 标题
    QLabel *titleLabel = new QLabel("🐍 贪吃蛇游戏说明", this);
    titleLabel->setStyleSheet(
        "QLabel {"
        "   color: #3498DB;"
        "   font-size: 24px;"
        "   font-weight: bold;"
        "   padding: 10px;"
        "}"
        );
    titleLabel->setAlignment(Qt::AlignCenter);

    // 内容区域（使用QTextEdit以支持滚动）
    QTextEdit *contentText = new QTextEdit(this);
    contentText->setReadOnly(true);
    contentText->setStyleSheet(
        "QTextEdit {"
        "   background-color: #34495E;"
        "   color: #ECF0F1;"
        "   border: 2px solid #3498DB;"
        "   border-radius: 8px;"
        "   padding: 15px;"
        "   font-size: 13px;"
        "   line-height: 1.6;"
        "}"
        );

    // 详细游戏说明
    QString helpContent = R"(
<h2 style='color:#3498DB;'>📖 游戏目标</h2>
<p>控制贪吃蛇吃食物，获得分数并变长。避免撞墙和撞到自己！</p>

<h2 style='color:#3498DB;'>⌨️ 操作控制</h2>
<p><b>移动方向：</b></p>
<ul>
<li>W 或 ↑ - 向上移动</li>
<li>S 或 ↓ - 向下移动</li>
<li>A 或 ← - 向左移动</li>
<li>D 或 → - 向右移动</li>
</ul>
<p><b>特殊操作：</b></p>
<ul>
<li>空格键（按住）- 临时加速</li>
<li>[开始] 按钮 - 开始/继续游戏</li>
<li>[暂停] 按钮 - 暂停游戏</li>
<li>[重新开始] - 重置游戏</li>
</ul>

<h2 style='color:#3498DB;'>🍎 食物类型</h2>
<p><span style='color:#FFD700;'>🟡 金色食物（大）</span></p>
<ul>
<li>效果：+10 分，蛇身变长</li>
<li>出现概率：60-80%（随等级降低）</li>
<li>这是主要得分来源</li>
</ul>

<p><span style='color:#32CD32;'>🟢 绿色食物（小）</span></p>
<ul>
<li>效果：临时加速，持续约10步</li>
<li>出现概率：15%</li>
<li>吃后蛇变成亮橙色</li>
<li>不会增加长度</li>
</ul>

<p><span style='color:#FF4500;'>🔴 红色食物（小）</span></p>
<ul>
<li>效果：临时减速，持续约10步</li>
<li>出现概率：5-20%（随等级提高）</li>
<li>吃后蛇变成冷蓝色</li>
<li>不会增加长度</li>
</ul>

<h2 style='color:#3498DB;'>📊 等级系统</h2>
<p><b>升级规则：</b></p>
<ul>
<li>每30分升一级</li>
<li>等级越高，基础速度越快</li>
<li>食物数量会随等级增加（最多5个）</li>
<li>金色食物出现概率会降低</li>
</ul>

<p><b>速度变化：</b></p>
<ul>
<li>1级：400ms/步（较慢）</li>
<li>2级：360ms/步</li>
<li>3级：320ms/步</li>
<li>...每级快40ms</li>
<li>7级+：120ms/步（最快）</li>
</ul>

<h2 style='color:#3498DB;'>🎵 背景音乐</h2>
<ul>
<li>0-59分：为什么要演奏（）</li>
<li>60-119分：电饭锅の小曲</li>
<li>120分以上：你 被 骗 了</li>
</ul>

<h2 style='color:#3498DB;'>💀 死亡条件</h2>
<ul>
<li>撞到墙壁边界</li>
<li>撞到自己的身体</li>
</ul>

<h2 style='color:#3498DB;'>🎮 祝你游戏愉快！</h2>
    )";

    contentText->setHtml(helpContent);

    // 关闭按钮
    QPushButton *closeButton = new QPushButton("关闭", this);
    closeButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498DB;"
        "   color: white;"
        "   font-size: 15px;"
        "   font-weight: bold;"
        "   padding: 10px 40px;"
        "   border: none;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #5DADE2;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #2980B9;"
        "}"
        );

    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    // 添加到布局
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(contentText);
    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

    setLayout(mainLayout);
}
