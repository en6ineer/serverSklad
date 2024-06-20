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

        // Заголовок таблицы и сама таблица помещены в Rectangle
        Rectangle {
            width: 750
            height: 500
            border.color: "black"
            border.width: 1
            radius: 10
            color: "white"

            Column {
                anchors.centerIn: parent
                spacing: 10

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

                // Таблица данных
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

                        // MouseArea для копирования содержимого ячейки в буфер обмена при клике
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (column === 0) {
                                    clipboard.text = model.barcode;
                                } else if (column === 1) {
                                    clipboard.text = model.quantity;
                                } else if (column === 2) {
                                    clipboard.text = model.comment;
                                }
                            }
                        }
                    }

                }
            }
        }


        // Новая строка с кнопками
                Row {
                    spacing: 20
                    Button {
                        text: "Скопировать штрихкоды"
                        onClicked: {
                            clipboard.text = barcodesData.getColumnData(0)
                        }
                    }
                    Button {
                        text: "Скопировать количество"
                        onClicked: {
                            clipboard.text = barcodesData.getColumnData(1)
                        }
                    }
                    Button {
                        text: "Скопировать комментарии"
                        onClicked: {
                            clipboard.text = barcodesData.getColumnData(2)
                        }
                    }
                }



        Button {
                    text: "Очистить таблицу"
                    onClicked: {
                        barcodesData.clear()
                    }
                }

        // TextArea {
        //     id: requestOutput
        //     width: parent.width * 0.9
        //     height: parent.height * 0.2
        //     readOnly: true // Разрешаем только чтение, чтобы позволить прокрутку
        //     placeholderText: "Received POST requests will appear here"
        //     text: postData
        // }
    }

    Connections {
           target: httpServer
           function onNewPostRequest(data) {
              // barcodesData.addRow("**Данные получены в ", getCurrentDateTime(), "**");
               // Разбираем JSON строку и добавляем данные в модель
               var jsonObject = JSON.parse(data);
               barcodesData.addRow(jsonObject.barcode, jsonObject.quantity, jsonObject.comment);
               // postData += data + "\n";
               //barcodesData.addRow("*****", 0, "*****");
           }
       }

    // function getCurrentDateTime() {
    //        var now = new Date();
    //        var day = now.getDate();
    //        var month = now.getMonth() + 1; // Месяцы начинаются с 0
    //        var hours = now.getHours();
    //        var minutes = now.getMinutes();
    //        var seconds = now.getSeconds();

    //        // Добавляем ведущие нули для однозначных чисел
    //        day = day < 10 ? '0' + day : day;
    //        month = month < 10 ? '0' + month : month;
    //        hours = hours < 10 ? '0' + hours : hours;
    //        minutes = minutes < 10 ? '0' + minutes : minutes;
    //        seconds = seconds < 10 ? '0' + seconds : seconds;

    //        return day +"."+ month + " " + hours + ":" + minutes + ":" + seconds;
    //    }
}
