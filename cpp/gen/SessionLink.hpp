#ifndef SESSIONLINK_HPP_
#define SESSIONLINK_HPP_

#include <QObject>
#include <qvariant.h>




class SessionLink: public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged FINAL)
	Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged FINAL)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)


public:
	SessionLink(QObject *parent = 0);


	void fillFromMap(const QVariantMap& sessionLinkMap);
	void fillFromForeignMap(const QVariantMap& sessionLinkMap);
	void fillFromCacheMap(const QVariantMap& sessionLinkMap);
	
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



	virtual ~SessionLink();

	Q_SIGNALS:

	void uuidChanged(QString uuid);
	void urlChanged(QString url);
	void titleChanged(QString title);
	

private:

	QString mUuid;
	QString mUrl;
	QString mTitle;

	Q_DISABLE_COPY (SessionLink)
};
Q_DECLARE_METATYPE(SessionLink*)

#endif /* SESSIONLINK_HPP_ */

