#include "SpeakerAPI.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString idKey = "id";
static const QString firstNameKey = "firstName";
static const QString lastNameKey = "lastName";
static const QString bioKey = "bio";
static const QString avatarKey = "avatar";

// keys used from Server API etc
static const QString idForeignKey = "id";
static const QString firstNameForeignKey = "first_name";
static const QString lastNameForeignKey = "last_name";
static const QString bioForeignKey = "abstract";
static const QString avatarForeignKey = "avatar";

/*
 * Default Constructor if SpeakerAPI not initialized from QVariantMap
 */
SpeakerAPI::SpeakerAPI(QObject *parent) :
        QObject(parent), mId(-1), mFirstName(""), mLastName(""), mBio(""), mAvatar("")
{
}

/*
 * initialize SpeakerAPI from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void SpeakerAPI::fillFromMap(const QVariantMap& speakerAPIMap)
{
	mId = speakerAPIMap.value(idKey).toInt();
	mFirstName = speakerAPIMap.value(firstNameKey).toString();
	mLastName = speakerAPIMap.value(lastNameKey).toString();
	mBio = speakerAPIMap.value(bioKey).toString();
	mAvatar = speakerAPIMap.value(avatarKey).toString();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void SpeakerAPI::fillFromForeignMap(const QVariantMap& speakerAPIMap)
{
	mId = speakerAPIMap.value(idForeignKey).toInt();
	mFirstName = speakerAPIMap.value(firstNameForeignKey).toString();
	mLastName = speakerAPIMap.value(lastNameForeignKey).toString();
	mBio = speakerAPIMap.value(bioForeignKey).toString();
	mAvatar = speakerAPIMap.value(avatarForeignKey).toString();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void SpeakerAPI::fillFromCacheMap(const QVariantMap& speakerAPIMap)
{
	mId = speakerAPIMap.value(idKey).toInt();
	mFirstName = speakerAPIMap.value(firstNameKey).toString();
	mLastName = speakerAPIMap.value(lastNameKey).toString();
	mBio = speakerAPIMap.value(bioKey).toString();
	mAvatar = speakerAPIMap.value(avatarKey).toString();
}

void SpeakerAPI::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool SpeakerAPI::isValid()
{
	if (mId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from SpeakerAPI as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap SpeakerAPI::toMap()
{
	QVariantMap speakerAPIMap;
	speakerAPIMap.insert(idKey, mId);
	speakerAPIMap.insert(firstNameKey, mFirstName);
	speakerAPIMap.insert(lastNameKey, mLastName);
	speakerAPIMap.insert(bioKey, mBio);
	speakerAPIMap.insert(avatarKey, mAvatar);
	return speakerAPIMap;
}

/*
 * Exports Properties from SpeakerAPI as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap SpeakerAPI::toForeignMap()
{
	QVariantMap speakerAPIMap;
	speakerAPIMap.insert(idForeignKey, mId);
	speakerAPIMap.insert(firstNameForeignKey, mFirstName);
	speakerAPIMap.insert(lastNameForeignKey, mLastName);
	speakerAPIMap.insert(bioForeignKey, mBio);
	speakerAPIMap.insert(avatarForeignKey, mAvatar);
	return speakerAPIMap;
}


/*
 * Exports Properties from SpeakerAPI as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap SpeakerAPI::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: id
// Domain KEY: id
int SpeakerAPI::id() const
{
	return mId;
}

void SpeakerAPI::setId(int id)
{
	if (id != mId) {
		mId = id;
		emit idChanged(id);
	}
}
// ATT 
// Optional: firstName
QString SpeakerAPI::firstName() const
{
	return mFirstName;
}

void SpeakerAPI::setFirstName(QString firstName)
{
	if (firstName != mFirstName) {
		mFirstName = firstName;
		emit firstNameChanged(firstName);
	}
}
// ATT 
// Optional: lastName
QString SpeakerAPI::lastName() const
{
	return mLastName;
}

void SpeakerAPI::setLastName(QString lastName)
{
	if (lastName != mLastName) {
		mLastName = lastName;
		emit lastNameChanged(lastName);
	}
}
// ATT 
// Optional: bio
QString SpeakerAPI::bio() const
{
	return mBio;
}

void SpeakerAPI::setBio(QString bio)
{
	if (bio != mBio) {
		mBio = bio;
		emit bioChanged(bio);
	}
}
// ATT 
// Optional: avatar
QString SpeakerAPI::avatar() const
{
	return mAvatar;
}

void SpeakerAPI::setAvatar(QString avatar)
{
	if (avatar != mAvatar) {
		mAvatar = avatar;
		emit avatarChanged(avatar);
	}
}


SpeakerAPI::~SpeakerAPI()
{
	// place cleanUp code here
}
	
