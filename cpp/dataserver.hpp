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

signals:

public slots:

private slots:
    // REST
    void onFinishedSchedule();

private:

    DataManager* mDataManager;
    QString mConferenceDataPath;

    // REST
    QNetworkAccessManager* mNetworkAccessManager;

};

#endif // DATASERVER_H
