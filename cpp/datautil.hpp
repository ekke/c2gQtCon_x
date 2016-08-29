#ifndef DATAUTIL_H
#define DATAUTIL_H

#include <QObject>
#include "gen/DataManager.hpp"
#include "imageloader.hpp"
#include "dataserver.hpp"

class DataUtil : public QObject
{
    Q_OBJECT
public:
    explicit DataUtil(QObject *parent = 0);

    void init(DataManager *dataManager, DataServer* dataServer);

    Q_INVOKABLE
    void prepareConference();
    void prepareSessions();
    void prepareSpeaker();
    void prepareSpeakerImages();

    Q_INVOKABLE
    void checkVersion();

    Q_INVOKABLE
    void startUpdate();

    Q_INVOKABLE
    void setSessionFavorites();
    void saveSessionFavorites();

    Q_INVOKABLE
    void resolveSessionsForSchedule();

    Q_INVOKABLE
    void resolveSessionsForTracks();

    Q_INVOKABLE
    void resolveSessionsForRooms();

    Q_INVOKABLE
    void resolveSessionsForSpeaker(Speaker *speaker);

    Q_INVOKABLE
    QString scheduleTabName(int tabBarIndex);

    Q_INVOKABLE
    SessionLists* mySchedule();

    Q_INVOKABLE
    void refreshMySchedule();

    Q_INVOKABLE
    int findFirstSessionItem(int conferenceDayIndex, QString pickedTime);

    Q_INVOKABLE
    int findFirstSpeakerItem(QString letter);

    Q_INVOKABLE
    QString localWeekdayAndTime(QString sessionSortkey);

    Q_INVOKABLE
    QString apiInfo();

    Q_INVOKABLE
    void reloadData();

    // some helpers for QML UI
    Q_INVOKABLE
    QString conferenceDataPath4QML();

    Q_INVOKABLE
    bool isDateTooLate();

    Q_INVOKABLE
    QString sessionInfoForSpeaker(Speaker* speaker);

    Q_INVOKABLE
    QString speakerNamesForSession(Session* session);

    Q_INVOKABLE
    QString scheduleItemImageForSession(Session* session);

    Q_INVOKABLE
    QString letterForButton(Session* session);

    Q_INVOKABLE
    QString textForSessionTrack(Session* session);

    Q_INVOKABLE
    QString textForSessionType(Session* session);

    Q_INVOKABLE
    QString trackColor(const int trackId);

signals:
    void myScheduleRefreshed();

    void updateAvailable(const QString apiVersion);
    void noUpdateRequired();
    void checkForUpdateFailed(const QString message);

    void updateDone();
    void updateFailed(const QString message);

    void progressInfo(const QString progressInfo);

public slots:
    void onSpeakerImageLoaded(QObject* dataObject, int width, int height);

    void onSpeakerImageUpdateLoaded(QObject *dataObject, int width, int height);
    void onSpeakerImageFailed(QObject *dataObject, QString message);
    void onSpeakerImageUpdateFailed(QObject *dataObject, QString message);
private slots:
    void onServerSuccess();
    void onServerFailed(const QString message);

    void onVersionSuccess(QByteArray currentVersionBytes);
    void onVersionFailed(const QString message);

private:

    DataManager* mDataManager;
    DataServer* mDataServer;

    ImageLoader* mImageLoader;

    SessionLists* mSessionLists;

    void prepareHighDpiImages(SpeakerImage *speakerImage, int width, int height);

    void prepareEventData();

    bool checkDirs();
    QString mConferenceDataPath;

    // UPDATE
    void continueUpdate();
    QString mNewApi;
    QString mProgressInfotext;
    QMultiMap<QString, Session*> mMultiSession;
    QMultiMap<QString, Speaker*> mMultiSpeaker;
    QMultiMap<bool, SpeakerImage*> mMultiSpeakerImages;
    QVariantMap readScheduleFile(const QString schedulePath);
    QVariantList readSpeakerFile(const QString speakerPath);
    void calcSpeakerName(Speaker *speaker, SpeakerAPI *speakerAPI);
    void updateSpeakerImages();
    void updateSessions();
    Day *findDayForServerDate(const QString &dayDate);
    void adjustPersons(QVariantMap &sessionMap);
    bool checkIfIgnored(SessionAPI *sessionAPI);
    void setDuration(SessionAPI *sessionAPI, Session* session);
    void setTrackAndType(SessionAPI *sessionAPI, Session *session, Conference *conference, const bool isUpdate);
    void createAndAdjustLinks(QVariantMap &sessionMap);
    void updateAndAdjustLinks(QVariantMap &sessionMap);
    void finishUpdate();
    void sortedSessionsIntoRoomDayTrackSpeaker();
};

#endif // DATAUTIL_H
