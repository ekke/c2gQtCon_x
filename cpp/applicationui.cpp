// ekke (Ekkehard Gentz) @ekkescorner
#include "applicationui.hpp"
#include "uiconstants.hpp"

#include <QtQml>
#include <QGuiApplication>

#include <QJsonObject>
#include <QFile>


#include <QDebug>

const QString settingsDataFile = "/settingsData.json";

using namespace ekke::constants;

ApplicationUI::ApplicationUI(QObject *parent) : QObject(parent), mDataManager(new DataManager(this)), mDataUtil(new DataUtil(this)), mDataServer(new DataServer(this))
{
    mSettingsData = mDataManager->settingsData();

    mDataServer->init(mDataManager);
    mDataUtil->init(mDataManager, mDataServer);

    mCachingDone = false;
    mCachingInWork = false;

}

void ApplicationUI::addContextProperty(QQmlContext *context)
{
    context->setContextProperty("dataManager", mDataManager);
    context->setContextProperty("dataUtil", mDataUtil);
}

/* Change Theme Palette */
QStringList ApplicationUI::swapThemePalette()
{
    mSettingsData->setDarkTheme(!mSettingsData->darkTheme());
    if (mSettingsData->darkTheme()) {
        return darkPalette;
    }
    return lightPalette;
}

/* Get current default Theme Palette */
QStringList ApplicationUI::defaultThemePalette()
{
    if (mSettingsData->darkTheme()) {
        return darkPalette;
    }
    return lightPalette;
}

/* Get one of the Primary Palettes */
QStringList ApplicationUI::primaryPalette(const int paletteIndex)
{
    mSettingsData->setPrimaryColor(paletteIndex);
    switch (paletteIndex) {
    case 0:
        return materialRed;
        break;
    case 1:
        return materialPink;
        break;
    case 2:
        return materialPurple;
        break;
    case 3:
        return materialDeepPurple;
        break;
    case 4:
        return materialIndigo;
        break;
    case 5:
        return materialBlue;
        break;
    case 6:
        return materialLightBlue;
        break;
    case 7:
        return materialCyan;
        break;
    case 8:
        return materialTeal;
        break;
    case 9:
        return materialGreen;
        break;
    case 10:
        return materialLightGreen;
        break;
    case 11:
        return materialLime;
        break;
    case 12:
        return materialYellow;
        break;
    case 13:
        return materialAmber;
        break;
    case 14:
        return materialOrange;
        break;
    case 15:
        return materialDeepOrange;
        break;
    case 16:
        return materialBrown;
        break;
    case 17:
        return materialGrey;
        break;
    default:
        return materialBlueGrey;
        break;
    }
}

/* Get one of the Accent Palettes */
QStringList ApplicationUI::accentPalette(const int paletteIndex)
{
    mSettingsData->setAccentColor(paletteIndex);
    int currentPrimary = mSettingsData->primaryColor();
    QStringList thePalette = primaryPalette(paletteIndex);
    mSettingsData->setPrimaryColor(currentPrimary);
    // we need: primaryColor, textOnPrimary, iconOnPrimaryFolder
    return QStringList{thePalette.at(1), thePalette.at(4), thePalette.at(7)};
}

/* Get Default Primary Palette */
QStringList ApplicationUI::defaultPrimaryPalette()
{
    return primaryPalette(mSettingsData->primaryColor());
}

/* Get Default Accent Palette */
QStringList ApplicationUI::defaultAccentPalette()
{
    return accentPalette(mSettingsData->accentColor());
}

bool ApplicationUI::isDebugBuild()
{
#ifdef QT_DEBUG
    return true;
#endif
    qDebug() << "Running a RELEASE build";
    return false;
}


// ATTENTION
// iOS: NO SIGNAL
// Android: SIGNAL if leaving the App with Android BACK Key
// Android: NO SIGNAL if using HOME or OVERVIEW and THEN CLOSE from there
void ApplicationUI::onAboutToQuit()
{
    qDebug() << "On About to Q U I T Signal received";
    startCaching();
}

void ApplicationUI::onApplicationStateChanged(Qt::ApplicationState applicationState)
{
    qDebug() << "S T A T E changed into: " << applicationState;
    if(applicationState == Qt::ApplicationState::ApplicationSuspended) {
        startCaching();
        return;
    }
    if(applicationState == Qt::ApplicationState::ApplicationActive) {
        resetCaching();
    }
}

void ApplicationUI::resetCaching()
{
    if(mCachingInWork) {
        qDebug() << "no reset caching - already in work";
        return;
    }
    qDebug() << "reset caching";
    mCachingDone = false;
}

void ApplicationUI::startCaching()
{
    if(mCachingInWork || mCachingDone) {
        qDebug() << "no start caching - already in work ? " << mCachingInWork << " done ? " << mCachingDone;
        return;
    }
    doCaching();
}

void ApplicationUI::doCaching()
{
    qDebug() << "DO Caching BEGIN";
    mCachingInWork = true;
    mCachingDone = false;

    mDataUtil->saveSessionFavorites();
    mDataManager->finish();

    mCachingInWork = false;
    mCachingDone = QGuiApplication::applicationState() != Qt::ApplicationState::ApplicationActive;
    qDebug() << "DO Caching END - Done ? " << mCachingDone;
}



