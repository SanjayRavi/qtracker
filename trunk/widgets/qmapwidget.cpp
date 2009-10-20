#include <QPainter>
#include <QFlags>
#include <cmath>
#include "ui.h"
#include "qmapwidget.h"
#include "qmapselectiondialog.h"

#include <iostream>
//#define LOG( a ) std::cout << a
#define LOG( a )


#ifdef Q_OS_SYMBIAN
#define MAPDIR "c:/data/qtracker/maps/"
#else
#define MAPDIR "/Users/hurenkam/workspace/qtracker/maps/"
#endif

QMapWidget::QMapWidget(QWidget *parent)
    : QGaugeWidget(parent)
    , state(StNoMap)
    , zoom(0.5)
    , x(0)
    , y(0)
    , latitude(0)
    , longitude(0)
    , meta(0)
    , mapimage(0)
    , bgimage(new QImage(QString(UIDIR "map.svg")))
{
    connect(this, SIGNAL(drag(int,int)), this, SLOT(moveMap(int,int)));
    connect(this, SIGNAL(singleTap()), this, SLOT(FollowGPS()));
    connect(this, SIGNAL(doubleTap()), this, SLOT(SelectMap()));

    CreateMapList();
}

QMapWidget::~QMapWidget()
{
/*
    delete mapimage;
    mapimage = 0;
    meta = 0;
    QStringList names = maplist.keys();
    for (int i; i<maplist.size(); ++i)
        delete maplist[names[i]];
*/
}

void QMapWidget::CreateMapList()
{
    QDir directory = QDir(MAPDIR);

    QStringList files = directory.entryList(QStringList(QString("*.xml")),
                                 QDir::Files | QDir::NoSymLinks);

    for (int i = 0; i < files.size(); ++i)
    {
        LOG( "QMapWidget::CreateMapList() " << files[i].toStdString() << "\n"; )
        maplist[files[i]] = new QMapMetaData(QString(MAPDIR) + files[i]);
    }
}

bool QMapWidget::SelectBestMapForCurrentPosition()
{
    LOG( "QMapWidget::SelectBestMapForCurrentPosition()\n"; )
    QStringList found;
    FindMapsForCurrentPosition(found);
    if (found.size() > 0)
    {
        LOG( "QMapWidget::SelectBestMapForCurrentPosition(): " << found[0].toStdString() << "\n"; )
        // for now select first entry
        int index = 0;
        int lon2x = maplist[found[0]]->Lon2x();
        for (int i=1; i<found.size(); ++i)
            if (maplist[found[i]]->Lon2x() > lon2x)
            {
                lon2x = maplist[found[i]]->Lon2x();
                index = i;
            }
        return LoadMap(found[index]);
    }
    return false;
}

void QMapWidget::FindMapsForCurrentPosition(QStringList &found)
{
    LOG( "QMapWidget::FindMapsForCurrentPosition()\n"; )
    QStringList keys = maplist.keys();
    for (int i=0; i<keys.size(); ++i)
    {
        if (maplist[keys[i]]->IsPositionOnMap(latitude,longitude))
        {
            LOG( "QMapWidget::FindMapsForCurrentPosition(): " << keys[i].toStdString() << "\n"; )
            found.append(keys[i]);
        }
    }
}

bool QMapWidget::LoadMap(QString filename)
{
    LOG( "QMapWidget::LoadMap(): " << filename.toStdString() << "\n"; )
    meta = maplist[filename];
    filename.remove(filename.size()-4,4);
    filename.append(".jpg");
    if (mapimage)
        delete mapimage;
    mapimage = new QImage();
    bool result = mapimage->load(QString(MAPDIR) + filename);
    if (!result)
    {
            QMessageBox msg;
            msg.setText(QString("Unable to load map ") + filename);
            msg.setIcon(QMessageBox::Warning);
            msg.exec();
    }
    else
    {
            meta->SetSize(mapimage->width(),mapimage->height());
            meta->Calibrate();
            x = mapimage->width()/2;
            y = mapimage->height()/2;
    }
    update();
    return result;
}

void QMapWidget::MapSelected(QString map)
{
    if (LoadMap(map))
        state = StScrolling;
    else
        state = StNoMap;
}

void QMapWidget::SelectMap()
{
    QMapSelectionDialog *dialog = new QMapSelectionDialog(maplist);
    connect(dialog,SIGNAL(selectmap(QString)),this,SLOT(MapSelected(QString)));
    dialog->setModal(true);
#ifdef Q_OS_SYMBIAN
    dialog->showFullScreen();
#else
    dialog->show();
#endif
}

void QMapWidget::updatePosition(double lat, double lon)
{
    //LOG( "QMapWidget::updatePosition()\n"; )
    latitude = lat;
    longitude = lon;
    if (state == StFollowGPS)
        FollowGPS();
    else
        update();
}

void QMapWidget::moveMap(int dx, int dy)
{
    x -= dx;
    y -= dy;
    if (state == StFollowGPS)
        state = StScrolling;
    update();
}

void QMapWidget::FollowGPS()
{
    LOG( "QMapWidget::FollowGPS()\n"; )

    if (IsPositionOnMap())
    {   // OnMap
        SetCursorToCurrentPosition();
        state = StFollowGPS;
    }
    else
    {   // OffMap
        QStringList foundmaps;
        FindMapsForCurrentPosition(foundmaps);
        if (foundmaps.size() > 0)
        {
            // MapAvailable
            if (SelectBestMapForCurrentPosition())
                // Load Succeeded
                state = StFollowGPS;
            else
                // Load Failed
                state = StNoMap;
        }
        else
        {
            // NoMapAvailable
            if (mapimage)
                // KeepPreviousMap
                state = StFollowGPS;
            else
                // NoPreviousMap
                state = StNoMap;
        }
    }
    update();
}


void QMapWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    double w = width();
    double h = height();
    double s = h / 36;

    QPainter painter(this);
    QRectF source(0, 0, 400, 360);
    QRectF target(-w/2, -h/2, w, h);
    painter.setWindow(-w/2,-h/2,w,h);

    painter.drawImage(target, *bgimage, source);
    painter.setViewport(20,20,w-40,h-40);
    if (mapimage)
    {
        source = QRectF(w*zoom/-2 + x, h*zoom/-2 + y, w*zoom, h*zoom);
        target = QRectF(w*zoom/-2, h*zoom/-2, w*zoom, h*zoom);
        painter.setWindow(-w/2*zoom,-h/2*zoom,w*zoom,h*zoom);
        painter.drawImage(target, *mapimage, source);
        painter.setWindow(-w/2,-h/2,w,h);
    }

    if ((state == StScrolling) || (!IsPositionOnMap()))
    {
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
    }
    else
    {
        painter.setPen(Qt::green);
        painter.setBrush(Qt::green);
    }
    painter.drawEllipse(s/-2,s/-2,s,s);
}
