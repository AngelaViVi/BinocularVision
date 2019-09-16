#include "EvisionParamEntity.h"

//���캯����˽�е�
EvisionParamEntity::EvisionParamEntity(QObject * parent) : QObject(parent)
{ 
}

//��ȡʵ��(����ģʽ)
EvisionParamEntity * EvisionParamEntity::getInstance()
{
     static EvisionParamEntity *m_pInstance;
     if (m_pInstance == NULL) //�ж��Ƿ��һ�ε���
         m_pInstance = new EvisionParamEntity();
     return m_pInstance;
}

EvisionParamEntity::~EvisionParamEntity()
{
}

QString EvisionParamEntity::getStatusBarText()
{
	return StatusBarText;
}
void EvisionParamEntity::setStatusBarText(QString value=QStringLiteral("����"))
{
	StatusBarText = value;
	emit paramChanged_StatusBar();
}