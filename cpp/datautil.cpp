#include "datautil.hpp"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>

DataUtil::DataUtil(QObject *parent) : QObject(parent)
{

}

void DataUtil::init(DataManager* dataManager)
{
    mDataManager = dataManager;
}

// P R E   C O N F E R E N C E   S T U F F
/**
 * Prepare was done before submitting the App to AppStore
 * Use public Cache
 * If all done send files from Phone to PC/Mac
 * Copy them into data-assets /prod or /test
 * Then users will get this directly by installing the app
 * and updates are fast
 */
void DataUtil::prepareConference() {
    // must use public cache
    if(!mDataManager->settingsData()->hasPublicCache()) {
        qWarning() << "Preparation must be done from public cache";
        return;
    }
    qDebug() << "PREPARE CONFERENCE ";
    // check dirs for pre-conference stuff
    // these directories never used for normal app-runtime
    // data/conference
    QString directory = mDataManager->mDataPath + "conference/";
    QDir myDir;
    bool exists;
    exists = myDir.exists(directory);
    if (!exists) {
        bool ok = myDir.mkpath(directory);
        if(!ok) {
            qWarning() << "Couldn't create conference dir " << directory;
            return;
        }
        qDebug() << "created directory conference " << directory;
    }
    // data/conference/speakerImages
    directory.append("speakerImages/");
    exists = myDir.exists(directory);
    if (!exists) {
        bool ok = myDir.mkpath(directory);
        if(!ok) {
            qWarning() << "Couldn't create speakerImages dir " << directory;
            return;
        }
        qDebug() << "created directory speakerImages " << directory;
    }
    // prepare sessions
    prepareSessions();
    // prepare speaker
    prepareSpeaker();
    // download speaker images
    prepareSpeakerImages();

}

void DataUtil::prepareSessions()
{
    QString schedulePath = mDataManager->mDataAssetsPath + "conference/schedule.json";
    qDebug() << "PREPARE SESSIONS ";
    QVariantList dataList;

    QFile readFile(schedulePath);
    if(!readFile.exists()) {
        qWarning() << "Schedule Path not found " << schedulePath;
        return;
    }
    if (!readFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file: " << schedulePath;
        return;
    }
    QJsonDocument jda = QJsonDocument::fromJson(readFile.readAll());
    readFile.close();
    if(!jda.isObject()) {
        qWarning() << "Couldn't create JSON from file: " << schedulePath;
        return;
    }
    qDebug() << "QJsonDocument for schedule with Object :)";
    // TODO
}

void DataUtil::prepareSpeaker()
{
    QString speakersPath = mDataManager->mDataAssetsPath + "conference/speakers.json";
    qDebug() << "PREPARE SPEAKER ";
    QVariantList dataList;

    QFile readFile(speakersPath);
    if(!readFile.exists()) {
        qWarning() << "Speakers Path not found " << speakersPath;
        return;
    }
    if (!readFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file: " << speakersPath;
        return;
    }
    QJsonDocument jda = QJsonDocument::fromJson(readFile.readAll());
    readFile.close();
    if(!jda.isArray()) {
        qWarning() << "Couldn't create JSON from file: " << speakersPath;
        return;
    }
    qDebug() << "QJsonDocument for speakers with Array :)";
    dataList = jda.toVariant().toList();
    const QString DEFAULT_IMAGE = "http://conf.qtcon.org/person_original.png";
    for (int i = 0; i < dataList.size(); ++i) {
        SpeakerAPI* speakerAPI = mDataManager->createSpeakerAPI();
        speakerAPI->fillFromForeignMap(dataList.at(i).toMap());

        Speaker* speaker = mDataManager->createSpeaker();
        speaker->setSpeakerId(speakerAPI->id());
        speaker->setBio(speakerAPI->bio());
        speaker->setName(speakerAPI->firstName());
        if(speaker->name().length() > 0) {
            speaker->setName(speaker->name()+" ");
        }
        speaker->setName(speaker->name()+speakerAPI->lastName());
        speaker->setSortKey(speaker->name().left(5).toUpper());
        speaker->setSortGroup(speaker->sortKey().left(1));
        if(speakerAPI->avatar().length() > 0 && speakerAPI->avatar() != DEFAULT_IMAGE) {
            QString avatar = speakerAPI->avatar();
            QStringList sl = avatar.split("?");
            if(sl.size() > 1) {
                sl.removeLast();
                avatar = sl.join("?");
            }
            sl = avatar.split(".");
            if(sl.size() < 2) {
                qWarning() << "AVATAR wrong "+speakerAPI->avatar();
            } else {
                SpeakerImage* speakerImage = mDataManager->createSpeakerImage();
                speakerImage->setSpeakerId(speaker->speakerId());
                speakerImage->setOriginImageUrl(avatar.replace("http://","https://"));
                speakerImage->setSuffix(sl.last());
                mDataManager->insertSpeakerImage(speakerImage);
                speaker->resolveSpeakerImageAsDataObject(speakerImage);
            }
        }
        mDataManager->insertSpeaker(speaker);
    } // end for
    qDebug() << "cache SPEAKER";
    mDataManager->saveSpeakerToCache();
}

