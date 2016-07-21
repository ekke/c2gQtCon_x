#ifndef BUILDING_HPP_
#define BUILDING_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>


// forward declaration (target references to this)
class Floor;


class Building: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int buildingId READ buildingId WRITE setBuildingId NOTIFY buildingIdChanged FINAL)
	Q_PROPERTY(QString buildingName READ buildingName WRITE setBuildingName NOTIFY buildingNameChanged FINAL)
	Q_PROPERTY(bool inAssets READ inAssets WRITE setInAssets NOTIFY inAssetsChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Floor> floorsPropertyList READ floorsPropertyList NOTIFY floorsPropertyListChanged)

public:
	Building(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& buildingMap);
	void fillFromForeignMap(const QVariantMap& buildingMap);
	void fillFromCacheMap(const QVariantMap& buildingMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int buildingId() const;
	void setBuildingId(int buildingId);
	QString buildingName() const;
	void setBuildingName(QString buildingName);
	bool inAssets() const;
	void setInAssets(bool inAssets);

	
	Q_INVOKABLE
	QVariantList floorsAsQVariantList();
	
	Q_INVOKABLE
	QVariantList floorsAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToFloors(Floor* floor);
	
	Q_INVOKABLE
	bool removeFromFloors(Floor* floor);

	Q_INVOKABLE
	void clearFloors();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areFloorsKeysResolved();

	Q_INVOKABLE
	QStringList floorsKeys();

	Q_INVOKABLE
	void resolveFloorsKeys(QList<Floor*> floors);
	
	Q_INVOKABLE
	int floorsCount();
	
	 // access from C++ to floors
	QList<Floor*> floors();
	void setFloors(QList<Floor*> floors);
	// access from QML to floors
	QQmlListProperty<Floor> floorsPropertyList();


	virtual ~Building();

	Q_SIGNALS:

	void buildingIdChanged(int buildingId);
	void buildingNameChanged(QString buildingName);
	void inAssetsChanged(bool inAssets);
	void floorsChanged(QList<Floor*> floors);
	void addedToFloors(Floor* floor);
	void floorsPropertyListChanged();
	
	

private:

	int mBuildingId;
	QString mBuildingName;
	bool mInAssets;
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mFloorsKeys;
	bool mFloorsKeysResolved;
	QList<Floor*> mFloors;
	// implementation for QQmlListProperty to use
	// QML functions for List of Floor*
	static void appendToFloorsProperty(QQmlListProperty<Floor> *floorsList,
		Floor* floor);
	static int floorsPropertyCount(QQmlListProperty<Floor> *floorsList);
	static Floor* atFloorsProperty(QQmlListProperty<Floor> *floorsList, int pos);
	static void clearFloorsProperty(QQmlListProperty<Floor> *floorsList);
	

	Q_DISABLE_COPY (Building)
};
Q_DECLARE_METATYPE(Building*)

#endif /* BUILDING_HPP_ */

