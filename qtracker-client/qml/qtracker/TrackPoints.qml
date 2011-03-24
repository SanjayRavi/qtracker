import QtQuick 1.0

Database {
    id: root
    table: "trackpoints"
    property int trackid: -1
    property var range: []

    function updateFilter() {
        var filter = "";
        filter += "trackid="+root.trackid.toString();
        if (range.length>0) {
            filter += " AND " + "longitude>=" + root.range[0].toString();
            filter += " AND " + "longitude<=" + root.range[2].toString();
            filter += " AND " + "latitude>="  + root.range[1].toString();
            filter += " AND " + "latitude<="  + root.range[3].toString();
        }
        root.filter=filter;
    }

    function selectRange(left, top, right, bottom) {
        root.range = [ left, top, right, bottom ];
        updateFilter();
    }

    onTrackidChanged: updatefilter()
    onRangeChanged: updatefilter()
    Component.onCompleted: updatefilter()
}
