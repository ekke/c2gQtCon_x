#include "datautil.hpp"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QImage>

const QString YYYY_MM_DD = "yyyy-MM-dd";
const QString HH_MM = "HH:mm";
const QString LOCAL_HH_MM = "hh:mm";
const QString YYYY_MM_DD_HH_MM = "yyyy-MM-ddHH:mm";
const QString DAYNAME = "dddd";
const QString DAYNAME_HH_MM = "dddd, HH:mm";
const QString DEFAULT_SPEAKER_IMAGE_URL = "http://conf.qtcon.org/person_original.png";
const QString EMPTY_TRACK = "*****";

DataUtil::DataUtil(QObject *parent) : QObject(parent)
{

}

void DataUtil::init(DataManager* dataManager, DataServer* dataServer)
{
    mDataManager = dataManager;
    mDataServer = dataServer;
    mConferenceDataPath = mDataManager->mDataPath + "conference/";
    mDataServer->setConferenceDataPath(mConferenceDataPath);
    // used for temp dynamic lists as QQmlPropertyLists
    mSessionLists = mDataManager->createSessionLists();

    // connections
    bool res = connect(mDataServer, SIGNAL(serverSuccess()), this,
                       SLOT(onServerSuccess()));
    if (!res) {
        Q_ASSERT(res);
    }
    res = connect(mDataServer, SIGNAL(serverFailed(QString)), this,
                  SLOT(onServerFailed(QString)));
    if (!res) {
        Q_ASSERT(res);
    }

    res = connect(mDataServer, SIGNAL(versionSuccess(QByteArray)), this,
                  SLOT(onVersionSuccess(QByteArray)));
    if (!res) {
        Q_ASSERT(res);
    }
    res = connect(mDataServer, SIGNAL(versionFailed(QString)), this,
                  SLOT(onVersionFailed(QString)));
    if (!res) {
        Q_ASSERT(res);
    }
}

QString DataUtil::conferenceDataPath4QML() {
    return "file://"+mConferenceDataPath;
}

bool DataUtil::isDateTooLate()
{
    QString todayDate = QDate::currentDate().toString(YYYY_MM_DD);
    QString lastConferenceDay = ((Day*) mDataManager->mAllDay.last())->conferenceDay().toString(YYYY_MM_DD);
    qDebug() << "todayDate" << todayDate << "lastConferenceDay" << lastConferenceDay;
    return todayDate > lastConferenceDay;
}

/**
 * @brief DataUtil::sessionInfoForSpeaker
 * @param speaker
 * @return all Session Title line by line
 */
QString DataUtil::sessionInfoForSpeaker(Speaker *speaker)
{
    QString info;
    if(!speaker) {
        return info;
    }
    for (int i = 0; i < speaker->sessions().size(); ++i) {
        if(i > 0) {
            info.append("\n");
        }
        info.append(speaker->sessions().at(i)->title());
    }
    return info;
}

QString DataUtil::speakerNamesForSession(Session *session)
{
    QString info;
    if(!session) {
        return info;
    }
    for (int i = 0; i < session->presenter().size(); ++i) {
        QString name = session->presenter().at(i)->name();
        if(name.length()) {
            if(info.length()) {
                info.append(", ");
            }
            info.append(name);
        }
    }
    return info;
}

QString DataUtil::scheduleItemImageForSession(Session *session)
{
    if(!session || !session->hasScheduleItem()) {
        return "";
    }
    if(session->scheduleItemAsDataObject()->isRegistration()) {
        return "key.png";
    }
    if(session->scheduleItemAsDataObject()->isLunch()) {
        return "lunch.png";
    }
    if(session->scheduleItemAsDataObject()->isEvent()) {
        return "party_event.png";
    }
    return "break.png";
}

QString DataUtil::letterForButton(Session *session)
{
    if(!session) {
        return "S";
    }
    if(session->isTraining()) {
        return "T";
    }
    if(session->isLightning()) {
        return "L";
    }
    if (session->isKeynote()) {
        return "K";
    }
    if(session->isCommunity()) {
        return "C";
    }
    if(session->isMeeting()) {
        return "M";
    }
    if(session->isUnconference()) {
        return "U";
    }
    return "S";
}

QString DataUtil::textForSessionTrack(Session *session)
{
    if(!session || !session->hasSessionTrack()) {
        return "";
    }
    QString name = session->sessionTrackAsDataObject()->name();
    //    if(name == "Community") {
    //        return "";
    //    }
    if(name == "*****") {
        return "";
    }
    if(name == "Unconference") {
        return "";
    }
    return name;
}

QString DataUtil::textForSessionType(Session *session)
{
    if(!session) {
        return "";
    }
    QString info = " (" + QString::number(session->minutes()) + tr(" Minutes)");
    if(session->hasScheduleItem()) {
        if(session->scheduleItemAsDataObject()->isRegistration()) {
            return tr("Registration")+info;
        }
        if(session->scheduleItemAsDataObject()->isEvent()) {
            return tr("Event")+info;
        }
        if(session->scheduleItemAsDataObject()->isLunch()) {
            return tr("Lunch")+info;
        }
        return tr("Break")+info;
    }
    if(session->isTraining()) {
        return tr("Training")+info;
    }
    if(session->isLightning()) {
        return tr("Lightning Talk")+info;
    }
    if(session->isKeynote()) {
        return tr("Keynote")+info;
    }
    if(session->isCommunity()) {
        return tr("Community")+info;
    }
    if(session->isUnconference()) {
        return tr("Unconference")+info;
    }
    if(session->isMeeting()) {
        return tr("Meeting")+info;
    }
    return tr("Session")+info;
}

QString DataUtil::trackColor(const int trackId)
{
    switch (trackId) {
    case 3:
        // *****
        return "transparent";
    case 25:
        // 3D
        return "#FFCDD2";
    case 18:
        // Automotive, Mobile and Embedded
        return "#F06292";
    case 21:
        // Best practices
        return "#BA68C8";
    case 14:
        // Beyond Code
        return "#D1C4E9";
    case 2:
        // Break "#FFFFFF"
        return "grey";
    case 23:
        // Codecs
        return "#E8EAF6";
    case 4:
        // Community
        return "#B3E5FC";
    case 12:
        // Distributing Software and Resources
        return "#64B5F6";
    case 6:
        // Free Software policies and politics
        return "#4DD0E1";
    case 16:
        // Geolocation
        return "#CDDC39";
    case 13:
        // In depth
        return "#26A69A";
    case 10:
        // KDE‘s Latest and Greatest
        return "#2196F3";
    case 5:
        // Let‘s talk business
        return "#FFEB3B";
    case 24:
        // Let‘s talk software
        return "#FFC107";
    case 1:
        // Misc
        return "#FF9890";
    case 17:
        // Multithreading
        return "#FF7043";
    case 22:
        // OpenGL and 3D
        return "#E53935";
    case 11:
        // Platforms and Integration
        return "#BCAAA4";
    case 9:
        // Qt in Use
        return "#4CAF50";
    case 15:
        // QtQuick
        return "#8BC34A";
    case 20:
        // Technical Discussions
        return "#8D6E63";
    case 8:
        // Testing and Continuous Integration
        return "#E0E0E0";
    case 7:
        // Tooling
        return "#90A4AE";
    case 19:
        // Web
        return "#607D8B";
    default:
        // transparent
        return "transparent";
    }
}

// if update failed Data in memory is inconsistent
// delete all, then do init again
void DataUtil::reloadData() {
    // delete all to avoid orphans
    mProgressInfotext.append(tr(" Rollback"));
    emit progressInfo(mProgressInfotext);
    // DO IT
    // reload
    mProgressInfotext.append(tr(" Reload"));
    emit progressInfo(mProgressInfotext);
    // DO IT
    mDataManager->init();
}

// creates missing dirs if preparing conference (pre-conf-stuff)
// or checking for schedule updates
bool DataUtil::checkDirs()
{
    // data/conference
    QString directory = mDataManager->mDataPath + "conference/";
    QDir myDir;
    bool exists;
    exists = myDir.exists(directory);
    if (!exists) {
        bool ok = myDir.mkpath(directory);
        if(!ok) {
            qWarning() << "Couldn't create conference dir " << directory;
            return false;
        }
        qDebug() << "created directory conference " << directory;
    }
    // data/conference/speakerImages
    directory.append("speakerImages/");
    exists = myDir.exists(directory);
    if (!exists) {
        bool ok = myDir.mkpath(directory);
        if(!ok) {
            qWarning() << "Couldn't create speakerImages dir " << directory;
            return false;
        }
        qDebug() << "created directory speakerImages " << directory;
    }
    return true;
}

