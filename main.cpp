#include <QApplication>
#include <QQmlApplicationEngine>

#include <ball.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<Ball>("pong.assets", 1, 0, "Ball");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
