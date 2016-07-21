#include "SessionAPI.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString idKey = "id";
static const QString dayIndexKey = "dayIndex";
static const QString conferenceDayKey = "conferenceDay";
static const QString titleKey = "title";
static const QString subtitleKey = "subtitle";
static const QString descriptionKey = "description";
static const QString sessionTypeKey = "sessionType";
static const QString startKey = "start";
static const QString durationKey = "duration";
static const QString abstractTextKey = "abstractText";
static const QString roomKey = "room";
static const QString trackKey = "track";
static const QString personsKey = "persons";

// keys used from Server API etc
static const QString idForeignKey = "id";
static const QString dayIndexForeignKey = "dayIndex";
static const QString conferenceDayForeignKey = "conferenceDay";
static const QString titleForeignKey = "title";
static const QString subtitleForeignKey = "subtitle";
static const QString descriptionForeignKey = "description";
static const QString sessionTypeForeignKey = "sessionType";
static const QString startForeignKey = "start";
static const QString durationForeignKey = "duration";
static const QString abstractTextForeignKey = "abstract";
static const QString roomForeignKey = "room";
static const QString trackForeignKey = "track";
static const QString personsForeignKey = "persons";

/*
 * Default Constructor if SessionAPI not initialized from QVariantMap
 */
SessionAPI::SessionAPI(QObject *parent) :
        QObject(parent), mId(-1), mDayIndex(0), mTitle(""), mSubtitle(""), mDescription(""), mSessionType(""), mDuration(""), mAbstractText(""), mRoom(""), mTrack("")
{
	// Date, Time or Timestamp ? construct null value
	mConferenceDay = QDate();
	mStart = QTime();
		// lazy Arrays where only keys are persisted
		mPersonsKeysResolved = false;
}

