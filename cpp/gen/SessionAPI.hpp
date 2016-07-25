#ifndef SESSIONAPI_HPP_
#define SESSIONAPI_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>
#include <QTime>


#include "PersonsAPI.hpp"
#include "SessionLinkAPI.hpp"


class SessionAPI: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int sessionId READ sessionId WRITE setSessionId NOTIFY sessionIdChanged FINAL)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
	Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged FINAL)
	Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged FINAL)
	Q_PROPERTY(QString sessionType READ sessionType WRITE setSessionType NOTIFY sessionTypeChanged FINAL)
	Q_PROPERTY(QTime startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged FINAL)
	Q_PROPERTY(QString duration READ duration WRITE setDuration NOTIFY durationChanged FINAL)
	Q_PROPERTY(QString abstractText READ abstractText WRITE setAbstractText NOTIFY abstractTextChanged FINAL)
	Q_PROPERTY(QString room READ room WRITE setRoom NOTIFY roomChanged FINAL)
	Q_PROPERTY(QString track READ track WRITE setTrack NOTIFY trackChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<PersonsAPI> presenterPropertyList READ presenterPropertyList NOTIFY presenterPropertyListChanged)
	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<SessionLinkAPI> sessionLinksPropertyList READ sessionLinksPropertyList NOTIFY sessionLinksPropertyListChanged)

public:
	SessionAPI(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& sessionAPIMap);
	void fillFromForeignMap(const QVariantMap& sessionAPIMap);
	void fillFromCacheMap(const QVariantMap& sessionAPIMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int sessionId() const;
	void setSessionId(int sessionId);
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
	QString duration() const;
	void setDuration(QString duration);
	QString abstractText() const;
	void setAbstractText(QString abstractText);
	QString room() const;
	void setRoom(QString room);
	QString track() const;
	void setTrack(QString track);

	
	Q_INVOKABLE
	QVariantList presenterAsQVariantList();
	
	Q_INVOKABLE
	QVariantList presenterAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToPresenter(PersonsAPI* personsAPI);
	
	Q_INVOKABLE
	bool removeFromPresenter(PersonsAPI* personsAPI);

	Q_INVOKABLE
	void clearPresenter();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool arePresenterKeysResolved();

	Q_INVOKABLE
	QStringList presenterKeys();

	Q_INVOKABLE
	void resolvePresenterKeys(QList<PersonsAPI*> presenter);
	
	Q_INVOKABLE
	int presenterCount();
	
	 // access from C++ to presenter
	QList<PersonsAPI*> presenter();
	void setPresenter(QList<PersonsAPI*> presenter);
	// access from QML to presenter
	QQmlListProperty<PersonsAPI> presenterPropertyList();
	
	Q_INVOKABLE
	QVariantList sessionLinksAsQVariantList();
	
	Q_INVOKABLE
	QVariantList sessionLinksAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToSessionLinks(SessionLinkAPI* sessionLinkAPI);
	
	Q_INVOKABLE
	bool removeFromSessionLinks(SessionLinkAPI* sessionLinkAPI);

	Q_INVOKABLE
	void clearSessionLinks();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areSessionLinksKeysResolved();

	Q_INVOKABLE
	QStringList sessionLinksKeys();

	Q_INVOKABLE
	void resolveSessionLinksKeys(QList<SessionLinkAPI*> sessionLinks);
	
	Q_INVOKABLE
	int sessionLinksCount();
	
	 // access from C++ to sessionLinks
	QList<SessionLinkAPI*> sessionLinks();
	void setSessionLinks(QList<SessionLinkAPI*> sessionLinks);
	// access from QML to sessionLinks
	QQmlListProperty<SessionLinkAPI> sessionLinksPropertyList();


	virtual ~SessionAPI();

	Q_SIGNALS:

	void sessionIdChanged(int sessionId);
	void titleChanged(QString title);
	void subtitleChanged(QString subtitle);
	void descriptionChanged(QString description);
	void sessionTypeChanged(QString sessionType);
	void startTimeChanged(QTime startTime);
	void durationChanged(QString duration);
	void abstractTextChanged(QString abstractText);
	void roomChanged(QString room);
	void trackChanged(QString track);
	void presenterChanged(QList<PersonsAPI*> presenter);
	void addedToPresenter(PersonsAPI* personsAPI);
	void presenterPropertyListChanged();
	
	void sessionLinksChanged(QList<SessionLinkAPI*> sessionLinks);
	void addedToSessionLinks(SessionLinkAPI* sessionLinkAPI);
	void sessionLinksPropertyListChanged();
	
	

private:

	int mSessionId;
	QString mTitle;
	QString mSubtitle;
	QString mDescription;
	QString mSessionType;
	QTime mStartTime;
	QString mDuration;
	QString mAbstractText;
	QString mRoom;
	QString mTrack;
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mPresenterKeys;
	bool mPresenterKeysResolved;
	QList<PersonsAPI*> mPresenter;
	// implementation for QQmlListProperty to use
	// QML functions for List of PersonsAPI*
	static void appendToPresenterProperty(QQmlListProperty<PersonsAPI> *presenterList,
		PersonsAPI* personsAPI);
	static int presenterPropertyCount(QQmlListProperty<PersonsAPI> *presenterList);
	static PersonsAPI* atPresenterProperty(QQmlListProperty<PersonsAPI> *presenterList, int pos);
	static void clearPresenterProperty(QQmlListProperty<PersonsAPI> *presenterList);
	
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mSessionLinksKeys;
	bool mSessionLinksKeysResolved;
	QList<SessionLinkAPI*> mSessionLinks;
	// implementation for QQmlListProperty to use
	// QML functions for List of SessionLinkAPI*
	static void appendToSessionLinksProperty(QQmlListProperty<SessionLinkAPI> *sessionLinksList,
		SessionLinkAPI* sessionLinkAPI);
	static int sessionLinksPropertyCount(QQmlListProperty<SessionLinkAPI> *sessionLinksList);
	static SessionLinkAPI* atSessionLinksProperty(QQmlListProperty<SessionLinkAPI> *sessionLinksList, int pos);
	static void clearSessionLinksProperty(QQmlListProperty<SessionLinkAPI> *sessionLinksList);
	

	Q_DISABLE_COPY (SessionAPI)
};
Q_DECLARE_METATYPE(SessionAPI*)

#endif /* SESSIONAPI_HPP_ */