// P R E   C O N F E R E N C E   S T U F F
/**
 * Prepare will be done before submitting the App to AppStore
 * Use public Cache
 * Best practice:
 * 1. remove app from dev device
 * 2. start app, switch to public cache
 * 3. restart app
 * If all done send files from Phone to PC/Mac
 * Copy them into data-assets /prod or /test
 * Test again after deleting app from dev device
 * Now Users will get this directly by installing the app
 * and updates are fast
 */
void DataUtil::prepareConference() {
    // must use public cache
    if(!mDataManager->settingsData()->hasPublicCache()) {
        qWarning() << "Preparation must be done from public cache";
        return;
    }
    qDebug() << "PREPARE CONFERENCE ";
    // check dirs for pre-conference stuff
    bool dirsOk = checkDirs();
    if(!dirsOk) {
        qWarning() << "cannot create directories";
        return;
    }
    // create some data for this specific conference
    prepareEventData();
    // prepare speaker
    prepareSpeaker();
    // prepare sessions
    // speaker must exist because sessions will be added to
    prepareSessions();
    // download speaker images
    prepareSpeakerImages();
    // now cache all the data
    // if ok - copied to qrc: data-assets
    qDebug() << "cache DATA";
    mDataManager->saveConferenceToCache();
    mDataManager->saveDayToCache();
    mDataManager->saveBuildingToCache();
    mDataManager->saveFloorToCache();
    mDataManager->saveRoomToCache();
    mDataManager->saveSessionToCache();
    // sort Tracks
    QMultiMap<QString, SessionTrack*> sessionTrackSortMap;
    for (int i = 0; i < mDataManager->allSessionTrack().size(); ++i) {
        SessionTrack* sessionTrack = (SessionTrack*) mDataManager->allSessionTrack().at(i);
        sessionTrackSortMap.insert(sessionTrack->name(), sessionTrack);
    }
    mDataManager->mAllSessionTrack.clear();
    QMapIterator<QString, SessionTrack*> sessionTrackIterator(sessionTrackSortMap);
    while (sessionTrackIterator.hasNext()) {
        sessionTrackIterator.next();
        SessionTrack* sessionTrack = sessionTrackIterator.value();
        mDataManager->insertSessionTrack(sessionTrack);
    }

    mDataManager->saveSessionTrackToCache();
    mDataManager->saveScheduleItemToCache();
    mDataManager->saveSessionLinkToCache();

    mDataManager->saveSpeakerToCache();
    // set API Version
    mDataManager->mSettingsData->setApiVersion(mNewApi);
    qDebug() << "PREPARE   D O N E - WAIT FOR IMAGES";
}

// conference, days, building-floor-rooms
void DataUtil::prepareEventData() {
    qDebug() << "PREPARE EVENT ";
    // CONFERENCE
    mDataManager->deleteConference();
    mDataManager->deleteDay();
    mDataManager->deleteSession();
    mDataManager->deleteScheduleItem();
    mDataManager->deleteSessionLink();
    mDataManager->deleteSpeaker();
    mDataManager->deleteSpeakerImage();
    //
    Conference* conference = mDataManager->createConference();
    int lastNr = 0;
    for (int i = 0; i < mDataManager->allBuilding().size(); ++i) {
        Building* building = (Building*) mDataManager->allBuilding().at(i);
        if(building->buildingId() > lastNr) {
            lastNr = building->buildingId();
        }
    }
    conference->setLastBuildingId(lastNr);
    lastNr = 0;
    for (int i = 0; i < mDataManager->allFloor().size(); ++i) {
        Floor* floor = (Floor*) mDataManager->allFloor().at(i);
        if(floor->floorId() > lastNr) {
            lastNr = floor->floorId();
        }
    }
    conference->setLastFloorId(lastNr);
    lastNr = 0;
    for (int i = 0; i < mDataManager->allRoom().size(); ++i) {
        Room* room = (Room*) mDataManager->allRoom().at(i);
        if(room->roomId() > lastNr) {
            lastNr = room->roomId();
        }
        room->clearSessions();
    }
    conference->setLastRoomId(lastNr);
    lastNr = 0;
    for (int i = 0; i < mDataManager->allSessionTrack().size(); ++i) {
        SessionTrack* track = (SessionTrack*) mDataManager->allSessionTrack().at(i);
        if(track->trackId() > lastNr) {
            lastNr = track->trackId();
        }
        track->clearSessions();
    }
    conference->setLastSessionTrackId(lastNr);

    conference->setConferenceName("QtCon");
    conference->setConferenceCity("Berlin");
    QString venueAddress;
    venueAddress = "BCC Berlin Congress Center";
    venueAddress.append("\n");
    venueAddress.append("Tagungszentrum Mitte");
    venueAddress.append("\n");
    venueAddress.append("Alexanderstr. 11");
    venueAddress.append("\n");
    venueAddress.append("10178 Berlin");
    venueAddress.append("\n");
    venueAddress.append("Deutschland");
    conference->setAddress(venueAddress);
    conference->setTimeZoneName("Europe/Amsterdam");
    conference->setTimeZoneOffsetSeconds(1 * 60 * 60); // +01:00 GMT
    conference->setConferenceFrom(QDate::fromString("2016-09-01", YYYY_MM_DD));
    conference->setConferenceTo(QDate::fromString("2016-09-04", YYYY_MM_DD));
    conference->setHashTag("#QtCon16");
    conference->setHomePage("http://qtcon.org/");
    QString coordinate;
    coordinate = QString::number(52.520778)+";"+QString::number(13.416515);
    // conference->coordinate()->setLatitude(52.520778);
    // conference->coordinate()->setLongitude(13.416515);
    conference->setCoordinate(coordinate);
    mDataManager->insertConference(conference);
    // DAYS
    // Days dayOfWeek 1=monday, 7 = sunday
    // thursday
    Day* day = mDataManager->createDay();
    day->setId(1);
    day->setWeekDay(4);
    day->setConferenceDay(QDate::fromString("2016-09-01", YYYY_MM_DD));
    conference->addToDays(day);
    mDataManager->insertDay(day);
    // friday
    day = mDataManager->createDay();
    day->setId(2);
    day->setWeekDay(5);
    day->setConferenceDay(QDate::fromString("2016-09-02", YYYY_MM_DD));
    conference->addToDays(day);
    mDataManager->insertDay(day);
    // saturday
    day = mDataManager->createDay();
    day->setId(3);
    day->setWeekDay(6);
    day->setConferenceDay(QDate::fromString("2016-09-03", YYYY_MM_DD));
    conference->addToDays(day);
    mDataManager->insertDay(day);
    // sunday
    day = mDataManager->createDay();
    day->setId(4);
    day->setWeekDay(1);
    day->setConferenceDay(QDate::fromString("2016-09-04", YYYY_MM_DD));
    conference->addToDays(day);
    mDataManager->insertDay(day);
    // BUILDING
    // Building and FLOORS already created
    // ROOMS created, but delete current sessions yet
    qDebug() << "ROOMS: #" << mDataManager->allRoom().size();
    for (int r = 0; r < mDataManager->allRoom().size(); ++r) {
        Room* room = (Room*) mDataManager->allRoom().at(r);
        room->clearSessions();
    }
    //
}

QVariantMap DataUtil::readScheduleFile(const QString schedulePath) {
    QVariantMap map;
    QFile readFile(schedulePath);
    if(!readFile.exists()) {
        qWarning() << "Schedule Path not found " << schedulePath;
        return map;
    }
    if (!readFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file: " << schedulePath;
        return map;
    }
    QJsonDocument jda = QJsonDocument::fromJson(readFile.readAll());

    readFile.close();
    if(!jda.isObject()) {
        qWarning() << "Couldn't create JSON from file: " << schedulePath;
        return map;
    }
    qDebug() << "QJsonDocument for schedule with Object :)";
    map = jda.toVariant().toMap();
    return map;
}

Day* DataUtil::findDayForServerDate(const QString& dayDate) {
    Day* day = nullptr;
    bool found = false;
    for (int dl = 0; dl < mDataManager->mAllDay.size(); ++dl) {
        day = (Day*) mDataManager->mAllDay.at(dl);
        if(day->conferenceDay().toString(YYYY_MM_DD) == dayDate) {
            found = true;
            break;
        }
    }
    if(found) {
        qDebug() << "Day found";
        return day;
    }
    qDebug() << "Day not found";
    return 0;
}

