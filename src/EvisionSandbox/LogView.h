#pragma once

#include <QWidget>
#include "ui_LogView.h"
#include "stdoutredirector.h"

class LogView : public QWidget
{
	Q_OBJECT

public:
	LogView(QWidget *parent = Q_NULLPTR);
	~LogView();
	static LogView *getInstance();
private:
	Ui::LogView ui;
	StdoutRedirector *redirector;
public slots:
	void readData();//�����ض����������Ϣ
	void onChecked_StayOnTop(bool value);//������ǰ��
	void onPush_Delete();//���
};
