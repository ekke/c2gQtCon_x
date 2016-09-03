// ekke (Ekkehard Gentz) @ekkescorner
#ifndef APPLICATIONUI_HPP
#define APPLICATIONUI_HPP

#include <QObject>

#include <QtQml>

#include "gen/SettingsData.hpp"
#include "gen/DataManager.hpp"
#include "dataserver.hpp"
#include "datautil.hpp"

class ApplicationUI : public QObject
{
    Q_OBJECT

public:
     ApplicationUI(QObject *parent = 0);

     Q_INVOKABLE
     QStringList swapThemePalette();

     Q_INVOKABLE
     QStringList defaultThemePalette();

     Q_INVOKABLE
     QStringList primaryPalette(const int paletteIndex);

     Q_INVOKABLE
     QStringList accentPalette(const int paletteIndex);

     Q_INVOKABLE
     QStringList defaultPrimaryPalette();

     Q_INVOKABLE
     QStringList defaultAccentPalette();

     Q_INVOKABLE
     bool isDebugBuild();

     void addContextProperty(QQmlContext* context);

signals:

public slots:
     void onAboutToQuit();
     void onApplicationStateChanged(Qt::ApplicationState applicationState);

private:
     DataManager* mDataManager;
     DataUtil* mDataUtil;
     DataServer* mDataServer;

     SettingsData* mSettingsData;

     bool mCachingInWork;
     bool mCachingDone;
     void resetCaching();
     void startCaching();
     void doCaching();
};

#endif // APPLICATIONUI_HPP
