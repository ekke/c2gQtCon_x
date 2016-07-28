#include "Favorite.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString sessionIdKey = "sessionId";
static const QString sessionKey = "session";

// keys used from Server API etc
static const QString sessionIdForeignKey = "sessionId";
static const QString sessionForeignKey = "session";

/*
 * Default Constructor if Favorite not initialized from QVariantMap
 */
Favorite::Favorite(QObject *parent) :
        QObject(parent), mSessionId(-1)
{
	// lazy references:
	mSession = -1;
	mSessionAsDataObject = 0;
	mSessionInvalid = false;
}

bool Favorite::isAllResolved()
{
	if (hasSession() && !isSessionResolvedAsDataObject()) {
		return false;
	}
    return true;
}

/*
 * initialize Favorite from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Favorite::fillFromMap(const QVariantMap& favoriteMap)
{
	mSessionId = favoriteMap.value(sessionIdKey).toInt();
	// session lazy pointing to Session* (domainKey: sessionId)
	if (favoriteMap.contains(sessionKey)) {
		mSession = favoriteMap.value(sessionKey).toInt();
		if (mSession != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Favorite::fillFromForeignMap(const QVariantMap& favoriteMap)
{
	mSessionId = favoriteMap.value(sessionIdForeignKey).toInt();
	// session lazy pointing to Session* (domainKey: sessionId)
	if (favoriteMap.contains(sessionForeignKey)) {
		mSession = favoriteMap.value(sessionForeignKey).toInt();
		if (mSession != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Favorite::fillFromCacheMap(const QVariantMap& favoriteMap)
{
	mSessionId = favoriteMap.value(sessionIdKey).toInt();
	// session lazy pointing to Session* (domainKey: sessionId)
	if (favoriteMap.contains(sessionKey)) {
		mSession = favoriteMap.value(sessionKey).toInt();
		if (mSession != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
}

void Favorite::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Favorite::isValid()
{
	if (mSessionId == -1) {
		return false;
	}
	// session lazy pointing to Session* (domainKey: sessionId)
	if (mSession == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Favorite as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Favorite::toMap()
{
	QVariantMap favoriteMap;
	// session lazy pointing to Session* (domainKey: sessionId)
	if (mSession != -1) {
		favoriteMap.insert(sessionKey, mSession);
	}
	favoriteMap.insert(sessionIdKey, mSessionId);
	return favoriteMap;
}

/*
 * Exports Properties from Favorite as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Favorite::toForeignMap()
{
	QVariantMap favoriteMap;
	// session lazy pointing to Session* (domainKey: sessionId)
	if (mSession != -1) {
		favoriteMap.insert(sessionForeignKey, mSession);
	}
	favoriteMap.insert(sessionIdForeignKey, mSessionId);
	return favoriteMap;
}


/*
 * Exports Properties from Favorite as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Favorite::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// REF
// Lazy: session
// Mandatory: session
// session lazy pointing to Session* (domainKey: sessionId)
int Favorite::session() const
{
	return mSession;
}
Session* Favorite::sessionAsDataObject() const
{
	return mSessionAsDataObject;
}
void Favorite::setSession(int session)
{
	if (session != mSession) {
        // remove old Data Object if one was resolved
        if (mSessionAsDataObject) {
            // reset pointer, don't delete the independent object !
            mSessionAsDataObject = 0;
        }
        // set the new lazy reference
        mSession = session;
        mSessionInvalid = false;
        emit sessionChanged(session);
        if (session != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Favorite::removeSession()
{
	if (mSession != -1) {
		setSession(-1);
	}
}
bool Favorite::hasSession()
{
    if (!mSessionInvalid && mSession != -1) {
        return true;
    } else {
        return false;
    }
}
bool Favorite::isSessionResolvedAsDataObject()
{
    if (!mSessionInvalid && mSessionAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite sessionId if different
void Favorite::resolveSessionAsDataObject(Session* session)
{
    if (session) {
        if (session->sessionId() != mSession) {
            setSession(session->sessionId());
        }
        mSessionAsDataObject = session;
        mSessionInvalid = false;
    }
}
void Favorite::markSessionAsInvalid()
{
    mSessionInvalid = true;
}
// ATT 
// Mandatory: sessionId
// Domain KEY: sessionId
int Favorite::sessionId() const
{
	return mSessionId;
}

void Favorite::setSessionId(int sessionId)
{
	if (sessionId != mSessionId) {
		mSessionId = sessionId;
		emit sessionIdChanged(sessionId);
	}
}


Favorite::~Favorite()
{
	// place cleanUp code here
}
	
