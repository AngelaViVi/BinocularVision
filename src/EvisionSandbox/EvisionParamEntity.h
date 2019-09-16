#pragma once
#include <qobject.h>
#include <core/core.hpp>
/*
 * ����ʵ����
 * ����ֵ��ֱ�Ӹ��Ļᷢ��valuechanged�¼�,��view��ӵ���Ϣ,Ȼ����½���
 * 
 */
class EvisionParamEntity : public QObject
{
	Q_OBJECT
	
private:
	EvisionParamEntity(QObject *parent = 0);
	QString StatusBarText="";

public:
	static EvisionParamEntity * getInstance();
	~EvisionParamEntity();
	QString getStatusBarText();
	void setStatusBarText(QString value);

signals:
	void paramChanged_distance();
	void paramChanged_StatusBar();
};

