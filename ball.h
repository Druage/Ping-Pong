#ifndef BALL
#define BALL

#include <QQuickItem>
#include <QColor>
#include <QPoint>
#include <QtGlobal>
#include <QQuickWindow>
//#include <QThread>

class Ball : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool coinFlip READ coinFlip WRITE setCoinFlip NOTIFY coinFlipChanged)
    Q_PROPERTY(bool movingUp READ movingUp WRITE setMovingUp NOTIFY movingUpChanged)
    Q_PROPERTY(bool movingLeft READ movingLeft WRITE setMovingLeft NOTIFY movingLeftChanged)
    Q_PROPERTY(int goalScored READ goalScored WRITE setGoalScored NOTIFY goalScoredChanged)
    Q_PROPERTY(qreal speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(bool refreshingState READ refreshingState WRITE setRefreshingState NOTIFY refreshingStateChanged)

public:
    Ball();
    ~Ball();
    void setColor(QColor &color);
    void setCoinFlip(bool coinFlip);
    void setMovingLeft(bool movingLeft);
    void setMovingUp(bool movingUp);
    void setSpeed(qreal &speed);
    void setGoalScored(int goal);
    void setRefreshingState(bool &state);

    QColor color();
    bool coinFlip();
    bool movingUp();
    bool movingLeft();
    qreal speed();
    int goalScored();
    bool refreshingState();

private:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *paint_data);
    int randInt(int low, int high);
    void detectWindowCollision();
    void detectGoal();

    QColor m_color;
    bool m_coin_flip;
    bool m_move_left;
    bool m_move_up;
    qreal m_speed;
    int m_goal_scored;
    bool m_refreshing_state;

    //QThread collision_thread;
    QQuickItem *right_paddle;
    QQuickItem *left_paddle;

public slots:
    void flipCoin();
    void setRightPaddle(QObject *paddle);
    void setLeftPaddle(QObject *paddle);

private slots:
    void detectCollision();
    void moveBallX();
    void moveBallY();
    void handleWindowChanged(QQuickWindow *win);

signals:
    void colorChanged();
    void coinFlipChanged();
    void movingLeftChanged();
    void movingUpChanged();
    void speedChanged();
    void rightPaddleScored();
    void leftPaddleScored();
    void goalScoredChanged();
    void refreshingStateChanged();
};


#endif // BALL

