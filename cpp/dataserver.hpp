#ifndef DATASERVER_H
#define DATASERVER_H

#include <QObject>

#include "gen/DataManager.hpp"

class DataServer : public QObject
{
    Q_OBJECT
public:
    explicit DataServer(QObject *parent = 0);

    virtual ~DataServer();

    void init(DataManager* dataManager);

signals:

public slots:

private:

    DataManager* mDataManager;

};

#endif // DATASERVER_H
