#include "Floor.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Building.hpp"
// target also references to this
#include "Room.hpp"

// keys of QVariantMap used in this APP
static const QString floorIdKey = "floorId";
static const QString floorNameKey = "floorName";
static const QString inAssetsKey = "inAssets";
static const QString buildingKey = "building";
static const QString roomsKey = "rooms";

// keys used from Server API etc
static const QString floorIdForeignKey = "floorId";
static const QString floorNameForeignKey = "floorName";
static const QString inAssetsForeignKey = "inAssets";
static const QString buildingForeignKey = "building";
static const QString roomsForeignKey = "rooms";

/*
 * Default Constructor if Floor not initialized from QVariantMap
 */
Floor::Floor(QObject *parent) :
        QObject(parent), mFloorId(-1), mFloorName(""), mInAssets(false)
{
	// lazy references:
	mBuilding = -1;
	mBuildingAsDataObject = 0;
	mBuildingInvalid = false;
		// lazy Arrays where only keys are persisted
		mRoomsKeysResolved = false;
}

bool Floor::isAllResolved()
{
	if (hasBuilding() && !isBuildingResolvedAsDataObject()) {
		return false;
	}
    if(!areRoomsKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize Floor from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Floor::fillFromMap(const QVariantMap& floorMap)
{
	mFloorId = floorMap.value(floorIdKey).toInt();
	mFloorName = floorMap.value(floorNameKey).toString();
	mInAssets = floorMap.value(inAssetsKey).toBool();
	// building lazy pointing to Building* (domainKey: buildingId)
	if (floorMap.contains(buildingKey)) {
		mBuilding = floorMap.value(buildingKey).toInt();
		if (mBuilding != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mRooms is (lazy loaded) Array of Room*
	mRoomsKeys = floorMap.value(roomsKey).toStringList();
	// mRooms must be resolved later if there are keys
	mRoomsKeysResolved = (mRoomsKeys.size() == 0);
	mRooms.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Floor::fillFromForeignMap(const QVariantMap& floorMap)
{
	mFloorId = floorMap.value(floorIdForeignKey).toInt();
	mFloorName = floorMap.value(floorNameForeignKey).toString();
	mInAssets = floorMap.value(inAssetsForeignKey).toBool();
	// building lazy pointing to Building* (domainKey: buildingId)
	if (floorMap.contains(buildingForeignKey)) {
		mBuilding = floorMap.value(buildingForeignKey).toInt();
		if (mBuilding != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mRooms is (lazy loaded) Array of Room*
	mRoomsKeys = floorMap.value(roomsForeignKey).toStringList();
	// mRooms must be resolved later if there are keys
	mRoomsKeysResolved = (mRoomsKeys.size() == 0);
	mRooms.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Floor::fillFromCacheMap(const QVariantMap& floorMap)
{
	mFloorId = floorMap.value(floorIdKey).toInt();
	mFloorName = floorMap.value(floorNameKey).toString();
	mInAssets = floorMap.value(inAssetsKey).toBool();
	// building lazy pointing to Building* (domainKey: buildingId)
	if (floorMap.contains(buildingKey)) {
		mBuilding = floorMap.value(buildingKey).toInt();
		if (mBuilding != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mRooms is (lazy loaded) Array of Room*
	mRoomsKeys = floorMap.value(roomsKey).toStringList();
	// mRooms must be resolved later if there are keys
	mRoomsKeysResolved = (mRoomsKeys.size() == 0);
	mRooms.clear();
}

void Floor::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Floor::isValid()
{
	if (mFloorId == -1) {
		return false;
	}
	// building lazy pointing to Building* (domainKey: buildingId)
	if (mBuilding == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Floor as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Floor::toMap()
{
	QVariantMap floorMap;
	// building lazy pointing to Building* (domainKey: buildingId)
	if (mBuilding != -1) {
		floorMap.insert(buildingKey, mBuilding);
	}
	// mRooms points to Room*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mRoomsKeysResolved) {
		mRoomsKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mRooms.size(); ++i) {
		Room* room;
		room = mRooms.at(i);
		mRoomsKeys << QString::number(room->roomId());
	}
	floorMap.insert(roomsKey, mRoomsKeys);
	floorMap.insert(floorIdKey, mFloorId);
	floorMap.insert(floorNameKey, mFloorName);
	floorMap.insert(inAssetsKey, mInAssets);
	return floorMap;
}

/*
 * Exports Properties from Floor as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Floor::toForeignMap()
{
	QVariantMap floorMap;
	// building lazy pointing to Building* (domainKey: buildingId)
	if (mBuilding != -1) {
		floorMap.insert(buildingForeignKey, mBuilding);
	}
	// mRooms points to Room*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mRoomsKeysResolved) {
		mRoomsKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mRooms.size(); ++i) {
		Room* room;
		room = mRooms.at(i);
		mRoomsKeys << QString::number(room->roomId());
	}
	floorMap.insert(roomsKey, mRoomsKeys);
	floorMap.insert(floorIdForeignKey, mFloorId);
	floorMap.insert(floorNameForeignKey, mFloorName);
	floorMap.insert(inAssetsForeignKey, mInAssets);
	return floorMap;
}


/*
 * Exports Properties from Floor as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Floor::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// REF
// Lazy: building
// Mandatory: building
// building lazy pointing to Building* (domainKey: buildingId)
int Floor::building() const
{
	return mBuilding;
}
Building* Floor::buildingAsDataObject() const
{
	return mBuildingAsDataObject;
}
void Floor::setBuilding(int building)
{
	if (building != mBuilding) {
        // remove old Data Object if one was resolved
        if (mBuildingAsDataObject) {
            // reset pointer, don't delete the independent object !
            mBuildingAsDataObject = 0;
        }
        // set the new lazy reference
        mBuilding = building;
        mBuildingInvalid = false;
        emit buildingChanged(building);
        if (building != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Floor::removeBuilding()
{
	if (mBuilding != -1) {
		setBuilding(-1);
	}
}
bool Floor::hasBuilding()
{
    if (!mBuildingInvalid && mBuilding != -1) {
        return true;
    } else {
        return false;
    }
}
bool Floor::isBuildingResolvedAsDataObject()
{
    if (!mBuildingInvalid && mBuildingAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite buildingId if different
void Floor::resolveBuildingAsDataObject(Building* building)
{
    if (building) {
        if (building->buildingId() != mBuilding) {
            setBuilding(building->buildingId());
        }
        mBuildingAsDataObject = building;
        mBuildingInvalid = false;
    }
}
void Floor::markBuildingAsInvalid()
{
    mBuildingInvalid = true;
}
// ATT 
// Mandatory: floorId
// Domain KEY: floorId
int Floor::floorId() const
{
	return mFloorId;
}

void Floor::setFloorId(int floorId)
{
	if (floorId != mFloorId) {
		mFloorId = floorId;
		emit floorIdChanged(floorId);
	}
}
// ATT 
// Optional: floorName
QString Floor::floorName() const
{
	return mFloorName;
}

void Floor::setFloorName(QString floorName)
{
	if (floorName != mFloorName) {
		mFloorName = floorName;
		emit floorNameChanged(floorName);
	}
}
// ATT 
// Optional: inAssets
bool Floor::inAssets() const
{
	return mInAssets;
}

void Floor::setInAssets(bool inAssets)
{
	if (inAssets != mInAssets) {
		mInAssets = inAssets;
		emit inAssetsChanged(inAssets);
	}
}
// ATT 
// Optional: rooms
QVariantList Floor::roomsAsQVariantList()
{
	QVariantList roomsList;
	for (int i = 0; i < mRooms.size(); ++i) {
        roomsList.append((mRooms.at(i))->toMap());
    }
	return roomsList;
}
QVariantList Floor::roomsAsForeignQVariantList()
{
	QVariantList roomsList;
	for (int i = 0; i < mRooms.size(); ++i) {
        roomsList.append((mRooms.at(i))->toForeignMap());
    }
	return roomsList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add rooms without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveRoomsKeys
 */
void Floor::addToRooms(Room* room)
{
    mRooms.append(room);
    emit addedToRooms(room);
    emit roomsPropertyListChanged();
}

bool Floor::removeFromRooms(Room* room)
{
    bool ok = false;
    ok = mRooms.removeOne(room);
    if (!ok) {
    	qDebug() << "Room* not found in rooms";
    	return false;
    }
    emit roomsPropertyListChanged();
    // rooms are independent - DON'T delete them
    return true;
}
void Floor::clearRooms()
{
    for (int i = mRooms.size(); i > 0; --i) {
        removeFromRooms(mRooms.last());
    }
    mRoomsKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: roomsKeys()
 * - resolve them from DataManager
 * - then resolveRoomsKeys()
 */
bool Floor::areRoomsKeysResolved()
{
    return mRoomsKeysResolved;
}

QStringList Floor::roomsKeys()
{
    return mRoomsKeys;
}

/**
 * Objects from roomsKeys will be added to existing rooms
 * This enables to use addToRooms() without resolving before
 * Hint: it's your responsibility to resolve before looping thru rooms
 */
void Floor::resolveRoomsKeys(QList<Room*> rooms)
{
    if(mRoomsKeysResolved){
        return;
    }
    // don't clear mRooms (see above)
    for (int i = 0; i < rooms.size(); ++i) {
        addToRooms(rooms.at(i));
    }
    mRoomsKeysResolved = true;
}

int Floor::roomsCount()
{
    return mRooms.size();
}
QList<Room*> Floor::rooms()
{
	return mRooms;
}
void Floor::setRooms(QList<Room*> rooms) 
{
	if (rooms != mRooms) {
		mRooms = rooms;
		emit roomsChanged(rooms);
		emit roomsPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * floor.roomsPropertyList.length to get the size
 * floor.roomsPropertyList[2] to get Room* at position 2
 * floor.roomsPropertyList = [] to clear the list
 * or get easy access to properties like
 * floor.roomsPropertyList[2].myPropertyName
 */
QQmlListProperty<Room> Floor::roomsPropertyList()
{
    return QQmlListProperty<Room>(this, 0, &Floor::appendToRoomsProperty,
            &Floor::roomsPropertyCount, &Floor::atRoomsProperty,
            &Floor::clearRoomsProperty);
}
void Floor::appendToRoomsProperty(QQmlListProperty<Room> *roomsList,
        Room* room)
{
    Floor *floorObject = qobject_cast<Floor *>(roomsList->object);
    if (floorObject) {
        floorObject->mRooms.append(room);
        emit floorObject->addedToRooms(room);
    } else {
        qWarning() << "cannot append Room* to rooms " << "Object is not of type Floor*";
    }
}
int Floor::roomsPropertyCount(QQmlListProperty<Room> *roomsList)
{
    Floor *floor = qobject_cast<Floor *>(roomsList->object);
    if (floor) {
        return floor->mRooms.size();
    } else {
        qWarning() << "cannot get size rooms " << "Object is not of type Floor*";
    }
    return 0;
}
Room* Floor::atRoomsProperty(QQmlListProperty<Room> *roomsList, int pos)
{
    Floor *floor = qobject_cast<Floor *>(roomsList->object);
    if (floor) {
        if (floor->mRooms.size() > pos) {
            return floor->mRooms.at(pos);
        }
        qWarning() << "cannot get Room* at pos " << pos << " size is "
                << floor->mRooms.size();
    } else {
        qWarning() << "cannot get Room* at pos " << pos << "Object is not of type Floor*";
    }
    return 0;
}
void Floor::clearRoomsProperty(QQmlListProperty<Room> *roomsList)
{
    Floor *floor = qobject_cast<Floor *>(roomsList->object);
    if (floor) {
        // rooms are independent - DON'T delete them
        floor->mRooms.clear();
    } else {
        qWarning() << "cannot clear rooms " << "Object is not of type Floor*";
    }
}



Floor::~Floor()
{
	// place cleanUp code here
}
	
