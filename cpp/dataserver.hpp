#ifndef DATASERVER_H
#define DATASERVER_H

#include <QObject>

#include "gen/DataManager.hpp"

#include <QUrl>
#include <QDebug>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>
#include <qfile.h>

class DataServer : public QObject
{
    Q_OBJECT
public:
    explicit DataServer(QObject *parent = 0);
    virtual ~DataServer();

    void init(DataManager* dataManager);
    void setConferenceDataPath(const QString& conferenceDataPath);

    void requestSchedule();
    void requestVersion();

signals:
    void versionSuccess(QByteArray currentVersion);
    void versionFailed(QString message);

    void serverSuccess();
    void serverFailed(QString message);

public slots:

private slots:
    // REST
    void onFinishedSchedule();
    void onFinishedSpeaker();

    void onFinishedVersion();

private:

    DataManager* mDataManager;
    QString mConferenceDataPath;

    // REST
    QNetworkAccessManager* mNetworkAccessManager;
    void requestSpeaker();

};

#endif // DATASERVER_H
