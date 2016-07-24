#include "SessionLinkAPI.hpp"
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
 * Default Constructor if SessionLinkAPI not initialized from QVariantMap
 */
SessionLinkAPI::SessionLinkAPI(QObject *parent) :
        QObject(parent), mUuid(""), mUrl(""), mTitle("")
{
}

/*
 * initialize SessionLinkAPI from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void SessionLinkAPI::fillFromMap(const QVariantMap& sessionLinkAPIMap)
{
	mUuid = sessionLinkAPIMap.value(uuidKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mUrl = sessionLinkAPIMap.value(urlKey).toString();
	mTitle = sessionLinkAPIMap.value(titleKey).toString();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void SessionLinkAPI::fillFromForeignMap(const QVariantMap& sessionLinkAPIMap)
{
	mUuid = sessionLinkAPIMap.value(uuidForeignKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mUrl = sessionLinkAPIMap.value(urlForeignKey).toString();
	mTitle = sessionLinkAPIMap.value(titleForeignKey).toString();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void SessionLinkAPI::fillFromCacheMap(const QVariantMap& sessionLinkAPIMap)
{
	mUuid = sessionLinkAPIMap.value(uuidKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mUrl = sessionLinkAPIMap.value(urlKey).toString();
	mTitle = sessionLinkAPIMap.value(titleKey).toString();
}

void SessionLinkAPI::prepareNew()
{
	mUuid = QUuid::createUuid().toString();
	mUuid = mUuid.right(mUuid.length() - 1);
	mUuid = mUuid.left(mUuid.length() - 1);
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool SessionLinkAPI::isValid()
{
	if (mUuid.isNull() || mUuid.isEmpty()) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from SessionLinkAPI as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap SessionLinkAPI::toMap()
{
	QVariantMap sessionLinkAPIMap;
	sessionLinkAPIMap.insert(uuidKey, mUuid);
	sessionLinkAPIMap.insert(urlKey, mUrl);
	sessionLinkAPIMap.insert(titleKey, mTitle);
	return sessionLinkAPIMap;
}

/*
 * Exports Properties from SessionLinkAPI as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap SessionLinkAPI::toForeignMap()
{
	QVariantMap sessionLinkAPIMap;
	sessionLinkAPIMap.insert(uuidForeignKey, mUuid);
	sessionLinkAPIMap.insert(urlForeignKey, mUrl);
	sessionLinkAPIMap.insert(titleForeignKey, mTitle);
	return sessionLinkAPIMap;
}


/*
 * Exports Properties from SessionLinkAPI as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap SessionLinkAPI::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: uuid
// Domain KEY: uuid
QString SessionLinkAPI::uuid() const
{
	return mUuid;
}

void SessionLinkAPI::setUuid(QString uuid)
{
	if (uuid != mUuid) {
		mUuid = uuid;
		emit uuidChanged(uuid);
	}
}
// ATT 
// Optional: url
QString SessionLinkAPI::url() const
{
	return mUrl;
}

void SessionLinkAPI::setUrl(QString url)
{
	if (url != mUrl) {
		mUrl = url;
		emit urlChanged(url);
	}
}
// ATT 
// Optional: title
QString SessionLinkAPI::title() const
{
	return mTitle;
}

void SessionLinkAPI::setTitle(QString title)
{
	if (title != mTitle) {
		mTitle = title;
		emit titleChanged(title);
	}
}


SessionLinkAPI::~SessionLinkAPI()
{
	// place cleanUp code here
}
	
