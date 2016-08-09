#include "dataserver.hpp"

#include <QSslConfiguration>

DataServer::DataServer(QObject *parent) : QObject(parent)
{
    //
}

DataServer::~DataServer()
{
    //
}

/**
 * init - called from starting the APP
 */
void DataServer::init(DataManager *dataManager)
{
    mDataManager = dataManager;

    // workaround bug iOS - cannot reuse QNetworkAccessManager
    // otherwise accessibility not detected if switch off and on again
    // mNetworkAccessManager = new QNetworkAccessManager(this);

    qDebug() << "Data Server INIT done";
}

void DataServer::setConferenceDataPath(const QString &conferenceDataPath)
{
    mConferenceDataPath = conferenceDataPath;
    qDebug() << "Conference Data path: " << mConferenceDataPath;
}

void DataServer::requestSchedule()
{
    // workaround bug iOS - cannot reuse QNetworkAccessManager
    // otherwise accessibility not detected if switch off and on again
    QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager(this);
    if(networkAccessManager->networkAccessible() != QNetworkAccessManager::Accessible) {
        if(networkAccessManager->networkAccessible() == QNetworkAccessManager::NotAccessible) {
            qDebug() << "requestSchedule NO ACCESS TO NETWORK";
            return;
        }
        qDebug() << "requestSchedule NO ACCESS: The network accessibility cannot be determined.";
        return;
    }

    QString uri;
    uri = "https://conf.qtcon.org/en/qtcon/public/schedule.json";
    qDebug() << "requestSchedule uri:" << uri;

    QNetworkRequest request(uri);

    // to avoid ssl errors:
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);

    QNetworkReply* reply = networkAccessManager->get(request);
    bool connectResult = connect(reply, SIGNAL(finished()), this, SLOT(onFinishedSchedule()));
    Q_ASSERT(connectResult);
    Q_UNUSED(connectResult);

}

void DataServer::requestSpeaker()
{
    // workaround bug iOS - cannot reuse QNetworkAccessManager
    // otherwise accessibility not detected if switch off and on again
    QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager(this);
    if(networkAccessManager->networkAccessible() != QNetworkAccessManager::Accessible) {
        if(networkAccessManager->networkAccessible() == QNetworkAccessManager::NotAccessible) {
            qDebug() << "requestSpeaker NO ACCESS TO NETWORK";
            return;
        }
        qDebug() << "requestSpeaker NO ACCESS: The network accessibility cannot be determined.";
        return;
    }

    QString uri;
    uri = "https://conf.qtcon.org/en/qtcon/public/speakers.json";
    qDebug() << "requestSpeaker uri:" << uri;

    QNetworkRequest request(uri);

    // to avoid ssl errors:
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);

    QNetworkReply* reply = networkAccessManager->get(request);
    bool connectResult = connect(reply, SIGNAL(finished()), this, SLOT(onFinishedSpeaker()));
    Q_ASSERT(connectResult);
    Q_UNUSED(connectResult);
}

// SLOTS
void DataServer::onFinishedSchedule()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply) {
        qWarning() << "Schedule REPLY is NULL";
        return;
    }
    const int available = reply->bytesAvailable();
    if(available == 0) {
        qWarning() << "Schedule: No Bytes received";
        return;
    }
    int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "schedule HTTP STATUS: " << httpStatusCode << " Bytes: " << available;
    if(httpStatusCode != 200) {
        qDebug() << "Schedule Status Code not 200";
        return;
    }
    QString scheduleFilePath = mConferenceDataPath+"schedule.json";
    QFile saveFile(scheduleFilePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file to write " << scheduleFilePath;
        return;
    }
    qint64 bytesWritten = saveFile.write(reply->readAll());
    saveFile.close();
    qDebug() << "Schedule Data Bytes written: " << bytesWritten << " to: " << scheduleFilePath;
    // now getting the speaker data
    requestSpeaker();
}

void DataServer::onFinishedSpeaker()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply) {
        qWarning() << "Speaker REPLY is NULL";
        return;
    }
    const int available = reply->bytesAvailable();
    if(available == 0) {
        qWarning() << "Speaker No Bytes received";
        return;
    }
    int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Speaker HTTP STATUS: " << httpStatusCode << " Bytes: " << available;
    if(httpStatusCode != 200) {
        qDebug() << "Speaker Status Code not 200";
        return;
    }
    QString speakerFilePath = mConferenceDataPath+"speaker.json";
    QFile saveFile(speakerFilePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file to write " << speakerFilePath;
        return;
    }
    qint64 bytesWritten = saveFile.write(reply->readAll());
    saveFile.close();
    qDebug() << "Data Bytes written: " << bytesWritten << " to: " << speakerFilePath;
}


