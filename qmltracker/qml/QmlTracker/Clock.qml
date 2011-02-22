import QtQuick 1.0
import QmlTrackerExtensions 1.0

Item {
    id: root
    property int hours: 2
    property int minutes: 50
    property int seconds: 20
    property int viewid: -1
    x:      parent.gaugeX(viewid)
    y:      parent.gaugeY(viewid)
    width:  parent.gaugeW(viewid)
    height: parent.gaugeH(viewid)
    Behavior on x      { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }
    Behavior on y      { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }
    Behavior on width  { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }
    Behavior on height { NumberAnimation { easing.type: Easing.InOutQuart; duration: 800 } }

    signal clicked()

    ClockModel {
        id: clockmodel
        onCurrentChanged: timeChanged()
    }

    MonitorModel {
        id:monitormodel
    }

    MouseHandler {
        id: mouseHandler
        anchors.fill: parent
        onSingleTap: root.clicked()
        onLongTap: clockmodel.reset()
    }

    function timeChanged() {
        setAnalogTime(clockmodel.current)
        setTopTime(clockmodel.trip)
        setBottomTime(monitormodel.time)
    }

    function setAnalogTime(date) {
        root.hours = date.getHours()
        root.minutes = date.getMinutes()
        root.seconds = date.getSeconds()
    }

    function setTopTime(date) {
        var h = date.getHours()
        var m = date.getMinutes()
        var s = date.getSeconds()
        top.text = h.toString() + ":" + m.toString() + "." + s.toString()
    }

    function setBottomTime(date) {
        var h = date.getHours()
        var m = date.getMinutes()
        var s = date.getSeconds()
        bottom.text = h.toString() + ":" + m.toString() + "." + s.toString()
    }

    Image {
        source: "/images/clock.svg";
        anchors.fill: parent
    }

    Rectangle {
        y: parent.height * 0.7
        height: parent.height * 0.18
        color: activePalette.dark
        width: parent.width/3.5
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id: top
            anchors.horizontalCenter: parent.horizontalCenter
            text: hours.toString() + ":" + minutes.toString() + "." + seconds.toString()
            color: "white"
            font.bold: true; font.pixelSize: parent.height/3
            style: Text.Raised; styleColor: "black"
        }
        Text {
            y: parent.height/2
            id: bottom
            anchors.horizontalCenter: parent.horizontalCenter
            text: hours.toString() + ":" + minutes.toString() + "." + seconds.toString()
            color: "white"
            font.bold: true; font.pixelSize: parent.height/3
            style: Text.Raised; styleColor: "black"
        }
    }

    Image {
        source: "/images/shorthand.svg"
        anchors.fill: parent
        transform: Rotation {
            id: shorthand
            origin.x: width/2
            origin.y: height/2
            angle: root.hours*360/12 + root.minutes/2
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                    modulus: 360
                }
            }
        }
    }
    Image {
        source: "/images/longhand.svg"
        anchors.fill: parent
        transform: Rotation {
            id: longhand
            origin.x: width/2
            origin.y: height/2
            angle: root.minutes*360/60
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                    modulus: 360
                }
            }
        }
    }
    Image {
        source: "/images/secondhand.svg"
        anchors.fill: parent
        transform: Rotation {
            id: secondhand
            origin.x: width/2
            origin.y: height/2
            angle: root.seconds*360/60
            Behavior on angle {
                SpringAnimation {
                    spring: 1.4
                    damping: .15
                    modulus: 360
                }
            }
        }
    }
}
