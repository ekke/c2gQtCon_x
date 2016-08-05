

#include "imageloader.hpp"

#include <QUrl>
#include <QDebug>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>
#include <QSslConfiguration>
#include <qfile.h>
#include <QImage>

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

    // QtCon redirects Speaker Images to https://
    // to avoid ssl errors:
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

    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const int available = reply->bytesAvailable();
            if (available > 0) {
                int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                if(httpStatusCode == 200) {
                    QImage originImage = QImage::fromData(reply->readAll());
                    if(originImage.isNull()) {
                        emit loadingFailed(reply->request().originatingObject(), "Cannot construct Image from data: " + m_imageUrl);
                        reply->deleteLater();
                        return;
                    }
                    // using a QImage because downloaded speaker images can have different dpi - so we adjust this here-
                    // dpi = dpm * 0.0254   dpm = spi/0.0254
                    // we want to have 72 dpi (2835 dpm) images
                    if(originImage.dotsPerMeterX() > 2835) {
                        originImage.setDotsPerMeterX(2835);
                    }
                    if(originImage.dotsPerMeterY() > 2835) {
                        originImage.setDotsPerMeterY(2835);
                    }
                    originImage.save(m_filename);
    //                // this is normaly used for images without any modifications:
    //                QFile dataFile(m_filename);
    //                dataFile.open(QIODevice::WriteOnly);
    //                dataFile.write(data);
    //                dataFile.close();
                    emit loaded(reply->request().originatingObject(), originImage.width(), originImage.height());
                } else {
                    QString message;
                    if(httpStatusCode == 301) {
                        message = "redirected to "+reply->header(QNetworkRequest::LocationHeader).toString();

                    } else {
                        message = "Wrong HTTP Status: " + QString::number(httpStatusCode);
                    }
                    emit loadingFailed(reply->request().originatingObject(), message);
                }
            }
        } else {
            qDebug() << "ERROR loading image " << m_filename << " " << reply->error() << reply->errorString();
            emit loadingFailed(reply->request().originatingObject(), reply->errorString());
        }

        reply->deleteLater();
    }
}
