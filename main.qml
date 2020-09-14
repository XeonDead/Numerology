import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.4 as OldControls
import QtQuick.Controls 2.12
import QtQuick.Controls.Universal 2.12
import QtCharts 2.3
import Qt.labs.calendar 1.0
import CulminationModel 0.1
import DayNumberModel 0.1

Item {
    id: main
    width: 1200
    height: 600
    visible: true
    property date now: new Date()

    RowLayout {
        id: dateRowLayout
        x: 8
        y: 8

        TextField {
            id: textDate
            width: 100
            height: 33
            text: Qt.formatDate(calendar.selectedDate, "dd.MM.yyyy")
            font.bold: true
            font.family: "MS Shell Dlg 2"
            font.pointSize: 12

            function validate_date(value) {
                var arrD = value.split(".")
                arrD[1] -= 1
                var d = new Date(arrD[2], arrD[1], arrD[0])
                if ((d.getFullYear() === arrD[2]) && (d.getMonth() === arrD[1])
                        && (d.getDate() === arrD[0])) {
                    return true
                } else {
                    return false
                }
            }

            Keys.onReleased: {
                validate_date(text)
            }

            validator: RegExpValidator {
                regExp: /^([0-2]?[1-9]|3[0-9]).(0?[1-9]|1[0-9]).([0-9][0-9][0-9][0-9])$ /
            } // /^([0-2]?[1-9]|3[0-1]).(0?[1-9]|1[0-2]).([0-9][0-9][0-9][0-9])$ /
            inputMask: "99.99.9999"
            inputMethodHints: Qt.ImhDigitsOnly
            Keys.onUpPressed: calendar.__selectNextDay()
            Keys.onDownPressed: calendar.__selectPreviousDay()
        }

        Popup {
            id: calendarPopup
            OldControls.Calendar {
                id: calendar
                visible: true
                dayOfWeekFormat: Locale.ShortFormat
                onClicked: {
                    calendarPopup.close()
                }

                function open() {
                    function get_date(value) {
                        var arrD = value.split(".")
                        arrD[1] -= 1
                        var d = new Date(arrD[2], arrD[1], arrD[0])
                        if ((d.getFullYear() === arrD[2]) && (d.getMonth(
                                                                  ) === arrD[1])
                                && (d.getDate() === arrD[0])) {
                            return d
                        } else {

                            return false
                        }
                    }

                    calendar.selectedDate = new Date(get_date(textDate.text))
                    visible = true
                }

                function close() {
                    visible = false
                }
            }
        }

        Button {
            id: btnOpen
            width: 80
            height: textDate.height
            text: qsTr("Выбрать дату")

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    calendarPopup.visible ? calendarPopup.close(
                                                ) : calendarPopup.open()
                }
            }
        }

        Button {
            id: btnCalc
            x: 198
            y: 8
            width: 80
            height: textDate.height
            text: qsTr("Рассчитать")
            onClicked: {
                BackendFunctionEnergy.setSeries(dateEnergyView.series(0))
                BackendFunctionEnergy.setEnergyData(textDate.text)
                axisX.min = BackendFunctionEnergy.getFirstYear()
                axisX.max = BackendFunctionEnergy.getLastYear()
                if (tabBar.currentIndex == 0) {
                    var culModel = culminationTableView.model
                    var q_model_index = culModel.index(0, 0)
                    culModel.setData(q_model_index, textDate.text, 1)
                } else if (tabBar.currentIndex == 1) {
                    var dayModel = dayNumberTableView.model
                    var q_bday_index = dayModel.index(1, 0)
                    var q_range_index = dayModel.index(2, 0)
                    dayModel.setData(q_bday_index, textDate.text, 1)
                    dayModel.setData(
                                q_range_index,
                                dayNumberFirst.text + ";" + dayNumberLast.text,
                                1)
                }
            }
        }
    }

    TabBar {
        id: tabBar
        x: 0
        y: 47
        width: parent.width

        TabButton {
            text: qsTr("Кульминационные циклы")
        }
        TabButton {
            text: qsTr("Расчёт числа дня")
        }
    }

    StackLayout {
        id: stackLayout
        x: 0
        y: 87
        width: parent.width
        height: 513
        currentIndex: tabBar.currentIndex

        Item {
            id: culminationCycleTab
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            RowLayout {
                id: columnLayout
                x: 0
                y: 0

                ChartView {
                    id: dateEnergyView
                    x: 0
                    y: 0
                    width: 400
                    height: 300
                    antialiasing: true
                    legend.visible: false
                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                    animationOptions: ChartView.NoAnimation
                    theme: ChartView.ChartThemeQt
                    property bool openGL: true
                    property bool openGLSupported: true

                    ValueAxis {
                        id: axisY
                        min: 0
                        max: 9
                        gridVisible: true
                        labelFormat: "%.0f"
                        tickCount: 10
                    }

                    ValueAxis {
                        id: axisX
                        min: 1900
                        max: 2100
                        gridVisible: true
                        labelFormat: "%.0f"
                        tickCount: 7
                    }

                    LineSeries {
                        id: lineSeries
                        axisX: axisX
                        axisY: axisY
                        useOpenGL: true
                        capStyle: "RoundCap"
                        style: "SolidLine"
                        color: "steelblue"
                    }
                }

                TableView {
                    id: culminationTableView
                    x: 0
                    y: 310
                    width: 600
                    height: 500
                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                    leftMargin: rowsHeader.implicitWidth
                    topMargin: columnsHeader.implicitHeight
                    clip: true

                    property var columnWidths: [90, 60, 110, 50, 160, 50]
                    columnWidthProvider: function (column) {
                        return columnWidths[column]
                    }

                    model: CulminationModel {}

                    delegate: Rectangle {
                        implicitHeight: 50
                        Text {
                            text: display
                            anchors.fill: parent
                            anchors.margins: 10
                            color: 'black'
                            font.pixelSize: 12
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    Rectangle {
                        // mask the headers
                        z: 3
                        color: "#222222"
                        y: culminationTableView.contentY
                        x: culminationTableView.contentX
                        width: culminationTableView.leftMargin
                        height: culminationTableView.topMargin
                    }

                    Row {
                        id: columnsHeader
                        y: culminationTableView.contentY
                        z: 2
                        Repeater {
                            model: culminationTableView.columns
                                   > 0 ? culminationTableView.columns : 1
                            Label {
                                width: culminationTableView.columnWidthProvider(
                                           modelData)
                                height: 35
                                text: culminationTableView.model.headerData(
                                          modelData, Qt.Horizontal)
                                color: '#aaaaaa'
                                font.pixelSize: 13
                                padding: 10
                                verticalAlignment: Text.AlignVCenter

                                background: Rectangle {
                                    color: "#333333"
                                }
                            }
                        }
                    }
                    Column {
                        id: rowsHeader
                        x: culminationTableView.contentX
                        z: 2
                        Repeater {
                            model: culminationTableView.rows > 0 ? culminationTableView.rows : 1
                            Label {
                                width: 60
                                height: 50
                                text: culminationTableView.model.headerData(
                                          modelData, Qt.Vertical)
                                color: '#aaaaaa'
                                font.pixelSize: 13
                                padding: 10
                                verticalAlignment: Text.AlignVCenter

                                background: Rectangle {
                                    color: "#333333"
                                }
                            }
                        }
                    }
                }
            }
        }
        Item {
            id: dayNumberTab

            ColumnLayout {
                id: dayNumberColumnLayout
                visible: true
                anchors.fill: parent

                RowLayout {
                    id: dayNumberRowLayout
                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                    TextField {
                        id: dayNumberFirst
                        placeholderText: qsTr("Дата начала периода")
                        text: Qt.formatDate(
                                  firstDayNumberCalendar.selectedDate,
                                  "dd.MM.yyyy")
                        font.bold: true
                        font.family: "MS Shell Dlg 2"
                        font.pointSize: 12

                        function validate_date(value) {
                            var arrD = value.split(".")
                            arrD[1] -= 1
                            var d = new Date(arrD[2], arrD[1], arrD[0])
                            if ((d.getFullYear() === arrD[2])
                                    && (d.getMonth() === arrD[1])
                                    && (d.getDate() === arrD[0])) {
                                return true
                            } else {
                                return false
                            }
                        }

                        Keys.onReleased: {
                            validate_date(text)
                        }

                        validator: RegExpValidator {
                            regExp: /^([0-2]?[1-9]|3[0-9]).(0?[1-9]|1[0-9]).([0-9][0-9][0-9][0-9])$ /
                        } // /^([0-2]?[1-9]|3[0-1]).(0?[1-9]|1[0-2]).([0-9][0-9][0-9][0-9])$ /
                        inputMask: "99.99.9999"
                        inputMethodHints: Qt.ImhDigitsOnly
                        Keys.onUpPressed: firstDayNumberCalendar.__selectNextDay()
                        Keys.onDownPressed: firstDayNumberCalendar.__selectPreviousDay()
                    }

                    Button {
                        id: btnFirstDayOpen
                        width: 60
                        height: dayNumberFirst.height
                        text: qsTr("Выбрать")

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                firstDayNumberCalendarPopup.visible ? firstDayNumberCalendarPopup.close() : firstDayNumberCalendarPopup.open()
                            }
                        }
                    }

                    Popup {
                        id: firstDayNumberCalendarPopup
                        OldControls.Calendar {
                            id: firstDayNumberCalendar
                            visible: true
                            dayOfWeekFormat: Locale.ShortFormat
                            onClicked: {
                                firstDayNumberCalendarPopup.close()
                            }
                            selectedDate: new Date(now.getFullYear(),
                                                   now.getMonth(), 1)
                            function open() {
                                function get_date(value) {
                                    var arrD = value.split(".")
                                    arrD[1] -= 1
                                    var d = new Date(arrD[2], arrD[1], arrD[0])
                                    if ((d.getFullYear() === arrD[2])
                                            && (d.getMonth() === arrD[1])
                                            && (d.getDate() === arrD[0])) {
                                        return d
                                    } else {

                                        return false
                                    }
                                }
                                firstDayNumberCalendar.selectedDate
                                        = new Date(get_date(
                                                       dayNumberFirst.text))
                                visible = true
                            }

                            function close() {
                                visible = false
                            }
                        }
                    }

                    TextField {
                        id: dayNumberLast
                        placeholderText: qsTr("Дата конца периода")
                        text: Qt.formatDate(lastDayNumberCalendar.selectedDate,
                                            "dd.MM.yyyy")
                        font.bold: true
                        font.family: "MS Shell Dlg 2"
                        font.pointSize: 12

                        function validate_date(value) {
                            var arrD = value.split(".")
                            arrD[1] -= 1
                            var d = new Date(arrD[2], arrD[1], arrD[0])
                            if ((d.getFullYear() === arrD[2])
                                    && (d.getMonth() === arrD[1])
                                    && (d.getDate() === arrD[0])) {
                                return true
                            } else {
                                return false
                            }
                        }

                        Keys.onReleased: {
                            validate_date(text)
                        }

                        validator: RegExpValidator {
                            regExp: /^([0-2]?[1-9]|3[0-9]).(0?[1-9]|1[0-9]).([0-9][0-9][0-9][0-9])$ /
                        } // /^([0-2]?[1-9]|3[0-1]).(0?[1-9]|1[0-2]).([0-9][0-9][0-9][0-9])$ /
                        inputMask: "99.99.9999"
                        inputMethodHints: Qt.ImhDigitsOnly
                        Keys.onUpPressed: lastDayNumberCalendar.__selectNextDay(
                                              )
                        Keys.onDownPressed: lastDayNumberCalendar.__selectPreviousDay()
                    }

                    Button {
                        id: btnLastDayOpen
                        width: 60
                        height: dayNumberLast.height
                        text: qsTr("Выбрать")

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                lastDayNumberCalendarPopup.visible ? lastDayNumberCalendarPopup.close() : lastDayNumberCalendarPopup.open()
                            }
                        }
                    }

                    Popup {
                        id: lastDayNumberCalendarPopup
                        OldControls.Calendar {
                            id: lastDayNumberCalendar
                            visible: true
                            dayOfWeekFormat: Locale.ShortFormat
                            onClicked: {
                                lastDayNumberCalendarPopup.close()
                            }
                            selectedDate: new Date(now.getFullYear(),
                                                   now.getMonth() + 1, 1)

                            function open() {
                                function get_date(value) {
                                    var arrD = value.split(".")
                                    arrD[1] -= 1
                                    var d = new Date(arrD[2], arrD[1], arrD[0])
                                    if ((d.getFullYear() === arrD[2])
                                            && (d.getMonth() === arrD[1])
                                            && (d.getDate() === arrD[0])) {
                                        return d
                                    } else {
                                        return false
                                    }
                                }
                                lastDayNumberCalendar.selectedDate
                                        = new Date(get_date(dayNumberLast.text))
                                visible = true
                            }

                            function close() {
                                visible = false
                            }
                        }
                    }
                }

                TableView {
                    id: dayNumberTableView
                    x: 0
                    width: 200
                    height: 450
                    clip: true
                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                    ScrollBar.vertical: ScrollBar {}

                    property var columnWidths: [100, 100]
                    columnWidthProvider: function (column) {
                        return columnWidths[column]
                    }
                    model: DayNumberModel {}

                    delegate: Rectangle {
                        implicitHeight: 50
                        Text {
                            text: display
                            anchors.fill: parent
                            anchors.margins: 10
                            color: 'black'
                            font.pixelSize: 12
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/

