#include "PersonsAPI.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString idKey = "id";

// keys used from Server API etc
static const QString idForeignKey = "id";

/*
 * Default Constructor if PersonsAPI not initialized from QVariantMap
 */
PersonsAPI::PersonsAPI(QObject *parent) :
        QObject(parent), mId(-1)
{
}

/*
 * initialize PersonsAPI from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void PersonsAPI::fillFromMap(const QVariantMap& personsAPIMap)
{
	mId = personsAPIMap.value(idKey).toInt();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void PersonsAPI::fillFromForeignMap(const QVariantMap& personsAPIMap)
{
	mId = personsAPIMap.value(idForeignKey).toInt();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void PersonsAPI::fillFromCacheMap(const QVariantMap& personsAPIMap)
{
	mId = personsAPIMap.value(idKey).toInt();
}

void PersonsAPI::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool PersonsAPI::isValid()
{
	if (mId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from PersonsAPI as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap PersonsAPI::toMap()
{
	QVariantMap personsAPIMap;
	personsAPIMap.insert(idKey, mId);
	return personsAPIMap;
}

/*
 * Exports Properties from PersonsAPI as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap PersonsAPI::toForeignMap()
{
	QVariantMap personsAPIMap;
	personsAPIMap.insert(idForeignKey, mId);
	return personsAPIMap;
}


/*
 * Exports Properties from PersonsAPI as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap PersonsAPI::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: id
// Domain KEY: id
int PersonsAPI::id() const
{
	return mId;
}

void PersonsAPI::setId(int id)
{
	if (id != mId) {
		mId = id;
		emit idChanged(id);
	}
}


PersonsAPI::~PersonsAPI()
{
	// place cleanUp code here
}
	
