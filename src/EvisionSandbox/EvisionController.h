#pragma once
#include "EvisionParamEntity.h"
#include <qstringlist.h>

class EvisionController: public QObject
{
	Q_OBJECT
private:
	EvisionParamEntity * m_entity;
	QStringList ImageListL, ImageListR;//��ͼƬ�궨ʱ��Ҫ������ͼƬ
	QString ImageL, ImageR, insFile, extFile;
	cv::Mat depth;
public:
	EvisionController(QObject *parent = Q_NULLPTR);
	~EvisionController();


public slots:
	void onOpenMessageBox(QString title, QString msg);
};

