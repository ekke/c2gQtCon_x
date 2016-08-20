#ifndef SETTINGSDATA_HPP_
#define SETTINGSDATA_HPP_

#include <QObject>
#include <qvariant.h>
#include <QDateTime>




class SettingsData: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
	Q_PROPERTY(int version READ version WRITE setVersion NOTIFY versionChanged FINAL)
	Q_PROPERTY(QString apiVersion READ apiVersion WRITE setApiVersion NOTIFY apiVersionChanged FINAL)
	Q_PROPERTY(QDateTime lastUpdate READ lastUpdate WRITE setLastUpdate NOTIFY lastUpdateChanged FINAL)
	Q_PROPERTY(bool isProductionEnvironment READ isProductionEnvironment WRITE setIsProductionEnvironment NOTIFY isProductionEnvironmentChanged FINAL)
	Q_PROPERTY(int primaryColor READ primaryColor WRITE setPrimaryColor NOTIFY primaryColorChanged FINAL)
	Q_PROPERTY(int accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged FINAL)
	Q_PROPERTY(bool darkTheme READ darkTheme WRITE setDarkTheme NOTIFY darkThemeChanged FINAL)
	Q_PROPERTY(bool useMarkerColors READ useMarkerColors WRITE setUseMarkerColors NOTIFY useMarkerColorsChanged FINAL)
	Q_PROPERTY(bool defaultMarkerColors READ defaultMarkerColors WRITE setDefaultMarkerColors NOTIFY defaultMarkerColorsChanged FINAL)
	Q_PROPERTY(QString markerColors READ markerColors WRITE setMarkerColors NOTIFY markerColorsChanged FINAL)
	Q_PROPERTY(bool hasPublicCache READ hasPublicCache WRITE setHasPublicCache NOTIFY hasPublicCacheChanged FINAL)
	Q_PROPERTY(bool useCompactJsonFormat READ useCompactJsonFormat WRITE setUseCompactJsonFormat NOTIFY useCompactJsonFormatChanged FINAL)
	Q_PROPERTY(int lastUsedNumber READ lastUsedNumber WRITE setLastUsedNumber NOTIFY lastUsedNumberChanged FINAL)
	Q_PROPERTY(QString publicRoot4Dev READ publicRoot4Dev WRITE setPublicRoot4Dev NOTIFY publicRoot4DevChanged FINAL)
	Q_PROPERTY(bool autoUpdate READ autoUpdate WRITE setAutoUpdate NOTIFY autoUpdateChanged FINAL)
	Q_PROPERTY(int autoUpdateEveryHours READ autoUpdateEveryHours WRITE setAutoUpdateEveryHours NOTIFY autoUpdateEveryHoursChanged FINAL)
	Q_PROPERTY(QDateTime lastUpdateStamp READ lastUpdateStamp WRITE setLastUpdateStamp NOTIFY lastUpdateStampChanged FINAL)
	Q_PROPERTY(int navigationStyle READ navigationStyle WRITE setNavigationStyle NOTIFY navigationStyleChanged FINAL)
	Q_PROPERTY(bool oneMenuButton READ oneMenuButton WRITE setOneMenuButton NOTIFY oneMenuButtonChanged FINAL)
	Q_PROPERTY(bool classicStackNavigation READ classicStackNavigation WRITE setClassicStackNavigation NOTIFY classicStackNavigationChanged FINAL)


