#include "Session.hpp"
#include <QDebug>
#include <quuid.h>
// target also references to this
#include "Speaker.hpp"
// target also references to this
#include "Day.hpp"
// target also references to this
#include "Room.hpp"
// target also references to this
#include "SessionTrack.hpp"
// target also references to this
#include "ScheduleItem.hpp"

// keys of QVariantMap used in this APP
static const QString sessionIdKey = "sessionId";
static const QString isDeprecatedKey = "isDeprecated";
static const QString sortKeyKey = "sortKey";
static const QString isTrainingKey = "isTraining";
static const QString isLightningKey = "isLightning";
static const QString isKeynoteKey = "isKeynote";
static const QString isSessionKey = "isSession";
static const QString isCommunityKey = "isCommunity";
static const QString isUnconferenceKey = "isUnconference";
static const QString isMeetingKey = "isMeeting";
static const QString titleKey = "title";
static const QString subtitleKey = "subtitle";
static const QString descriptionKey = "description";
static const QString sessionTypeKey = "sessionType";
static const QString startTimeKey = "startTime";
static const QString endTimeKey = "endTime";
static const QString minutesKey = "minutes";
static const QString abstractTextKey = "abstractText";
static const QString isFavoriteKey = "isFavorite";
static const QString isBookmarkedKey = "isBookmarked";
static const QString presenterKey = "presenter";
static const QString sessionLinksKey = "sessionLinks";
static const QString sessionDayKey = "sessionDay";
static const QString roomKey = "room";
static const QString sessionTrackKey = "sessionTrack";
static const QString scheduleItemKey = "scheduleItem";

// keys used from Server API etc
static const QString sessionIdForeignKey = "sessionId";
static const QString isDeprecatedForeignKey = "isDeprecated";
static const QString sortKeyForeignKey = "sortKey";
static const QString isTrainingForeignKey = "isTraining";
static const QString isLightningForeignKey = "isLightning";
static const QString isKeynoteForeignKey = "isKeynote";
static const QString isSessionForeignKey = "isSession";
static const QString isCommunityForeignKey = "isCommunity";
static const QString isUnconferenceForeignKey = "isUnconference";
static const QString isMeetingForeignKey = "isMeeting";
static const QString titleForeignKey = "title";
static const QString subtitleForeignKey = "subtitle";
static const QString descriptionForeignKey = "description";
static const QString sessionTypeForeignKey = "sessionType";
static const QString startTimeForeignKey = "startTime";
static const QString endTimeForeignKey = "endTime";
static const QString minutesForeignKey = "minutes";
static const QString abstractTextForeignKey = "abstractText";
static const QString isFavoriteForeignKey = "isFavorite";
static const QString isBookmarkedForeignKey = "isBookmarked";
static const QString presenterForeignKey = "presenter";
static const QString sessionLinksForeignKey = "sessionLinks";
static const QString sessionDayForeignKey = "sessionDay";
static const QString roomForeignKey = "room";
static const QString sessionTrackForeignKey = "sessionTrack";
static const QString scheduleItemForeignKey = "scheduleItem";

/*
 * Default Constructor if Session not initialized from QVariantMap
 */
Session::Session(QObject *parent) :
        QObject(parent), mSessionId(-1), mIsDeprecated(false), mSortKey(""), mIsTraining(false), mIsLightning(false), mIsKeynote(false), mIsSession(false), mIsCommunity(false), mIsUnconference(false), mIsMeeting(false), mTitle(""), mSubtitle(""), mDescription(""), mSessionType(""), mMinutes(0), mAbstractText(""), mIsFavorite(false), mIsBookmarked(false)
{
	// lazy references:
	mSessionDay = -1;
	mSessionDayAsDataObject = 0;
	mSessionDayInvalid = false;
	mRoom = -1;
	mRoomAsDataObject = 0;
	mRoomInvalid = false;
	mSessionTrack = -1;
	mSessionTrackAsDataObject = 0;
	mSessionTrackInvalid = false;
	mScheduleItem = -1;
	mScheduleItemAsDataObject = 0;
	mScheduleItemInvalid = false;
	// Date, Time or Timestamp ? construct null value
	mStartTime = QTime();
	mEndTime = QTime();
	// transient values (not cached)
	// bool mIsFavorite
	// bool mIsBookmarked
		// lazy Arrays where only keys are persisted
		mPresenterKeysResolved = false;
		mSessionLinksKeysResolved = false;
}

bool Session::isAllResolved()
{
	if (hasSessionDay() && !isSessionDayResolvedAsDataObject()) {
		return false;
	}
	if (hasRoom() && !isRoomResolvedAsDataObject()) {
		return false;
	}
	if (hasSessionTrack() && !isSessionTrackResolvedAsDataObject()) {
		return false;
	}
	if (hasScheduleItem() && !isScheduleItemResolvedAsDataObject()) {
		return false;
	}
    if(!arePresenterKeysResolved()) {
        return false;
    }
    if(!areSessionLinksKeysResolved()) {
        return false;
    }
    return true;
}

