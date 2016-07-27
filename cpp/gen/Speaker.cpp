#include "Speaker.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Session.hpp"

// keys of QVariantMap used in this APP
static const QString speakerIdKey = "speakerId";
static const QString isDeprecatedKey = "isDeprecated";
static const QString sortKeyKey = "sortKey";
static const QString sortGroupKey = "sortGroup";
static const QString nameKey = "name";
static const QString publicNameKey = "publicName";
static const QString bioKey = "bio";
static const QString speakerImageKey = "speakerImage";
static const QString sessionsKey = "sessions";

// keys used from Server API etc
static const QString speakerIdForeignKey = "speakerId";
static const QString isDeprecatedForeignKey = "isDeprecated";
static const QString sortKeyForeignKey = "sortKey";
static const QString sortGroupForeignKey = "sortGroup";
static const QString nameForeignKey = "name";
static const QString publicNameForeignKey = "publicName";
static const QString bioForeignKey = "bio";
static const QString speakerImageForeignKey = "speakerImage";
static const QString sessionsForeignKey = "sessions";

/*
 * Default Constructor if Speaker not initialized from QVariantMap
 */
Speaker::Speaker(QObject *parent) :
        QObject(parent), mSpeakerId(-1), mIsDeprecated(false), mSortKey(""), mSortGroup(""), mName(""), mPublicName(""), mBio("")
{
	// lazy references:
	mSpeakerImage = -1;
	mSpeakerImageAsDataObject = 0;
	mSpeakerImageInvalid = false;
		// lazy Arrays where only keys are persisted
		mSessionsKeysResolved = false;
}

