#ifndef CONFERENCE_HPP_
#define CONFERENCE_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>
#include <QDate>


#include "Day.hpp"


class Conference: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
	Q_PROPERTY(QString conferenceName READ conferenceName WRITE setConferenceName NOTIFY conferenceNameChanged FINAL)
	Q_PROPERTY(QString conferenceCity READ conferenceCity WRITE setConferenceCity NOTIFY conferenceCityChanged FINAL)
	Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged FINAL)
	Q_PROPERTY(QString timeZoneName READ timeZoneName WRITE setTimeZoneName NOTIFY timeZoneNameChanged FINAL)
	Q_PROPERTY(int timeZoneOffsetSeconds READ timeZoneOffsetSeconds WRITE setTimeZoneOffsetSeconds NOTIFY timeZoneOffsetSecondsChanged FINAL)
	Q_PROPERTY(QDate conferenceFrom READ conferenceFrom WRITE setConferenceFrom NOTIFY conferenceFromChanged FINAL)
	Q_PROPERTY(QDate conferenceTo READ conferenceTo WRITE setConferenceTo NOTIFY conferenceToChanged FINAL)
	Q_PROPERTY(QString hashTag READ hashTag WRITE setHashTag NOTIFY hashTagChanged FINAL)
	Q_PROPERTY(QString homePage READ homePage WRITE setHomePage NOTIFY homePageChanged FINAL)
	Q_PROPERTY(QString coordinate READ coordinate WRITE setCoordinate NOTIFY coordinateChanged FINAL)
	Q_PROPERTY(int lastBuildingId READ lastBuildingId WRITE setLastBuildingId NOTIFY lastBuildingIdChanged FINAL)
	Q_PROPERTY(int lastFloorId READ lastFloorId WRITE setLastFloorId NOTIFY lastFloorIdChanged FINAL)
	Q_PROPERTY(int lastRoomId READ lastRoomId WRITE setLastRoomId NOTIFY lastRoomIdChanged FINAL)
	Q_PROPERTY(int lastSessionTrackId READ lastSessionTrackId WRITE setLastSessionTrackId NOTIFY lastSessionTrackIdChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Day> daysPropertyList READ daysPropertyList NOTIFY daysPropertyListChanged)

public:
	Conference(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& conferenceMap);
	void fillFromForeignMap(const QVariantMap& conferenceMap);
	void fillFromCacheMap(const QVariantMap& conferenceMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int id() const;
	void setId(int id);
	QString conferenceName() const;
	void setConferenceName(QString conferenceName);
	QString conferenceCity() const;
	void setConferenceCity(QString conferenceCity);
	QString address() const;
	void setAddress(QString address);
	QString timeZoneName() const;
	void setTimeZoneName(QString timeZoneName);
	int timeZoneOffsetSeconds() const;
	void setTimeZoneOffsetSeconds(int timeZoneOffsetSeconds);
	QDate conferenceFrom() const;

	Q_INVOKABLE
	bool hasConferenceFrom();
	void setConferenceFrom(QDate conferenceFrom);
	QDate conferenceTo() const;

	Q_INVOKABLE
	bool hasConferenceTo();
	void setConferenceTo(QDate conferenceTo);
	QString hashTag() const;
	void setHashTag(QString hashTag);
	QString homePage() const;
	void setHomePage(QString homePage);
	QString coordinate() const;
	void setCoordinate(QString coordinate);
	int lastBuildingId() const;
	void setLastBuildingId(int lastBuildingId);
	int lastFloorId() const;
	void setLastFloorId(int lastFloorId);
	int lastRoomId() const;
	void setLastRoomId(int lastRoomId);
	int lastSessionTrackId() const;
	void setLastSessionTrackId(int lastSessionTrackId);

	
	Q_INVOKABLE
	QVariantList daysAsQVariantList();
	
	Q_INVOKABLE
	QVariantList daysAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToDays(Day* day);
	
	Q_INVOKABLE
	bool removeFromDays(Day* day);

	Q_INVOKABLE
	void clearDays();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areDaysKeysResolved();

	Q_INVOKABLE
	QStringList daysKeys();

	Q_INVOKABLE
	void resolveDaysKeys(QList<Day*> days);
	
	Q_INVOKABLE
	int daysCount();
	
	 // access from C++ to days
	QList<Day*> days();
	void setDays(QList<Day*> days);
	// access from QML to days
	QQmlListProperty<Day> daysPropertyList();


	virtual ~Conference();

	Q_SIGNALS:

	void idChanged(int id);
	void conferenceNameChanged(QString conferenceName);
	void conferenceCityChanged(QString conferenceCity);
	void addressChanged(QString address);
	void timeZoneNameChanged(QString timeZoneName);
	void timeZoneOffsetSecondsChanged(int timeZoneOffsetSeconds);
	void conferenceFromChanged(QDate conferenceFrom);
	void conferenceToChanged(QDate conferenceTo);
	void hashTagChanged(QString hashTag);
	void homePageChanged(QString homePage);
	void coordinateChanged(QString coordinate);
	void lastBuildingIdChanged(int lastBuildingId);
	void lastFloorIdChanged(int lastFloorId);
	void lastRoomIdChanged(int lastRoomId);
	void lastSessionTrackIdChanged(int lastSessionTrackId);
	void daysChanged(QList<Day*> days);
	void addedToDays(Day* day);
	void daysPropertyListChanged();
	
	

private:

	int mId;
	QString mConferenceName;
	QString mConferenceCity;
	QString mAddress;
	QString mTimeZoneName;
	int mTimeZoneOffsetSeconds;
	QDate mConferenceFrom;
	QDate mConferenceTo;
	QString mHashTag;
	QString mHomePage;
	QString mCoordinate;
	int mLastBuildingId;
	int mLastFloorId;
	int mLastRoomId;
	int mLastSessionTrackId;
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mDaysKeys;
	bool mDaysKeysResolved;
	QList<Day*> mDays;
	// implementation for QQmlListProperty to use
	// QML functions for List of Day*
	static void appendToDaysProperty(QQmlListProperty<Day> *daysList,
		Day* day);
	static int daysPropertyCount(QQmlListProperty<Day> *daysList);
	static Day* atDaysProperty(QQmlListProperty<Day> *daysList, int pos);
	static void clearDaysProperty(QQmlListProperty<Day> *daysList);
	

	Q_DISABLE_COPY (Conference)
};
Q_DECLARE_METATYPE(Conference*)

#endif /* CONFERENCE_HPP_ */

