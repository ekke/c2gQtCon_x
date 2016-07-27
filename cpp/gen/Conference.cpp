#include "Conference.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString idKey = "id";
static const QString conferenceNameKey = "conferenceName";
static const QString conferenceCityKey = "conferenceCity";
static const QString addressKey = "address";
static const QString timeZoneNameKey = "timeZoneName";
static const QString timeZoneOffsetSecondsKey = "timeZoneOffsetSeconds";
static const QString conferenceFromKey = "conferenceFrom";
static const QString conferenceToKey = "conferenceTo";
static const QString hashTagKey = "hashTag";
static const QString homePageKey = "homePage";
static const QString coordinateKey = "coordinate";
static const QString lastBuildingIdKey = "lastBuildingId";
static const QString lastFloorIdKey = "lastFloorId";
static const QString lastRoomIdKey = "lastRoomId";
static const QString lastSessionTrackIdKey = "lastSessionTrackId";
static const QString daysKey = "days";

// keys used from Server API etc
static const QString idForeignKey = "id";
static const QString conferenceNameForeignKey = "conferenceName";
static const QString conferenceCityForeignKey = "conferenceCity";
static const QString addressForeignKey = "address";
static const QString timeZoneNameForeignKey = "timeZoneName";
static const QString timeZoneOffsetSecondsForeignKey = "timeZoneOffsetSeconds";
static const QString conferenceFromForeignKey = "conferenceFrom";
static const QString conferenceToForeignKey = "conferenceTo";
static const QString hashTagForeignKey = "hashTag";
static const QString homePageForeignKey = "homePage";
static const QString coordinateForeignKey = "coordinate";
static const QString lastBuildingIdForeignKey = "lastBuildingId";
static const QString lastFloorIdForeignKey = "lastFloorId";
static const QString lastRoomIdForeignKey = "lastRoomId";
static const QString lastSessionTrackIdForeignKey = "lastSessionTrackId";
static const QString daysForeignKey = "days";

/*
 * Default Constructor if Conference not initialized from QVariantMap
 */
Conference::Conference(QObject *parent) :
        QObject(parent), mId(-1), mConferenceName(""), mConferenceCity(""), mAddress(""), mTimeZoneName(""), mTimeZoneOffsetSeconds(0), mHashTag(""), mHomePage(""), mCoordinate(""), mLastBuildingId(0), mLastFloorId(0), mLastRoomId(0), mLastSessionTrackId(0)
{
	// Date, Time or Timestamp ? construct null value
	mConferenceFrom = QDate();
	mConferenceTo = QDate();
		// lazy Arrays where only keys are persisted
		mDaysKeysResolved = false;
}

