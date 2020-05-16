import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import Board 1.0

ApplicationWindow {
    visible: true

    height: 564
    maximumHeight: 564
    minimumHeight: 564
    width: 524
    maximumWidth: 524
    minimumWidth: 524

    color: "dark grey"
    title: qsTr("Conways's Game Of Life")

    TableView {
        id: boardView
        anchors.fill: parent

        rowSpacing: 1
        columnSpacing: 1

        delegate: Rectangle {
            id: cell
            implicitHeight: 20
            implicitWidth: 20

            color: model.display ? "black" : "light grey"

            MouseArea {
                anchors.fill:parent

                onClicked: model.display = !model.display
            }
        }

        model: Board {
            id:boardModel

            onCleared: timer.running = false
        }
    }

    footer: Rectangle {
        id: footer
        height: 40
        color: "dark grey"

        RowLayout {
            anchors.centerIn: parent

            Button {
                id: randomizeButton
                text: "Randomize Board"
                onClicked: boardModel.randomize_buffer()
            }

            Button {
                id: timerButton
                text: timer.running ? "❙❙" : "▶️"
                onClicked: timer.running = !timer.running
            }

            Button {
                id: clearButton
                text: "Clear"
                onClicked: boardModel.clear()
            }
        }
    }

    Timer {
        id: timer
        running: false
        repeat: true
        interval: 100
        onTriggered: boardModel.tick()
    }

}
