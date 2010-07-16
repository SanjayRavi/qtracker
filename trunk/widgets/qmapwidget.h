#ifndef QMAPWIDGET_H
#define QMAPWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGeoPositionInfo>
#include <QGeoCoordinate>
#include "qgaugewidget.h"
#include "geodata.h"

using namespace QtMobility;

class ScreenPos
{
public:
	int x;
	int y;
};

class QMapWidget : public QGaugeWidget
{
    Q_OBJECT

public:
    QMapWidget(QWidget *parent = 0);
    ~QMapWidget();
    bool IsPositionOnMap() { return (meta && meta->IsPositionOnMap(latitude,longitude)); }
    bool IsPositionOnScreen(WayPoint& wpt);
    ScreenPos PositionOnScreen(WayPoint& wpt);

signals:
    void zoomin();
    void zoomout();
    void waypoint();
    void track();
    void options();
    void datum();

public slots:
    //void updatePosition(double lat, double lon, double ele=0.0);
    void updatePosition(const QGeoCoordinate& pos);
    void moveMap(int x, int y);
    void SelectMap();
    void SelectPoint();
    void SelectMapForCurrentPosition();
    void MapSelected(QString map);
    void WaypointSelected(QString name, double lat, double lon);
    void RefpointSelected(QString name, double lat, double lon);
    void FollowGPS();
    
    void StartTrack();
    void TrackStarted(QString n, int t, int d);
    void TrackUpdated(QString n, int t, int d);
    void TrackStopped(QString name);
    void ShowTrack(Track* t);
    void HideTracks();
    void ShowTrackPoint(WayPoint& w);

private slots:
    void zoomRepeat();
    void zoomIn();
    void zoomOut();

protected:
    void FindMapsForCurrentPosition(QStringList& found);
    bool LoadMap(QString filename);
    bool SelectBestMapForCurrentPosition();
    bool SetCursorToCurrentPosition()
    {
        if (meta)
            return meta->Wgs2XY(latitude,longitude,x,y);
        else
        	return false;
    }
    
    virtual void paintTrack(Track* t);
    virtual void paintBackground(QPainter& painter);
    virtual void paintMap(QPainter& painter);
    virtual void paintWaypoints(QPainter& painter);
    virtual void paintWidgets(QPainter& painter);
    virtual void paintDot(QPainter& painter,int x,int y,QColor c);
    virtual void paintBar(QPainter& painter);
    virtual void paintEvent(QPaintEvent *event);
    void CreateMapList();

private:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    static const int StNoMap = 0;
    static const int StScrolling = 1;
    static const int StFollowGPS = 2;
    int state;
    int zoom;

    double x, y;
    double latitude;
    double longitude;
    double altitude;
    WayPoint *prevpos;
    int updatetime;
    int updatedistance;
    QDateTime prevtime;
    QImage* mapimage;
    QImage* bgimage;
    QImage* svgZoomIn;
    QImage* svgZoomOut;
    QImage* svgOptions;
    QImage* svgFlag;
    QImage* svgHiker;
    QImage* svgBar;
    MapMetaData *meta;
    //MapList maplist;
    int zooming;
    QTimer zoomtimer;
    QString mapname;
    QList<Track*> tracks;
    Track* recordtrack;
};

#endif // QMAPWIDGET_H
