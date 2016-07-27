#include "Building.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Floor.hpp"

// keys of QVariantMap used in this APP
static const QString buildingIdKey = "buildingId";
static const QString buildingNameKey = "buildingName";
static const QString inAssetsKey = "inAssets";
static const QString floorsKey = "floors";

// keys used from Server API etc
static const QString buildingIdForeignKey = "buildingId";
static const QString buildingNameForeignKey = "buildingName";
static const QString inAssetsForeignKey = "inAssets";
static const QString floorsForeignKey = "floors";

/*
 * Default Constructor if Building not initialized from QVariantMap
 */
Building::Building(QObject *parent) :
        QObject(parent), mBuildingId(-1), mBuildingName(""), mInAssets(false)
{
		// lazy Arrays where only keys are persisted
		mFloorsKeysResolved = false;
}

bool Building::isAllResolved()
{
    if(!areFloorsKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize Building from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Building::fillFromMap(const QVariantMap& buildingMap)
{
	mBuildingId = buildingMap.value(buildingIdKey).toInt();
	mBuildingName = buildingMap.value(buildingNameKey).toString();
	mInAssets = buildingMap.value(inAssetsKey).toBool();
	// mFloors is (lazy loaded) Array of Floor*
	mFloorsKeys = buildingMap.value(floorsKey).toStringList();
	// mFloors must be resolved later if there are keys
	mFloorsKeysResolved = (mFloorsKeys.size() == 0);
	mFloors.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Building::fillFromForeignMap(const QVariantMap& buildingMap)
{
	mBuildingId = buildingMap.value(buildingIdForeignKey).toInt();
	mBuildingName = buildingMap.value(buildingNameForeignKey).toString();
	mInAssets = buildingMap.value(inAssetsForeignKey).toBool();
	// mFloors is (lazy loaded) Array of Floor*
	mFloorsKeys = buildingMap.value(floorsForeignKey).toStringList();
	// mFloors must be resolved later if there are keys
	mFloorsKeysResolved = (mFloorsKeys.size() == 0);
	mFloors.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Building::fillFromCacheMap(const QVariantMap& buildingMap)
{
	mBuildingId = buildingMap.value(buildingIdKey).toInt();
	mBuildingName = buildingMap.value(buildingNameKey).toString();
	mInAssets = buildingMap.value(inAssetsKey).toBool();
	// mFloors is (lazy loaded) Array of Floor*
	mFloorsKeys = buildingMap.value(floorsKey).toStringList();
	// mFloors must be resolved later if there are keys
	mFloorsKeysResolved = (mFloorsKeys.size() == 0);
	mFloors.clear();
}

void Building::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Building::isValid()
{
	if (mBuildingId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Building as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Building::toMap()
{
	QVariantMap buildingMap;
	// mFloors points to Floor*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mFloorsKeysResolved) {
		mFloorsKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mFloors.size(); ++i) {
		Floor* floor;
		floor = mFloors.at(i);
		mFloorsKeys << QString::number(floor->floorId());
	}
	buildingMap.insert(floorsKey, mFloorsKeys);
	buildingMap.insert(buildingIdKey, mBuildingId);
	buildingMap.insert(buildingNameKey, mBuildingName);
	buildingMap.insert(inAssetsKey, mInAssets);
	return buildingMap;
}

/*
 * Exports Properties from Building as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Building::toForeignMap()
{
	QVariantMap buildingMap;
	// mFloors points to Floor*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mFloorsKeysResolved) {
		mFloorsKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mFloors.size(); ++i) {
		Floor* floor;
		floor = mFloors.at(i);
		mFloorsKeys << QString::number(floor->floorId());
	}
	buildingMap.insert(floorsKey, mFloorsKeys);
	buildingMap.insert(buildingIdForeignKey, mBuildingId);
	buildingMap.insert(buildingNameForeignKey, mBuildingName);
	buildingMap.insert(inAssetsForeignKey, mInAssets);
	return buildingMap;
}


/*
 * Exports Properties from Building as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Building::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: buildingId
// Domain KEY: buildingId
int Building::buildingId() const
{
	return mBuildingId;
}

void Building::setBuildingId(int buildingId)
{
	if (buildingId != mBuildingId) {
		mBuildingId = buildingId;
		emit buildingIdChanged(buildingId);
	}
}
// ATT 
// Optional: buildingName
QString Building::buildingName() const
{
	return mBuildingName;
}

void Building::setBuildingName(QString buildingName)
{
	if (buildingName != mBuildingName) {
		mBuildingName = buildingName;
		emit buildingNameChanged(buildingName);
	}
}
// ATT 
// Optional: inAssets
bool Building::inAssets() const
{
	return mInAssets;
}

void Building::setInAssets(bool inAssets)
{
	if (inAssets != mInAssets) {
		mInAssets = inAssets;
		emit inAssetsChanged(inAssets);
	}
}
// ATT 
// Optional: floors
QVariantList Building::floorsAsQVariantList()
{
	QVariantList floorsList;
	for (int i = 0; i < mFloors.size(); ++i) {
        floorsList.append((mFloors.at(i))->toMap());
    }
	return floorsList;
}
QVariantList Building::floorsAsForeignQVariantList()
{
	QVariantList floorsList;
	for (int i = 0; i < mFloors.size(); ++i) {
        floorsList.append((mFloors.at(i))->toForeignMap());
    }
	return floorsList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add floors without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveFloorsKeys
 */
void Building::addToFloors(Floor* floor)
{
    mFloors.append(floor);
    emit addedToFloors(floor);
    emit floorsPropertyListChanged();
}

bool Building::removeFromFloors(Floor* floor)
{
    bool ok = false;
    ok = mFloors.removeOne(floor);
    if (!ok) {
    	qDebug() << "Floor* not found in floors";
    	return false;
    }
    emit floorsPropertyListChanged();
    // floors are independent - DON'T delete them
    return true;
}
void Building::clearFloors()
{
    for (int i = mFloors.size(); i > 0; --i) {
        removeFromFloors(mFloors.last());
    }
    mFloorsKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: floorsKeys()
 * - resolve them from DataManager
 * - then resolveFloorsKeys()
 */
bool Building::areFloorsKeysResolved()
{
    return mFloorsKeysResolved;
}

QStringList Building::floorsKeys()
{
    return mFloorsKeys;
}

/**
 * Objects from floorsKeys will be added to existing floors
 * This enables to use addToFloors() without resolving before
 * Hint: it's your responsibility to resolve before looping thru floors
 */
void Building::resolveFloorsKeys(QList<Floor*> floors)
{
    if(mFloorsKeysResolved){
        return;
    }
    // don't clear mFloors (see above)
    for (int i = 0; i < floors.size(); ++i) {
        addToFloors(floors.at(i));
    }
    mFloorsKeysResolved = true;
}

int Building::floorsCount()
{
    return mFloors.size();
}
QList<Floor*> Building::floors()
{
	return mFloors;
}
void Building::setFloors(QList<Floor*> floors) 
{
	if (floors != mFloors) {
		mFloors = floors;
		emit floorsChanged(floors);
		emit floorsPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * building.floorsPropertyList.length to get the size
 * building.floorsPropertyList[2] to get Floor* at position 2
 * building.floorsPropertyList = [] to clear the list
 * or get easy access to properties like
 * building.floorsPropertyList[2].myPropertyName
 */
QQmlListProperty<Floor> Building::floorsPropertyList()
{
    return QQmlListProperty<Floor>(this, 0, &Building::appendToFloorsProperty,
            &Building::floorsPropertyCount, &Building::atFloorsProperty,
            &Building::clearFloorsProperty);
}
void Building::appendToFloorsProperty(QQmlListProperty<Floor> *floorsList,
        Floor* floor)
{
    Building *buildingObject = qobject_cast<Building *>(floorsList->object);
    if (buildingObject) {
        buildingObject->mFloors.append(floor);
        emit buildingObject->addedToFloors(floor);
    } else {
        qWarning() << "cannot append Floor* to floors " << "Object is not of type Building*";
    }
}
int Building::floorsPropertyCount(QQmlListProperty<Floor> *floorsList)
{
    Building *building = qobject_cast<Building *>(floorsList->object);
    if (building) {
        return building->mFloors.size();
    } else {
        qWarning() << "cannot get size floors " << "Object is not of type Building*";
    }
    return 0;
}
Floor* Building::atFloorsProperty(QQmlListProperty<Floor> *floorsList, int pos)
{
    Building *building = qobject_cast<Building *>(floorsList->object);
    if (building) {
        if (building->mFloors.size() > pos) {
            return building->mFloors.at(pos);
        }
        qWarning() << "cannot get Floor* at pos " << pos << " size is "
                << building->mFloors.size();
    } else {
        qWarning() << "cannot get Floor* at pos " << pos << "Object is not of type Building*";
    }
    return 0;
}
void Building::clearFloorsProperty(QQmlListProperty<Floor> *floorsList)
{
    Building *building = qobject_cast<Building *>(floorsList->object);
    if (building) {
        // floors are independent - DON'T delete them
        building->mFloors.clear();
    } else {
        qWarning() << "cannot clear floors " << "Object is not of type Building*";
    }
}



Building::~Building()
{
	// place cleanUp code here
}
	
