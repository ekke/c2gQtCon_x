#ifndef SESSIONLISTS_HPP_
#define SESSIONLISTS_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>


#include "Session.hpp"
#include "Session.hpp"
#include "Session.hpp"


class SessionLists: public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Session> scheduledSessionsPropertyList READ scheduledSessionsPropertyList NOTIFY scheduledSessionsPropertyListChanged)
	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Session> sameTimeSessionsPropertyList READ sameTimeSessionsPropertyList NOTIFY sameTimeSessionsPropertyListChanged)
	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Session> specialTimeSessionsPropertyList READ specialTimeSessionsPropertyList NOTIFY specialTimeSessionsPropertyListChanged)

public:
	SessionLists(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& sessionListsMap);
	void fillFromForeignMap(const QVariantMap& sessionListsMap);
	void fillFromCacheMap(const QVariantMap& sessionListsMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	QString uuid() const;
	void setUuid(QString uuid);

	
	Q_INVOKABLE
	QVariantList scheduledSessionsAsQVariantList();
	
	Q_INVOKABLE
	QVariantList scheduledSessionsAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToScheduledSessions(Session* session);
	
	Q_INVOKABLE
	bool removeFromScheduledSessions(Session* session);

	Q_INVOKABLE
	void clearScheduledSessions();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areScheduledSessionsKeysResolved();

	Q_INVOKABLE
	QStringList scheduledSessionsKeys();

	Q_INVOKABLE
	void resolveScheduledSessionsKeys(QList<Session*> scheduledSessions);
	
	Q_INVOKABLE
	int scheduledSessionsCount();
	
	 // access from C++ to scheduledSessions
	QList<Session*> scheduledSessions();
	void setScheduledSessions(QList<Session*> scheduledSessions);
	// access from QML to scheduledSessions
	QQmlListProperty<Session> scheduledSessionsPropertyList();
	
	Q_INVOKABLE
	QVariantList sameTimeSessionsAsQVariantList();
	
	Q_INVOKABLE
	QVariantList sameTimeSessionsAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToSameTimeSessions(Session* session);
	
	Q_INVOKABLE
	bool removeFromSameTimeSessions(Session* session);

	Q_INVOKABLE
	void clearSameTimeSessions();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areSameTimeSessionsKeysResolved();

	Q_INVOKABLE
	QStringList sameTimeSessionsKeys();

	Q_INVOKABLE
	void resolveSameTimeSessionsKeys(QList<Session*> sameTimeSessions);
	
	Q_INVOKABLE
	int sameTimeSessionsCount();
	
	 // access from C++ to sameTimeSessions
	QList<Session*> sameTimeSessions();
	void setSameTimeSessions(QList<Session*> sameTimeSessions);
	// access from QML to sameTimeSessions
	QQmlListProperty<Session> sameTimeSessionsPropertyList();
	
	Q_INVOKABLE
	QVariantList specialTimeSessionsAsQVariantList();
	
	Q_INVOKABLE
	QVariantList specialTimeSessionsAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToSpecialTimeSessions(Session* session);
	
	Q_INVOKABLE
	bool removeFromSpecialTimeSessions(Session* session);

	Q_INVOKABLE
	void clearSpecialTimeSessions();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areSpecialTimeSessionsKeysResolved();

	Q_INVOKABLE
	QStringList specialTimeSessionsKeys();

	Q_INVOKABLE
	void resolveSpecialTimeSessionsKeys(QList<Session*> specialTimeSessions);
	
	Q_INVOKABLE
	int specialTimeSessionsCount();
	
	 // access from C++ to specialTimeSessions
	QList<Session*> specialTimeSessions();
	void setSpecialTimeSessions(QList<Session*> specialTimeSessions);
	// access from QML to specialTimeSessions
	QQmlListProperty<Session> specialTimeSessionsPropertyList();


	virtual ~SessionLists();

	Q_SIGNALS:

	void uuidChanged(QString uuid);
	void scheduledSessionsChanged(QList<Session*> scheduledSessions);
	void addedToScheduledSessions(Session* session);
	void scheduledSessionsPropertyListChanged();
	
	void sameTimeSessionsChanged(QList<Session*> sameTimeSessions);
	void addedToSameTimeSessions(Session* session);
	void sameTimeSessionsPropertyListChanged();
	
	void specialTimeSessionsChanged(QList<Session*> specialTimeSessions);
	void addedToSpecialTimeSessions(Session* session);
	void specialTimeSessionsPropertyListChanged();
	
	

private:

	QString mUuid;
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mScheduledSessionsKeys;
	bool mScheduledSessionsKeysResolved;
	QList<Session*> mScheduledSessions;
	// implementation for QQmlListProperty to use
	// QML functions for List of Session*
	static void appendToScheduledSessionsProperty(QQmlListProperty<Session> *scheduledSessionsList,
		Session* session);
	static int scheduledSessionsPropertyCount(QQmlListProperty<Session> *scheduledSessionsList);
	static Session* atScheduledSessionsProperty(QQmlListProperty<Session> *scheduledSessionsList, int pos);
	static void clearScheduledSessionsProperty(QQmlListProperty<Session> *scheduledSessionsList);
	
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mSameTimeSessionsKeys;
	bool mSameTimeSessionsKeysResolved;
	QList<Session*> mSameTimeSessions;
	// implementation for QQmlListProperty to use
	// QML functions for List of Session*
	static void appendToSameTimeSessionsProperty(QQmlListProperty<Session> *sameTimeSessionsList,
		Session* session);
	static int sameTimeSessionsPropertyCount(QQmlListProperty<Session> *sameTimeSessionsList);
	static Session* atSameTimeSessionsProperty(QQmlListProperty<Session> *sameTimeSessionsList, int pos);
	static void clearSameTimeSessionsProperty(QQmlListProperty<Session> *sameTimeSessionsList);
	
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mSpecialTimeSessionsKeys;
	bool mSpecialTimeSessionsKeysResolved;
	QList<Session*> mSpecialTimeSessions;
	// implementation for QQmlListProperty to use
	// QML functions for List of Session*
	static void appendToSpecialTimeSessionsProperty(QQmlListProperty<Session> *specialTimeSessionsList,
		Session* session);
	static int specialTimeSessionsPropertyCount(QQmlListProperty<Session> *specialTimeSessionsList);
	static Session* atSpecialTimeSessionsProperty(QQmlListProperty<Session> *specialTimeSessionsList, int pos);
	static void clearSpecialTimeSessionsProperty(QQmlListProperty<Session> *specialTimeSessionsList);
	

	Q_DISABLE_COPY (SessionLists)
};
Q_DECLARE_METATYPE(SessionLists*)

#endif /* SESSIONLISTS_HPP_ */

