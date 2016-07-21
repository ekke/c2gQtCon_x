#ifndef DAY_HPP_
#define DAY_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>
#include <QDate>


// forward declaration (target references to this)
class Session;


class Day: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
	Q_PROPERTY(int weekDay READ weekDay WRITE setWeekDay NOTIFY weekDayChanged FINAL)
	Q_PROPERTY(QDate conferenceDay READ conferenceDay WRITE setConferenceDay NOTIFY conferenceDayChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Session> sessionsPropertyList READ sessionsPropertyList NOTIFY sessionsPropertyListChanged)

public:
	Day(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& dayMap);
	void fillFromForeignMap(const QVariantMap& dayMap);
	void fillFromCacheMap(const QVariantMap& dayMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int id() const;
	void setId(int id);
	int weekDay() const;
	void setWeekDay(int weekDay);
	QDate conferenceDay() const;

	Q_INVOKABLE
	bool hasConferenceDay();
	void setConferenceDay(QDate conferenceDay);

	
	Q_INVOKABLE
	QVariantList sessionsAsQVariantList();
	
	Q_INVOKABLE
	QVariantList sessionsAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToSessions(Session* session);
	
	Q_INVOKABLE
	bool removeFromSessions(Session* session);

	Q_INVOKABLE
	void clearSessions();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areSessionsKeysResolved();

	Q_INVOKABLE
	QStringList sessionsKeys();

	Q_INVOKABLE
	void resolveSessionsKeys(QList<Session*> sessions);
	
	Q_INVOKABLE
	int sessionsCount();
	
	 // access from C++ to sessions
	QList<Session*> sessions();
	void setSessions(QList<Session*> sessions);
	// access from QML to sessions
	QQmlListProperty<Session> sessionsPropertyList();


	virtual ~Day();

	Q_SIGNALS:

	void idChanged(int id);
	void weekDayChanged(int weekDay);
	void conferenceDayChanged(QDate conferenceDay);
	void sessionsChanged(QList<Session*> sessions);
	void addedToSessions(Session* session);
	void sessionsPropertyListChanged();
	
	

private:

	int mId;
	int mWeekDay;
	QDate mConferenceDay;
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mSessionsKeys;
	bool mSessionsKeysResolved;
	QList<Session*> mSessions;
	// implementation for QQmlListProperty to use
	// QML functions for List of Session*
	static void appendToSessionsProperty(QQmlListProperty<Session> *sessionsList,
		Session* session);
	static int sessionsPropertyCount(QQmlListProperty<Session> *sessionsList);
	static Session* atSessionsProperty(QQmlListProperty<Session> *sessionsList, int pos);
	static void clearSessionsProperty(QQmlListProperty<Session> *sessionsList);
	

	Q_DISABLE_COPY (Day)
};
Q_DECLARE_METATYPE(Day*)

#endif /* DAY_HPP_ */

