#include "SettingsData.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString idKey = "id";
static const QString versionKey = "version";
static const QString apiVersionKey = "apiVersion";
static const QString lastUpdateKey = "lastUpdate";
static const QString isProductionEnvironmentKey = "isProductionEnvironment";
static const QString primaryColorKey = "primaryColor";
static const QString accentColorKey = "accentColor";
static const QString darkThemeKey = "darkTheme";
static const QString useMarkerColorsKey = "useMarkerColors";
static const QString defaultMarkerColorsKey = "defaultMarkerColors";
static const QString markerColorsKey = "markerColors";
static const QString hasPublicCacheKey = "hasPublicCache";
static const QString useCompactJsonFormatKey = "useCompactJsonFormat";
static const QString lastUsedNumberKey = "lastUsedNumber";
static const QString publicRoot4DevKey = "publicRoot4Dev";
static const QString autoUpdateKey = "autoUpdate";
static const QString autoUpdateEveryHoursKey = "autoUpdateEveryHours";
static const QString lastUpdateStampKey = "lastUpdateStamp";
static const QString navigationStyleKey = "navigationStyle";
static const QString oneMenuButtonKey = "oneMenuButton";
static const QString classicStackNavigationKey = "classicStackNavigation";

// keys used from Server API etc
static const QString idForeignKey = "id";
static const QString versionForeignKey = "version";
static const QString apiVersionForeignKey = "apiVersion";
static const QString lastUpdateForeignKey = "lastUpdate";
static const QString isProductionEnvironmentForeignKey = "isProductionEnvironment";
static const QString primaryColorForeignKey = "primaryColor";
static const QString accentColorForeignKey = "accentColor";
static const QString darkThemeForeignKey = "darkTheme";
static const QString useMarkerColorsForeignKey = "useMarkerColors";
static const QString defaultMarkerColorsForeignKey = "defaultMarkerColors";
static const QString markerColorsForeignKey = "markerColors";
static const QString hasPublicCacheForeignKey = "hasPublicCache";
static const QString useCompactJsonFormatForeignKey = "useCompactJsonFormat";
static const QString lastUsedNumberForeignKey = "lastUsedNumber";
static const QString publicRoot4DevForeignKey = "publicRoot4Dev";
static const QString autoUpdateForeignKey = "autoUpdate";
static const QString autoUpdateEveryHoursForeignKey = "autoUpdateEveryHours";
static const QString lastUpdateStampForeignKey = "lastUpdateStamp";
static const QString navigationStyleForeignKey = "navigationStyle";
static const QString oneMenuButtonForeignKey = "oneMenuButton";
static const QString classicStackNavigationForeignKey = "classicStackNavigation";

/*
 * Default Constructor if SettingsData not initialized from QVariantMap
 */
SettingsData::SettingsData(QObject *parent) :
        QObject(parent), mId(-1), mVersion(0), mApiVersion(""), mIsProductionEnvironment(false), mPrimaryColor(0), mAccentColor(0), mDarkTheme(false), mUseMarkerColors(false), mDefaultMarkerColors(false), mMarkerColors(""), mHasPublicCache(false), mUseCompactJsonFormat(false), mLastUsedNumber(0), mPublicRoot4Dev(""), mAutoUpdate(false), mAutoUpdateEveryHours(0), mNavigationStyle(0), mOneMenuButton(false), mClassicStackNavigation(false)
{
	// Date, Time or Timestamp ? construct null value
	mLastUpdate = QDateTime();
	mLastUpdateStamp = QDateTime();
}

