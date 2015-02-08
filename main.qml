import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import pong.assets 1.0

ApplicationWindow {
    id: root;
    title: qsTr("Pong")
    width: Screen.width * 0.8
    height: 480
    visible: true
    property int paddleMovement: 20;
    property real gameSpeed: 8;

    function goalScored(player)
    {
        leftPaddle.y = root.height / 2;
        player.score += 1;
        ball.visible = false;
        gameReset.start();
    }

    color: "black";
    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: messageDialog.show(qsTr("Open action triggered"));
            }

        }
    }

    Text {
        id: player1Score;
        property int score: 0;
        anchors {
            top: parent.top;
            topMargin: 48;
        }
        x: root.width / 4
        text: player1Score.score;
        color: "white";
        font {
            pixelSize: 48;
        }


    }

    Text {
        id: player2Score;
        property int score: 0;
        anchors {
            top: parent.top;
            topMargin: 48;
        }
        x: root.width / 2 + (root.width / 4)
        text: player2Score.score;
        color: "white";
        font {
            pixelSize: 48;
        }
    }

    Column {
        // Center line
        z: 2;
        anchors.centerIn: parent;
        spacing: 10;
        Repeater {
            id: repeater;
            model: root.height / (15 + parent.spacing) ;
            //onCountChanged: console.log(count);
            delegate: Rectangle {
                width: 5;
                height: 15;
                color: "white";
            }

        }
    }

    Rectangle {
        id: leftPaddle;
        z: 1;
        color: "white";
        anchors {
            left: parent.left;
            leftMargin: 100;
        }
        height: 80;
        y: root.height / 2;
        width: 24;

        property point bottomRightEdge: Qt.point(x + width, y + height);
        property point bottomLeftEdge: Qt.point(x, y + height);
        property point topLeftEdge: Qt.point(x, y);
        property point topRightEdge: Qt.point(x + width, y);
        property int rightSide: x + width;
        property int leftSide: x;

        //Behavior on y {
          //  SmoothedAnimation {velocity: 100;}
        //}


    }

    Rectangle {

        id: rightPaddle;
        z: 1;
        color: "white";
        anchors {
            right: parent.right;
            rightMargin: 100;
        }
        focus: true;
        height: 80;
        width: 24;
        y: root.height / 2;
        Keys.onDownPressed: {
            if (rightPaddle.y !== root.height - rightPaddle.height)
                rightPaddle.y += root.paddleMovement;
        }

        Keys.onUpPressed: {
            if (rightPaddle.y !== 0)
                rightPaddle.y -= root.paddleMovement;
        }

        Behavior on y {
            SmoothedAnimation {duration: 100;}
        }

        property point bottomRightEdge: Qt.point(x + width, y + height);
        property point bottomLeftEdge: Qt.point(x, y + height);
        property point topLeftEdge: Qt.point(x, y);
        property point topRightEdge: Qt.point(x + width, y);
        property int rightSide: x + width;
        property int leftSide: x;

    }

    Ball {
        id: ball;
        //anchors.centerIn: parent;
        height: 16;
        width: 16;
        color: "white";
        x: root.width / 2;
        y: root.height / 2;
        z: 1;
        speed: root.gameSpeed;
        Component.onCompleted: {
            setLeftPaddle(leftPaddle);
            setRightPaddle(rightPaddle);
        }

        onYChanged: {
            if (ball.refreshingState !== true)
                leftPaddle.y = ball.y;

        }

        onGoalScoredChanged: {
            if (goalScored !== -1) {
                refreshingState = true;
                if (goalScored === 1)
                    root.goalScored(player1Score);
                else if (goalScored === 2)
                    root.goalScored(player2Score);
            }
        }
    }

    Timer {
        id: gameReset;
        running: false;
        interval: 1000;
        onTriggered: {
            ball.x = root.width / 2;
            ball.y = root.height / 2;
            ball.flipCoin();
            ball.movingLeft = ball.coinFlip;
            ball.movingUp = ball.coinFlip;
            ball.refreshingState = false;
            ball.visible = true;
            //ball.opacity = 1.0;
        }
    }
    /*Connections {
        target: ball;
        onGoalScored: {
            if (goalScored !== -1) {
                if (goalScored === 1)
                    root.goalScored(player1Score);
                else if (goalScored === 2)
                    root.goalScored(player2Score);
            }
        }
    }*/

    /*Timer {
        id: ballTimer;
        interval: 13;
        running: true;
        repeat: true;
        onTriggered: {
            if (ball.x <= 0) {

                root.goalScored(player2Score)

            }

            else if (ball.x >= root.width) {
                root.goalScored(player1Score);
                //ball.movingLeft = true;
            }
        }
    }*/

}
