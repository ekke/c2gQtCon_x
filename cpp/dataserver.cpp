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

    // workaround bug iOS - cannot reuse QNetworkAccessManager QTBUG-49751
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
    // workaround bug iOS - cannot reuse QNetworkAccessManager QTBUG-49751
    // otherwise accessibility not detected if switch off and on again
    QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager(this);
    if(networkAccessManager->networkAccessible() != QNetworkAccessManager::Accessible) {
        if(networkAccessManager->networkAccessible() == QNetworkAccessManager::NotAccessible) {
            qDebug() << "requestSchedule NO ACCESS TO NETWORK";
            emit serverFailed(tr("No Network Access"));
            return;
        }
        qDebug() << "requestSchedule NO ACCESS: The network accessibility cannot be determined.";
        emit serverFailed(tr("No Network Access"));
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

void DataServer::requestVersion()
{
    // workaround bug iOS - cannot reuse QNetworkAccessManager QTBUG-49751
    // otherwise accessibility not detected if switch off and on again
    QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager(this);
    if(networkAccessManager->networkAccessible() != QNetworkAccessManager::Accessible) {
        if(networkAccessManager->networkAccessible() == QNetworkAccessManager::NotAccessible) {
            qDebug() << "requestVersion NO ACCESS TO NETWORK";
            emit versionFailed(tr("No Network Access"));
            return;
        }
        qDebug() << "requestVersion NO ACCESS: The network accessibility cannot be determined.";
        emit versionFailed(tr("No Network Access"));
        return;
    }

    QString uri;
    uri = "https://conf.qtcon.org/en/qtcon/public/schedule/version.json";
    qDebug() << "requestVersion uri:" << uri;

    QNetworkRequest request(uri);

    // to avoid ssl errors:
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);

    QNetworkReply* reply = networkAccessManager->get(request);
    bool connectResult = connect(reply, SIGNAL(finished()), this, SLOT(onFinishedVersion()));
    Q_ASSERT(connectResult);
    Q_UNUSED(connectResult);
}

void DataServer::requestSpeaker()
{
    // workaround bug iOS - cannot reuse QNetworkAccessManager QTBUG-49751
    // otherwise accessibility not detected if switch off and on again
    QNetworkAccessManager* networkAccessManager = new QNetworkAccessManager(this);
    if(networkAccessManager->networkAccessible() != QNetworkAccessManager::Accessible) {
        if(networkAccessManager->networkAccessible() == QNetworkAccessManager::NotAccessible) {
            qDebug() << "requestSpeaker NO ACCESS TO NETWORK";
            emit serverFailed(tr("No Network Access"));
            return;
        }
        qDebug() << "requestSpeaker NO ACCESS: The network accessibility cannot be determined.";
        emit serverFailed(tr("No Network Access"));
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
        emit serverFailed(tr("No Network Reply"));
        return;
    }
    const qint64 available = reply->bytesAvailable();
    if(available == 0) {
        qWarning() << "Schedule: No Bytes received";
        emit serverFailed(tr("No Schedule Data received"));
        return;
    }
    int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "schedule HTTP STATUS: " << httpStatusCode << " Bytes: " << available;
    if(httpStatusCode != 200) {
        qDebug() << "Schedule Status Code not 200";
        emit serverFailed(tr("No sucess getting Schedule from Server. Got HTTP Status ")+QString::number(httpStatusCode));
        return;
    }
    QString scheduleFilePath = mConferenceDataPath+"schedule.json";
    QFile saveFile(scheduleFilePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file to write " << scheduleFilePath;
        emit serverFailed(tr("Schedule Data cannot be written"));
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
        emit serverFailed(tr("No Network Reply"));
        return;
    }
    const qint64 available = reply->bytesAvailable();
    if(available == 0) {
        qWarning() << "Speaker No Bytes received";
        emit serverFailed(tr("No Speaker Data received"));
        return;
    }
    int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Speaker HTTP STATUS: " << httpStatusCode << " Bytes: " << available;
    if(httpStatusCode != 200) {
        qDebug() << "Speaker Status Code not 200";
        emit serverFailed(tr("No sucess getting Speaker from Server. Got HTTP Status ")+QString::number(httpStatusCode));
        return;
    }
    QString speakerFilePath = mConferenceDataPath+"speaker.json";
    QFile saveFile(speakerFilePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file to write " << speakerFilePath;
        emit serverFailed(tr("Speaker Data cannot be written"));
        return;
    }
    qint64 bytesWritten = saveFile.write(reply->readAll());
    saveFile.close();
    qDebug() << "Data Bytes written: " << bytesWritten << " to: " << speakerFilePath;
    emit serverSuccess();
}

void DataServer::onFinishedVersion()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply) {
        qWarning() << "Version REPLY is NULL";
        emit versionFailed(tr("No Network Reply"));
        return;
    }
    const qint64 available = reply->bytesAvailable();
    if(available == 0) {
        qWarning() << "Version No Bytes received";
        emit versionFailed(tr("No Version Data received"));
        return;
    }
    int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Version HTTP STATUS: " << httpStatusCode << " Bytes: " << available;
    if(httpStatusCode != 200) {
        qDebug() << "Version Status Code not 200";
        emit versionFailed(tr("No sucess getting Version from Server. Got HTTP Status ")+QString::number(httpStatusCode));
        return;
    }
    emit versionSuccess(reply->readAll());
}


