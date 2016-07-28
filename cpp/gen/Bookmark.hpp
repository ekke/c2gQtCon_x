#ifndef BOOKMARK_HPP_
#define BOOKMARK_HPP_

#include <QObject>
#include <qvariant.h>


#include "Session.hpp"


class Bookmark: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int sessionId READ sessionId WRITE setSessionId NOTIFY sessionIdChanged FINAL)
	// session lazy pointing to Session* (domainKey: sessionId)
	Q_PROPERTY(int session READ session WRITE setSession NOTIFY sessionChanged FINAL)
	Q_PROPERTY(Session* sessionAsDataObject READ sessionAsDataObject WRITE resolveSessionAsDataObject NOTIFY sessionAsDataObjectChanged FINAL)


public:
	Bookmark(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& bookmarkMap);
	void fillFromForeignMap(const QVariantMap& bookmarkMap);
	void fillFromCacheMap(const QVariantMap& bookmarkMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int sessionId() const;
	void setSessionId(int sessionId);
	// session lazy pointing to Session* (domainKey: sessionId)
	int session() const;
	void setSession(int session);
	Session* sessionAsDataObject() const;
	
	Q_INVOKABLE
	void resolveSessionAsDataObject(Session* session);
	
	Q_INVOKABLE
	void removeSession();
	
	Q_INVOKABLE
	bool hasSession();
	
	Q_INVOKABLE
	bool isSessionResolvedAsDataObject();
	
	Q_INVOKABLE
	void markSessionAsInvalid();
	



	virtual ~Bookmark();

	Q_SIGNALS:

	void sessionIdChanged(int sessionId);
	// session lazy pointing to Session* (domainKey: sessionId)
	void sessionChanged(int session);
	void sessionAsDataObjectChanged(Session* session);
	

private:

	int mSessionId;
	int mSession;
	bool mSessionInvalid;
	Session* mSessionAsDataObject;

	Q_DISABLE_COPY (Bookmark)
};
Q_DECLARE_METATYPE(Bookmark*)

#endif /* BOOKMARK_HPP_ */

