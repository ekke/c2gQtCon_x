#include "SessionTrack.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Session.hpp"

// keys of QVariantMap used in this APP
static const QString trackIdKey = "trackId";
static const QString nameKey = "name";
static const QString inAssetsKey = "inAssets";
static const QString sessionsKey = "sessions";

// keys used from Server API etc
static const QString trackIdForeignKey = "trackId";
static const QString nameForeignKey = "name";
static const QString inAssetsForeignKey = "inAssets";
static const QString sessionsForeignKey = "sessions";

/*
 * Default Constructor if SessionTrack not initialized from QVariantMap
 */
SessionTrack::SessionTrack(QObject *parent) :
        QObject(parent), mTrackId(-1), mName(""), mInAssets(false)
{
		// lazy Arrays where only keys are persisted
		mSessionsKeysResolved = false;
}

bool SessionTrack::isAllResolved()
{
    if(!areSessionsKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize SessionTrack from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void SessionTrack::fillFromMap(const QVariantMap& sessionTrackMap)
{
	mTrackId = sessionTrackMap.value(trackIdKey).toInt();
	mName = sessionTrackMap.value(nameKey).toString();
	mInAssets = sessionTrackMap.value(inAssetsKey).toBool();
	// mSessions is (lazy loaded) Array of Session*
	mSessionsKeys = sessionTrackMap.value(sessionsKey).toStringList();
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
void SessionTrack::fillFromForeignMap(const QVariantMap& sessionTrackMap)
{
	mTrackId = sessionTrackMap.value(trackIdForeignKey).toInt();
	mName = sessionTrackMap.value(nameForeignKey).toString();
	mInAssets = sessionTrackMap.value(inAssetsForeignKey).toBool();
	// mSessions is (lazy loaded) Array of Session*
	mSessionsKeys = sessionTrackMap.value(sessionsForeignKey).toStringList();
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
void SessionTrack::fillFromCacheMap(const QVariantMap& sessionTrackMap)
{
	mTrackId = sessionTrackMap.value(trackIdKey).toInt();
	mName = sessionTrackMap.value(nameKey).toString();
	mInAssets = sessionTrackMap.value(inAssetsKey).toBool();
	// mSessions is (lazy loaded) Array of Session*
	mSessionsKeys = sessionTrackMap.value(sessionsKey).toStringList();
	// mSessions must be resolved later if there are keys
	mSessionsKeysResolved = (mSessionsKeys.size() == 0);
	mSessions.clear();
}

void SessionTrack::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool SessionTrack::isValid()
{
	if (mTrackId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from SessionTrack as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap SessionTrack::toMap()
{
	QVariantMap sessionTrackMap;
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
	sessionTrackMap.insert(sessionsKey, mSessionsKeys);
	sessionTrackMap.insert(trackIdKey, mTrackId);
	sessionTrackMap.insert(nameKey, mName);
	sessionTrackMap.insert(inAssetsKey, mInAssets);
	return sessionTrackMap;
}

/*
 * Exports Properties from SessionTrack as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap SessionTrack::toForeignMap()
{
	QVariantMap sessionTrackMap;
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
	sessionTrackMap.insert(sessionsKey, mSessionsKeys);
	sessionTrackMap.insert(trackIdForeignKey, mTrackId);
	sessionTrackMap.insert(nameForeignKey, mName);
	sessionTrackMap.insert(inAssetsForeignKey, mInAssets);
	return sessionTrackMap;
}


/*
 * Exports Properties from SessionTrack as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap SessionTrack::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: trackId
// Domain KEY: trackId
int SessionTrack::trackId() const
{
	return mTrackId;
}

void SessionTrack::setTrackId(int trackId)
{
	if (trackId != mTrackId) {
		mTrackId = trackId;
		emit trackIdChanged(trackId);
	}
}
// ATT 
// Optional: name
QString SessionTrack::name() const
{
	return mName;
}

void SessionTrack::setName(QString name)
{
	if (name != mName) {
		mName = name;
		emit nameChanged(name);
	}
}
// ATT 
// Optional: inAssets
bool SessionTrack::inAssets() const
{
	return mInAssets;
}

void SessionTrack::setInAssets(bool inAssets)
{
	if (inAssets != mInAssets) {
		mInAssets = inAssets;
		emit inAssetsChanged(inAssets);
	}
}
// ATT 
// Optional: sessions
QVariantList SessionTrack::sessionsAsQVariantList()
{
	QVariantList sessionsList;
	for (int i = 0; i < mSessions.size(); ++i) {
        sessionsList.append((mSessions.at(i))->toMap());
    }
	return sessionsList;
}
QVariantList SessionTrack::sessionsAsForeignQVariantList()
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
void SessionTrack::addToSessions(Session* session)
{
    mSessions.append(session);
    emit addedToSessions(session);
    emit sessionsPropertyListChanged();
}

bool SessionTrack::removeFromSessions(Session* session)
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
void SessionTrack::clearSessions()
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
bool SessionTrack::areSessionsKeysResolved()
{
    return mSessionsKeysResolved;
}

QStringList SessionTrack::sessionsKeys()
{
    return mSessionsKeys;
}

/**
 * Objects from sessionsKeys will be added to existing sessions
 * This enables to use addToSessions() without resolving before
 * Hint: it's your responsibility to resolve before looping thru sessions
 */
void SessionTrack::resolveSessionsKeys(QList<Session*> sessions)
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

int SessionTrack::sessionsCount()
{
    return mSessions.size();
}
QList<Session*> SessionTrack::sessions()
{
	return mSessions;
}
void SessionTrack::setSessions(QList<Session*> sessions) 
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
 * sessionTrack.sessionsPropertyList.length to get the size
 * sessionTrack.sessionsPropertyList[2] to get Session* at position 2
 * sessionTrack.sessionsPropertyList = [] to clear the list
 * or get easy access to properties like
 * sessionTrack.sessionsPropertyList[2].myPropertyName
 */
QQmlListProperty<Session> SessionTrack::sessionsPropertyList()
{
    return QQmlListProperty<Session>(this, 0, &SessionTrack::appendToSessionsProperty,
            &SessionTrack::sessionsPropertyCount, &SessionTrack::atSessionsProperty,
            &SessionTrack::clearSessionsProperty);
}
void SessionTrack::appendToSessionsProperty(QQmlListProperty<Session> *sessionsList,
        Session* session)
{
    SessionTrack *sessionTrackObject = qobject_cast<SessionTrack *>(sessionsList->object);
    if (sessionTrackObject) {
        sessionTrackObject->mSessions.append(session);
        emit sessionTrackObject->addedToSessions(session);
    } else {
        qWarning() << "cannot append Session* to sessions " << "Object is not of type SessionTrack*";
    }
}
int SessionTrack::sessionsPropertyCount(QQmlListProperty<Session> *sessionsList)
{
    SessionTrack *sessionTrack = qobject_cast<SessionTrack *>(sessionsList->object);
    if (sessionTrack) {
        return sessionTrack->mSessions.size();
    } else {
        qWarning() << "cannot get size sessions " << "Object is not of type SessionTrack*";
    }
    return 0;
}
Session* SessionTrack::atSessionsProperty(QQmlListProperty<Session> *sessionsList, int pos)
{
    SessionTrack *sessionTrack = qobject_cast<SessionTrack *>(sessionsList->object);
    if (sessionTrack) {
        if (sessionTrack->mSessions.size() > pos) {
            return sessionTrack->mSessions.at(pos);
        }
        qWarning() << "cannot get Session* at pos " << pos << " size is "
                << sessionTrack->mSessions.size();
    } else {
        qWarning() << "cannot get Session* at pos " << pos << "Object is not of type SessionTrack*";
    }
    return 0;
}
void SessionTrack::clearSessionsProperty(QQmlListProperty<Session> *sessionsList)
{
    SessionTrack *sessionTrack = qobject_cast<SessionTrack *>(sessionsList->object);
    if (sessionTrack) {
        // sessions are independent - DON'T delete them
        sessionTrack->mSessions.clear();
    } else {
        qWarning() << "cannot clear sessions " << "Object is not of type SessionTrack*";
    }
}



SessionTrack::~SessionTrack()
{
	// place cleanUp code here
}
	
