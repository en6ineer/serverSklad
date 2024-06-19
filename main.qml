import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Window 2.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: qsTr("Server 1Sklad")

    property string postData: ""

    Column {
        anchors.centerIn: parent
        spacing: 20

        Text {
            id: urlText
            text: httpServer.serverUrl
            font.pointSize: 20
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // Заголовок таблицы
        Row {
            spacing: 1

            Rectangle {
                width: 260
                height: 40
                color: "lightgray"
                border.color: "black"
                Text {
                    anchors.centerIn: parent
                    text: "Штрихкод"
                    font.bold: true
                }
            }
            Rectangle {
                width: 130
                height: 40
                color: "lightgray"
                border.color: "black"
                Text {
                    anchors.centerIn: parent
                    text: "Количество"
                    font.bold: true
                }
            }
            Rectangle {
                width: 260
                height: 40
                color: "lightgray"
                border.color: "black"
                Text {
                    anchors.centerIn: parent
                    text: "Комментарий"
                    font.bold: true
                }
            }
        }


        TableView {
            id: tableView
            width: 650
            height: 400
            model: barcodesData
            rowSpacing: 10
            columnSpacing: 20

            // Указываем ширину колонок
            columnWidthProvider: function(column) {
                return column === 0 ? 260 : (column === 1 ? 130 : 260);
            }

            delegate: Rectangle {
                implicitHeight: 50
                implicitWidth: tableView.columnWidth(column)  // Устанавливаем ширину делегата в зависимости от колонки
                border.color: "black"
                border.width: 1

                // Внутренний текстовый элемент, который будет отображать данные модели
                Text {
                    anchors.centerIn: parent
                    wrapMode: Text.Wrap
                    text: {
                        if (column === 0) {
                            return model.barcode
                        } else if (column === 1) {
                            return model.quantity
                        } else if (column === 2) {
                            return model.comment
                        }
                    }
                }

                MouseArea {
                               anchors.fill: parent
                               onDoubleClicked: {
                                   if (column === 0) {
                                       clipboard.text = model.barcode
                                   } else if (column === 1) {
                                       clipboard.text = model.quantity
                                   } else if (column === 2) {
                                       clipboard.text = model.comment
                                   }

                               }
                       }
            }

        }




        // Таблица данных
       // TableView {
       //     width: 650
       //     height: 400
       //     model: barcodesData

       //     delegate: Row {
       //         spacing: 1

       //         Rectangle {
       //             width: 260
       //             height: 50
       //             border.color: "black"
       //             Text {
       //                 anchors.centerIn: parent
       //                 text: model.barcode
       //             }
       //             MouseArea {
       //                 anchors.fill: parent
       //                 onDoubleClicked: {
       //                     clipboard.text = model.barcode
       //                 }
       //             }
       //         }
       //         Rectangle {
       //             width: 130
       //             height: 50
       //             border.color: "black"
       //             Text {
       //                 anchors.centerIn: parent
       //                 text: model.quantity
       //             }
       //             MouseArea {
       //                 anchors.fill: parent
       //                 onDoubleClicked: {
       //                     clipboard.text = model.quantity
       //                 }
       //             }
       //         }
       //         Rectangle {
       //             width: 260
       //             height: 50
       //             border.color: "black"
       //             Text {
       //                 anchors.centerIn: parent
       //                 text: model.comment
       //             }
       //             MouseArea {
       //                 anchors.fill: parent
       //                 onDoubleClicked: {
       //                     clipboard.text = model.comment
       //                 }
       //             }
       //         }
       //     }
       // }

        Button {
            text: "Добавить штрихкод"
            onClicked: {
                barcodesData.addRow("122321312", 2, "abababa")
            }
        }

        TextArea {
            id: requestOutput
            width: parent.width * 0.9
            height: parent.height * 0.2
            readOnly: true // Разрешаем только чтение, чтобы позволить прокрутку
            placeholderText: "Received POST requests will appear here"
            text: postData
        }
    }

    Connections {
        target: httpServer
        function onNewPostRequest(data) {
            // Разбираем JSON строку и добавляем данные в модель
            var jsonObject = JSON.parse(data);
            barcodesData.addRow(jsonObject.barcode, jsonObject.quantity, jsonObject.comment);
            postData += data + "\n";
        }
    }
}