public:
	SettingsData(QObject *parent = 0);


	void fillFromMap(const QVariantMap& settingsDataMap);
	void fillFromForeignMap(const QVariantMap& settingsDataMap);
	void fillFromCacheMap(const QVariantMap& settingsDataMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int id() const;
	void setId(int id);
	int version() const;
	void setVersion(int version);
	QString apiVersion() const;
	void setApiVersion(QString apiVersion);
	QDateTime lastUpdate() const;

	Q_INVOKABLE
	bool hasLastUpdate();
	void setLastUpdate(QDateTime lastUpdate);
	bool isProductionEnvironment() const;
	void setIsProductionEnvironment(bool isProductionEnvironment);
	int primaryColor() const;
	void setPrimaryColor(int primaryColor);
	int accentColor() const;
	void setAccentColor(int accentColor);
	bool darkTheme() const;
	void setDarkTheme(bool darkTheme);
	bool useMarkerColors() const;
	void setUseMarkerColors(bool useMarkerColors);
	bool defaultMarkerColors() const;
	void setDefaultMarkerColors(bool defaultMarkerColors);
	QString markerColors() const;
	void setMarkerColors(QString markerColors);
	bool hasPublicCache() const;
	void setHasPublicCache(bool hasPublicCache);
	bool useCompactJsonFormat() const;
	void setUseCompactJsonFormat(bool useCompactJsonFormat);
	int lastUsedNumber() const;
	void setLastUsedNumber(int lastUsedNumber);
	QString publicRoot4Dev() const;
	void setPublicRoot4Dev(QString publicRoot4Dev);
	bool autoUpdate() const;
	void setAutoUpdate(bool autoUpdate);
	int autoUpdateEveryHours() const;
	void setAutoUpdateEveryHours(int autoUpdateEveryHours);
	QDateTime lastUpdateStamp() const;

	Q_INVOKABLE
	bool hasLastUpdateStamp();
	void setLastUpdateStamp(QDateTime lastUpdateStamp);
	int navigationStyle() const;
	void setNavigationStyle(int navigationStyle);
	bool oneMenuButton() const;
	void setOneMenuButton(bool oneMenuButton);
	bool classicStackNavigation() const;
	void setClassicStackNavigation(bool classicStackNavigation);



	virtual ~SettingsData();

	Q_SIGNALS:

	void idChanged(int id);
	void versionChanged(int version);
	void apiVersionChanged(QString apiVersion);
	void lastUpdateChanged(QDateTime lastUpdate);
	void isProductionEnvironmentChanged(bool isProductionEnvironment);
	void primaryColorChanged(int primaryColor);
	void accentColorChanged(int accentColor);
	void darkThemeChanged(bool darkTheme);
	void useMarkerColorsChanged(bool useMarkerColors);
	void defaultMarkerColorsChanged(bool defaultMarkerColors);
	void markerColorsChanged(QString markerColors);
	void hasPublicCacheChanged(bool hasPublicCache);
	void useCompactJsonFormatChanged(bool useCompactJsonFormat);
	void lastUsedNumberChanged(int lastUsedNumber);
	void publicRoot4DevChanged(QString publicRoot4Dev);
	void autoUpdateChanged(bool autoUpdate);
	void autoUpdateEveryHoursChanged(int autoUpdateEveryHours);
	void lastUpdateStampChanged(QDateTime lastUpdateStamp);
	void navigationStyleChanged(int navigationStyle);
	void oneMenuButtonChanged(bool oneMenuButton);
	void classicStackNavigationChanged(bool classicStackNavigation);
	

private:

	int mId;
	int mVersion;
	QString mApiVersion;
	QDateTime mLastUpdate;
	bool mIsProductionEnvironment;
	int mPrimaryColor;
	int mAccentColor;
	bool mDarkTheme;
	bool mUseMarkerColors;
	bool mDefaultMarkerColors;
	QString mMarkerColors;
	bool mHasPublicCache;
	bool mUseCompactJsonFormat;
	int mLastUsedNumber;
	QString mPublicRoot4Dev;
	bool mAutoUpdate;
	int mAutoUpdateEveryHours;
	QDateTime mLastUpdateStamp;
	int mNavigationStyle;
	bool mOneMenuButton;
	bool mClassicStackNavigation;

	Q_DISABLE_COPY (SettingsData)
};
Q_DECLARE_METATYPE(SettingsData*)

#endif /* SETTINGSDATA_HPP_ */

