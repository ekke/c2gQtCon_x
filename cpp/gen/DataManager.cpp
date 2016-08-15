#include <QObject>

#include "DataManager.hpp"

#include <QtQml>
#include <QJsonObject>
#include <QFile>


static const QString PRODUCTION_ENVIRONMENT = "prod/";
static const QString TEST_ENVIRONMENT = "test/";
static bool isProductionEnvironment = true;


static const QString cacheSettingsData = "cacheSettingsData.json";
static const QString cacheConference = "cacheConference.json";
static const QString cacheBuilding = "cacheBuilding.json";
static const QString cacheFloor = "cacheFloor.json";
static const QString cacheRoom = "cacheRoom.json";
static const QString cacheSession = "cacheSession.json";
static const QString cacheScheduleItem = "cacheScheduleItem.json";
static const QString cacheFavorite = "cacheFavorite.json";
static const QString cacheBookmark = "cacheBookmark.json";
static const QString cacheSessionLists = "cacheSessionLists.json";
static const QString cacheSessionLink = "cacheSessionLink.json";
static const QString cacheSpeaker = "cacheSpeaker.json";
static const QString cacheSpeakerImage = "cacheSpeakerImage.json";
static const QString cacheSessionTrack = "cacheSessionTrack.json";
static const QString cacheDay = "cacheDay.json";
static const QString cacheSessionAPI = "cacheSessionAPI.json";
static const QString cachePersonsAPI = "cachePersonsAPI.json";
static const QString cacheSessionLinkAPI = "cacheSessionLinkAPI.json";
static const QString cacheSpeakerAPI = "cacheSpeakerAPI.json";


DataManager::DataManager(QObject *parent) :
        QObject(parent)
{
   // Android: HomeLocation works, iOS: not writable
    // Android: AppDataLocation works out of the box, iOS you must create the DIR first !!
    mDataRoot = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).value(0);
    mDataPath = mDataRoot+"/data/";
    mDataAssetsPath = ":/data-assets/";
    qDebug() << "Data Path: " << mDataPath << " data-assets: " << mDataAssetsPath;
    // guarantee that dirs exist
    bool ok = checkDirs();
    if(!ok) {
        qFatal("App won't work - cannot create data directory");
    }

    // at first read settingsData (always from Sandbox)
    mSettingsPath = mDataRoot+"/"+cacheSettingsData;
    qDebug() << "Settings Path: " << mSettingsPath;
    readSettings();

#ifdef QT_DEBUG
    qDebug() << "Running a DEBUG BUILD";
    // DEBUG MODE ?
    // now check if public cache is used
    if (mSettingsData->hasPublicCache()) {
        // great while testing: access files from file explorer
        // only works on Android - on iOS it helps to use a 2nd cache for dev
        mDataRoot = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).value(0);
        // per ex. /data/ekkescorner/theAppName
        mDataRoot += mSettingsData->publicRoot4Dev();
        mDataPath = mDataRoot+"/data/";
        ok = checkDirs();
        if(!ok) {
            qFatal("App won't work - cannot create data directory");
        }
        qDebug() << "Data Path redirected to PUBLIC CACHE: " << mDataPath;
        // tip: copy settingsData to public cache to see the content
        // but settings will always be used from AppDataLocation
    }
#else
    qDebug() << "Running a RELEASE BUILD";
    // always use compact JSON in release builds
    mSettingsData->setUseCompactJsonFormat(true);
    // never use public data path in releae build
    mSettingsData->setHasPublicCache(false);
#endif
    // now set the compact or indent mode for JSON Documents
    mCompactJson = mSettingsData->useCompactJsonFormat();
	isProductionEnvironment = mSettingsData->isProductionEnvironment();

    // ApplicationUI is parent of DataManager
    // DataManager is parent of all root DataObjects
    // ROOT DataObjects are parent of contained DataObjects
    // ROOT:
    // SettingsData
    // Conference
    // Building
    // Floor
    // Room
    // Session
    // ScheduleItem
    // Favorite
    // Bookmark
    // SessionLists
    // SessionLink
    // Speaker
    // SpeakerImage
    // SessionTrack
    // Day
    // SessionAPI
    // PersonsAPI
    // SessionLinkAPI
    // SpeakerAPI

    // register all DataObjects to get access to properties from QML:
	qmlRegisterType<SettingsData>("org.ekkescorner.data", 1, 0, "SettingsData");
	qmlRegisterType<Conference>("org.ekkescorner.data", 1, 0, "Conference");
	qmlRegisterType<Building>("org.ekkescorner.data", 1, 0, "Building");
	qmlRegisterType<Floor>("org.ekkescorner.data", 1, 0, "Floor");
	qmlRegisterType<Room>("org.ekkescorner.data", 1, 0, "Room");
	qmlRegisterType<Session>("org.ekkescorner.data", 1, 0, "Session");
	qmlRegisterType<ScheduleItem>("org.ekkescorner.data", 1, 0, "ScheduleItem");
	qmlRegisterType<Favorite>("org.ekkescorner.data", 1, 0, "Favorite");
	qmlRegisterType<Bookmark>("org.ekkescorner.data", 1, 0, "Bookmark");
	qmlRegisterType<SessionLists>("org.ekkescorner.data", 1, 0, "SessionLists");
	qmlRegisterType<SessionLink>("org.ekkescorner.data", 1, 0, "SessionLink");
	qmlRegisterType<Speaker>("org.ekkescorner.data", 1, 0, "Speaker");
	qmlRegisterType<SpeakerImage>("org.ekkescorner.data", 1, 0, "SpeakerImage");
	qmlRegisterType<SessionTrack>("org.ekkescorner.data", 1, 0, "SessionTrack");
	qmlRegisterType<Day>("org.ekkescorner.data", 1, 0, "Day");
	qmlRegisterType<SessionAPI>("org.ekkescorner.data", 1, 0, "SessionAPI");
	qmlRegisterType<PersonsAPI>("org.ekkescorner.data", 1, 0, "PersonsAPI");
	qmlRegisterType<SessionLinkAPI>("org.ekkescorner.data", 1, 0, "SessionLinkAPI");
	qmlRegisterType<SpeakerAPI>("org.ekkescorner.data", 1, 0, "SpeakerAPI");
	// register all ENUMs to get access from QML
	
	
	
}

QString DataManager::dataAssetsPath(const QString& fileName)
{
    return mDataAssetsPath + (isProductionEnvironment?PRODUCTION_ENVIRONMENT:TEST_ENVIRONMENT) + fileName;
}
QString DataManager::dataPath(const QString& fileName)
{
    return mDataPath + (isProductionEnvironment?PRODUCTION_ENVIRONMENT:TEST_ENVIRONMENT) + fileName;
}

bool DataManager::checkDirs()
{
    QDir myDir;
    bool exists;
    exists = myDir.exists(mDataRoot);
    if (!exists) {
        bool ok = myDir.mkpath(mDataRoot);
        if(!ok) {
            qWarning() << "Couldn't create mDataRoot " << mDataRoot;
            return false;
        }
        qDebug() << "created directory mDataRoot " << mDataRoot;
    }

    exists = myDir.exists(mDataPath);
    if (!exists) {
        bool ok = myDir.mkpath(mDataPath);
        if(!ok) {
            qWarning() << "Couldn't create mDataPath " << mDataPath;
            return false;
        }
        qDebug() << "created directory mDataPath"  << mDataRoot;
    }

    exists = myDir.exists(mDataPath+PRODUCTION_ENVIRONMENT);
    if (!exists) {
        bool ok = myDir.mkpath(mDataPath+PRODUCTION_ENVIRONMENT);
        if(!ok) {
            qWarning() << "Couldn't create /data/prod " << mDataPath+PRODUCTION_ENVIRONMENT;
            return false;
        }
        qDebug() << "created directory /data/prod " << mDataPath+PRODUCTION_ENVIRONMENT;
    }
    exists = myDir.exists(mDataPath+TEST_ENVIRONMENT);
    if (!exists) {
        bool ok = myDir.mkpath(mDataPath+TEST_ENVIRONMENT);
        if(!ok) {
            qWarning() << "Couldn't create /data/test " << mDataPath+TEST_ENVIRONMENT;
            return false;
        }
        qDebug() << "created directory /data/test " << mDataPath+TEST_ENVIRONMENT;
    }
    return true;
}

/*
 * loads all data from cache.
 * called from main.qml with delay using QTimer
 * Data with 2PhaseInit Caching Policy will only
 * load priority records needed to resolve from others
 */
void DataManager::init()
{
    // get all from cache

    initConferenceFromCache();
    initBuildingFromCache();
    initFloorFromCache();
    initRoomFromCache();
    initSessionFromCache();
    initScheduleItemFromCache();
    initFavoriteFromCache();
    initBookmarkFromCache();
    // SessionLists is transient - not automatically read from cache
    initSessionLinkFromCache();
    initSpeakerFromCache();
    initSpeakerImageFromCache();
    initSessionTrackFromCache();
    initDayFromCache();
    // SessionAPI is transient - not automatically read from cache
    // PersonsAPI is transient - not automatically read from cache
    // SessionLinkAPI is transient - not automatically read from cache
    // SpeakerAPI is transient - not automatically read from cache
}



void DataManager::finish()
{
    saveSettings();
    // Conference is read-only - not saved to cache
    // Building is read-only - not saved to cache
    // Floor is read-only - not saved to cache
    // Room is read-only - not saved to cache
    // Session is read-only - not saved to cache
    // ScheduleItem is read-only - not saved to cache
    saveFavoriteToCache();
    saveBookmarkToCache();
    // SessionLists is read-only - not saved to cache
    // SessionLink is read-only - not saved to cache
    // Speaker is read-only - not saved to cache
    // SpeakerImage is read-only - not saved to cache
    // SessionTrack is read-only - not saved to cache
    // Day is read-only - not saved to cache
    // SessionAPI is read-only - not saved to cache
    // PersonsAPI is read-only - not saved to cache
    // SessionLinkAPI is read-only - not saved to cache
    // SpeakerAPI is read-only - not saved to cache
}







/**
 * creates a new SettingsData
 * parent is DataManager
 * if data is successfully entered you must insertSettingsData
 * if edit was canceled you must undoCreateSettingsData to free up memory
 */
SettingsData* DataManager::createSettingsData()
{
    SettingsData* settingsData;
    settingsData = new SettingsData();
    settingsData->setParent(this);
    settingsData->prepareNew();
    return settingsData;
}

/**
 * deletes SettingsData
 * if createSettingsData was canceled from UI
 * to delete a previous successfully inserted SettingsData
 * use deleteSettingsData
 */
void DataManager::undoCreateSettingsData(SettingsData* settingsData)
{
    if (settingsData) {
        qDebug() << "undoCreateSettingsData " << settingsData->id();
        settingsData->deleteLater();
        settingsData = 0;
    }
}




/*
 * reads Maps of Conference in from JSON cache
 * creates List of Conference*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initConferenceFromCache()
{
	qDebug() << "start initConferenceFromCache";
    mAllConference.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheConference);
    qDebug() << "read Conference from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Conference* conference = new Conference();
        // Important: DataManager must be parent of all root DTOs
        conference->setParent(this);
        conference->fillFromCacheMap(cacheMap);
        mAllConference.append(conference);
    }
    qDebug() << "created Conference* #" << mAllConference.size();
}


/*
 * save List of Conference* to JSON cache
 * convert list of Conference* to QVariantList
 * toCacheMap stores all properties without transient values
 * Conference is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveConferenceToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Conference* #" << mAllConference.size();
    for (int i = 0; i < mAllConference.size(); ++i) {
        Conference* conference;
        conference = (Conference*)mAllConference.at(i);
        QVariantMap cacheMap;
        cacheMap = conference->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Conference* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheConference, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Conference*> DataManager::listOfConferenceForKeys(
        QStringList keyList)
{
    QList<Conference*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllConference.size(); ++i) {
        Conference* conference;
        conference = (Conference*) mAllConference.at(i);
        if (keyList.contains(QString::number(conference->id()))) {
            listOfData.append(conference);
            keyList.removeOne(QString::number(conference->id()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Conference: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::conferenceAsQVariantList()
{
    QVariantList conferenceList;
    for (int i = 0; i < mAllConference.size(); ++i) {
        conferenceList.append(((Conference*) (mAllConference.at(i)))->toMap());
    }
    return conferenceList;
}

QList<QObject*> DataManager::allConference()
{
    return mAllConference;
}

QQmlListProperty<Conference> DataManager::conferencePropertyList()
{
    return QQmlListProperty<Conference>(this, 0,
            &DataManager::appendToConferenceProperty, &DataManager::conferencePropertyCount,
            &DataManager::atConferenceProperty, &DataManager::clearConferenceProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Conference*
void DataManager::appendToConferenceProperty(
        QQmlListProperty<Conference> *conferenceList,
        Conference* conference)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(conferenceList->object);
    if (dataManagerObject) {
        conference->setParent(dataManagerObject);
        dataManagerObject->mAllConference.append(conference);
        emit dataManagerObject->addedToAllConference(conference);
    } else {
        qWarning() << "cannot append Conference* to mAllConference "
                << "Object is not of type DataManager*";
    }
}
int DataManager::conferencePropertyCount(
        QQmlListProperty<Conference> *conferenceList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(conferenceList->object);
    if (dataManager) {
        return dataManager->mAllConference.size();
    } else {
        qWarning() << "cannot get size mAllConference " << "Object is not of type DataManager*";
    }
    return 0;
}
Conference* DataManager::atConferenceProperty(
        QQmlListProperty<Conference> *conferenceList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(conferenceList->object);
    if (dataManager) {
        if (dataManager->mAllConference.size() > pos) {
            return (Conference*) dataManager->mAllConference.at(pos);
        }
        qWarning() << "cannot get Conference* at pos " << pos << " size is "
                << dataManager->mAllConference.size();
    } else {
        qWarning() << "cannot get Conference* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearConferenceProperty(
        QQmlListProperty<Conference> *conferenceList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(conferenceList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllConference.size(); ++i) {
            Conference* conference;
            conference = (Conference*) dataManager->mAllConference.at(i);
			emit dataManager->deletedFromAllConferenceById(conference->id());
			emit dataManager->deletedFromAllConference(conference);
            conference->deleteLater();
            conference = 0;
        }
        dataManager->mAllConference.clear();
    } else {
        qWarning() << "cannot clear mAllConference " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Conference
 * and clears the list
 */
void DataManager::deleteConference()
{
    for (int i = 0; i < mAllConference.size(); ++i) {
        Conference* conference;
        conference = (Conference*) mAllConference.at(i);
        emit deletedFromAllConferenceById(conference->id());
		emit deletedFromAllConference(conference);
		emit conferencePropertyListChanged();
        conference->deleteLater();
        conference = 0;
     }
     mAllConference.clear();
}

/**
 * creates a new Conference
 * parent is DataManager
 * if data is successfully entered you must insertConference
 * if edit was canceled you must undoCreateConference to free up memory
 */
Conference* DataManager::createConference()
{
    Conference* conference;
    conference = new Conference();
    conference->setParent(this);
    conference->prepareNew();
    return conference;
}

/**
 * deletes Conference
 * if createConference was canceled from UI
 * to delete a previous successfully inserted Conference
 * use deleteConference
 */
void DataManager::undoCreateConference(Conference* conference)
{
    if (conference) {
        qDebug() << "undoCreateConference " << conference->id();
        conference->deleteLater();
        conference = 0;
    }
}

void DataManager::insertConference(Conference* conference)
{
    // Important: DataManager must be parent of all root DTOs
    conference->setParent(this);
    mAllConference.append(conference);
    emit addedToAllConference(conference);
    emit conferencePropertyListChanged();
}

void DataManager::insertConferenceFromMap(const QVariantMap& conferenceMap,
        const bool& useForeignProperties)
{
    Conference* conference = new Conference();
    conference->setParent(this);
    if (useForeignProperties) {
        conference->fillFromForeignMap(conferenceMap);
    } else {
        conference->fillFromMap(conferenceMap);
    }
    mAllConference.append(conference);
    emit addedToAllConference(conference);
    conferencePropertyListChanged();
}

bool DataManager::deleteConference(Conference* conference)
{
    bool ok = false;
    ok = mAllConference.removeOne(conference);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllConferenceById(conference->id());
    emit deletedFromAllConference(conference);
    emit conferencePropertyListChanged();
    conference->deleteLater();
    conference = 0;
    return ok;
}