void DataUtil::adjustPersons(QVariantMap& sessionMap) {
    QStringList personKeys;
    QVariantList personsList;
    personsList = sessionMap.value("persons").toList();
    if (personsList.size() > 0) {
        for (int pvl = 0; pvl < personsList.size(); ++pvl) {
            QVariantMap map = personsList.at(pvl).toMap();
            if(map.contains("id")) {
                personKeys.append(map.value("id").toString());
            }
        }
        sessionMap.insert("persons", personKeys);
    }
}

void DataUtil::createAndAdjustLinks(QVariantMap& sessionMap) {
    QStringList linkKeys;
    QVariantList linksList;
    linksList = sessionMap.value("links").toList();
    if (linksList.size() > 0) {
        for (int lvl = 0; lvl < linksList.size(); ++lvl) {
            QVariantMap map = linksList.at(lvl).toMap();
            if(map.contains("url")) {
                SessionLink* sessionLink = mDataManager->createSessionLink();
                sessionLink->setUrl(map.value("url").toString());
                sessionLink->setTitle(map.value("title").toString());
                mDataManager->insertSessionLink(sessionLink);
                linkKeys.append(sessionLink->uuid());
            }
        }
        sessionMap.insert("links", linkKeys);
    }
}

bool DataUtil::checkIfIgnored(SessionAPI* sessionAPI) {
    if(sessionAPI->title() == "Registration and Coffee" && sessionAPI->room() != "B02") {
        qDebug() << "unwanted session: " << sessionAPI->sessionId() << " " << sessionAPI->title() << " " << sessionAPI->room();
        return true;
    }
    if(sessionAPI->title() == "Lunch" && sessionAPI->room() != "B02") {
        qDebug() << "unwanted session: " << sessionAPI->sessionId() << " " << sessionAPI->title() << " " << sessionAPI->room();
        return true;
    }
    if(sessionAPI->title() == "Coffee break" && sessionAPI->room() != "B02") {
        qDebug() << "unwanted session: " << sessionAPI->sessionId() << " " << sessionAPI->title() << " " << sessionAPI->room();
        return true;
    }
    if(sessionAPI->title() == "Evening event" && sessionAPI->room() != "B02") {
        qDebug() << "unwanted session: " << sessionAPI->sessionId() << " " << sessionAPI->title() << " " << sessionAPI->room();
        return true;
    }
    if(sessionAPI->title() == "Welcome" && sessionAPI->room() != "C01") {
        qDebug() << "unwanted session: " << sessionAPI->sessionId() << " " << sessionAPI->title() << " " << sessionAPI->room();
        return true;
    }
    return false;
}

void DataUtil::setDuration(SessionAPI* sessionAPI, Session* session) {
    QStringList duration;
    duration = sessionAPI->duration().split(":");
    int minutes = 0;
    if(duration.length() == 2) {
        minutes = duration.last().toInt();
        minutes += duration.first().toInt()*60;
    } else {
        qWarning() << "Duration wrong: " << sessionAPI->duration() << " session ID: " << sessionAPI->sessionId();
    }
    session->setMinutes(minutes);
    session->setEndTime(session->startTime().addSecs(minutes * 60));
}

void DataUtil::setTrackAndType(SessionAPI* sessionAPI, Session* session, Conference* conference, const bool isUpdate) {
    SessionTrack* sessionTrack = nullptr;
    bool found = false;
    QString trackName;
    trackName = sessionAPI->track();
    if(trackName.isEmpty()) {
        trackName = EMPTY_TRACK;
    }
    for (int tr = 0; tr < mDataManager->mAllSessionTrack.size(); ++tr) {
        sessionTrack = (SessionTrack*) mDataManager->mAllSessionTrack.at(tr);
        if(sessionTrack->name() == trackName) {
            found = true;
            break;
        }
    }
    if(!found) {
        sessionTrack = mDataManager->createSessionTrack();
        conference->setLastSessionTrackId(conference->lastSessionTrackId()+1);
        sessionTrack->setTrackId(conference->lastSessionTrackId());
        sessionTrack->setName(trackName);
        sessionTrack->setInAssets(isUpdate?false:true);
        mDataManager->insertSessionTrack(sessionTrack);
    }
    if (sessionTrack) {
        session->setSessionTrack(sessionTrack->trackId());
        session->resolveSessionTrackAsDataObject(sessionTrack);
    }
    // SCHEDULE or what else
    // setting some boolean here makes it easier to distinguish in UI
    if (trackName == "Break" || (trackName == "Misc" && session->title().contains("Registration"))) {
        ScheduleItem* scheduleItem;
        if(isUpdate && session->hasScheduleItem()) {
            scheduleItem = session->scheduleItemAsDataObject();
        } else {
            scheduleItem = mDataManager->createScheduleItem();
            scheduleItem->setSessionId(session->sessionId());
            session->setScheduleItem(scheduleItem->sessionId());
            session->resolveScheduleItemAsDataObject(scheduleItem);
            scheduleItem->setSession(session->sessionId());
            mDataManager->insertScheduleItem(scheduleItem);
        }
        if(session->title().contains("Evening event") || session->title().contains("SHOW EUROPE")) {
            scheduleItem->setIsEvent(true);
        } else {
            if(session->title().contains("Registration")) {
                scheduleItem->setIsRegistration(true);
            } else {
                if(session->title().contains("Lunch")) {
                    scheduleItem->setIsLunch(true);
                } else {
                    scheduleItem->setIsBreak(true);
                }
            }
        }
    } else {
        if(isUpdate && session->hasScheduleItem()) {
            // Session type changed, so remove any old schedule items
            mDataManager->deleteScheduleItemBySessionId(session->sessionId());
        }
        if(session->title().contains("Lightning") || session->sessionType().contains("lightning_talk")) {
            session->setIsLightning(true);
        } else {
            if(session->title().contains("Keynote")) {
                session->setIsKeynote(true);
            } else {
                if(trackName == "Community") {
                    session->setIsCommunity(true);
                } else {
                    if(session->sessionDayAsDataObject()->conferenceDay().toString(YYYY_MM_DD) == "2016-09-01" && session->title().contains("Training")) {
                        session->setIsTraining(true);
                    } else {
                        if(session->sessionType() == "meeting") {
                            session->setIsMeeting(true);
                        } else {
                            if(session->title().contains("Unconference")) {
                                session->setIsUnconference(true);
                            } else {
                                session->setIsSession(true);
                            }
                        }
                    }
                }
            }
        }
    }
}

void DataUtil::prepareSessions()
{
    const QString schedulePath = mDataManager->mDataAssetsPath + "conference/schedule.json";
    qDebug() << "PREPARE SESSIONS ";
    QVariantMap map;
    map = readScheduleFile(schedulePath);

    if(map.isEmpty()) {
        qWarning() << "Schedule is no Map";
        return;
    }
    map = map.value("schedule").toMap();
    if(map.isEmpty()) {
        qWarning() << "No 'schedule' found";
        return;
    }
    mNewApi = map.value("version").toString();
    qDebug() << "VERSION: " + mNewApi;

    map = map.value("conference").toMap();
    if(map.isEmpty()) {
        qWarning() << "No 'conference' found";
        return;
    }

    qDebug() << "TITLE: " << map.value("title").toString();
    Conference* conference;
    conference = (Conference*) mDataManager->allConference().first();
    QVariantList dayList;
    dayList = map.value("days").toList();
    if(dayList.isEmpty()) {
        qWarning() << "No 'days' found";
        return;
    }
    mMultiSession.clear();
    qDebug() << "DAYS: " << dayList.size();
    for (int i = 0; i < dayList.size(); ++i) {
        QVariantMap dayMap;
        dayMap = dayList.at(i).toMap();
        if(dayMap.isEmpty()) {
            qWarning() << "No 'DAY' found #" << i;
            continue;
        }
        QString dayDate;
        dayDate = dayMap.value("date").toString();
        qDebug() << "processing DATE: " << dayDate;

        Day* day = findDayForServerDate(dayDate);
        if(!day) {
            qWarning() << "No Day* found for " << dayDate;
            continue;
        }

        bool found = false;
        QVariantMap roomMap;
        roomMap = dayMap.value("rooms").toMap();
        QStringList roomKeys = roomMap.keys();
        if(roomKeys.isEmpty()) {
            qWarning() << "No 'ROOMS' found for DAY # i";
            continue;
        }
        for (int r = 0; r < roomKeys.size(); ++r) {
            QVariantList sessionList;
            sessionList = roomMap.value(roomKeys.at(r)).toList();
            if(sessionList.isEmpty()) {
                qWarning() << "DAY: " << dayDate << " ROOM: " << roomKeys.at(r) << " ignored - No Sessions available";
                continue;
            }
            Room* room = nullptr;
            found = false;
            for (int rl = 0; rl < mDataManager->mAllRoom.size(); ++rl) {
                room = (Room*) mDataManager->mAllRoom.at(rl);
                if(room->roomName() == roomKeys.at(r)) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                qWarning() << "Room* not found for " << dayDate << " Room: " << roomKeys.at(r);
                // TODO add new Room
                continue;
            }
            for (int sl = 0; sl < sessionList.size(); ++sl) {
                QVariantMap sessionMap;
                sessionMap = sessionList.at(sl).toMap();
                if(sessionMap.isEmpty()) {
                    qWarning() << "No 'SESSION' Map DAY: " << dayDate << " ROOM: " << roomKeys.at(r);
                    continue;
                }
                // adjust persons
                adjustPersons(sessionMap);

                // create and adjust links
                createAndAdjustLinks(sessionMap);

                SessionAPI* sessionAPI = mDataManager->createSessionAPI();
                sessionAPI->fillFromForeignMap(sessionMap);
                // ignore unwanted Sessions
                if (checkIfIgnored(sessionAPI)) {
                    continue;
                }
                Session* session = mDataManager->createSession();
                session->fillFromMap(sessionAPI->toMap());
                setDuration(sessionAPI, session);
                // refs
                // DAY
                session->setSessionDay(day->id());
                // ROOM
                session->setRoom(room->roomId());
                // TRACK TYPE SCHEDULE
                setTrackAndType(sessionAPI, session, conference, false);
                // SORT
                session->setSortKey(day->conferenceDay().toString(YYYY_MM_DD)+session->startTime().toString(HH_MM));
                mMultiSession.insert(session->sortKey(), session);
            } // end for session of a room
        } // room keys
    } // for days list
    // insert sorted Sessions
    sortedSessionsIntoRoomDayTrackSpeaker();
}

