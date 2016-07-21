#ifndef SESSIONAPI_HPP_
#define SESSIONAPI_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>
#include <QDate>
#include <QTime>


#include "PersonsAPI.hpp"


class SessionAPI: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
	Q_PROPERTY(int dayIndex READ dayIndex WRITE setDayIndex NOTIFY dayIndexChanged FINAL)
	Q_PROPERTY(QDate conferenceDay READ conferenceDay WRITE setConferenceDay NOTIFY conferenceDayChanged FINAL)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
	Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged FINAL)
	Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged FINAL)
	Q_PROPERTY(QString sessionType READ sessionType WRITE setSessionType NOTIFY sessionTypeChanged FINAL)
	Q_PROPERTY(QTime start READ start WRITE setStart NOTIFY startChanged FINAL)
	Q_PROPERTY(QString duration READ duration WRITE setDuration NOTIFY durationChanged FINAL)
	Q_PROPERTY(QString abstractText READ abstractText WRITE setAbstractText NOTIFY abstractTextChanged FINAL)
	Q_PROPERTY(QString room READ room WRITE setRoom NOTIFY roomChanged FINAL)
	Q_PROPERTY(QString track READ track WRITE setTrack NOTIFY trackChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<PersonsAPI> personsPropertyList READ personsPropertyList NOTIFY personsPropertyListChanged)

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

	int id() const;
	void setId(int id);
	int dayIndex() const;
	void setDayIndex(int dayIndex);
	QDate conferenceDay() const;

	Q_INVOKABLE
	bool hasConferenceDay();
	void setConferenceDay(QDate conferenceDay);
	QString title() const;
	void setTitle(QString title);
	QString subtitle() const;
	void setSubtitle(QString subtitle);
	QString description() const;
	void setDescription(QString description);
	QString sessionType() const;
	void setSessionType(QString sessionType);
	QTime start() const;

	Q_INVOKABLE
	bool hasStart();

	Q_INVOKABLE
	void setStartFromPickerValue(QString startValue);
	void setStart(QTime start);
	QString duration() const;
	void setDuration(QString duration);
	QString abstractText() const;
	void setAbstractText(QString abstractText);
	QString room() const;
	void setRoom(QString room);
	QString track() const;
	void setTrack(QString track);

	
	Q_INVOKABLE
	QVariantList personsAsQVariantList();
	
	Q_INVOKABLE
	QVariantList personsAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToPersons(PersonsAPI* personsAPI);
	
	Q_INVOKABLE
	bool removeFromPersons(PersonsAPI* personsAPI);

	Q_INVOKABLE
	void clearPersons();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool arePersonsKeysResolved();

	Q_INVOKABLE
	QStringList personsKeys();

	Q_INVOKABLE
	void resolvePersonsKeys(QList<PersonsAPI*> persons);
	
	Q_INVOKABLE
	int personsCount();
	
	 // access from C++ to persons
	QList<PersonsAPI*> persons();
	void setPersons(QList<PersonsAPI*> persons);
	// access from QML to persons
	QQmlListProperty<PersonsAPI> personsPropertyList();


	virtual ~SessionAPI();

	Q_SIGNALS:

	void idChanged(int id);
	void dayIndexChanged(int dayIndex);
	void conferenceDayChanged(QDate conferenceDay);
	void titleChanged(QString title);
	void subtitleChanged(QString subtitle);
	void descriptionChanged(QString description);
	void sessionTypeChanged(QString sessionType);
	void startChanged(QTime start);
	void durationChanged(QString duration);
	void abstractTextChanged(QString abstractText);
	void roomChanged(QString room);
	void trackChanged(QString track);
	void personsChanged(QList<PersonsAPI*> persons);
	void addedToPersons(PersonsAPI* personsAPI);
	void personsPropertyListChanged();
	
	

private:

	int mId;
	int mDayIndex;
	QDate mConferenceDay;
	QString mTitle;
	QString mSubtitle;
	QString mDescription;
	QString mSessionType;
	QTime mStart;
	QString mDuration;
	QString mAbstractText;
	QString mRoom;
	QString mTrack;
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mPersonsKeys;
	bool mPersonsKeysResolved;
	QList<PersonsAPI*> mPersons;
	// implementation for QQmlListProperty to use
	// QML functions for List of PersonsAPI*
	static void appendToPersonsProperty(QQmlListProperty<PersonsAPI> *personsList,
		PersonsAPI* personsAPI);
	static int personsPropertyCount(QQmlListProperty<PersonsAPI> *personsList);
	static PersonsAPI* atPersonsProperty(QQmlListProperty<PersonsAPI> *personsList, int pos);
	static void clearPersonsProperty(QQmlListProperty<PersonsAPI> *personsList);
	

	Q_DISABLE_COPY (SessionAPI)
};
Q_DECLARE_METATYPE(SessionAPI*)

#endif /* SESSIONAPI_HPP_ */

