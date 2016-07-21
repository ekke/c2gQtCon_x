#ifndef SPEAKERIMAGE_HPP_
#define SPEAKERIMAGE_HPP_

#include <QObject>
#include <qvariant.h>




class SpeakerImage: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int speakerId READ speakerId WRITE setSpeakerId NOTIFY speakerIdChanged FINAL)
	Q_PROPERTY(QString originImageUrl READ originImageUrl WRITE setOriginImageUrl NOTIFY originImageUrlChanged FINAL)
	Q_PROPERTY(bool downloadSuccess READ downloadSuccess WRITE setDownloadSuccess NOTIFY downloadSuccessChanged FINAL)
	Q_PROPERTY(bool downloadFailed READ downloadFailed WRITE setDownloadFailed NOTIFY downloadFailedChanged FINAL)
	Q_PROPERTY(bool inAssets READ inAssets WRITE setInAssets NOTIFY inAssetsChanged FINAL)
	Q_PROPERTY(bool inData READ inData WRITE setInData NOTIFY inDataChanged FINAL)
	Q_PROPERTY(QString suffix READ suffix WRITE setSuffix NOTIFY suffixChanged FINAL)
	Q_PROPERTY(int maxScaleFactor READ maxScaleFactor WRITE setMaxScaleFactor NOTIFY maxScaleFactorChanged FINAL)


public:
	SpeakerImage(QObject *parent = 0);


	void fillFromMap(const QVariantMap& speakerImageMap);
	void fillFromForeignMap(const QVariantMap& speakerImageMap);
	void fillFromCacheMap(const QVariantMap& speakerImageMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int speakerId() const;
	void setSpeakerId(int speakerId);
	QString originImageUrl() const;
	void setOriginImageUrl(QString originImageUrl);
	bool downloadSuccess() const;
	void setDownloadSuccess(bool downloadSuccess);
	bool downloadFailed() const;
	void setDownloadFailed(bool downloadFailed);
	bool inAssets() const;
	void setInAssets(bool inAssets);
	bool inData() const;
	void setInData(bool inData);
	QString suffix() const;
	void setSuffix(QString suffix);
	int maxScaleFactor() const;
	void setMaxScaleFactor(int maxScaleFactor);



	virtual ~SpeakerImage();

	Q_SIGNALS:

	void speakerIdChanged(int speakerId);
	void originImageUrlChanged(QString originImageUrl);
	void downloadSuccessChanged(bool downloadSuccess);
	void downloadFailedChanged(bool downloadFailed);
	void inAssetsChanged(bool inAssets);
	void inDataChanged(bool inData);
	void suffixChanged(QString suffix);
	void maxScaleFactorChanged(int maxScaleFactor);
	

private:

	int mSpeakerId;
	QString mOriginImageUrl;
	bool mDownloadSuccess;
	bool mDownloadFailed;
	bool mInAssets;
	bool mInData;
	QString mSuffix;
	int mMaxScaleFactor;

	Q_DISABLE_COPY (SpeakerImage)
};
Q_DECLARE_METATYPE(SpeakerImage*)

#endif /* SPEAKERIMAGE_HPP_ */

