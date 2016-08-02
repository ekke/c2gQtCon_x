#ifndef SESSION_HPP_
#define SESSION_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>
#include <QTime>


// forward declaration (target references to this)
class Speaker;
#include "SessionLink.hpp"
// forward declaration (target references to this)
class Day;
// forward declaration (target references to this)
class Room;
// forward declaration (target references to this)
class SessionTrack;
// forward declaration (target references to this)
class ScheduleItem;


class Session: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int sessionId READ sessionId WRITE setSessionId NOTIFY sessionIdChanged FINAL)
	Q_PROPERTY(bool isDeprecated READ isDeprecated WRITE setIsDeprecated NOTIFY isDeprecatedChanged FINAL)
	Q_PROPERTY(QString sortKey READ sortKey WRITE setSortKey NOTIFY sortKeyChanged FINAL)
	Q_PROPERTY(bool isTraining READ isTraining WRITE setIsTraining NOTIFY isTrainingChanged FINAL)
	Q_PROPERTY(bool isLightning READ isLightning WRITE setIsLightning NOTIFY isLightningChanged FINAL)
	Q_PROPERTY(bool isKeynote READ isKeynote WRITE setIsKeynote NOTIFY isKeynoteChanged FINAL)
	Q_PROPERTY(bool isSession READ isSession WRITE setIsSession NOTIFY isSessionChanged FINAL)
	Q_PROPERTY(bool isCommunity READ isCommunity WRITE setIsCommunity NOTIFY isCommunityChanged FINAL)
	Q_PROPERTY(bool isUnconference READ isUnconference WRITE setIsUnconference NOTIFY isUnconferenceChanged FINAL)
	Q_PROPERTY(bool isMeeting READ isMeeting WRITE setIsMeeting NOTIFY isMeetingChanged FINAL)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
	Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged FINAL)
	Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged FINAL)
	Q_PROPERTY(QString sessionType READ sessionType WRITE setSessionType NOTIFY sessionTypeChanged FINAL)
	Q_PROPERTY(QTime startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged FINAL)
	Q_PROPERTY(QTime endTime READ endTime WRITE setEndTime NOTIFY endTimeChanged FINAL)
	Q_PROPERTY(int minutes READ minutes WRITE setMinutes NOTIFY minutesChanged FINAL)
	Q_PROPERTY(QString abstractText READ abstractText WRITE setAbstractText NOTIFY abstractTextChanged FINAL)
	Q_PROPERTY(bool isFavorite READ isFavorite WRITE setIsFavorite NOTIFY isFavoriteChanged FINAL)
	Q_PROPERTY(bool isBookmarked READ isBookmarked WRITE setIsBookmarked NOTIFY isBookmarkedChanged FINAL)
	// sessionDay lazy pointing to Day* (domainKey: id)
	Q_PROPERTY(int sessionDay READ sessionDay WRITE setSessionDay NOTIFY sessionDayChanged FINAL)
	Q_PROPERTY(Day* sessionDayAsDataObject READ sessionDayAsDataObject WRITE resolveSessionDayAsDataObject NOTIFY sessionDayAsDataObjectChanged FINAL)
	// room lazy pointing to Room* (domainKey: roomId)
	Q_PROPERTY(int room READ room WRITE setRoom NOTIFY roomChanged FINAL)
	Q_PROPERTY(Room* roomAsDataObject READ roomAsDataObject WRITE resolveRoomAsDataObject NOTIFY roomAsDataObjectChanged FINAL)
	// sessionTrack lazy pointing to SessionTrack* (domainKey: trackId)
	Q_PROPERTY(int sessionTrack READ sessionTrack WRITE setSessionTrack NOTIFY sessionTrackChanged FINAL)
	Q_PROPERTY(SessionTrack* sessionTrackAsDataObject READ sessionTrackAsDataObject WRITE resolveSessionTrackAsDataObject NOTIFY sessionTrackAsDataObjectChanged FINAL)
	// scheduleItem lazy pointing to ScheduleItem* (domainKey: sessionId)
	Q_PROPERTY(int scheduleItem READ scheduleItem WRITE setScheduleItem NOTIFY scheduleItemChanged FINAL)
	Q_PROPERTY(ScheduleItem* scheduleItemAsDataObject READ scheduleItemAsDataObject WRITE resolveScheduleItemAsDataObject NOTIFY scheduleItemAsDataObjectChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Speaker> presenterPropertyList READ presenterPropertyList NOTIFY presenterPropertyListChanged)
	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<SessionLink> sessionLinksPropertyList READ sessionLinksPropertyList NOTIFY sessionLinksPropertyListChanged)

