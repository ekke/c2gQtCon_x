#ifndef DATAMANAGER_HPP_
#define DATAMANAGER_HPP_

#include <qobject.h>
#include <QQmlListProperty>
#include <QStringList>

#include "SettingsData.hpp"
#include "Conference.hpp"
#include "Building.hpp"
#include "Floor.hpp"
#include "Room.hpp"
#include "Session.hpp"
#include "ScheduleItem.hpp"
#include "Favorite.hpp"
#include "Bookmark.hpp"
#include "SessionLists.hpp"
#include "SessionLink.hpp"
#include "Speaker.hpp"
#include "SpeakerImage.hpp"
#include "SessionTrack.hpp"
#include "Day.hpp"
#include "SessionAPI.hpp"
#include "PersonsAPI.hpp"
#include "SessionLinkAPI.hpp"
#include "SpeakerAPI.hpp"

class DataManager: public QObject
{
Q_OBJECT

// QQmlListProperty to get easy access from QML
Q_PROPERTY(QQmlListProperty<Conference> conferencePropertyList READ conferencePropertyList NOTIFY conferencePropertyListChanged)
Q_PROPERTY(QQmlListProperty<Building> buildingPropertyList READ buildingPropertyList NOTIFY buildingPropertyListChanged)
Q_PROPERTY(QQmlListProperty<Floor> floorPropertyList READ floorPropertyList NOTIFY floorPropertyListChanged)
Q_PROPERTY(QQmlListProperty<Room> roomPropertyList READ roomPropertyList NOTIFY roomPropertyListChanged)
Q_PROPERTY(QQmlListProperty<Session> sessionPropertyList READ sessionPropertyList NOTIFY sessionPropertyListChanged)
Q_PROPERTY(QQmlListProperty<ScheduleItem> scheduleItemPropertyList READ scheduleItemPropertyList NOTIFY scheduleItemPropertyListChanged)
Q_PROPERTY(QQmlListProperty<Favorite> favoritePropertyList READ favoritePropertyList NOTIFY favoritePropertyListChanged)
Q_PROPERTY(QQmlListProperty<Bookmark> bookmarkPropertyList READ bookmarkPropertyList NOTIFY bookmarkPropertyListChanged)
Q_PROPERTY(QQmlListProperty<SessionLists> sessionListsPropertyList READ sessionListsPropertyList NOTIFY sessionListsPropertyListChanged)
Q_PROPERTY(QQmlListProperty<SessionLink> sessionLinkPropertyList READ sessionLinkPropertyList NOTIFY sessionLinkPropertyListChanged)
Q_PROPERTY(QQmlListProperty<Speaker> speakerPropertyList READ speakerPropertyList NOTIFY speakerPropertyListChanged)
Q_PROPERTY(QQmlListProperty<SpeakerImage> speakerImagePropertyList READ speakerImagePropertyList NOTIFY speakerImagePropertyListChanged)
Q_PROPERTY(QQmlListProperty<SessionTrack> sessionTrackPropertyList READ sessionTrackPropertyList NOTIFY sessionTrackPropertyListChanged)
Q_PROPERTY(QQmlListProperty<Day> dayPropertyList READ dayPropertyList NOTIFY dayPropertyListChanged)
Q_PROPERTY(QQmlListProperty<SessionAPI> sessionAPIPropertyList READ sessionAPIPropertyList NOTIFY sessionAPIPropertyListChanged)
Q_PROPERTY(QQmlListProperty<PersonsAPI> personsAPIPropertyList READ personsAPIPropertyList NOTIFY personsAPIPropertyListChanged)
Q_PROPERTY(QQmlListProperty<SessionLinkAPI> sessionLinkAPIPropertyList READ sessionLinkAPIPropertyList NOTIFY sessionLinkAPIPropertyListChanged)
Q_PROPERTY(QQmlListProperty<SpeakerAPI> speakerAPIPropertyList READ speakerAPIPropertyList NOTIFY speakerAPIPropertyListChanged)

public:
    DataManager(QObject *parent = 0);
	
	friend class DataUtil;
	friend class DataServer;
	
    virtual ~DataManager();
    Q_INVOKABLE
    void init();
	bool checkDirs();

	
	

	Q_INVOKABLE
	SettingsData* createSettingsData();

	Q_INVOKABLE
	void undoCreateSettingsData(SettingsData* settingsData);


	


	
	
	Q_INVOKABLE
	QList<Conference*> listOfConferenceForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList conferenceAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allConference();

	Q_INVOKABLE
	void deleteConference();

	// access from QML to list of all Conference
	QQmlListProperty<Conference> conferencePropertyList();

	Q_INVOKABLE
	Conference* createConference();

	Q_INVOKABLE
	void undoCreateConference(Conference* conference);

