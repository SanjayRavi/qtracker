import QtQuick 1.0

Item {
    id: root
    property alias title: header.text
    property QtObject items: null
    height: header.offset + box.height + 10

    Text {
        id: header
        x: 20
        y: 20
        color: "#505050"
        style: Text.Sunken
        styleColor: "black"
        text: ""
        property int offset: (text=="")? 10 : 40
    }

    Rectangle {
        id: box

        x: 10
        y: 10 + header.offset
        width: root.width -20
        height: content.height
        radius: 12
        color: "white"
        border.color: "grey"
        border.width: 1

        Item {
            id: content
        }

        Item {
            id: seperators

            function clear() {
                for (var i=0; i<seperators.children.length; ++i) {
                    seperators.children[i].destroy();
                }
            }
        }

        function layout() {
            items.parent = content
            if (items) {
                content.height = layoutItems(box.width);
            }
        }

        function layoutItems(w) {
            //console.log("got", items.children.length, "items")
            var h = 2;
            seperators.clear();
            var seperator;
            for (var i=0; i<items.children.length; ++i) {
                h = layoutOptionItem(items.children[i],w,h+10) + 10;
                if ((i+1) < items.children.length) {
                    seperator = line.createObject(seperators)
                    h = h+1
                    seperator.x = 0
                    seperator.width = w
                    seperator.y = h
                }
            }
            return h+2;
        }

        function layoutOptionItem(item,w,h) {
            //item.parent = content;
            item.x = 10;
            item.y = h;
            item.width = w-20;
            h = h + item.height;
            //console.log("item:",item.x,item.y,item.width,item.height);
            return h;
        }

        onWidthChanged:         layout();
        Component.onCompleted:  layout();
    }

    Component {
        id: line

        Rectangle {
            height: 1
            color: "grey"
        }
    }

    function layout() {
        box.layout();
    }

    onItemsChanged:         box.layout();
    onHeightChanged:        box.layout();
}