/*
 * initialize Session from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Session::fillFromMap(const QVariantMap& sessionMap)
{
	mSessionId = sessionMap.value(sessionIdKey).toInt();
	mIsDeprecated = sessionMap.value(isDeprecatedKey).toBool();
	mSortKey = sessionMap.value(sortKeyKey).toString();
	mIsTraining = sessionMap.value(isTrainingKey).toBool();
	mIsLightning = sessionMap.value(isLightningKey).toBool();
	mIsKeynote = sessionMap.value(isKeynoteKey).toBool();
	mIsSession = sessionMap.value(isSessionKey).toBool();
	mIsCommunity = sessionMap.value(isCommunityKey).toBool();
	mIsUnconference = sessionMap.value(isUnconferenceKey).toBool();
	mIsMeeting = sessionMap.value(isMeetingKey).toBool();
	mTitle = sessionMap.value(titleKey).toString();
	mSubtitle = sessionMap.value(subtitleKey).toString();
	mDescription = sessionMap.value(descriptionKey).toString();
	mSessionType = sessionMap.value(sessionTypeKey).toString();
	if (sessionMap.contains(startTimeKey)) {
		// always getting the Date as a String (from server or JSON)
		QString startTimeAsString = sessionMap.value(startTimeKey).toString();
		mStartTime = QTime::fromString(startTimeAsString, "HH:mm");
		if (!mStartTime.isValid()) {
			mStartTime = QTime();
			qDebug() << "mStartTime is not valid for String: " << startTimeAsString;
		}
	}
	if (sessionMap.contains(endTimeKey)) {
		// always getting the Date as a String (from server or JSON)
		QString endTimeAsString = sessionMap.value(endTimeKey).toString();
		mEndTime = QTime::fromString(endTimeAsString, "HH:mm");
		if (!mEndTime.isValid()) {
			mEndTime = QTime();
			qDebug() << "mEndTime is not valid for String: " << endTimeAsString;
		}
	}
	mMinutes = sessionMap.value(minutesKey).toInt();
	mAbstractText = sessionMap.value(abstractTextKey).toString();
	// mIsFavorite is transient
	if (sessionMap.contains(isFavoriteKey)) {
		mIsFavorite = sessionMap.value(isFavoriteKey).toBool();
	}
	// mIsBookmarked is transient
	if (sessionMap.contains(isBookmarkedKey)) {
		mIsBookmarked = sessionMap.value(isBookmarkedKey).toBool();
	}
	// sessionDay lazy pointing to Day* (domainKey: id)
	if (sessionMap.contains(sessionDayKey)) {
		mSessionDay = sessionMap.value(sessionDayKey).toInt();
		if (mSessionDay != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// room lazy pointing to Room* (domainKey: roomId)
	if (sessionMap.contains(roomKey)) {
		mRoom = sessionMap.value(roomKey).toInt();
		if (mRoom != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// sessionTrack lazy pointing to SessionTrack* (domainKey: trackId)
	if (sessionMap.contains(sessionTrackKey)) {
		mSessionTrack = sessionMap.value(sessionTrackKey).toInt();
		if (mSessionTrack != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// scheduleItem lazy pointing to ScheduleItem* (domainKey: sessionId)
	if (sessionMap.contains(scheduleItemKey)) {
		mScheduleItem = sessionMap.value(scheduleItemKey).toInt();
		if (mScheduleItem != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mPresenter is (lazy loaded) Array of Speaker*
	mPresenterKeys = sessionMap.value(presenterKey).toStringList();
	// mPresenter must be resolved later if there are keys
	mPresenterKeysResolved = (mPresenterKeys.size() == 0);
	mPresenter.clear();
	// mSessionLinks is (lazy loaded) Array of SessionLink*
	mSessionLinksKeys = sessionMap.value(sessionLinksKey).toStringList();
	// mSessionLinks must be resolved later if there are keys
	mSessionLinksKeysResolved = (mSessionLinksKeys.size() == 0);
	mSessionLinks.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Session::fillFromForeignMap(const QVariantMap& sessionMap)
{
	mSessionId = sessionMap.value(sessionIdForeignKey).toInt();
	mIsDeprecated = sessionMap.value(isDeprecatedForeignKey).toBool();
	mSortKey = sessionMap.value(sortKeyForeignKey).toString();
	mIsTraining = sessionMap.value(isTrainingForeignKey).toBool();
	mIsLightning = sessionMap.value(isLightningForeignKey).toBool();
	mIsKeynote = sessionMap.value(isKeynoteForeignKey).toBool();
	mIsSession = sessionMap.value(isSessionForeignKey).toBool();
	mIsCommunity = sessionMap.value(isCommunityForeignKey).toBool();
	mIsUnconference = sessionMap.value(isUnconferenceForeignKey).toBool();
	mIsMeeting = sessionMap.value(isMeetingForeignKey).toBool();
	mTitle = sessionMap.value(titleForeignKey).toString();
	mSubtitle = sessionMap.value(subtitleForeignKey).toString();
	mDescription = sessionMap.value(descriptionForeignKey).toString();
	mSessionType = sessionMap.value(sessionTypeForeignKey).toString();
	if (sessionMap.contains(startTimeForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString startTimeAsString = sessionMap.value(startTimeForeignKey).toString();
		mStartTime = QTime::fromString(startTimeAsString, "HH:mm");
		if (!mStartTime.isValid()) {
			mStartTime = QTime();
			qDebug() << "mStartTime is not valid for String: " << startTimeAsString;
		}
	}
	if (sessionMap.contains(endTimeForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString endTimeAsString = sessionMap.value(endTimeForeignKey).toString();
		mEndTime = QTime::fromString(endTimeAsString, "HH:mm");
		if (!mEndTime.isValid()) {
			mEndTime = QTime();
			qDebug() << "mEndTime is not valid for String: " << endTimeAsString;
		}
	}
	mMinutes = sessionMap.value(minutesForeignKey).toInt();
	mAbstractText = sessionMap.value(abstractTextForeignKey).toString();
	// mIsFavorite is transient
	if (sessionMap.contains(isFavoriteForeignKey)) {
		mIsFavorite = sessionMap.value(isFavoriteForeignKey).toBool();
	}
	// mIsBookmarked is transient
	if (sessionMap.contains(isBookmarkedForeignKey)) {
		mIsBookmarked = sessionMap.value(isBookmarkedForeignKey).toBool();
	}
	// sessionDay lazy pointing to Day* (domainKey: id)
	if (sessionMap.contains(sessionDayForeignKey)) {
		mSessionDay = sessionMap.value(sessionDayForeignKey).toInt();
		if (mSessionDay != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// room lazy pointing to Room* (domainKey: roomId)
	if (sessionMap.contains(roomForeignKey)) {
		mRoom = sessionMap.value(roomForeignKey).toInt();
		if (mRoom != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// sessionTrack lazy pointing to SessionTrack* (domainKey: trackId)
	if (sessionMap.contains(sessionTrackForeignKey)) {
		mSessionTrack = sessionMap.value(sessionTrackForeignKey).toInt();
		if (mSessionTrack != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// scheduleItem lazy pointing to ScheduleItem* (domainKey: sessionId)
	if (sessionMap.contains(scheduleItemForeignKey)) {
		mScheduleItem = sessionMap.value(scheduleItemForeignKey).toInt();
		if (mScheduleItem != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mPresenter is (lazy loaded) Array of Speaker*
	mPresenterKeys = sessionMap.value(presenterForeignKey).toStringList();
	// mPresenter must be resolved later if there are keys
	mPresenterKeysResolved = (mPresenterKeys.size() == 0);
	mPresenter.clear();
	// mSessionLinks is (lazy loaded) Array of SessionLink*
	mSessionLinksKeys = sessionMap.value(sessionLinksForeignKey).toStringList();
	// mSessionLinks must be resolved later if there are keys
	mSessionLinksKeysResolved = (mSessionLinksKeys.size() == 0);
	mSessionLinks.clear();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Session::fillFromCacheMap(const QVariantMap& sessionMap)
{
	mSessionId = sessionMap.value(sessionIdKey).toInt();
	mIsDeprecated = sessionMap.value(isDeprecatedKey).toBool();
	mSortKey = sessionMap.value(sortKeyKey).toString();
	mIsTraining = sessionMap.value(isTrainingKey).toBool();
	mIsLightning = sessionMap.value(isLightningKey).toBool();
	mIsKeynote = sessionMap.value(isKeynoteKey).toBool();
	mIsSession = sessionMap.value(isSessionKey).toBool();
	mIsCommunity = sessionMap.value(isCommunityKey).toBool();
	mIsUnconference = sessionMap.value(isUnconferenceKey).toBool();
	mIsMeeting = sessionMap.value(isMeetingKey).toBool();
	mTitle = sessionMap.value(titleKey).toString();
	mSubtitle = sessionMap.value(subtitleKey).toString();
	mDescription = sessionMap.value(descriptionKey).toString();
	mSessionType = sessionMap.value(sessionTypeKey).toString();
	if (sessionMap.contains(startTimeKey)) {
		// always getting the Date as a String (from server or JSON)
		QString startTimeAsString = sessionMap.value(startTimeKey).toString();
		mStartTime = QTime::fromString(startTimeAsString, "HH:mm");
		if (!mStartTime.isValid()) {
			mStartTime = QTime();
			qDebug() << "mStartTime is not valid for String: " << startTimeAsString;
		}
	}
	if (sessionMap.contains(endTimeKey)) {
		// always getting the Date as a String (from server or JSON)
		QString endTimeAsString = sessionMap.value(endTimeKey).toString();
		mEndTime = QTime::fromString(endTimeAsString, "HH:mm");
		if (!mEndTime.isValid()) {
			mEndTime = QTime();
			qDebug() << "mEndTime is not valid for String: " << endTimeAsString;
		}
	}
	mMinutes = sessionMap.value(minutesKey).toInt();
	mAbstractText = sessionMap.value(abstractTextKey).toString();
	// mIsFavorite is transient - don't forget to initialize
	// mIsBookmarked is transient - don't forget to initialize
	// sessionDay lazy pointing to Day* (domainKey: id)
	if (sessionMap.contains(sessionDayKey)) {
		mSessionDay = sessionMap.value(sessionDayKey).toInt();
		if (mSessionDay != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// room lazy pointing to Room* (domainKey: roomId)
	if (sessionMap.contains(roomKey)) {
		mRoom = sessionMap.value(roomKey).toInt();
		if (mRoom != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// sessionTrack lazy pointing to SessionTrack* (domainKey: trackId)
	if (sessionMap.contains(sessionTrackKey)) {
		mSessionTrack = sessionMap.value(sessionTrackKey).toInt();
		if (mSessionTrack != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// scheduleItem lazy pointing to ScheduleItem* (domainKey: sessionId)
	if (sessionMap.contains(scheduleItemKey)) {
		mScheduleItem = sessionMap.value(scheduleItemKey).toInt();
		if (mScheduleItem != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mPresenter is (lazy loaded) Array of Speaker*
	mPresenterKeys = sessionMap.value(presenterKey).toStringList();
	// mPresenter must be resolved later if there are keys
	mPresenterKeysResolved = (mPresenterKeys.size() == 0);
	mPresenter.clear();
	// mSessionLinks is (lazy loaded) Array of SessionLink*
	mSessionLinksKeys = sessionMap.value(sessionLinksKey).toStringList();
	// mSessionLinks must be resolved later if there are keys
	mSessionLinksKeysResolved = (mSessionLinksKeys.size() == 0);
	mSessionLinks.clear();
}

void Session::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Session::isValid()
{
	if (mSessionId == -1) {
		return false;
	}
	// sessionDay lazy pointing to Day* (domainKey: id)
	if (mSessionDay == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Session as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Session::toMap()
{
	QVariantMap sessionMap;
	// sessionDay lazy pointing to Day* (domainKey: id)
	if (mSessionDay != -1) {
		sessionMap.insert(sessionDayKey, mSessionDay);
	}
	// room lazy pointing to Room* (domainKey: roomId)
	if (mRoom != -1) {
		sessionMap.insert(roomKey, mRoom);
	}
	// sessionTrack lazy pointing to SessionTrack* (domainKey: trackId)
	if (mSessionTrack != -1) {
		sessionMap.insert(sessionTrackKey, mSessionTrack);
	}
	// scheduleItem lazy pointing to ScheduleItem* (domainKey: sessionId)
	if (mScheduleItem != -1) {
		sessionMap.insert(scheduleItemKey, mScheduleItem);
	}
	// mPresenter points to Speaker*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mPresenterKeysResolved) {
		mPresenterKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mPresenter.size(); ++i) {
		Speaker* speaker;
		speaker = mPresenter.at(i);
		mPresenterKeys << QString::number(speaker->speakerId());
	}
	sessionMap.insert(presenterKey, mPresenterKeys);
	// mSessionLinks points to SessionLink*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mSessionLinksKeysResolved) {
		mSessionLinksKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mSessionLinks.size(); ++i) {
		SessionLink* sessionLink;
		sessionLink = mSessionLinks.at(i);
		mSessionLinksKeys << sessionLink->uuid();
	}
	sessionMap.insert(sessionLinksKey, mSessionLinksKeys);
	sessionMap.insert(sessionIdKey, mSessionId);
	sessionMap.insert(isDeprecatedKey, mIsDeprecated);
	sessionMap.insert(sortKeyKey, mSortKey);
	sessionMap.insert(isTrainingKey, mIsTraining);
	sessionMap.insert(isLightningKey, mIsLightning);
	sessionMap.insert(isKeynoteKey, mIsKeynote);
	sessionMap.insert(isSessionKey, mIsSession);
	sessionMap.insert(isCommunityKey, mIsCommunity);
	sessionMap.insert(isUnconferenceKey, mIsUnconference);
	sessionMap.insert(isMeetingKey, mIsMeeting);
	sessionMap.insert(titleKey, mTitle);
	sessionMap.insert(subtitleKey, mSubtitle);
	sessionMap.insert(descriptionKey, mDescription);
	sessionMap.insert(sessionTypeKey, mSessionType);
	if (hasStartTime()) {
		sessionMap.insert(startTimeKey, mStartTime.toString("HH:mm"));
	}
	if (hasEndTime()) {
		sessionMap.insert(endTimeKey, mEndTime.toString("HH:mm"));
	}
	sessionMap.insert(minutesKey, mMinutes);
	sessionMap.insert(abstractTextKey, mAbstractText);
	sessionMap.insert(isFavoriteKey, mIsFavorite);
	sessionMap.insert(isBookmarkedKey, mIsBookmarked);
	return sessionMap;
}

/*
 * Exports Properties from Session as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Session::toForeignMap()
{
	QVariantMap sessionMap;
	// sessionDay lazy pointing to Day* (domainKey: id)
	if (mSessionDay != -1) {
		sessionMap.insert(sessionDayForeignKey, mSessionDay);
	}
	// room lazy pointing to Room* (domainKey: roomId)
	if (mRoom != -1) {
		sessionMap.insert(roomForeignKey, mRoom);
	}
	// sessionTrack lazy pointing to SessionTrack* (domainKey: trackId)
	if (mSessionTrack != -1) {
		sessionMap.insert(sessionTrackForeignKey, mSessionTrack);
	}
	// scheduleItem lazy pointing to ScheduleItem* (domainKey: sessionId)
	if (mScheduleItem != -1) {
		sessionMap.insert(scheduleItemForeignKey, mScheduleItem);
	}
	// mPresenter points to Speaker*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mPresenterKeysResolved) {
		mPresenterKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mPresenter.size(); ++i) {
		Speaker* speaker;
		speaker = mPresenter.at(i);
		mPresenterKeys << QString::number(speaker->speakerId());
	}
	sessionMap.insert(presenterKey, mPresenterKeys);
	// mSessionLinks points to SessionLink*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mSessionLinksKeysResolved) {
		mSessionLinksKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mSessionLinks.size(); ++i) {
		SessionLink* sessionLink;
		sessionLink = mSessionLinks.at(i);
		mSessionLinksKeys << sessionLink->uuid();
	}
	sessionMap.insert(sessionLinksKey, mSessionLinksKeys);
	sessionMap.insert(sessionIdForeignKey, mSessionId);
	sessionMap.insert(isDeprecatedForeignKey, mIsDeprecated);
	sessionMap.insert(sortKeyForeignKey, mSortKey);
	sessionMap.insert(isTrainingForeignKey, mIsTraining);
	sessionMap.insert(isLightningForeignKey, mIsLightning);
	sessionMap.insert(isKeynoteForeignKey, mIsKeynote);
	sessionMap.insert(isSessionForeignKey, mIsSession);
	sessionMap.insert(isCommunityForeignKey, mIsCommunity);
	sessionMap.insert(isUnconferenceForeignKey, mIsUnconference);
	sessionMap.insert(isMeetingForeignKey, mIsMeeting);
	sessionMap.insert(titleForeignKey, mTitle);
	sessionMap.insert(subtitleForeignKey, mSubtitle);
	sessionMap.insert(descriptionForeignKey, mDescription);
	sessionMap.insert(sessionTypeForeignKey, mSessionType);
	if (hasStartTime()) {
		sessionMap.insert(startTimeForeignKey, mStartTime.toString("HH:mm"));
	}
	if (hasEndTime()) {
		sessionMap.insert(endTimeForeignKey, mEndTime.toString("HH:mm"));
	}
	sessionMap.insert(minutesForeignKey, mMinutes);
	sessionMap.insert(abstractTextForeignKey, mAbstractText);
	return sessionMap;
}


/*
 * Exports Properties from Session as QVariantMap
 * transient properties are excluded:
 * mIsFavorite, mIsBookmarked
 * To export ALL data use toMap()
 */
