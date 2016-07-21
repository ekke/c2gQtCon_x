#ifndef DATAUTIL_H
#define DATAUTIL_H

#include <QObject>
#include "gen/DataManager.hpp"

class DataUtil : public QObject
{
    Q_OBJECT
public:
    explicit DataUtil(QObject *parent = 0);

    void init(DataManager *dataManager);

    Q_INVOKABLE
    void prepareConference();

signals:

public slots:

private:

    DataManager* mDataManager;

};

#endif // DATAUTIL_H