public:
	Session(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& sessionMap);
	void fillFromForeignMap(const QVariantMap& sessionMap);
	void fillFromCacheMap(const QVariantMap& sessionMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int sessionId() const;
	void setSessionId(int sessionId);
	bool isDeprecated() const;
	void setIsDeprecated(bool isDeprecated);
	QString sortKey() const;
	void setSortKey(QString sortKey);
	bool isTraining() const;
	void setIsTraining(bool isTraining);
	bool isLightning() const;
	void setIsLightning(bool isLightning);
	bool isKeynote() const;
	void setIsKeynote(bool isKeynote);
	bool isSession() const;
	void setIsSession(bool isSession);
	bool isCommunity() const;
	void setIsCommunity(bool isCommunity);
	bool isUnconference() const;
	void setIsUnconference(bool isUnconference);
	bool isMeeting() const;
	void setIsMeeting(bool isMeeting);
	QString title() const;
	void setTitle(QString title);
	QString subtitle() const;
	void setSubtitle(QString subtitle);
	QString description() const;
	void setDescription(QString description);
	QString sessionType() const;
	void setSessionType(QString sessionType);
	QTime startTime() const;

	Q_INVOKABLE
	bool hasStartTime();

	Q_INVOKABLE
	void setStartTimeFromPickerValue(QString startTimeValue);
	void setStartTime(QTime startTime);
	QTime endTime() const;

	Q_INVOKABLE
	bool hasEndTime();

	Q_INVOKABLE
	void setEndTimeFromPickerValue(QString endTimeValue);
	void setEndTime(QTime endTime);
	int minutes() const;
	void setMinutes(int minutes);
	QString abstractText() const;
	void setAbstractText(QString abstractText);
	bool isFavorite() const;
	void setIsFavorite(bool isFavorite);
	bool isBookmarked() const;
	void setIsBookmarked(bool isBookmarked);
	// sessionDay lazy pointing to Day* (domainKey: id)
	int sessionDay() const;
	void setSessionDay(int sessionDay);
	Day* sessionDayAsDataObject() const;
	
	Q_INVOKABLE
	void resolveSessionDayAsDataObject(Day* day);
	
	Q_INVOKABLE
	void removeSessionDay();
	
	Q_INVOKABLE
	bool hasSessionDay();
	
	Q_INVOKABLE
	bool isSessionDayResolvedAsDataObject();
	
	Q_INVOKABLE
	void markSessionDayAsInvalid();
	
	// room lazy pointing to Room* (domainKey: roomId)
	int room() const;
	void setRoom(int room);
	Room* roomAsDataObject() const;
	
	Q_INVOKABLE
	void resolveRoomAsDataObject(Room* room);
	
	Q_INVOKABLE
	void removeRoom();
	
	Q_INVOKABLE
	bool hasRoom();
	
	Q_INVOKABLE
	bool isRoomResolvedAsDataObject();
	
	Q_INVOKABLE
	void markRoomAsInvalid();
	
	// sessionTrack lazy pointing to SessionTrack* (domainKey: trackId)
	int sessionTrack() const;
	void setSessionTrack(int sessionTrack);
	SessionTrack* sessionTrackAsDataObject() const;
	
	Q_INVOKABLE
	void resolveSessionTrackAsDataObject(SessionTrack* sessionTrack);
	
	Q_INVOKABLE
	void removeSessionTrack();
	
	Q_INVOKABLE
	bool hasSessionTrack();
	
	Q_INVOKABLE
	bool isSessionTrackResolvedAsDataObject();
	
	Q_INVOKABLE
	void markSessionTrackAsInvalid();
	
	// scheduleItem lazy pointing to ScheduleItem* (domainKey: sessionId)
	int scheduleItem() const;
	void setScheduleItem(int scheduleItem);
	ScheduleItem* scheduleItemAsDataObject() const;
	
	Q_INVOKABLE
	void resolveScheduleItemAsDataObject(ScheduleItem* scheduleItem);
	
	Q_INVOKABLE
	void removeScheduleItem();
	
	Q_INVOKABLE
	bool hasScheduleItem();
	
	Q_INVOKABLE
	bool isScheduleItemResolvedAsDataObject();
	
	Q_INVOKABLE
	void markScheduleItemAsInvalid();
	

	
	Q_INVOKABLE
	QVariantList presenterAsQVariantList();
	
	Q_INVOKABLE
	QVariantList presenterAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToPresenter(Speaker* speaker);
	
	Q_INVOKABLE
	bool removeFromPresenter(Speaker* speaker);

	Q_INVOKABLE
	void clearPresenter();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool arePresenterKeysResolved();

	Q_INVOKABLE
	QStringList presenterKeys();

	Q_INVOKABLE
	void resolvePresenterKeys(QList<Speaker*> presenter);
	
	Q_INVOKABLE
	int presenterCount();
	
	 // access from C++ to presenter
	QList<Speaker*> presenter();
	void setPresenter(QList<Speaker*> presenter);
	// access from QML to presenter
	QQmlListProperty<Speaker> presenterPropertyList();
	
	Q_INVOKABLE
	QVariantList sessionLinksAsQVariantList();
	
	Q_INVOKABLE
	QVariantList sessionLinksAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToSessionLinks(SessionLink* sessionLink);
	
	Q_INVOKABLE
	bool removeFromSessionLinks(SessionLink* sessionLink);

	Q_INVOKABLE
	void clearSessionLinks();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areSessionLinksKeysResolved();

	Q_INVOKABLE
	QStringList sessionLinksKeys();

	Q_INVOKABLE
	void resolveSessionLinksKeys(QList<SessionLink*> sessionLinks);
	
	Q_INVOKABLE
	int sessionLinksCount();
	
	 // access from C++ to sessionLinks
	QList<SessionLink*> sessionLinks();
	void setSessionLinks(QList<SessionLink*> sessionLinks);
	// access from QML to sessionLinks
	QQmlListProperty<SessionLink> sessionLinksPropertyList();


	virtual ~Session();

	Q_SIGNALS:

	void sessionIdChanged(int sessionId);
	void isDeprecatedChanged(bool isDeprecated);
	void sortKeyChanged(QString sortKey);
	void isTrainingChanged(bool isTraining);
	void isLightningChanged(bool isLightning);
	void isKeynoteChanged(bool isKeynote);
	void isSessionChanged(bool isSession);
	void isCommunityChanged(bool isCommunity);
	void isUnconferenceChanged(bool isUnconference);
	void isMeetingChanged(bool isMeeting);
	void titleChanged(QString title);
	void subtitleChanged(QString subtitle);
	void descriptionChanged(QString description);
	void sessionTypeChanged(QString sessionType);
	void startTimeChanged(QTime startTime);
	void endTimeChanged(QTime endTime);
	void minutesChanged(int minutes);
	void abstractTextChanged(QString abstractText);
	void isFavoriteChanged(bool isFavorite);
	void isBookmarkedChanged(bool isBookmarked);
	// sessionDay lazy pointing to Day* (domainKey: id)
	void sessionDayChanged(int sessionDay);
	void sessionDayAsDataObjectChanged(Day* day);
	// room lazy pointing to Room* (domainKey: roomId)
	void roomChanged(int room);
	void roomAsDataObjectChanged(Room* room);
	// sessionTrack lazy pointing to SessionTrack* (domainKey: trackId)
	void sessionTrackChanged(int sessionTrack);
	void sessionTrackAsDataObjectChanged(SessionTrack* sessionTrack);
	// scheduleItem lazy pointing to ScheduleItem* (domainKey: sessionId)
	void scheduleItemChanged(int scheduleItem);
	void scheduleItemAsDataObjectChanged(ScheduleItem* scheduleItem);
	void presenterChanged(QList<Speaker*> presenter);
	void addedToPresenter(Speaker* speaker);
	void presenterPropertyListChanged();
	
	void sessionLinksChanged(QList<SessionLink*> sessionLinks);
	void addedToSessionLinks(SessionLink* sessionLink);
	void sessionLinksPropertyListChanged();
	
	

private:

	int mSessionId;
	bool mIsDeprecated;
	QString mSortKey;
	bool mIsTraining;
	bool mIsLightning;
	bool mIsKeynote;
	bool mIsSession;
	bool mIsCommunity;
	bool mIsUnconference;
	bool mIsMeeting;
	QString mTitle;
	QString mSubtitle;
	QString mDescription;
	QString mSessionType;
	QTime mStartTime;
	QTime mEndTime;
	int mMinutes;
	QString mAbstractText;
	bool mIsFavorite;
	bool mIsBookmarked;
	int mSessionDay;
	bool mSessionDayInvalid;
	Day* mSessionDayAsDataObject;
	int mRoom;
	bool mRoomInvalid;
	Room* mRoomAsDataObject;
	int mSessionTrack;
	bool mSessionTrackInvalid;
	SessionTrack* mSessionTrackAsDataObject;
	int mScheduleItem;
	bool mScheduleItemInvalid;
	ScheduleItem* mScheduleItemAsDataObject;
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mPresenterKeys;
	bool mPresenterKeysResolved;
	QList<Speaker*> mPresenter;
	// implementation for QQmlListProperty to use
	// QML functions for List of Speaker*
	static void appendToPresenterProperty(QQmlListProperty<Speaker> *presenterList,
		Speaker* speaker);
	static int presenterPropertyCount(QQmlListProperty<Speaker> *presenterList);
	static Speaker* atPresenterProperty(QQmlListProperty<Speaker> *presenterList, int pos);
	static void clearPresenterProperty(QQmlListProperty<Speaker> *presenterList);
	
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mSessionLinksKeys;
	bool mSessionLinksKeysResolved;
	QList<SessionLink*> mSessionLinks;
	// implementation for QQmlListProperty to use
	// QML functions for List of SessionLink*
	static void appendToSessionLinksProperty(QQmlListProperty<SessionLink> *sessionLinksList,
		SessionLink* sessionLink);
	static int sessionLinksPropertyCount(QQmlListProperty<SessionLink> *sessionLinksList);
	static SessionLink* atSessionLinksProperty(QQmlListProperty<SessionLink> *sessionLinksList, int pos);
	static void clearSessionLinksProperty(QQmlListProperty<SessionLink> *sessionLinksList);
	

	Q_DISABLE_COPY (Session)
};
Q_DECLARE_METATYPE(Session*)

#endif /* SESSION_HPP_ */