bool Conference::isAllResolved()
{
    if(!areDaysKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize Conference from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Conference::fillFromMap(const QVariantMap& conferenceMap)
{
	mId = conferenceMap.value(idKey).toInt();
	mConferenceName = conferenceMap.value(conferenceNameKey).toString();
	mConferenceCity = conferenceMap.value(conferenceCityKey).toString();
	mAddress = conferenceMap.value(addressKey).toString();
	mTimeZoneName = conferenceMap.value(timeZoneNameKey).toString();
	mTimeZoneOffsetSeconds = conferenceMap.value(timeZoneOffsetSecondsKey).toInt();
	if (conferenceMap.contains(conferenceFromKey)) {
		// always getting the Date as a String (from server or JSON)
		QString conferenceFromAsString = conferenceMap.value(conferenceFromKey).toString();
		mConferenceFrom = QDate::fromString(conferenceFromAsString, "yyyy-MM-dd");
		if (!mConferenceFrom.isValid()) {
			mConferenceFrom = QDate();
			qDebug() << "mConferenceFrom is not valid for String: " << conferenceFromAsString;
		}
	}
	if (conferenceMap.contains(conferenceToKey)) {
		// always getting the Date as a String (from server or JSON)
		QString conferenceToAsString = conferenceMap.value(conferenceToKey).toString();
		mConferenceTo = QDate::fromString(conferenceToAsString, "yyyy-MM-dd");
		if (!mConferenceTo.isValid()) {
			mConferenceTo = QDate();
			qDebug() << "mConferenceTo is not valid for String: " << conferenceToAsString;
		}
	}
	mHashTag = conferenceMap.value(hashTagKey).toString();
	mHomePage = conferenceMap.value(homePageKey).toString();
	mCoordinate = conferenceMap.value(coordinateKey).toString();
	mLastBuildingId = conferenceMap.value(lastBuildingIdKey).toInt();
	mLastFloorId = conferenceMap.value(lastFloorIdKey).toInt();
	mLastRoomId = conferenceMap.value(lastRoomIdKey).toInt();
	mLastSessionTrackId = conferenceMap.value(lastSessionTrackIdKey).toInt();
	// mDays is (lazy loaded) Array of Day*
	mDaysKeys = conferenceMap.value(daysKey).toStringList();
	// mDays must be resolved later if there are keys
	mDaysKeysResolved = (mDaysKeys.size() == 0);
	mDays.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Conference::fillFromForeignMap(const QVariantMap& conferenceMap)
{
	mId = conferenceMap.value(idForeignKey).toInt();
	mConferenceName = conferenceMap.value(conferenceNameForeignKey).toString();
	mConferenceCity = conferenceMap.value(conferenceCityForeignKey).toString();
	mAddress = conferenceMap.value(addressForeignKey).toString();
	mTimeZoneName = conferenceMap.value(timeZoneNameForeignKey).toString();
	mTimeZoneOffsetSeconds = conferenceMap.value(timeZoneOffsetSecondsForeignKey).toInt();
	if (conferenceMap.contains(conferenceFromForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString conferenceFromAsString = conferenceMap.value(conferenceFromForeignKey).toString();
		mConferenceFrom = QDate::fromString(conferenceFromAsString, "yyyy-MM-dd");
		if (!mConferenceFrom.isValid()) {
			mConferenceFrom = QDate();
			qDebug() << "mConferenceFrom is not valid for String: " << conferenceFromAsString;
		}
	}
	if (conferenceMap.contains(conferenceToForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString conferenceToAsString = conferenceMap.value(conferenceToForeignKey).toString();
		mConferenceTo = QDate::fromString(conferenceToAsString, "yyyy-MM-dd");
		if (!mConferenceTo.isValid()) {
			mConferenceTo = QDate();
			qDebug() << "mConferenceTo is not valid for String: " << conferenceToAsString;
		}
	}
	mHashTag = conferenceMap.value(hashTagForeignKey).toString();
	mHomePage = conferenceMap.value(homePageForeignKey).toString();
	mCoordinate = conferenceMap.value(coordinateForeignKey).toString();
	mLastBuildingId = conferenceMap.value(lastBuildingIdForeignKey).toInt();
	mLastFloorId = conferenceMap.value(lastFloorIdForeignKey).toInt();
	mLastRoomId = conferenceMap.value(lastRoomIdForeignKey).toInt();
	mLastSessionTrackId = conferenceMap.value(lastSessionTrackIdForeignKey).toInt();
	// mDays is (lazy loaded) Array of Day*
	mDaysKeys = conferenceMap.value(daysForeignKey).toStringList();
	// mDays must be resolved later if there are keys
	mDaysKeysResolved = (mDaysKeys.size() == 0);
	mDays.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Conference::fillFromCacheMap(const QVariantMap& conferenceMap)
{
	mId = conferenceMap.value(idKey).toInt();
	mConferenceName = conferenceMap.value(conferenceNameKey).toString();
	mConferenceCity = conferenceMap.value(conferenceCityKey).toString();
	mAddress = conferenceMap.value(addressKey).toString();
	mTimeZoneName = conferenceMap.value(timeZoneNameKey).toString();
	mTimeZoneOffsetSeconds = conferenceMap.value(timeZoneOffsetSecondsKey).toInt();
	if (conferenceMap.contains(conferenceFromKey)) {
		// always getting the Date as a String (from server or JSON)
		QString conferenceFromAsString = conferenceMap.value(conferenceFromKey).toString();
		mConferenceFrom = QDate::fromString(conferenceFromAsString, "yyyy-MM-dd");
		if (!mConferenceFrom.isValid()) {
			mConferenceFrom = QDate();
			qDebug() << "mConferenceFrom is not valid for String: " << conferenceFromAsString;
		}
	}
	if (conferenceMap.contains(conferenceToKey)) {
		// always getting the Date as a String (from server or JSON)
		QString conferenceToAsString = conferenceMap.value(conferenceToKey).toString();
		mConferenceTo = QDate::fromString(conferenceToAsString, "yyyy-MM-dd");
		if (!mConferenceTo.isValid()) {
			mConferenceTo = QDate();
			qDebug() << "mConferenceTo is not valid for String: " << conferenceToAsString;
		}
	}
	mHashTag = conferenceMap.value(hashTagKey).toString();
	mHomePage = conferenceMap.value(homePageKey).toString();
	mCoordinate = conferenceMap.value(coordinateKey).toString();
	mLastBuildingId = conferenceMap.value(lastBuildingIdKey).toInt();
	mLastFloorId = conferenceMap.value(lastFloorIdKey).toInt();
	mLastRoomId = conferenceMap.value(lastRoomIdKey).toInt();
	mLastSessionTrackId = conferenceMap.value(lastSessionTrackIdKey).toInt();
	// mDays is (lazy loaded) Array of Day*
	mDaysKeys = conferenceMap.value(daysKey).toStringList();
	// mDays must be resolved later if there are keys
	mDaysKeysResolved = (mDaysKeys.size() == 0);
	mDays.clear();
}

void Conference::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Conference::isValid()
{
	if (mId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Conference as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Conference::toMap()
{
	QVariantMap conferenceMap;
	// mDays points to Day*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mDaysKeysResolved) {
		mDaysKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mDays.size(); ++i) {
		Day* day;
		day = mDays.at(i);
		mDaysKeys << QString::number(day->id());
	}
	conferenceMap.insert(daysKey, mDaysKeys);
	conferenceMap.insert(idKey, mId);
	conferenceMap.insert(conferenceNameKey, mConferenceName);
	conferenceMap.insert(conferenceCityKey, mConferenceCity);
	conferenceMap.insert(addressKey, mAddress);
	conferenceMap.insert(timeZoneNameKey, mTimeZoneName);
	conferenceMap.insert(timeZoneOffsetSecondsKey, mTimeZoneOffsetSeconds);
	if (hasConferenceFrom()) {
		conferenceMap.insert(conferenceFromKey, mConferenceFrom.toString("yyyy-MM-dd"));
	}
	if (hasConferenceTo()) {
		conferenceMap.insert(conferenceToKey, mConferenceTo.toString("yyyy-MM-dd"));
	}
	conferenceMap.insert(hashTagKey, mHashTag);
	conferenceMap.insert(homePageKey, mHomePage);
	conferenceMap.insert(coordinateKey, mCoordinate);
	conferenceMap.insert(lastBuildingIdKey, mLastBuildingId);
	conferenceMap.insert(lastFloorIdKey, mLastFloorId);
	conferenceMap.insert(lastRoomIdKey, mLastRoomId);
	conferenceMap.insert(lastSessionTrackIdKey, mLastSessionTrackId);
	return conferenceMap;
}

/*
 * Exports Properties from Conference as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Conference::toForeignMap()
{
	QVariantMap conferenceMap;
	// mDays points to Day*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mDaysKeysResolved) {
		mDaysKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mDays.size(); ++i) {
		Day* day;
		day = mDays.at(i);
		mDaysKeys << QString::number(day->id());
	}
	conferenceMap.insert(daysKey, mDaysKeys);
	conferenceMap.insert(idForeignKey, mId);
	conferenceMap.insert(conferenceNameForeignKey, mConferenceName);
	conferenceMap.insert(conferenceCityForeignKey, mConferenceCity);
	conferenceMap.insert(addressForeignKey, mAddress);
	conferenceMap.insert(timeZoneNameForeignKey, mTimeZoneName);
	conferenceMap.insert(timeZoneOffsetSecondsForeignKey, mTimeZoneOffsetSeconds);
	if (hasConferenceFrom()) {
		conferenceMap.insert(conferenceFromForeignKey, mConferenceFrom.toString("yyyy-MM-dd"));
	}
	if (hasConferenceTo()) {
		conferenceMap.insert(conferenceToForeignKey, mConferenceTo.toString("yyyy-MM-dd"));
	}
	conferenceMap.insert(hashTagForeignKey, mHashTag);
	conferenceMap.insert(homePageForeignKey, mHomePage);
	conferenceMap.insert(coordinateForeignKey, mCoordinate);
	conferenceMap.insert(lastBuildingIdForeignKey, mLastBuildingId);
	conferenceMap.insert(lastFloorIdForeignKey, mLastFloorId);
	conferenceMap.insert(lastRoomIdForeignKey, mLastRoomId);
	conferenceMap.insert(lastSessionTrackIdForeignKey, mLastSessionTrackId);
	return conferenceMap;
}


/*
 * Exports Properties from Conference as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Conference::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: id
// Domain KEY: id
int Conference::id() const
{
	return mId;
}

void Conference::setId(int id)
{
	if (id != mId) {
		mId = id;
		emit idChanged(id);
	}
}
// ATT 
// Optional: conferenceName
QString Conference::conferenceName() const
{
	return mConferenceName;
}

void Conference::setConferenceName(QString conferenceName)
{
	if (conferenceName != mConferenceName) {
		mConferenceName = conferenceName;
		emit conferenceNameChanged(conferenceName);
	}
}
// ATT 
// Optional: conferenceCity
QString Conference::conferenceCity() const
{
	return mConferenceCity;
}

void Conference::setConferenceCity(QString conferenceCity)
{
	if (conferenceCity != mConferenceCity) {
		mConferenceCity = conferenceCity;
		emit conferenceCityChanged(conferenceCity);
	}
}
// ATT 
// Optional: address
QString Conference::address() const
{
	return mAddress;
}

void Conference::setAddress(QString address)
{
	if (address != mAddress) {
		mAddress = address;
		emit addressChanged(address);
	}
}
// ATT 
// Optional: timeZoneName
QString Conference::timeZoneName() const
{
	return mTimeZoneName;
}

void Conference::setTimeZoneName(QString timeZoneName)
{
	if (timeZoneName != mTimeZoneName) {
		mTimeZoneName = timeZoneName;
		emit timeZoneNameChanged(timeZoneName);
	}
}
// ATT 
// Optional: timeZoneOffsetSeconds
int Conference::timeZoneOffsetSeconds() const
{
	return mTimeZoneOffsetSeconds;
}

void Conference::setTimeZoneOffsetSeconds(int timeZoneOffsetSeconds)
{
	if (timeZoneOffsetSeconds != mTimeZoneOffsetSeconds) {
		mTimeZoneOffsetSeconds = timeZoneOffsetSeconds;
		emit timeZoneOffsetSecondsChanged(timeZoneOffsetSeconds);
	}
}
// ATT 
// Optional: conferenceFrom
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = new Date(conferenceFrom)
 */
QDate Conference::conferenceFrom() const
{
	return mConferenceFrom;
}

/**
 * from QML DateTime Picker use as parameter:
 * conferenceFrom = new Date(myPicker.value)
 */
void Conference::setConferenceFrom(QDate conferenceFrom)
{
	if (conferenceFrom != mConferenceFrom) {
		mConferenceFrom = conferenceFrom;
		emit conferenceFromChanged(conferenceFrom);
	}
}
bool Conference::hasConferenceFrom()
{
	return !mConferenceFrom.isNull() && mConferenceFrom.isValid();
}
// ATT 
// Optional: conferenceTo
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = new Date(conferenceTo)
 */
QDate Conference::conferenceTo() const
{
	return mConferenceTo;
}

/**
 * from QML DateTime Picker use as parameter:
 * conferenceTo = new Date(myPicker.value)
 */
void Conference::setConferenceTo(QDate conferenceTo)
{
	if (conferenceTo != mConferenceTo) {
		mConferenceTo = conferenceTo;
		emit conferenceToChanged(conferenceTo);
	}
}
bool Conference::hasConferenceTo()
{
	return !mConferenceTo.isNull() && mConferenceTo.isValid();
}
// ATT 
// Optional: hashTag
QString Conference::hashTag() const
{
	return mHashTag;
}

void Conference::setHashTag(QString hashTag)
{
	if (hashTag != mHashTag) {
		mHashTag = hashTag;
		emit hashTagChanged(hashTag);
	}
}
// ATT 
// Optional: homePage
QString Conference::homePage() const
{
	return mHomePage;
}

void Conference::setHomePage(QString homePage)
{
	if (homePage != mHomePage) {
		mHomePage = homePage;
		emit homePageChanged(homePage);
	}
}
// ATT 
// Optional: coordinate
QString Conference::coordinate() const
{
	return mCoordinate;
}

void Conference::setCoordinate(QString coordinate)
{
	if (coordinate != mCoordinate) {
		mCoordinate = coordinate;
		emit coordinateChanged(coordinate);
	}
}
// ATT 
// Optional: lastBuildingId
int Conference::lastBuildingId() const
{
	return mLastBuildingId;
}

void Conference::setLastBuildingId(int lastBuildingId)
{
	if (lastBuildingId != mLastBuildingId) {
		mLastBuildingId = lastBuildingId;
		emit lastBuildingIdChanged(lastBuildingId);
	}
}
// ATT 
// Optional: lastFloorId
int Conference::lastFloorId() const
{
	return mLastFloorId;
}

void Conference::setLastFloorId(int lastFloorId)
{
	if (lastFloorId != mLastFloorId) {
		mLastFloorId = lastFloorId;
		emit lastFloorIdChanged(lastFloorId);
	}
}
// ATT 
// Optional: lastRoomId
int Conference::lastRoomId() const
{
	return mLastRoomId;
}

void Conference::setLastRoomId(int lastRoomId)
{
	if (lastRoomId != mLastRoomId) {
		mLastRoomId = lastRoomId;
		emit lastRoomIdChanged(lastRoomId);
	}
}
// ATT 
// Optional: lastSessionTrackId
int Conference::lastSessionTrackId() const
{
	return mLastSessionTrackId;
}

void Conference::setLastSessionTrackId(int lastSessionTrackId)
{
	if (lastSessionTrackId != mLastSessionTrackId) {
		mLastSessionTrackId = lastSessionTrackId;
		emit lastSessionTrackIdChanged(lastSessionTrackId);
	}
}
// ATT 
// Optional: days
QVariantList Conference::daysAsQVariantList()
{
	QVariantList daysList;
	for (int i = 0; i < mDays.size(); ++i) {
        daysList.append((mDays.at(i))->toMap());
    }
	return daysList;
}
QVariantList Conference::daysAsForeignQVariantList()
{
	QVariantList daysList;
	for (int i = 0; i < mDays.size(); ++i) {
        daysList.append((mDays.at(i))->toForeignMap());
    }
	return daysList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add days without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveDaysKeys
 */
void Conference::addToDays(Day* day)
{
    mDays.append(day);
    emit addedToDays(day);
    emit daysPropertyListChanged();
}

bool Conference::removeFromDays(Day* day)
{
    bool ok = false;
    ok = mDays.removeOne(day);
    if (!ok) {
    	qDebug() << "Day* not found in days";
    	return false;
    }
    emit daysPropertyListChanged();
    // days are independent - DON'T delete them
    return true;
}
void Conference::clearDays()
{
    for (int i = mDays.size(); i > 0; --i) {
        removeFromDays(mDays.last());
    }
    mDaysKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: daysKeys()
 * - resolve them from DataManager
 * - then resolveDaysKeys()
 */
bool Conference::areDaysKeysResolved()
{
    return mDaysKeysResolved;
}

QStringList Conference::daysKeys()
{
    return mDaysKeys;
}

/**
 * Objects from daysKeys will be added to existing days
 * This enables to use addToDays() without resolving before
 * Hint: it's your responsibility to resolve before looping thru days
 */
void Conference::resolveDaysKeys(QList<Day*> days)
{
    if(mDaysKeysResolved){
        return;
    }
    // don't clear mDays (see above)
    for (int i = 0; i < days.size(); ++i) {
        addToDays(days.at(i));
    }
    mDaysKeysResolved = true;
}

int Conference::daysCount()
{
    return mDays.size();
}
QList<Day*> Conference::days()
{
	return mDays;
}
void Conference::setDays(QList<Day*> days) 
{
	if (days != mDays) {
		mDays = days;
		emit daysChanged(days);
		emit daysPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * conference.daysPropertyList.length to get the size
 * conference.daysPropertyList[2] to get Day* at position 2
 * conference.daysPropertyList = [] to clear the list
 * or get easy access to properties like
 * conference.daysPropertyList[2].myPropertyName
 */
QQmlListProperty<Day> Conference::daysPropertyList()
{
    return QQmlListProperty<Day>(this, 0, &Conference::appendToDaysProperty,
            &Conference::daysPropertyCount, &Conference::atDaysProperty,
            &Conference::clearDaysProperty);
}
void Conference::appendToDaysProperty(QQmlListProperty<Day> *daysList,
        Day* day)
{
    Conference *conferenceObject = qobject_cast<Conference *>(daysList->object);
    if (conferenceObject) {
        conferenceObject->mDays.append(day);
        emit conferenceObject->addedToDays(day);
    } else {
        qWarning() << "cannot append Day* to days " << "Object is not of type Conference*";
    }
}
int Conference::daysPropertyCount(QQmlListProperty<Day> *daysList)
{
    Conference *conference = qobject_cast<Conference *>(daysList->object);
    if (conference) {
        return conference->mDays.size();
    } else {
        qWarning() << "cannot get size days " << "Object is not of type Conference*";
    }
    return 0;
}
Day* Conference::atDaysProperty(QQmlListProperty<Day> *daysList, int pos)
{
    Conference *conference = qobject_cast<Conference *>(daysList->object);
    if (conference) {
        if (conference->mDays.size() > pos) {
            return conference->mDays.at(pos);
        }
        qWarning() << "cannot get Day* at pos " << pos << " size is "
                << conference->mDays.size();
    } else {
        qWarning() << "cannot get Day* at pos " << pos << "Object is not of type Conference*";
    }
    return 0;
}
void Conference::clearDaysProperty(QQmlListProperty<Day> *daysList)
{
    Conference *conference = qobject_cast<Conference *>(daysList->object);
    if (conference) {
        // days are independent - DON'T delete them
        conference->mDays.clear();
    } else {
        qWarning() << "cannot clear days " << "Object is not of type Conference*";
    }
}



Conference::~Conference()
{
	// place cleanUp code here
}
	
