#include "SessionAPI.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString sessionIdKey = "sessionId";
static const QString titleKey = "title";
static const QString subtitleKey = "subtitle";
static const QString descriptionKey = "description";
static const QString sessionTypeKey = "sessionType";
static const QString startTimeKey = "startTime";
static const QString durationKey = "duration";
static const QString abstractTextKey = "abstractText";
static const QString roomKey = "room";
static const QString trackKey = "track";
static const QString personsKey = "persons";
static const QString sessionLinksKey = "sessionLinks";

// keys used from Server API etc
static const QString sessionIdForeignKey = "id";
static const QString titleForeignKey = "title";
static const QString subtitleForeignKey = "subtitle";
static const QString descriptionForeignKey = "description";
static const QString sessionTypeForeignKey = "type";
static const QString startTimeForeignKey = "start";
static const QString durationForeignKey = "duration";
static const QString abstractTextForeignKey = "abstract";
static const QString roomForeignKey = "room";
static const QString trackForeignKey = "track";
static const QString personsForeignKey = "persons";
static const QString sessionLinksForeignKey = "links";

/*
 * Default Constructor if SessionAPI not initialized from QVariantMap
 */
SessionAPI::SessionAPI(QObject *parent) :
        QObject(parent), mSessionId(-1), mTitle(""), mSubtitle(""), mDescription(""), mSessionType(""), mDuration(""), mAbstractText(""), mRoom(""), mTrack("")
{
	// Date, Time or Timestamp ? construct null value
	mStartTime = QTime();
		// lazy Arrays where only keys are persisted
		mPersonsKeysResolved = false;
		mSessionLinksKeysResolved = false;
}

