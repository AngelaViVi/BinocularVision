#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Evision.h"
#include "EvisionParamEntity.h"
#include "EvisionController.h"
#include <QLabel>
#include "LogView.h"

class EvisionView : public QMainWindow
{
	Q_OBJECT

public:
	EvisionView(QWidget *parent = Q_NULLPTR);

private:
	Ui::EvisionClass ui;
	EvisionParamEntity * m_entity;
	EvisionController * m_controller;
	QLabel *msgLabel;

	QPoint old_pos;
	QSize old_size;
	LogView * logView;
public slots:
	//�˵�
	void onCamera();
	void onStereoCamera();
	void onShowPointCloud();//��ʾ����

	void on_action_calibrate_view();		//�����궨��ͼ
	void on_action_rectify();				//����У����ͼ
	void on_action_stereoMatch_view();		//����ƥ����ͼ
	void on_action_Measure_view();			//������������
	void on_action_ObjectDetection_view();	//����Ŀ������ͼ
	void on_action_LogViewSwitch();			//LOGVIEW
	void on_action_disp_to_pcd();			//�Ӳ�ת����
	void on_action_create_param();			//������������ļ�
	//״̬��
	void onParamChanged_StatusBarText();
private:
	void dragEnterEvent(QDragEnterEvent *event) override;//�ļ����ϵ�����������
	void dropEvent(QDropEvent *event) override;//�ļ��ڴ��������ϱ�����
	void mouseReleaseEvent(QMouseEvent *event) override;//����ͷ��¼�
	void moveEvent(QMoveEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	void changeEvent(QEvent*) override;
	void closeEvent(QCloseEvent* event) override;
};
