#ifndef SESSIONLINKAPI_HPP_
#define SESSIONLINKAPI_HPP_

#include <QObject>
#include <qvariant.h>




class SessionLinkAPI: public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged FINAL)
	Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged FINAL)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)


public:
	SessionLinkAPI(QObject *parent = 0);


	void fillFromMap(const QVariantMap& sessionLinkAPIMap);
	void fillFromForeignMap(const QVariantMap& sessionLinkAPIMap);
	void fillFromCacheMap(const QVariantMap& sessionLinkAPIMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	QString uuid() const;
	void setUuid(QString uuid);
	QString url() const;
	void setUrl(QString url);
	QString title() const;
	void setTitle(QString title);



	virtual ~SessionLinkAPI();

	Q_SIGNALS:

	void uuidChanged(QString uuid);
	void urlChanged(QString url);
	void titleChanged(QString title);
	

private:

	QString mUuid;
	QString mUrl;
	QString mTitle;

	Q_DISABLE_COPY (SessionLinkAPI)
};
Q_DECLARE_METATYPE(SessionLinkAPI*)

#endif /* SESSIONLINKAPI_HPP_ */