bool SessionAPI::isAllResolved()
{
    if(!arePersonsKeysResolved()) {
        return false;
    }
    if(!areSessionLinksKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize SessionAPI from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void SessionAPI::fillFromMap(const QVariantMap& sessionAPIMap)
{
	mSessionId = sessionAPIMap.value(sessionIdKey).toInt();
	mTitle = sessionAPIMap.value(titleKey).toString();
	mSubtitle = sessionAPIMap.value(subtitleKey).toString();
	mDescription = sessionAPIMap.value(descriptionKey).toString();
	mSessionType = sessionAPIMap.value(sessionTypeKey).toString();
	if (sessionAPIMap.contains(startTimeKey)) {
		// always getting the Date as a String (from server or JSON)
		QString startTimeAsString = sessionAPIMap.value(startTimeKey).toString();
		mStartTime = QTime::fromString(startTimeAsString, "HH:mm");
		if (!mStartTime.isValid()) {
			mStartTime = QTime();
			qDebug() << "mStartTime is not valid for String: " << startTimeAsString;
		}
	}
	mDuration = sessionAPIMap.value(durationKey).toString();
	mAbstractText = sessionAPIMap.value(abstractTextKey).toString();
	mRoom = sessionAPIMap.value(roomKey).toString();
	mTrack = sessionAPIMap.value(trackKey).toString();
	// mPersons is (lazy loaded) Array of PersonsAPI*
	mPersonsKeys = sessionAPIMap.value(personsKey).toStringList();
	// mPersons must be resolved later if there are keys
	mPersonsKeysResolved = (mPersonsKeys.size() == 0);
	mPersons.clear();
	// mSessionLinks is (lazy loaded) Array of SessionLinkAPI*
	mSessionLinksKeys = sessionAPIMap.value(sessionLinksKey).toStringList();
	// mSessionLinks must be resolved later if there are keys
	mSessionLinksKeysResolved = (mSessionLinksKeys.size() == 0);
	mSessionLinks.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void SessionAPI::fillFromForeignMap(const QVariantMap& sessionAPIMap)
{
	mSessionId = sessionAPIMap.value(sessionIdForeignKey).toInt();
	mTitle = sessionAPIMap.value(titleForeignKey).toString();
	mSubtitle = sessionAPIMap.value(subtitleForeignKey).toString();
	mDescription = sessionAPIMap.value(descriptionForeignKey).toString();
	mSessionType = sessionAPIMap.value(sessionTypeForeignKey).toString();
	if (sessionAPIMap.contains(startTimeForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString startTimeAsString = sessionAPIMap.value(startTimeForeignKey).toString();
		mStartTime = QTime::fromString(startTimeAsString, "HH:mm");
		if (!mStartTime.isValid()) {
			mStartTime = QTime();
			qDebug() << "mStartTime is not valid for String: " << startTimeAsString;
		}
	}
	mDuration = sessionAPIMap.value(durationForeignKey).toString();
	mAbstractText = sessionAPIMap.value(abstractTextForeignKey).toString();
	mRoom = sessionAPIMap.value(roomForeignKey).toString();
	mTrack = sessionAPIMap.value(trackForeignKey).toString();
	// mPersons is (lazy loaded) Array of PersonsAPI*
	mPersonsKeys = sessionAPIMap.value(personsForeignKey).toStringList();
	// mPersons must be resolved later if there are keys
	mPersonsKeysResolved = (mPersonsKeys.size() == 0);
	mPersons.clear();
	// mSessionLinks is (lazy loaded) Array of SessionLinkAPI*
	mSessionLinksKeys = sessionAPIMap.value(sessionLinksForeignKey).toStringList();
	// mSessionLinks must be resolved later if there are keys
	mSessionLinksKeysResolved = (mSessionLinksKeys.size() == 0);
	mSessionLinks.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void SessionAPI::fillFromCacheMap(const QVariantMap& sessionAPIMap)
{
	mSessionId = sessionAPIMap.value(sessionIdKey).toInt();
	mTitle = sessionAPIMap.value(titleKey).toString();
	mSubtitle = sessionAPIMap.value(subtitleKey).toString();
	mDescription = sessionAPIMap.value(descriptionKey).toString();
	mSessionType = sessionAPIMap.value(sessionTypeKey).toString();
	if (sessionAPIMap.contains(startTimeKey)) {
		// always getting the Date as a String (from server or JSON)
		QString startTimeAsString = sessionAPIMap.value(startTimeKey).toString();
		mStartTime = QTime::fromString(startTimeAsString, "HH:mm");
		if (!mStartTime.isValid()) {
			mStartTime = QTime();
			qDebug() << "mStartTime is not valid for String: " << startTimeAsString;
		}
	}
	mDuration = sessionAPIMap.value(durationKey).toString();
	mAbstractText = sessionAPIMap.value(abstractTextKey).toString();
	mRoom = sessionAPIMap.value(roomKey).toString();
	mTrack = sessionAPIMap.value(trackKey).toString();
	// mPersons is (lazy loaded) Array of PersonsAPI*
	mPersonsKeys = sessionAPIMap.value(personsKey).toStringList();
	// mPersons must be resolved later if there are keys
	mPersonsKeysResolved = (mPersonsKeys.size() == 0);
	mPersons.clear();
	// mSessionLinks is (lazy loaded) Array of SessionLinkAPI*
	mSessionLinksKeys = sessionAPIMap.value(sessionLinksKey).toStringList();
	// mSessionLinks must be resolved later if there are keys
	mSessionLinksKeysResolved = (mSessionLinksKeys.size() == 0);
	mSessionLinks.clear();
}

void SessionAPI::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool SessionAPI::isValid()
{
	if (mSessionId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from SessionAPI as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap SessionAPI::toMap()
{
	QVariantMap sessionAPIMap;
	// mPersons points to PersonsAPI*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mPersonsKeysResolved) {
		mPersonsKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mPersons.size(); ++i) {
		PersonsAPI* personsAPI;
		personsAPI = mPersons.at(i);
		mPersonsKeys << QString::number(personsAPI->speakerId());
	}
	sessionAPIMap.insert(personsKey, mPersonsKeys);
	// mSessionLinks points to SessionLinkAPI*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mSessionLinksKeysResolved) {
		mSessionLinksKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mSessionLinks.size(); ++i) {
		SessionLinkAPI* sessionLinkAPI;
		sessionLinkAPI = mSessionLinks.at(i);
		mSessionLinksKeys << sessionLinkAPI->uuid();
	}
	sessionAPIMap.insert(sessionLinksKey, mSessionLinksKeys);
	sessionAPIMap.insert(sessionIdKey, mSessionId);
	sessionAPIMap.insert(titleKey, mTitle);
	sessionAPIMap.insert(subtitleKey, mSubtitle);
	sessionAPIMap.insert(descriptionKey, mDescription);
	sessionAPIMap.insert(sessionTypeKey, mSessionType);
	if (hasStartTime()) {
		sessionAPIMap.insert(startTimeKey, mStartTime.toString("HH:mm"));
	}
	sessionAPIMap.insert(durationKey, mDuration);
	sessionAPIMap.insert(abstractTextKey, mAbstractText);
	sessionAPIMap.insert(roomKey, mRoom);
	sessionAPIMap.insert(trackKey, mTrack);
	return sessionAPIMap;
}

/*
 * Exports Properties from SessionAPI as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap SessionAPI::toForeignMap()
{
	QVariantMap sessionAPIMap;
	// mPersons points to PersonsAPI*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mPersonsKeysResolved) {
		mPersonsKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mPersons.size(); ++i) {
		PersonsAPI* personsAPI;
		personsAPI = mPersons.at(i);
		mPersonsKeys << QString::number(personsAPI->speakerId());
	}
	sessionAPIMap.insert(personsKey, mPersonsKeys);
	// mSessionLinks points to SessionLinkAPI*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mSessionLinksKeysResolved) {
		mSessionLinksKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mSessionLinks.size(); ++i) {
		SessionLinkAPI* sessionLinkAPI;
		sessionLinkAPI = mSessionLinks.at(i);
		mSessionLinksKeys << sessionLinkAPI->uuid();
	}
	sessionAPIMap.insert(sessionLinksKey, mSessionLinksKeys);
	sessionAPIMap.insert(sessionIdForeignKey, mSessionId);
	sessionAPIMap.insert(titleForeignKey, mTitle);
	sessionAPIMap.insert(subtitleForeignKey, mSubtitle);
	sessionAPIMap.insert(descriptionForeignKey, mDescription);
	sessionAPIMap.insert(sessionTypeForeignKey, mSessionType);
	if (hasStartTime()) {
		sessionAPIMap.insert(startTimeForeignKey, mStartTime.toString("HH:mm"));
	}
	sessionAPIMap.insert(durationForeignKey, mDuration);
	sessionAPIMap.insert(abstractTextForeignKey, mAbstractText);
	sessionAPIMap.insert(roomForeignKey, mRoom);
	sessionAPIMap.insert(trackForeignKey, mTrack);
	return sessionAPIMap;
}


/*
 * Exports Properties from SessionAPI as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap SessionAPI::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: sessionId
// Domain KEY: sessionId
int SessionAPI::sessionId() const
{
	return mSessionId;
}

void SessionAPI::setSessionId(int sessionId)
{
	if (sessionId != mSessionId) {
		mSessionId = sessionId;
		emit sessionIdChanged(sessionId);
	}
}
// ATT 
// Optional: title
QString SessionAPI::title() const
{
	return mTitle;
}

void SessionAPI::setTitle(QString title)
{
	if (title != mTitle) {
		mTitle = title;
		emit titleChanged(title);
	}
}
// ATT 
// Optional: subtitle
QString SessionAPI::subtitle() const
{
	return mSubtitle;
}

void SessionAPI::setSubtitle(QString subtitle)
{
	if (subtitle != mSubtitle) {
		mSubtitle = subtitle;
		emit subtitleChanged(subtitle);
	}
}
// ATT 
// Optional: description
QString SessionAPI::description() const
{
	return mDescription;
}

void SessionAPI::setDescription(QString description)
{
	if (description != mDescription) {
		mDescription = description;
		emit descriptionChanged(description);
	}
}
// ATT 
// Optional: sessionType
QString SessionAPI::sessionType() const
{
	return mSessionType;
}

void SessionAPI::setSessionType(QString sessionType)
{
	if (sessionType != mSessionType) {
		mSessionType = sessionType;
		emit sessionTypeChanged(sessionType);
	}
}
// ATT 
// Optional: startTime
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = myPicker.dateFromTime(startTime)
 */
QTime SessionAPI::startTime() const
{
	return mStartTime;
}

void SessionAPI::setStartTime(QTime startTime)
{
	if (startTime != mStartTime) {
		mStartTime = startTime;
		emit startTimeChanged(startTime);
	}
}
/**
 * INVOKABLE
 * Convenience method to make it easy to set the value from QML
 * use myPicker.value.toTimeString() as Parameter
 */
void SessionAPI::setStartTimeFromPickerValue(QString startTimeValue)
{
    QTime startTime = QTime::fromString(startTimeValue.left(8), "HH:mm:ss");
    if (startTime != mStartTime) {
        mStartTime = startTime;
        emit startTimeChanged(startTime);
    }
}
bool SessionAPI::hasStartTime()
{
	return !mStartTime.isNull() && mStartTime.isValid();
}
// ATT 
// Optional: duration
QString SessionAPI::duration() const
{
	return mDuration;
}

void SessionAPI::setDuration(QString duration)
{
	if (duration != mDuration) {
		mDuration = duration;
		emit durationChanged(duration);
	}
}
// ATT 
// Optional: abstractText
QString SessionAPI::abstractText() const
{
	return mAbstractText;
}

void SessionAPI::setAbstractText(QString abstractText)
{
	if (abstractText != mAbstractText) {
		mAbstractText = abstractText;
		emit abstractTextChanged(abstractText);
	}
}
// ATT 
// Optional: room
QString SessionAPI::room() const
{
	return mRoom;
}

void SessionAPI::setRoom(QString room)
{
	if (room != mRoom) {
		mRoom = room;
		emit roomChanged(room);
	}
}
// ATT 
// Optional: track
QString SessionAPI::track() const
{
	return mTrack;
}

void SessionAPI::setTrack(QString track)
{
	if (track != mTrack) {
		mTrack = track;
		emit trackChanged(track);
	}
}
// ATT 
// Optional: persons
QVariantList SessionAPI::personsAsQVariantList()
{
	QVariantList personsList;
	for (int i = 0; i < mPersons.size(); ++i) {
        personsList.append((mPersons.at(i))->toMap());
    }
	return personsList;
}
QVariantList SessionAPI::personsAsForeignQVariantList()
{
	QVariantList personsList;
	for (int i = 0; i < mPersons.size(); ++i) {
        personsList.append((mPersons.at(i))->toForeignMap());
    }
	return personsList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add persons without resolving existing keys before
 * attention: before looping through the objects
 * you must resolvePersonsKeys
 */
void SessionAPI::addToPersons(PersonsAPI* personsAPI)
{
    mPersons.append(personsAPI);
    emit addedToPersons(personsAPI);
    emit personsPropertyListChanged();
}

bool SessionAPI::removeFromPersons(PersonsAPI* personsAPI)
{
    bool ok = false;
    ok = mPersons.removeOne(personsAPI);
    if (!ok) {
    	qDebug() << "PersonsAPI* not found in persons";
    	return false;
    }
    emit personsPropertyListChanged();
    // persons are independent - DON'T delete them
    return true;
}
void SessionAPI::clearPersons()
{
    for (int i = mPersons.size(); i > 0; --i) {
        removeFromPersons(mPersons.last());
    }
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: personsKeys()
 * - resolve them from DataManager
 * - then resolvePersonsKeys()
 */
bool SessionAPI::arePersonsKeysResolved()
{
    return mPersonsKeysResolved;
}

QStringList SessionAPI::personsKeys()
{
    return mPersonsKeys;
}

/**
 * Objects from personsKeys will be added to existing persons
 * This enables to use addToPersons() without resolving before
 * Hint: it's your responsibility to resolve before looping thru persons
 */
void SessionAPI::resolvePersonsKeys(QList<PersonsAPI*> persons)
{
    if(mPersonsKeysResolved){
        return;
    }
    // don't clear mPersons (see above)
    for (int i = 0; i < persons.size(); ++i) {
        addToPersons(persons.at(i));
    }
    mPersonsKeysResolved = true;
}

int SessionAPI::personsCount()
{
    return mPersons.size();
}
QList<PersonsAPI*> SessionAPI::persons()
{
	return mPersons;
}
void SessionAPI::setPersons(QList<PersonsAPI*> persons) 
{
	if (persons != mPersons) {
		mPersons = persons;
		emit personsChanged(persons);
		emit personsPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * sessionAPI.personsPropertyList.length to get the size
 * sessionAPI.personsPropertyList[2] to get PersonsAPI* at position 2
 * sessionAPI.personsPropertyList = [] to clear the list
 * or get easy access to properties like
 * sessionAPI.personsPropertyList[2].myPropertyName
 */
QQmlListProperty<PersonsAPI> SessionAPI::personsPropertyList()
{
    return QQmlListProperty<PersonsAPI>(this, 0, &SessionAPI::appendToPersonsProperty,
            &SessionAPI::personsPropertyCount, &SessionAPI::atPersonsProperty,
            &SessionAPI::clearPersonsProperty);
}
void SessionAPI::appendToPersonsProperty(QQmlListProperty<PersonsAPI> *personsList,
        PersonsAPI* personsAPI)
{
    SessionAPI *sessionAPIObject = qobject_cast<SessionAPI *>(personsList->object);
    if (sessionAPIObject) {
        sessionAPIObject->mPersons.append(personsAPI);
        emit sessionAPIObject->addedToPersons(personsAPI);
    } else {
        qWarning() << "cannot append PersonsAPI* to persons " << "Object is not of type SessionAPI*";
    }
}
int SessionAPI::personsPropertyCount(QQmlListProperty<PersonsAPI> *personsList)
{
    SessionAPI *sessionAPI = qobject_cast<SessionAPI *>(personsList->object);
    if (sessionAPI) {
        return sessionAPI->mPersons.size();
    } else {
        qWarning() << "cannot get size persons " << "Object is not of type SessionAPI*";
    }
    return 0;
}
PersonsAPI* SessionAPI::atPersonsProperty(QQmlListProperty<PersonsAPI> *personsList, int pos)
{
    SessionAPI *sessionAPI = qobject_cast<SessionAPI *>(personsList->object);
    if (sessionAPI) {
        if (sessionAPI->mPersons.size() > pos) {
            return sessionAPI->mPersons.at(pos);
        }
        qWarning() << "cannot get PersonsAPI* at pos " << pos << " size is "
                << sessionAPI->mPersons.size();
    } else {
        qWarning() << "cannot get PersonsAPI* at pos " << pos << "Object is not of type SessionAPI*";
    }
    return 0;
}
void SessionAPI::clearPersonsProperty(QQmlListProperty<PersonsAPI> *personsList)
{
    SessionAPI *sessionAPI = qobject_cast<SessionAPI *>(personsList->object);
    if (sessionAPI) {
        // persons are independent - DON'T delete them
        sessionAPI->mPersons.clear();
    } else {
        qWarning() << "cannot clear persons " << "Object is not of type SessionAPI*";
    }
}

// ATT 
// Optional: sessionLinks
QVariantList SessionAPI::sessionLinksAsQVariantList()
{
	QVariantList sessionLinksList;
	for (int i = 0; i < mSessionLinks.size(); ++i) {
        sessionLinksList.append((mSessionLinks.at(i))->toMap());
    }
	return sessionLinksList;
}
QVariantList SessionAPI::sessionLinksAsForeignQVariantList()
{
	QVariantList sessionLinksList;
	for (int i = 0; i < mSessionLinks.size(); ++i) {
        sessionLinksList.append((mSessionLinks.at(i))->toForeignMap());
    }
	return sessionLinksList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add sessionLinks without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveSessionLinksKeys
 */
void SessionAPI::addToSessionLinks(SessionLinkAPI* sessionLinkAPI)
{
    mSessionLinks.append(sessionLinkAPI);
    emit addedToSessionLinks(sessionLinkAPI);
    emit sessionLinksPropertyListChanged();
}

bool SessionAPI::removeFromSessionLinks(SessionLinkAPI* sessionLinkAPI)
{
    bool ok = false;
    ok = mSessionLinks.removeOne(sessionLinkAPI);
    if (!ok) {
    	qDebug() << "SessionLinkAPI* not found in sessionLinks";
    	return false;
    }
    emit sessionLinksPropertyListChanged();
    // sessionLinks are independent - DON'T delete them
    return true;
}
void SessionAPI::clearSessionLinks()
{
    for (int i = mSessionLinks.size(); i > 0; --i) {
        removeFromSessionLinks(mSessionLinks.last());
    }
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: sessionLinksKeys()
 * - resolve them from DataManager
 * - then resolveSessionLinksKeys()
 */
bool SessionAPI::areSessionLinksKeysResolved()
{
    return mSessionLinksKeysResolved;
}

QStringList SessionAPI::sessionLinksKeys()
{
    return mSessionLinksKeys;
}

/**
 * Objects from sessionLinksKeys will be added to existing sessionLinks
 * This enables to use addToSessionLinks() without resolving before
 * Hint: it's your responsibility to resolve before looping thru sessionLinks
 */
void SessionAPI::resolveSessionLinksKeys(QList<SessionLinkAPI*> sessionLinks)
{
    if(mSessionLinksKeysResolved){
        return;
    }
    // don't clear mSessionLinks (see above)
    for (int i = 0; i < sessionLinks.size(); ++i) {
        addToSessionLinks(sessionLinks.at(i));
    }
    mSessionLinksKeysResolved = true;
}

int SessionAPI::sessionLinksCount()
{
    return mSessionLinks.size();
}
QList<SessionLinkAPI*> SessionAPI::sessionLinks()
{
	return mSessionLinks;
}
void SessionAPI::setSessionLinks(QList<SessionLinkAPI*> sessionLinks) 
{
	if (sessionLinks != mSessionLinks) {
		mSessionLinks = sessionLinks;
		emit sessionLinksChanged(sessionLinks);
		emit sessionLinksPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * sessionAPI.sessionLinksPropertyList.length to get the size
 * sessionAPI.sessionLinksPropertyList[2] to get SessionLinkAPI* at position 2
 * sessionAPI.sessionLinksPropertyList = [] to clear the list
 * or get easy access to properties like
 * sessionAPI.sessionLinksPropertyList[2].myPropertyName
 */
QQmlListProperty<SessionLinkAPI> SessionAPI::sessionLinksPropertyList()
{
    return QQmlListProperty<SessionLinkAPI>(this, 0, &SessionAPI::appendToSessionLinksProperty,
            &SessionAPI::sessionLinksPropertyCount, &SessionAPI::atSessionLinksProperty,
            &SessionAPI::clearSessionLinksProperty);
}
void SessionAPI::appendToSessionLinksProperty(QQmlListProperty<SessionLinkAPI> *sessionLinksList,
        SessionLinkAPI* sessionLinkAPI)
{
    SessionAPI *sessionAPIObject = qobject_cast<SessionAPI *>(sessionLinksList->object);
    if (sessionAPIObject) {
        sessionAPIObject->mSessionLinks.append(sessionLinkAPI);
        emit sessionAPIObject->addedToSessionLinks(sessionLinkAPI);
    } else {
        qWarning() << "cannot append SessionLinkAPI* to sessionLinks " << "Object is not of type SessionAPI*";
    }
}
int SessionAPI::sessionLinksPropertyCount(QQmlListProperty<SessionLinkAPI> *sessionLinksList)
{
    SessionAPI *sessionAPI = qobject_cast<SessionAPI *>(sessionLinksList->object);
    if (sessionAPI) {
        return sessionAPI->mSessionLinks.size();
    } else {
        qWarning() << "cannot get size sessionLinks " << "Object is not of type SessionAPI*";
    }
    return 0;
}
SessionLinkAPI* SessionAPI::atSessionLinksProperty(QQmlListProperty<SessionLinkAPI> *sessionLinksList, int pos)
{
    SessionAPI *sessionAPI = qobject_cast<SessionAPI *>(sessionLinksList->object);
    if (sessionAPI) {
        if (sessionAPI->mSessionLinks.size() > pos) {
            return sessionAPI->mSessionLinks.at(pos);
        }
        qWarning() << "cannot get SessionLinkAPI* at pos " << pos << " size is "
                << sessionAPI->mSessionLinks.size();
    } else {
        qWarning() << "cannot get SessionLinkAPI* at pos " << pos << "Object is not of type SessionAPI*";
    }
    return 0;
}
void SessionAPI::clearSessionLinksProperty(QQmlListProperty<SessionLinkAPI> *sessionLinksList)
{
    SessionAPI *sessionAPI = qobject_cast<SessionAPI *>(sessionLinksList->object);
    if (sessionAPI) {
        // sessionLinks are independent - DON'T delete them
        sessionAPI->mSessionLinks.clear();
    } else {
        qWarning() << "cannot clear sessionLinks " << "Object is not of type SessionAPI*";
    }
}



SessionAPI::~SessionAPI()
{
	// place cleanUp code here
}
	
