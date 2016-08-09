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
    mNetworkAccessManager = new QNetworkAccessManager(this);
    qDebug() << "Data Server INIT done";
}

void DataServer::setConferenceDataPath(const QString &conferenceDataPath)
{
    mConferenceDataPath = conferenceDataPath;
    qDebug() << "Conference Data path: " << mConferenceDataPath;
}

void DataServer::requestSchedule()
{
    QString uri;
    uri = "https://conf.qtcon.org/en/qtcon/public/schedule.json";
    qDebug() << "R E S T uri:" << uri;

    QNetworkRequest request(uri);

    // to avoid ssl errors:
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);

    QNetworkReply* reply = mNetworkAccessManager->get(request);
    bool connectResult = connect(reply, SIGNAL(finished()), this, SLOT(onFinishedSchedule()));
    Q_ASSERT(connectResult);
    Q_UNUSED(connectResult);

}

// SLOT
void DataServer::onFinishedSchedule()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply) {
        qWarning() << "REPLY is NULL";
        return;
    }
    const int available = reply->bytesAvailable();
    if(available == 0) {
        qWarning() << "No Bytes received";
        return;
    }
    int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "HTTP STATUS: " << httpStatusCode << " Bytes: " << available;
    if(httpStatusCode != 200) {
        qDebug() << "Status Code not 200";
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
    qDebug() << "Data Bytes written: " << bytesWritten << " to: " << scheduleFilePath;
}
