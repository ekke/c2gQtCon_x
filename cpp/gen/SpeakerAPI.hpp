#ifndef SPEAKERAPI_HPP_
#define SPEAKERAPI_HPP_

#include <QObject>
#include <qvariant.h>




class SpeakerAPI: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
	Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged FINAL)
	Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged FINAL)
	Q_PROPERTY(QString publicName READ publicName WRITE setPublicName NOTIFY publicNameChanged FINAL)
	Q_PROPERTY(QString bio READ bio WRITE setBio NOTIFY bioChanged FINAL)
	Q_PROPERTY(QString avatar READ avatar WRITE setAvatar NOTIFY avatarChanged FINAL)


public:
	SpeakerAPI(QObject *parent = 0);


	void fillFromMap(const QVariantMap& speakerAPIMap);
	void fillFromForeignMap(const QVariantMap& speakerAPIMap);
	void fillFromCacheMap(const QVariantMap& speakerAPIMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int id() const;
	void setId(int id);
	QString firstName() const;
	void setFirstName(QString firstName);
	QString lastName() const;
	void setLastName(QString lastName);
	QString publicName() const;
	void setPublicName(QString publicName);
	QString bio() const;
	void setBio(QString bio);
	QString avatar() const;
	void setAvatar(QString avatar);



	virtual ~SpeakerAPI();

	Q_SIGNALS:

	void idChanged(int id);
	void firstNameChanged(QString firstName);
	void lastNameChanged(QString lastName);
	void publicNameChanged(QString publicName);
	void bioChanged(QString bio);
	void avatarChanged(QString avatar);
	

private:

	int mId;
	QString mFirstName;
	QString mLastName;
	QString mPublicName;
	QString mBio;
	QString mAvatar;

	Q_DISABLE_COPY (SpeakerAPI)
};
Q_DECLARE_METATYPE(SpeakerAPI*)

#endif /* SPEAKERAPI_HPP_ */

