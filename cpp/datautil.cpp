#include "datautil.hpp"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

DataUtil::DataUtil(QObject *parent) : QObject(parent)
{

}

void DataUtil::init(DataManager* dataManager)
{
    mDataManager = dataManager;
}

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

    // QString schedulePath = mDataManager->mDataAssetsPath + "conference/schedule.json";
    QString speakersPath = mDataManager->mDataAssetsPath + "conference/speakers.json";
    qDebug() << "PREPARE CONFERENCE ";
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
                speakerImage->setOriginImageUrl(avatar);
                speakerImage->setSuffix(sl.last());
                mDataManager->insertSpeakerImage(speakerImage);
                speaker->resolveSpeakerImageAsDataObject(speakerImage);
            }
        }
        mDataManager->insertSpeaker(speaker);
    } // end for
    mDataManager->saveSpeakerToCache();
    mDataManager->saveSpeakerImageToCache();
}