	Q_INVOKABLE
	void insertConference(Conference* conference);

	Q_INVOKABLE
	void insertConferenceFromMap(const QVariantMap& conferenceMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteConference(Conference* conference);
	

	Q_INVOKABLE
	bool deleteConferenceById(const int& id);

	Q_INVOKABLE
    Conference* findConferenceById(const int& id);

	
	
	Q_INVOKABLE
	QList<Building*> listOfBuildingForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList buildingAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allBuilding();

	Q_INVOKABLE
	void deleteBuilding();

	// access from QML to list of all Building
	QQmlListProperty<Building> buildingPropertyList();

	Q_INVOKABLE
	Building* createBuilding();

	Q_INVOKABLE
	void undoCreateBuilding(Building* building);

	Q_INVOKABLE
	void insertBuilding(Building* building);

	Q_INVOKABLE
	void insertBuildingFromMap(const QVariantMap& buildingMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteBuilding(Building* building);
	

	Q_INVOKABLE
	bool deleteBuildingByBuildingId(const int& buildingId);

	Q_INVOKABLE
    Building* findBuildingByBuildingId(const int& buildingId);

	

	Q_INVOKABLE
	void resolveFloorReferences(Floor* floor);

	Q_INVOKABLE
	void resolveReferencesForAllFloor();
	
	Q_INVOKABLE
	QList<Floor*> listOfFloorForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList floorAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allFloor();

	Q_INVOKABLE
	void deleteFloor();

	// access from QML to list of all Floor
	QQmlListProperty<Floor> floorPropertyList();

	Q_INVOKABLE
	Floor* createFloor();

	Q_INVOKABLE
	void undoCreateFloor(Floor* floor);

	Q_INVOKABLE
	void insertFloor(Floor* floor);

	Q_INVOKABLE
	void insertFloorFromMap(const QVariantMap& floorMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteFloor(Floor* floor);
	

	Q_INVOKABLE
	bool deleteFloorByFloorId(const int& floorId);

	Q_INVOKABLE
    Floor* findFloorByFloorId(const int& floorId);

	

	Q_INVOKABLE
	void resolveRoomReferences(Room* room);

	Q_INVOKABLE
	void resolveReferencesForAllRoom();
	
	Q_INVOKABLE
	QList<Room*> listOfRoomForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList roomAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allRoom();

	Q_INVOKABLE
	void deleteRoom();

	// access from QML to list of all Room
	QQmlListProperty<Room> roomPropertyList();

	Q_INVOKABLE
	Room* createRoom();

	Q_INVOKABLE
	void undoCreateRoom(Room* room);

	Q_INVOKABLE
	void insertRoom(Room* room);

	Q_INVOKABLE
	void insertRoomFromMap(const QVariantMap& roomMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteRoom(Room* room);
	

	Q_INVOKABLE
	bool deleteRoomByRoomId(const int& roomId);

	Q_INVOKABLE
    Room* findRoomByRoomId(const int& roomId);

	

	Q_INVOKABLE
	void resolveSessionReferences(Session* session);

	Q_INVOKABLE
	void resolveReferencesForAllSession();
	
	Q_INVOKABLE
	QList<Session*> listOfSessionForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList sessionAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allSession();

	Q_INVOKABLE
	void deleteSession();

	// access from QML to list of all Session
	QQmlListProperty<Session> sessionPropertyList();

	Q_INVOKABLE
	Session* createSession();

	Q_INVOKABLE
	void undoCreateSession(Session* session);

	Q_INVOKABLE
	void insertSession(Session* session);

	Q_INVOKABLE
	void insertSessionFromMap(const QVariantMap& sessionMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteSession(Session* session);
	

	Q_INVOKABLE
	bool deleteSessionBySessionId(const int& sessionId);

	Q_INVOKABLE
    Session* findSessionBySessionId(const int& sessionId);

	

	Q_INVOKABLE
	void resolveScheduleItemReferences(ScheduleItem* scheduleItem);

	Q_INVOKABLE
	void resolveReferencesForAllScheduleItem();
	
	Q_INVOKABLE
	QList<ScheduleItem*> listOfScheduleItemForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList scheduleItemAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allScheduleItem();

	Q_INVOKABLE
	void deleteScheduleItem();

	// access from QML to list of all ScheduleItem
	QQmlListProperty<ScheduleItem> scheduleItemPropertyList();

	Q_INVOKABLE
	ScheduleItem* createScheduleItem();

	Q_INVOKABLE
	void undoCreateScheduleItem(ScheduleItem* scheduleItem);

	Q_INVOKABLE
	void insertScheduleItem(ScheduleItem* scheduleItem);

	Q_INVOKABLE
	void insertScheduleItemFromMap(const QVariantMap& scheduleItemMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteScheduleItem(ScheduleItem* scheduleItem);
	

	Q_INVOKABLE
	bool deleteScheduleItemBySessionId(const int& sessionId);

	Q_INVOKABLE
    ScheduleItem* findScheduleItemBySessionId(const int& sessionId);

	

	Q_INVOKABLE
	void resolveFavoriteReferences(Favorite* favorite);

	Q_INVOKABLE
	void resolveReferencesForAllFavorite();
	
	Q_INVOKABLE
	QList<Favorite*> listOfFavoriteForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList favoriteAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allFavorite();

	Q_INVOKABLE
	void deleteFavorite();

	// access from QML to list of all Favorite
	QQmlListProperty<Favorite> favoritePropertyList();

	Q_INVOKABLE
	Favorite* createFavorite();

	Q_INVOKABLE
	void undoCreateFavorite(Favorite* favorite);

	Q_INVOKABLE
	void insertFavorite(Favorite* favorite);

	Q_INVOKABLE
	void insertFavoriteFromMap(const QVariantMap& favoriteMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteFavorite(Favorite* favorite);
	

	Q_INVOKABLE
	bool deleteFavoriteBySessionId(const int& sessionId);

	Q_INVOKABLE
    Favorite* findFavoriteBySessionId(const int& sessionId);

	

	Q_INVOKABLE
	void resolveBookmarkReferences(Bookmark* bookmark);

	Q_INVOKABLE
	void resolveReferencesForAllBookmark();
	
	Q_INVOKABLE
	QList<Bookmark*> listOfBookmarkForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList bookmarkAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allBookmark();

	Q_INVOKABLE
	void deleteBookmark();

	// access from QML to list of all Bookmark
	QQmlListProperty<Bookmark> bookmarkPropertyList();

	Q_INVOKABLE
	Bookmark* createBookmark();

	Q_INVOKABLE
	void undoCreateBookmark(Bookmark* bookmark);

	Q_INVOKABLE
	void insertBookmark(Bookmark* bookmark);

	Q_INVOKABLE
	void insertBookmarkFromMap(const QVariantMap& bookmarkMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteBookmark(Bookmark* bookmark);
	

	Q_INVOKABLE
	bool deleteBookmarkBySessionId(const int& sessionId);

	Q_INVOKABLE
    Bookmark* findBookmarkBySessionId(const int& sessionId);

	
	
	Q_INVOKABLE
	QList<SessionLists*> listOfSessionListsForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList sessionListsAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allSessionLists();

	Q_INVOKABLE
	void deleteSessionLists();

	// access from QML to list of all SessionLists
	QQmlListProperty<SessionLists> sessionListsPropertyList();

	Q_INVOKABLE
	SessionLists* createSessionLists();

	Q_INVOKABLE
	void undoCreateSessionLists(SessionLists* sessionLists);

	Q_INVOKABLE
	void insertSessionLists(SessionLists* sessionLists);

	Q_INVOKABLE
	void insertSessionListsFromMap(const QVariantMap& sessionListsMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteSessionLists(SessionLists* sessionLists);
	

	Q_INVOKABLE
	bool deleteSessionListsByUuid(const QString& uuid);

	Q_INVOKABLE
	SessionLists* findSessionListsByUuid(const QString& uuid);
	
	
	Q_INVOKABLE
	QList<SessionLink*> listOfSessionLinkForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList sessionLinkAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allSessionLink();

	Q_INVOKABLE
	void deleteSessionLink();

	// access from QML to list of all SessionLink
	QQmlListProperty<SessionLink> sessionLinkPropertyList();

	Q_INVOKABLE
	SessionLink* createSessionLink();

	Q_INVOKABLE
	void undoCreateSessionLink(SessionLink* sessionLink);

	Q_INVOKABLE
	void insertSessionLink(SessionLink* sessionLink);

	Q_INVOKABLE
	void insertSessionLinkFromMap(const QVariantMap& sessionLinkMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteSessionLink(SessionLink* sessionLink);
	

	Q_INVOKABLE
	bool deleteSessionLinkByUuid(const QString& uuid);

	Q_INVOKABLE
	SessionLink* findSessionLinkByUuid(const QString& uuid);
	

	Q_INVOKABLE
	void resolveSpeakerReferences(Speaker* speaker);

	Q_INVOKABLE
	void resolveReferencesForAllSpeaker();
	
	Q_INVOKABLE
	QList<Speaker*> listOfSpeakerForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList speakerAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allSpeaker();

	Q_INVOKABLE
	void deleteSpeaker();

	// access from QML to list of all Speaker
	QQmlListProperty<Speaker> speakerPropertyList();

	Q_INVOKABLE
	Speaker* createSpeaker();

	Q_INVOKABLE
	void undoCreateSpeaker(Speaker* speaker);

	Q_INVOKABLE
	void insertSpeaker(Speaker* speaker);

	Q_INVOKABLE
	void insertSpeakerFromMap(const QVariantMap& speakerMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteSpeaker(Speaker* speaker);
	

	Q_INVOKABLE
	bool deleteSpeakerBySpeakerId(const int& speakerId);

	Q_INVOKABLE
    Speaker* findSpeakerBySpeakerId(const int& speakerId);

	
	
	Q_INVOKABLE
	QList<SpeakerImage*> listOfSpeakerImageForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList speakerImageAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allSpeakerImage();

	Q_INVOKABLE
	void deleteSpeakerImage();

	// access from QML to list of all SpeakerImage
	QQmlListProperty<SpeakerImage> speakerImagePropertyList();

	Q_INVOKABLE
	SpeakerImage* createSpeakerImage();

	Q_INVOKABLE
	void undoCreateSpeakerImage(SpeakerImage* speakerImage);

	Q_INVOKABLE
	void insertSpeakerImage(SpeakerImage* speakerImage);

	Q_INVOKABLE
	void insertSpeakerImageFromMap(const QVariantMap& speakerImageMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteSpeakerImage(SpeakerImage* speakerImage);
	

	Q_INVOKABLE
	bool deleteSpeakerImageBySpeakerId(const int& speakerId);

	Q_INVOKABLE
    SpeakerImage* findSpeakerImageBySpeakerId(const int& speakerId);

	
	
	Q_INVOKABLE
	QList<SessionTrack*> listOfSessionTrackForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList sessionTrackAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allSessionTrack();

	Q_INVOKABLE
	void deleteSessionTrack();

	// access from QML to list of all SessionTrack
	QQmlListProperty<SessionTrack> sessionTrackPropertyList();

	Q_INVOKABLE
	SessionTrack* createSessionTrack();

	Q_INVOKABLE
	void undoCreateSessionTrack(SessionTrack* sessionTrack);

	Q_INVOKABLE
	void insertSessionTrack(SessionTrack* sessionTrack);

	Q_INVOKABLE
	void insertSessionTrackFromMap(const QVariantMap& sessionTrackMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteSessionTrack(SessionTrack* sessionTrack);
	

	Q_INVOKABLE
	bool deleteSessionTrackByTrackId(const int& trackId);

	Q_INVOKABLE
    SessionTrack* findSessionTrackByTrackId(const int& trackId);

	
	
	Q_INVOKABLE
	QList<Day*> listOfDayForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList dayAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allDay();

	Q_INVOKABLE
	void deleteDay();

	// access from QML to list of all Day
	QQmlListProperty<Day> dayPropertyList();

	Q_INVOKABLE
	Day* createDay();

	Q_INVOKABLE
	void undoCreateDay(Day* day);

	Q_INVOKABLE
	void insertDay(Day* day);

	Q_INVOKABLE
	void insertDayFromMap(const QVariantMap& dayMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteDay(Day* day);
	

	Q_INVOKABLE
	bool deleteDayById(const int& id);

	Q_INVOKABLE
    Day* findDayById(const int& id);

	
	
	Q_INVOKABLE
	QList<SessionAPI*> listOfSessionAPIForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList sessionAPIAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allSessionAPI();

	Q_INVOKABLE
	void deleteSessionAPI();

	// access from QML to list of all SessionAPI
	QQmlListProperty<SessionAPI> sessionAPIPropertyList();

	Q_INVOKABLE
	SessionAPI* createSessionAPI();

	Q_INVOKABLE
	void undoCreateSessionAPI(SessionAPI* sessionAPI);

	Q_INVOKABLE
	void insertSessionAPI(SessionAPI* sessionAPI);

	Q_INVOKABLE
	void insertSessionAPIFromMap(const QVariantMap& sessionAPIMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteSessionAPI(SessionAPI* sessionAPI);
	

	Q_INVOKABLE
	bool deleteSessionAPIBySessionId(const int& sessionId);

	Q_INVOKABLE
    SessionAPI* findSessionAPIBySessionId(const int& sessionId);

	
	
	Q_INVOKABLE
	QList<PersonsAPI*> listOfPersonsAPIForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList personsAPIAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allPersonsAPI();

	Q_INVOKABLE
	void deletePersonsAPI();

	// access from QML to list of all PersonsAPI
	QQmlListProperty<PersonsAPI> personsAPIPropertyList();

	Q_INVOKABLE
	PersonsAPI* createPersonsAPI();

	Q_INVOKABLE
	void undoCreatePersonsAPI(PersonsAPI* personsAPI);

	Q_INVOKABLE
	void insertPersonsAPI(PersonsAPI* personsAPI);

	Q_INVOKABLE
	void insertPersonsAPIFromMap(const QVariantMap& personsAPIMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deletePersonsAPI(PersonsAPI* personsAPI);
	

	Q_INVOKABLE
	bool deletePersonsAPIBySpeakerId(const int& speakerId);

	Q_INVOKABLE
    PersonsAPI* findPersonsAPIBySpeakerId(const int& speakerId);

	
	
	Q_INVOKABLE
	QList<SessionLinkAPI*> listOfSessionLinkAPIForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList sessionLinkAPIAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allSessionLinkAPI();

	Q_INVOKABLE
	void deleteSessionLinkAPI();

	// access from QML to list of all SessionLinkAPI
	QQmlListProperty<SessionLinkAPI> sessionLinkAPIPropertyList();

	Q_INVOKABLE
	SessionLinkAPI* createSessionLinkAPI();

	Q_INVOKABLE
	void undoCreateSessionLinkAPI(SessionLinkAPI* sessionLinkAPI);

	Q_INVOKABLE
	void insertSessionLinkAPI(SessionLinkAPI* sessionLinkAPI);

	Q_INVOKABLE
	void insertSessionLinkAPIFromMap(const QVariantMap& sessionLinkAPIMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteSessionLinkAPI(SessionLinkAPI* sessionLinkAPI);
	

	Q_INVOKABLE
	bool deleteSessionLinkAPIByUuid(const QString& uuid);

	Q_INVOKABLE
	SessionLinkAPI* findSessionLinkAPIByUuid(const QString& uuid);
	
	
	Q_INVOKABLE
	QList<SpeakerAPI*> listOfSpeakerAPIForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList speakerAPIAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allSpeakerAPI();

	Q_INVOKABLE
	void deleteSpeakerAPI();

	// access from QML to list of all SpeakerAPI
	QQmlListProperty<SpeakerAPI> speakerAPIPropertyList();

	Q_INVOKABLE
	SpeakerAPI* createSpeakerAPI();

	Q_INVOKABLE
	void undoCreateSpeakerAPI(SpeakerAPI* speakerAPI);

	Q_INVOKABLE
	void insertSpeakerAPI(SpeakerAPI* speakerAPI);

	Q_INVOKABLE
	void insertSpeakerAPIFromMap(const QVariantMap& speakerAPIMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteSpeakerAPI(SpeakerAPI* speakerAPI);
	

	Q_INVOKABLE
	bool deleteSpeakerAPIById(const int& id);

	Q_INVOKABLE
    SpeakerAPI* findSpeakerAPIById(const int& id);



    void initConferenceFromCache();
    void initBuildingFromCache();
    void initFloorFromCache();
    void initRoomFromCache();
    void initSessionFromCache();
    void initScheduleItemFromCache();
    void initFavoriteFromCache();
    void initBookmarkFromCache();
    void initSessionListsFromCache();
    void initSessionLinkFromCache();
    void initSpeakerFromCache();
    void initSpeakerImageFromCache();
    void initSessionTrackFromCache();
    void initDayFromCache();
    void initSessionAPIFromCache();
    void initPersonsAPIFromCache();
    void initSessionLinkAPIFromCache();
    void initSpeakerAPIFromCache();
	Q_INVOKABLE
	SettingsData* settingsData();
	
	void finish();

Q_SIGNALS:

	void shuttingDown();
	void addedToAllConference(Conference* conference);
	void deletedFromAllConferenceById(int id);
	void deletedFromAllConference(Conference* conference);
	void conferencePropertyListChanged();
	void addedToAllBuilding(Building* building);
	void deletedFromAllBuildingByBuildingId(int buildingId);
	void deletedFromAllBuilding(Building* building);
	void buildingPropertyListChanged();
	void addedToAllFloor(Floor* floor);
	void deletedFromAllFloorByFloorId(int floorId);
	void deletedFromAllFloor(Floor* floor);
	void floorPropertyListChanged();
	void addedToAllRoom(Room* room);
	void deletedFromAllRoomByRoomId(int roomId);
	void deletedFromAllRoom(Room* room);
	void roomPropertyListChanged();
	void addedToAllSession(Session* session);
	void deletedFromAllSessionBySessionId(int sessionId);
	void deletedFromAllSession(Session* session);
	void sessionPropertyListChanged();
	void addedToAllScheduleItem(ScheduleItem* scheduleItem);
	void deletedFromAllScheduleItemBySessionId(int sessionId);
	void deletedFromAllScheduleItem(ScheduleItem* scheduleItem);
	void scheduleItemPropertyListChanged();
	void addedToAllFavorite(Favorite* favorite);
	void deletedFromAllFavoriteBySessionId(int sessionId);
	void deletedFromAllFavorite(Favorite* favorite);
	void favoritePropertyListChanged();
	void addedToAllBookmark(Bookmark* bookmark);
	void deletedFromAllBookmarkBySessionId(int sessionId);
	void deletedFromAllBookmark(Bookmark* bookmark);
	void bookmarkPropertyListChanged();
	void addedToAllSessionLists(SessionLists* sessionLists);
	void deletedFromAllSessionListsByUuid(QString uuid);
	void deletedFromAllSessionLists(SessionLists* sessionLists);
	void sessionListsPropertyListChanged();
	void addedToAllSessionLink(SessionLink* sessionLink);
	void deletedFromAllSessionLinkByUuid(QString uuid);
	void deletedFromAllSessionLink(SessionLink* sessionLink);
	void sessionLinkPropertyListChanged();
	void addedToAllSpeaker(Speaker* speaker);
	void deletedFromAllSpeakerBySpeakerId(int speakerId);
	void deletedFromAllSpeaker(Speaker* speaker);
	void speakerPropertyListChanged();
	void addedToAllSpeakerImage(SpeakerImage* speakerImage);
	void deletedFromAllSpeakerImageBySpeakerId(int speakerId);
	void deletedFromAllSpeakerImage(SpeakerImage* speakerImage);
	void speakerImagePropertyListChanged();
	void addedToAllSessionTrack(SessionTrack* sessionTrack);
	void deletedFromAllSessionTrackByTrackId(int trackId);
	void deletedFromAllSessionTrack(SessionTrack* sessionTrack);
	void sessionTrackPropertyListChanged();
	void addedToAllDay(Day* day);
	void deletedFromAllDayById(int id);
	void deletedFromAllDay(Day* day);
	void dayPropertyListChanged();
	void addedToAllSessionAPI(SessionAPI* sessionAPI);
	void deletedFromAllSessionAPIBySessionId(int sessionId);
	void deletedFromAllSessionAPI(SessionAPI* sessionAPI);
	void sessionAPIPropertyListChanged();
	void addedToAllPersonsAPI(PersonsAPI* personsAPI);
	void deletedFromAllPersonsAPIBySpeakerId(int speakerId);
	void deletedFromAllPersonsAPI(PersonsAPI* personsAPI);
	void personsAPIPropertyListChanged();
	void addedToAllSessionLinkAPI(SessionLinkAPI* sessionLinkAPI);
	void deletedFromAllSessionLinkAPIByUuid(QString uuid);
	void deletedFromAllSessionLinkAPI(SessionLinkAPI* sessionLinkAPI);
	void sessionLinkAPIPropertyListChanged();
	void addedToAllSpeakerAPI(SpeakerAPI* speakerAPI);
	void deletedFromAllSpeakerAPIById(int id);
	void deletedFromAllSpeakerAPI(SpeakerAPI* speakerAPI);
	void speakerAPIPropertyListChanged();


private:
	QString mDataRoot;
	QString mDataPath;
	QString mSettingsPath;
	QString mDataAssetsPath;
	QString dataAssetsPath(const QString& fileName);
	QString dataPath(const QString& fileName);

	SettingsData* mSettingsData;
	void readSettings();
	void saveSettings();
	bool mCompactJson;
	// DataObject stored in List of QObject*
	// GroupDataModel only supports QObject*
    	
    QList<QObject*> mAllConference;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Conference*
    static void appendToConferenceProperty(
    	QQmlListProperty<Conference> *conferenceList,
    	Conference* conference);
    static int conferencePropertyCount(
    	QQmlListProperty<Conference> *conferenceList);
    static Conference* atConferenceProperty(
    	QQmlListProperty<Conference> *conferenceList, int pos);
    static void clearConferenceProperty(
    	QQmlListProperty<Conference> *conferenceList);
    	
    QList<QObject*> mAllBuilding;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Building*
    static void appendToBuildingProperty(
    	QQmlListProperty<Building> *buildingList,
    	Building* building);
    static int buildingPropertyCount(
    	QQmlListProperty<Building> *buildingList);
    static Building* atBuildingProperty(
    	QQmlListProperty<Building> *buildingList, int pos);
    static void clearBuildingProperty(
    	QQmlListProperty<Building> *buildingList);
    	
    QList<QObject*> mAllFloor;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Floor*
    static void appendToFloorProperty(
    	QQmlListProperty<Floor> *floorList,
    	Floor* floor);
    static int floorPropertyCount(
    	QQmlListProperty<Floor> *floorList);
    static Floor* atFloorProperty(
    	QQmlListProperty<Floor> *floorList, int pos);
    static void clearFloorProperty(
    	QQmlListProperty<Floor> *floorList);
    	
    QList<QObject*> mAllRoom;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Room*
    static void appendToRoomProperty(
    	QQmlListProperty<Room> *roomList,
    	Room* room);
    static int roomPropertyCount(
    	QQmlListProperty<Room> *roomList);
    static Room* atRoomProperty(
    	QQmlListProperty<Room> *roomList, int pos);
    static void clearRoomProperty(
    	QQmlListProperty<Room> *roomList);
    	
    QList<QObject*> mAllSession;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Session*
    static void appendToSessionProperty(
    	QQmlListProperty<Session> *sessionList,
    	Session* session);
    static int sessionPropertyCount(
    	QQmlListProperty<Session> *sessionList);
    static Session* atSessionProperty(
    	QQmlListProperty<Session> *sessionList, int pos);
    static void clearSessionProperty(
    	QQmlListProperty<Session> *sessionList);
    	
    QList<QObject*> mAllScheduleItem;
    // implementation for QQmlListProperty to use
    // QML functions for List of All ScheduleItem*
    static void appendToScheduleItemProperty(
    	QQmlListProperty<ScheduleItem> *scheduleItemList,
    	ScheduleItem* scheduleItem);
    static int scheduleItemPropertyCount(
    	QQmlListProperty<ScheduleItem> *scheduleItemList);
    static ScheduleItem* atScheduleItemProperty(
    	QQmlListProperty<ScheduleItem> *scheduleItemList, int pos);
    static void clearScheduleItemProperty(
    	QQmlListProperty<ScheduleItem> *scheduleItemList);
    	
    QList<QObject*> mAllFavorite;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Favorite*
    static void appendToFavoriteProperty(
    	QQmlListProperty<Favorite> *favoriteList,
    	Favorite* favorite);
    static int favoritePropertyCount(
    	QQmlListProperty<Favorite> *favoriteList);
    static Favorite* atFavoriteProperty(
    	QQmlListProperty<Favorite> *favoriteList, int pos);
    static void clearFavoriteProperty(
    	QQmlListProperty<Favorite> *favoriteList);
    	
    QList<QObject*> mAllBookmark;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Bookmark*
    static void appendToBookmarkProperty(
    	QQmlListProperty<Bookmark> *bookmarkList,
    	Bookmark* bookmark);
    static int bookmarkPropertyCount(
    	QQmlListProperty<Bookmark> *bookmarkList);
    static Bookmark* atBookmarkProperty(
    	QQmlListProperty<Bookmark> *bookmarkList, int pos);
    static void clearBookmarkProperty(
    	QQmlListProperty<Bookmark> *bookmarkList);
    	
    QList<QObject*> mAllSessionLists;
    // implementation for QQmlListProperty to use
    // QML functions for List of All SessionLists*
    static void appendToSessionListsProperty(
    	QQmlListProperty<SessionLists> *sessionListsList,
    	SessionLists* sessionLists);
    static int sessionListsPropertyCount(
    	QQmlListProperty<SessionLists> *sessionListsList);
    static SessionLists* atSessionListsProperty(
    	QQmlListProperty<SessionLists> *sessionListsList, int pos);
    static void clearSessionListsProperty(
    	QQmlListProperty<SessionLists> *sessionListsList);
    	
    QList<QObject*> mAllSessionLink;
    // implementation for QQmlListProperty to use
    // QML functions for List of All SessionLink*
    static void appendToSessionLinkProperty(
    	QQmlListProperty<SessionLink> *sessionLinkList,
    	SessionLink* sessionLink);
    static int sessionLinkPropertyCount(
    	QQmlListProperty<SessionLink> *sessionLinkList);
    static SessionLink* atSessionLinkProperty(
    	QQmlListProperty<SessionLink> *sessionLinkList, int pos);
    static void clearSessionLinkProperty(
    	QQmlListProperty<SessionLink> *sessionLinkList);
    	
    QList<QObject*> mAllSpeaker;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Speaker*
    static void appendToSpeakerProperty(
    	QQmlListProperty<Speaker> *speakerList,
    	Speaker* speaker);
    static int speakerPropertyCount(
    	QQmlListProperty<Speaker> *speakerList);
    static Speaker* atSpeakerProperty(
    	QQmlListProperty<Speaker> *speakerList, int pos);
    static void clearSpeakerProperty(
    	QQmlListProperty<Speaker> *speakerList);
    	
    QList<QObject*> mAllSpeakerImage;
    // implementation for QQmlListProperty to use
    // QML functions for List of All SpeakerImage*
    static void appendToSpeakerImageProperty(
    	QQmlListProperty<SpeakerImage> *speakerImageList,
    	SpeakerImage* speakerImage);
    static int speakerImagePropertyCount(
    	QQmlListProperty<SpeakerImage> *speakerImageList);
    static SpeakerImage* atSpeakerImageProperty(
    	QQmlListProperty<SpeakerImage> *speakerImageList, int pos);
    static void clearSpeakerImageProperty(
    	QQmlListProperty<SpeakerImage> *speakerImageList);
    	
    QList<QObject*> mAllSessionTrack;
    // implementation for QQmlListProperty to use
    // QML functions for List of All SessionTrack*
    static void appendToSessionTrackProperty(
    	QQmlListProperty<SessionTrack> *sessionTrackList,
    	SessionTrack* sessionTrack);
    static int sessionTrackPropertyCount(
    	QQmlListProperty<SessionTrack> *sessionTrackList);
    static SessionTrack* atSessionTrackProperty(
    	QQmlListProperty<SessionTrack> *sessionTrackList, int pos);
    static void clearSessionTrackProperty(
    	QQmlListProperty<SessionTrack> *sessionTrackList);
    	
    QList<QObject*> mAllDay;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Day*
    static void appendToDayProperty(
    	QQmlListProperty<Day> *dayList,
    	Day* day);
    static int dayPropertyCount(
    	QQmlListProperty<Day> *dayList);
    static Day* atDayProperty(
    	QQmlListProperty<Day> *dayList, int pos);
    static void clearDayProperty(
    	QQmlListProperty<Day> *dayList);
    	
    QList<QObject*> mAllSessionAPI;
    // implementation for QQmlListProperty to use
    // QML functions for List of All SessionAPI*
    static void appendToSessionAPIProperty(
    	QQmlListProperty<SessionAPI> *sessionAPIList,
    	SessionAPI* sessionAPI);
    static int sessionAPIPropertyCount(
    	QQmlListProperty<SessionAPI> *sessionAPIList);
    static SessionAPI* atSessionAPIProperty(
    	QQmlListProperty<SessionAPI> *sessionAPIList, int pos);
    static void clearSessionAPIProperty(
    	QQmlListProperty<SessionAPI> *sessionAPIList);
    	
    QList<QObject*> mAllPersonsAPI;
    // implementation for QQmlListProperty to use
    // QML functions for List of All PersonsAPI*
    static void appendToPersonsAPIProperty(
    	QQmlListProperty<PersonsAPI> *personsAPIList,
    	PersonsAPI* personsAPI);
    static int personsAPIPropertyCount(
    	QQmlListProperty<PersonsAPI> *personsAPIList);
    static PersonsAPI* atPersonsAPIProperty(
    	QQmlListProperty<PersonsAPI> *personsAPIList, int pos);
    static void clearPersonsAPIProperty(
    	QQmlListProperty<PersonsAPI> *personsAPIList);
    	
    QList<QObject*> mAllSessionLinkAPI;
    // implementation for QQmlListProperty to use
    // QML functions for List of All SessionLinkAPI*
    static void appendToSessionLinkAPIProperty(
    	QQmlListProperty<SessionLinkAPI> *sessionLinkAPIList,
    	SessionLinkAPI* sessionLinkAPI);
    static int sessionLinkAPIPropertyCount(
    	QQmlListProperty<SessionLinkAPI> *sessionLinkAPIList);
    static SessionLinkAPI* atSessionLinkAPIProperty(
    	QQmlListProperty<SessionLinkAPI> *sessionLinkAPIList, int pos);
    static void clearSessionLinkAPIProperty(
    	QQmlListProperty<SessionLinkAPI> *sessionLinkAPIList);
    	
    QList<QObject*> mAllSpeakerAPI;
    // implementation for QQmlListProperty to use
    // QML functions for List of All SpeakerAPI*
    static void appendToSpeakerAPIProperty(
    	QQmlListProperty<SpeakerAPI> *speakerAPIList,
    	SpeakerAPI* speakerAPI);
    static int speakerAPIPropertyCount(
    	QQmlListProperty<SpeakerAPI> *speakerAPIList);
    static SpeakerAPI* atSpeakerAPIProperty(
    	QQmlListProperty<SpeakerAPI> *speakerAPIList, int pos);
    static void clearSpeakerAPIProperty(
    	QQmlListProperty<SpeakerAPI> *speakerAPIList);
    	

    void saveConferenceToCache();
    void saveBuildingToCache();
    void saveFloorToCache();
    void saveRoomToCache();
    void saveSessionToCache();
    void saveScheduleItemToCache();
    void saveFavoriteToCache();
    void saveBookmarkToCache();
    void saveSessionListsToCache();
    void saveSessionLinkToCache();
    void saveSpeakerToCache();
    void saveSpeakerImageToCache();
    void saveSessionTrackToCache();
    void saveDayToCache();
    void saveSessionAPIToCache();
    void savePersonsAPIToCache();
    void saveSessionLinkAPIToCache();
    void saveSpeakerAPIToCache();


	QVariantList readFromCache(const QString& fileName);
	void writeToCache(const QString& fileName, QVariantList& data);
};

#endif /* DATAMANAGER_HPP_ */
