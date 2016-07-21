#include "SpeakerImage.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString speakerIdKey = "speakerId";
static const QString originImageUrlKey = "originImageUrl";
static const QString downloadSuccessKey = "downloadSuccess";
static const QString downloadFailedKey = "downloadFailed";
static const QString inAssetsKey = "inAssets";
static const QString inDataKey = "inData";
static const QString suffixKey = "suffix";
static const QString maxScaleFactorKey = "maxScaleFactor";

// keys used from Server API etc
static const QString speakerIdForeignKey = "speakerId";
static const QString originImageUrlForeignKey = "originImageUrl";
static const QString downloadSuccessForeignKey = "downloadSuccess";
static const QString downloadFailedForeignKey = "downloadFailed";
static const QString inAssetsForeignKey = "inAssets";
static const QString inDataForeignKey = "inData";
static const QString suffixForeignKey = "suffix";
static const QString maxScaleFactorForeignKey = "maxScaleFactor";

/*
 * Default Constructor if SpeakerImage not initialized from QVariantMap
 */
SpeakerImage::SpeakerImage(QObject *parent) :
        QObject(parent), mSpeakerId(-1), mOriginImageUrl(""), mDownloadSuccess(false), mDownloadFailed(false), mInAssets(false), mInData(false), mSuffix(""), mMaxScaleFactor(0)
{
}

