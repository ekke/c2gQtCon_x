#ifndef SESSIONTRACK_HPP_
#define SESSIONTRACK_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>


// forward declaration (target references to this)
class Session;


class SessionTrack: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int trackId READ trackId WRITE setTrackId NOTIFY trackIdChanged FINAL)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
	Q_PROPERTY(bool inAssets READ inAssets WRITE setInAssets NOTIFY inAssetsChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Session> sessionsPropertyList READ sessionsPropertyList NOTIFY sessionsPropertyListChanged)

public:
	SessionTrack(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& sessionTrackMap);
	void fillFromForeignMap(const QVariantMap& sessionTrackMap);
	void fillFromCacheMap(const QVariantMap& sessionTrackMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int trackId() const;
	void setTrackId(int trackId);
	QString name() const;
	void setName(QString name);
	bool inAssets() const;
	void setInAssets(bool inAssets);

	
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


	virtual ~SessionTrack();

	Q_SIGNALS:

	void trackIdChanged(int trackId);
	void nameChanged(QString name);
	void inAssetsChanged(bool inAssets);
	void sessionsChanged(QList<Session*> sessions);
	void addedToSessions(Session* session);
	void sessionsPropertyListChanged();
	
	

private:

	int mTrackId;
	QString mName;
	bool mInAssets;
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
	

	Q_DISABLE_COPY (SessionTrack)
};
Q_DECLARE_METATYPE(SessionTrack*)

#endif /* SESSIONTRACK_HPP_ */

