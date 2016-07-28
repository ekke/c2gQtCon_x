#include "Bookmark.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString sessionIdKey = "sessionId";
static const QString sessionKey = "session";

// keys used from Server API etc
static const QString sessionIdForeignKey = "sessionId";
static const QString sessionForeignKey = "session";

/*
 * Default Constructor if Bookmark not initialized from QVariantMap
 */
Bookmark::Bookmark(QObject *parent) :
        QObject(parent), mSessionId(-1)
{
	// lazy references:
	mSession = -1;
	mSessionAsDataObject = 0;
	mSessionInvalid = false;
}

bool Bookmark::isAllResolved()
{
	if (hasSession() && !isSessionResolvedAsDataObject()) {
		return false;
	}
    return true;
}

/*
 * initialize Bookmark from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Bookmark::fillFromMap(const QVariantMap& bookmarkMap)
{
	mSessionId = bookmarkMap.value(sessionIdKey).toInt();
	// session lazy pointing to Session* (domainKey: sessionId)
	if (bookmarkMap.contains(sessionKey)) {
		mSession = bookmarkMap.value(sessionKey).toInt();
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
void Bookmark::fillFromForeignMap(const QVariantMap& bookmarkMap)
{
	mSessionId = bookmarkMap.value(sessionIdForeignKey).toInt();
	// session lazy pointing to Session* (domainKey: sessionId)
	if (bookmarkMap.contains(sessionForeignKey)) {
		mSession = bookmarkMap.value(sessionForeignKey).toInt();
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
void Bookmark::fillFromCacheMap(const QVariantMap& bookmarkMap)
{
	mSessionId = bookmarkMap.value(sessionIdKey).toInt();
	// session lazy pointing to Session* (domainKey: sessionId)
	if (bookmarkMap.contains(sessionKey)) {
		mSession = bookmarkMap.value(sessionKey).toInt();
		if (mSession != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
}

void Bookmark::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Bookmark::isValid()
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
 * Exports Properties from Bookmark as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Bookmark::toMap()
{
	QVariantMap bookmarkMap;
	// session lazy pointing to Session* (domainKey: sessionId)
	if (mSession != -1) {
		bookmarkMap.insert(sessionKey, mSession);
	}
	bookmarkMap.insert(sessionIdKey, mSessionId);
	return bookmarkMap;
}

/*
 * Exports Properties from Bookmark as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Bookmark::toForeignMap()
{
	QVariantMap bookmarkMap;
	// session lazy pointing to Session* (domainKey: sessionId)
	if (mSession != -1) {
		bookmarkMap.insert(sessionForeignKey, mSession);
	}
	bookmarkMap.insert(sessionIdForeignKey, mSessionId);
	return bookmarkMap;
}


/*
 * Exports Properties from Bookmark as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Bookmark::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// REF
// Lazy: session
// Mandatory: session
// session lazy pointing to Session* (domainKey: sessionId)
int Bookmark::session() const
{
	return mSession;
}
Session* Bookmark::sessionAsDataObject() const
{
	return mSessionAsDataObject;
}
void Bookmark::setSession(int session)
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
void Bookmark::removeSession()
{
	if (mSession != -1) {
		setSession(-1);
	}
}
bool Bookmark::hasSession()
{
    if (!mSessionInvalid && mSession != -1) {
        return true;
    } else {
        return false;
    }
}
bool Bookmark::isSessionResolvedAsDataObject()
{
    if (!mSessionInvalid && mSessionAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite sessionId if different
void Bookmark::resolveSessionAsDataObject(Session* session)
{
    if (session) {
        if (session->sessionId() != mSession) {
            setSession(session->sessionId());
        }
        mSessionAsDataObject = session;
        mSessionInvalid = false;
    }
}
void Bookmark::markSessionAsInvalid()
{
    mSessionInvalid = true;
}
// ATT 
// Mandatory: sessionId
// Domain KEY: sessionId
int Bookmark::sessionId() const
{
	return mSessionId;
}

void Bookmark::setSessionId(int sessionId)
{
	if (sessionId != mSessionId) {
		mSessionId = sessionId;
		emit sessionIdChanged(sessionId);
	}
}


Bookmark::~Bookmark()
{
	// place cleanUp code here
}
	