/*
 * initialize SettingsData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void SettingsData::fillFromMap(const QVariantMap& settingsDataMap)
{
	mId = settingsDataMap.value(idKey).toInt();
	mVersion = settingsDataMap.value(versionKey).toInt();
	mApiVersion = settingsDataMap.value(apiVersionKey).toString();
	if (settingsDataMap.contains(lastUpdateKey)) {
		// always getting the Date as a String (from server or JSON)
		QString lastUpdateAsString = settingsDataMap.value(lastUpdateKey).toString();
		mLastUpdate = QDateTime::fromString(lastUpdateAsString, Qt::ISODate);
		if (!mLastUpdate.isValid()) {
			mLastUpdate = QDateTime();
			qDebug() << "mLastUpdate is not valid for String: " << lastUpdateAsString;
		}
	}
	mIsProductionEnvironment = settingsDataMap.value(isProductionEnvironmentKey).toBool();
	mPrimaryColor = settingsDataMap.value(primaryColorKey).toInt();
	mAccentColor = settingsDataMap.value(accentColorKey).toInt();
	mDarkTheme = settingsDataMap.value(darkThemeKey).toBool();
	mUseMarkerColors = settingsDataMap.value(useMarkerColorsKey).toBool();
	mDefaultMarkerColors = settingsDataMap.value(defaultMarkerColorsKey).toBool();
	mMarkerColors = settingsDataMap.value(markerColorsKey).toString();
	mHasPublicCache = settingsDataMap.value(hasPublicCacheKey).toBool();
	mUseCompactJsonFormat = settingsDataMap.value(useCompactJsonFormatKey).toBool();
	mLastUsedNumber = settingsDataMap.value(lastUsedNumberKey).toInt();
	mPublicRoot4Dev = settingsDataMap.value(publicRoot4DevKey).toString();
	mAutoUpdate = settingsDataMap.value(autoUpdateKey).toBool();
	mAutoUpdateEveryHours = settingsDataMap.value(autoUpdateEveryHoursKey).toInt();
	if (settingsDataMap.contains(lastUpdateStampKey)) {
		// always getting the Date as a String (from server or JSON)
		QString lastUpdateStampAsString = settingsDataMap.value(lastUpdateStampKey).toString();
		mLastUpdateStamp = QDateTime::fromString(lastUpdateStampAsString, Qt::ISODate);
		if (!mLastUpdateStamp.isValid()) {
			mLastUpdateStamp = QDateTime();
			qDebug() << "mLastUpdateStamp is not valid for String: " << lastUpdateStampAsString;
		}
	}
	mNavigationStyle = settingsDataMap.value(navigationStyleKey).toInt();
	mOneMenuButton = settingsDataMap.value(oneMenuButtonKey).toBool();
	mClassicStackNavigation = settingsDataMap.value(classicStackNavigationKey).toBool();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void SettingsData::fillFromForeignMap(const QVariantMap& settingsDataMap)
{
	mId = settingsDataMap.value(idForeignKey).toInt();
	mVersion = settingsDataMap.value(versionForeignKey).toInt();
	mApiVersion = settingsDataMap.value(apiVersionForeignKey).toString();
	if (settingsDataMap.contains(lastUpdateForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString lastUpdateAsString = settingsDataMap.value(lastUpdateForeignKey).toString();
		mLastUpdate = QDateTime::fromString(lastUpdateAsString, Qt::ISODate);
		if (!mLastUpdate.isValid()) {
			mLastUpdate = QDateTime();
			qDebug() << "mLastUpdate is not valid for String: " << lastUpdateAsString;
		}
	}
	mIsProductionEnvironment = settingsDataMap.value(isProductionEnvironmentForeignKey).toBool();
	mPrimaryColor = settingsDataMap.value(primaryColorForeignKey).toInt();
	mAccentColor = settingsDataMap.value(accentColorForeignKey).toInt();
	mDarkTheme = settingsDataMap.value(darkThemeForeignKey).toBool();
	mUseMarkerColors = settingsDataMap.value(useMarkerColorsForeignKey).toBool();
	mDefaultMarkerColors = settingsDataMap.value(defaultMarkerColorsForeignKey).toBool();
	mMarkerColors = settingsDataMap.value(markerColorsForeignKey).toString();
	mHasPublicCache = settingsDataMap.value(hasPublicCacheForeignKey).toBool();
	mUseCompactJsonFormat = settingsDataMap.value(useCompactJsonFormatForeignKey).toBool();
	mLastUsedNumber = settingsDataMap.value(lastUsedNumberForeignKey).toInt();
	mPublicRoot4Dev = settingsDataMap.value(publicRoot4DevForeignKey).toString();
	mAutoUpdate = settingsDataMap.value(autoUpdateForeignKey).toBool();
	mAutoUpdateEveryHours = settingsDataMap.value(autoUpdateEveryHoursForeignKey).toInt();
	if (settingsDataMap.contains(lastUpdateStampForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString lastUpdateStampAsString = settingsDataMap.value(lastUpdateStampForeignKey).toString();
		mLastUpdateStamp = QDateTime::fromString(lastUpdateStampAsString, Qt::ISODate);
		if (!mLastUpdateStamp.isValid()) {
			mLastUpdateStamp = QDateTime();
			qDebug() << "mLastUpdateStamp is not valid for String: " << lastUpdateStampAsString;
		}
	}
	mNavigationStyle = settingsDataMap.value(navigationStyleForeignKey).toInt();
	mOneMenuButton = settingsDataMap.value(oneMenuButtonForeignKey).toBool();
	mClassicStackNavigation = settingsDataMap.value(classicStackNavigationForeignKey).toBool();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void SettingsData::fillFromCacheMap(const QVariantMap& settingsDataMap)
{
	mId = settingsDataMap.value(idKey).toInt();
	mVersion = settingsDataMap.value(versionKey).toInt();
	mApiVersion = settingsDataMap.value(apiVersionKey).toString();
	if (settingsDataMap.contains(lastUpdateKey)) {
		// always getting the Date as a String (from server or JSON)
		QString lastUpdateAsString = settingsDataMap.value(lastUpdateKey).toString();
		mLastUpdate = QDateTime::fromString(lastUpdateAsString, Qt::ISODate);
		if (!mLastUpdate.isValid()) {
			mLastUpdate = QDateTime();
			qDebug() << "mLastUpdate is not valid for String: " << lastUpdateAsString;
		}
	}
	mIsProductionEnvironment = settingsDataMap.value(isProductionEnvironmentKey).toBool();
	mPrimaryColor = settingsDataMap.value(primaryColorKey).toInt();
	mAccentColor = settingsDataMap.value(accentColorKey).toInt();
	mDarkTheme = settingsDataMap.value(darkThemeKey).toBool();
	mUseMarkerColors = settingsDataMap.value(useMarkerColorsKey).toBool();
	mDefaultMarkerColors = settingsDataMap.value(defaultMarkerColorsKey).toBool();
	mMarkerColors = settingsDataMap.value(markerColorsKey).toString();
	mHasPublicCache = settingsDataMap.value(hasPublicCacheKey).toBool();
	mUseCompactJsonFormat = settingsDataMap.value(useCompactJsonFormatKey).toBool();
	mLastUsedNumber = settingsDataMap.value(lastUsedNumberKey).toInt();
	mPublicRoot4Dev = settingsDataMap.value(publicRoot4DevKey).toString();
	mAutoUpdate = settingsDataMap.value(autoUpdateKey).toBool();
	mAutoUpdateEveryHours = settingsDataMap.value(autoUpdateEveryHoursKey).toInt();
	if (settingsDataMap.contains(lastUpdateStampKey)) {
		// always getting the Date as a String (from server or JSON)
		QString lastUpdateStampAsString = settingsDataMap.value(lastUpdateStampKey).toString();
		mLastUpdateStamp = QDateTime::fromString(lastUpdateStampAsString, Qt::ISODate);
		if (!mLastUpdateStamp.isValid()) {
			mLastUpdateStamp = QDateTime();
			qDebug() << "mLastUpdateStamp is not valid for String: " << lastUpdateStampAsString;
		}
	}
	mNavigationStyle = settingsDataMap.value(navigationStyleKey).toInt();
	mOneMenuButton = settingsDataMap.value(oneMenuButtonKey).toBool();
	mClassicStackNavigation = settingsDataMap.value(classicStackNavigationKey).toBool();
}

void SettingsData::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool SettingsData::isValid()
{
	if (mId == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from SettingsData as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap SettingsData::toMap()
{
	QVariantMap settingsDataMap;
	settingsDataMap.insert(idKey, mId);
	settingsDataMap.insert(versionKey, mVersion);
	settingsDataMap.insert(apiVersionKey, mApiVersion);
	if (hasLastUpdate()) {
		settingsDataMap.insert(lastUpdateKey, mLastUpdate.toString(Qt::ISODate));
	}
	settingsDataMap.insert(isProductionEnvironmentKey, mIsProductionEnvironment);
	settingsDataMap.insert(primaryColorKey, mPrimaryColor);
	settingsDataMap.insert(accentColorKey, mAccentColor);
	settingsDataMap.insert(darkThemeKey, mDarkTheme);
	settingsDataMap.insert(useMarkerColorsKey, mUseMarkerColors);
	settingsDataMap.insert(defaultMarkerColorsKey, mDefaultMarkerColors);
	settingsDataMap.insert(markerColorsKey, mMarkerColors);
	settingsDataMap.insert(hasPublicCacheKey, mHasPublicCache);
	settingsDataMap.insert(useCompactJsonFormatKey, mUseCompactJsonFormat);
	settingsDataMap.insert(lastUsedNumberKey, mLastUsedNumber);
	settingsDataMap.insert(publicRoot4DevKey, mPublicRoot4Dev);
	settingsDataMap.insert(autoUpdateKey, mAutoUpdate);
	settingsDataMap.insert(autoUpdateEveryHoursKey, mAutoUpdateEveryHours);
	if (hasLastUpdateStamp()) {
		settingsDataMap.insert(lastUpdateStampKey, mLastUpdateStamp.toString(Qt::ISODate));
	}
	settingsDataMap.insert(navigationStyleKey, mNavigationStyle);
	settingsDataMap.insert(oneMenuButtonKey, mOneMenuButton);
	settingsDataMap.insert(classicStackNavigationKey, mClassicStackNavigation);
	return settingsDataMap;
}

/*
 * Exports Properties from SettingsData as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap SettingsData::toForeignMap()
{
	QVariantMap settingsDataMap;
	settingsDataMap.insert(idForeignKey, mId);
	settingsDataMap.insert(versionForeignKey, mVersion);
	settingsDataMap.insert(apiVersionForeignKey, mApiVersion);
	if (hasLastUpdate()) {
		settingsDataMap.insert(lastUpdateForeignKey, mLastUpdate.toString(Qt::ISODate));
	}
	settingsDataMap.insert(isProductionEnvironmentForeignKey, mIsProductionEnvironment);
	settingsDataMap.insert(primaryColorForeignKey, mPrimaryColor);
	settingsDataMap.insert(accentColorForeignKey, mAccentColor);
	settingsDataMap.insert(darkThemeForeignKey, mDarkTheme);
	settingsDataMap.insert(useMarkerColorsForeignKey, mUseMarkerColors);
	settingsDataMap.insert(defaultMarkerColorsForeignKey, mDefaultMarkerColors);
	settingsDataMap.insert(markerColorsForeignKey, mMarkerColors);
	settingsDataMap.insert(hasPublicCacheForeignKey, mHasPublicCache);
	settingsDataMap.insert(useCompactJsonFormatForeignKey, mUseCompactJsonFormat);
	settingsDataMap.insert(lastUsedNumberForeignKey, mLastUsedNumber);
	settingsDataMap.insert(publicRoot4DevForeignKey, mPublicRoot4Dev);
	settingsDataMap.insert(autoUpdateForeignKey, mAutoUpdate);
	settingsDataMap.insert(autoUpdateEveryHoursForeignKey, mAutoUpdateEveryHours);
	if (hasLastUpdateStamp()) {
		settingsDataMap.insert(lastUpdateStampForeignKey, mLastUpdateStamp.toString(Qt::ISODate));
	}
	settingsDataMap.insert(navigationStyleForeignKey, mNavigationStyle);
	settingsDataMap.insert(oneMenuButtonForeignKey, mOneMenuButton);
	settingsDataMap.insert(classicStackNavigationForeignKey, mClassicStackNavigation);
	return settingsDataMap;
}


/*
 * Exports Properties from SettingsData as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap SettingsData::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: id
// Domain KEY: id
int SettingsData::id() const
{
	return mId;
}

void SettingsData::setId(int id)
{
	if (id != mId) {
		mId = id;
		emit idChanged(id);
	}
}
// ATT 
// Optional: version
int SettingsData::version() const
{
	return mVersion;
}

void SettingsData::setVersion(int version)
{
	if (version != mVersion) {
		mVersion = version;
		emit versionChanged(version);
	}
}
// ATT 
// Optional: apiVersion
QString SettingsData::apiVersion() const
{
	return mApiVersion;
}

void SettingsData::setApiVersion(QString apiVersion)
{
	if (apiVersion != mApiVersion) {
		mApiVersion = apiVersion;
		emit apiVersionChanged(apiVersion);
	}
}
// ATT 
// Optional: lastUpdate
QDateTime SettingsData::lastUpdate() const
{
	return mLastUpdate;
}

void SettingsData::setLastUpdate(QDateTime lastUpdate)
{
	if (lastUpdate != mLastUpdate) {
		mLastUpdate = lastUpdate;
		emit lastUpdateChanged(lastUpdate);
	}
}
bool SettingsData::hasLastUpdate()
{
	return !mLastUpdate.isNull() && mLastUpdate.isValid();
}
// ATT 
// Optional: isProductionEnvironment
bool SettingsData::isProductionEnvironment() const
{
	return mIsProductionEnvironment;
}

void SettingsData::setIsProductionEnvironment(bool isProductionEnvironment)
{
	if (isProductionEnvironment != mIsProductionEnvironment) {
		mIsProductionEnvironment = isProductionEnvironment;
		emit isProductionEnvironmentChanged(isProductionEnvironment);
	}
}
// ATT 
// Optional: primaryColor
int SettingsData::primaryColor() const
{
	return mPrimaryColor;
}

void SettingsData::setPrimaryColor(int primaryColor)
{
	if (primaryColor != mPrimaryColor) {
		mPrimaryColor = primaryColor;
		emit primaryColorChanged(primaryColor);
	}
}
// ATT 
// Optional: accentColor
int SettingsData::accentColor() const
{
	return mAccentColor;
}

void SettingsData::setAccentColor(int accentColor)
{
	if (accentColor != mAccentColor) {
		mAccentColor = accentColor;
		emit accentColorChanged(accentColor);
	}
}
// ATT 
// Optional: darkTheme
bool SettingsData::darkTheme() const
{
	return mDarkTheme;
}

void SettingsData::setDarkTheme(bool darkTheme)
{
	if (darkTheme != mDarkTheme) {
		mDarkTheme = darkTheme;
		emit darkThemeChanged(darkTheme);
	}
}
// ATT 
// Optional: useMarkerColors
bool SettingsData::useMarkerColors() const
{
	return mUseMarkerColors;
}

void SettingsData::setUseMarkerColors(bool useMarkerColors)
{
	if (useMarkerColors != mUseMarkerColors) {
		mUseMarkerColors = useMarkerColors;
		emit useMarkerColorsChanged(useMarkerColors);
	}
}
// ATT 
// Optional: defaultMarkerColors
bool SettingsData::defaultMarkerColors() const
{
	return mDefaultMarkerColors;
}

void SettingsData::setDefaultMarkerColors(bool defaultMarkerColors)
{
	if (defaultMarkerColors != mDefaultMarkerColors) {
		mDefaultMarkerColors = defaultMarkerColors;
		emit defaultMarkerColorsChanged(defaultMarkerColors);
	}
}
// ATT 
// Optional: markerColors
QString SettingsData::markerColors() const
{
	return mMarkerColors;
}

void SettingsData::setMarkerColors(QString markerColors)
{
	if (markerColors != mMarkerColors) {
		mMarkerColors = markerColors;
		emit markerColorsChanged(markerColors);
	}
}
// ATT 
// Optional: hasPublicCache
bool SettingsData::hasPublicCache() const
{
	return mHasPublicCache;
}

void SettingsData::setHasPublicCache(bool hasPublicCache)
{
	if (hasPublicCache != mHasPublicCache) {
		mHasPublicCache = hasPublicCache;
		emit hasPublicCacheChanged(hasPublicCache);
	}
}
// ATT 
// Optional: useCompactJsonFormat
bool SettingsData::useCompactJsonFormat() const
{
	return mUseCompactJsonFormat;
}

void SettingsData::setUseCompactJsonFormat(bool useCompactJsonFormat)
{
	if (useCompactJsonFormat != mUseCompactJsonFormat) {
		mUseCompactJsonFormat = useCompactJsonFormat;
		emit useCompactJsonFormatChanged(useCompactJsonFormat);
	}
}
// ATT 
// Optional: lastUsedNumber
int SettingsData::lastUsedNumber() const
{
	return mLastUsedNumber;
}

void SettingsData::setLastUsedNumber(int lastUsedNumber)
{
	if (lastUsedNumber != mLastUsedNumber) {
		mLastUsedNumber = lastUsedNumber;
		emit lastUsedNumberChanged(lastUsedNumber);
	}
}
// ATT 
// Optional: publicRoot4Dev
QString SettingsData::publicRoot4Dev() const
{
	return mPublicRoot4Dev;
}

void SettingsData::setPublicRoot4Dev(QString publicRoot4Dev)
{
	if (publicRoot4Dev != mPublicRoot4Dev) {
		mPublicRoot4Dev = publicRoot4Dev;
		emit publicRoot4DevChanged(publicRoot4Dev);
	}
}
// ATT 
// Optional: autoUpdate
bool SettingsData::autoUpdate() const
{
	return mAutoUpdate;
}

void SettingsData::setAutoUpdate(bool autoUpdate)
{
	if (autoUpdate != mAutoUpdate) {
		mAutoUpdate = autoUpdate;
		emit autoUpdateChanged(autoUpdate);
	}
}
// ATT 
// Optional: autoUpdateEveryHours
int SettingsData::autoUpdateEveryHours() const
{
	return mAutoUpdateEveryHours;
}

void SettingsData::setAutoUpdateEveryHours(int autoUpdateEveryHours)
{
	if (autoUpdateEveryHours != mAutoUpdateEveryHours) {
		mAutoUpdateEveryHours = autoUpdateEveryHours;
		emit autoUpdateEveryHoursChanged(autoUpdateEveryHours);
	}
}
// ATT 
// Optional: lastUpdateStamp
QDateTime SettingsData::lastUpdateStamp() const
{
	return mLastUpdateStamp;
}

void SettingsData::setLastUpdateStamp(QDateTime lastUpdateStamp)
{
	if (lastUpdateStamp != mLastUpdateStamp) {
		mLastUpdateStamp = lastUpdateStamp;
		emit lastUpdateStampChanged(lastUpdateStamp);
	}
}
bool SettingsData::hasLastUpdateStamp()
{
	return !mLastUpdateStamp.isNull() && mLastUpdateStamp.isValid();
}
// ATT 
// Optional: navigationStyle
int SettingsData::navigationStyle() const
{
	return mNavigationStyle;
}

void SettingsData::setNavigationStyle(int navigationStyle)
{
	if (navigationStyle != mNavigationStyle) {
		mNavigationStyle = navigationStyle;
		emit navigationStyleChanged(navigationStyle);
	}
}
// ATT 
// Optional: oneMenuButton
bool SettingsData::oneMenuButton() const
{
	return mOneMenuButton;
}

void SettingsData::setOneMenuButton(bool oneMenuButton)
{
	if (oneMenuButton != mOneMenuButton) {
		mOneMenuButton = oneMenuButton;
		emit oneMenuButtonChanged(oneMenuButton);
	}
}
// ATT 
// Optional: classicStackNavigation
bool SettingsData::classicStackNavigation() const
{
	return mClassicStackNavigation;
}

void SettingsData::setClassicStackNavigation(bool classicStackNavigation)
{
	if (classicStackNavigation != mClassicStackNavigation) {
		mClassicStackNavigation = classicStackNavigation;
		emit classicStackNavigationChanged(classicStackNavigation);
	}
}


SettingsData::~SettingsData()
{
	// place cleanUp code here
}
	