void DataUtil::prepareSpeakerImages()
{
    QString speakerImagesPath = mDataManager->mDataPath + "conference/speakerImages/";
    qDebug() << "storing Speaker Images here: " << speakerImagesPath;
    if (mDataManager->allSpeakerImage().size() > 0) {
        SpeakerImage* speakerImage = (SpeakerImage*) mDataManager->mAllSpeakerImage.at(0);
        QString fileName;
        fileName = "speaker_";
        fileName.append(QString::number(speakerImage->speakerId()));
        fileName.append('.');
        fileName.append(speakerImage->suffix());
        mImageLoader = new ImageLoader(speakerImage->originImageUrl(), speakerImagesPath+fileName, this);
        bool res = connect(mImageLoader, SIGNAL(loaded(QObject*, int, int)), this,
                           SLOT(onSpeakerImageLoaded(QObject*, int, int)));
        if (!res) {
            Q_ASSERT(res);
        }
        mImageLoader->loadSpeaker(speakerImage);
    }
}

void DataUtil::onSpeakerImageLoaded(QObject *dataObject, int width, int height)
{
    mImageLoader->deleteLater();
    qDebug() << "onSpeakerImage  L O A D E D ";
    SpeakerImage* speakerImage = (SpeakerImage*) dataObject;
    speakerImage->setDownloadSuccess(true);
    speakerImage->setDownloadFailed(false);
    speakerImage->setInAssets(true);
    speakerImage->setInData(false);
    // more to load ?
    QString speakerImagesPath = mDataManager->mDataPath + "conference/speakerImages/";
    for (int i = 0; i < mDataManager->allSpeakerImage().size(); ++i) {
        SpeakerImage* speakerImage = (SpeakerImage*) mDataManager->allSpeakerImage().at(i);
        if (!speakerImage->downloadSuccess() && !speakerImage->downloadFailed()) {
            qDebug() << "loading..." << speakerImage->speakerId() << " " << width << "x" << height;
            QString fileName;
            fileName = "speaker_";
            fileName.append(QString::number(speakerImage->speakerId()));
            fileName.append('.');
            fileName.append(speakerImage->suffix());
            mImageLoader = new ImageLoader(speakerImage->originImageUrl(), speakerImagesPath+fileName, this);
            bool res = connect(mImageLoader, SIGNAL(loaded(QObject*, int, int)), this,
                               SLOT(onSpeakerImageLoaded(QObject*, int, int)));
            if (!res) {
                Q_ASSERT(res);
            }
            mImageLoader->loadSpeaker(speakerImage);
            return;
        }
    } // for all speaker images
    qDebug() << "cache SPEAKER IMAGES";
    mDataManager->saveSpeakerImageToCache();
}
