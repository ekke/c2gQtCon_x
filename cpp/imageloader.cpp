

#include "imageloader.hpp"

#include <QUrl>
#include <QDebug>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>
#include <QSslConfiguration>
#include <qfile.h>

/**
 *  This class implements a image loader which will initialize a network request in asynchronous manner.
 *  After receiving response from the network, it saves Image to /data.
 *  Then it signals the interested parties about the result.
 */
ImageLoader::ImageLoader(const QString &imageUrl, const QString &filename, QObject* parent) :
        QObject(parent), m_imageUrl(imageUrl), m_filename(filename)
{
}

ImageLoader::~ImageLoader()
{
}

/**
 * QObject* will be stored at request and sent back from response
 *
 */
void ImageLoader::loadSpeaker(QObject* dataObject)
{
    QNetworkAccessManager* netManager = new QNetworkAccessManager(this);

    const QUrl url(m_imageUrl);
    QNetworkRequest request(url);
    // stores the object so we can catch it later
    request.setOriginatingObject(dataObject);

    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);

    QNetworkReply* reply = netManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
}


/**
 * ImageLoader::onReplyFinished()
 *
 * Handler for the signal indicating the response for the previous network request.
 */
void ImageLoader::onReplyFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    QString response;
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                const QByteArray data(reply->readAll());

                int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                if(httpStatusCode == 301) {
                    qDebug() << "301: Content Moved Permanently: " << m_imageUrl;
                    qDebug() << "NOW: " << reply->header(QNetworkRequest::LocationHeader);
                    emit loaded(reply->request().originatingObject(), false);
                    return;
                }
                qDebug() << "HTTP Status: " << httpStatusCode << " " << m_filename;
                // write to file
                QFile dataFile(m_filename);
                dataFile.open(QIODevice::WriteOnly);
                dataFile.write(data);
                dataFile.close();
                emit loaded(reply->request().originatingObject(), true);
            }
        } else {
            qDebug() << "CANNOT write image " << m_filename << " error: " << reply->error() << reply->errorString();
            emit loaded(reply->request().originatingObject(), false);
        }

        reply->deleteLater();
    } else {
        // ????
    }
}