bool DataManager::deleteConferenceById(const int& id)
{
    for (int i = 0; i < mAllConference.size(); ++i) {
        Conference* conference;
        conference = (Conference*) mAllConference.at(i);
        if (conference->id() == id) {
            mAllConference.removeAt(i);
            emit deletedFromAllConferenceById(id);
            emit deletedFromAllConference(conference);
            emit conferencePropertyListChanged();
            conference->deleteLater();
            conference = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Conference* DataManager::findConferenceById(const int& id){
    for (int i = 0; i < mAllConference.size(); ++i) {
        Conference* conference;
        conference = (Conference*)mAllConference.at(i);
        if(conference->id() == id){
            return conference;
        }
    }
    qDebug() << "no Conference found for id " << id;
    return 0;
}

/*
 * reads Maps of Building in from JSON cache
 * creates List of Building*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initBuildingFromCache()
{
	qDebug() << "start initBuildingFromCache";
    mAllBuilding.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheBuilding);
    qDebug() << "read Building from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Building* building = new Building();
        // Important: DataManager must be parent of all root DTOs
        building->setParent(this);
        building->fillFromCacheMap(cacheMap);
        mAllBuilding.append(building);
    }
    qDebug() << "created Building* #" << mAllBuilding.size();
}


/*
 * save List of Building* to JSON cache
 * convert list of Building* to QVariantList
 * toCacheMap stores all properties without transient values
 * Building is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveBuildingToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Building* #" << mAllBuilding.size();
    for (int i = 0; i < mAllBuilding.size(); ++i) {
        Building* building;
        building = (Building*)mAllBuilding.at(i);
        QVariantMap cacheMap;
        cacheMap = building->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Building* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheBuilding, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Building*> DataManager::listOfBuildingForKeys(
        QStringList keyList)
{
    QList<Building*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllBuilding.size(); ++i) {
        Building* building;
        building = (Building*) mAllBuilding.at(i);
        if (keyList.contains(QString::number(building->buildingId()))) {
            listOfData.append(building);
            keyList.removeOne(QString::number(building->buildingId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Building: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::buildingAsQVariantList()
{
    QVariantList buildingList;
    for (int i = 0; i < mAllBuilding.size(); ++i) {
        buildingList.append(((Building*) (mAllBuilding.at(i)))->toMap());
    }
    return buildingList;
}

QList<QObject*> DataManager::allBuilding()
{
    return mAllBuilding;
}

QQmlListProperty<Building> DataManager::buildingPropertyList()
{
    return QQmlListProperty<Building>(this, 0,
            &DataManager::appendToBuildingProperty, &DataManager::buildingPropertyCount,
            &DataManager::atBuildingProperty, &DataManager::clearBuildingProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Building*
void DataManager::appendToBuildingProperty(
        QQmlListProperty<Building> *buildingList,
        Building* building)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(buildingList->object);
    if (dataManagerObject) {
        building->setParent(dataManagerObject);
        dataManagerObject->mAllBuilding.append(building);
        emit dataManagerObject->addedToAllBuilding(building);
    } else {
        qWarning() << "cannot append Building* to mAllBuilding "
                << "Object is not of type DataManager*";
    }
}
int DataManager::buildingPropertyCount(
        QQmlListProperty<Building> *buildingList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(buildingList->object);
    if (dataManager) {
        return dataManager->mAllBuilding.size();
    } else {
        qWarning() << "cannot get size mAllBuilding " << "Object is not of type DataManager*";
    }
    return 0;
}
Building* DataManager::atBuildingProperty(
        QQmlListProperty<Building> *buildingList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(buildingList->object);
    if (dataManager) {
        if (dataManager->mAllBuilding.size() > pos) {
            return (Building*) dataManager->mAllBuilding.at(pos);
        }
        qWarning() << "cannot get Building* at pos " << pos << " size is "
                << dataManager->mAllBuilding.size();
    } else {
        qWarning() << "cannot get Building* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearBuildingProperty(
        QQmlListProperty<Building> *buildingList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(buildingList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllBuilding.size(); ++i) {
            Building* building;
            building = (Building*) dataManager->mAllBuilding.at(i);
			emit dataManager->deletedFromAllBuildingByBuildingId(building->buildingId());
			emit dataManager->deletedFromAllBuilding(building);
            building->deleteLater();
            building = 0;
        }
        dataManager->mAllBuilding.clear();
    } else {
        qWarning() << "cannot clear mAllBuilding " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Building
 * and clears the list
 */
void DataManager::deleteBuilding()
{
    for (int i = 0; i < mAllBuilding.size(); ++i) {
        Building* building;
        building = (Building*) mAllBuilding.at(i);
        emit deletedFromAllBuildingByBuildingId(building->buildingId());
		emit deletedFromAllBuilding(building);
		emit buildingPropertyListChanged();
        building->deleteLater();
        building = 0;
     }
     mAllBuilding.clear();
}

/**
 * creates a new Building
 * parent is DataManager
 * if data is successfully entered you must insertBuilding
 * if edit was canceled you must undoCreateBuilding to free up memory
 */
Building* DataManager::createBuilding()
{
    Building* building;
    building = new Building();
    building->setParent(this);
    building->prepareNew();
    return building;
}

/**
 * deletes Building
 * if createBuilding was canceled from UI
 * to delete a previous successfully inserted Building
 * use deleteBuilding
 */
void DataManager::undoCreateBuilding(Building* building)
{
    if (building) {
        qDebug() << "undoCreateBuilding " << building->buildingId();
        building->deleteLater();
        building = 0;
    }
}

void DataManager::insertBuilding(Building* building)
{
    // Important: DataManager must be parent of all root DTOs
    building->setParent(this);
    mAllBuilding.append(building);
    emit addedToAllBuilding(building);
    emit buildingPropertyListChanged();
}

void DataManager::insertBuildingFromMap(const QVariantMap& buildingMap,
        const bool& useForeignProperties)
{
    Building* building = new Building();
    building->setParent(this);
    if (useForeignProperties) {
        building->fillFromForeignMap(buildingMap);
    } else {
        building->fillFromMap(buildingMap);
    }
    mAllBuilding.append(building);
    emit addedToAllBuilding(building);
    buildingPropertyListChanged();
}

bool DataManager::deleteBuilding(Building* building)
{
    bool ok = false;
    ok = mAllBuilding.removeOne(building);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllBuildingByBuildingId(building->buildingId());
    emit deletedFromAllBuilding(building);
    emit buildingPropertyListChanged();
    building->deleteLater();
    building = 0;
    return ok;
}


bool DataManager::deleteBuildingByBuildingId(const int& buildingId)
{
    for (int i = 0; i < mAllBuilding.size(); ++i) {
        Building* building;
        building = (Building*) mAllBuilding.at(i);
        if (building->buildingId() == buildingId) {
            mAllBuilding.removeAt(i);
            emit deletedFromAllBuildingByBuildingId(buildingId);
            emit deletedFromAllBuilding(building);
            emit buildingPropertyListChanged();
            building->deleteLater();
            building = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Building* DataManager::findBuildingByBuildingId(const int& buildingId){
    for (int i = 0; i < mAllBuilding.size(); ++i) {
        Building* building;
        building = (Building*)mAllBuilding.at(i);
        if(building->buildingId() == buildingId){
            return building;
        }
    }
    qDebug() << "no Building found for buildingId " << buildingId;
    return 0;
}

/*
 * reads Maps of Floor in from JSON cache
 * creates List of Floor*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initFloorFromCache()
{
	qDebug() << "start initFloorFromCache";
    mAllFloor.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheFloor);
    qDebug() << "read Floor from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Floor* floor = new Floor();
        // Important: DataManager must be parent of all root DTOs
        floor->setParent(this);
        floor->fillFromCacheMap(cacheMap);
        mAllFloor.append(floor);
    }
    qDebug() << "created Floor* #" << mAllFloor.size();
}


/*
 * save List of Floor* to JSON cache
 * convert list of Floor* to QVariantList
 * toCacheMap stores all properties without transient values
 * Floor is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveFloorToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Floor* #" << mAllFloor.size();
    for (int i = 0; i < mAllFloor.size(); ++i) {
        Floor* floor;
        floor = (Floor*)mAllFloor.at(i);
        QVariantMap cacheMap;
        cacheMap = floor->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Floor* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheFloor, cacheList);
}


void DataManager::resolveFloorReferences(Floor* floor)
{
	if (!floor) {
        qDebug() << "cannot resolveFloorReferences with floor NULL";
        return;
    }
    if(floor->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (floor->hasBuilding() && !floor->isBuildingResolvedAsDataObject()) {
    	Building* building;
   		building = findBuildingByBuildingId(floor->building());
    	if (building) {
    		floor->resolveBuildingAsDataObject(building);
    	} else {
    		qDebug() << "markBuildingAsInvalid: " << floor->building();
    		floor->markBuildingAsInvalid();
    	}
    }
    if (!floor->areRoomsKeysResolved()) {
        floor->resolveRoomsKeys(
                listOfRoomForKeys(floor->roomsKeys()));
    }
}

void DataManager::resolveReferencesForAllFloor()
{
    for (int i = 0; i < mAllFloor.size(); ++i) {
        Floor* floor;
        floor = (Floor*)mAllFloor.at(i);
    	resolveFloorReferences(floor);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Floor*> DataManager::listOfFloorForKeys(
        QStringList keyList)
{
    QList<Floor*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllFloor.size(); ++i) {
        Floor* floor;
        floor = (Floor*) mAllFloor.at(i);
        if (keyList.contains(QString::number(floor->floorId()))) {
            listOfData.append(floor);
            keyList.removeOne(QString::number(floor->floorId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Floor: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::floorAsQVariantList()
{
    QVariantList floorList;
    for (int i = 0; i < mAllFloor.size(); ++i) {
        floorList.append(((Floor*) (mAllFloor.at(i)))->toMap());
    }
    return floorList;
}

QList<QObject*> DataManager::allFloor()
{
    return mAllFloor;
}

QQmlListProperty<Floor> DataManager::floorPropertyList()
{
    return QQmlListProperty<Floor>(this, 0,
            &DataManager::appendToFloorProperty, &DataManager::floorPropertyCount,
            &DataManager::atFloorProperty, &DataManager::clearFloorProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Floor*
void DataManager::appendToFloorProperty(
        QQmlListProperty<Floor> *floorList,
        Floor* floor)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(floorList->object);
    if (dataManagerObject) {
        floor->setParent(dataManagerObject);
        dataManagerObject->mAllFloor.append(floor);
        emit dataManagerObject->addedToAllFloor(floor);
    } else {
        qWarning() << "cannot append Floor* to mAllFloor "
                << "Object is not of type DataManager*";
    }
}
int DataManager::floorPropertyCount(
        QQmlListProperty<Floor> *floorList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(floorList->object);
    if (dataManager) {
        return dataManager->mAllFloor.size();
    } else {
        qWarning() << "cannot get size mAllFloor " << "Object is not of type DataManager*";
    }
    return 0;
}
Floor* DataManager::atFloorProperty(
        QQmlListProperty<Floor> *floorList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(floorList->object);
    if (dataManager) {
        if (dataManager->mAllFloor.size() > pos) {
            return (Floor*) dataManager->mAllFloor.at(pos);
        }
        qWarning() << "cannot get Floor* at pos " << pos << " size is "
                << dataManager->mAllFloor.size();
    } else {
        qWarning() << "cannot get Floor* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearFloorProperty(
        QQmlListProperty<Floor> *floorList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(floorList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllFloor.size(); ++i) {
            Floor* floor;
            floor = (Floor*) dataManager->mAllFloor.at(i);
			emit dataManager->deletedFromAllFloorByFloorId(floor->floorId());
			emit dataManager->deletedFromAllFloor(floor);
            floor->deleteLater();
            floor = 0;
        }
        dataManager->mAllFloor.clear();
    } else {
        qWarning() << "cannot clear mAllFloor " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Floor
 * and clears the list
 */
void DataManager::deleteFloor()
{
    for (int i = 0; i < mAllFloor.size(); ++i) {
        Floor* floor;
        floor = (Floor*) mAllFloor.at(i);
        emit deletedFromAllFloorByFloorId(floor->floorId());
		emit deletedFromAllFloor(floor);
		emit floorPropertyListChanged();
        floor->deleteLater();
        floor = 0;
     }
     mAllFloor.clear();
}

/**
 * creates a new Floor
 * parent is DataManager
 * if data is successfully entered you must insertFloor
 * if edit was canceled you must undoCreateFloor to free up memory
 */
Floor* DataManager::createFloor()
{
    Floor* floor;
    floor = new Floor();
    floor->setParent(this);
    floor->prepareNew();
    return floor;
}

/**
 * deletes Floor
 * if createFloor was canceled from UI
 * to delete a previous successfully inserted Floor
 * use deleteFloor
 */
void DataManager::undoCreateFloor(Floor* floor)
{
    if (floor) {
        qDebug() << "undoCreateFloor " << floor->floorId();
        floor->deleteLater();
        floor = 0;
    }
}

void DataManager::insertFloor(Floor* floor)
{
    // Important: DataManager must be parent of all root DTOs
    floor->setParent(this);
    mAllFloor.append(floor);
    emit addedToAllFloor(floor);
    emit floorPropertyListChanged();
}

void DataManager::insertFloorFromMap(const QVariantMap& floorMap,
        const bool& useForeignProperties)
{
    Floor* floor = new Floor();
    floor->setParent(this);
    if (useForeignProperties) {
        floor->fillFromForeignMap(floorMap);
    } else {
        floor->fillFromMap(floorMap);
    }
    mAllFloor.append(floor);
    emit addedToAllFloor(floor);
    floorPropertyListChanged();
}

bool DataManager::deleteFloor(Floor* floor)
{
    bool ok = false;
    ok = mAllFloor.removeOne(floor);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllFloorByFloorId(floor->floorId());
    emit deletedFromAllFloor(floor);
    emit floorPropertyListChanged();
    floor->deleteLater();
    floor = 0;
    return ok;
}


bool DataManager::deleteFloorByFloorId(const int& floorId)
{
    for (int i = 0; i < mAllFloor.size(); ++i) {
        Floor* floor;
        floor = (Floor*) mAllFloor.at(i);
        if (floor->floorId() == floorId) {
            mAllFloor.removeAt(i);
            emit deletedFromAllFloorByFloorId(floorId);
            emit deletedFromAllFloor(floor);
            emit floorPropertyListChanged();
            floor->deleteLater();
            floor = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Floor* DataManager::findFloorByFloorId(const int& floorId){
    for (int i = 0; i < mAllFloor.size(); ++i) {
        Floor* floor;
        floor = (Floor*)mAllFloor.at(i);
        if(floor->floorId() == floorId){
            return floor;
        }
    }
    qDebug() << "no Floor found for floorId " << floorId;
    return 0;
}

/*
 * reads Maps of Room in from JSON cache
 * creates List of Room*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initRoomFromCache()
{
	qDebug() << "start initRoomFromCache";
    mAllRoom.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheRoom);
    qDebug() << "read Room from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Room* room = new Room();
        // Important: DataManager must be parent of all root DTOs
        room->setParent(this);
        room->fillFromCacheMap(cacheMap);
        mAllRoom.append(room);
    }
    qDebug() << "created Room* #" << mAllRoom.size();
}


/*
 * save List of Room* to JSON cache
 * convert list of Room* to QVariantList
 * toCacheMap stores all properties without transient values
 * Room is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveRoomToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Room* #" << mAllRoom.size();
    for (int i = 0; i < mAllRoom.size(); ++i) {
        Room* room;
        room = (Room*)mAllRoom.at(i);
        QVariantMap cacheMap;
        cacheMap = room->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Room* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheRoom, cacheList);
}


void DataManager::resolveRoomReferences(Room* room)
{
	if (!room) {
        qDebug() << "cannot resolveRoomReferences with room NULL";
        return;
    }
    if(room->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (room->hasFloor() && !room->isFloorResolvedAsDataObject()) {
    	Floor* floor;
   		floor = findFloorByFloorId(room->floor());
    	if (floor) {
    		room->resolveFloorAsDataObject(floor);
    	} else {
    		qDebug() << "markFloorAsInvalid: " << room->floor();
    		room->markFloorAsInvalid();
    	}
    }
    if (!room->areSessionsKeysResolved()) {
        room->resolveSessionsKeys(
                listOfSessionForKeys(room->sessionsKeys()));
    }
}

void DataManager::resolveReferencesForAllRoom()
{
    for (int i = 0; i < mAllRoom.size(); ++i) {
        Room* room;
        room = (Room*)mAllRoom.at(i);
    	resolveRoomReferences(room);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Room*> DataManager::listOfRoomForKeys(
        QStringList keyList)
{
    QList<Room*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllRoom.size(); ++i) {
        Room* room;
        room = (Room*) mAllRoom.at(i);
        if (keyList.contains(QString::number(room->roomId()))) {
            listOfData.append(room);
            keyList.removeOne(QString::number(room->roomId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Room: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::roomAsQVariantList()
{
    QVariantList roomList;
    for (int i = 0; i < mAllRoom.size(); ++i) {
        roomList.append(((Room*) (mAllRoom.at(i)))->toMap());
    }
    return roomList;
}

QList<QObject*> DataManager::allRoom()
{
    return mAllRoom;
}

QQmlListProperty<Room> DataManager::roomPropertyList()
{
    return QQmlListProperty<Room>(this, 0,
            &DataManager::appendToRoomProperty, &DataManager::roomPropertyCount,
            &DataManager::atRoomProperty, &DataManager::clearRoomProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Room*
void DataManager::appendToRoomProperty(
        QQmlListProperty<Room> *roomList,
        Room* room)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(roomList->object);
    if (dataManagerObject) {
        room->setParent(dataManagerObject);
        dataManagerObject->mAllRoom.append(room);
        emit dataManagerObject->addedToAllRoom(room);
    } else {
        qWarning() << "cannot append Room* to mAllRoom "
                << "Object is not of type DataManager*";
    }
}
int DataManager::roomPropertyCount(
        QQmlListProperty<Room> *roomList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(roomList->object);
    if (dataManager) {
        return dataManager->mAllRoom.size();
    } else {
        qWarning() << "cannot get size mAllRoom " << "Object is not of type DataManager*";
    }
    return 0;
}
Room* DataManager::atRoomProperty(
        QQmlListProperty<Room> *roomList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(roomList->object);
    if (dataManager) {
        if (dataManager->mAllRoom.size() > pos) {
            return (Room*) dataManager->mAllRoom.at(pos);
        }
        qWarning() << "cannot get Room* at pos " << pos << " size is "
                << dataManager->mAllRoom.size();
    } else {
        qWarning() << "cannot get Room* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearRoomProperty(
        QQmlListProperty<Room> *roomList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(roomList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllRoom.size(); ++i) {
            Room* room;
            room = (Room*) dataManager->mAllRoom.at(i);
			emit dataManager->deletedFromAllRoomByRoomId(room->roomId());
			emit dataManager->deletedFromAllRoom(room);
            room->deleteLater();
            room = 0;
        }
        dataManager->mAllRoom.clear();
    } else {
        qWarning() << "cannot clear mAllRoom " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Room
 * and clears the list
 */
void DataManager::deleteRoom()
{
    for (int i = 0; i < mAllRoom.size(); ++i) {
        Room* room;
        room = (Room*) mAllRoom.at(i);
        emit deletedFromAllRoomByRoomId(room->roomId());
		emit deletedFromAllRoom(room);
		emit roomPropertyListChanged();
        room->deleteLater();
        room = 0;
     }
     mAllRoom.clear();
}

/**
 * creates a new Room
 * parent is DataManager
 * if data is successfully entered you must insertRoom
 * if edit was canceled you must undoCreateRoom to free up memory
 */
Room* DataManager::createRoom()
{
    Room* room;
    room = new Room();
    room->setParent(this);
    room->prepareNew();
    return room;
}

/**
 * deletes Room
 * if createRoom was canceled from UI
 * to delete a previous successfully inserted Room
 * use deleteRoom
 */
void DataManager::undoCreateRoom(Room* room)
{
    if (room) {
        qDebug() << "undoCreateRoom " << room->roomId();
        room->deleteLater();
        room = 0;
    }
}

void DataManager::insertRoom(Room* room)
{
    // Important: DataManager must be parent of all root DTOs
    room->setParent(this);
    mAllRoom.append(room);
    emit addedToAllRoom(room);
    emit roomPropertyListChanged();
}

void DataManager::insertRoomFromMap(const QVariantMap& roomMap,
        const bool& useForeignProperties)
{
    Room* room = new Room();
    room->setParent(this);
    if (useForeignProperties) {
        room->fillFromForeignMap(roomMap);
    } else {
        room->fillFromMap(roomMap);
    }
    mAllRoom.append(room);
    emit addedToAllRoom(room);
    roomPropertyListChanged();
}

bool DataManager::deleteRoom(Room* room)
{
    bool ok = false;
    ok = mAllRoom.removeOne(room);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllRoomByRoomId(room->roomId());
    emit deletedFromAllRoom(room);
    emit roomPropertyListChanged();
    room->deleteLater();
    room = 0;
    return ok;
}


bool DataManager::deleteRoomByRoomId(const int& roomId)
{
    for (int i = 0; i < mAllRoom.size(); ++i) {
        Room* room;
        room = (Room*) mAllRoom.at(i);
        if (room->roomId() == roomId) {
            mAllRoom.removeAt(i);
            emit deletedFromAllRoomByRoomId(roomId);
            emit deletedFromAllRoom(room);
            emit roomPropertyListChanged();
            room->deleteLater();
            room = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Room* DataManager::findRoomByRoomId(const int& roomId){
    for (int i = 0; i < mAllRoom.size(); ++i) {
        Room* room;
        room = (Room*)mAllRoom.at(i);
        if(room->roomId() == roomId){
            return room;
        }
    }
    qDebug() << "no Room found for roomId " << roomId;
    return 0;
}

/*
 * reads Maps of Session in from JSON cache
 * creates List of Session*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initSessionFromCache()
{
	qDebug() << "start initSessionFromCache";
    mAllSession.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheSession);
    qDebug() << "read Session from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Session* session = new Session();
        // Important: DataManager must be parent of all root DTOs
        session->setParent(this);
        session->fillFromCacheMap(cacheMap);
        mAllSession.append(session);
    }
    qDebug() << "created Session* #" << mAllSession.size();
}


/*
 * save List of Session* to JSON cache
 * convert list of Session* to QVariantList
 * toCacheMap stores all properties without transient values
 * Session is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveSessionToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Session* #" << mAllSession.size();
    for (int i = 0; i < mAllSession.size(); ++i) {
        Session* session;
        session = (Session*)mAllSession.at(i);
        QVariantMap cacheMap;
        cacheMap = session->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Session* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheSession, cacheList);
}


void DataManager::resolveSessionReferences(Session* session)
{
	if (!session) {
        qDebug() << "cannot resolveSessionReferences with session NULL";
        return;
    }
    if(session->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (session->hasSessionDay() && !session->isSessionDayResolvedAsDataObject()) {
    	Day* sessionDay;
   		sessionDay = findDayById(session->sessionDay());
    	if (sessionDay) {
    		session->resolveSessionDayAsDataObject(sessionDay);
    	} else {
    		qDebug() << "markSessionDayAsInvalid: " << session->sessionDay();
    		session->markSessionDayAsInvalid();
    	}
    }
    if (session->hasRoom() && !session->isRoomResolvedAsDataObject()) {
    	Room* room;
   		room = findRoomByRoomId(session->room());
    	if (room) {
    		session->resolveRoomAsDataObject(room);
    	} else {
    		qDebug() << "markRoomAsInvalid: " << session->room();
    		session->markRoomAsInvalid();
    	}
    }
    if (session->hasSessionTrack() && !session->isSessionTrackResolvedAsDataObject()) {
    	SessionTrack* sessionTrack;
   		sessionTrack = findSessionTrackByTrackId(session->sessionTrack());
    	if (sessionTrack) {
    		session->resolveSessionTrackAsDataObject(sessionTrack);
    	} else {
    		qDebug() << "markSessionTrackAsInvalid: " << session->sessionTrack();
    		session->markSessionTrackAsInvalid();
    	}
    }
    if (session->hasScheduleItem() && !session->isScheduleItemResolvedAsDataObject()) {
    	ScheduleItem* scheduleItem;
   		scheduleItem = findScheduleItemBySessionId(session->scheduleItem());
    	if (scheduleItem) {
    		session->resolveScheduleItemAsDataObject(scheduleItem);
    	} else {
    		qDebug() << "markScheduleItemAsInvalid: " << session->scheduleItem();
    		session->markScheduleItemAsInvalid();
    	}
    }
    if (!session->arePresenterKeysResolved()) {
        session->resolvePresenterKeys(
                listOfSpeakerForKeys(session->presenterKeys()));
    }
    if (!session->areSessionLinksKeysResolved()) {
        session->resolveSessionLinksKeys(
                listOfSessionLinkForKeys(session->sessionLinksKeys()));
    }
}

void DataManager::resolveReferencesForAllSession()
{
    for (int i = 0; i < mAllSession.size(); ++i) {
        Session* session;
        session = (Session*)mAllSession.at(i);
    	resolveSessionReferences(session);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Session*> DataManager::listOfSessionForKeys(
        QStringList keyList)
{
    QList<Session*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllSession.size(); ++i) {
        Session* session;
        session = (Session*) mAllSession.at(i);
        if (keyList.contains(QString::number(session->sessionId()))) {
            listOfData.append(session);
            keyList.removeOne(QString::number(session->sessionId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Session: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::sessionAsQVariantList()
{
    QVariantList sessionList;
    for (int i = 0; i < mAllSession.size(); ++i) {
        sessionList.append(((Session*) (mAllSession.at(i)))->toMap());
    }
    return sessionList;
}

QList<QObject*> DataManager::allSession()
{
    return mAllSession;
}

QQmlListProperty<Session> DataManager::sessionPropertyList()
{
    return QQmlListProperty<Session>(this, 0,
            &DataManager::appendToSessionProperty, &DataManager::sessionPropertyCount,
            &DataManager::atSessionProperty, &DataManager::clearSessionProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Session*
void DataManager::appendToSessionProperty(
        QQmlListProperty<Session> *sessionList,
        Session* session)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(sessionList->object);
    if (dataManagerObject) {
        session->setParent(dataManagerObject);
        dataManagerObject->mAllSession.append(session);
        emit dataManagerObject->addedToAllSession(session);
    } else {
        qWarning() << "cannot append Session* to mAllSession "
                << "Object is not of type DataManager*";
    }
}
int DataManager::sessionPropertyCount(
        QQmlListProperty<Session> *sessionList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionList->object);
    if (dataManager) {
        return dataManager->mAllSession.size();
    } else {
        qWarning() << "cannot get size mAllSession " << "Object is not of type DataManager*";
    }
    return 0;
}
Session* DataManager::atSessionProperty(
        QQmlListProperty<Session> *sessionList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionList->object);
    if (dataManager) {
        if (dataManager->mAllSession.size() > pos) {
            return (Session*) dataManager->mAllSession.at(pos);
        }
        qWarning() << "cannot get Session* at pos " << pos << " size is "
                << dataManager->mAllSession.size();
    } else {
        qWarning() << "cannot get Session* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearSessionProperty(
        QQmlListProperty<Session> *sessionList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllSession.size(); ++i) {
            Session* session;
            session = (Session*) dataManager->mAllSession.at(i);
			emit dataManager->deletedFromAllSessionBySessionId(session->sessionId());
			emit dataManager->deletedFromAllSession(session);
            session->deleteLater();
            session = 0;
        }
        dataManager->mAllSession.clear();
    } else {
        qWarning() << "cannot clear mAllSession " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Session
 * and clears the list
 */
void DataManager::deleteSession()
{
    for (int i = 0; i < mAllSession.size(); ++i) {
        Session* session;
        session = (Session*) mAllSession.at(i);
        emit deletedFromAllSessionBySessionId(session->sessionId());
		emit deletedFromAllSession(session);
		emit sessionPropertyListChanged();
        session->deleteLater();
        session = 0;
     }
     mAllSession.clear();
}

/**
 * creates a new Session
 * parent is DataManager
 * if data is successfully entered you must insertSession
 * if edit was canceled you must undoCreateSession to free up memory
 */
Session* DataManager::createSession()
{
    Session* session;
    session = new Session();
    session->setParent(this);
    session->prepareNew();
    return session;
}

/**
 * deletes Session
 * if createSession was canceled from UI
 * to delete a previous successfully inserted Session
 * use deleteSession
 */
void DataManager::undoCreateSession(Session* session)
{
    if (session) {
        qDebug() << "undoCreateSession " << session->sessionId();
        session->deleteLater();
        session = 0;
    }
}

void DataManager::insertSession(Session* session)
{
    // Important: DataManager must be parent of all root DTOs
    session->setParent(this);
    mAllSession.append(session);
    emit addedToAllSession(session);
    emit sessionPropertyListChanged();
}

void DataManager::insertSessionFromMap(const QVariantMap& sessionMap,
        const bool& useForeignProperties)
{
    Session* session = new Session();
    session->setParent(this);
    if (useForeignProperties) {
        session->fillFromForeignMap(sessionMap);
    } else {
        session->fillFromMap(sessionMap);
    }
    mAllSession.append(session);
    emit addedToAllSession(session);
    sessionPropertyListChanged();
}

bool DataManager::deleteSession(Session* session)
{
    bool ok = false;
    ok = mAllSession.removeOne(session);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllSessionBySessionId(session->sessionId());
    emit deletedFromAllSession(session);
    emit sessionPropertyListChanged();
    session->deleteLater();
    session = 0;
    return ok;
}


bool DataManager::deleteSessionBySessionId(const int& sessionId)
{
    for (int i = 0; i < mAllSession.size(); ++i) {
        Session* session;
        session = (Session*) mAllSession.at(i);
        if (session->sessionId() == sessionId) {
            mAllSession.removeAt(i);
            emit deletedFromAllSessionBySessionId(sessionId);
            emit deletedFromAllSession(session);
            emit sessionPropertyListChanged();
            session->deleteLater();
            session = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Session* DataManager::findSessionBySessionId(const int& sessionId){
    for (int i = 0; i < mAllSession.size(); ++i) {
        Session* session;
        session = (Session*)mAllSession.at(i);
        if(session->sessionId() == sessionId){
            return session;
        }
    }
    qDebug() << "no Session found for sessionId " << sessionId;
    return 0;
}

/*
 * reads Maps of ScheduleItem in from JSON cache
 * creates List of ScheduleItem*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initScheduleItemFromCache()
{
	qDebug() << "start initScheduleItemFromCache";
    mAllScheduleItem.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheScheduleItem);
    qDebug() << "read ScheduleItem from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        ScheduleItem* scheduleItem = new ScheduleItem();
        // Important: DataManager must be parent of all root DTOs
        scheduleItem->setParent(this);
        scheduleItem->fillFromCacheMap(cacheMap);
        mAllScheduleItem.append(scheduleItem);
    }
    qDebug() << "created ScheduleItem* #" << mAllScheduleItem.size();
}


/*
 * save List of ScheduleItem* to JSON cache
 * convert list of ScheduleItem* to QVariantList
 * toCacheMap stores all properties without transient values
 * ScheduleItem is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveScheduleItemToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching ScheduleItem* #" << mAllScheduleItem.size();
    for (int i = 0; i < mAllScheduleItem.size(); ++i) {
        ScheduleItem* scheduleItem;
        scheduleItem = (ScheduleItem*)mAllScheduleItem.at(i);
        QVariantMap cacheMap;
        cacheMap = scheduleItem->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "ScheduleItem* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheScheduleItem, cacheList);
}


void DataManager::resolveScheduleItemReferences(ScheduleItem* scheduleItem)
{
	if (!scheduleItem) {
        qDebug() << "cannot resolveScheduleItemReferences with scheduleItem NULL";
        return;
    }
    if(scheduleItem->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (scheduleItem->hasSession() && !scheduleItem->isSessionResolvedAsDataObject()) {
    	Session* session;
   		session = findSessionBySessionId(scheduleItem->session());
    	if (session) {
    		scheduleItem->resolveSessionAsDataObject(session);
    	} else {
    		qDebug() << "markSessionAsInvalid: " << scheduleItem->session();
    		scheduleItem->markSessionAsInvalid();
    	}
    }
}

void DataManager::resolveReferencesForAllScheduleItem()
{
    for (int i = 0; i < mAllScheduleItem.size(); ++i) {
        ScheduleItem* scheduleItem;
        scheduleItem = (ScheduleItem*)mAllScheduleItem.at(i);
    	resolveScheduleItemReferences(scheduleItem);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<ScheduleItem*> DataManager::listOfScheduleItemForKeys(
        QStringList keyList)
{
    QList<ScheduleItem*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllScheduleItem.size(); ++i) {
        ScheduleItem* scheduleItem;
        scheduleItem = (ScheduleItem*) mAllScheduleItem.at(i);
        if (keyList.contains(QString::number(scheduleItem->sessionId()))) {
            listOfData.append(scheduleItem);
            keyList.removeOne(QString::number(scheduleItem->sessionId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for ScheduleItem: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::scheduleItemAsQVariantList()
{
    QVariantList scheduleItemList;
    for (int i = 0; i < mAllScheduleItem.size(); ++i) {
        scheduleItemList.append(((ScheduleItem*) (mAllScheduleItem.at(i)))->toMap());
    }
    return scheduleItemList;
}

QList<QObject*> DataManager::allScheduleItem()
{
    return mAllScheduleItem;
}

QQmlListProperty<ScheduleItem> DataManager::scheduleItemPropertyList()
{
    return QQmlListProperty<ScheduleItem>(this, 0,
            &DataManager::appendToScheduleItemProperty, &DataManager::scheduleItemPropertyCount,
            &DataManager::atScheduleItemProperty, &DataManager::clearScheduleItemProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of ScheduleItem*
void DataManager::appendToScheduleItemProperty(
        QQmlListProperty<ScheduleItem> *scheduleItemList,
        ScheduleItem* scheduleItem)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(scheduleItemList->object);
    if (dataManagerObject) {
        scheduleItem->setParent(dataManagerObject);
        dataManagerObject->mAllScheduleItem.append(scheduleItem);
        emit dataManagerObject->addedToAllScheduleItem(scheduleItem);
    } else {
        qWarning() << "cannot append ScheduleItem* to mAllScheduleItem "
                << "Object is not of type DataManager*";
    }
}
int DataManager::scheduleItemPropertyCount(
        QQmlListProperty<ScheduleItem> *scheduleItemList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(scheduleItemList->object);
    if (dataManager) {
        return dataManager->mAllScheduleItem.size();
    } else {
        qWarning() << "cannot get size mAllScheduleItem " << "Object is not of type DataManager*";
    }
    return 0;
}
ScheduleItem* DataManager::atScheduleItemProperty(
        QQmlListProperty<ScheduleItem> *scheduleItemList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(scheduleItemList->object);
    if (dataManager) {
        if (dataManager->mAllScheduleItem.size() > pos) {
            return (ScheduleItem*) dataManager->mAllScheduleItem.at(pos);
        }
        qWarning() << "cannot get ScheduleItem* at pos " << pos << " size is "
                << dataManager->mAllScheduleItem.size();
    } else {
        qWarning() << "cannot get ScheduleItem* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearScheduleItemProperty(
        QQmlListProperty<ScheduleItem> *scheduleItemList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(scheduleItemList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllScheduleItem.size(); ++i) {
            ScheduleItem* scheduleItem;
            scheduleItem = (ScheduleItem*) dataManager->mAllScheduleItem.at(i);
			emit dataManager->deletedFromAllScheduleItemBySessionId(scheduleItem->sessionId());
			emit dataManager->deletedFromAllScheduleItem(scheduleItem);
            scheduleItem->deleteLater();
            scheduleItem = 0;
        }
        dataManager->mAllScheduleItem.clear();
    } else {
        qWarning() << "cannot clear mAllScheduleItem " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all ScheduleItem
 * and clears the list
 */
void DataManager::deleteScheduleItem()
{
    for (int i = 0; i < mAllScheduleItem.size(); ++i) {
        ScheduleItem* scheduleItem;
        scheduleItem = (ScheduleItem*) mAllScheduleItem.at(i);
        emit deletedFromAllScheduleItemBySessionId(scheduleItem->sessionId());
		emit deletedFromAllScheduleItem(scheduleItem);
		emit scheduleItemPropertyListChanged();
        scheduleItem->deleteLater();
        scheduleItem = 0;
     }
     mAllScheduleItem.clear();
}

/**
 * creates a new ScheduleItem
 * parent is DataManager
 * if data is successfully entered you must insertScheduleItem
 * if edit was canceled you must undoCreateScheduleItem to free up memory
 */
ScheduleItem* DataManager::createScheduleItem()
{
    ScheduleItem* scheduleItem;
    scheduleItem = new ScheduleItem();
    scheduleItem->setParent(this);
    scheduleItem->prepareNew();
    return scheduleItem;
}

/**
 * deletes ScheduleItem
 * if createScheduleItem was canceled from UI
 * to delete a previous successfully inserted ScheduleItem
 * use deleteScheduleItem
 */
void DataManager::undoCreateScheduleItem(ScheduleItem* scheduleItem)
{
    if (scheduleItem) {
        qDebug() << "undoCreateScheduleItem " << scheduleItem->sessionId();
        scheduleItem->deleteLater();
        scheduleItem = 0;
    }
}

void DataManager::insertScheduleItem(ScheduleItem* scheduleItem)
{
    // Important: DataManager must be parent of all root DTOs
    scheduleItem->setParent(this);
    mAllScheduleItem.append(scheduleItem);
    emit addedToAllScheduleItem(scheduleItem);
    emit scheduleItemPropertyListChanged();
}

void DataManager::insertScheduleItemFromMap(const QVariantMap& scheduleItemMap,
        const bool& useForeignProperties)
{
    ScheduleItem* scheduleItem = new ScheduleItem();
    scheduleItem->setParent(this);
    if (useForeignProperties) {
        scheduleItem->fillFromForeignMap(scheduleItemMap);
    } else {
        scheduleItem->fillFromMap(scheduleItemMap);
    }
    mAllScheduleItem.append(scheduleItem);
    emit addedToAllScheduleItem(scheduleItem);
    scheduleItemPropertyListChanged();
}

bool DataManager::deleteScheduleItem(ScheduleItem* scheduleItem)
{
    bool ok = false;
    ok = mAllScheduleItem.removeOne(scheduleItem);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllScheduleItemBySessionId(scheduleItem->sessionId());
    emit deletedFromAllScheduleItem(scheduleItem);
    emit scheduleItemPropertyListChanged();
    scheduleItem->deleteLater();
    scheduleItem = 0;
    return ok;
}


bool DataManager::deleteScheduleItemBySessionId(const int& sessionId)
{
    for (int i = 0; i < mAllScheduleItem.size(); ++i) {
        ScheduleItem* scheduleItem;
        scheduleItem = (ScheduleItem*) mAllScheduleItem.at(i);
        if (scheduleItem->sessionId() == sessionId) {
            mAllScheduleItem.removeAt(i);
            emit deletedFromAllScheduleItemBySessionId(sessionId);
            emit deletedFromAllScheduleItem(scheduleItem);
            emit scheduleItemPropertyListChanged();
            scheduleItem->deleteLater();
            scheduleItem = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
ScheduleItem* DataManager::findScheduleItemBySessionId(const int& sessionId){
    for (int i = 0; i < mAllScheduleItem.size(); ++i) {
        ScheduleItem* scheduleItem;
        scheduleItem = (ScheduleItem*)mAllScheduleItem.at(i);
        if(scheduleItem->sessionId() == sessionId){
            return scheduleItem;
        }
    }
    qDebug() << "no ScheduleItem found for sessionId " << sessionId;
    return 0;
}

/*
 * reads Maps of Favorite in from JSON cache
 * creates List of Favorite*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initFavoriteFromCache()
{
	qDebug() << "start initFavoriteFromCache";
    mAllFavorite.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheFavorite);
    qDebug() << "read Favorite from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Favorite* favorite = new Favorite();
        // Important: DataManager must be parent of all root DTOs
        favorite->setParent(this);
        favorite->fillFromCacheMap(cacheMap);
        mAllFavorite.append(favorite);
    }
    qDebug() << "created Favorite* #" << mAllFavorite.size();
}


/*
 * save List of Favorite* to JSON cache
 * convert list of Favorite* to QVariantList
 * toCacheMap stores all properties without transient values
 */
void DataManager::saveFavoriteToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Favorite* #" << mAllFavorite.size();
    for (int i = 0; i < mAllFavorite.size(); ++i) {
        Favorite* favorite;
        favorite = (Favorite*)mAllFavorite.at(i);
        QVariantMap cacheMap;
        cacheMap = favorite->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Favorite* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheFavorite, cacheList);
}


void DataManager::resolveFavoriteReferences(Favorite* favorite)
{
	if (!favorite) {
        qDebug() << "cannot resolveFavoriteReferences with favorite NULL";
        return;
    }
    if(favorite->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (favorite->hasSession() && !favorite->isSessionResolvedAsDataObject()) {
    	Session* session;
   		session = findSessionBySessionId(favorite->session());
    	if (session) {
    		favorite->resolveSessionAsDataObject(session);
    	} else {
    		qDebug() << "markSessionAsInvalid: " << favorite->session();
    		favorite->markSessionAsInvalid();
    	}
    }
}

void DataManager::resolveReferencesForAllFavorite()
{
    for (int i = 0; i < mAllFavorite.size(); ++i) {
        Favorite* favorite;
        favorite = (Favorite*)mAllFavorite.at(i);
    	resolveFavoriteReferences(favorite);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Favorite*> DataManager::listOfFavoriteForKeys(
        QStringList keyList)
{
    QList<Favorite*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllFavorite.size(); ++i) {
        Favorite* favorite;
        favorite = (Favorite*) mAllFavorite.at(i);
        if (keyList.contains(QString::number(favorite->sessionId()))) {
            listOfData.append(favorite);
            keyList.removeOne(QString::number(favorite->sessionId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Favorite: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::favoriteAsQVariantList()
{
    QVariantList favoriteList;
    for (int i = 0; i < mAllFavorite.size(); ++i) {
        favoriteList.append(((Favorite*) (mAllFavorite.at(i)))->toMap());
    }
    return favoriteList;
}

QList<QObject*> DataManager::allFavorite()
{
    return mAllFavorite;
}

QQmlListProperty<Favorite> DataManager::favoritePropertyList()
{
    return QQmlListProperty<Favorite>(this, 0,
            &DataManager::appendToFavoriteProperty, &DataManager::favoritePropertyCount,
            &DataManager::atFavoriteProperty, &DataManager::clearFavoriteProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Favorite*
void DataManager::appendToFavoriteProperty(
        QQmlListProperty<Favorite> *favoriteList,
        Favorite* favorite)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(favoriteList->object);
    if (dataManagerObject) {
        favorite->setParent(dataManagerObject);
        dataManagerObject->mAllFavorite.append(favorite);
        emit dataManagerObject->addedToAllFavorite(favorite);
    } else {
        qWarning() << "cannot append Favorite* to mAllFavorite "
                << "Object is not of type DataManager*";
    }
}
int DataManager::favoritePropertyCount(
        QQmlListProperty<Favorite> *favoriteList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(favoriteList->object);
    if (dataManager) {
        return dataManager->mAllFavorite.size();
    } else {
        qWarning() << "cannot get size mAllFavorite " << "Object is not of type DataManager*";
    }
    return 0;
}
Favorite* DataManager::atFavoriteProperty(
        QQmlListProperty<Favorite> *favoriteList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(favoriteList->object);
    if (dataManager) {
        if (dataManager->mAllFavorite.size() > pos) {
            return (Favorite*) dataManager->mAllFavorite.at(pos);
        }
        qWarning() << "cannot get Favorite* at pos " << pos << " size is "
                << dataManager->mAllFavorite.size();
    } else {
        qWarning() << "cannot get Favorite* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearFavoriteProperty(
        QQmlListProperty<Favorite> *favoriteList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(favoriteList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllFavorite.size(); ++i) {
            Favorite* favorite;
            favorite = (Favorite*) dataManager->mAllFavorite.at(i);
			emit dataManager->deletedFromAllFavoriteBySessionId(favorite->sessionId());
			emit dataManager->deletedFromAllFavorite(favorite);
            favorite->deleteLater();
            favorite = 0;
        }
        dataManager->mAllFavorite.clear();
    } else {
        qWarning() << "cannot clear mAllFavorite " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Favorite
 * and clears the list
 */
void DataManager::deleteFavorite()
{
    for (int i = 0; i < mAllFavorite.size(); ++i) {
        Favorite* favorite;
        favorite = (Favorite*) mAllFavorite.at(i);
        emit deletedFromAllFavoriteBySessionId(favorite->sessionId());
		emit deletedFromAllFavorite(favorite);
		emit favoritePropertyListChanged();
        favorite->deleteLater();
        favorite = 0;
     }
     mAllFavorite.clear();
}

/**
 * creates a new Favorite
 * parent is DataManager
 * if data is successfully entered you must insertFavorite
 * if edit was canceled you must undoCreateFavorite to free up memory
 */
Favorite* DataManager::createFavorite()
{
    Favorite* favorite;
    favorite = new Favorite();
    favorite->setParent(this);
    favorite->prepareNew();
    return favorite;
}

/**
 * deletes Favorite
 * if createFavorite was canceled from UI
 * to delete a previous successfully inserted Favorite
 * use deleteFavorite
 */
void DataManager::undoCreateFavorite(Favorite* favorite)
{
    if (favorite) {
        qDebug() << "undoCreateFavorite " << favorite->sessionId();
        favorite->deleteLater();
        favorite = 0;
    }
}

void DataManager::insertFavorite(Favorite* favorite)
{
    // Important: DataManager must be parent of all root DTOs
    favorite->setParent(this);
    mAllFavorite.append(favorite);
    emit addedToAllFavorite(favorite);
    emit favoritePropertyListChanged();
}

void DataManager::insertFavoriteFromMap(const QVariantMap& favoriteMap,
        const bool& useForeignProperties)
{
    Favorite* favorite = new Favorite();
    favorite->setParent(this);
    if (useForeignProperties) {
        favorite->fillFromForeignMap(favoriteMap);
    } else {
        favorite->fillFromMap(favoriteMap);
    }
    mAllFavorite.append(favorite);
    emit addedToAllFavorite(favorite);
    favoritePropertyListChanged();
}

bool DataManager::deleteFavorite(Favorite* favorite)
{
    bool ok = false;
    ok = mAllFavorite.removeOne(favorite);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllFavoriteBySessionId(favorite->sessionId());
    emit deletedFromAllFavorite(favorite);
    emit favoritePropertyListChanged();
    favorite->deleteLater();
    favorite = 0;
    return ok;
}


bool DataManager::deleteFavoriteBySessionId(const int& sessionId)
{
    for (int i = 0; i < mAllFavorite.size(); ++i) {
        Favorite* favorite;
        favorite = (Favorite*) mAllFavorite.at(i);
        if (favorite->sessionId() == sessionId) {
            mAllFavorite.removeAt(i);
            emit deletedFromAllFavoriteBySessionId(sessionId);
            emit deletedFromAllFavorite(favorite);
            emit favoritePropertyListChanged();
            favorite->deleteLater();
            favorite = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Favorite* DataManager::findFavoriteBySessionId(const int& sessionId){
    for (int i = 0; i < mAllFavorite.size(); ++i) {
        Favorite* favorite;
        favorite = (Favorite*)mAllFavorite.at(i);
        if(favorite->sessionId() == sessionId){
            return favorite;
        }
    }
    qDebug() << "no Favorite found for sessionId " << sessionId;
    return 0;
}

/*
 * reads Maps of Bookmark in from JSON cache
 * creates List of Bookmark*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initBookmarkFromCache()
{
	qDebug() << "start initBookmarkFromCache";
    mAllBookmark.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheBookmark);
    qDebug() << "read Bookmark from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Bookmark* bookmark = new Bookmark();
        // Important: DataManager must be parent of all root DTOs
        bookmark->setParent(this);
        bookmark->fillFromCacheMap(cacheMap);
        mAllBookmark.append(bookmark);
    }
    qDebug() << "created Bookmark* #" << mAllBookmark.size();
}


/*
 * save List of Bookmark* to JSON cache
 * convert list of Bookmark* to QVariantList
 * toCacheMap stores all properties without transient values
 */
void DataManager::saveBookmarkToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Bookmark* #" << mAllBookmark.size();
    for (int i = 0; i < mAllBookmark.size(); ++i) {
        Bookmark* bookmark;
        bookmark = (Bookmark*)mAllBookmark.at(i);
        QVariantMap cacheMap;
        cacheMap = bookmark->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Bookmark* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheBookmark, cacheList);
}


void DataManager::resolveBookmarkReferences(Bookmark* bookmark)
{
	if (!bookmark) {
        qDebug() << "cannot resolveBookmarkReferences with bookmark NULL";
        return;
    }
    if(bookmark->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (bookmark->hasSession() && !bookmark->isSessionResolvedAsDataObject()) {
    	Session* session;
   		session = findSessionBySessionId(bookmark->session());
    	if (session) {
    		bookmark->resolveSessionAsDataObject(session);
    	} else {
    		qDebug() << "markSessionAsInvalid: " << bookmark->session();
    		bookmark->markSessionAsInvalid();
    	}
    }
}

void DataManager::resolveReferencesForAllBookmark()
{
    for (int i = 0; i < mAllBookmark.size(); ++i) {
        Bookmark* bookmark;
        bookmark = (Bookmark*)mAllBookmark.at(i);
    	resolveBookmarkReferences(bookmark);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Bookmark*> DataManager::listOfBookmarkForKeys(
        QStringList keyList)
{
    QList<Bookmark*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllBookmark.size(); ++i) {
        Bookmark* bookmark;
        bookmark = (Bookmark*) mAllBookmark.at(i);
        if (keyList.contains(QString::number(bookmark->sessionId()))) {
            listOfData.append(bookmark);
            keyList.removeOne(QString::number(bookmark->sessionId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Bookmark: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::bookmarkAsQVariantList()
{
    QVariantList bookmarkList;
    for (int i = 0; i < mAllBookmark.size(); ++i) {
        bookmarkList.append(((Bookmark*) (mAllBookmark.at(i)))->toMap());
    }
    return bookmarkList;
}

QList<QObject*> DataManager::allBookmark()
{
    return mAllBookmark;
}

QQmlListProperty<Bookmark> DataManager::bookmarkPropertyList()
{
    return QQmlListProperty<Bookmark>(this, 0,
            &DataManager::appendToBookmarkProperty, &DataManager::bookmarkPropertyCount,
            &DataManager::atBookmarkProperty, &DataManager::clearBookmarkProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Bookmark*
void DataManager::appendToBookmarkProperty(
        QQmlListProperty<Bookmark> *bookmarkList,
        Bookmark* bookmark)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(bookmarkList->object);
    if (dataManagerObject) {
        bookmark->setParent(dataManagerObject);
        dataManagerObject->mAllBookmark.append(bookmark);
        emit dataManagerObject->addedToAllBookmark(bookmark);
    } else {
        qWarning() << "cannot append Bookmark* to mAllBookmark "
                << "Object is not of type DataManager*";
    }
}
int DataManager::bookmarkPropertyCount(
        QQmlListProperty<Bookmark> *bookmarkList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(bookmarkList->object);
    if (dataManager) {
        return dataManager->mAllBookmark.size();
    } else {
        qWarning() << "cannot get size mAllBookmark " << "Object is not of type DataManager*";
    }
    return 0;
}
Bookmark* DataManager::atBookmarkProperty(
        QQmlListProperty<Bookmark> *bookmarkList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(bookmarkList->object);
    if (dataManager) {
        if (dataManager->mAllBookmark.size() > pos) {
            return (Bookmark*) dataManager->mAllBookmark.at(pos);
        }
        qWarning() << "cannot get Bookmark* at pos " << pos << " size is "
                << dataManager->mAllBookmark.size();
    } else {
        qWarning() << "cannot get Bookmark* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearBookmarkProperty(
        QQmlListProperty<Bookmark> *bookmarkList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(bookmarkList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllBookmark.size(); ++i) {
            Bookmark* bookmark;
            bookmark = (Bookmark*) dataManager->mAllBookmark.at(i);
			emit dataManager->deletedFromAllBookmarkBySessionId(bookmark->sessionId());
			emit dataManager->deletedFromAllBookmark(bookmark);
            bookmark->deleteLater();
            bookmark = 0;
        }
        dataManager->mAllBookmark.clear();
    } else {
        qWarning() << "cannot clear mAllBookmark " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Bookmark
 * and clears the list
 */
void DataManager::deleteBookmark()
{
    for (int i = 0; i < mAllBookmark.size(); ++i) {
        Bookmark* bookmark;
        bookmark = (Bookmark*) mAllBookmark.at(i);
        emit deletedFromAllBookmarkBySessionId(bookmark->sessionId());
		emit deletedFromAllBookmark(bookmark);
		emit bookmarkPropertyListChanged();
        bookmark->deleteLater();
        bookmark = 0;
     }
     mAllBookmark.clear();
}

/**
 * creates a new Bookmark
 * parent is DataManager
 * if data is successfully entered you must insertBookmark
 * if edit was canceled you must undoCreateBookmark to free up memory
 */
Bookmark* DataManager::createBookmark()
{
    Bookmark* bookmark;
    bookmark = new Bookmark();
    bookmark->setParent(this);
    bookmark->prepareNew();
    return bookmark;
}

/**
 * deletes Bookmark
 * if createBookmark was canceled from UI
 * to delete a previous successfully inserted Bookmark
 * use deleteBookmark
 */
void DataManager::undoCreateBookmark(Bookmark* bookmark)
{
    if (bookmark) {
        qDebug() << "undoCreateBookmark " << bookmark->sessionId();
        bookmark->deleteLater();
        bookmark = 0;
    }
}

void DataManager::insertBookmark(Bookmark* bookmark)
{
    // Important: DataManager must be parent of all root DTOs
    bookmark->setParent(this);
    mAllBookmark.append(bookmark);
    emit addedToAllBookmark(bookmark);
    emit bookmarkPropertyListChanged();
}

void DataManager::insertBookmarkFromMap(const QVariantMap& bookmarkMap,
        const bool& useForeignProperties)
{
    Bookmark* bookmark = new Bookmark();
    bookmark->setParent(this);
    if (useForeignProperties) {
        bookmark->fillFromForeignMap(bookmarkMap);
    } else {
        bookmark->fillFromMap(bookmarkMap);
    }
    mAllBookmark.append(bookmark);
    emit addedToAllBookmark(bookmark);
    bookmarkPropertyListChanged();
}

bool DataManager::deleteBookmark(Bookmark* bookmark)
{
    bool ok = false;
    ok = mAllBookmark.removeOne(bookmark);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllBookmarkBySessionId(bookmark->sessionId());
    emit deletedFromAllBookmark(bookmark);
    emit bookmarkPropertyListChanged();
    bookmark->deleteLater();
    bookmark = 0;
    return ok;
}


bool DataManager::deleteBookmarkBySessionId(const int& sessionId)
{
    for (int i = 0; i < mAllBookmark.size(); ++i) {
        Bookmark* bookmark;
        bookmark = (Bookmark*) mAllBookmark.at(i);
        if (bookmark->sessionId() == sessionId) {
            mAllBookmark.removeAt(i);
            emit deletedFromAllBookmarkBySessionId(sessionId);
            emit deletedFromAllBookmark(bookmark);
            emit bookmarkPropertyListChanged();
            bookmark->deleteLater();
            bookmark = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Bookmark* DataManager::findBookmarkBySessionId(const int& sessionId){
    for (int i = 0; i < mAllBookmark.size(); ++i) {
        Bookmark* bookmark;
        bookmark = (Bookmark*)mAllBookmark.at(i);
        if(bookmark->sessionId() == sessionId){
            return bookmark;
        }
    }
    qDebug() << "no Bookmark found for sessionId " << sessionId;
    return 0;
}

/*
 * reads Maps of SessionLists in from JSON cache
 * creates List of SessionLists*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initSessionListsFromCache()
{
	qDebug() << "start initSessionListsFromCache";
    mAllSessionLists.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheSessionLists);
    qDebug() << "read SessionLists from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        SessionLists* sessionLists = new SessionLists();
        // Important: DataManager must be parent of all root DTOs
        sessionLists->setParent(this);
        sessionLists->fillFromCacheMap(cacheMap);
        mAllSessionLists.append(sessionLists);
    }
    qDebug() << "created SessionLists* #" << mAllSessionLists.size();
}


/*
 * save List of SessionLists* to JSON cache
 * convert list of SessionLists* to QVariantList
 * toCacheMap stores all properties without transient values
 * SessionLists is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveSessionListsToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching SessionLists* #" << mAllSessionLists.size();
    for (int i = 0; i < mAllSessionLists.size(); ++i) {
        SessionLists* sessionLists;
        sessionLists = (SessionLists*)mAllSessionLists.at(i);
        QVariantMap cacheMap;
        cacheMap = sessionLists->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "SessionLists* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheSessionLists, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<SessionLists*> DataManager::listOfSessionListsForKeys(
        QStringList keyList)
{
    QList<SessionLists*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllSessionLists.size(); ++i) {
        SessionLists* sessionLists;
        sessionLists = (SessionLists*) mAllSessionLists.at(i);
        if (keyList.contains(sessionLists->uuid())) {
            listOfData.append(sessionLists);
            keyList.removeOne(sessionLists->uuid());
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for SessionLists: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::sessionListsAsQVariantList()
{
    QVariantList sessionListsList;
    for (int i = 0; i < mAllSessionLists.size(); ++i) {
        sessionListsList.append(((SessionLists*) (mAllSessionLists.at(i)))->toMap());
    }
    return sessionListsList;
}

QList<QObject*> DataManager::allSessionLists()
{
    return mAllSessionLists;
}

QQmlListProperty<SessionLists> DataManager::sessionListsPropertyList()
{
    return QQmlListProperty<SessionLists>(this, 0,
            &DataManager::appendToSessionListsProperty, &DataManager::sessionListsPropertyCount,
            &DataManager::atSessionListsProperty, &DataManager::clearSessionListsProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of SessionLists*
void DataManager::appendToSessionListsProperty(
        QQmlListProperty<SessionLists> *sessionListsList,
        SessionLists* sessionLists)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(sessionListsList->object);
    if (dataManagerObject) {
        sessionLists->setParent(dataManagerObject);
        dataManagerObject->mAllSessionLists.append(sessionLists);
        emit dataManagerObject->addedToAllSessionLists(sessionLists);
    } else {
        qWarning() << "cannot append SessionLists* to mAllSessionLists "
                << "Object is not of type DataManager*";
    }
}
int DataManager::sessionListsPropertyCount(
        QQmlListProperty<SessionLists> *sessionListsList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionListsList->object);
    if (dataManager) {
        return dataManager->mAllSessionLists.size();
    } else {
        qWarning() << "cannot get size mAllSessionLists " << "Object is not of type DataManager*";
    }
    return 0;
}
SessionLists* DataManager::atSessionListsProperty(
        QQmlListProperty<SessionLists> *sessionListsList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionListsList->object);
    if (dataManager) {
        if (dataManager->mAllSessionLists.size() > pos) {
            return (SessionLists*) dataManager->mAllSessionLists.at(pos);
        }
        qWarning() << "cannot get SessionLists* at pos " << pos << " size is "
                << dataManager->mAllSessionLists.size();
    } else {
        qWarning() << "cannot get SessionLists* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearSessionListsProperty(
        QQmlListProperty<SessionLists> *sessionListsList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionListsList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllSessionLists.size(); ++i) {
            SessionLists* sessionLists;
            sessionLists = (SessionLists*) dataManager->mAllSessionLists.at(i);
			emit dataManager->deletedFromAllSessionListsByUuid(sessionLists->uuid());
			emit dataManager->deletedFromAllSessionLists(sessionLists);
            sessionLists->deleteLater();
            sessionLists = 0;
        }
        dataManager->mAllSessionLists.clear();
    } else {
        qWarning() << "cannot clear mAllSessionLists " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all SessionLists
 * and clears the list
 */
void DataManager::deleteSessionLists()
{
    for (int i = 0; i < mAllSessionLists.size(); ++i) {
        SessionLists* sessionLists;
        sessionLists = (SessionLists*) mAllSessionLists.at(i);
        emit deletedFromAllSessionListsByUuid(sessionLists->uuid());
		emit deletedFromAllSessionLists(sessionLists);
		emit sessionListsPropertyListChanged();
        sessionLists->deleteLater();
        sessionLists = 0;
     }
     mAllSessionLists.clear();
}

/**
 * creates a new SessionLists
 * parent is DataManager
 * if data is successfully entered you must insertSessionLists
 * if edit was canceled you must undoCreateSessionLists to free up memory
 */
SessionLists* DataManager::createSessionLists()
{
    SessionLists* sessionLists;
    sessionLists = new SessionLists();
    sessionLists->setParent(this);
    sessionLists->prepareNew();
    return sessionLists;
}

/**
 * deletes SessionLists
 * if createSessionLists was canceled from UI
 * to delete a previous successfully inserted SessionLists
 * use deleteSessionLists
 */
void DataManager::undoCreateSessionLists(SessionLists* sessionLists)
{
    if (sessionLists) {
        qDebug() << "undoCreateSessionLists " << sessionLists->uuid();
        sessionLists->deleteLater();
        sessionLists = 0;
    }
}

void DataManager::insertSessionLists(SessionLists* sessionLists)
{
    // Important: DataManager must be parent of all root DTOs
    sessionLists->setParent(this);
    mAllSessionLists.append(sessionLists);
    emit addedToAllSessionLists(sessionLists);
    emit sessionListsPropertyListChanged();
}

void DataManager::insertSessionListsFromMap(const QVariantMap& sessionListsMap,
        const bool& useForeignProperties)
{
    SessionLists* sessionLists = new SessionLists();
    sessionLists->setParent(this);
    if (useForeignProperties) {
        sessionLists->fillFromForeignMap(sessionListsMap);
    } else {
        sessionLists->fillFromMap(sessionListsMap);
    }
    mAllSessionLists.append(sessionLists);
    emit addedToAllSessionLists(sessionLists);
    sessionListsPropertyListChanged();
}

bool DataManager::deleteSessionLists(SessionLists* sessionLists)
{
    bool ok = false;
    ok = mAllSessionLists.removeOne(sessionLists);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllSessionListsByUuid(sessionLists->uuid());
    emit deletedFromAllSessionLists(sessionLists);
    emit sessionListsPropertyListChanged();
    sessionLists->deleteLater();
    sessionLists = 0;
    return ok;
}

bool DataManager::deleteSessionListsByUuid(const QString& uuid)
{
    if (uuid.isNull() || uuid.isEmpty()) {
        qDebug() << "cannot delete SessionLists from empty uuid";
        return false;
    }
    for (int i = 0; i < mAllSessionLists.size(); ++i) {
        SessionLists* sessionLists;
        sessionLists = (SessionLists*) mAllSessionLists.at(i);
        if (sessionLists->uuid() == uuid) {
            mAllSessionLists.removeAt(i);
            emit deletedFromAllSessionListsByUuid(uuid);
            emit deletedFromAllSessionLists(sessionLists);
            emit sessionListsPropertyListChanged();
            sessionLists->deleteLater();
            sessionLists = 0;
            return true;
        }
    }
    return false;
}


SessionLists* DataManager::findSessionListsByUuid(const QString& uuid){
    if (uuid.isNull() || uuid.isEmpty()) {
        qDebug() << "cannot find SessionLists from empty uuid";
        return 0;
    }
    for (int i = 0; i < mAllSessionLists.size(); ++i) {
        SessionLists* sessionLists;
        sessionLists = (SessionLists*)mAllSessionLists.at(i);
        if(sessionLists->uuid() == uuid){
            return sessionLists;
        }
    }
    qDebug() << "no SessionLists found for uuid " << uuid;
    return 0;
}


/*
 * reads Maps of SessionLink in from JSON cache
 * creates List of SessionLink*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initSessionLinkFromCache()
{
	qDebug() << "start initSessionLinkFromCache";
    mAllSessionLink.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheSessionLink);
    qDebug() << "read SessionLink from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        SessionLink* sessionLink = new SessionLink();
        // Important: DataManager must be parent of all root DTOs
        sessionLink->setParent(this);
        sessionLink->fillFromCacheMap(cacheMap);
        mAllSessionLink.append(sessionLink);
    }
    qDebug() << "created SessionLink* #" << mAllSessionLink.size();
}


/*
 * save List of SessionLink* to JSON cache
 * convert list of SessionLink* to QVariantList
 * toCacheMap stores all properties without transient values
 * SessionLink is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveSessionLinkToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching SessionLink* #" << mAllSessionLink.size();
    for (int i = 0; i < mAllSessionLink.size(); ++i) {
        SessionLink* sessionLink;
        sessionLink = (SessionLink*)mAllSessionLink.at(i);
        QVariantMap cacheMap;
        cacheMap = sessionLink->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "SessionLink* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheSessionLink, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<SessionLink*> DataManager::listOfSessionLinkForKeys(
        QStringList keyList)
{
    QList<SessionLink*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllSessionLink.size(); ++i) {
        SessionLink* sessionLink;
        sessionLink = (SessionLink*) mAllSessionLink.at(i);
        if (keyList.contains(sessionLink->uuid())) {
            listOfData.append(sessionLink);
            keyList.removeOne(sessionLink->uuid());
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for SessionLink: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::sessionLinkAsQVariantList()
{
    QVariantList sessionLinkList;
    for (int i = 0; i < mAllSessionLink.size(); ++i) {
        sessionLinkList.append(((SessionLink*) (mAllSessionLink.at(i)))->toMap());
    }
    return sessionLinkList;
}

QList<QObject*> DataManager::allSessionLink()
{
    return mAllSessionLink;
}

QQmlListProperty<SessionLink> DataManager::sessionLinkPropertyList()
{
    return QQmlListProperty<SessionLink>(this, 0,
            &DataManager::appendToSessionLinkProperty, &DataManager::sessionLinkPropertyCount,
            &DataManager::atSessionLinkProperty, &DataManager::clearSessionLinkProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of SessionLink*
void DataManager::appendToSessionLinkProperty(
        QQmlListProperty<SessionLink> *sessionLinkList,
        SessionLink* sessionLink)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(sessionLinkList->object);
    if (dataManagerObject) {
        sessionLink->setParent(dataManagerObject);
        dataManagerObject->mAllSessionLink.append(sessionLink);
        emit dataManagerObject->addedToAllSessionLink(sessionLink);
    } else {
        qWarning() << "cannot append SessionLink* to mAllSessionLink "
                << "Object is not of type DataManager*";
    }
}
int DataManager::sessionLinkPropertyCount(
        QQmlListProperty<SessionLink> *sessionLinkList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionLinkList->object);
    if (dataManager) {
        return dataManager->mAllSessionLink.size();
    } else {
        qWarning() << "cannot get size mAllSessionLink " << "Object is not of type DataManager*";
    }
    return 0;
}
SessionLink* DataManager::atSessionLinkProperty(
        QQmlListProperty<SessionLink> *sessionLinkList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionLinkList->object);
    if (dataManager) {
        if (dataManager->mAllSessionLink.size() > pos) {
            return (SessionLink*) dataManager->mAllSessionLink.at(pos);
        }
        qWarning() << "cannot get SessionLink* at pos " << pos << " size is "
                << dataManager->mAllSessionLink.size();
    } else {
        qWarning() << "cannot get SessionLink* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearSessionLinkProperty(
        QQmlListProperty<SessionLink> *sessionLinkList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionLinkList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllSessionLink.size(); ++i) {
            SessionLink* sessionLink;
            sessionLink = (SessionLink*) dataManager->mAllSessionLink.at(i);
			emit dataManager->deletedFromAllSessionLinkByUuid(sessionLink->uuid());
			emit dataManager->deletedFromAllSessionLink(sessionLink);
            sessionLink->deleteLater();
            sessionLink = 0;
        }
        dataManager->mAllSessionLink.clear();
    } else {
        qWarning() << "cannot clear mAllSessionLink " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all SessionLink
 * and clears the list
 */
void DataManager::deleteSessionLink()
{
    for (int i = 0; i < mAllSessionLink.size(); ++i) {
        SessionLink* sessionLink;
        sessionLink = (SessionLink*) mAllSessionLink.at(i);
        emit deletedFromAllSessionLinkByUuid(sessionLink->uuid());
		emit deletedFromAllSessionLink(sessionLink);
		emit sessionLinkPropertyListChanged();
        sessionLink->deleteLater();
        sessionLink = 0;
     }
     mAllSessionLink.clear();
}

/**
 * creates a new SessionLink
 * parent is DataManager
 * if data is successfully entered you must insertSessionLink
 * if edit was canceled you must undoCreateSessionLink to free up memory
 */
SessionLink* DataManager::createSessionLink()
{
    SessionLink* sessionLink;
    sessionLink = new SessionLink();
    sessionLink->setParent(this);
    sessionLink->prepareNew();
    return sessionLink;
}

/**
 * deletes SessionLink
 * if createSessionLink was canceled from UI
 * to delete a previous successfully inserted SessionLink
 * use deleteSessionLink
 */
void DataManager::undoCreateSessionLink(SessionLink* sessionLink)
{
    if (sessionLink) {
        qDebug() << "undoCreateSessionLink " << sessionLink->uuid();
        sessionLink->deleteLater();
        sessionLink = 0;
    }
}

void DataManager::insertSessionLink(SessionLink* sessionLink)
{
    // Important: DataManager must be parent of all root DTOs
    sessionLink->setParent(this);
    mAllSessionLink.append(sessionLink);
    emit addedToAllSessionLink(sessionLink);
    emit sessionLinkPropertyListChanged();
}

void DataManager::insertSessionLinkFromMap(const QVariantMap& sessionLinkMap,
        const bool& useForeignProperties)
{
    SessionLink* sessionLink = new SessionLink();
    sessionLink->setParent(this);
    if (useForeignProperties) {
        sessionLink->fillFromForeignMap(sessionLinkMap);
    } else {
        sessionLink->fillFromMap(sessionLinkMap);
    }
    mAllSessionLink.append(sessionLink);
    emit addedToAllSessionLink(sessionLink);
    sessionLinkPropertyListChanged();
}

bool DataManager::deleteSessionLink(SessionLink* sessionLink)
{
    bool ok = false;
    ok = mAllSessionLink.removeOne(sessionLink);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllSessionLinkByUuid(sessionLink->uuid());
    emit deletedFromAllSessionLink(sessionLink);
    emit sessionLinkPropertyListChanged();
    sessionLink->deleteLater();
    sessionLink = 0;
    return ok;
}

bool DataManager::deleteSessionLinkByUuid(const QString& uuid)
{
    if (uuid.isNull() || uuid.isEmpty()) {
        qDebug() << "cannot delete SessionLink from empty uuid";
        return false;
    }
    for (int i = 0; i < mAllSessionLink.size(); ++i) {
        SessionLink* sessionLink;
        sessionLink = (SessionLink*) mAllSessionLink.at(i);
        if (sessionLink->uuid() == uuid) {
            mAllSessionLink.removeAt(i);
            emit deletedFromAllSessionLinkByUuid(uuid);
            emit deletedFromAllSessionLink(sessionLink);
            emit sessionLinkPropertyListChanged();
            sessionLink->deleteLater();
            sessionLink = 0;
            return true;
        }
    }
    return false;
}


SessionLink* DataManager::findSessionLinkByUuid(const QString& uuid){
    if (uuid.isNull() || uuid.isEmpty()) {
        qDebug() << "cannot find SessionLink from empty uuid";
        return 0;
    }
    for (int i = 0; i < mAllSessionLink.size(); ++i) {
        SessionLink* sessionLink;
        sessionLink = (SessionLink*)mAllSessionLink.at(i);
        if(sessionLink->uuid() == uuid){
            return sessionLink;
        }
    }
    qDebug() << "no SessionLink found for uuid " << uuid;
    return 0;
}


/*
 * reads Maps of Speaker in from JSON cache
 * creates List of Speaker*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initSpeakerFromCache()
{
	qDebug() << "start initSpeakerFromCache";
    mAllSpeaker.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheSpeaker);
    qDebug() << "read Speaker from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Speaker* speaker = new Speaker();
        // Important: DataManager must be parent of all root DTOs
        speaker->setParent(this);
        speaker->fillFromCacheMap(cacheMap);
        mAllSpeaker.append(speaker);
    }
    qDebug() << "created Speaker* #" << mAllSpeaker.size();
}


/*
 * save List of Speaker* to JSON cache
 * convert list of Speaker* to QVariantList
 * toCacheMap stores all properties without transient values
 * Speaker is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveSpeakerToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Speaker* #" << mAllSpeaker.size();
    for (int i = 0; i < mAllSpeaker.size(); ++i) {
        Speaker* speaker;
        speaker = (Speaker*)mAllSpeaker.at(i);
        QVariantMap cacheMap;
        cacheMap = speaker->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Speaker* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheSpeaker, cacheList);
}


void DataManager::resolveSpeakerReferences(Speaker* speaker)
{
	if (!speaker) {
        qDebug() << "cannot resolveSpeakerReferences with speaker NULL";
        return;
    }
    if(speaker->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (speaker->hasSpeakerImage() && !speaker->isSpeakerImageResolvedAsDataObject()) {
    	SpeakerImage* speakerImage;
   		speakerImage = findSpeakerImageBySpeakerId(speaker->speakerImage());
    	if (speakerImage) {
    		speaker->resolveSpeakerImageAsDataObject(speakerImage);
    	} else {
    		qDebug() << "markSpeakerImageAsInvalid: " << speaker->speakerImage();
    		speaker->markSpeakerImageAsInvalid();
    	}
    }
    if (!speaker->areSessionsKeysResolved()) {
        speaker->resolveSessionsKeys(
                listOfSessionForKeys(speaker->sessionsKeys()));
    }
}

void DataManager::resolveReferencesForAllSpeaker()
{
    for (int i = 0; i < mAllSpeaker.size(); ++i) {
        Speaker* speaker;
        speaker = (Speaker*)mAllSpeaker.at(i);
    	resolveSpeakerReferences(speaker);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Speaker*> DataManager::listOfSpeakerForKeys(
        QStringList keyList)
{
    QList<Speaker*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllSpeaker.size(); ++i) {
        Speaker* speaker;
        speaker = (Speaker*) mAllSpeaker.at(i);
        if (keyList.contains(QString::number(speaker->speakerId()))) {
            listOfData.append(speaker);
            keyList.removeOne(QString::number(speaker->speakerId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Speaker: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::speakerAsQVariantList()
{
    QVariantList speakerList;
    for (int i = 0; i < mAllSpeaker.size(); ++i) {
        speakerList.append(((Speaker*) (mAllSpeaker.at(i)))->toMap());
    }
    return speakerList;
}

QList<QObject*> DataManager::allSpeaker()
{
    return mAllSpeaker;
}

QQmlListProperty<Speaker> DataManager::speakerPropertyList()
{
    return QQmlListProperty<Speaker>(this, 0,
            &DataManager::appendToSpeakerProperty, &DataManager::speakerPropertyCount,
            &DataManager::atSpeakerProperty, &DataManager::clearSpeakerProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Speaker*
void DataManager::appendToSpeakerProperty(
        QQmlListProperty<Speaker> *speakerList,
        Speaker* speaker)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(speakerList->object);
    if (dataManagerObject) {
        speaker->setParent(dataManagerObject);
        dataManagerObject->mAllSpeaker.append(speaker);
        emit dataManagerObject->addedToAllSpeaker(speaker);
    } else {
        qWarning() << "cannot append Speaker* to mAllSpeaker "
                << "Object is not of type DataManager*";
    }
}
int DataManager::speakerPropertyCount(
        QQmlListProperty<Speaker> *speakerList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(speakerList->object);
    if (dataManager) {
        return dataManager->mAllSpeaker.size();
    } else {
        qWarning() << "cannot get size mAllSpeaker " << "Object is not of type DataManager*";
    }
    return 0;
}
Speaker* DataManager::atSpeakerProperty(
        QQmlListProperty<Speaker> *speakerList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(speakerList->object);
    if (dataManager) {
        if (dataManager->mAllSpeaker.size() > pos) {
            return (Speaker*) dataManager->mAllSpeaker.at(pos);
        }
        qWarning() << "cannot get Speaker* at pos " << pos << " size is "
                << dataManager->mAllSpeaker.size();
    } else {
        qWarning() << "cannot get Speaker* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearSpeakerProperty(
        QQmlListProperty<Speaker> *speakerList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(speakerList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllSpeaker.size(); ++i) {
            Speaker* speaker;
            speaker = (Speaker*) dataManager->mAllSpeaker.at(i);
			emit dataManager->deletedFromAllSpeakerBySpeakerId(speaker->speakerId());
			emit dataManager->deletedFromAllSpeaker(speaker);
            speaker->deleteLater();
            speaker = 0;
        }
        dataManager->mAllSpeaker.clear();
    } else {
        qWarning() << "cannot clear mAllSpeaker " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Speaker
 * and clears the list
 */
void DataManager::deleteSpeaker()
{
    for (int i = 0; i < mAllSpeaker.size(); ++i) {
        Speaker* speaker;
        speaker = (Speaker*) mAllSpeaker.at(i);
        emit deletedFromAllSpeakerBySpeakerId(speaker->speakerId());
		emit deletedFromAllSpeaker(speaker);
		emit speakerPropertyListChanged();
        speaker->deleteLater();
        speaker = 0;
     }
     mAllSpeaker.clear();
}

/**
 * creates a new Speaker
 * parent is DataManager
 * if data is successfully entered you must insertSpeaker
 * if edit was canceled you must undoCreateSpeaker to free up memory
 */
Speaker* DataManager::createSpeaker()
{
    Speaker* speaker;
    speaker = new Speaker();
    speaker->setParent(this);
    speaker->prepareNew();
    return speaker;
}

/**
 * deletes Speaker
 * if createSpeaker was canceled from UI
 * to delete a previous successfully inserted Speaker
 * use deleteSpeaker
 */
void DataManager::undoCreateSpeaker(Speaker* speaker)
{
    if (speaker) {
        qDebug() << "undoCreateSpeaker " << speaker->speakerId();
        speaker->deleteLater();
        speaker = 0;
    }
}

void DataManager::insertSpeaker(Speaker* speaker)
{
    // Important: DataManager must be parent of all root DTOs
    speaker->setParent(this);
    mAllSpeaker.append(speaker);
    emit addedToAllSpeaker(speaker);
    emit speakerPropertyListChanged();
}

void DataManager::insertSpeakerFromMap(const QVariantMap& speakerMap,
        const bool& useForeignProperties)
{
    Speaker* speaker = new Speaker();
    speaker->setParent(this);
    if (useForeignProperties) {
        speaker->fillFromForeignMap(speakerMap);
    } else {
        speaker->fillFromMap(speakerMap);
    }
    mAllSpeaker.append(speaker);
    emit addedToAllSpeaker(speaker);
    speakerPropertyListChanged();
}

bool DataManager::deleteSpeaker(Speaker* speaker)
{
    bool ok = false;
    ok = mAllSpeaker.removeOne(speaker);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllSpeakerBySpeakerId(speaker->speakerId());
    emit deletedFromAllSpeaker(speaker);
    emit speakerPropertyListChanged();
    speaker->deleteLater();
    speaker = 0;
    return ok;
}


bool DataManager::deleteSpeakerBySpeakerId(const int& speakerId)
{
    for (int i = 0; i < mAllSpeaker.size(); ++i) {
        Speaker* speaker;
        speaker = (Speaker*) mAllSpeaker.at(i);
        if (speaker->speakerId() == speakerId) {
            mAllSpeaker.removeAt(i);
            emit deletedFromAllSpeakerBySpeakerId(speakerId);
            emit deletedFromAllSpeaker(speaker);
            emit speakerPropertyListChanged();
            speaker->deleteLater();
            speaker = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Speaker* DataManager::findSpeakerBySpeakerId(const int& speakerId){
    for (int i = 0; i < mAllSpeaker.size(); ++i) {
        Speaker* speaker;
        speaker = (Speaker*)mAllSpeaker.at(i);
        if(speaker->speakerId() == speakerId){
            return speaker;
        }
    }
    qDebug() << "no Speaker found for speakerId " << speakerId;
    return 0;
}

/*
 * reads Maps of SpeakerImage in from JSON cache
 * creates List of SpeakerImage*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initSpeakerImageFromCache()
{
	qDebug() << "start initSpeakerImageFromCache";
    mAllSpeakerImage.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheSpeakerImage);
    qDebug() << "read SpeakerImage from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        SpeakerImage* speakerImage = new SpeakerImage();
        // Important: DataManager must be parent of all root DTOs
        speakerImage->setParent(this);
        speakerImage->fillFromCacheMap(cacheMap);
        mAllSpeakerImage.append(speakerImage);
    }
    qDebug() << "created SpeakerImage* #" << mAllSpeakerImage.size();
}


/*
 * save List of SpeakerImage* to JSON cache
 * convert list of SpeakerImage* to QVariantList
 * toCacheMap stores all properties without transient values
 * SpeakerImage is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveSpeakerImageToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching SpeakerImage* #" << mAllSpeakerImage.size();
    for (int i = 0; i < mAllSpeakerImage.size(); ++i) {
        SpeakerImage* speakerImage;
        speakerImage = (SpeakerImage*)mAllSpeakerImage.at(i);
        QVariantMap cacheMap;
        cacheMap = speakerImage->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "SpeakerImage* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheSpeakerImage, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<SpeakerImage*> DataManager::listOfSpeakerImageForKeys(
        QStringList keyList)
{
    QList<SpeakerImage*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllSpeakerImage.size(); ++i) {
        SpeakerImage* speakerImage;
        speakerImage = (SpeakerImage*) mAllSpeakerImage.at(i);
        if (keyList.contains(QString::number(speakerImage->speakerId()))) {
            listOfData.append(speakerImage);
            keyList.removeOne(QString::number(speakerImage->speakerId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for SpeakerImage: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::speakerImageAsQVariantList()
{
    QVariantList speakerImageList;
    for (int i = 0; i < mAllSpeakerImage.size(); ++i) {
        speakerImageList.append(((SpeakerImage*) (mAllSpeakerImage.at(i)))->toMap());
    }
    return speakerImageList;
}

QList<QObject*> DataManager::allSpeakerImage()
{
    return mAllSpeakerImage;
}

QQmlListProperty<SpeakerImage> DataManager::speakerImagePropertyList()
{
    return QQmlListProperty<SpeakerImage>(this, 0,
            &DataManager::appendToSpeakerImageProperty, &DataManager::speakerImagePropertyCount,
            &DataManager::atSpeakerImageProperty, &DataManager::clearSpeakerImageProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of SpeakerImage*
void DataManager::appendToSpeakerImageProperty(
        QQmlListProperty<SpeakerImage> *speakerImageList,
        SpeakerImage* speakerImage)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(speakerImageList->object);
    if (dataManagerObject) {
        speakerImage->setParent(dataManagerObject);
        dataManagerObject->mAllSpeakerImage.append(speakerImage);
        emit dataManagerObject->addedToAllSpeakerImage(speakerImage);
    } else {
        qWarning() << "cannot append SpeakerImage* to mAllSpeakerImage "
                << "Object is not of type DataManager*";
    }
}
int DataManager::speakerImagePropertyCount(
        QQmlListProperty<SpeakerImage> *speakerImageList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(speakerImageList->object);
    if (dataManager) {
        return dataManager->mAllSpeakerImage.size();
    } else {
        qWarning() << "cannot get size mAllSpeakerImage " << "Object is not of type DataManager*";
    }
    return 0;
}
SpeakerImage* DataManager::atSpeakerImageProperty(
        QQmlListProperty<SpeakerImage> *speakerImageList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(speakerImageList->object);
    if (dataManager) {
        if (dataManager->mAllSpeakerImage.size() > pos) {
            return (SpeakerImage*) dataManager->mAllSpeakerImage.at(pos);
        }
        qWarning() << "cannot get SpeakerImage* at pos " << pos << " size is "
                << dataManager->mAllSpeakerImage.size();
    } else {
        qWarning() << "cannot get SpeakerImage* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearSpeakerImageProperty(
        QQmlListProperty<SpeakerImage> *speakerImageList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(speakerImageList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllSpeakerImage.size(); ++i) {
            SpeakerImage* speakerImage;
            speakerImage = (SpeakerImage*) dataManager->mAllSpeakerImage.at(i);
			emit dataManager->deletedFromAllSpeakerImageBySpeakerId(speakerImage->speakerId());
			emit dataManager->deletedFromAllSpeakerImage(speakerImage);
            speakerImage->deleteLater();
            speakerImage = 0;
        }
        dataManager->mAllSpeakerImage.clear();
    } else {
        qWarning() << "cannot clear mAllSpeakerImage " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all SpeakerImage
 * and clears the list
 */
void DataManager::deleteSpeakerImage()
{
    for (int i = 0; i < mAllSpeakerImage.size(); ++i) {
        SpeakerImage* speakerImage;
        speakerImage = (SpeakerImage*) mAllSpeakerImage.at(i);
        emit deletedFromAllSpeakerImageBySpeakerId(speakerImage->speakerId());
		emit deletedFromAllSpeakerImage(speakerImage);
		emit speakerImagePropertyListChanged();
        speakerImage->deleteLater();
        speakerImage = 0;
     }
     mAllSpeakerImage.clear();
}

/**
 * creates a new SpeakerImage
 * parent is DataManager
 * if data is successfully entered you must insertSpeakerImage
 * if edit was canceled you must undoCreateSpeakerImage to free up memory
 */
SpeakerImage* DataManager::createSpeakerImage()
{
    SpeakerImage* speakerImage;
    speakerImage = new SpeakerImage();
    speakerImage->setParent(this);
    speakerImage->prepareNew();
    return speakerImage;
}

/**
 * deletes SpeakerImage
 * if createSpeakerImage was canceled from UI
 * to delete a previous successfully inserted SpeakerImage
 * use deleteSpeakerImage
 */
void DataManager::undoCreateSpeakerImage(SpeakerImage* speakerImage)
{
    if (speakerImage) {
        qDebug() << "undoCreateSpeakerImage " << speakerImage->speakerId();
        speakerImage->deleteLater();
        speakerImage = 0;
    }
}

void DataManager::insertSpeakerImage(SpeakerImage* speakerImage)
{
    // Important: DataManager must be parent of all root DTOs
    speakerImage->setParent(this);
    mAllSpeakerImage.append(speakerImage);
    emit addedToAllSpeakerImage(speakerImage);
    emit speakerImagePropertyListChanged();
}

void DataManager::insertSpeakerImageFromMap(const QVariantMap& speakerImageMap,
        const bool& useForeignProperties)
{
    SpeakerImage* speakerImage = new SpeakerImage();
    speakerImage->setParent(this);
    if (useForeignProperties) {
        speakerImage->fillFromForeignMap(speakerImageMap);
    } else {
        speakerImage->fillFromMap(speakerImageMap);
    }
    mAllSpeakerImage.append(speakerImage);
    emit addedToAllSpeakerImage(speakerImage);
    speakerImagePropertyListChanged();
}

bool DataManager::deleteSpeakerImage(SpeakerImage* speakerImage)
{
    bool ok = false;
    ok = mAllSpeakerImage.removeOne(speakerImage);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllSpeakerImageBySpeakerId(speakerImage->speakerId());
    emit deletedFromAllSpeakerImage(speakerImage);
    emit speakerImagePropertyListChanged();
    speakerImage->deleteLater();
    speakerImage = 0;
    return ok;
}


bool DataManager::deleteSpeakerImageBySpeakerId(const int& speakerId)
{
    for (int i = 0; i < mAllSpeakerImage.size(); ++i) {
        SpeakerImage* speakerImage;
        speakerImage = (SpeakerImage*) mAllSpeakerImage.at(i);
        if (speakerImage->speakerId() == speakerId) {
            mAllSpeakerImage.removeAt(i);
            emit deletedFromAllSpeakerImageBySpeakerId(speakerId);
            emit deletedFromAllSpeakerImage(speakerImage);
            emit speakerImagePropertyListChanged();
            speakerImage->deleteLater();
            speakerImage = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
SpeakerImage* DataManager::findSpeakerImageBySpeakerId(const int& speakerId){
    for (int i = 0; i < mAllSpeakerImage.size(); ++i) {
        SpeakerImage* speakerImage;
        speakerImage = (SpeakerImage*)mAllSpeakerImage.at(i);
        if(speakerImage->speakerId() == speakerId){
            return speakerImage;
        }
    }
    qDebug() << "no SpeakerImage found for speakerId " << speakerId;
    return 0;
}

/*
 * reads Maps of SessionTrack in from JSON cache
 * creates List of SessionTrack*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initSessionTrackFromCache()
{
	qDebug() << "start initSessionTrackFromCache";
    mAllSessionTrack.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheSessionTrack);
    qDebug() << "read SessionTrack from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        SessionTrack* sessionTrack = new SessionTrack();
        // Important: DataManager must be parent of all root DTOs
        sessionTrack->setParent(this);
        sessionTrack->fillFromCacheMap(cacheMap);
        mAllSessionTrack.append(sessionTrack);
    }
    qDebug() << "created SessionTrack* #" << mAllSessionTrack.size();
}


/*
 * save List of SessionTrack* to JSON cache
 * convert list of SessionTrack* to QVariantList
 * toCacheMap stores all properties without transient values
 * SessionTrack is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveSessionTrackToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching SessionTrack* #" << mAllSessionTrack.size();
    for (int i = 0; i < mAllSessionTrack.size(); ++i) {
        SessionTrack* sessionTrack;
        sessionTrack = (SessionTrack*)mAllSessionTrack.at(i);
        QVariantMap cacheMap;
        cacheMap = sessionTrack->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "SessionTrack* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheSessionTrack, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<SessionTrack*> DataManager::listOfSessionTrackForKeys(
        QStringList keyList)
{
    QList<SessionTrack*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllSessionTrack.size(); ++i) {
        SessionTrack* sessionTrack;
        sessionTrack = (SessionTrack*) mAllSessionTrack.at(i);
        if (keyList.contains(QString::number(sessionTrack->trackId()))) {
            listOfData.append(sessionTrack);
            keyList.removeOne(QString::number(sessionTrack->trackId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for SessionTrack: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::sessionTrackAsQVariantList()
{
    QVariantList sessionTrackList;
    for (int i = 0; i < mAllSessionTrack.size(); ++i) {
        sessionTrackList.append(((SessionTrack*) (mAllSessionTrack.at(i)))->toMap());
    }
    return sessionTrackList;
}

QList<QObject*> DataManager::allSessionTrack()
{
    return mAllSessionTrack;
}

QQmlListProperty<SessionTrack> DataManager::sessionTrackPropertyList()
{
    return QQmlListProperty<SessionTrack>(this, 0,
            &DataManager::appendToSessionTrackProperty, &DataManager::sessionTrackPropertyCount,
            &DataManager::atSessionTrackProperty, &DataManager::clearSessionTrackProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of SessionTrack*
void DataManager::appendToSessionTrackProperty(
        QQmlListProperty<SessionTrack> *sessionTrackList,
        SessionTrack* sessionTrack)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(sessionTrackList->object);
    if (dataManagerObject) {
        sessionTrack->setParent(dataManagerObject);
        dataManagerObject->mAllSessionTrack.append(sessionTrack);
        emit dataManagerObject->addedToAllSessionTrack(sessionTrack);
    } else {
        qWarning() << "cannot append SessionTrack* to mAllSessionTrack "
                << "Object is not of type DataManager*";
    }
}
int DataManager::sessionTrackPropertyCount(
        QQmlListProperty<SessionTrack> *sessionTrackList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionTrackList->object);
    if (dataManager) {
        return dataManager->mAllSessionTrack.size();
    } else {
        qWarning() << "cannot get size mAllSessionTrack " << "Object is not of type DataManager*";
    }
    return 0;
}
SessionTrack* DataManager::atSessionTrackProperty(
        QQmlListProperty<SessionTrack> *sessionTrackList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionTrackList->object);
    if (dataManager) {
        if (dataManager->mAllSessionTrack.size() > pos) {
            return (SessionTrack*) dataManager->mAllSessionTrack.at(pos);
        }
        qWarning() << "cannot get SessionTrack* at pos " << pos << " size is "
                << dataManager->mAllSessionTrack.size();
    } else {
        qWarning() << "cannot get SessionTrack* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearSessionTrackProperty(
        QQmlListProperty<SessionTrack> *sessionTrackList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionTrackList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllSessionTrack.size(); ++i) {
            SessionTrack* sessionTrack;
            sessionTrack = (SessionTrack*) dataManager->mAllSessionTrack.at(i);
			emit dataManager->deletedFromAllSessionTrackByTrackId(sessionTrack->trackId());
			emit dataManager->deletedFromAllSessionTrack(sessionTrack);
            sessionTrack->deleteLater();
            sessionTrack = 0;
        }
        dataManager->mAllSessionTrack.clear();
    } else {
        qWarning() << "cannot clear mAllSessionTrack " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all SessionTrack
 * and clears the list
 */
void DataManager::deleteSessionTrack()
{
    for (int i = 0; i < mAllSessionTrack.size(); ++i) {
        SessionTrack* sessionTrack;
        sessionTrack = (SessionTrack*) mAllSessionTrack.at(i);
        emit deletedFromAllSessionTrackByTrackId(sessionTrack->trackId());
		emit deletedFromAllSessionTrack(sessionTrack);
		emit sessionTrackPropertyListChanged();
        sessionTrack->deleteLater();
        sessionTrack = 0;
     }
     mAllSessionTrack.clear();
}

/**
 * creates a new SessionTrack
 * parent is DataManager
 * if data is successfully entered you must insertSessionTrack
 * if edit was canceled you must undoCreateSessionTrack to free up memory
 */
SessionTrack* DataManager::createSessionTrack()
{
    SessionTrack* sessionTrack;
    sessionTrack = new SessionTrack();
    sessionTrack->setParent(this);
    sessionTrack->prepareNew();
    return sessionTrack;
}

/**
 * deletes SessionTrack
 * if createSessionTrack was canceled from UI
 * to delete a previous successfully inserted SessionTrack
 * use deleteSessionTrack
 */
void DataManager::undoCreateSessionTrack(SessionTrack* sessionTrack)
{
    if (sessionTrack) {
        qDebug() << "undoCreateSessionTrack " << sessionTrack->trackId();
        sessionTrack->deleteLater();
        sessionTrack = 0;
    }
}

void DataManager::insertSessionTrack(SessionTrack* sessionTrack)
{
    // Important: DataManager must be parent of all root DTOs
    sessionTrack->setParent(this);
    mAllSessionTrack.append(sessionTrack);
    emit addedToAllSessionTrack(sessionTrack);
    emit sessionTrackPropertyListChanged();
}

void DataManager::insertSessionTrackFromMap(const QVariantMap& sessionTrackMap,
        const bool& useForeignProperties)
{
    SessionTrack* sessionTrack = new SessionTrack();
    sessionTrack->setParent(this);
    if (useForeignProperties) {
        sessionTrack->fillFromForeignMap(sessionTrackMap);
    } else {
        sessionTrack->fillFromMap(sessionTrackMap);
    }
    mAllSessionTrack.append(sessionTrack);
    emit addedToAllSessionTrack(sessionTrack);
    sessionTrackPropertyListChanged();
}

bool DataManager::deleteSessionTrack(SessionTrack* sessionTrack)
{
    bool ok = false;
    ok = mAllSessionTrack.removeOne(sessionTrack);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllSessionTrackByTrackId(sessionTrack->trackId());
    emit deletedFromAllSessionTrack(sessionTrack);
    emit sessionTrackPropertyListChanged();
    sessionTrack->deleteLater();
    sessionTrack = 0;
    return ok;
}


bool DataManager::deleteSessionTrackByTrackId(const int& trackId)
{
    for (int i = 0; i < mAllSessionTrack.size(); ++i) {
        SessionTrack* sessionTrack;
        sessionTrack = (SessionTrack*) mAllSessionTrack.at(i);
        if (sessionTrack->trackId() == trackId) {
            mAllSessionTrack.removeAt(i);
            emit deletedFromAllSessionTrackByTrackId(trackId);
            emit deletedFromAllSessionTrack(sessionTrack);
            emit sessionTrackPropertyListChanged();
            sessionTrack->deleteLater();
            sessionTrack = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
SessionTrack* DataManager::findSessionTrackByTrackId(const int& trackId){
    for (int i = 0; i < mAllSessionTrack.size(); ++i) {
        SessionTrack* sessionTrack;
        sessionTrack = (SessionTrack*)mAllSessionTrack.at(i);
        if(sessionTrack->trackId() == trackId){
            return sessionTrack;
        }
    }
    qDebug() << "no SessionTrack found for trackId " << trackId;
    return 0;
}

/*
 * reads Maps of Day in from JSON cache
 * creates List of Day*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initDayFromCache()
{
	qDebug() << "start initDayFromCache";
    mAllDay.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheDay);
    qDebug() << "read Day from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Day* day = new Day();
        // Important: DataManager must be parent of all root DTOs
        day->setParent(this);
        day->fillFromCacheMap(cacheMap);
        mAllDay.append(day);
    }
    qDebug() << "created Day* #" << mAllDay.size();
}


/*
 * save List of Day* to JSON cache
 * convert list of Day* to QVariantList
 * toCacheMap stores all properties without transient values
 * Day is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveDayToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Day* #" << mAllDay.size();
    for (int i = 0; i < mAllDay.size(); ++i) {
        Day* day;
        day = (Day*)mAllDay.at(i);
        QVariantMap cacheMap;
        cacheMap = day->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Day* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheDay, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Day*> DataManager::listOfDayForKeys(
        QStringList keyList)
{
    QList<Day*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllDay.size(); ++i) {
        Day* day;
        day = (Day*) mAllDay.at(i);
        if (keyList.contains(QString::number(day->id()))) {
            listOfData.append(day);
            keyList.removeOne(QString::number(day->id()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Day: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::dayAsQVariantList()
{
    QVariantList dayList;
    for (int i = 0; i < mAllDay.size(); ++i) {
        dayList.append(((Day*) (mAllDay.at(i)))->toMap());
    }
    return dayList;
}

QList<QObject*> DataManager::allDay()
{
    return mAllDay;
}

QQmlListProperty<Day> DataManager::dayPropertyList()
{
    return QQmlListProperty<Day>(this, 0,
            &DataManager::appendToDayProperty, &DataManager::dayPropertyCount,
            &DataManager::atDayProperty, &DataManager::clearDayProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Day*
void DataManager::appendToDayProperty(
        QQmlListProperty<Day> *dayList,
        Day* day)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(dayList->object);
    if (dataManagerObject) {
        day->setParent(dataManagerObject);
        dataManagerObject->mAllDay.append(day);
        emit dataManagerObject->addedToAllDay(day);
    } else {
        qWarning() << "cannot append Day* to mAllDay "
                << "Object is not of type DataManager*";
    }
}
int DataManager::dayPropertyCount(
        QQmlListProperty<Day> *dayList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(dayList->object);
    if (dataManager) {
        return dataManager->mAllDay.size();
    } else {
        qWarning() << "cannot get size mAllDay " << "Object is not of type DataManager*";
    }
    return 0;
}
Day* DataManager::atDayProperty(
        QQmlListProperty<Day> *dayList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(dayList->object);
    if (dataManager) {
        if (dataManager->mAllDay.size() > pos) {
            return (Day*) dataManager->mAllDay.at(pos);
        }
        qWarning() << "cannot get Day* at pos " << pos << " size is "
                << dataManager->mAllDay.size();
    } else {
        qWarning() << "cannot get Day* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearDayProperty(
        QQmlListProperty<Day> *dayList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(dayList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllDay.size(); ++i) {
            Day* day;
            day = (Day*) dataManager->mAllDay.at(i);
			emit dataManager->deletedFromAllDayById(day->id());
			emit dataManager->deletedFromAllDay(day);
            day->deleteLater();
            day = 0;
        }
        dataManager->mAllDay.clear();
    } else {
        qWarning() << "cannot clear mAllDay " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Day
 * and clears the list
 */
void DataManager::deleteDay()
{
    for (int i = 0; i < mAllDay.size(); ++i) {
        Day* day;
        day = (Day*) mAllDay.at(i);
        emit deletedFromAllDayById(day->id());
		emit deletedFromAllDay(day);
		emit dayPropertyListChanged();
        day->deleteLater();
        day = 0;
     }
     mAllDay.clear();
}

/**
 * creates a new Day
 * parent is DataManager
 * if data is successfully entered you must insertDay
 * if edit was canceled you must undoCreateDay to free up memory
 */
Day* DataManager::createDay()
{
    Day* day;
    day = new Day();
    day->setParent(this);
    day->prepareNew();
    return day;
}

/**
 * deletes Day
 * if createDay was canceled from UI
 * to delete a previous successfully inserted Day
 * use deleteDay
 */
void DataManager::undoCreateDay(Day* day)
{
    if (day) {
        qDebug() << "undoCreateDay " << day->id();
        day->deleteLater();
        day = 0;
    }
}

void DataManager::insertDay(Day* day)
{
    // Important: DataManager must be parent of all root DTOs
    day->setParent(this);
    mAllDay.append(day);
    emit addedToAllDay(day);
    emit dayPropertyListChanged();
}

void DataManager::insertDayFromMap(const QVariantMap& dayMap,
        const bool& useForeignProperties)
{
    Day* day = new Day();
    day->setParent(this);
    if (useForeignProperties) {
        day->fillFromForeignMap(dayMap);
    } else {
        day->fillFromMap(dayMap);
    }
    mAllDay.append(day);
    emit addedToAllDay(day);
    dayPropertyListChanged();
}

bool DataManager::deleteDay(Day* day)
{
    bool ok = false;
    ok = mAllDay.removeOne(day);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllDayById(day->id());
    emit deletedFromAllDay(day);
    emit dayPropertyListChanged();
    day->deleteLater();
    day = 0;
    return ok;
}


bool DataManager::deleteDayById(const int& id)
{
    for (int i = 0; i < mAllDay.size(); ++i) {
        Day* day;
        day = (Day*) mAllDay.at(i);
        if (day->id() == id) {
            mAllDay.removeAt(i);
            emit deletedFromAllDayById(id);
            emit deletedFromAllDay(day);
            emit dayPropertyListChanged();
            day->deleteLater();
            day = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Day* DataManager::findDayById(const int& id){
    for (int i = 0; i < mAllDay.size(); ++i) {
        Day* day;
        day = (Day*)mAllDay.at(i);
        if(day->id() == id){
            return day;
        }
    }
    qDebug() << "no Day found for id " << id;
    return 0;
}

/*
 * reads Maps of SessionAPI in from JSON cache
 * creates List of SessionAPI*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initSessionAPIFromCache()
{
	qDebug() << "start initSessionAPIFromCache";
    mAllSessionAPI.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheSessionAPI);
    qDebug() << "read SessionAPI from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        SessionAPI* sessionAPI = new SessionAPI();
        // Important: DataManager must be parent of all root DTOs
        sessionAPI->setParent(this);
        sessionAPI->fillFromCacheMap(cacheMap);
        mAllSessionAPI.append(sessionAPI);
    }
    qDebug() << "created SessionAPI* #" << mAllSessionAPI.size();
}


/*
 * save List of SessionAPI* to JSON cache
 * convert list of SessionAPI* to QVariantList
 * toCacheMap stores all properties without transient values
 * SessionAPI is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveSessionAPIToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching SessionAPI* #" << mAllSessionAPI.size();
    for (int i = 0; i < mAllSessionAPI.size(); ++i) {
        SessionAPI* sessionAPI;
        sessionAPI = (SessionAPI*)mAllSessionAPI.at(i);
        QVariantMap cacheMap;
        cacheMap = sessionAPI->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "SessionAPI* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheSessionAPI, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<SessionAPI*> DataManager::listOfSessionAPIForKeys(
        QStringList keyList)
{
    QList<SessionAPI*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllSessionAPI.size(); ++i) {
        SessionAPI* sessionAPI;
        sessionAPI = (SessionAPI*) mAllSessionAPI.at(i);
        if (keyList.contains(QString::number(sessionAPI->sessionId()))) {
            listOfData.append(sessionAPI);
            keyList.removeOne(QString::number(sessionAPI->sessionId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for SessionAPI: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::sessionAPIAsQVariantList()
{
    QVariantList sessionAPIList;
    for (int i = 0; i < mAllSessionAPI.size(); ++i) {
        sessionAPIList.append(((SessionAPI*) (mAllSessionAPI.at(i)))->toMap());
    }
    return sessionAPIList;
}

QList<QObject*> DataManager::allSessionAPI()
{
    return mAllSessionAPI;
}

QQmlListProperty<SessionAPI> DataManager::sessionAPIPropertyList()
{
    return QQmlListProperty<SessionAPI>(this, 0,
            &DataManager::appendToSessionAPIProperty, &DataManager::sessionAPIPropertyCount,
            &DataManager::atSessionAPIProperty, &DataManager::clearSessionAPIProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of SessionAPI*
void DataManager::appendToSessionAPIProperty(
        QQmlListProperty<SessionAPI> *sessionAPIList,
        SessionAPI* sessionAPI)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(sessionAPIList->object);
    if (dataManagerObject) {
        sessionAPI->setParent(dataManagerObject);
        dataManagerObject->mAllSessionAPI.append(sessionAPI);
        emit dataManagerObject->addedToAllSessionAPI(sessionAPI);
    } else {
        qWarning() << "cannot append SessionAPI* to mAllSessionAPI "
                << "Object is not of type DataManager*";
    }
}
int DataManager::sessionAPIPropertyCount(
        QQmlListProperty<SessionAPI> *sessionAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionAPIList->object);
    if (dataManager) {
        return dataManager->mAllSessionAPI.size();
    } else {
        qWarning() << "cannot get size mAllSessionAPI " << "Object is not of type DataManager*";
    }
    return 0;
}
SessionAPI* DataManager::atSessionAPIProperty(
        QQmlListProperty<SessionAPI> *sessionAPIList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionAPIList->object);
    if (dataManager) {
        if (dataManager->mAllSessionAPI.size() > pos) {
            return (SessionAPI*) dataManager->mAllSessionAPI.at(pos);
        }
        qWarning() << "cannot get SessionAPI* at pos " << pos << " size is "
                << dataManager->mAllSessionAPI.size();
    } else {
        qWarning() << "cannot get SessionAPI* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearSessionAPIProperty(
        QQmlListProperty<SessionAPI> *sessionAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionAPIList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllSessionAPI.size(); ++i) {
            SessionAPI* sessionAPI;
            sessionAPI = (SessionAPI*) dataManager->mAllSessionAPI.at(i);
			emit dataManager->deletedFromAllSessionAPIBySessionId(sessionAPI->sessionId());
			emit dataManager->deletedFromAllSessionAPI(sessionAPI);
            sessionAPI->deleteLater();
            sessionAPI = 0;
        }
        dataManager->mAllSessionAPI.clear();
    } else {
        qWarning() << "cannot clear mAllSessionAPI " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all SessionAPI
 * and clears the list
 */
void DataManager::deleteSessionAPI()
{
    for (int i = 0; i < mAllSessionAPI.size(); ++i) {
        SessionAPI* sessionAPI;
        sessionAPI = (SessionAPI*) mAllSessionAPI.at(i);
        emit deletedFromAllSessionAPIBySessionId(sessionAPI->sessionId());
		emit deletedFromAllSessionAPI(sessionAPI);
		emit sessionAPIPropertyListChanged();
        sessionAPI->deleteLater();
        sessionAPI = 0;
     }
     mAllSessionAPI.clear();
}

/**
 * creates a new SessionAPI
 * parent is DataManager
 * if data is successfully entered you must insertSessionAPI
 * if edit was canceled you must undoCreateSessionAPI to free up memory
 */
SessionAPI* DataManager::createSessionAPI()
{
    SessionAPI* sessionAPI;
    sessionAPI = new SessionAPI();
    sessionAPI->setParent(this);
    sessionAPI->prepareNew();
    return sessionAPI;
}

/**
 * deletes SessionAPI
 * if createSessionAPI was canceled from UI
 * to delete a previous successfully inserted SessionAPI
 * use deleteSessionAPI
 */
void DataManager::undoCreateSessionAPI(SessionAPI* sessionAPI)
{
    if (sessionAPI) {
        qDebug() << "undoCreateSessionAPI " << sessionAPI->sessionId();
        sessionAPI->deleteLater();
        sessionAPI = 0;
    }
}

void DataManager::insertSessionAPI(SessionAPI* sessionAPI)
{
    // Important: DataManager must be parent of all root DTOs
    sessionAPI->setParent(this);
    mAllSessionAPI.append(sessionAPI);
    emit addedToAllSessionAPI(sessionAPI);
    emit sessionAPIPropertyListChanged();
}

void DataManager::insertSessionAPIFromMap(const QVariantMap& sessionAPIMap,
        const bool& useForeignProperties)
{
    SessionAPI* sessionAPI = new SessionAPI();
    sessionAPI->setParent(this);
    if (useForeignProperties) {
        sessionAPI->fillFromForeignMap(sessionAPIMap);
    } else {
        sessionAPI->fillFromMap(sessionAPIMap);
    }
    mAllSessionAPI.append(sessionAPI);
    emit addedToAllSessionAPI(sessionAPI);
    sessionAPIPropertyListChanged();
}

bool DataManager::deleteSessionAPI(SessionAPI* sessionAPI)
{
    bool ok = false;
    ok = mAllSessionAPI.removeOne(sessionAPI);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllSessionAPIBySessionId(sessionAPI->sessionId());
    emit deletedFromAllSessionAPI(sessionAPI);
    emit sessionAPIPropertyListChanged();
    sessionAPI->deleteLater();
    sessionAPI = 0;
    return ok;
}


bool DataManager::deleteSessionAPIBySessionId(const int& sessionId)
{
    for (int i = 0; i < mAllSessionAPI.size(); ++i) {
        SessionAPI* sessionAPI;
        sessionAPI = (SessionAPI*) mAllSessionAPI.at(i);
        if (sessionAPI->sessionId() == sessionId) {
            mAllSessionAPI.removeAt(i);
            emit deletedFromAllSessionAPIBySessionId(sessionId);
            emit deletedFromAllSessionAPI(sessionAPI);
            emit sessionAPIPropertyListChanged();
            sessionAPI->deleteLater();
            sessionAPI = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
SessionAPI* DataManager::findSessionAPIBySessionId(const int& sessionId){
    for (int i = 0; i < mAllSessionAPI.size(); ++i) {
        SessionAPI* sessionAPI;
        sessionAPI = (SessionAPI*)mAllSessionAPI.at(i);
        if(sessionAPI->sessionId() == sessionId){
            return sessionAPI;
        }
    }
    qDebug() << "no SessionAPI found for sessionId " << sessionId;
    return 0;
}

/*
 * reads Maps of PersonsAPI in from JSON cache
 * creates List of PersonsAPI*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initPersonsAPIFromCache()
{
	qDebug() << "start initPersonsAPIFromCache";
    mAllPersonsAPI.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cachePersonsAPI);
    qDebug() << "read PersonsAPI from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        PersonsAPI* personsAPI = new PersonsAPI();
        // Important: DataManager must be parent of all root DTOs
        personsAPI->setParent(this);
        personsAPI->fillFromCacheMap(cacheMap);
        mAllPersonsAPI.append(personsAPI);
    }
    qDebug() << "created PersonsAPI* #" << mAllPersonsAPI.size();
}


/*
 * save List of PersonsAPI* to JSON cache
 * convert list of PersonsAPI* to QVariantList
 * toCacheMap stores all properties without transient values
 * PersonsAPI is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::savePersonsAPIToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching PersonsAPI* #" << mAllPersonsAPI.size();
    for (int i = 0; i < mAllPersonsAPI.size(); ++i) {
        PersonsAPI* personsAPI;
        personsAPI = (PersonsAPI*)mAllPersonsAPI.at(i);
        QVariantMap cacheMap;
        cacheMap = personsAPI->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "PersonsAPI* converted to JSON cache #" << cacheList.size();
    writeToCache(cachePersonsAPI, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<PersonsAPI*> DataManager::listOfPersonsAPIForKeys(
        QStringList keyList)
{
    QList<PersonsAPI*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllPersonsAPI.size(); ++i) {
        PersonsAPI* personsAPI;
        personsAPI = (PersonsAPI*) mAllPersonsAPI.at(i);
        if (keyList.contains(QString::number(personsAPI->speakerId()))) {
            listOfData.append(personsAPI);
            keyList.removeOne(QString::number(personsAPI->speakerId()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for PersonsAPI: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::personsAPIAsQVariantList()
{
    QVariantList personsAPIList;
    for (int i = 0; i < mAllPersonsAPI.size(); ++i) {
        personsAPIList.append(((PersonsAPI*) (mAllPersonsAPI.at(i)))->toMap());
    }
    return personsAPIList;
}

QList<QObject*> DataManager::allPersonsAPI()
{
    return mAllPersonsAPI;
}

QQmlListProperty<PersonsAPI> DataManager::personsAPIPropertyList()
{
    return QQmlListProperty<PersonsAPI>(this, 0,
            &DataManager::appendToPersonsAPIProperty, &DataManager::personsAPIPropertyCount,
            &DataManager::atPersonsAPIProperty, &DataManager::clearPersonsAPIProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of PersonsAPI*
void DataManager::appendToPersonsAPIProperty(
        QQmlListProperty<PersonsAPI> *personsAPIList,
        PersonsAPI* personsAPI)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(personsAPIList->object);
    if (dataManagerObject) {
        personsAPI->setParent(dataManagerObject);
        dataManagerObject->mAllPersonsAPI.append(personsAPI);
        emit dataManagerObject->addedToAllPersonsAPI(personsAPI);
    } else {
        qWarning() << "cannot append PersonsAPI* to mAllPersonsAPI "
                << "Object is not of type DataManager*";
    }
}
int DataManager::personsAPIPropertyCount(
        QQmlListProperty<PersonsAPI> *personsAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(personsAPIList->object);
    if (dataManager) {
        return dataManager->mAllPersonsAPI.size();
    } else {
        qWarning() << "cannot get size mAllPersonsAPI " << "Object is not of type DataManager*";
    }
    return 0;
}
PersonsAPI* DataManager::atPersonsAPIProperty(
        QQmlListProperty<PersonsAPI> *personsAPIList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(personsAPIList->object);
    if (dataManager) {
        if (dataManager->mAllPersonsAPI.size() > pos) {
            return (PersonsAPI*) dataManager->mAllPersonsAPI.at(pos);
        }
        qWarning() << "cannot get PersonsAPI* at pos " << pos << " size is "
                << dataManager->mAllPersonsAPI.size();
    } else {
        qWarning() << "cannot get PersonsAPI* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearPersonsAPIProperty(
        QQmlListProperty<PersonsAPI> *personsAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(personsAPIList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllPersonsAPI.size(); ++i) {
            PersonsAPI* personsAPI;
            personsAPI = (PersonsAPI*) dataManager->mAllPersonsAPI.at(i);
			emit dataManager->deletedFromAllPersonsAPIBySpeakerId(personsAPI->speakerId());
			emit dataManager->deletedFromAllPersonsAPI(personsAPI);
            personsAPI->deleteLater();
            personsAPI = 0;
        }
        dataManager->mAllPersonsAPI.clear();
    } else {
        qWarning() << "cannot clear mAllPersonsAPI " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all PersonsAPI
 * and clears the list
 */
void DataManager::deletePersonsAPI()
{
    for (int i = 0; i < mAllPersonsAPI.size(); ++i) {
        PersonsAPI* personsAPI;
        personsAPI = (PersonsAPI*) mAllPersonsAPI.at(i);
        emit deletedFromAllPersonsAPIBySpeakerId(personsAPI->speakerId());
		emit deletedFromAllPersonsAPI(personsAPI);
		emit personsAPIPropertyListChanged();
        personsAPI->deleteLater();
        personsAPI = 0;
     }
     mAllPersonsAPI.clear();
}

/**
 * creates a new PersonsAPI
 * parent is DataManager
 * if data is successfully entered you must insertPersonsAPI
 * if edit was canceled you must undoCreatePersonsAPI to free up memory
 */
PersonsAPI* DataManager::createPersonsAPI()
{
    PersonsAPI* personsAPI;
    personsAPI = new PersonsAPI();
    personsAPI->setParent(this);
    personsAPI->prepareNew();
    return personsAPI;
}

/**
 * deletes PersonsAPI
 * if createPersonsAPI was canceled from UI
 * to delete a previous successfully inserted PersonsAPI
 * use deletePersonsAPI
 */
void DataManager::undoCreatePersonsAPI(PersonsAPI* personsAPI)
{
    if (personsAPI) {
        qDebug() << "undoCreatePersonsAPI " << personsAPI->speakerId();
        personsAPI->deleteLater();
        personsAPI = 0;
    }
}

void DataManager::insertPersonsAPI(PersonsAPI* personsAPI)
{
    // Important: DataManager must be parent of all root DTOs
    personsAPI->setParent(this);
    mAllPersonsAPI.append(personsAPI);
    emit addedToAllPersonsAPI(personsAPI);
    emit personsAPIPropertyListChanged();
}

void DataManager::insertPersonsAPIFromMap(const QVariantMap& personsAPIMap,
        const bool& useForeignProperties)
{
    PersonsAPI* personsAPI = new PersonsAPI();
    personsAPI->setParent(this);
    if (useForeignProperties) {
        personsAPI->fillFromForeignMap(personsAPIMap);
    } else {
        personsAPI->fillFromMap(personsAPIMap);
    }
    mAllPersonsAPI.append(personsAPI);
    emit addedToAllPersonsAPI(personsAPI);
    personsAPIPropertyListChanged();
}

bool DataManager::deletePersonsAPI(PersonsAPI* personsAPI)
{
    bool ok = false;
    ok = mAllPersonsAPI.removeOne(personsAPI);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllPersonsAPIBySpeakerId(personsAPI->speakerId());
    emit deletedFromAllPersonsAPI(personsAPI);
    emit personsAPIPropertyListChanged();
    personsAPI->deleteLater();
    personsAPI = 0;
    return ok;
}


bool DataManager::deletePersonsAPIBySpeakerId(const int& speakerId)
{
    for (int i = 0; i < mAllPersonsAPI.size(); ++i) {
        PersonsAPI* personsAPI;
        personsAPI = (PersonsAPI*) mAllPersonsAPI.at(i);
        if (personsAPI->speakerId() == speakerId) {
            mAllPersonsAPI.removeAt(i);
            emit deletedFromAllPersonsAPIBySpeakerId(speakerId);
            emit deletedFromAllPersonsAPI(personsAPI);
            emit personsAPIPropertyListChanged();
            personsAPI->deleteLater();
            personsAPI = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
PersonsAPI* DataManager::findPersonsAPIBySpeakerId(const int& speakerId){
    for (int i = 0; i < mAllPersonsAPI.size(); ++i) {
        PersonsAPI* personsAPI;
        personsAPI = (PersonsAPI*)mAllPersonsAPI.at(i);
        if(personsAPI->speakerId() == speakerId){
            return personsAPI;
        }
    }
    qDebug() << "no PersonsAPI found for speakerId " << speakerId;
    return 0;
}

/*
 * reads Maps of SessionLinkAPI in from JSON cache
 * creates List of SessionLinkAPI*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initSessionLinkAPIFromCache()
{
	qDebug() << "start initSessionLinkAPIFromCache";
    mAllSessionLinkAPI.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheSessionLinkAPI);
    qDebug() << "read SessionLinkAPI from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        SessionLinkAPI* sessionLinkAPI = new SessionLinkAPI();
        // Important: DataManager must be parent of all root DTOs
        sessionLinkAPI->setParent(this);
        sessionLinkAPI->fillFromCacheMap(cacheMap);
        mAllSessionLinkAPI.append(sessionLinkAPI);
    }
    qDebug() << "created SessionLinkAPI* #" << mAllSessionLinkAPI.size();
}


/*
 * save List of SessionLinkAPI* to JSON cache
 * convert list of SessionLinkAPI* to QVariantList
 * toCacheMap stores all properties without transient values
 * SessionLinkAPI is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveSessionLinkAPIToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching SessionLinkAPI* #" << mAllSessionLinkAPI.size();
    for (int i = 0; i < mAllSessionLinkAPI.size(); ++i) {
        SessionLinkAPI* sessionLinkAPI;
        sessionLinkAPI = (SessionLinkAPI*)mAllSessionLinkAPI.at(i);
        QVariantMap cacheMap;
        cacheMap = sessionLinkAPI->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "SessionLinkAPI* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheSessionLinkAPI, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<SessionLinkAPI*> DataManager::listOfSessionLinkAPIForKeys(
        QStringList keyList)
{
    QList<SessionLinkAPI*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllSessionLinkAPI.size(); ++i) {
        SessionLinkAPI* sessionLinkAPI;
        sessionLinkAPI = (SessionLinkAPI*) mAllSessionLinkAPI.at(i);
        if (keyList.contains(sessionLinkAPI->uuid())) {
            listOfData.append(sessionLinkAPI);
            keyList.removeOne(sessionLinkAPI->uuid());
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for SessionLinkAPI: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::sessionLinkAPIAsQVariantList()
{
    QVariantList sessionLinkAPIList;
    for (int i = 0; i < mAllSessionLinkAPI.size(); ++i) {
        sessionLinkAPIList.append(((SessionLinkAPI*) (mAllSessionLinkAPI.at(i)))->toMap());
    }
    return sessionLinkAPIList;
}

QList<QObject*> DataManager::allSessionLinkAPI()
{
    return mAllSessionLinkAPI;
}

QQmlListProperty<SessionLinkAPI> DataManager::sessionLinkAPIPropertyList()
{
    return QQmlListProperty<SessionLinkAPI>(this, 0,
            &DataManager::appendToSessionLinkAPIProperty, &DataManager::sessionLinkAPIPropertyCount,
            &DataManager::atSessionLinkAPIProperty, &DataManager::clearSessionLinkAPIProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of SessionLinkAPI*
void DataManager::appendToSessionLinkAPIProperty(
        QQmlListProperty<SessionLinkAPI> *sessionLinkAPIList,
        SessionLinkAPI* sessionLinkAPI)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(sessionLinkAPIList->object);
    if (dataManagerObject) {
        sessionLinkAPI->setParent(dataManagerObject);
        dataManagerObject->mAllSessionLinkAPI.append(sessionLinkAPI);
        emit dataManagerObject->addedToAllSessionLinkAPI(sessionLinkAPI);
    } else {
        qWarning() << "cannot append SessionLinkAPI* to mAllSessionLinkAPI "
                << "Object is not of type DataManager*";
    }
}
int DataManager::sessionLinkAPIPropertyCount(
        QQmlListProperty<SessionLinkAPI> *sessionLinkAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionLinkAPIList->object);
    if (dataManager) {
        return dataManager->mAllSessionLinkAPI.size();
    } else {
        qWarning() << "cannot get size mAllSessionLinkAPI " << "Object is not of type DataManager*";
    }
    return 0;
}
SessionLinkAPI* DataManager::atSessionLinkAPIProperty(
        QQmlListProperty<SessionLinkAPI> *sessionLinkAPIList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionLinkAPIList->object);
    if (dataManager) {
        if (dataManager->mAllSessionLinkAPI.size() > pos) {
            return (SessionLinkAPI*) dataManager->mAllSessionLinkAPI.at(pos);
        }
        qWarning() << "cannot get SessionLinkAPI* at pos " << pos << " size is "
                << dataManager->mAllSessionLinkAPI.size();
    } else {
        qWarning() << "cannot get SessionLinkAPI* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearSessionLinkAPIProperty(
        QQmlListProperty<SessionLinkAPI> *sessionLinkAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(sessionLinkAPIList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllSessionLinkAPI.size(); ++i) {
            SessionLinkAPI* sessionLinkAPI;
            sessionLinkAPI = (SessionLinkAPI*) dataManager->mAllSessionLinkAPI.at(i);
			emit dataManager->deletedFromAllSessionLinkAPIByUuid(sessionLinkAPI->uuid());
			emit dataManager->deletedFromAllSessionLinkAPI(sessionLinkAPI);
            sessionLinkAPI->deleteLater();
            sessionLinkAPI = 0;
        }
        dataManager->mAllSessionLinkAPI.clear();
    } else {
        qWarning() << "cannot clear mAllSessionLinkAPI " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all SessionLinkAPI
 * and clears the list
 */
void DataManager::deleteSessionLinkAPI()
{
    for (int i = 0; i < mAllSessionLinkAPI.size(); ++i) {
        SessionLinkAPI* sessionLinkAPI;
        sessionLinkAPI = (SessionLinkAPI*) mAllSessionLinkAPI.at(i);
        emit deletedFromAllSessionLinkAPIByUuid(sessionLinkAPI->uuid());
		emit deletedFromAllSessionLinkAPI(sessionLinkAPI);
		emit sessionLinkAPIPropertyListChanged();
        sessionLinkAPI->deleteLater();
        sessionLinkAPI = 0;
     }
     mAllSessionLinkAPI.clear();
}

/**
 * creates a new SessionLinkAPI
 * parent is DataManager
 * if data is successfully entered you must insertSessionLinkAPI
 * if edit was canceled you must undoCreateSessionLinkAPI to free up memory
 */
SessionLinkAPI* DataManager::createSessionLinkAPI()
{
    SessionLinkAPI* sessionLinkAPI;
    sessionLinkAPI = new SessionLinkAPI();
    sessionLinkAPI->setParent(this);
    sessionLinkAPI->prepareNew();
    return sessionLinkAPI;
}

/**
 * deletes SessionLinkAPI
 * if createSessionLinkAPI was canceled from UI
 * to delete a previous successfully inserted SessionLinkAPI
 * use deleteSessionLinkAPI
 */
void DataManager::undoCreateSessionLinkAPI(SessionLinkAPI* sessionLinkAPI)
{
    if (sessionLinkAPI) {
        qDebug() << "undoCreateSessionLinkAPI " << sessionLinkAPI->uuid();
        sessionLinkAPI->deleteLater();
        sessionLinkAPI = 0;
    }
}

void DataManager::insertSessionLinkAPI(SessionLinkAPI* sessionLinkAPI)
{
    // Important: DataManager must be parent of all root DTOs
    sessionLinkAPI->setParent(this);
    mAllSessionLinkAPI.append(sessionLinkAPI);
    emit addedToAllSessionLinkAPI(sessionLinkAPI);
    emit sessionLinkAPIPropertyListChanged();
}

void DataManager::insertSessionLinkAPIFromMap(const QVariantMap& sessionLinkAPIMap,
        const bool& useForeignProperties)
{
    SessionLinkAPI* sessionLinkAPI = new SessionLinkAPI();
    sessionLinkAPI->setParent(this);
    if (useForeignProperties) {
        sessionLinkAPI->fillFromForeignMap(sessionLinkAPIMap);
    } else {
        sessionLinkAPI->fillFromMap(sessionLinkAPIMap);
    }
    mAllSessionLinkAPI.append(sessionLinkAPI);
    emit addedToAllSessionLinkAPI(sessionLinkAPI);
    sessionLinkAPIPropertyListChanged();
}

bool DataManager::deleteSessionLinkAPI(SessionLinkAPI* sessionLinkAPI)
{
    bool ok = false;
    ok = mAllSessionLinkAPI.removeOne(sessionLinkAPI);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllSessionLinkAPIByUuid(sessionLinkAPI->uuid());
    emit deletedFromAllSessionLinkAPI(sessionLinkAPI);
    emit sessionLinkAPIPropertyListChanged();
    sessionLinkAPI->deleteLater();
    sessionLinkAPI = 0;
    return ok;
}

bool DataManager::deleteSessionLinkAPIByUuid(const QString& uuid)
{
    if (uuid.isNull() || uuid.isEmpty()) {
        qDebug() << "cannot delete SessionLinkAPI from empty uuid";
        return false;
    }
    for (int i = 0; i < mAllSessionLinkAPI.size(); ++i) {
        SessionLinkAPI* sessionLinkAPI;
        sessionLinkAPI = (SessionLinkAPI*) mAllSessionLinkAPI.at(i);
        if (sessionLinkAPI->uuid() == uuid) {
            mAllSessionLinkAPI.removeAt(i);
            emit deletedFromAllSessionLinkAPIByUuid(uuid);
            emit deletedFromAllSessionLinkAPI(sessionLinkAPI);
            emit sessionLinkAPIPropertyListChanged();
            sessionLinkAPI->deleteLater();
            sessionLinkAPI = 0;
            return true;
        }
    }
    return false;
}


SessionLinkAPI* DataManager::findSessionLinkAPIByUuid(const QString& uuid){
    if (uuid.isNull() || uuid.isEmpty()) {
        qDebug() << "cannot find SessionLinkAPI from empty uuid";
        return 0;
    }
    for (int i = 0; i < mAllSessionLinkAPI.size(); ++i) {
        SessionLinkAPI* sessionLinkAPI;
        sessionLinkAPI = (SessionLinkAPI*)mAllSessionLinkAPI.at(i);
        if(sessionLinkAPI->uuid() == uuid){
            return sessionLinkAPI;
        }
    }
    qDebug() << "no SessionLinkAPI found for uuid " << uuid;
    return 0;
}


/*
 * reads Maps of SpeakerAPI in from JSON cache
 * creates List of SpeakerAPI*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initSpeakerAPIFromCache()
{
	qDebug() << "start initSpeakerAPIFromCache";
    mAllSpeakerAPI.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheSpeakerAPI);
    qDebug() << "read SpeakerAPI from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        SpeakerAPI* speakerAPI = new SpeakerAPI();
        // Important: DataManager must be parent of all root DTOs
        speakerAPI->setParent(this);
        speakerAPI->fillFromCacheMap(cacheMap);
        mAllSpeakerAPI.append(speakerAPI);
    }
    qDebug() << "created SpeakerAPI* #" << mAllSpeakerAPI.size();
}


/*
 * save List of SpeakerAPI* to JSON cache
 * convert list of SpeakerAPI* to QVariantList
 * toCacheMap stores all properties without transient values
 * SpeakerAPI is read-only Cache - so it's not saved automatically at exit
 */
void DataManager::saveSpeakerAPIToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching SpeakerAPI* #" << mAllSpeakerAPI.size();
    for (int i = 0; i < mAllSpeakerAPI.size(); ++i) {
        SpeakerAPI* speakerAPI;
        speakerAPI = (SpeakerAPI*)mAllSpeakerAPI.at(i);
        QVariantMap cacheMap;
        cacheMap = speakerAPI->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "SpeakerAPI* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheSpeakerAPI, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<SpeakerAPI*> DataManager::listOfSpeakerAPIForKeys(
        QStringList keyList)
{
    QList<SpeakerAPI*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllSpeakerAPI.size(); ++i) {
        SpeakerAPI* speakerAPI;
        speakerAPI = (SpeakerAPI*) mAllSpeakerAPI.at(i);
        if (keyList.contains(QString::number(speakerAPI->id()))) {
            listOfData.append(speakerAPI);
            keyList.removeOne(QString::number(speakerAPI->id()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for SpeakerAPI: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::speakerAPIAsQVariantList()
{
    QVariantList speakerAPIList;
    for (int i = 0; i < mAllSpeakerAPI.size(); ++i) {
        speakerAPIList.append(((SpeakerAPI*) (mAllSpeakerAPI.at(i)))->toMap());
    }
    return speakerAPIList;
}

QList<QObject*> DataManager::allSpeakerAPI()
{
    return mAllSpeakerAPI;
}

QQmlListProperty<SpeakerAPI> DataManager::speakerAPIPropertyList()
{
    return QQmlListProperty<SpeakerAPI>(this, 0,
            &DataManager::appendToSpeakerAPIProperty, &DataManager::speakerAPIPropertyCount,
            &DataManager::atSpeakerAPIProperty, &DataManager::clearSpeakerAPIProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of SpeakerAPI*
void DataManager::appendToSpeakerAPIProperty(
        QQmlListProperty<SpeakerAPI> *speakerAPIList,
        SpeakerAPI* speakerAPI)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(speakerAPIList->object);
    if (dataManagerObject) {
        speakerAPI->setParent(dataManagerObject);
        dataManagerObject->mAllSpeakerAPI.append(speakerAPI);
        emit dataManagerObject->addedToAllSpeakerAPI(speakerAPI);
    } else {
        qWarning() << "cannot append SpeakerAPI* to mAllSpeakerAPI "
                << "Object is not of type DataManager*";
    }
}
int DataManager::speakerAPIPropertyCount(
        QQmlListProperty<SpeakerAPI> *speakerAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(speakerAPIList->object);
    if (dataManager) {
        return dataManager->mAllSpeakerAPI.size();
    } else {
        qWarning() << "cannot get size mAllSpeakerAPI " << "Object is not of type DataManager*";
    }
    return 0;
}
SpeakerAPI* DataManager::atSpeakerAPIProperty(
        QQmlListProperty<SpeakerAPI> *speakerAPIList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(speakerAPIList->object);
    if (dataManager) {
        if (dataManager->mAllSpeakerAPI.size() > pos) {
            return (SpeakerAPI*) dataManager->mAllSpeakerAPI.at(pos);
        }
        qWarning() << "cannot get SpeakerAPI* at pos " << pos << " size is "
                << dataManager->mAllSpeakerAPI.size();
    } else {
        qWarning() << "cannot get SpeakerAPI* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearSpeakerAPIProperty(
        QQmlListProperty<SpeakerAPI> *speakerAPIList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(speakerAPIList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllSpeakerAPI.size(); ++i) {
            SpeakerAPI* speakerAPI;
            speakerAPI = (SpeakerAPI*) dataManager->mAllSpeakerAPI.at(i);
			emit dataManager->deletedFromAllSpeakerAPIById(speakerAPI->id());
			emit dataManager->deletedFromAllSpeakerAPI(speakerAPI);
            speakerAPI->deleteLater();
            speakerAPI = 0;
        }
        dataManager->mAllSpeakerAPI.clear();
    } else {
        qWarning() << "cannot clear mAllSpeakerAPI " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all SpeakerAPI
 * and clears the list
 */
void DataManager::deleteSpeakerAPI()
{
    for (int i = 0; i < mAllSpeakerAPI.size(); ++i) {
        SpeakerAPI* speakerAPI;
        speakerAPI = (SpeakerAPI*) mAllSpeakerAPI.at(i);
        emit deletedFromAllSpeakerAPIById(speakerAPI->id());
		emit deletedFromAllSpeakerAPI(speakerAPI);
		emit speakerAPIPropertyListChanged();
        speakerAPI->deleteLater();
        speakerAPI = 0;
     }
     mAllSpeakerAPI.clear();
}

/**
 * creates a new SpeakerAPI
 * parent is DataManager
 * if data is successfully entered you must insertSpeakerAPI
 * if edit was canceled you must undoCreateSpeakerAPI to free up memory
 */
SpeakerAPI* DataManager::createSpeakerAPI()
{
    SpeakerAPI* speakerAPI;
    speakerAPI = new SpeakerAPI();
    speakerAPI->setParent(this);
    speakerAPI->prepareNew();
    return speakerAPI;
}

/**
 * deletes SpeakerAPI
 * if createSpeakerAPI was canceled from UI
 * to delete a previous successfully inserted SpeakerAPI
 * use deleteSpeakerAPI
 */
void DataManager::undoCreateSpeakerAPI(SpeakerAPI* speakerAPI)
{
    if (speakerAPI) {
        qDebug() << "undoCreateSpeakerAPI " << speakerAPI->id();
        speakerAPI->deleteLater();
        speakerAPI = 0;
    }
}

void DataManager::insertSpeakerAPI(SpeakerAPI* speakerAPI)
{
    // Important: DataManager must be parent of all root DTOs
    speakerAPI->setParent(this);
    mAllSpeakerAPI.append(speakerAPI);
    emit addedToAllSpeakerAPI(speakerAPI);
    emit speakerAPIPropertyListChanged();
}

void DataManager::insertSpeakerAPIFromMap(const QVariantMap& speakerAPIMap,
        const bool& useForeignProperties)
{
    SpeakerAPI* speakerAPI = new SpeakerAPI();
    speakerAPI->setParent(this);
    if (useForeignProperties) {
        speakerAPI->fillFromForeignMap(speakerAPIMap);
    } else {
        speakerAPI->fillFromMap(speakerAPIMap);
    }
    mAllSpeakerAPI.append(speakerAPI);
    emit addedToAllSpeakerAPI(speakerAPI);
    speakerAPIPropertyListChanged();
}

bool DataManager::deleteSpeakerAPI(SpeakerAPI* speakerAPI)
{
    bool ok = false;
    ok = mAllSpeakerAPI.removeOne(speakerAPI);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllSpeakerAPIById(speakerAPI->id());
    emit deletedFromAllSpeakerAPI(speakerAPI);
    emit speakerAPIPropertyListChanged();
    speakerAPI->deleteLater();
    speakerAPI = 0;
    return ok;
}


bool DataManager::deleteSpeakerAPIById(const int& id)
{
    for (int i = 0; i < mAllSpeakerAPI.size(); ++i) {
        SpeakerAPI* speakerAPI;
        speakerAPI = (SpeakerAPI*) mAllSpeakerAPI.at(i);
        if (speakerAPI->id() == id) {
            mAllSpeakerAPI.removeAt(i);
            emit deletedFromAllSpeakerAPIById(id);
            emit deletedFromAllSpeakerAPI(speakerAPI);
            emit speakerAPIPropertyListChanged();
            speakerAPI->deleteLater();
            speakerAPI = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
SpeakerAPI* DataManager::findSpeakerAPIById(const int& id){
    for (int i = 0; i < mAllSpeakerAPI.size(); ++i) {
        SpeakerAPI* speakerAPI;
        speakerAPI = (SpeakerAPI*)mAllSpeakerAPI.at(i);
        if(speakerAPI->id() == id){
            return speakerAPI;
        }
    }
    qDebug() << "no SpeakerAPI found for id " << id;
    return 0;
}

	

SettingsData* DataManager::settingsData()
{
    return mSettingsData;
}

void DataManager::readSettings()
{
    qDebug() << "Read the Settings File";
    mSettingsData = new SettingsData();
    mSettingsData->setParent(this);
    //
    QString assetsFilePath;

    QFile readFile(mSettingsPath);
    if(!readFile.exists()) {
        qDebug() << "settings cache doesn't exist: " << mSettingsPath;
        assetsFilePath = mDataAssetsPath+cacheSettingsData;
        QFile assetDataFile(assetsFilePath);
        if(assetDataFile.exists()) {
            // copy file from assets to data
            bool copyOk = assetDataFile.copy(mSettingsPath);
            if (!copyOk) {
                qDebug() << "cannot copy settings from data-assets to cache";
                return;
            }
            // IMPORTANT !!! copying from RESOURCES ":/data-assets/" to AppDataLocation
            // makes the target file READ ONLY - you must set PERMISSIONS
            // copying from RESOURCES ":/data-assets/" to GenericDataLocation the target is READ-WRITE
            copyOk = readFile.setPermissions(QFileDevice::ReadUser | QFileDevice::WriteUser);
            if (!copyOk) {
                qDebug() << "cannot set Permissions to read / write settings";
                return;
            }
        } else {
            qDebug() << "no settings from data-assets: " << assetsFilePath;
            return;
        }
    }
    if (!readFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file: " << mSettingsPath;
        return;
    }
    // create JSON Document from settings file
    QJsonDocument jda = QJsonDocument::fromJson(readFile.readAll());
    readFile.close();
    if(!jda.isObject()) {
        qWarning() << "Couldn't create JSON from file: " << mSettingsPath;
        return;
    }
    // create SettingsData* from JSON
    mSettingsData->fillFromMap(jda.toVariant().toMap());
    isProductionEnvironment = mSettingsData->isProductionEnvironment();
    qDebug() << "Settings* created";
}

void DataManager::saveSettings()
{
    qDebug() << "Save the Settings";
    // convert Settings* into JSONDocument and store to app data
    QJsonDocument jda = QJsonDocument::fromVariant(mSettingsData->toMap());
    // save JSON to data directory
    QFile saveFile(mSettingsPath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file to write " << mSettingsPath;
        return;
    }
    qint64 bytesWritten = saveFile.write(jda.toJson());
    saveFile.close();
    qDebug() << "SettingsData Bytes written: " << bytesWritten;
}

/*
 * reads data in from stored cache
 * if no cache found tries to get data from assets/datamodel
 */
QVariantList DataManager::readFromCache(const QString& fileName)
{
    QJsonDocument jda;
    QVariantList cacheList;
    QString cacheFilePath = dataPath(fileName);
    QFile dataFile(cacheFilePath);
    // check if already something cached
    if (!dataFile.exists()) {
        // check if there are some pre-defined data in data-assets
        QString dataAssetsFilePath = dataAssetsPath(fileName);
        qDebug() << fileName << "not found in cache" << cacheFilePath;
        qDebug() << "try copy from: " << dataAssetsFilePath;
        QFile dataAssetsFile(dataAssetsFilePath);
        if (dataAssetsFile.exists()) {
            // copy file from data-assets to cached data
            bool copyOk = dataAssetsFile.copy(cacheFilePath);
            if (!copyOk) {
                qDebug() << "cannot copy " << dataAssetsFilePath << " to " << cacheFilePath;
                return cacheList;
            }
            if(!mSettingsData->hasPublicCache()) {
                // IMPORTANT !!! copying from RESOURCES ":/data-assets/" to AppDataLocation
                // makes the target file READ ONLY - you must set PERMISSIONS
                // copying from RESOURCES ":/data-assets/" to GenericDataLocation the target is READ-WRITE
                copyOk = dataFile.setPermissions(QFileDevice::ReadUser | QFileDevice::WriteUser);
                if (!copyOk) {
                    qDebug() << "cannot set Permissions to read / write settings";
                    return cacheList;
                }
            }
        } else {
            // no cache, no prefilled data-assets - empty list
            return cacheList;
        }
    }
    if (!dataFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file: " << cacheFilePath;
        return cacheList;
    }
    jda = QJsonDocument::fromJson(dataFile.readAll());
    dataFile.close();
    if(!jda.isArray()) {
        qWarning() << "Couldn't create JSON Array from file: " << cacheFilePath;
        return cacheList;
    }
    cacheList = jda.toVariant().toList();
    return cacheList;
}

void DataManager::writeToCache(const QString& fileName, QVariantList& data)
{
    QString cacheFilePath = dataPath(fileName);
    QJsonDocument jda = QJsonDocument::fromVariant(data);

    QFile saveFile(cacheFilePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file to write " << cacheFilePath;
        return;
    }
    qint64 bytesWritten = saveFile.write(jda.toJson(mCompactJson?QJsonDocument::Compact:QJsonDocument::Indented));
    saveFile.close();
    qDebug() << "Data Bytes written: " << bytesWritten << " to: " << cacheFilePath;
}

DataManager::~DataManager()
{
    // clean up
}