QVariantMap Session::toCacheMap()
{
	QVariantMap sessionMap;
	// sessionDay lazy pointing to Day* (domainKey: id)
	if (mSessionDay != -1) {
		sessionMap.insert(sessionDayKey, mSessionDay);
	}
	// room lazy pointing to Room* (domainKey: roomId)
	if (mRoom != -1) {
		sessionMap.insert(roomKey, mRoom);
	}
	// sessionTrack lazy pointing to SessionTrack* (domainKey: trackId)
	if (mSessionTrack != -1) {
		sessionMap.insert(sessionTrackKey, mSessionTrack);
	}
	// scheduleItem lazy pointing to ScheduleItem* (domainKey: sessionId)
	if (mScheduleItem != -1) {
		sessionMap.insert(scheduleItemKey, mScheduleItem);
	}
	// mPresenter points to Speaker*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mPresenterKeysResolved) {
		mPresenterKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mPresenter.size(); ++i) {
		Speaker* speaker;
		speaker = mPresenter.at(i);
		mPresenterKeys << QString::number(speaker->speakerId());
	}
	sessionMap.insert(presenterKey, mPresenterKeys);
	// mSessionLinks points to SessionLink*
	// lazy array: persist only keys
	//
	// if keys alreadyy resolved: clear them
	// otherwise reuse the keys and add objects from mPositions
	// this can happen if added to objects without resolving keys before
	if(mSessionLinksKeysResolved) {
		mSessionLinksKeys.clear();
	}
	// add objects from mPositions
	for (int i = 0; i < mSessionLinks.size(); ++i) {
		SessionLink* sessionLink;
		sessionLink = mSessionLinks.at(i);
		mSessionLinksKeys << sessionLink->uuid();
	}
	sessionMap.insert(sessionLinksKey, mSessionLinksKeys);
	sessionMap.insert(sessionIdKey, mSessionId);
	sessionMap.insert(isDeprecatedKey, mIsDeprecated);
	sessionMap.insert(sortKeyKey, mSortKey);
	sessionMap.insert(isTrainingKey, mIsTraining);
	sessionMap.insert(isLightningKey, mIsLightning);
	sessionMap.insert(isKeynoteKey, mIsKeynote);
	sessionMap.insert(isSessionKey, mIsSession);
	sessionMap.insert(isCommunityKey, mIsCommunity);
	sessionMap.insert(isUnconferenceKey, mIsUnconference);
	sessionMap.insert(isMeetingKey, mIsMeeting);
	sessionMap.insert(titleKey, mTitle);
	sessionMap.insert(subtitleKey, mSubtitle);
	sessionMap.insert(descriptionKey, mDescription);
	sessionMap.insert(sessionTypeKey, mSessionType);
	if (hasStartTime()) {
		sessionMap.insert(startTimeKey, mStartTime.toString("HH:mm"));
	}
	if (hasEndTime()) {
		sessionMap.insert(endTimeKey, mEndTime.toString("HH:mm"));
	}
	sessionMap.insert(minutesKey, mMinutes);
	sessionMap.insert(abstractTextKey, mAbstractText);
	// excluded: mIsFavorite
	// excluded: mIsBookmarked
	return sessionMap;
}
// REF
// Lazy: sessionDay
// Mandatory: sessionDay
// sessionDay lazy pointing to Day* (domainKey: id)
int Session::sessionDay() const
{
	return mSessionDay;
}
Day* Session::sessionDayAsDataObject() const
{
	return mSessionDayAsDataObject;
}
void Session::setSessionDay(int sessionDay)
{
	if (sessionDay != mSessionDay) {
        // remove old Data Object if one was resolved
        if (mSessionDayAsDataObject) {
            // reset pointer, don't delete the independent object !
            mSessionDayAsDataObject = 0;
        }
        // set the new lazy reference
        mSessionDay = sessionDay;
        mSessionDayInvalid = false;
        emit sessionDayChanged(sessionDay);
        if (sessionDay != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Session::removeSessionDay()
{
	if (mSessionDay != -1) {
		setSessionDay(-1);
	}
}
bool Session::hasSessionDay()
{
    if (!mSessionDayInvalid && mSessionDay != -1) {
        return true;
    } else {
        return false;
    }
}
bool Session::isSessionDayResolvedAsDataObject()
{
    if (!mSessionDayInvalid && mSessionDayAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite id if different
void Session::resolveSessionDayAsDataObject(Day* day)
{
    if (day) {
        if (day->id() != mSessionDay) {
            setSessionDay(day->id());
        }
        mSessionDayAsDataObject = day;
        mSessionDayInvalid = false;
    }
}
void Session::markSessionDayAsInvalid()
{
    mSessionDayInvalid = true;
}
// REF
// Lazy: room
// Optional: room
// room lazy pointing to Room* (domainKey: roomId)
int Session::room() const
{
	return mRoom;
}
Room* Session::roomAsDataObject() const
{
	return mRoomAsDataObject;
}
void Session::setRoom(int room)
{
	if (room != mRoom) {
        // remove old Data Object if one was resolved
        if (mRoomAsDataObject) {
            // reset pointer, don't delete the independent object !
            mRoomAsDataObject = 0;
        }
        // set the new lazy reference
        mRoom = room;
        mRoomInvalid = false;
        emit roomChanged(room);
        if (room != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Session::removeRoom()
{
	if (mRoom != -1) {
		setRoom(-1);
	}
}
bool Session::hasRoom()
{
    if (!mRoomInvalid && mRoom != -1) {
        return true;
    } else {
        return false;
    }
}
bool Session::isRoomResolvedAsDataObject()
{
    if (!mRoomInvalid && mRoomAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite roomId if different
void Session::resolveRoomAsDataObject(Room* room)
{
    if (room) {
        if (room->roomId() != mRoom) {
            setRoom(room->roomId());
        }
        mRoomAsDataObject = room;
        mRoomInvalid = false;
    }
}
void Session::markRoomAsInvalid()
{
    mRoomInvalid = true;
}
// REF
// Lazy: sessionTrack
// Optional: sessionTrack
// sessionTrack lazy pointing to SessionTrack* (domainKey: trackId)
int Session::sessionTrack() const
{
	return mSessionTrack;
}
SessionTrack* Session::sessionTrackAsDataObject() const
{
	return mSessionTrackAsDataObject;
}
void Session::setSessionTrack(int sessionTrack)
{
	if (sessionTrack != mSessionTrack) {
        // remove old Data Object if one was resolved
        if (mSessionTrackAsDataObject) {
            // reset pointer, don't delete the independent object !
            mSessionTrackAsDataObject = 0;
        }
        // set the new lazy reference
        mSessionTrack = sessionTrack;
        mSessionTrackInvalid = false;
        emit sessionTrackChanged(sessionTrack);
        if (sessionTrack != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Session::removeSessionTrack()
{
	if (mSessionTrack != -1) {
		setSessionTrack(-1);
	}
}
bool Session::hasSessionTrack()
{
    if (!mSessionTrackInvalid && mSessionTrack != -1) {
        return true;
    } else {
        return false;
    }
}
bool Session::isSessionTrackResolvedAsDataObject()
{
    if (!mSessionTrackInvalid && mSessionTrackAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite trackId if different
void Session::resolveSessionTrackAsDataObject(SessionTrack* sessionTrack)
{
    if (sessionTrack) {
        if (sessionTrack->trackId() != mSessionTrack) {
            setSessionTrack(sessionTrack->trackId());
        }
        mSessionTrackAsDataObject = sessionTrack;
        mSessionTrackInvalid = false;
    }
}
void Session::markSessionTrackAsInvalid()
{
    mSessionTrackInvalid = true;
}
// REF
// Lazy: scheduleItem
// Optional: scheduleItem
// scheduleItem lazy pointing to ScheduleItem* (domainKey: sessionId)
int Session::scheduleItem() const
{
	return mScheduleItem;
}
ScheduleItem* Session::scheduleItemAsDataObject() const
{
	return mScheduleItemAsDataObject;
}
void Session::setScheduleItem(int scheduleItem)
{
	if (scheduleItem != mScheduleItem) {
        // remove old Data Object if one was resolved
        if (mScheduleItemAsDataObject) {
            // reset pointer, don't delete the independent object !
            mScheduleItemAsDataObject = 0;
        }
        // set the new lazy reference
        mScheduleItem = scheduleItem;
        mScheduleItemInvalid = false;
        emit scheduleItemChanged(scheduleItem);
        if (scheduleItem != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Session::removeScheduleItem()
{
	if (mScheduleItem != -1) {
		setScheduleItem(-1);
	}
}
bool Session::hasScheduleItem()
{
    if (!mScheduleItemInvalid && mScheduleItem != -1) {
        return true;
    } else {
        return false;
    }
}
bool Session::isScheduleItemResolvedAsDataObject()
{
    if (!mScheduleItemInvalid && mScheduleItemAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite sessionId if different
void Session::resolveScheduleItemAsDataObject(ScheduleItem* scheduleItem)
{
    if (scheduleItem) {
        if (scheduleItem->sessionId() != mScheduleItem) {
            setScheduleItem(scheduleItem->sessionId());
        }
        mScheduleItemAsDataObject = scheduleItem;
        mScheduleItemInvalid = false;
    }
}
void Session::markScheduleItemAsInvalid()
{
    mScheduleItemInvalid = true;
}
// ATT 
// Mandatory: sessionId
// Domain KEY: sessionId
int Session::sessionId() const
{
	return mSessionId;
}

void Session::setSessionId(int sessionId)
{
	if (sessionId != mSessionId) {
		mSessionId = sessionId;
		emit sessionIdChanged(sessionId);
	}
}
// ATT 
// Optional: isDeprecated
bool Session::isDeprecated() const
{
	return mIsDeprecated;
}

void Session::setIsDeprecated(bool isDeprecated)
{
	if (isDeprecated != mIsDeprecated) {
		mIsDeprecated = isDeprecated;
		emit isDeprecatedChanged(isDeprecated);
	}
}
// ATT 
// Optional: sortKey
QString Session::sortKey() const
{
	return mSortKey;
}

void Session::setSortKey(QString sortKey)
{
	if (sortKey != mSortKey) {
		mSortKey = sortKey;
		emit sortKeyChanged(sortKey);
	}
}
// ATT 
// Optional: isTraining
bool Session::isTraining() const
{
	return mIsTraining;
}

void Session::setIsTraining(bool isTraining)
{
	if (isTraining != mIsTraining) {
		mIsTraining = isTraining;
		emit isTrainingChanged(isTraining);
	}
}
// ATT 
// Optional: isLightning
bool Session::isLightning() const
{
	return mIsLightning;
}

void Session::setIsLightning(bool isLightning)
{
	if (isLightning != mIsLightning) {
		mIsLightning = isLightning;
		emit isLightningChanged(isLightning);
	}
}
// ATT 
// Optional: isKeynote
bool Session::isKeynote() const
{
	return mIsKeynote;
}

void Session::setIsKeynote(bool isKeynote)
{
	if (isKeynote != mIsKeynote) {
		mIsKeynote = isKeynote;
		emit isKeynoteChanged(isKeynote);
	}
}
// ATT 
// Optional: isSession
bool Session::isSession() const
{
	return mIsSession;
}

void Session::setIsSession(bool isSession)
{
	if (isSession != mIsSession) {
		mIsSession = isSession;
		emit isSessionChanged(isSession);
	}
}
// ATT 
// Optional: isCommunity
bool Session::isCommunity() const
{
	return mIsCommunity;
}

void Session::setIsCommunity(bool isCommunity)
{
	if (isCommunity != mIsCommunity) {
		mIsCommunity = isCommunity;
		emit isCommunityChanged(isCommunity);
	}
}
// ATT 
// Optional: isUnconference
bool Session::isUnconference() const
{
	return mIsUnconference;
}

void Session::setIsUnconference(bool isUnconference)
{
	if (isUnconference != mIsUnconference) {
		mIsUnconference = isUnconference;
		emit isUnconferenceChanged(isUnconference);
	}
}
// ATT 
// Optional: isMeeting
bool Session::isMeeting() const
{
	return mIsMeeting;
}

void Session::setIsMeeting(bool isMeeting)
{
	if (isMeeting != mIsMeeting) {
		mIsMeeting = isMeeting;
		emit isMeetingChanged(isMeeting);
	}
}
// ATT 
// Optional: title
QString Session::title() const
{
	return mTitle;
}

void Session::setTitle(QString title)
{
	if (title != mTitle) {
		mTitle = title;
		emit titleChanged(title);
	}
}
// ATT 
// Optional: subtitle
QString Session::subtitle() const
{
	return mSubtitle;
}

void Session::setSubtitle(QString subtitle)
{
	if (subtitle != mSubtitle) {
		mSubtitle = subtitle;
		emit subtitleChanged(subtitle);
	}
}
// ATT 
// Optional: description
QString Session::description() const
{
	return mDescription;
}

void Session::setDescription(QString description)
{
	if (description != mDescription) {
		mDescription = description;
		emit descriptionChanged(description);
	}
}
// ATT 
// Optional: sessionType
QString Session::sessionType() const
{
	return mSessionType;
}

void Session::setSessionType(QString sessionType)
{
	if (sessionType != mSessionType) {
		mSessionType = sessionType;
		emit sessionTypeChanged(sessionType);
	}
}
// ATT 
// Optional: startTime
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = myPicker.dateFromTime(startTime)
 */
QTime Session::startTime() const
{
	return mStartTime;
}

void Session::setStartTime(QTime startTime)
{
	if (startTime != mStartTime) {
		mStartTime = startTime;
		emit startTimeChanged(startTime);
	}
}
/**
 * INVOKABLE
 * Convenience method to make it easy to set the value from QML
 * use myPicker.value.toTimeString() as Parameter
 */
void Session::setStartTimeFromPickerValue(QString startTimeValue)
{
    QTime startTime = QTime::fromString(startTimeValue.left(8), "HH:mm:ss");
    if (startTime != mStartTime) {
        mStartTime = startTime;
        emit startTimeChanged(startTime);
    }
}
bool Session::hasStartTime()
{
	return !mStartTime.isNull() && mStartTime.isValid();
}
// ATT 
// Optional: endTime
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = myPicker.dateFromTime(endTime)
 */
QTime Session::endTime() const
{
	return mEndTime;
}

void Session::setEndTime(QTime endTime)
{
	if (endTime != mEndTime) {
		mEndTime = endTime;
		emit endTimeChanged(endTime);
	}
}
/**
 * INVOKABLE
 * Convenience method to make it easy to set the value from QML
 * use myPicker.value.toTimeString() as Parameter
 */
void Session::setEndTimeFromPickerValue(QString endTimeValue)
{
    QTime endTime = QTime::fromString(endTimeValue.left(8), "HH:mm:ss");
    if (endTime != mEndTime) {
        mEndTime = endTime;
        emit endTimeChanged(endTime);
    }
}
bool Session::hasEndTime()
{
	return !mEndTime.isNull() && mEndTime.isValid();
}
// ATT 
// Optional: minutes
int Session::minutes() const
{
	return mMinutes;
}

void Session::setMinutes(int minutes)
{
	if (minutes != mMinutes) {
		mMinutes = minutes;
		emit minutesChanged(minutes);
	}
}
// ATT 
// Optional: abstractText
QString Session::abstractText() const
{
	return mAbstractText;
}

void Session::setAbstractText(QString abstractText)
{
	if (abstractText != mAbstractText) {
		mAbstractText = abstractText;
		emit abstractTextChanged(abstractText);
	}
}
// ATT 
// Optional: isFavorite
bool Session::isFavorite() const
{
	return mIsFavorite;
}

void Session::setIsFavorite(bool isFavorite)
{
	if (isFavorite != mIsFavorite) {
		mIsFavorite = isFavorite;
		emit isFavoriteChanged(isFavorite);
	}
}
// ATT 
// Optional: isBookmarked
bool Session::isBookmarked() const
{
	return mIsBookmarked;
}

void Session::setIsBookmarked(bool isBookmarked)
{
	if (isBookmarked != mIsBookmarked) {
		mIsBookmarked = isBookmarked;
		emit isBookmarkedChanged(isBookmarked);
	}
}
// ATT 
// Optional: presenter
QVariantList Session::presenterAsQVariantList()
{
	QVariantList presenterList;
	for (int i = 0; i < mPresenter.size(); ++i) {
        presenterList.append((mPresenter.at(i))->toMap());
    }
	return presenterList;
}
QVariantList Session::presenterAsForeignQVariantList()
{
	QVariantList presenterList;
	for (int i = 0; i < mPresenter.size(); ++i) {
        presenterList.append((mPresenter.at(i))->toForeignMap());
    }
	return presenterList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add presenter without resolving existing keys before
 * attention: before looping through the objects
 * you must resolvePresenterKeys
 */
void Session::addToPresenter(Speaker* speaker)
{
    mPresenter.append(speaker);
    emit addedToPresenter(speaker);
    emit presenterPropertyListChanged();
}

bool Session::removeFromPresenter(Speaker* speaker)
{
    bool ok = false;
    ok = mPresenter.removeOne(speaker);
    if (!ok) {
    	qDebug() << "Speaker* not found in presenter";
    	return false;
    }
    emit presenterPropertyListChanged();
    // presenter are independent - DON'T delete them
    return true;
}
void Session::clearPresenter()
{
    for (int i = mPresenter.size(); i > 0; --i) {
        removeFromPresenter(mPresenter.last());
    }
    mPresenterKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: presenterKeys()
 * - resolve them from DataManager
 * - then resolvePresenterKeys()
 */
bool Session::arePresenterKeysResolved()
{
    return mPresenterKeysResolved;
}

QStringList Session::presenterKeys()
{
    return mPresenterKeys;
}

/**
 * Objects from presenterKeys will be added to existing presenter
 * This enables to use addToPresenter() without resolving before
 * Hint: it's your responsibility to resolve before looping thru presenter
 */
void Session::resolvePresenterKeys(QList<Speaker*> presenter)
{
    if(mPresenterKeysResolved){
        return;
    }
    // don't clear mPresenter (see above)
    for (int i = 0; i < presenter.size(); ++i) {
        addToPresenter(presenter.at(i));
    }
    mPresenterKeysResolved = true;
}

int Session::presenterCount()
{
    return mPresenter.size();
}
QList<Speaker*> Session::presenter()
{
	return mPresenter;
}
void Session::setPresenter(QList<Speaker*> presenter) 
{
	if (presenter != mPresenter) {
		mPresenter = presenter;
		emit presenterChanged(presenter);
		emit presenterPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * session.presenterPropertyList.length to get the size
 * session.presenterPropertyList[2] to get Speaker* at position 2
 * session.presenterPropertyList = [] to clear the list
 * or get easy access to properties like
 * session.presenterPropertyList[2].myPropertyName
 */
QQmlListProperty<Speaker> Session::presenterPropertyList()
{
    return QQmlListProperty<Speaker>(this, 0, &Session::appendToPresenterProperty,
            &Session::presenterPropertyCount, &Session::atPresenterProperty,
            &Session::clearPresenterProperty);
}
void Session::appendToPresenterProperty(QQmlListProperty<Speaker> *presenterList,
        Speaker* speaker)
{
    Session *sessionObject = qobject_cast<Session *>(presenterList->object);
    if (sessionObject) {
        sessionObject->mPresenter.append(speaker);
        emit sessionObject->addedToPresenter(speaker);
    } else {
        qWarning() << "cannot append Speaker* to presenter " << "Object is not of type Session*";
    }
}
int Session::presenterPropertyCount(QQmlListProperty<Speaker> *presenterList)
{
    Session *session = qobject_cast<Session *>(presenterList->object);
    if (session) {
        return session->mPresenter.size();
    } else {
        qWarning() << "cannot get size presenter " << "Object is not of type Session*";
    }
    return 0;
}
Speaker* Session::atPresenterProperty(QQmlListProperty<Speaker> *presenterList, int pos)
{
    Session *session = qobject_cast<Session *>(presenterList->object);
    if (session) {
        if (session->mPresenter.size() > pos) {
            return session->mPresenter.at(pos);
        }
        qWarning() << "cannot get Speaker* at pos " << pos << " size is "
                << session->mPresenter.size();
    } else {
        qWarning() << "cannot get Speaker* at pos " << pos << "Object is not of type Session*";
    }
    return 0;
}
void Session::clearPresenterProperty(QQmlListProperty<Speaker> *presenterList)
{
    Session *session = qobject_cast<Session *>(presenterList->object);
    if (session) {
        // presenter are independent - DON'T delete them
        session->mPresenter.clear();
    } else {
        qWarning() << "cannot clear presenter " << "Object is not of type Session*";
    }
}

// ATT 
// Optional: sessionLinks
QVariantList Session::sessionLinksAsQVariantList()
{
	QVariantList sessionLinksList;
	for (int i = 0; i < mSessionLinks.size(); ++i) {
        sessionLinksList.append((mSessionLinks.at(i))->toMap());
    }
	return sessionLinksList;
}
QVariantList Session::sessionLinksAsForeignQVariantList()
{
	QVariantList sessionLinksList;
	for (int i = 0; i < mSessionLinks.size(); ++i) {
        sessionLinksList.append((mSessionLinks.at(i))->toForeignMap());
    }
	return sessionLinksList;
}
// no create() or undoCreate() because dto is root object
// see methods in DataManager
/**
 * you can add sessionLinks without resolving existing keys before
 * attention: before looping through the objects
 * you must resolveSessionLinksKeys
 */
void Session::addToSessionLinks(SessionLink* sessionLink)
{
    mSessionLinks.append(sessionLink);
    emit addedToSessionLinks(sessionLink);
    emit sessionLinksPropertyListChanged();
}

bool Session::removeFromSessionLinks(SessionLink* sessionLink)
{
    bool ok = false;
    ok = mSessionLinks.removeOne(sessionLink);
    if (!ok) {
    	qDebug() << "SessionLink* not found in sessionLinks";
    	return false;
    }
    emit sessionLinksPropertyListChanged();
    // sessionLinks are independent - DON'T delete them
    return true;
}
void Session::clearSessionLinks()
{
    for (int i = mSessionLinks.size(); i > 0; --i) {
        removeFromSessionLinks(mSessionLinks.last());
    }
    mSessionLinksKeys.clear();
}

/**
 * lazy Array of independent Data Objects: only keys are persited
 * so we get a list of keys (uuid or domain keys) from map
 * and we persist only the keys toMap()
 * after initializing the keys must be resolved:
 * - get the list of keys: sessionLinksKeys()
 * - resolve them from DataManager
 * - then resolveSessionLinksKeys()
 */
bool Session::areSessionLinksKeysResolved()
{
    return mSessionLinksKeysResolved;
}

QStringList Session::sessionLinksKeys()
{
    return mSessionLinksKeys;
}

/**
 * Objects from sessionLinksKeys will be added to existing sessionLinks
 * This enables to use addToSessionLinks() without resolving before
 * Hint: it's your responsibility to resolve before looping thru sessionLinks
 */
void Session::resolveSessionLinksKeys(QList<SessionLink*> sessionLinks)
{
    if(mSessionLinksKeysResolved){
        return;
    }
    // don't clear mSessionLinks (see above)
    for (int i = 0; i < sessionLinks.size(); ++i) {
        addToSessionLinks(sessionLinks.at(i));
    }
    mSessionLinksKeysResolved = true;
}

int Session::sessionLinksCount()
{
    return mSessionLinks.size();
}
QList<SessionLink*> Session::sessionLinks()
{
	return mSessionLinks;
}
void Session::setSessionLinks(QList<SessionLink*> sessionLinks) 
{
	if (sessionLinks != mSessionLinks) {
		mSessionLinks = sessionLinks;
		emit sessionLinksChanged(sessionLinks);
		emit sessionLinksPropertyListChanged();
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * session.sessionLinksPropertyList.length to get the size
 * session.sessionLinksPropertyList[2] to get SessionLink* at position 2
 * session.sessionLinksPropertyList = [] to clear the list
 * or get easy access to properties like
 * session.sessionLinksPropertyList[2].myPropertyName
 */
QQmlListProperty<SessionLink> Session::sessionLinksPropertyList()
{
    return QQmlListProperty<SessionLink>(this, 0, &Session::appendToSessionLinksProperty,
            &Session::sessionLinksPropertyCount, &Session::atSessionLinksProperty,
            &Session::clearSessionLinksProperty);
}
void Session::appendToSessionLinksProperty(QQmlListProperty<SessionLink> *sessionLinksList,
        SessionLink* sessionLink)
{
    Session *sessionObject = qobject_cast<Session *>(sessionLinksList->object);
    if (sessionObject) {
        sessionObject->mSessionLinks.append(sessionLink);
        emit sessionObject->addedToSessionLinks(sessionLink);
    } else {
        qWarning() << "cannot append SessionLink* to sessionLinks " << "Object is not of type Session*";
    }
}
int Session::sessionLinksPropertyCount(QQmlListProperty<SessionLink> *sessionLinksList)
{
    Session *session = qobject_cast<Session *>(sessionLinksList->object);
    if (session) {
        return session->mSessionLinks.size();
    } else {
        qWarning() << "cannot get size sessionLinks " << "Object is not of type Session*";
    }
    return 0;
}
SessionLink* Session::atSessionLinksProperty(QQmlListProperty<SessionLink> *sessionLinksList, int pos)
{
    Session *session = qobject_cast<Session *>(sessionLinksList->object);
    if (session) {
        if (session->mSessionLinks.size() > pos) {
            return session->mSessionLinks.at(pos);
        }
        qWarning() << "cannot get SessionLink* at pos " << pos << " size is "
                << session->mSessionLinks.size();
    } else {
        qWarning() << "cannot get SessionLink* at pos " << pos << "Object is not of type Session*";
    }
    return 0;
}
void Session::clearSessionLinksProperty(QQmlListProperty<SessionLink> *sessionLinksList)
{
    Session *session = qobject_cast<Session *>(sessionLinksList->object);
    if (session) {
        // sessionLinks are independent - DON'T delete them
        session->mSessionLinks.clear();
    } else {
        qWarning() << "cannot clear sessionLinks " << "Object is not of type Session*";
    }
}



Session::~Session()
{
	// place cleanUp code here
}
	