// sessions must be cleared before for Day Track, Room, Speaker
// always if prepare Conference, explicitely if Update Schedule
void DataUtil::sortedSessionsIntoRoomDayTrackSpeaker() {
    QMapIterator<QString, Session*> sessionIterator(mMultiSession);
    while (sessionIterator.hasNext()) {
        sessionIterator.next();
        Session* session = sessionIterator.value();
        mDataManager->insertSession(session);
        Room* room = mDataManager->findRoomByRoomId(session->room());
        if(room != NULL) {
            room->addToSessions(session);
        } else {
            qWarning() << "ROOM is NULL for Session " << session->sessionId() << " #:" << session->room();
        }
        Day* day = mDataManager->findDayById(session->sessionDay());
        if(day != NULL) {
            day->addToSessions(session);
        } else {
            qWarning() << "DAY is NULL for Session " << session->sessionId() << " #:" << session->sessionDay();
        }
        SessionTrack* sessionTrack = mDataManager->findSessionTrackByTrackId(session->sessionTrack());
        if(sessionTrack != NULL) {
            sessionTrack->addToSessions(session);
        } else {
            qWarning() << "TRACK is NULL for Session " << session->sessionId() << " #:" << session->sessionTrack();
        }
        for (int i = 0; i < session->presenterKeys().size(); ++i) {
            int pKey = session->presenterKeys().at(i).toInt();
            Speaker* speaker = (Speaker*) mDataManager->findSpeakerBySpeakerId(pKey);
            if(speaker != NULL) {
                speaker->addToSessions(session);
            } else {
                qWarning() << "SPEAKER is NULL for Session " << session->sessionId() << " #:" << pKey;
            }
        } // for presenter
    } // while all sessions
}

QVariantList DataUtil::readSpeakerFile(const QString speakerPath) {
    QVariantList dataList;
    QFile readFile(speakerPath);
    if(!readFile.exists()) {
        qWarning() << "Speaker Path not found " << speakerPath;
        return dataList;
    }
    if (!readFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file: " << speakerPath;
        return dataList;
    }
    QJsonDocument jda = QJsonDocument::fromJson(readFile.readAll());

    readFile.close();
    if(!jda.isArray()) {
        qWarning() << "Couldn't create JSON from file: " << speakerPath;
        return dataList;
    }
    qDebug() << "QJsonDocument for speaker with Array :)";
    dataList = jda.toVariant().toList();
    return dataList;
}

void DataUtil::calcSpeakerName(Speaker* speaker, SpeakerAPI* speakerAPI) {
    speaker->setName(speakerAPI->firstName());
    if(speaker->name().length() > 0) {
        speaker->setName(speaker->name()+" ");
    }
    speaker->setName(speaker->name()+speakerAPI->lastName());
    if(speaker->name().length() > 0) {
        if(speakerAPI->lastName().length() > 0) {
            speaker->setSortKey(speakerAPI->lastName().left(5).toUpper());
        } else {
            speaker->setSortKey(speakerAPI->firstName().left(5).toUpper());
        }
        speaker->setSortGroup(speaker->sortKey().left(1));
    } else {
        speaker->setSortKey("*");
        speaker->setSortGroup("*");
    }
}

void DataUtil::prepareSpeaker()
{
    const QString speakersPath = mDataManager->mDataAssetsPath + "conference/speaker.json";
    qDebug() << "PREPARE SPEAKER ";
    QVariantList dataList;
    dataList = readSpeakerFile(speakersPath);
    if(dataList.size() == 0) {
        qWarning() << "Speaker List empty";
        return;
    }

    mMultiSpeaker.clear();
    for (int i = 0; i < dataList.size(); ++i) {
        SpeakerAPI* speakerAPI = mDataManager->createSpeakerAPI();
        speakerAPI->fillFromForeignMap(dataList.at(i).toMap());

        Speaker* speaker = mDataManager->createSpeaker();
        speaker->setSpeakerId(speakerAPI->id());

        calcSpeakerName(speaker, speakerAPI);
        speaker->setBio(speakerAPI->bio());

        if(speakerAPI->avatar().length() > 0 && speakerAPI->avatar() != DEFAULT_SPEAKER_IMAGE_URL) {
            QString avatar = speakerAPI->avatar();
            QStringList sl = avatar.split("?");
            if(sl.size() > 1) {
                sl.removeLast();
                avatar = sl.join("?");
            }
            sl = avatar.split(".");
            if(sl.size() < 2) {
                qWarning() << "AVATAR wrong "+speakerAPI->avatar();
            } else {
                avatar = avatar.replace("http://","https://");
                SpeakerImage* speakerImage = mDataManager->createSpeakerImage();
                speakerImage->setSpeakerId(speaker->speakerId());
                speakerImage->setOriginImageUrl(avatar);
                speakerImage->setSuffix(sl.last());
                mDataManager->insertSpeakerImage(speakerImage);
                speaker->resolveSpeakerImageAsDataObject(speakerImage);
            }
        }
        // using MultiMap to get Speakers sorted
        mMultiSpeaker.insert(speaker->sortKey(), speaker);
    } // end for all SpeakersAPI
    // insert sorted Speakers
    mDataManager->mAllSpeaker.clear();
    QMapIterator<QString, Speaker*> speakerIterator(mMultiSpeaker);
    while (speakerIterator.hasNext()) {
        speakerIterator.next();
        mDataManager->insertSpeaker(speakerIterator.value());
    }
}

void DataUtil::prepareSpeakerImages()
{
    const QString speakerImagesPath = mConferenceDataPath + "speakerImages/";
    for (int i = 0; i < mDataManager->allSpeakerImage().size(); ++i) {
        SpeakerImage* speakerImage = (SpeakerImage*) mDataManager->allSpeakerImage().at(i);
        if (!speakerImage->downloadSuccess() && !speakerImage->downloadFailed()) {
            qDebug() << "loading..." << speakerImage->speakerId();
            QString fileName;
            fileName = "speaker_";
            fileName.append(QString::number(speakerImage->speakerId()));
            fileName.append('.');
            fileName.append(speakerImage->suffix());
            mImageLoader = new ImageLoader(speakerImage->originImageUrl(), speakerImagesPath+fileName, this);
            bool res = connect(mImageLoader, SIGNAL(loaded(QObject*, int, int)), this,
                               SLOT(onSpeakerImageLoaded(QObject*, int, int)));
            if (!res) {
                Q_ASSERT(res);
            }
            res = connect(mImageLoader, SIGNAL(loadingFailed(QObject*, QString)), this,
                          SLOT(onSpeakerImageFailed(QObject*, QString)));
            if (!res) {
                Q_ASSERT(res);
            }
            mImageLoader->loadSpeaker(speakerImage);
            return;
        }
    } // for all speaker images
    // N OW cache speaker images
    mDataManager->saveSpeakerImageToCache();
    qDebug() << "SPEAKER IMAGES   D O W N L O A D E D";
}

