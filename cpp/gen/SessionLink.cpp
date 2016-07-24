#include "SessionLink.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString uuidKey = "uuid";
static const QString urlKey = "url";
static const QString titleKey = "title";

// keys used from Server API etc
static const QString uuidForeignKey = "uuid";
static const QString urlForeignKey = "url";
static const QString titleForeignKey = "title";

/*
 * Default Constructor if SessionLink not initialized from QVariantMap
 */
SessionLink::SessionLink(QObject *parent) :
        QObject(parent), mUuid(""), mUrl(""), mTitle("")
{
}

/*
 * initialize SessionLink from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void SessionLink::fillFromMap(const QVariantMap& sessionLinkMap)
{
	mUuid = sessionLinkMap.value(uuidKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mUrl = sessionLinkMap.value(urlKey).toString();
	mTitle = sessionLinkMap.value(titleKey).toString();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void SessionLink::fillFromForeignMap(const QVariantMap& sessionLinkMap)
{
	mUuid = sessionLinkMap.value(uuidForeignKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mUrl = sessionLinkMap.value(urlForeignKey).toString();
	mTitle = sessionLinkMap.value(titleForeignKey).toString();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void SessionLink::fillFromCacheMap(const QVariantMap& sessionLinkMap)
{
	mUuid = sessionLinkMap.value(uuidKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mUrl = sessionLinkMap.value(urlKey).toString();
	mTitle = sessionLinkMap.value(titleKey).toString();
}

void SessionLink::prepareNew()
{
	mUuid = QUuid::createUuid().toString();
	mUuid = mUuid.right(mUuid.length() - 1);
	mUuid = mUuid.left(mUuid.length() - 1);
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool SessionLink::isValid()
{
	if (mUuid.isNull() || mUuid.isEmpty()) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from SessionLink as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap SessionLink::toMap()
{
	QVariantMap sessionLinkMap;
	sessionLinkMap.insert(uuidKey, mUuid);
	sessionLinkMap.insert(urlKey, mUrl);
	sessionLinkMap.insert(titleKey, mTitle);
	return sessionLinkMap;
}

/*
 * Exports Properties from SessionLink as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap SessionLink::toForeignMap()
{
	QVariantMap sessionLinkMap;
	sessionLinkMap.insert(uuidForeignKey, mUuid);
	sessionLinkMap.insert(urlForeignKey, mUrl);
	sessionLinkMap.insert(titleForeignKey, mTitle);
	return sessionLinkMap;
}


/*
 * Exports Properties from SessionLink as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap SessionLink::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: uuid
// Domain KEY: uuid
QString SessionLink::uuid() const
{
	return mUuid;
}

void SessionLink::setUuid(QString uuid)
{
	if (uuid != mUuid) {
		mUuid = uuid;
		emit uuidChanged(uuid);
	}
}
// ATT 
// Optional: url
QString SessionLink::url() const
{
	return mUrl;
}

void SessionLink::setUrl(QString url)
{
	if (url != mUrl) {
		mUrl = url;
		emit urlChanged(url);
	}
}
// ATT 
// Optional: title
QString SessionLink::title() const
{
	return mTitle;
}

void SessionLink::setTitle(QString title)
{
	if (title != mTitle) {
		mTitle = title;
		emit titleChanged(title);
	}
}


SessionLink::~SessionLink()
{
	// place cleanUp code here
}
	
