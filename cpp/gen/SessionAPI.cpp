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
static const QString presenterKey = "presenter";
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
static const QString presenterForeignKey = "persons";
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
		mPresenterKeysResolved = false;
		mSessionLinksKeysResolved = false;
}

bool SessionAPI::isAllResolved()
{
    if(!arePresenterKeysResolved()) {
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
	// mPresenter is (lazy loaded) Array of PersonsAPI*
	mPresenterKeys = sessionAPIMap.value(presenterKey).toStringList();
	// mPresenter must be resolved later if there are keys
	mPresenterKeysResolved = (mPresenterKeys.size() == 0);
	mPresenter.clear();
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
	// mPresenter is (lazy loaded) Array of PersonsAPI*
	mPresenterKeys = sessionAPIMap.value(presenterForeignKey).toStringList();
	// mPresenter must be resolved later if there are keys
	mPresenterKeysResolved = (mPresenterKeys.size() == 0);
	mPresenter.clear();
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
	// mPresenter is (lazy loaded) Array of PersonsAPI*
	mPresenterKeys = sessionAPIMap.value(presenterKey).toStringList();
	// mPresenter must be resolved later if there are keys
	mPresenterKeysResolved = (mPresenterKeys.size() == 0);
	mPresenter.clear();
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
	// mPresenter points to PersonsAPI*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mPresenterKeysResolved) {
		mPresenterKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mPresenter.size(); ++i) {
		PersonsAPI* personsAPI;
		personsAPI = mPresenter.at(i);
		mPresenterKeys << QString::number(personsAPI->speakerId());
	}
	sessionAPIMap.insert(presenterKey, mPresenterKeys);
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
	// mPresenter points to PersonsAPI*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mPresenterKeysResolved) {
		mPresenterKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mPresenter.size(); ++i) {
		PersonsAPI* personsAPI;
		personsAPI = mPresenter.at(i);
		mPresenterKeys << QString::number(personsAPI->speakerId());
	}
	sessionAPIMap.insert(presenterKey, mPresenterKeys);
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
// Optional: presenter
QVariantList SessionAPI::presenterAsQVariantList()
{
	QVariantList presenterList;
	for (int i = 0; i < mPresenter.size(); ++i) {
        presenterList.append((mPresenter.at(i))->toMap());
    }
	return presenterList;
}
QVariantList SessionAPI::presenterAsForeignQVariantList()
{
	QVariantList presenterList;
	for (int i = 0; i < mPresenter.size(); ++i) {
        presenterList.append((mPresenter.at(i))->toForeignMap());
    }
	return presenterList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add presenter without resolving existing keys before
 * attention: before looping through the objects
 * you must resolvePresenterKeys
 */
void SessionAPI::addToPresenter(PersonsAPI* personsAPI)
{
    mPresenter.append(personsAPI);
    emit addedToPresenter(personsAPI);
    emit presenterPropertyListChanged();
}

bool SessionAPI::removeFromPresenter(PersonsAPI* personsAPI)
{
    bool ok = false;
    ok = mPresenter.removeOne(personsAPI);
    if (!ok) {
    	qDebug() << "PersonsAPI* not found in presenter";
    	return false;
    }
    emit presenterPropertyListChanged();
    // presenter are independent - DON'T delete them
    return true;
}
void SessionAPI::clearPresenter()
{
    for (int i = mPresenter.size(); i > 0; --i) {
        removeFromPresenter(mPresenter.last());
    }
    mPresenterKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: presenterKeys()
 * - resolve them from DataManager
 * - then resolvePresenterKeys()
 */
bool SessionAPI::arePresenterKeysResolved()
{
    return mPresenterKeysResolved;
}

QStringList SessionAPI::presenterKeys()
{
    return mPresenterKeys;
}

/**
 * Objects from presenterKeys will be added to existing presenter
 * This enables to use addToPresenter() without resolving before
 * Hint: it's your responsibility to resolve before looping thru presenter
 */
void SessionAPI::resolvePresenterKeys(QList<PersonsAPI*> presenter)
{
    if(mPresenterKeysResolved){
        return;
    }
    // don't clear mPresenter (see above)
    for (int i = 0; i < presenter.size(); ++i) {
        addToPresenter(presenter.at(i));
    }
    mPresenterKeysResolved = true;
}

int SessionAPI::presenterCount()
{
    return mPresenter.size();
}
QList<PersonsAPI*> SessionAPI::presenter()
{
	return mPresenter;
}
void SessionAPI::setPresenter(QList<PersonsAPI*> presenter) 
{
	if (presenter != mPresenter) {
		mPresenter = presenter;
		emit presenterChanged(presenter);
		emit presenterPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * sessionAPI.presenterPropertyList.length to get the size
 * sessionAPI.presenterPropertyList[2] to get PersonsAPI* at position 2
 * sessionAPI.presenterPropertyList = [] to clear the list
 * or get easy access to properties like
 * sessionAPI.presenterPropertyList[2].myPropertyName
 */
QQmlListProperty<PersonsAPI> SessionAPI::presenterPropertyList()
{
    return QQmlListProperty<PersonsAPI>(this, 0, &SessionAPI::appendToPresenterProperty,
            &SessionAPI::presenterPropertyCount, &SessionAPI::atPresenterProperty,
            &SessionAPI::clearPresenterProperty);
}
void SessionAPI::appendToPresenterProperty(QQmlListProperty<PersonsAPI> *presenterList,
        PersonsAPI* personsAPI)
{
    SessionAPI *sessionAPIObject = qobject_cast<SessionAPI *>(presenterList->object);
    if (sessionAPIObject) {
        sessionAPIObject->mPresenter.append(personsAPI);
        emit sessionAPIObject->addedToPresenter(personsAPI);
    } else {
        qWarning() << "cannot append PersonsAPI* to presenter " << "Object is not of type SessionAPI*";
    }
}
int SessionAPI::presenterPropertyCount(QQmlListProperty<PersonsAPI> *presenterList)
{
    SessionAPI *sessionAPI = qobject_cast<SessionAPI *>(presenterList->object);
    if (sessionAPI) {
        return sessionAPI->mPresenter.size();
    } else {
        qWarning() << "cannot get size presenter " << "Object is not of type SessionAPI*";
    }
    return 0;
}
PersonsAPI* SessionAPI::atPresenterProperty(QQmlListProperty<PersonsAPI> *presenterList, int pos)
{
    SessionAPI *sessionAPI = qobject_cast<SessionAPI *>(presenterList->object);
    if (sessionAPI) {
        if (sessionAPI->mPresenter.size() > pos) {
            return sessionAPI->mPresenter.at(pos);
        }
        qWarning() << "cannot get PersonsAPI* at pos " << pos << " size is "
                << sessionAPI->mPresenter.size();
    } else {
        qWarning() << "cannot get PersonsAPI* at pos " << pos << "Object is not of type SessionAPI*";
    }
    return 0;
}
void SessionAPI::clearPresenterProperty(QQmlListProperty<PersonsAPI> *presenterList)
{
    SessionAPI *sessionAPI = qobject_cast<SessionAPI *>(presenterList->object);
    if (sessionAPI) {
        // presenter are independent - DON'T delete them
        sessionAPI->mPresenter.clear();
    } else {
        qWarning() << "cannot clear presenter " << "Object is not of type SessionAPI*";
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
    mSessionLinksKeys.clear();
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
	