void DataUtil::checkVersion()
{
    mDataServer->requestVersion();
}

void DataUtil::startUpdate()
{
    bool dirOk = checkDirs();
    if(!dirOk) {
        qWarning() << "Cannot create Directories";
        emit updateFailed(tr("startUpdate - Cannot create Directories"));
        return;
    }
    mProgressInfotext = tr("Request Schedule and Speakers from Server");
    emit progressInfo(mProgressInfotext);
    mDataServer->requestSchedule();
}

void DataUtil::continueUpdate()
{
    mProgressInfotext = tr("Save Favorites");
    emit progressInfo(mProgressInfotext);
    // save F A V O R I T E S and bookmarks
    saveSessionFavorites();
    // S P E A K E R
    mProgressInfotext = tr("Sync Speaker");
    emit progressInfo(mProgressInfotext);
    const QString speakersPath = mConferenceDataPath + "speaker.json";
    qDebug() << "PREPARE SPEAKER ";
    QVariantList dataList;
    dataList = readSpeakerFile(speakersPath);
    if(dataList.size() == 0) {
        qWarning() << "Speaker List empty";
        emit updateFailed(tr("Update failed. No Speaker received.\nReloading current Data"));
        return;
    }
    mMultiSession.clear();
    mMultiSpeaker.clear();
    mMultiSpeakerImages.clear();
    for (int i = 0; i < dataList.size(); ++i) {
        SpeakerAPI* speakerAPI = mDataManager->createSpeakerAPI();
        speakerAPI->fillFromForeignMap(dataList.at(i).toMap());

        Speaker* speaker = mDataManager->findSpeakerBySpeakerId(speakerAPI->id());
        if(!speaker) {
            // NEW speaker
            qDebug() << "NEW SPEAKER";
            mProgressInfotext.append("+");
            speaker = mDataManager->createSpeaker();
            speaker->setSpeakerId(speakerAPI->id());
        } else {
            // update Speaker
            mProgressInfotext.append(".");
        }
        emit progressInfo(mProgressInfotext);
        calcSpeakerName(speaker, speakerAPI);
        speaker->setBio(speakerAPI->bio());
        if(speakerAPI->avatar().length() > 0 && speakerAPI->avatar() != DEFAULT_SPEAKER_IMAGE_URL) {
            QString avatar = speakerAPI->avatar();
            QStringList sl = avatar.split("?");
            if(sl.size() > 1) {
                sl.removeLast();
                avatar = sl.join("?");
            }
            sl = avatar.split(".");
            if(sl.size() < 2) {
                qWarning() << "AVATAR wrong "+speakerAPI->avatar();
            } else {
                // check if modified
                avatar = avatar.replace("http://","https://");
                if(speaker->hasSpeakerImage()) {
                    if(speaker->speakerImageAsDataObject()->originImageUrl() != avatar) {
                        qDebug() << "IMAGE Changed";
                        SpeakerImage* speakerImage = speaker->speakerImageAsDataObject();
                        speakerImage->setOriginImageUrl(avatar);
                        speakerImage->setSuffix(sl.last());
                        mMultiSpeakerImages.insert(false, speakerImage);
                    }
                } else {
                    qDebug() << "IMAGE NEW";
                    SpeakerImage* speakerImage = mDataManager->createSpeakerImage();
                    speakerImage->setSpeakerId(speaker->speakerId());
                    speakerImage->setOriginImageUrl(avatar);
                    speakerImage->setSuffix(sl.last());
                    speakerImage->setInAssets(false);
                    speaker->resolveSpeakerImageAsDataObject(speakerImage);
                    mMultiSpeakerImages.insert(false, speakerImage);
                }
            } // end if valid Avatar URL
        } // end check avatar if URL && not default
        // using MultiMap to get Speakers sorted
        mMultiSpeaker.insert(speaker->sortKey(), speaker);
    } // for speaker from server
    //
    qDebug() << "SPEAKERS: " << mDataManager->mAllSpeaker.size() << " --> " << mMultiSpeaker.size() << " IMG: " << mMultiSpeakerImages.size();
    mProgressInfotext.append("\n").append(tr("Sync Speaker Images"));
    emit progressInfo(mProgressInfotext);
    updateSpeakerImages();
}

void DataUtil::updateSpeakerImages() {
    if(mMultiSpeakerImages.size() > 0) {
        QList<SpeakerImage*> waitingForDownload = mMultiSpeakerImages.values(false);
        if(waitingForDownload.size() > 0) {
            mProgressInfotext.append(".");
            emit progressInfo(mProgressInfotext);
            // DO IT
            const QString speakerImagesPath = mConferenceDataPath + "speakerImages/";
            SpeakerImage* speakerImage = waitingForDownload.first();
            QString fileName;
            fileName = "speaker_";
            fileName.append(QString::number(speakerImage->speakerId()));
            fileName.append('.');
            fileName.append(speakerImage->suffix());
            mImageLoader = new ImageLoader(speakerImage->originImageUrl(), speakerImagesPath+fileName, this);
            bool res = connect(mImageLoader, SIGNAL(loaded(QObject*, int, int)), this,
                               SLOT(onSpeakerImageUpdateLoaded(QObject*, int, int)));
            if (!res) {
                Q_ASSERT(res);
            }
            res = connect(mImageLoader, SIGNAL(loadingFailed(QObject*, QString)), this,
                          SLOT(onSpeakerImageUpdateFailed(QObject*, QString)));
            if (!res) {
                Q_ASSERT(res);
            }
            mImageLoader->loadSpeaker(speakerImage);
            return;
        } // waiting for download
    } // new images map
    // all done
    updateSessions();
}

void DataUtil::updateAndAdjustLinks(QVariantMap &sessionMap) {
    QStringList linkKeys;
    QVariantList linksList;
    linksList = sessionMap.value("links").toList();
    if (linksList.size() > 0) {
        for (int lvl = 0; lvl < linksList.size(); ++lvl) {
            QVariantMap map = linksList.at(lvl).toMap();
            if(map.contains("url")) {
                QString linkUrl = map.value("url").toString();
                SessionLink* sessionLink = nullptr;
                bool linkFound = false;
                for (int xsl = 0; xsl < mDataManager->allSessionLink().size(); ++xsl) {
                    sessionLink = (SessionLink*) mDataManager->allSessionLink().at(xsl);
                    if(sessionLink->url() == linkUrl) {
                        linkFound = true;
                        break;
                    }
                } // end for all existing links
                if(!linkFound) {
                    sessionLink = mDataManager->createSessionLink();
                }
                sessionLink->setUrl(linkUrl);
                sessionLink->setTitle(map.value("title").toString());
                if(!linkFound) {
                    mDataManager->insertSessionLink(sessionLink);
                }
                linkKeys.append(sessionLink->uuid());
            } // map contains url
        } // end for all links
        sessionMap.insert("links", linkKeys);
    } // end if linklist size
}

