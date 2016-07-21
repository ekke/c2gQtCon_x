#ifndef SCHEDULEITEM_HPP_
#define SCHEDULEITEM_HPP_

#include <QObject>
#include <qvariant.h>


// forward declaration (target references to this)
class Session;


class ScheduleItem: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int sessionId READ sessionId WRITE setSessionId NOTIFY sessionIdChanged FINAL)
	Q_PROPERTY(bool isBreak READ isBreak WRITE setIsBreak NOTIFY isBreakChanged FINAL)
	Q_PROPERTY(bool isLunch READ isLunch WRITE setIsLunch NOTIFY isLunchChanged FINAL)
	Q_PROPERTY(bool isEvent READ isEvent WRITE setIsEvent NOTIFY isEventChanged FINAL)
	Q_PROPERTY(bool isRegistration READ isRegistration WRITE setIsRegistration NOTIFY isRegistrationChanged FINAL)
	// session lazy pointing to Session* (domainKey: sessionId)
	Q_PROPERTY(int session READ session WRITE setSession NOTIFY sessionChanged FINAL)
	Q_PROPERTY(Session* sessionAsDataObject READ sessionAsDataObject WRITE resolveSessionAsDataObject NOTIFY sessionAsDataObjectChanged FINAL)


public:
	ScheduleItem(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& scheduleItemMap);
	void fillFromForeignMap(const QVariantMap& scheduleItemMap);
	void fillFromCacheMap(const QVariantMap& scheduleItemMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int sessionId() const;
	void setSessionId(int sessionId);
	bool isBreak() const;
	void setIsBreak(bool isBreak);
	bool isLunch() const;
	void setIsLunch(bool isLunch);
	bool isEvent() const;
	void setIsEvent(bool isEvent);
	bool isRegistration() const;
	void setIsRegistration(bool isRegistration);
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
	



	virtual ~ScheduleItem();

	Q_SIGNALS:

	void sessionIdChanged(int sessionId);
	void isBreakChanged(bool isBreak);
	void isLunchChanged(bool isLunch);
	void isEventChanged(bool isEvent);
	void isRegistrationChanged(bool isRegistration);
	// session lazy pointing to Session* (domainKey: sessionId)
	void sessionChanged(int session);
	void sessionAsDataObjectChanged(Session* session);
	

private:

	int mSessionId;
	bool mIsBreak;
	bool mIsLunch;
	bool mIsEvent;
	bool mIsRegistration;
	int mSession;
	bool mSessionInvalid;
	Session* mSessionAsDataObject;

	Q_DISABLE_COPY (ScheduleItem)
};
Q_DECLARE_METATYPE(ScheduleItem*)

#endif /* SCHEDULEITEM_HPP_ */

