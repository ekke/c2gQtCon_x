#ifndef FLOOR_HPP_
#define FLOOR_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QStringList>


// forward declaration (target references to this)
class Building;
// forward declaration (target references to this)
class Room;


class Floor: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int floorId READ floorId WRITE setFloorId NOTIFY floorIdChanged FINAL)
	Q_PROPERTY(QString floorName READ floorName WRITE setFloorName NOTIFY floorNameChanged FINAL)
	Q_PROPERTY(bool inAssets READ inAssets WRITE setInAssets NOTIFY inAssetsChanged FINAL)
	// building lazy pointing to Building* (domainKey: buildingId)
	Q_PROPERTY(int building READ building WRITE setBuilding NOTIFY buildingChanged FINAL)
	Q_PROPERTY(Building* buildingAsDataObject READ buildingAsDataObject WRITE resolveBuildingAsDataObject NOTIFY buildingAsDataObjectChanged FINAL)

	// QQmlListProperty to get easy access from QML
	Q_PROPERTY(QQmlListProperty<Room> roomsPropertyList READ roomsPropertyList NOTIFY roomsPropertyListChanged)

public:
	Floor(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& floorMap);
	void fillFromForeignMap(const QVariantMap& floorMap);
	void fillFromCacheMap(const QVariantMap& floorMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int floorId() const;
	void setFloorId(int floorId);
	QString floorName() const;
	void setFloorName(QString floorName);
	bool inAssets() const;
	void setInAssets(bool inAssets);
	// building lazy pointing to Building* (domainKey: buildingId)
	int building() const;
	void setBuilding(int building);
	Building* buildingAsDataObject() const;
	
	Q_INVOKABLE
	void resolveBuildingAsDataObject(Building* building);
	
	Q_INVOKABLE
	void removeBuilding();
	
	Q_INVOKABLE
	bool hasBuilding();
	
	Q_INVOKABLE
	bool isBuildingResolvedAsDataObject();
	
	Q_INVOKABLE
	void markBuildingAsInvalid();
	

	
	Q_INVOKABLE
	QVariantList roomsAsQVariantList();
	
	Q_INVOKABLE
	QVariantList roomsAsForeignQVariantList();

	
	Q_INVOKABLE
	void addToRooms(Room* room);
	
	Q_INVOKABLE
	bool removeFromRooms(Room* room);

	Q_INVOKABLE
	void clearRooms();

	// lazy Array of independent Data Objects: only keys are persisted
	Q_INVOKABLE
	bool areRoomsKeysResolved();

	Q_INVOKABLE
	QStringList roomsKeys();

	Q_INVOKABLE
	void resolveRoomsKeys(QList<Room*> rooms);
	
	Q_INVOKABLE
	int roomsCount();
	
	 // access from C++ to rooms
	QList<Room*> rooms();
	void setRooms(QList<Room*> rooms);
	// access from QML to rooms
	QQmlListProperty<Room> roomsPropertyList();


	virtual ~Floor();

	Q_SIGNALS:

	void floorIdChanged(int floorId);
	void floorNameChanged(QString floorName);
	void inAssetsChanged(bool inAssets);
	// building lazy pointing to Building* (domainKey: buildingId)
	void buildingChanged(int building);
	void buildingAsDataObjectChanged(Building* building);
	void roomsChanged(QList<Room*> rooms);
	void addedToRooms(Room* room);
	void roomsPropertyListChanged();
	
	

private:

	int mFloorId;
	QString mFloorName;
	bool mInAssets;
	int mBuilding;
	bool mBuildingInvalid;
	Building* mBuildingAsDataObject;
	// lazy Array of independent Data Objects: only keys are persisted
	QStringList mRoomsKeys;
	bool mRoomsKeysResolved;
	QList<Room*> mRooms;
	// implementation for QQmlListProperty to use
	// QML functions for List of Room*
	static void appendToRoomsProperty(QQmlListProperty<Room> *roomsList,
		Room* room);
	static int roomsPropertyCount(QQmlListProperty<Room> *roomsList);
	static Room* atRoomsProperty(QQmlListProperty<Room> *roomsList, int pos);
	static void clearRoomsProperty(QQmlListProperty<Room> *roomsList);
	

	Q_DISABLE_COPY (Floor)
};
Q_DECLARE_METATYPE(Floor*)

#endif /* FLOOR_HPP_ */