void DataUtil::updateSessions() {
    mProgressInfotext.append("\n").append(tr("Sync Sessions"));
    emit progressInfo(mProgressInfotext);
    mMultiSession.clear();
    const QString schedulePath = mConferenceDataPath + "schedule.json";
    QVariantMap map;
    map = readScheduleFile(schedulePath);
    if(map.isEmpty()) {
        qWarning() << "Schedule is no Map";
        emit updateFailed(tr("Error: Received Map is empty."));
        return;
    }
    map = map.value("schedule").toMap();
    if(map.isEmpty()) {
        qWarning() << "No 'schedule' found";
        emit updateFailed(tr("Error: Received Map missed 'schedule'."));
        return;
    }
    map = map.value("conference").toMap();
    if(map.isEmpty()) {
        qWarning() << "No 'conference' found";
        emit updateFailed(tr("Error: Received Map missed 'conference'."));
        return;
    }
    Conference* conference;
    conference = (Conference*) mDataManager->allConference().first();
    QVariantList dayList;
    dayList = map.value("days").toList();
    if(dayList.isEmpty()) {
        qWarning() << "No 'days' found";
        emit updateFailed(tr("Error: Received Map missed 'days'."));
        return;
    }
    if(dayList.size() != 4) {
        qWarning() << "Wrong number of 'days' found";
        emit updateFailed(tr("Error: # of 'days' expected: 4 - but got ")+QString::number(dayList.size()));
        return;
    }
    for (int i = 0; i < dayList.size(); ++i) {
        QVariantMap dayMap;
        dayMap = dayList.at(i).toMap();
        if(dayMap.isEmpty()) {
            qWarning() << "No 'DAY' found #" << i;
            emit updateFailed(tr("Map for Day missed from Server"));
            continue;
        }
        QString dayDate;
        dayDate = dayMap.value("date").toString();
        qDebug() << "processing DATE: " << dayDate;
        Day* day = findDayForServerDate(dayDate);
        if(!day) {
            qWarning() << "No Day* found for " << dayDate;
            emit updateFailed(tr("No Day* found for ")+dayDate);
            return;
        }
        bool found = false;
        QVariantMap roomMap;
        roomMap = dayMap.value("rooms").toMap();
        QStringList roomKeys = roomMap.keys();
        if(roomKeys.isEmpty()) {
            qWarning() << "No 'ROOMS' found for DAY # i";
            emit updateFailed(tr("No 'ROOMS' found for DAY ") + dayDate);
            return;
        }
        for (int r = 0; r < roomKeys.size(); ++r) {
            QVariantList sessionList;
            sessionList = roomMap.value(roomKeys.at(r)).toList();
            if(sessionList.isEmpty()) {
                qWarning() << "DAY: " << dayDate << " ROOM: " << roomKeys.at(r) << " ignored - No Sessions available";
                continue;
            }
            Room* room = nullptr;
            found = false;
            for (int rl = 0; rl < mDataManager->mAllRoom.size(); ++rl) {
                room = (Room*) mDataManager->mAllRoom.at(rl);
                if(room->roomName() == roomKeys.at(r)) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                qDebug() << "Room* not found for " << dayDate << " Room: " << roomKeys.at(r);
                room = mDataManager->createRoom();
                conference->setLastRoomId(conference->lastRoomId()+1);
                room->setRoomId(conference->lastRoomId());
                room->setInAssets(false);
                room->setRoomName(roomKeys.at(r));
                mDataManager->insertRoom(room);
                mProgressInfotext.append("R");
                progressInfo(mProgressInfotext);
            }
            for (int sl = 0; sl < sessionList.size(); ++sl) {
                QVariantMap sessionMap;
                sessionMap = sessionList.at(sl).toMap();
                if(sessionMap.isEmpty()) {
                    qWarning() << "No 'SESSION' Map DAY: " << dayDate << " ROOM: " << roomKeys.at(r);
                    continue;
                }
                // adjust persons
                adjustPersons(sessionMap);
                // update and adjust SessionLink
                updateAndAdjustLinks(sessionMap);

                SessionAPI* sessionAPI = mDataManager->createSessionAPI();
                sessionAPI->fillFromForeignMap(sessionMap);
                // ignore unwanted Sessions
                if (checkIfIgnored(sessionAPI)) {
                    continue;
                }

                Session* session = mDataManager->findSessionBySessionId(sessionAPI->sessionId());
                if(!session) {
                    // NEW
                    qDebug() << "NEW Session " << QString::number(sessionAPI->sessionId());
                    mProgressInfotext.append("+");
                    session = mDataManager->createSession();
                    session->setSessionId(sessionAPI->sessionId());
                } else {
                    // Update
                    qDebug() << "Update Session " << QString::number(sessionAPI->sessionId());
                    mProgressInfotext.append(".");
                }
                emit progressInfo(mProgressInfotext);
                session->fillFromMap(sessionAPI->toMap());
                setDuration(sessionAPI, session);
                // refs
                // DAY
                session->setSessionDay(day->id());
                session->resolveSessionDayAsDataObject(day);
                // ROOM
                session->setRoom(room->roomId());
                session->resolveRoomAsDataObject(room);
                // TRACK TYPE SCHEDULE isUpdate=true
                setTrackAndType(sessionAPI, session, conference, true);
                // SessionLinks

                // SORT
                session->setSortKey(day->conferenceDay().toString(YYYY_MM_DD)+session->startTime().toString(HH_MM));
                mMultiSession.insert(session->sortKey(), session);
            } // end for sessions of a room of a day
        } // end for rooms of a day
    } // end for list of days from server

    qDebug() << "SESSIONS: " << mDataManager->mAllSession.size() << " --> " << mMultiSession.size();
    // speaker, images, sessions, days, rooms, tracks --> cache
    // delete orphans
    mProgressInfotext.append("\n").append(tr("Schedule and Speaker successfully synchronized :)"));
    emit progressInfo(mProgressInfotext);
    finishUpdate();
}

void DataUtil::finishUpdate() {
    mProgressInfotext.append("\n").append(tr("Now saving Conference Data to Cache"));
    emit progressInfo(mProgressInfotext);
    // Conference save last xxx Id's
    mDataManager->saveConferenceToCache();
    qDebug() << "FINISH: Conference saved";

    // Building not changed - always initialized from Prepare Conference

    // Floor not changed - always initialized from Prepare Conference

    // Room: clear sessions for update
    for (int r = 0; r < mDataManager->allRoom().size(); ++r) {
        Room* room = (Room*) mDataManager->allRoom().at(r);
        room->clearSessions();
    }
    // to reset all correct
    resolveSessionsForRooms();
    qDebug() << "FINISH: Rooms sessions cleared";
    // SessionTrack: clear sessions for update
    for (int st = 0; st < mDataManager->allSessionTrack().size(); ++st) {
        SessionTrack* track = (SessionTrack*) mDataManager->allSessionTrack().at(st);
        track->clearSessions();
        qDebug() << "clear sessions for Track " << track->name();
    }
    // to reset all correct
    resolveSessionsForTracks();
    qDebug() << "FINISH: Tracks sessions cleared";
    // Day: clear sessions for update
    for (int d = 0; d < mDataManager->allDay().size(); ++d) {
        Day* day = (Day*) mDataManager->allDay().at(d);
        day->clearSessions();
    }
    // to reset all correct
    resolveSessionsForSchedule();
    qDebug() << "FINISH: Days sessions cleared";
    // Speaker: insert sorted Speakers, clear Sessions
    mDataManager->mAllSpeaker.clear();
    QMapIterator<QString, Speaker*> speakerIterator(mMultiSpeaker);
    while (speakerIterator.hasNext()) {
        speakerIterator.next();
        Speaker* speaker = speakerIterator.value();
        speaker->clearSessions();
        resolveSessionsForSpeaker(speaker);
        mDataManager->insertSpeaker(speaker);
    }
    qDebug() << "FINISH: Sorted Speakers inserted";

    // Check orphans
    for (int i = 0; i < mDataManager->mAllSession.size(); ++i) {
        Session* session = (Session*) mDataManager->mAllSession.at(i);
        bool sessionFound = false;
        QMapIterator<QString, Session*> sessionIterator(mMultiSession);
        while (sessionIterator.hasNext()) {
            sessionIterator.next();
            Session* updatedSession = sessionIterator.value();
            if(updatedSession->sessionId() == session->sessionId()) {
                sessionFound = true;
                break;
            }
        }
        if(!sessionFound) {
            session->deleteLater();
            qDebug() << "Session removed: " << session->sessionId();
        }
    }
    qDebug() << "FINISH: Session orphans deleted";

    // Session: insert sorted Sessions
    // presenter, sessionLinks, day, room, track scheduleItem are updated
    mDataManager->mAllSession.clear();
    // now insert sorted Sessions, update sessions for Day, Room, Tracks, Speaker
    sortedSessionsIntoRoomDayTrackSpeaker();
    qDebug() << "FINISH: Rooms Days Tracks Speaker Sessions sorted";
    mDataManager->saveSessionToCache();
    qDebug() << "FINISH: Sessions saved";
    // SessionLink
    mDataManager->saveSessionLinkToCache();
    qDebug() << "FINISH: SessionLinks saved";

    // ScheduleItem
    mDataManager->saveScheduleItemToCache();
    qDebug() << "FINISH: ScheduleItems saved";

    // SPEAKER
    mDataManager->saveSpeakerToCache();
    qDebug() << "FINISH: Speaker saved";

    // insert Speaker Images
    // do NOT mDataManager->mAllSpeakerImage.clear();
    // mMultiSpeakerImages contains NEW or UPDATED Speaker Images !
    QMapIterator<bool, SpeakerImage*> speakerImagesIterator(mMultiSpeakerImages);
    while (speakerImagesIterator.hasNext()) {
        speakerImagesIterator.next();
        SpeakerImage* speakerImage = speakerImagesIterator.value();
        if(!mDataManager->findSpeakerImageBySpeakerId(speakerImage->speakerId())) {
            // a NEW Speaker Image
            qDebug() << "detcted NEW Speaker Image: " << speakerImage->speakerId();
            mDataManager->insertSpeakerImage(speakerImage);
        } else {
            qDebug() << "detcted UPDATED Speaker Image: " << speakerImage->speakerId();
        }
    }
    mDataManager->saveSpeakerImageToCache();
    qDebug() << "FINISH: Speaker Images saved";

    // Track sort by Name
    QMultiMap<QString, SessionTrack*> sessionTrackSortMap;
    for (int i = 0; i < mDataManager->allSessionTrack().size(); ++i) {
        SessionTrack* sessionTrack = (SessionTrack*) mDataManager->allSessionTrack().at(i);
        sessionTrackSortMap.insert(sessionTrack->name(), sessionTrack);
    }
    qDebug() << "FINISH: Tracks sorted by Name";
    mDataManager->mAllSessionTrack.clear();
    QMapIterator<QString, SessionTrack*> sessionTrackIterator(sessionTrackSortMap);
    while (sessionTrackIterator.hasNext()) {
        sessionTrackIterator.next();
        SessionTrack* sessionTrack = sessionTrackIterator.value();
        mDataManager->insertSessionTrack(sessionTrack);
    }
    // save all Tracks with sorted Sessions
    mDataManager->saveSessionTrackToCache();
    qDebug() << "FINISH: Tracks saved";

    // Day: save all days with sorted Sessions
    mDataManager->saveDayToCache();
    qDebug() << "FINISH: Days saved";

    // Rooms: save all Rooms with sorted Sessions
    mDataManager->saveRoomToCache();
    qDebug() << "FINISH: Rooms saved";

    // SETTINGS update API
    mDataManager->mSettingsData->setApiVersion(mNewApi);
    mDataManager->mSettingsData->setLastUpdate(QDateTime::currentDateTime());
    //
    mProgressInfotext.append("\n").append(tr("All done"));
    emit progressInfo(mProgressInfotext);
    qDebug() << "FINISH: All Done";
    emit updateDone();
}

