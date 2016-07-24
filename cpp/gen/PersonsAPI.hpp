#ifndef PERSONSAPI_HPP_
#define PERSONSAPI_HPP_

#include <QObject>
#include <qvariant.h>




class PersonsAPI: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int speakerId READ speakerId WRITE setSpeakerId NOTIFY speakerIdChanged FINAL)


public:
	PersonsAPI(QObject *parent = 0);


	void fillFromMap(const QVariantMap& personsAPIMap);
	void fillFromForeignMap(const QVariantMap& personsAPIMap);
	void fillFromCacheMap(const QVariantMap& personsAPIMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int speakerId() const;
	void setSpeakerId(int speakerId);



	virtual ~PersonsAPI();

	Q_SIGNALS:

	void speakerIdChanged(int speakerId);
	

private:

	int mSpeakerId;

	Q_DISABLE_COPY (PersonsAPI)
};
Q_DECLARE_METATYPE(PersonsAPI*)

#endif /* PERSONSAPI_HPP_ */

