#include "Room.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Floor.hpp"
// target also references to this
#include "Session.hpp"

// keys of QVariantMap used in this APP
static const QString roomIdKey = "roomId";
static const QString roomNameKey = "roomName";
static const QString inAssetsKey = "inAssets";
static const QString floorKey = "floor";
static const QString sessionsKey = "sessions";

// keys used from Server API etc
static const QString roomIdForeignKey = "roomId";
static const QString roomNameForeignKey = "roomName";
static const QString inAssetsForeignKey = "inAssets";
static const QString floorForeignKey = "floor";
static const QString sessionsForeignKey = "sessions";

/*
 * Default Constructor if Room not initialized from QVariantMap
 */
Room::Room(QObject *parent) :
        QObject(parent), mRoomId(-1), mRoomName(""), mInAssets(false)
{
	// lazy references:
	mFloor = -1;
	mFloorAsDataObject = 0;
	mFloorInvalid = false;
		// lazy Arrays where only keys are persisted
		mSessionsKeysResolved = false;
}

bool Room::isAllResolved()
{
	if (hasFloor() && !isFloorResolvedAsDataObject()) {
		return false;
	}
    if(!areSessionsKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize Room from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Room::fillFromMap(const QVariantMap& roomMap)
{
	mRoomId = roomMap.value(roomIdKey).toInt();
	mRoomName = roomMap.value(roomNameKey).toString();
	mInAssets = roomMap.value(inAssetsKey).toBool();
	// floor lazy pointing to Floor* (domainKey: floorId)
	if (roomMap.contains(floorKey)) {
		mFloor = roomMap.value(floorKey).toInt();
		if (mFloor != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mSessions is (lazy loaded) Array of Session*
	mSessionsKeys = roomMap.value(sessionsKey).toStringList();
	// mSessions must be resolved later if there are keys
	mSessionsKeysResolved = (mSessionsKeys.size() == 0);
	mSessions.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Room::fillFromForeignMap(const QVariantMap& roomMap)
{
	mRoomId = roomMap.value(roomIdForeignKey).toInt();
	mRoomName = roomMap.value(roomNameForeignKey).toString();
	mInAssets = roomMap.value(inAssetsForeignKey).toBool();
	// floor lazy pointing to Floor* (domainKey: floorId)
	if (roomMap.contains(floorForeignKey)) {
		mFloor = roomMap.value(floorForeignKey).toInt();
		if (mFloor != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mSessions is (lazy loaded) Array of Session*
	mSessionsKeys = roomMap.value(sessionsForeignKey).toStringList();
	// mSessions must be resolved later if there are keys
	mSessionsKeysResolved = (mSessionsKeys.size() == 0);
	mSessions.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Room::fillFromCacheMap(const QVariantMap& roomMap)
{
	mRoomId = roomMap.value(roomIdKey).toInt();
	mRoomName = roomMap.value(roomNameKey).toString();
	mInAssets = roomMap.value(inAssetsKey).toBool();
	// floor lazy pointing to Floor* (domainKey: floorId)
	if (roomMap.contains(floorKey)) {
		mFloor = roomMap.value(floorKey).toInt();
		if (mFloor != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mSessions is (lazy loaded) Array of Session*
	mSessionsKeys = roomMap.value(sessionsKey).toStringList();
	// mSessions must be resolved later if there are keys
	mSessionsKeysResolved = (mSessionsKeys.size() == 0);
	mSessions.clear();
}

void Room::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Room::isValid()
{
	if (mRoomId == -1) {
		return false;
	}
	// floor lazy pointing to Floor* (domainKey: floorId)
	if (mFloor == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Room as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Room::toMap()
{
	QVariantMap roomMap;
	// floor lazy pointing to Floor* (domainKey: floorId)
	if (mFloor != -1) {
		roomMap.insert(floorKey, mFloor);
	}
	// mSessions points to Session*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mSessionsKeysResolved) {
		mSessionsKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mSessions.size(); ++i) {
		Session* session;
		session = mSessions.at(i);
		mSessionsKeys << QString::number(session->sessionId());
	}
	roomMap.insert(sessionsKey, mSessionsKeys);
	roomMap.insert(roomIdKey, mRoomId);
	roomMap.insert(roomNameKey, mRoomName);
	roomMap.insert(inAssetsKey, mInAssets);
	return roomMap;
}

/*
 * Exports Properties from Room as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Room::toForeignMap()
{
	QVariantMap roomMap;
	// floor lazy pointing to Floor* (domainKey: floorId)
	if (mFloor != -1) {
		roomMap.insert(floorForeignKey, mFloor);
	}
	// mSessions points to Session*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mSessionsKeysResolved) {
		mSessionsKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mSessions.size(); ++i) {
		Session* session;
		session = mSessions.at(i);
		mSessionsKeys << QString::number(session->sessionId());
	}
	roomMap.insert(sessionsKey, mSessionsKeys);
	roomMap.insert(roomIdForeignKey, mRoomId);
	roomMap.insert(roomNameForeignKey, mRoomName);
	roomMap.insert(inAssetsForeignKey, mInAssets);
	return roomMap;
}


/*
 * Exports Properties from Room as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Room::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// REF
// Lazy: floor
// Mandatory: floor
// floor lazy pointing to Floor* (domainKey: floorId)
int Room::floor() const
{
	return mFloor;
}
Floor* Room::floorAsDataObject() const
{
	return mFloorAsDataObject;
}
void Room::setFloor(int floor)
{
	if (floor != mFloor) {
        // remove old Data Object if one was resolved
        if (mFloorAsDataObject) {
            // reset pointer, don't delete the independent object !
            mFloorAsDataObject = 0;
        }
        // set the new lazy reference
        mFloor = floor;
        mFloorInvalid = false;
        emit floorChanged(floor);
        if (floor != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Room::removeFloor()
{
	if (mFloor != -1) {
		setFloor(-1);
	}
}
bool Room::hasFloor()
{
    if (!mFloorInvalid && mFloor != -1) {
        return true;
    } else {
        return false;
    }
}
bool Room::isFloorResolvedAsDataObject()
{
    if (!mFloorInvalid && mFloorAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite floorId if different
void Room::resolveFloorAsDataObject(Floor* floor)
{
    if (floor) {
        if (floor->floorId() != mFloor) {
            setFloor(floor->floorId());
        }
        mFloorAsDataObject = floor;
        mFloorInvalid = false;
    }
}
void Room::markFloorAsInvalid()
{
    mFloorInvalid = true;
}
// ATT 
// Mandatory: roomId
// Domain KEY: roomId
int Room::roomId() const
{
	return mRoomId;
}

void Room::setRoomId(int roomId)
{
	if (roomId != mRoomId) {
		mRoomId = roomId;
		emit roomIdChanged(roomId);
	}
}
// ATT 
// Optional: roomName
QString Room::roomName() const
{
	return mRoomName;
}

void Room::setRoomName(QString roomName)
{
	if (roomName != mRoomName) {
		mRoomName = roomName;
		emit roomNameChanged(roomName);
	}
}
// ATT 
// Optional: inAssets
bool Room::inAssets() const
{
	return mInAssets;
}

void Room::setInAssets(bool inAssets)
{
	if (inAssets != mInAssets) {
		mInAssets = inAssets;
		emit inAssetsChanged(inAssets);
	}
}
// ATT 
// Optional: sessions
QVariantList Room::sessionsAsQVariantList()
{
	QVariantList sessionsList;
	for (int i = 0; i < mSessions.size(); ++i) {
        sessionsList.append((mSessions.at(i))->toMap());
    }
	return sessionsList;
}
QVariantList Room::sessionsAsForeignQVariantList()
{
	QVariantList sessionsList;
	for (int i = 0; i < mSessions.size(); ++i) {
        sessionsList.append((mSessions.at(i))->toForeignMap());
    }
	return sessionsList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add sessions without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveSessionsKeys
 */
void Room::addToSessions(Session* session)
{
    mSessions.append(session);
    emit addedToSessions(session);
    emit sessionsPropertyListChanged();
}

bool Room::removeFromSessions(Session* session)
{
    bool ok = false;
    ok = mSessions.removeOne(session);
    if (!ok) {
    	qDebug() << "Session* not found in sessions";
    	return false;
    }
    emit sessionsPropertyListChanged();
    // sessions are independent - DON'T delete them
    return true;
}
void Room::clearSessions()
{
    for (int i = mSessions.size(); i > 0; --i) {
        removeFromSessions(mSessions.last());
    }
    mSessionsKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: sessionsKeys()
 * - resolve them from DataManager
 * - then resolveSessionsKeys()
 */
bool Room::areSessionsKeysResolved()
{
    return mSessionsKeysResolved;
}

QStringList Room::sessionsKeys()
{
    return mSessionsKeys;
}

/**
 * Objects from sessionsKeys will be added to existing sessions
 * This enables to use addToSessions() without resolving before
 * Hint: it's your responsibility to resolve before looping thru sessions
 */
void Room::resolveSessionsKeys(QList<Session*> sessions)
{
    if(mSessionsKeysResolved){
        return;
    }
    // don't clear mSessions (see above)
    for (int i = 0; i < sessions.size(); ++i) {
        addToSessions(sessions.at(i));
    }
    mSessionsKeysResolved = true;
}

int Room::sessionsCount()
{
    return mSessions.size();
}
QList<Session*> Room::sessions()
{
	return mSessions;
}
void Room::setSessions(QList<Session*> sessions) 
{
	if (sessions != mSessions) {
		mSessions = sessions;
		emit sessionsChanged(sessions);
		emit sessionsPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * room.sessionsPropertyList.length to get the size
 * room.sessionsPropertyList[2] to get Session* at position 2
 * room.sessionsPropertyList = [] to clear the list
 * or get easy access to properties like
 * room.sessionsPropertyList[2].myPropertyName
 */
QQmlListProperty<Session> Room::sessionsPropertyList()
{
    return QQmlListProperty<Session>(this, 0, &Room::appendToSessionsProperty,
            &Room::sessionsPropertyCount, &Room::atSessionsProperty,
            &Room::clearSessionsProperty);
}
void Room::appendToSessionsProperty(QQmlListProperty<Session> *sessionsList,
        Session* session)
{
    Room *roomObject = qobject_cast<Room *>(sessionsList->object);
    if (roomObject) {
        roomObject->mSessions.append(session);
        emit roomObject->addedToSessions(session);
    } else {
        qWarning() << "cannot append Session* to sessions " << "Object is not of type Room*";
    }
}
int Room::sessionsPropertyCount(QQmlListProperty<Session> *sessionsList)
{
    Room *room = qobject_cast<Room *>(sessionsList->object);
    if (room) {
        return room->mSessions.size();
    } else {
        qWarning() << "cannot get size sessions " << "Object is not of type Room*";
    }
    return 0;
}
Session* Room::atSessionsProperty(QQmlListProperty<Session> *sessionsList, int pos)
{
    Room *room = qobject_cast<Room *>(sessionsList->object);
    if (room) {
        if (room->mSessions.size() > pos) {
            return room->mSessions.at(pos);
        }
        qWarning() << "cannot get Session* at pos " << pos << " size is "
                << room->mSessions.size();
    } else {
        qWarning() << "cannot get Session* at pos " << pos << "Object is not of type Room*";
    }
    return 0;
}
void Room::clearSessionsProperty(QQmlListProperty<Session> *sessionsList)
{
    Room *room = qobject_cast<Room *>(sessionsList->object);
    if (room) {
        // sessions are independent - DON'T delete them
        room->mSessions.clear();
    } else {
        qWarning() << "cannot clear sessions " << "Object is not of type Room*";
    }
}



Room::~Room()
{
	// place cleanUp code here
}
	