//  U T I L I T Y S  to manage Conference data

/**
 * Favorites are transient on Sessions
 * Sessions are read-only
 * So at startup the propertie will be set
 */
void DataUtil::setSessionFavorites()
{
    for (int i = 0; i < mDataManager->mAllFavorite.size(); ++i) {
        Favorite* favorite = (Favorite*) mDataManager->mAllFavorite.at(i);
        Session* session = mDataManager->findSessionBySessionId(favorite->sessionId());
        if(session != NULL) {
            session->setIsFavorite(true);
        }
    }
}

/**
 * Favorites are transient on Sessions
 * Sessions are read-only
 * So while caching data Favorites will be created
 */
void DataUtil::saveSessionFavorites()
{
    mDataManager->mAllFavorite.clear();
    for (int i = 0; i < mDataManager->mAllSession.size(); ++i) {
        Session* session = (Session*) mDataManager->mAllSession.at(i);
        if(session->isFavorite()) {
            Favorite* favorite = mDataManager->createFavorite();
            favorite->setSessionId(session->sessionId());
            mDataManager->insertFavorite(favorite);
        }
    }
}

/**
 * list of sessions for a Day are lazy
 * (only IDs stored in an Array)
 * for the Conference we always need all to create the schedule
 * so at startup or update this will be called
 */
void DataUtil::resolveSessionsForSchedule() {
    for (int i = 0; i < mDataManager->mAllDay.size(); ++i) {
        Day* day = (Day*) mDataManager->mAllDay.at(i);
        day->resolveSessionsKeys(mDataManager->listOfSessionForKeys(day->sessionsKeys()));
    }
}

/**
 * list of sessions for a Track are lazy
 * (only IDs stored in an Array)
 * this will be called as soon as Tracks are first time displayed
 */
void DataUtil::resolveSessionsForTracks()
{
    for (int i = 0; i < mDataManager->mAllSessionTrack.size(); ++i) {
        SessionTrack* sessionTrack = (SessionTrack*) mDataManager->mAllSessionTrack.at(i);
        sessionTrack->resolveSessionsKeys(mDataManager->listOfSessionForKeys(sessionTrack->sessionsKeys()));
    }
}

void DataUtil::resolveSessionsForRooms()
{
    for (int i = 0; i < mDataManager->mAllRoom.size(); ++i) {
        Room* room = (Room*) mDataManager->mAllRoom.at(i);
        room->resolveSessionsKeys(mDataManager->listOfSessionForKeys(room->sessionsKeys()));
    }
}

void DataUtil::resolveSessionsForSpeaker(Speaker* speaker)
{
    speaker->resolveSessionsKeys(mDataManager->listOfSessionForKeys(speaker->sessionsKeys()));
}

QString DataUtil::scheduleTabName(int tabBarIndex)
{
    if(mDataManager->mAllDay.size()<(tabBarIndex +1)) {
        return "??";
    }
    Day* day = (Day*) mDataManager->mAllDay.at(tabBarIndex);
    //return day->conferenceDay().toString("ddd (dd)");
    return QDate::shortDayName(day->conferenceDay().dayOfWeek());
}

SessionLists *DataUtil::mySchedule()
{
    return mSessionLists;
}

void DataUtil::refreshMySchedule()
{
    mSessionLists->clearScheduledSessions();
    for (int i = 0; i < mDataManager->allSession().size(); ++i) {
        Session* session = (Session*) mDataManager->allSession().at(i);
        if(!session->isDeprecated() && session->isFavorite()) {
            mSessionLists->addToScheduledSessions(session);
        }
    }
    qDebug() << "MY SCHEDLUE #:" << mSessionLists->scheduledSessionsCount();
    emit myScheduleRefreshed();
}

int DataUtil::findFirstSessionItem(int conferenceDayIndex, QString pickedTime)
{
    if(conferenceDayIndex < 0 || conferenceDayIndex > (mDataManager->mAllDay.size()-1)) {
        qDebug() << "Day Index wrong: conferenceDayIndex";
        return -1;
    }
    Day* day = (Day*) mDataManager->mAllDay.at(conferenceDayIndex);
    for (int i = 0; i < day->sessions().size(); ++i) {
        Session* session = day->sessions().at(i);
        QString theTime = session->sortKey().right(5);
        if(theTime >= pickedTime) {
            return i;
        }
    }
    return day->sessions().size();
}

int DataUtil::findFirstSpeakerItem(QString letter)
{
    for (int i = 0; i < mDataManager->mAllSpeaker.size(); ++i) {
        Speaker* speaker = (Speaker*) mDataManager->mAllSpeaker.at(i);
        if(speaker->sortGroup() >= letter) {
            return i;
        }
    }
    return mDataManager->mAllSpeaker.size();
}

// Sortkey: day->conferenceDay().toString(YYYY_MM_DD)+session->startTime().toString("HH:mm")
QString DataUtil::localWeekdayAndTime(QString sessionSortkey)
{
    QDateTime sortTime = QDateTime::fromString(sessionSortkey, YYYY_MM_DD_HH_MM);
    QString weekDayAndTime = sortTime.toString(DAYNAME);
    weekDayAndTime.append(", ");
    QTime myTime = QTime::fromString(sessionSortkey.right(5));
    weekDayAndTime.append(myTime.toString(Qt::SystemLocaleShortDate));
    return weekDayAndTime;
}

QString DataUtil::apiInfo()
{
    QString apiInfo = tr("API Version ");
    apiInfo.append(mDataManager->settingsData()->apiVersion());
    apiInfo.append(("\n")).append(tr("Last Update "));
    if(mDataManager->mSettingsData->hasLastUpdate()) {
        apiInfo.append(mDataManager->mSettingsData->lastUpdate().toString(Qt::SystemLocaleShortDate));
    } else {
        apiInfo.append("n/a");
    }
    return apiInfo;
}


