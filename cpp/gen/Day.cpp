#include "Day.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Session.hpp"

// keys of QVariantMap used in this APP
static const QString idKey = "id";
static const QString weekDayKey = "weekDay";
static const QString conferenceDayKey = "conferenceDay";
static const QString sessionsKey = "sessions";

// keys used from Server API etc
static const QString idForeignKey = "id";
static const QString weekDayForeignKey = "weekDay";
static const QString conferenceDayForeignKey = "conferenceDay";
static const QString sessionsForeignKey = "sessions";

/*
 * Default Constructor if Day not initialized from QVariantMap
 */
Day::Day(QObject *parent) :
        QObject(parent), mId(-1), mWeekDay(0)
{
	// Date, Time or Timestamp ? construct null value
	mConferenceDay = QDate();
		// lazy Arrays where only keys are persisted
		mSessionsKeysResolved = false;
}

bool Day::isAllResolved()
{
    if(!areSessionsKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize Day from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Day::fillFromMap(const QVariantMap& dayMap)
{
	mId = dayMap.value(idKey).toInt();
	mWeekDay = dayMap.value(weekDayKey).toInt();
	if (dayMap.contains(conferenceDayKey)) {
		// always getting the Date as a String (from server or JSON)
		QString conferenceDayAsString = dayMap.value(conferenceDayKey).toString();
		mConferenceDay = QDate::fromString(conferenceDayAsString, "yyyy-MM-dd");
		if (!mConferenceDay.isValid()) {
			mConferenceDay = QDate();
			qDebug() << "mConferenceDay is not valid for String: " << conferenceDayAsString;
		}
	}
	// mSessions is (lazy loaded) Array of Session*
	mSessionsKeys = dayMap.value(sessionsKey).toStringList();
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
void Day::fillFromForeignMap(const QVariantMap& dayMap)
{
	mId = dayMap.value(idForeignKey).toInt();
	mWeekDay = dayMap.value(weekDayForeignKey).toInt();
	if (dayMap.contains(conferenceDayForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString conferenceDayAsString = dayMap.value(conferenceDayForeignKey).toString();
		mConferenceDay = QDate::fromString(conferenceDayAsString, "yyyy-MM-dd");
		if (!mConferenceDay.isValid()) {
			mConferenceDay = QDate();
			qDebug() << "mConferenceDay is not valid for String: " << conferenceDayAsString;
		}
	}
	// mSessions is (lazy loaded) Array of Session*
	mSessionsKeys = dayMap.value(sessionsForeignKey).toStringList();
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
void Day::fillFromCacheMap(const QVariantMap& dayMap)
{
	mId = dayMap.value(idKey).toInt();
	mWeekDay = dayMap.value(weekDayKey).toInt();
	if (dayMap.contains(conferenceDayKey)) {
		// always getting the Date as a String (from server or JSON)
		QString conferenceDayAsString = dayMap.value(conferenceDayKey).toString();
		mConferenceDay = QDate::fromString(conferenceDayAsString, "yyyy-MM-dd");
		if (!mConferenceDay.isValid()) {
			mConferenceDay = QDate();
			qDebug() << "mConferenceDay is not valid for String: " << conferenceDayAsString;
		}
	}
	// mSessions is (lazy loaded) Array of Session*
	mSessionsKeys = dayMap.value(sessionsKey).toStringList();
	// mSessions must be resolved later if there are keys
	mSessionsKeysResolved = (mSessionsKeys.size() == 0);
	mSessions.clear();
}

void Day::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Day::isValid()
{
	if (mId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Day as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Day::toMap()
{
	QVariantMap dayMap;
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
	dayMap.insert(sessionsKey, mSessionsKeys);
	dayMap.insert(idKey, mId);
	dayMap.insert(weekDayKey, mWeekDay);
	if (hasConferenceDay()) {
		dayMap.insert(conferenceDayKey, mConferenceDay.toString("yyyy-MM-dd"));
	}
	return dayMap;
}

/*
 * Exports Properties from Day as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Day::toForeignMap()
{
	QVariantMap dayMap;
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
	dayMap.insert(sessionsKey, mSessionsKeys);
	dayMap.insert(idForeignKey, mId);
	dayMap.insert(weekDayForeignKey, mWeekDay);
	if (hasConferenceDay()) {
		dayMap.insert(conferenceDayForeignKey, mConferenceDay.toString("yyyy-MM-dd"));
	}
	return dayMap;
}


/*
 * Exports Properties from Day as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Day::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: id
// Domain KEY: id
int Day::id() const
{
	return mId;
}

void Day::setId(int id)
{
	if (id != mId) {
		mId = id;
		emit idChanged(id);
	}
}
// ATT 
// Optional: weekDay
int Day::weekDay() const
{
	return mWeekDay;
}

void Day::setWeekDay(int weekDay)
{
	if (weekDay != mWeekDay) {
		mWeekDay = weekDay;
		emit weekDayChanged(weekDay);
	}
}
// ATT 
// Optional: conferenceDay
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = new Date(conferenceDay)
 */
QDate Day::conferenceDay() const
{
	return mConferenceDay;
}

/**
 * from QML DateTime Picker use as parameter:
 * conferenceDay = new Date(myPicker.value)
 */
void Day::setConferenceDay(QDate conferenceDay)
{
	if (conferenceDay != mConferenceDay) {
		mConferenceDay = conferenceDay;
		emit conferenceDayChanged(conferenceDay);
	}
}
bool Day::hasConferenceDay()
{
	return !mConferenceDay.isNull() && mConferenceDay.isValid();
}
// ATT 
// Optional: sessions
QVariantList Day::sessionsAsQVariantList()
{
	QVariantList sessionsList;
	for (int i = 0; i < mSessions.size(); ++i) {
        sessionsList.append((mSessions.at(i))->toMap());
    }
	return sessionsList;
}
QVariantList Day::sessionsAsForeignQVariantList()
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
void Day::addToSessions(Session* session)
{
    mSessions.append(session);
    emit addedToSessions(session);
    emit sessionsPropertyListChanged();
}

bool Day::removeFromSessions(Session* session)
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
void Day::clearSessions()
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
bool Day::areSessionsKeysResolved()
{
    return mSessionsKeysResolved;
}

QStringList Day::sessionsKeys()
{
    return mSessionsKeys;
}

/**
 * Objects from sessionsKeys will be added to existing sessions
 * This enables to use addToSessions() without resolving before
 * Hint: it's your responsibility to resolve before looping thru sessions
 */
void Day::resolveSessionsKeys(QList<Session*> sessions)
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

int Day::sessionsCount()
{
    return mSessions.size();
}
QList<Session*> Day::sessions()
{
	return mSessions;
}
void Day::setSessions(QList<Session*> sessions) 
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
 * day.sessionsPropertyList.length to get the size
 * day.sessionsPropertyList[2] to get Session* at position 2
 * day.sessionsPropertyList = [] to clear the list
 * or get easy access to properties like
 * day.sessionsPropertyList[2].myPropertyName
 */
QQmlListProperty<Session> Day::sessionsPropertyList()
{
    return QQmlListProperty<Session>(this, 0, &Day::appendToSessionsProperty,
            &Day::sessionsPropertyCount, &Day::atSessionsProperty,
            &Day::clearSessionsProperty);
}
void Day::appendToSessionsProperty(QQmlListProperty<Session> *sessionsList,
        Session* session)
{
    Day *dayObject = qobject_cast<Day *>(sessionsList->object);
    if (dayObject) {
        dayObject->mSessions.append(session);
        emit dayObject->addedToSessions(session);
    } else {
        qWarning() << "cannot append Session* to sessions " << "Object is not of type Day*";
    }
}
int Day::sessionsPropertyCount(QQmlListProperty<Session> *sessionsList)
{
    Day *day = qobject_cast<Day *>(sessionsList->object);
    if (day) {
        return day->mSessions.size();
    } else {
        qWarning() << "cannot get size sessions " << "Object is not of type Day*";
    }
    return 0;
}
Session* Day::atSessionsProperty(QQmlListProperty<Session> *sessionsList, int pos)
{
    Day *day = qobject_cast<Day *>(sessionsList->object);
    if (day) {
        if (day->mSessions.size() > pos) {
            return day->mSessions.at(pos);
        }
        qWarning() << "cannot get Session* at pos " << pos << " size is "
                << day->mSessions.size();
    } else {
        qWarning() << "cannot get Session* at pos " << pos << "Object is not of type Day*";
    }
    return 0;
}
void Day::clearSessionsProperty(QQmlListProperty<Session> *sessionsList)
{
    Day *day = qobject_cast<Day *>(sessionsList->object);
    if (day) {
        // sessions are independent - DON'T delete them
        day->mSessions.clear();
    } else {
        qWarning() << "cannot clear sessions " << "Object is not of type Day*";
    }
}



Day::~Day()
{
	// place cleanUp code here
}
	
