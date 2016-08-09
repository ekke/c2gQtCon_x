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

    Q_INVOKABLE
    void prepareSessions();

    Q_INVOKABLE
    void prepareSpeaker();

    Q_INVOKABLE
    void prepareSpeakerImages();

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

signals:
    void myScheduleRefreshed();

public slots:
    void onSpeakerImageLoaded(QObject* dataObject, int width, int height);

private:

    DataManager* mDataManager;
    DataServer* mDataServer;

    ImageLoader* mImageLoader;

    SessionLists* mSessionLists;

    void prepareHighDpiImages(SpeakerImage *speakerImage, int width, int height);

    void prepareEventData();
};

#endif // DATAUTIL_H
