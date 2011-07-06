#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include "database.h"
#include "qmldatabase.h"

#define ENABLE_DEBUG
#include "helpers.h"

qmlTrack::qmlTrack()
    : _name("trk")
    , _trkid(-1)
{
    ENTER("")
}

qmlTrack::qmlTrack(int id)
    : _name("trk")
    , _trkid(-1)
{
    ENTER("")
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT * FROM tracks WHERE trk='" + QString::number(id) + "'",db);
    if (q.next())
        load(q);

    EXIT("")
}

qmlTrack::qmlTrack(const QSqlQuery& q)
    : _name("trk")
    , _trkid(-1)
{
    load(q);
}

void
qmlTrack::load(const QSqlQuery& q)
{
    ENTER("")
    _trkid     = getIntField(q,"trk");
    _name      = getStringField(q,"name");
    _top       = getDoubleField(q,"north");
    _left      = getDoubleField(q,"west");
    _bottom    = getDoubleField(q,"south");
    _right     = getDoubleField(q,"east");
    _interval  = getIntField(q,"interval");
    EXIT("")
}

void
qmlTrack::save()
{
    ENTER("")
    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
    if (_trkid>0)
    {
        q.exec("REPLACE INTO tracks (trk,name,north,west,south,east,interval) VALUES (\""
               + QString::number(_trkid)    + "\",\""
               + _name                      + "\",\""
               + QString::number(_top)      + "\",\""
               + QString::number(_left)     + "\",\""
               + QString::number(_bottom)   + "\",\""
               + QString::number(_right)    + "\",\""
               + QString::number(_interval) + "\")");
    }
    else
    {
        q.exec("INSERT  INTO tracks (name,north,west,south,east,interval) VALUES (\""
               + _name                      + "\",\""
               + QString::number(_top)      + "\",\""
               + QString::number(_left)     + "\",\""
               + QString::number(_bottom)   + "\",\""
               + QString::number(_right)    + "\",\""
               + QString::number(_interval) + "\")");
        _trkid = q.lastInsertId().toInt();
        emit trkidChanged();
    }
    //_dirty = false;
    EXIT("")
}
