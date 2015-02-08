#include "ball.h"

#include <QSGSimpleRectNode>
#include <QQuickWindow>
#include <qdebug.h>
#include <QTime>

Ball::Ball()
{


    m_color = Qt::white;
    m_speed = 1;
    m_move_left = coinFlip();
    m_move_up = coinFlip();
    m_goal_scored = -1;
    m_refreshing_state = false;

    right_paddle = nullptr;
    left_paddle = nullptr;

    QTime time_seed = QTime::currentTime();
    qsrand(static_cast<uint>(time_seed.msec()));

    m_coin_flip = randInt(0, 100) % 2 == 0;

    setFlag(QQuickItem::ItemHasContents, true);

    //connect(&collision_thread, &QThread::started, this, &Ball::startCollisionDetection, Qt::DirectConnection);

    //collision_thread.start(QThread::TimeCriticalPriority);
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

}

Ball::~Ball()
{
    //collision_thread.quit();
}

void Ball::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(window(), &QQuickWindow::beforeSynchronizing, this, &Ball::detectCollision, Qt::DirectConnection);
    }
}

void Ball::setMovingLeft(bool movingLeft)
{
    m_move_left = movingLeft;
    emit movingLeftChanged();
}

void Ball::setMovingUp(bool movingUp)
{
    m_move_up = movingUp;
    emit movingUpChanged();
}

void Ball::moveBallX()
{
    if (movingLeft())
        setX(x() - 1 * speed());
    else
        setX(x() + 1 * speed());
}

void Ball::moveBallY()
{
    if (movingUp())
        setY(y() - 1 * speed());
    else
        setY(y() + 1 * speed());
}

bool Ball::movingLeft()
{
    return m_move_left;
}

bool Ball::movingUp()
{
    return m_move_up;
}

