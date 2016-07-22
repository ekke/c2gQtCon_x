#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#include <QObject>
#include <QByteArray>


/*
 * This class retrieves an image from the web.
 */
class ImageLoader : public QObject
{
    Q_OBJECT


public:
    /*
     * Creates a new image loader.
     *
     * @param imageUrl The url to load the image from.
     * @param filename The full path with filename to store the image.
     */
    ImageLoader(const QString &imageUrl, const QString &filename, QObject* parent = 0);

    ~ImageLoader();

    void loadSpeaker(QObject* dataObject);

Q_SIGNALS:

    void loaded(QObject* dataObject, int width, int height);
    void loadingFailed(QObject* dataObject, QString message);

private Q_SLOTS:

    void onReplyFinished();


private:

    // The URL of the image that should be loaded
    QString m_imageUrl;

    // The full qualified filename (optional)
    QString m_filename;

};

#endif