//   DOWNLOAD   S P E A K E R I M A G E S
// SLOT   UPDATE CONFERENCE
void DataUtil::onSpeakerImageUpdateLoaded(QObject *dataObject, int width, int height)
{
    mImageLoader->deleteLater();
    SpeakerImage* speakerImage = (SpeakerImage*) dataObject;
    qDebug() << "onSpeakerImage  L O A D E D ";
    speakerImage->setDownloadSuccess(true);
    speakerImage->setDownloadFailed(false);
    speakerImage->setInAssets(false);
    speakerImage->setInData(true);
    prepareHighDpiImages(speakerImage, width, height);
    // set update flag
    int count = mMultiSpeakerImages.remove(false, speakerImage);
    if(count != 1) {
        qWarning() << "something went wrong: the SpeakerImage MUST exist in MultiMap";
    }
    mMultiSpeakerImages.insert(true, speakerImage);
    // check for more
    updateSpeakerImages();
}
void DataUtil::onSpeakerImageUpdateFailed(QObject *dataObject, QString message) {
    mImageLoader->deleteLater();
    SpeakerImage* speakerImage = (SpeakerImage*) dataObject;
    qDebug() << "UPDATE: Cannot load Speaker Image:  " << message << speakerImage->speakerId();
    speakerImage->setDownloadSuccess(false);
    speakerImage->setDownloadFailed(true);
    speakerImage->setInAssets(false);
    speakerImage->setInData(false);
    // set update flag
    int count = mMultiSpeakerImages.remove(false, speakerImage);
    if(count != 1) {
        qWarning() << "something went wrong: the SpeakerImage MUST exist in MultiMap";
    }
    mMultiSpeakerImages.insert(true, speakerImage);
    // check for more
    updateSpeakerImages();
}


// SLOT  PREPARE CONFERENCE
void DataUtil::onSpeakerImageLoaded(QObject *dataObject, int width, int height)
{
    mImageLoader->deleteLater();
    SpeakerImage* speakerImage = (SpeakerImage*) dataObject;
    qDebug() << "loaded..." << speakerImage->speakerId() << " " << width << "x" << height;
    speakerImage->setDownloadSuccess(true);
    speakerImage->setDownloadFailed(false);
    speakerImage->setInAssets(true);
    speakerImage->setInData(false);
    prepareHighDpiImages(speakerImage, width, height);
    // more to load ?
    prepareSpeakerImages();
}

void DataUtil::onSpeakerImageFailed(QObject *dataObject, QString message) {
    mImageLoader->deleteLater();
    SpeakerImage* speakerImage = (SpeakerImage*) dataObject;
    qDebug() << "PREPARE: Cannot load Speaker Image:  " << message << speakerImage->speakerId();
    speakerImage->setDownloadSuccess(false);
    speakerImage->setDownloadFailed(true);
    speakerImage->setInAssets(false);
    speakerImage->setInData(false);
    // more to load ?
    prepareSpeakerImages();
}

void DataUtil::prepareHighDpiImages(SpeakerImage* speakerImage, int width, int height) {
    const QString speakerImagesPath = mConferenceDataPath + "speakerImages/";
    QString fileName;
    fileName = speakerImagesPath + "speaker_";
    fileName.append(QString::number(speakerImage->speakerId()));
    QString originFileName;
    originFileName = fileName + "." + speakerImage->suffix();
    const int size1 = 64;
    const int size2 = 128;
    const int size3 = 192;
    const int size4 = 256;
    if(width >= height) {
        if(width < size1) {
            speakerImage->setMaxScaleFactor(0);
            return;
        }
        QFile originFile(originFileName);
        if(!originFile.exists()) {
            qWarning() << "SpeakerImage Path not found " << originFileName;
            return;
        }
        if (!originFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Couldn't open file: " << originFileName;
            return;
        }
        QImage originImage = QImage::fromData(originFile.readAll());
        if(originImage.isNull()) {
            qWarning() << "Cannot construct Image from file: " << originFileName;
            return;
        }
        QImage scaledImage;
        if(width >= size1) {
            scaledImage = originImage.scaledToWidth(size1);
            scaledImage.save(originFileName);
            speakerImage->setMaxScaleFactor(1);
        }
        if(width >= size2) {
            scaledImage = originImage.scaledToWidth(size2);
            scaledImage.save(fileName+"@2x."+speakerImage->suffix());
            speakerImage->setMaxScaleFactor(2);
        }
        if(width >= size3) {
            scaledImage = originImage.scaledToWidth(size3);
            scaledImage.save(fileName+"@3x."+speakerImage->suffix());
            speakerImage->setMaxScaleFactor(3);
        }
        if(width >= size4) {
            scaledImage = originImage.scaledToWidth(size4);
            scaledImage.save(fileName+"@4x."+speakerImage->suffix());
            speakerImage->setMaxScaleFactor(4);
        }
    } else {
        if(height < size1) {
            speakerImage->setMaxScaleFactor(0);
            return;
        }
        QFile originFile(originFileName);
        if(!originFile.exists()) {
            qWarning() << "SpeakerImage Path not found " << originFileName;
            return;
        }
        if (!originFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Couldn't open file: " << originFileName;
            return;
        }
        QImage originImage = QImage::fromData(originFile.readAll());
        if(originImage.isNull()) {
            qWarning() << "Cannot construct Image from file: " << originFileName;
            return;
        }
        QImage scaledImage;
        if(height >= size1) {
            scaledImage = originImage.scaledToHeight(size1);
            scaledImage.save(originFileName);
            speakerImage->setMaxScaleFactor(1);
        }
        if(height >= size2) {
            scaledImage = originImage.scaledToHeight(size2);
            scaledImage.save(fileName+"@2x."+speakerImage->suffix());
            speakerImage->setMaxScaleFactor(2);
        }
        if(height >= size3) {
            scaledImage = originImage.scaledToHeight(size3);
            scaledImage.save(fileName+"@3x."+speakerImage->suffix());
            speakerImage->setMaxScaleFactor(3);
        }
        if(height >= size4) {
            scaledImage = originImage.scaledToHeight(size4);
            scaledImage.save(fileName+"@4x."+speakerImage->suffix());
            speakerImage->setMaxScaleFactor(4);
        }
    }
}

// S L O T S
void DataUtil::onServerSuccess()
{
    qDebug() << "S U C C E S S";

    const QString schedulePath = mConferenceDataPath + "schedule.json";
    qDebug() << "CHECK FOR UPDATE SESSIONS ";
    QVariantMap map;
    map = readScheduleFile(schedulePath);

    if(map.isEmpty()) {
        qWarning() << "Schedule is no Map";
        emit checkForUpdateFailed(tr("Error: Received Map is empty."));
        return;
    }
    map = map.value("schedule").toMap();
    if(map.isEmpty()) {
        qWarning() << "No 'schedule' found";
        emit checkForUpdateFailed(tr("Error: Received Map missed 'schedule'."));
        return;
    }

    // now do the real work
    continueUpdate();

}

void DataUtil::onVersionSuccess(QByteArray currentVersionBytes)
{
    QJsonDocument jda;
    jda = QJsonDocument::fromJson(currentVersionBytes);
    if(!jda.isObject()) {
        onVersionFailed(tr("No JSON Object received for Version Check"));
        return;
    }
    QVariantMap versionMap = jda.toVariant().toMap();
    if(versionMap.isEmpty()) {
        onVersionFailed(tr("Version Check: received Version Map is empty"));
        return;
    }
    if(!versionMap.contains("version")) {
        onVersionFailed(tr("Version Check: received Version Map missed Version Property"));
        return;
    }
    mNewApi = versionMap.value("version").toString();
    if(mNewApi.isEmpty()) {
        onVersionFailed(tr("Error: Received Map missed 'version'."));
        return;
    }
    qDebug() << "VERSION: " + mNewApi;

    QStringList versionList;
    versionList = mNewApi.split(".");
    if(versionList.size() != 2) {
        emit onVersionFailed(tr("Error: 'Version' wrong: ")+mNewApi);
        return;
    }

    if(mDataManager->mSettingsData->apiVersion().length() == 0) {
        emit updateAvailable(mNewApi);
        return;
    }
    QStringList oldVersionList;
    oldVersionList = mDataManager->mSettingsData->apiVersion().split(".");
    if(oldVersionList.size() != 2) {
        emit updateAvailable(mNewApi);
        return;
    }

    int oldValue = oldVersionList.at(0).toInt();
    int newValue = versionList.at(0).toInt();
    if(oldValue > newValue) {
        emit noUpdateRequired();
        return;
    }
    oldValue = oldVersionList.at(0).toInt();
    newValue = versionList.at(0).toInt();
    if(oldValue < newValue) {
        emit updateAvailable(mNewApi);
        return;
    }
    oldValue = oldVersionList.at(1).toInt();
    newValue = versionList.at(1).toInt();
    if(oldValue <  newValue) {
        emit updateAvailable(mNewApi);
        return;
    }
    emit noUpdateRequired();
}

void DataUtil::onVersionFailed(const QString message)
{
    qDebug() << "Version failed" << message;
    emit checkForUpdateFailed(message);
}

void DataUtil::onServerFailed(QString message)
{
    qDebug() << "FAILED: " << message;
    emit updateFailed(message);
}