bool Speaker::isAllResolved()
{
	if (hasSpeakerImage() && !isSpeakerImageResolvedAsDataObject()) {
		return false;
	}
    if(!areSessionsKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize Speaker from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Speaker::fillFromMap(const QVariantMap& speakerMap)
{
	mSpeakerId = speakerMap.value(speakerIdKey).toInt();
	mIsDeprecated = speakerMap.value(isDeprecatedKey).toBool();
	mSortKey = speakerMap.value(sortKeyKey).toString();
	mSortGroup = speakerMap.value(sortGroupKey).toString();
	mName = speakerMap.value(nameKey).toString();
	mPublicName = speakerMap.value(publicNameKey).toString();
	mBio = speakerMap.value(bioKey).toString();
	// speakerImage lazy pointing to SpeakerImage* (domainKey: speakerId)
	if (speakerMap.contains(speakerImageKey)) {
		mSpeakerImage = speakerMap.value(speakerImageKey).toInt();
		if (mSpeakerImage != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mSessions is (lazy loaded) Array of Session*
	mSessionsKeys = speakerMap.value(sessionsKey).toStringList();
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
void Speaker::fillFromForeignMap(const QVariantMap& speakerMap)
{
	mSpeakerId = speakerMap.value(speakerIdForeignKey).toInt();
	mIsDeprecated = speakerMap.value(isDeprecatedForeignKey).toBool();
	mSortKey = speakerMap.value(sortKeyForeignKey).toString();
	mSortGroup = speakerMap.value(sortGroupForeignKey).toString();
	mName = speakerMap.value(nameForeignKey).toString();
	mPublicName = speakerMap.value(publicNameForeignKey).toString();
	mBio = speakerMap.value(bioForeignKey).toString();
	// speakerImage lazy pointing to SpeakerImage* (domainKey: speakerId)
	if (speakerMap.contains(speakerImageForeignKey)) {
		mSpeakerImage = speakerMap.value(speakerImageForeignKey).toInt();
		if (mSpeakerImage != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mSessions is (lazy loaded) Array of Session*
	mSessionsKeys = speakerMap.value(sessionsForeignKey).toStringList();
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
void Speaker::fillFromCacheMap(const QVariantMap& speakerMap)
{
	mSpeakerId = speakerMap.value(speakerIdKey).toInt();
	mIsDeprecated = speakerMap.value(isDeprecatedKey).toBool();
	mSortKey = speakerMap.value(sortKeyKey).toString();
	mSortGroup = speakerMap.value(sortGroupKey).toString();
	mName = speakerMap.value(nameKey).toString();
	mPublicName = speakerMap.value(publicNameKey).toString();
	mBio = speakerMap.value(bioKey).toString();
	// speakerImage lazy pointing to SpeakerImage* (domainKey: speakerId)
	if (speakerMap.contains(speakerImageKey)) {
		mSpeakerImage = speakerMap.value(speakerImageKey).toInt();
		if (mSpeakerImage != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mSessions is (lazy loaded) Array of Session*
	mSessionsKeys = speakerMap.value(sessionsKey).toStringList();
	// mSessions must be resolved later if there are keys
	mSessionsKeysResolved = (mSessionsKeys.size() == 0);
	mSessions.clear();
}

void Speaker::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Speaker::isValid()
{
	if (mSpeakerId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Speaker as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Speaker::toMap()
{
	QVariantMap speakerMap;
	// speakerImage lazy pointing to SpeakerImage* (domainKey: speakerId)
	if (mSpeakerImage != -1) {
		speakerMap.insert(speakerImageKey, mSpeakerImage);
	}
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
	speakerMap.insert(sessionsKey, mSessionsKeys);
	speakerMap.insert(speakerIdKey, mSpeakerId);
	speakerMap.insert(isDeprecatedKey, mIsDeprecated);
	speakerMap.insert(sortKeyKey, mSortKey);
	speakerMap.insert(sortGroupKey, mSortGroup);
	speakerMap.insert(nameKey, mName);
	speakerMap.insert(publicNameKey, mPublicName);
	speakerMap.insert(bioKey, mBio);
	return speakerMap;
}

/*
 * Exports Properties from Speaker as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Speaker::toForeignMap()
{
	QVariantMap speakerMap;
	// speakerImage lazy pointing to SpeakerImage* (domainKey: speakerId)
	if (mSpeakerImage != -1) {
		speakerMap.insert(speakerImageForeignKey, mSpeakerImage);
	}
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
	speakerMap.insert(sessionsKey, mSessionsKeys);
	speakerMap.insert(speakerIdForeignKey, mSpeakerId);
	speakerMap.insert(isDeprecatedForeignKey, mIsDeprecated);
	speakerMap.insert(sortKeyForeignKey, mSortKey);
	speakerMap.insert(sortGroupForeignKey, mSortGroup);
	speakerMap.insert(nameForeignKey, mName);
	speakerMap.insert(publicNameForeignKey, mPublicName);
	speakerMap.insert(bioForeignKey, mBio);
	return speakerMap;
}


/*
 * Exports Properties from Speaker as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Speaker::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// REF
// Lazy: speakerImage
// Optional: speakerImage
// speakerImage lazy pointing to SpeakerImage* (domainKey: speakerId)
int Speaker::speakerImage() const
{
	return mSpeakerImage;
}
SpeakerImage* Speaker::speakerImageAsDataObject() const
{
	return mSpeakerImageAsDataObject;
}
void Speaker::setSpeakerImage(int speakerImage)
{
	if (speakerImage != mSpeakerImage) {
        // remove old Data Object if one was resolved
        if (mSpeakerImageAsDataObject) {
            // reset pointer, don't delete the independent object !
            mSpeakerImageAsDataObject = 0;
        }
        // set the new lazy reference
        mSpeakerImage = speakerImage;
        mSpeakerImageInvalid = false;
        emit speakerImageChanged(speakerImage);
        if (speakerImage != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Speaker::removeSpeakerImage()
{
	if (mSpeakerImage != -1) {
		setSpeakerImage(-1);
	}
}
bool Speaker::hasSpeakerImage()
{
    if (!mSpeakerImageInvalid && mSpeakerImage != -1) {
        return true;
    } else {
        return false;
    }
}
bool Speaker::isSpeakerImageResolvedAsDataObject()
{
    if (!mSpeakerImageInvalid && mSpeakerImageAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite speakerId if different
void Speaker::resolveSpeakerImageAsDataObject(SpeakerImage* speakerImage)
{
    if (speakerImage) {
        if (speakerImage->speakerId() != mSpeakerImage) {
            setSpeakerImage(speakerImage->speakerId());
        }
        mSpeakerImageAsDataObject = speakerImage;
        mSpeakerImageInvalid = false;
    }
}
void Speaker::markSpeakerImageAsInvalid()
{
    mSpeakerImageInvalid = true;
}
// ATT 
// Mandatory: speakerId
// Domain KEY: speakerId
int Speaker::speakerId() const
{
	return mSpeakerId;
}

void Speaker::setSpeakerId(int speakerId)
{
	if (speakerId != mSpeakerId) {
		mSpeakerId = speakerId;
		emit speakerIdChanged(speakerId);
	}
}
// ATT 
// Optional: isDeprecated
bool Speaker::isDeprecated() const
{
	return mIsDeprecated;
}

void Speaker::setIsDeprecated(bool isDeprecated)
{
	if (isDeprecated != mIsDeprecated) {
		mIsDeprecated = isDeprecated;
		emit isDeprecatedChanged(isDeprecated);
	}
}
// ATT 
// Optional: sortKey
QString Speaker::sortKey() const
{
	return mSortKey;
}

void Speaker::setSortKey(QString sortKey)
{
	if (sortKey != mSortKey) {
		mSortKey = sortKey;
		emit sortKeyChanged(sortKey);
	}
}
// ATT 
// Optional: sortGroup
QString Speaker::sortGroup() const
{
	return mSortGroup;
}

void Speaker::setSortGroup(QString sortGroup)
{
	if (sortGroup != mSortGroup) {
		mSortGroup = sortGroup;
		emit sortGroupChanged(sortGroup);
	}
}
// ATT 
// Optional: name
QString Speaker::name() const
{
	return mName;
}

void Speaker::setName(QString name)
{
	if (name != mName) {
		mName = name;
		emit nameChanged(name);
	}
}
// ATT 
// Optional: publicName
QString Speaker::publicName() const
{
	return mPublicName;
}

void Speaker::setPublicName(QString publicName)
{
	if (publicName != mPublicName) {
		mPublicName = publicName;
		emit publicNameChanged(publicName);
	}
}
// ATT 
// Optional: bio
QString Speaker::bio() const
{
	return mBio;
}

void Speaker::setBio(QString bio)
{
	if (bio != mBio) {
		mBio = bio;
		emit bioChanged(bio);
	}
}
// ATT 
// Optional: sessions
QVariantList Speaker::sessionsAsQVariantList()
{
	QVariantList sessionsList;
	for (int i = 0; i < mSessions.size(); ++i) {
        sessionsList.append((mSessions.at(i))->toMap());
    }
	return sessionsList;
}
QVariantList Speaker::sessionsAsForeignQVariantList()
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
void Speaker::addToSessions(Session* session)
{
    mSessions.append(session);
    emit addedToSessions(session);
    emit sessionsPropertyListChanged();
}

bool Speaker::removeFromSessions(Session* session)
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
void Speaker::clearSessions()
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
bool Speaker::areSessionsKeysResolved()
{
    return mSessionsKeysResolved;
}

QStringList Speaker::sessionsKeys()
{
    return mSessionsKeys;
}

/**
 * Objects from sessionsKeys will be added to existing sessions
 * This enables to use addToSessions() without resolving before
 * Hint: it's your responsibility to resolve before looping thru sessions
 */
void Speaker::resolveSessionsKeys(QList<Session*> sessions)
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

int Speaker::sessionsCount()
{
    return mSessions.size();
}
QList<Session*> Speaker::sessions()
{
	return mSessions;
}
void Speaker::setSessions(QList<Session*> sessions) 
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
 * speaker.sessionsPropertyList.length to get the size
 * speaker.sessionsPropertyList[2] to get Session* at position 2
 * speaker.sessionsPropertyList = [] to clear the list
 * or get easy access to properties like
 * speaker.sessionsPropertyList[2].myPropertyName
 */
QQmlListProperty<Session> Speaker::sessionsPropertyList()
{
    return QQmlListProperty<Session>(this, 0, &Speaker::appendToSessionsProperty,
            &Speaker::sessionsPropertyCount, &Speaker::atSessionsProperty,
            &Speaker::clearSessionsProperty);
}
void Speaker::appendToSessionsProperty(QQmlListProperty<Session> *sessionsList,
        Session* session)
{
    Speaker *speakerObject = qobject_cast<Speaker *>(sessionsList->object);
    if (speakerObject) {
        speakerObject->mSessions.append(session);
        emit speakerObject->addedToSessions(session);
    } else {
        qWarning() << "cannot append Session* to sessions " << "Object is not of type Speaker*";
    }
}
int Speaker::sessionsPropertyCount(QQmlListProperty<Session> *sessionsList)
{
    Speaker *speaker = qobject_cast<Speaker *>(sessionsList->object);
    if (speaker) {
        return speaker->mSessions.size();
    } else {
        qWarning() << "cannot get size sessions " << "Object is not of type Speaker*";
    }
    return 0;
}
Session* Speaker::atSessionsProperty(QQmlListProperty<Session> *sessionsList, int pos)
{
    Speaker *speaker = qobject_cast<Speaker *>(sessionsList->object);
    if (speaker) {
        if (speaker->mSessions.size() > pos) {
            return speaker->mSessions.at(pos);
        }
        qWarning() << "cannot get Session* at pos " << pos << " size is "
                << speaker->mSessions.size();
    } else {
        qWarning() << "cannot get Session* at pos " << pos << "Object is not of type Speaker*";
    }
    return 0;
}
void Speaker::clearSessionsProperty(QQmlListProperty<Session> *sessionsList)
{
    Speaker *speaker = qobject_cast<Speaker *>(sessionsList->object);
    if (speaker) {
        // sessions are independent - DON'T delete them
        speaker->mSessions.clear();
    } else {
        qWarning() << "cannot clear sessions " << "Object is not of type Speaker*";
    }
}



Speaker::~Speaker()
{
	// place cleanUp code here
}
	