/*
 * initialize SpeakerImage from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void SpeakerImage::fillFromMap(const QVariantMap& speakerImageMap)
{
	mSpeakerId = speakerImageMap.value(speakerIdKey).toInt();
	mOriginImageUrl = speakerImageMap.value(originImageUrlKey).toString();
	mDownloadSuccess = speakerImageMap.value(downloadSuccessKey).toBool();
	mDownloadFailed = speakerImageMap.value(downloadFailedKey).toBool();
	mInAssets = speakerImageMap.value(inAssetsKey).toBool();
	mInData = speakerImageMap.value(inDataKey).toBool();
	mSuffix = speakerImageMap.value(suffixKey).toString();
	mMaxScaleFactor = speakerImageMap.value(maxScaleFactorKey).toInt();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void SpeakerImage::fillFromForeignMap(const QVariantMap& speakerImageMap)
{
	mSpeakerId = speakerImageMap.value(speakerIdForeignKey).toInt();
	mOriginImageUrl = speakerImageMap.value(originImageUrlForeignKey).toString();
	mDownloadSuccess = speakerImageMap.value(downloadSuccessForeignKey).toBool();
	mDownloadFailed = speakerImageMap.value(downloadFailedForeignKey).toBool();
	mInAssets = speakerImageMap.value(inAssetsForeignKey).toBool();
	mInData = speakerImageMap.value(inDataForeignKey).toBool();
	mSuffix = speakerImageMap.value(suffixForeignKey).toString();
	mMaxScaleFactor = speakerImageMap.value(maxScaleFactorForeignKey).toInt();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void SpeakerImage::fillFromCacheMap(const QVariantMap& speakerImageMap)
{
	mSpeakerId = speakerImageMap.value(speakerIdKey).toInt();
	mOriginImageUrl = speakerImageMap.value(originImageUrlKey).toString();
	mDownloadSuccess = speakerImageMap.value(downloadSuccessKey).toBool();
	mDownloadFailed = speakerImageMap.value(downloadFailedKey).toBool();
	mInAssets = speakerImageMap.value(inAssetsKey).toBool();
	mInData = speakerImageMap.value(inDataKey).toBool();
	mSuffix = speakerImageMap.value(suffixKey).toString();
	mMaxScaleFactor = speakerImageMap.value(maxScaleFactorKey).toInt();
}

void SpeakerImage::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool SpeakerImage::isValid()
{
	if (mSpeakerId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from SpeakerImage as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap SpeakerImage::toMap()
{
	QVariantMap speakerImageMap;
	speakerImageMap.insert(speakerIdKey, mSpeakerId);
	speakerImageMap.insert(originImageUrlKey, mOriginImageUrl);
	speakerImageMap.insert(downloadSuccessKey, mDownloadSuccess);
	speakerImageMap.insert(downloadFailedKey, mDownloadFailed);
	speakerImageMap.insert(inAssetsKey, mInAssets);
	speakerImageMap.insert(inDataKey, mInData);
	speakerImageMap.insert(suffixKey, mSuffix);
	speakerImageMap.insert(maxScaleFactorKey, mMaxScaleFactor);
	return speakerImageMap;
}

/*
 * Exports Properties from SpeakerImage as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap SpeakerImage::toForeignMap()
{
	QVariantMap speakerImageMap;
	speakerImageMap.insert(speakerIdForeignKey, mSpeakerId);
	speakerImageMap.insert(originImageUrlForeignKey, mOriginImageUrl);
	speakerImageMap.insert(downloadSuccessForeignKey, mDownloadSuccess);
	speakerImageMap.insert(downloadFailedForeignKey, mDownloadFailed);
	speakerImageMap.insert(inAssetsForeignKey, mInAssets);
	speakerImageMap.insert(inDataForeignKey, mInData);
	speakerImageMap.insert(suffixForeignKey, mSuffix);
	speakerImageMap.insert(maxScaleFactorForeignKey, mMaxScaleFactor);
	return speakerImageMap;
}


/*
 * Exports Properties from SpeakerImage as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap SpeakerImage::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: speakerId
// Domain KEY: speakerId
int SpeakerImage::speakerId() const
{
	return mSpeakerId;
}

void SpeakerImage::setSpeakerId(int speakerId)
{
	if (speakerId != mSpeakerId) {
		mSpeakerId = speakerId;
		emit speakerIdChanged(speakerId);
	}
}
// ATT 
// Optional: originImageUrl
QString SpeakerImage::originImageUrl() const
{
	return mOriginImageUrl;
}

void SpeakerImage::setOriginImageUrl(QString originImageUrl)
{
	if (originImageUrl != mOriginImageUrl) {
		mOriginImageUrl = originImageUrl;
		emit originImageUrlChanged(originImageUrl);
	}
}
// ATT 
// Optional: downloadSuccess
bool SpeakerImage::downloadSuccess() const
{
	return mDownloadSuccess;
}

void SpeakerImage::setDownloadSuccess(bool downloadSuccess)
{
	if (downloadSuccess != mDownloadSuccess) {
		mDownloadSuccess = downloadSuccess;
		emit downloadSuccessChanged(downloadSuccess);
	}
}
// ATT 
// Optional: downloadFailed
bool SpeakerImage::downloadFailed() const
{
	return mDownloadFailed;
}

void SpeakerImage::setDownloadFailed(bool downloadFailed)
{
	if (downloadFailed != mDownloadFailed) {
		mDownloadFailed = downloadFailed;
		emit downloadFailedChanged(downloadFailed);
	}
}
// ATT 
// Optional: inAssets
bool SpeakerImage::inAssets() const
{
	return mInAssets;
}

void SpeakerImage::setInAssets(bool inAssets)
{
	if (inAssets != mInAssets) {
		mInAssets = inAssets;
		emit inAssetsChanged(inAssets);
	}
}
// ATT 
// Optional: inData
bool SpeakerImage::inData() const
{
	return mInData;
}

void SpeakerImage::setInData(bool inData)
{
	if (inData != mInData) {
		mInData = inData;
		emit inDataChanged(inData);
	}
}
// ATT 
// Optional: suffix
QString SpeakerImage::suffix() const
{
	return mSuffix;
}

void SpeakerImage::setSuffix(QString suffix)
{
	if (suffix != mSuffix) {
		mSuffix = suffix;
		emit suffixChanged(suffix);
	}
}
// ATT 
// Optional: maxScaleFactor
int SpeakerImage::maxScaleFactor() const
{
	return mMaxScaleFactor;
}

void SpeakerImage::setMaxScaleFactor(int maxScaleFactor)
{
	if (maxScaleFactor != mMaxScaleFactor) {
		mMaxScaleFactor = maxScaleFactor;
		emit maxScaleFactorChanged(maxScaleFactor);
	}
}


SpeakerImage::~SpeakerImage()
{
	// place cleanUp code here
}
	
