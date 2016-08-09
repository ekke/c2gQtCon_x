#include "dataserver.hpp"

#include <QDebug>

DataServer::DataServer(QObject *parent) : QObject(parent)
{
    //
}

DataServer::~DataServer()
{
    //
}

/**
 * init - called from starting the APP
 */
void DataServer::init(DataManager *dataManager)
{
    mDataManager = dataManager;
    qDebug() << "Data Server INIT done";
}

void DataServer::setConferenceDataPath(const QString &conferenceDataPath)
{
    mConferenceDataPath = conferenceDataPath;
    qDebug() << "Conference Data path: " << mConferenceDataPath;
}
