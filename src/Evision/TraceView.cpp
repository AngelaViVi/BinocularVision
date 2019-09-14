#include "TraceView.h"
#include "EvisionUtils.h"
#include "QGraphicsScene"
#include <QFileDialog>
#include "StereoMatch.h"
#include <QMessageBox>
#include <iostream>
//ֱ��������ͼ,�����ò���
TraceView::TraceView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	trace_graphics_view = new TraceGraphicsView(this);
	//
	ui.TraceLayout->addWidget(trace_graphics_view);
	connect(trace_graphics_view, SIGNAL(mouseMove(int,int)), this, SLOT(onMouseMove(int, int)), Qt::QueuedConnection);
	connect(trace_graphics_view, SIGNAL(mouseLDown(int, int)), this, SLOT(onMouseLButtonDown(int, int)), Qt::QueuedConnection);
	connect(trace_graphics_view, SIGNAL(mouseRDown(int, int)), this, SLOT(onMouseRButtonDown(int, int)), Qt::QueuedConnection);
	//
	ui.checkBox_RawDispOK->setEnabled(false);
	ui.checkBox_OriginOK->setEnabled(false);
	ui.checkBox_CameraParamOK->setEnabled(false);
	ui.pushButton_start->setEnabled(false);
	sceneL = new QGraphicsScene();
}

TraceView::~TraceView()
{
}
//��O���ڻ�ͼ
void TraceView::printImgToO(cv::Mat value)
{
	sceneL->clear();
	QImage QImage = EvisionUtils::cvMat2QImage(value);
	sceneL->addPixmap(QPixmap::fromImage(QImage));
	trace_graphics_view->setScene(sceneL);
	trace_graphics_view->setMinimumSize(QImage.width(), QImage.height());
	trace_graphics_view->setMaximumSize(QImage.width(), QImage.height());
	trace_graphics_view->show();
	trace_graphics_view->update();
}

void TraceView::checkEnable()
{
	if(ui.checkBox_RawDispOK->isChecked()&&ui.checkBox_OriginOK->isChecked()&&ui.checkBox_CameraParamOK->isChecked())
	{
		ui.pushButton_start->setEnabled(true);
	}
}

//��Ӧ����ƶ�
void TraceView::onMouseMove(int x, int y)
{
	ui.lineEdit_ImgX->setText(QString::fromStdString(std::to_string(x)));
	ui.lineEdit_ImgY->setText(QString::fromStdString(std::to_string(y)));

}
//��Ӧ����������
void TraceView::onMouseLButtonDown(int x, int y)
{
	if (started)
	{
		float disp;

		if (RawDisp.type() == CV_32F)
			disp = RawDisp.at<float>(y, x);
		else
			disp = RawDisp.at<uchar>(y, x);

		float vec3DAbs;
		if (disp > 0)
		{
			cv::Point3f vec3D = image3D.at<cv::Point3f>(y, x);
			vec3DAbs = sqrt(pow(vec3D.x, 2) + pow(vec3D.y, 2) + pow(vec3D.z, 2));
		}
		else
		{
			vec3DAbs = -1;
		}
		ui.lineEdit_Res->setText(QString::fromStdString(std::to_string(vec3DAbs)));
	}
}
//��Ӧ����Ҽ�����
void TraceView::onMouseRButtonDown(int x, int y)
{

}
/*
 * ѡ��ԭʼ�Ӳ��ļ�
 */
void TraceView::onSelectRawDispFile()
{
	QFileDialog * fileDialog = new QFileDialog();
	fileDialog->setWindowTitle(QStringLiteral("��ѡ��ԭʼ�Ӳ��ļ�"));
	fileDialog->setNameFilter(QStringLiteral("���л�(*.xml)"));
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	if (fileDialog->exec() == QDialog::Accepted)
	{
		try
		{
			cv::FileStorage fStorage(fileDialog->selectedFiles().at(0).toStdString(), cv::FileStorage::READ);
			fStorage["disp"] >> RawDisp;
			fStorage.release();
		}
		catch (cv::Exception e)
		{
			std::cout << "ԭʼ�Ӳ����ݶ�ȡʧ��!" << e.err << std::endl;
		}
		ui.checkBox_RawDispOK->setChecked(true);
		checkEnable();
	}
}
/*
 * ѡ��ԭͼ
 */
void TraceView::onSelectOriginImg()
{
	QFileDialog * fileDialog = new QFileDialog();
	fileDialog->setWindowTitle(QStringLiteral("��ѡ�����������ѡ�Ӳ�ͼ������ͼ������ͼ"));
	fileDialog->setNameFilter(QStringLiteral("ͼƬ�ļ�(*.jpg *.png *.jpeg *.bmp)"));
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	if (fileDialog->exec() == QDialog::Accepted)
	{
		try
		{
			this->img = cv::imread(fileDialog->selectedFiles().at(0).toStdString());
		}
		catch (cv::Exception e)
		{
			std::cout << "ԭͼ��ȡʧ��!" << e.err << std::endl;
		}
		ui.checkBox_OriginOK->setChecked(true);
		checkEnable();
	}
}
/*
 * ѡ����������ļ�
 */
void TraceView::onSelectCameraParamFile()
{
	QFileDialog * fileDialog = new QFileDialog();
	fileDialog->setWindowTitle(QStringLiteral("��ѡ����������ļ�"));
	fileDialog->setNameFilter(QStringLiteral("���л��ļ�(*.xml *.yml)"));
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	if (fileDialog->exec() == QDialog::Accepted)
	{
		try
		{
			cv::FileStorage fStorage(fileDialog->selectedFiles().at(0).toStdString(), cv::FileStorage::READ);
			fStorage["Q"] >> Q;
			fStorage.release();
			ui.checkBox_CameraParamOK->setChecked(true);
			checkEnable();
		}
		catch (cv::Exception e)
		{
			std::cout << "���������ȡʧ��!" << e.err << std::endl;
		}

	}
}

void TraceView::onStart()
{
	cv::reprojectImageTo3D(RawDisp, image3D, Q);
	cv::Mat resizedImage, resizedDispMap;
	cv::resize(img, resizedImage, cv::Size(), scaleFactor, scaleFactor);
	cv::resize(RawDisp, resizedDispMap, cv::Size(), scaleFactor, scaleFactor);
	img = resizedImage.clone();
	RawDisp = resizedDispMap.clone();
	cv::resize(image3D, image3D, cv::Size(), scaleFactor, scaleFactor);

	if (RawDisp.type() == CV_32F)
	{
		//ADCensus��ԭʼ�Ӳ�������CV_32F,����ֱ����ʾ
		EvisionUtils::getGrayDisparity<float>(RawDisp, disparityGary, true);
	}
	else if(RawDisp.type() == CV_8U)
	{
		//BM,SGBM��ELAS���Ӳ�������CV_8U,����ֱ����ʾ
		disparityGary = RawDisp;
	}
	printImgToO(disparityGary);
	ui.pushButton_selectRawDisp->setEnabled(false);
	ui.pushButton_selectOrigin->setEnabled(false);
	ui.pushButton_selectCameraParam->setEnabled(false);
	started = true;
}

void TraceView::onSwitchImageToShow()
{
	if(DispIsShowing)
	{
		printImgToO(img);
		DispIsShowing = !DispIsShowing;
	}
	else
	{
		printImgToO(disparityGary);
		DispIsShowing = !DispIsShowing;
	}
}
