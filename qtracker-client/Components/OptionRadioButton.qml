import QtQuick 1.0

OptionTextItem {
    id: root
    property bool ticked: false
    button: true
    buttonsource: ticked? "../Images/ticked.svg" : "../Images/unticked.svg"
}