bool SessionAPI::isAllResolved()
{
    if(!arePersonsKeysResolved()) {
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
	mId = sessionAPIMap.value(idKey).toInt();
	mDayIndex = sessionAPIMap.value(dayIndexKey).toInt();
	if (sessionAPIMap.contains(conferenceDayKey)) {
		// always getting the Date as a String (from server or JSON)
		QString conferenceDayAsString = sessionAPIMap.value(conferenceDayKey).toString();
		mConferenceDay = QDate::fromString(conferenceDayAsString, "yyyy-MM-dd");
		if (!mConferenceDay.isValid()) {
			mConferenceDay = QDate();
			qDebug() << "mConferenceDay is not valid for String: " << conferenceDayAsString;
		}
	}
	mTitle = sessionAPIMap.value(titleKey).toString();
	mSubtitle = sessionAPIMap.value(subtitleKey).toString();
	mDescription = sessionAPIMap.value(descriptionKey).toString();
	mSessionType = sessionAPIMap.value(sessionTypeKey).toString();
	if (sessionAPIMap.contains(startKey)) {
		// always getting the Date as a String (from server or JSON)
		QString startAsString = sessionAPIMap.value(startKey).toString();
		mStart = QTime::fromString(startAsString, "HH:mm");
		if (!mStart.isValid()) {
			mStart = QTime();
			qDebug() << "mStart is not valid for String: " << startAsString;
		}
	}
	mDuration = sessionAPIMap.value(durationKey).toString();
	mAbstractText = sessionAPIMap.value(abstractTextKey).toString();
	mRoom = sessionAPIMap.value(roomKey).toString();
	mTrack = sessionAPIMap.value(trackKey).toString();
	// mPersons is (lazy loaded) Array of PersonsAPI*
	mPersonsKeys = sessionAPIMap.value(personsKey).toStringList();
	// mPersons must be resolved later if there are keys
	mPersonsKeysResolved = (mPersonsKeys.size() == 0);
	mPersons.clear();
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
	mId = sessionAPIMap.value(idForeignKey).toInt();
	mDayIndex = sessionAPIMap.value(dayIndexForeignKey).toInt();
	if (sessionAPIMap.contains(conferenceDayForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString conferenceDayAsString = sessionAPIMap.value(conferenceDayForeignKey).toString();
		mConferenceDay = QDate::fromString(conferenceDayAsString, "yyyy-MM-dd");
		if (!mConferenceDay.isValid()) {
			mConferenceDay = QDate();
			qDebug() << "mConferenceDay is not valid for String: " << conferenceDayAsString;
		}
	}
	mTitle = sessionAPIMap.value(titleForeignKey).toString();
	mSubtitle = sessionAPIMap.value(subtitleForeignKey).toString();
	mDescription = sessionAPIMap.value(descriptionForeignKey).toString();
	mSessionType = sessionAPIMap.value(sessionTypeForeignKey).toString();
	if (sessionAPIMap.contains(startForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString startAsString = sessionAPIMap.value(startForeignKey).toString();
		mStart = QTime::fromString(startAsString, "HH:mm");
		if (!mStart.isValid()) {
			mStart = QTime();
			qDebug() << "mStart is not valid for String: " << startAsString;
		}
	}
	mDuration = sessionAPIMap.value(durationForeignKey).toString();
	mAbstractText = sessionAPIMap.value(abstractTextForeignKey).toString();
	mRoom = sessionAPIMap.value(roomForeignKey).toString();
	mTrack = sessionAPIMap.value(trackForeignKey).toString();
	// mPersons is (lazy loaded) Array of PersonsAPI*
	mPersonsKeys = sessionAPIMap.value(personsForeignKey).toStringList();
	// mPersons must be resolved later if there are keys
	mPersonsKeysResolved = (mPersonsKeys.size() == 0);
	mPersons.clear();
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
	mId = sessionAPIMap.value(idKey).toInt();
	mDayIndex = sessionAPIMap.value(dayIndexKey).toInt();
	if (sessionAPIMap.contains(conferenceDayKey)) {
		// always getting the Date as a String (from server or JSON)
		QString conferenceDayAsString = sessionAPIMap.value(conferenceDayKey).toString();
		mConferenceDay = QDate::fromString(conferenceDayAsString, "yyyy-MM-dd");
		if (!mConferenceDay.isValid()) {
			mConferenceDay = QDate();
			qDebug() << "mConferenceDay is not valid for String: " << conferenceDayAsString;
		}
	}
	mTitle = sessionAPIMap.value(titleKey).toString();
	mSubtitle = sessionAPIMap.value(subtitleKey).toString();
	mDescription = sessionAPIMap.value(descriptionKey).toString();
	mSessionType = sessionAPIMap.value(sessionTypeKey).toString();
	if (sessionAPIMap.contains(startKey)) {
		// always getting the Date as a String (from server or JSON)
		QString startAsString = sessionAPIMap.value(startKey).toString();
		mStart = QTime::fromString(startAsString, "HH:mm");
		if (!mStart.isValid()) {
			mStart = QTime();
			qDebug() << "mStart is not valid for String: " << startAsString;
		}
	}
	mDuration = sessionAPIMap.value(durationKey).toString();
	mAbstractText = sessionAPIMap.value(abstractTextKey).toString();
	mRoom = sessionAPIMap.value(roomKey).toString();
	mTrack = sessionAPIMap.value(trackKey).toString();
	// mPersons is (lazy loaded) Array of PersonsAPI*
	mPersonsKeys = sessionAPIMap.value(personsKey).toStringList();
	// mPersons must be resolved later if there are keys
	mPersonsKeysResolved = (mPersonsKeys.size() == 0);
	mPersons.clear();
}

void SessionAPI::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool SessionAPI::isValid()
{
	if (mId == -1) {
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
	// mPersons points to PersonsAPI*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mPersonsKeysResolved) {
		mPersonsKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mPersons.size(); ++i) {
		PersonsAPI* personsAPI;
		personsAPI = mPersons.at(i);
		mPersonsKeys << QString::number(personsAPI->id());
	}
	sessionAPIMap.insert(personsKey, mPersonsKeys);
	sessionAPIMap.insert(idKey, mId);
	sessionAPIMap.insert(dayIndexKey, mDayIndex);
	if (hasConferenceDay()) {
		sessionAPIMap.insert(conferenceDayKey, mConferenceDay.toString("yyyy-MM-dd"));
	}
	sessionAPIMap.insert(titleKey, mTitle);
	sessionAPIMap.insert(subtitleKey, mSubtitle);
	sessionAPIMap.insert(descriptionKey, mDescription);
	sessionAPIMap.insert(sessionTypeKey, mSessionType);
	if (hasStart()) {
		sessionAPIMap.insert(startKey, mStart.toString("HH:mm"));
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
	// mPersons points to PersonsAPI*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mPersonsKeysResolved) {
		mPersonsKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mPersons.size(); ++i) {
		PersonsAPI* personsAPI;
		personsAPI = mPersons.at(i);
		mPersonsKeys << QString::number(personsAPI->id());
	}
	sessionAPIMap.insert(personsKey, mPersonsKeys);
	sessionAPIMap.insert(idForeignKey, mId);
	sessionAPIMap.insert(dayIndexForeignKey, mDayIndex);
	if (hasConferenceDay()) {
		sessionAPIMap.insert(conferenceDayForeignKey, mConferenceDay.toString("yyyy-MM-dd"));
	}
	sessionAPIMap.insert(titleForeignKey, mTitle);
	sessionAPIMap.insert(subtitleForeignKey, mSubtitle);
	sessionAPIMap.insert(descriptionForeignKey, mDescription);
	sessionAPIMap.insert(sessionTypeForeignKey, mSessionType);
	if (hasStart()) {
		sessionAPIMap.insert(startForeignKey, mStart.toString("HH:mm"));
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
// Mandatory: id
// Domain KEY: id
int SessionAPI::id() const
{
	return mId;
}

void SessionAPI::setId(int id)
{
	if (id != mId) {
		mId = id;
		emit idChanged(id);
	}
}
// ATT 
// Optional: dayIndex
int SessionAPI::dayIndex() const
{
	return mDayIndex;
}

void SessionAPI::setDayIndex(int dayIndex)
{
	if (dayIndex != mDayIndex) {
		mDayIndex = dayIndex;
		emit dayIndexChanged(dayIndex);
	}
}
// ATT 
// Optional: conferenceDay
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = new Date(conferenceDay)
 */
QDate SessionAPI::conferenceDay() const
{
	return mConferenceDay;
}

/**
 * from QML DateTime Picker use as parameter:
 * conferenceDay = new Date(myPicker.value)
 */
void SessionAPI::setConferenceDay(QDate conferenceDay)
{
	if (conferenceDay != mConferenceDay) {
		mConferenceDay = conferenceDay;
		emit conferenceDayChanged(conferenceDay);
	}
}
bool SessionAPI::hasConferenceDay()
{
	return !mConferenceDay.isNull() && mConferenceDay.isValid();
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
// Optional: start
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = myPicker.dateFromTime(start)
 */
QTime SessionAPI::start() const
{
	return mStart;
}

void SessionAPI::setStart(QTime start)
{
	if (start != mStart) {
		mStart = start;
		emit startChanged(start);
	}
}
/**
 * INVOKABLE
 * Convenience method to make it easy to set the value from QML
 * use myPicker.value.toTimeString() as Parameter
 */
void SessionAPI::setStartFromPickerValue(QString startValue)
{
    QTime start = QTime::fromString(startValue.left(8), "HH:mm:ss");
    if (start != mStart) {
        mStart = start;
        emit startChanged(start);
    }
}
bool SessionAPI::hasStart()
{
	return !mStart.isNull() && mStart.isValid();
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
// Optional: persons
QVariantList SessionAPI::personsAsQVariantList()
{
	QVariantList personsList;
	for (int i = 0; i < mPersons.size(); ++i) {
        personsList.append((mPersons.at(i))->toMap());
    }
	return personsList;
}
QVariantList SessionAPI::personsAsForeignQVariantList()
{
	QVariantList personsList;
	for (int i = 0; i < mPersons.size(); ++i) {
        personsList.append((mPersons.at(i))->toForeignMap());
    }
	return personsList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add persons without resolving existing keys before
 * attention: before looping through the objects
 * you must resolvePersonsKeys
 */
void SessionAPI::addToPersons(PersonsAPI* personsAPI)
{
    mPersons.append(personsAPI);
    emit addedToPersons(personsAPI);
    emit personsPropertyListChanged();
}

bool SessionAPI::removeFromPersons(PersonsAPI* personsAPI)
{
    bool ok = false;
    ok = mPersons.removeOne(personsAPI);
    if (!ok) {
    	qDebug() << "PersonsAPI* not found in persons";
    	return false;
    }
    emit personsPropertyListChanged();
    // persons are independent - DON'T delete them
    return true;
}
void SessionAPI::clearPersons()
{
    for (int i = mPersons.size(); i > 0; --i) {
        removeFromPersons(mPersons.last());
    }
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: personsKeys()
 * - resolve them from DataManager
 * - then resolvePersonsKeys()
 */
bool SessionAPI::arePersonsKeysResolved()
{
    return mPersonsKeysResolved;
}

QStringList SessionAPI::personsKeys()
{
    return mPersonsKeys;
}

/**
 * Objects from personsKeys will be added to existing persons
 * This enables to use addToPersons() without resolving before
 * Hint: it's your responsibility to resolve before looping thru persons
 */
void SessionAPI::resolvePersonsKeys(QList<PersonsAPI*> persons)
{
    if(mPersonsKeysResolved){
        return;
    }
    // don't clear mPersons (see above)
    for (int i = 0; i < persons.size(); ++i) {
        addToPersons(persons.at(i));
    }
    mPersonsKeysResolved = true;
}

int SessionAPI::personsCount()
{
    return mPersons.size();
}
QList<PersonsAPI*> SessionAPI::persons()
{
	return mPersons;
}
void SessionAPI::setPersons(QList<PersonsAPI*> persons) 
{
	if (persons != mPersons) {
		mPersons = persons;
		emit personsChanged(persons);
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * sessionAPI.personsPropertyList.length to get the size
 * sessionAPI.personsPropertyList[2] to get PersonsAPI* at position 2
 * sessionAPI.personsPropertyList = [] to clear the list
 * or get easy access to properties like
 * sessionAPI.personsPropertyList[2].myPropertyName
 */
QQmlListProperty<PersonsAPI> SessionAPI::personsPropertyList()
{
    return QQmlListProperty<PersonsAPI>(this, 0, &SessionAPI::appendToPersonsProperty,
            &SessionAPI::personsPropertyCount, &SessionAPI::atPersonsProperty,
            &SessionAPI::clearPersonsProperty);
}
void SessionAPI::appendToPersonsProperty(QQmlListProperty<PersonsAPI> *personsList,
        PersonsAPI* personsAPI)
{
    SessionAPI *sessionAPIObject = qobject_cast<SessionAPI *>(personsList->object);
    if (sessionAPIObject) {
        sessionAPIObject->mPersons.append(personsAPI);
        emit sessionAPIObject->addedToPersons(personsAPI);
    } else {
        qWarning() << "cannot append PersonsAPI* to persons " << "Object is not of type SessionAPI*";
    }
}
int SessionAPI::personsPropertyCount(QQmlListProperty<PersonsAPI> *personsList)
{
    SessionAPI *sessionAPI = qobject_cast<SessionAPI *>(personsList->object);
    if (sessionAPI) {
        return sessionAPI->mPersons.size();
    } else {
        qWarning() << "cannot get size persons " << "Object is not of type SessionAPI*";
    }
    return 0;
}
PersonsAPI* SessionAPI::atPersonsProperty(QQmlListProperty<PersonsAPI> *personsList, int pos)
{
    SessionAPI *sessionAPI = qobject_cast<SessionAPI *>(personsList->object);
    if (sessionAPI) {
        if (sessionAPI->mPersons.size() > pos) {
            return sessionAPI->mPersons.at(pos);
        }
        qWarning() << "cannot get PersonsAPI* at pos " << pos << " size is "
                << sessionAPI->mPersons.size();
    } else {
        qWarning() << "cannot get PersonsAPI* at pos " << pos << "Object is not of type SessionAPI*";
    }
    return 0;
}
void SessionAPI::clearPersonsProperty(QQmlListProperty<PersonsAPI> *personsList)
{
    SessionAPI *sessionAPI = qobject_cast<SessionAPI *>(personsList->object);
    if (sessionAPI) {
        // persons are independent - DON'T delete them
        sessionAPI->mPersons.clear();
    } else {
        qWarning() << "cannot clear persons " << "Object is not of type SessionAPI*";
    }
}



SessionAPI::~SessionAPI()
{
	// place cleanUp code here
}
	
