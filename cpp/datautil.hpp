#ifndef DATAUTIL_H
#define DATAUTIL_H

#include <QObject>
#include "gen/DataManager.hpp"
#include "imageloader.hpp"

class DataUtil : public QObject
{
    Q_OBJECT
public:
    explicit DataUtil(QObject *parent = 0);

    void init(DataManager *dataManager);

    Q_INVOKABLE
    void prepareConference();

    Q_INVOKABLE
    void prepareSessions();

    Q_INVOKABLE
    void prepareSpeaker();

    Q_INVOKABLE
    void prepareSpeakerImages();

signals:

public slots:
    void onSpeakerImageLoaded(QObject* dataObject, int width, int height);

private:

    DataManager* mDataManager;

    ImageLoader* mImageLoader;

};

#endif // DATAUTIL_H
