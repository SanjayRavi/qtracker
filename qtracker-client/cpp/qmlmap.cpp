#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QStringList>
#include "database.h"
#include "qmldatabase.h"

#define ENABLE_DEBUG
#include "helpers.h"

qmlMap::qmlMap()
    : _name("map")
    , _mapid(-1)
{
    ENTER("")
}

qmlMap::qmlMap(int id)
    : _name("map")
    , _mapid(-1)
{
    ENTER("")
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT * FROM maps WHERE mapid='" + QString::number(id) + "'",db);
    if (q.next())
        load(q);

    EXIT("")
}

qmlMap::qmlMap(const QSqlQuery& q)
    : _name("map")
    , _mapid(-1)
{
    load(q);
}

void
qmlMap::load(const QSqlQuery& q)
{
    ENTER("")
    _mapid     = getIntField(q,"mapid");
    _name      = getStringField(q,"name");
    _top       = getDoubleField(q,"north");
    _left      = getDoubleField(q,"west");
    _bottom    = getDoubleField(q,"south");
    _right     = getDoubleField(q,"east");
    EXIT("")
}

void qmlMap::save()
{
    QSqlDatabase& db = Database::Db();
    QSqlQuery q(db);
    if (_mapid>0)
    {
        q.exec("REPLACE INTO maps (mapid,name,north,west,south,east) VALUES (\"" + QString::number(_mapid) + "\",\"" + _name  + "\",\"" + QString::number(_top)  + "\",\"" + QString::number(_left)  + "\",\"" + QString::number(_bottom)  + "\",\"" + QString::number(_right) + "\")");
    }
    else
    {
        q.exec("INSERT  INTO categories (name) VALUES (\"" + _name  + "\",\"" + QString::number(_top)  + "\",\"" + QString::number(_left)  + "\",\"" + QString::number(_bottom)  + "\",\"" + QString::number(_right) + "\")");
        _mapid = q.lastInsertId().toInt();
        emit mapidChanged();
    }
    //_dirty = false;
}

QDeclarativeListProperty<qmlRefpoint>
qmlMap::refpoints()
{
    ENTER("")
    return QDeclarativeListProperty<qmlRefpoint>(this, _refpts);
}

void
qmlMap::selectRefpoints(int offset, int limit)
{
    ENTER("")
    QDeclarativeListReference r(this,"refpoints");
    r.clear();
    QSqlDatabase& db = Database::Db();
    QSqlQuery q("SELECT mappt FROM mappoints WHERE mapid='" + QString::number(_mapid) + "' LIMIT " + QString::number(limit) + " OFFSET " + QString::number(offset),db);
    while (q.next()) { r.append(new qmlRefpoint(q.value(0).toInt())); }
    EXIT("")
}