int Ball::randInt(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

void Ball::setRefreshingState(bool &state)
{
    m_refreshing_state = state;
    emit refreshingStateChanged();
}

void Ball::setColor(QColor &color)
{
    m_color = color;
    emit colorChanged();
}

void Ball::setCoinFlip(bool coinFlip)
{
    m_coin_flip = coinFlip;
    emit coinFlipChanged();
}

void Ball::setSpeed(qreal &speed)
{
    m_speed = speed;
    emit speedChanged();
}

QColor Ball::color()
{
    return m_color;
}

bool Ball::coinFlip()
{
    return m_coin_flip;
}

qreal Ball::speed()
{
    return m_speed;
}

int Ball::goalScored()
{
    return m_goal_scored;
}

bool Ball::refreshingState()
{
    return m_refreshing_state;
}

void Ball::setGoalScored(int goal)
{
    m_goal_scored = goal;
    emit goalScoredChanged();
}

void Ball::flipCoin()
{
    setCoinFlip(randInt(0, 100) % 2 == 0);
}

void Ball::detectWindowCollision()
{
    if (!window())
        return;

    if (y() <= 0) {
        if (movingUp())
            setMovingUp(false);
    }

    else if (y() >= window()->height() - height()) {
        if (!movingUp())
            setMovingUp(true);
    }
}

void Ball::detectGoal()
{
    if (!window() || refreshingState())
        return;

    if (x() <= 0)
        setGoalScored(2);

    else if (x() >= window()->width())
        setGoalScored(1);
}



void Ball::detectCollision()
{
    //qDebug() << "false";
    if (!right_paddle && !left_paddle)
        return;

    qreal topSide = y();
    qreal bottomSide = y() + width();
    QPoint topLeftEdge = QPoint(x(), y());
    QPoint topRightEdge = QPoint(x() + width(), y());
    QPoint bottomRightEdge = QPoint(x() + width(), y() + height());
    QPoint bottomLeftEdge = QPoint(x(), y() + height());

    // Add in detection for top and bottom of paddles.


    if (!movingLeft()) {


        qreal rightSide = x() + width();

        QPoint right_paddle_topLeftEdge = QPoint(right_paddle->x(), right_paddle->y());
        QPoint right_paddle_bottomLeftEdge = QPoint(right_paddle->x(), right_paddle->y() + right_paddle->height());
        QPoint right_paddle_bottomRightEdge = QPoint(right_paddle->x() + right_paddle->width(), right_paddle->y() + right_paddle->height());
        QPoint right_paddle_topRightEdge = QPoint(right_paddle->x() + right_paddle->width(), right_paddle->y());


        if (right_paddle_topLeftEdge.x() == bottomRightEdge.x() && right_paddle_topLeftEdge.y() == bottomRightEdge.y()) {
            if (!movingLeft())
                setMovingLeft(true);
            if (!movingUp())
                setMovingUp(true);

        }

        else if (right_paddle_bottomLeftEdge.x() == topRightEdge.x() && right_paddle_bottomLeftEdge.y() == topRightEdge.y()) {
            if (movingLeft())
                setMovingLeft(false);
            if (movingUp())
                setMovingUp(false);
        }

        else if ((bottomSide == right_paddle->y()
                 && (bottomLeftEdge.x() > right_paddle_topLeftEdge.x()
                 && bottomRightEdge.x() <= right_paddle_topRightEdge.x()))) {

            if (!movingUp())
                setMovingUp(true);
        }

        else if ((topSide == right_paddle_bottomLeftEdge.y()
                 && (topLeftEdge.x() > right_paddle_bottomLeftEdge.x()
                 && topRightEdge.x() <= right_paddle_bottomRightEdge.x()))) {
            qDebug() << "hit bottom";
            if (movingUp())
                setMovingUp(false);
        }
//reduce the bounds on this V
        else if ((topRightEdge.y() >= right_paddle_topLeftEdge.y() && bottomRightEdge.y() <= right_paddle_bottomLeftEdge.y())

            || (topRightEdge.y() < right_paddle_topLeftEdge.y()
            && (bottomRightEdge.y() < right_paddle_bottomLeftEdge.y()
            && bottomRightEdge.y() > right_paddle_topLeftEdge.y()))

            || (topRightEdge.y() > right_paddle_topLeftEdge.y()
            && (topRightEdge.y() < right_paddle_bottomLeftEdge.y()
            && bottomRightEdge.y() > right_paddle_bottomLeftEdge.y()))) {
            if (rightSide >= right_paddle->x() && rightSide < right_paddle->x() + right_paddle->width()) {
                setMovingLeft(true);
                qDebug() << "hit left";

            }
        }
    }

    else if (movingLeft()) {

        qreal leftSide = x();

        QPoint left_paddle_topRightEdge = QPoint(left_paddle->x() + left_paddle->width(), left_paddle->y());
        QPoint left_paddle_bottomRightEdge = QPoint(left_paddle->x() + left_paddle->width(), left_paddle->y() + left_paddle->height());


        if ((topLeftEdge.y() >= left_paddle_topRightEdge.y() && bottomLeftEdge.y() <= left_paddle_bottomRightEdge.y())

            || (topLeftEdge.y() < left_paddle_topRightEdge.y()
            && (bottomLeftEdge.y() < left_paddle_bottomRightEdge.y()
            && bottomLeftEdge.y() > left_paddle_topRightEdge.y()))

            || (topLeftEdge.y() > left_paddle_topRightEdge.y()
            && (topLeftEdge.y() < left_paddle_bottomRightEdge.y()
            && bottomLeftEdge.y() > left_paddle_bottomRightEdge.y()))) {

            if (leftSide <= left_paddle->x() + left_paddle->width() && leftSide > left_paddle->x()) {
                setMovingLeft(false);
            }
        }
    }

    detectWindowCollision();
    detectGoal();

    moveBallX();
    moveBallY();

    update();
}

void Ball::setLeftPaddle(QObject *paddle)
{
    left_paddle = static_cast<QQuickItem *>(paddle);
}

void Ball::setRightPaddle(QObject *paddle)
{
    right_paddle = static_cast<QQuickItem *>(paddle);
}

QSGNode *Ball::updatePaintNode(QSGNode *node, UpdatePaintNodeData *paint_data)
{
    Q_UNUSED(paint_data);

    QSGSimpleRectNode *rect = static_cast<QSGSimpleRectNode *>(node);
    if (!rect) {
        rect = new QSGSimpleRectNode();
        rect->setColor(color());
    }

    rect->setRect(boundingRect());

    return rect;

}
