#include "ScheduleItem.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Session.hpp"

// keys of QVariantMap used in this APP
static const QString sessionIdKey = "sessionId";
static const QString isBreakKey = "isBreak";
static const QString isLunchKey = "isLunch";
static const QString isEventKey = "isEvent";
static const QString isRegistrationKey = "isRegistration";
static const QString sessionKey = "session";

// keys used from Server API etc
static const QString sessionIdForeignKey = "sessionId";
static const QString isBreakForeignKey = "isBreak";
static const QString isLunchForeignKey = "isLunch";
static const QString isEventForeignKey = "isEvent";
static const QString isRegistrationForeignKey = "isRegistration";
static const QString sessionForeignKey = "session";

/*
 * Default Constructor if ScheduleItem not initialized from QVariantMap
 */
ScheduleItem::ScheduleItem(QObject *parent) :
        QObject(parent), mSessionId(-1), mIsBreak(false), mIsLunch(false), mIsEvent(false), mIsRegistration(false)
{
	// lazy references:
	mSession = -1;
	mSessionAsDataObject = 0;
	mSessionInvalid = false;
}

bool ScheduleItem::isAllResolved()
{
	if (hasSession() && !isSessionResolvedAsDataObject()) {
		return false;
	}
    return true;
}

/*
 * initialize ScheduleItem from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void ScheduleItem::fillFromMap(const QVariantMap& scheduleItemMap)
{
	mSessionId = scheduleItemMap.value(sessionIdKey).toInt();
	mIsBreak = scheduleItemMap.value(isBreakKey).toBool();
	mIsLunch = scheduleItemMap.value(isLunchKey).toBool();
	mIsEvent = scheduleItemMap.value(isEventKey).toBool();
	mIsRegistration = scheduleItemMap.value(isRegistrationKey).toBool();
	// session lazy pointing to Session* (domainKey: sessionId)
	if (scheduleItemMap.contains(sessionKey)) {
		mSession = scheduleItemMap.value(sessionKey).toInt();
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
void ScheduleItem::fillFromForeignMap(const QVariantMap& scheduleItemMap)
{
	mSessionId = scheduleItemMap.value(sessionIdForeignKey).toInt();
	mIsBreak = scheduleItemMap.value(isBreakForeignKey).toBool();
	mIsLunch = scheduleItemMap.value(isLunchForeignKey).toBool();
	mIsEvent = scheduleItemMap.value(isEventForeignKey).toBool();
	mIsRegistration = scheduleItemMap.value(isRegistrationForeignKey).toBool();
	// session lazy pointing to Session* (domainKey: sessionId)
	if (scheduleItemMap.contains(sessionForeignKey)) {
		mSession = scheduleItemMap.value(sessionForeignKey).toInt();
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
void ScheduleItem::fillFromCacheMap(const QVariantMap& scheduleItemMap)
{
	mSessionId = scheduleItemMap.value(sessionIdKey).toInt();
	mIsBreak = scheduleItemMap.value(isBreakKey).toBool();
	mIsLunch = scheduleItemMap.value(isLunchKey).toBool();
	mIsEvent = scheduleItemMap.value(isEventKey).toBool();
	mIsRegistration = scheduleItemMap.value(isRegistrationKey).toBool();
	// session lazy pointing to Session* (domainKey: sessionId)
	if (scheduleItemMap.contains(sessionKey)) {
		mSession = scheduleItemMap.value(sessionKey).toInt();
		if (mSession != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
}

void ScheduleItem::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool ScheduleItem::isValid()
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
 * Exports Properties from ScheduleItem as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap ScheduleItem::toMap()
{
	QVariantMap scheduleItemMap;
	// session lazy pointing to Session* (domainKey: sessionId)
	if (mSession != -1) {
		scheduleItemMap.insert(sessionKey, mSession);
	}
	scheduleItemMap.insert(sessionIdKey, mSessionId);
	scheduleItemMap.insert(isBreakKey, mIsBreak);
	scheduleItemMap.insert(isLunchKey, mIsLunch);
	scheduleItemMap.insert(isEventKey, mIsEvent);
	scheduleItemMap.insert(isRegistrationKey, mIsRegistration);
	return scheduleItemMap;
}

/*
 * Exports Properties from ScheduleItem as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap ScheduleItem::toForeignMap()
{
	QVariantMap scheduleItemMap;
	// session lazy pointing to Session* (domainKey: sessionId)
	if (mSession != -1) {
		scheduleItemMap.insert(sessionForeignKey, mSession);
	}
	scheduleItemMap.insert(sessionIdForeignKey, mSessionId);
	scheduleItemMap.insert(isBreakForeignKey, mIsBreak);
	scheduleItemMap.insert(isLunchForeignKey, mIsLunch);
	scheduleItemMap.insert(isEventForeignKey, mIsEvent);
	scheduleItemMap.insert(isRegistrationForeignKey, mIsRegistration);
	return scheduleItemMap;
}


/*
 * Exports Properties from ScheduleItem as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap ScheduleItem::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// REF
// Lazy: session
// Mandatory: session
// session lazy pointing to Session* (domainKey: sessionId)
int ScheduleItem::session() const
{
	return mSession;
}
Session* ScheduleItem::sessionAsDataObject() const
{
	return mSessionAsDataObject;
}
void ScheduleItem::setSession(int session)
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
void ScheduleItem::removeSession()
{
	if (mSession != -1) {
		setSession(-1);
	}
}
bool ScheduleItem::hasSession()
{
    if (!mSessionInvalid && mSession != -1) {
        return true;
    } else {
        return false;
    }
}
bool ScheduleItem::isSessionResolvedAsDataObject()
{
    if (!mSessionInvalid && mSessionAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite sessionId if different
void ScheduleItem::resolveSessionAsDataObject(Session* session)
{
    if (session) {
        if (session->sessionId() != mSession) {
            setSession(session->sessionId());
        }
        mSessionAsDataObject = session;
        mSessionInvalid = false;
    }
}
void ScheduleItem::markSessionAsInvalid()
{
    mSessionInvalid = true;
}
// ATT 
// Mandatory: sessionId
// Domain KEY: sessionId
int ScheduleItem::sessionId() const
{
	return mSessionId;
}

void ScheduleItem::setSessionId(int sessionId)
{
	if (sessionId != mSessionId) {
		mSessionId = sessionId;
		emit sessionIdChanged(sessionId);
	}
}
// ATT 
// Optional: isBreak
bool ScheduleItem::isBreak() const
{
	return mIsBreak;
}

void ScheduleItem::setIsBreak(bool isBreak)
{
	if (isBreak != mIsBreak) {
		mIsBreak = isBreak;
		emit isBreakChanged(isBreak);
	}
}
// ATT 
// Optional: isLunch
bool ScheduleItem::isLunch() const
{
	return mIsLunch;
}

void ScheduleItem::setIsLunch(bool isLunch)
{
	if (isLunch != mIsLunch) {
		mIsLunch = isLunch;
		emit isLunchChanged(isLunch);
	}
}
// ATT 
// Optional: isEvent
bool ScheduleItem::isEvent() const
{
	return mIsEvent;
}

void ScheduleItem::setIsEvent(bool isEvent)
{
	if (isEvent != mIsEvent) {
		mIsEvent = isEvent;
		emit isEventChanged(isEvent);
	}
}
// ATT 
// Optional: isRegistration
bool ScheduleItem::isRegistration() const
{
	return mIsRegistration;
}

void ScheduleItem::setIsRegistration(bool isRegistration)
{
	if (isRegistration != mIsRegistration) {
		mIsRegistration = isRegistration;
		emit isRegistrationChanged(isRegistration);
	}
}


ScheduleItem::~ScheduleItem()
{
	// place cleanUp code here
}
	
