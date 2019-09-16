#include "CreateCameraParamFile.h"
#include <QMessageBox>
#include <core/mat.hpp>
#include <opencv2/opencv.hpp>
#include "EvisionRectifyView.h"
#include "EvisionUtils.h"

CreateCameraParamFile::CreateCameraParamFile(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

CreateCameraParamFile::~CreateCameraParamFile()
{
}
/*
 * У�����
 */
void CreateCameraParamFile::onPush_check()
{
	checkpass = false;
	fxl = ui.lineEdit_fxl->text().toDouble();
	fyl = ui.lineEdit_fyl->text().toDouble();
	cxl = ui.lineEdit_cxl->text().toDouble();
	cyl = ui.lineEdit_cyl->text().toDouble();
	if (fxl<=0|| fyl<=0|| cxl<=0||cyl<=0)
	{
		QMessageBox::information(this, QStringLiteral("����������"), QStringLiteral("������ڲ�����!"));
		return;
	}
	fxr = ui.lineEdit_fxr->text().toDouble();
	fyr = ui.lineEdit_fyr->text().toDouble();
	cxr = ui.lineEdit_cxr->text().toDouble();
	cyr = ui.lineEdit_cyr->text().toDouble();

	if (fxr <= 0 || fyr <= 0 || cxr <= 0 || cyr <= 0)
	{
		QMessageBox::information(this, QStringLiteral("����������"), QStringLiteral("������ڲ�����!"));
		return;
	}
	QList<QString> qlistL = ui.lineEdit_coeffsl->text().split(',', QString::SkipEmptyParts);
	
	for (int i = 0; i < qlistL.size(); ++i)
	{
		coeffsl.push_back(qlistL.at(i).toDouble());
	}
	coeffsl.resize(qlistL.size());
	if (coeffsl.size()<=1|| coeffsl.size()>=14)
	{
		QMessageBox::information(this, QStringLiteral("����������"), QStringLiteral("�������������!"));
		return;
	}
	QList<QString> qlistR = ui.lineEdit_coeffsr->text().split(',', QString::SkipEmptyParts);
	
	for (int i = 0; i < qlistR.size(); ++i)
	{
		coeffsr.push_back(qlistR.at(i).toDouble());
	}
	coeffsr.resize(qlistR.size());
	if (coeffsr.size() <=1|| coeffsr.size()>=14)
	{
		QMessageBox::information(this, QStringLiteral("����������"), QStringLiteral("�������������!"));
		return;
	}
	QList<QString> qlistRotation=ui.lineEdit_R->text().split(',', QString::SkipEmptyParts);
	
	for (int i = 0; i < qlistRotation.size(); ++i)
	{
		rotation.push_back(qlistRotation.at(i).toDouble());
	}
	rotation.resize(qlistRotation.size());
	if (rotation.size() != 9)
	{
		QMessageBox::information(this, QStringLiteral("����������"), QStringLiteral("��ת��������!"));
		return;
	}
	QList<QString> qlistTranslation=ui.lineEdit_t->text().split(',', QString::SkipEmptyParts);
	
	for (int i = 0; i < qlistTranslation.size(); ++i)
	{
		translation.push_back(qlistTranslation.at(i).toDouble());
	}
	translation.resize(qlistTranslation.size());
	if (translation.size() != 3)
	{
		QMessageBox::information(this, QStringLiteral("����������"), QStringLiteral("ƽ�ƾ�������!"));
		return;
	}
	QMessageBox::information(this, QStringLiteral("��Ϣ"), QStringLiteral("У��ͨ��!"));

	checkpass = true;

}
/*
 * �������
 */
void CreateCameraParamFile::onPush_save()
{
	QString fileName = QFileDialog::getSaveFileName(this,QStringLiteral("�������ɵ���������ļ�"),"",tr("opencv yml (*.yml)"));
	if (fileName.isNull())
	{
		return;
	}
	//�����������
	cv::Mat cameraMatrix_L = cv::Mat::zeros(3, 3, CV_64F);
	cv::Mat distCoeffs_L = cv::Mat::zeros(coeffsl.size(), 1, CV_64F);
	cv::Mat cameraMatrix_R = cv::Mat::zeros(3, 3, CV_64F);;
	cv::Mat distCoeffs_R = cv::Mat::zeros(coeffsr.size(), 1, CV_64F);;
	cv::Mat R = cv::Mat::zeros(3, 3, CV_64F);;
	cv::Mat T = cv::Mat::zeros(3, 1, CV_64F);;

	cameraMatrix_L.at<double>(0, 0) = fxl; cameraMatrix_L.at<double>(0, 1) = 0;   cameraMatrix_L.at<double>(0, 2) = cxl;
	cameraMatrix_L.at<double>(1, 0) = 0;   cameraMatrix_L.at<double>(1, 1) = fyl; cameraMatrix_L.at<double>(1, 2) = cyl;
	cameraMatrix_L.at<double>(2, 0) = 0;   cameraMatrix_L.at<double>(2, 1) = 0;   cameraMatrix_L.at<double>(2, 2) = 1;

	for (int i = 0; i < coeffsl.size(); ++i)
	{
		distCoeffs_L.at<double>(i,0)= coeffsl.at(i);
	}

	cameraMatrix_R.at<double>(0, 0) = fxr; cameraMatrix_R.at<double>(0, 1) = 0;   cameraMatrix_R.at<double>(0, 2) = cxr;
	cameraMatrix_R.at<double>(1, 0) = 0;   cameraMatrix_R.at<double>(1, 1) = fyr; cameraMatrix_R.at<double>(1, 2) = cyr;
	cameraMatrix_R.at<double>(2, 0) = 0;   cameraMatrix_R.at<double>(2, 1) = 0;   cameraMatrix_R.at<double>(2, 2) = 1;

	for (int i = 0; i < coeffsr.size(); ++i)
	{
		distCoeffs_R.at<double>(i, 0) = coeffsr.at(i);
	}
	R.at<double>(0, 0) = rotation.at(0); R.at<double>(0, 1) = rotation.at(1); R.at<double>(0, 2) = rotation.at(2);
	R.at<double>(1, 0) = rotation.at(3); R.at<double>(1, 1) = rotation.at(4); R.at<double>(1, 2) = rotation.at(5);
	R.at<double>(2, 0) = rotation.at(6); R.at<double>(2, 1) = rotation.at(7); R.at<double>(2, 2) = rotation.at(8);

	T.at<double>(0, 0) = translation.at(0);T.at<double>(1, 0) = translation.at(1);T.at<double>(2, 0) = translation.at(2);

	cv::Size imageSize(752, 480);
	imageSize.width = ui.lineEdit_W->text().toInt();
	imageSize.height = ui.lineEdit_H->text().toInt();
	cv::Size tempsize(0, 0);

	//�������
	try
	{
		cv::Rect validRoi[2];
		cv::Mat R1, R2, P1, P2, Q;
		cv::stereoRectify(cameraMatrix_L, distCoeffs_L, cameraMatrix_R, distCoeffs_R,
			imageSize, R, T, R1, R2, P1, P2, Q,
			0, 1, tempsize, &validRoi[0], &validRoi[1]);

		std::string cameraParamsFilename = fileName.toStdString();

		bool flag = EvisionUtils::write_AllCameraParams(cameraParamsFilename, cameraMatrix_L, distCoeffs_L,
			cameraMatrix_R, distCoeffs_R, R, T, imageSize, R1, P1, R2, P2, Q, validRoi[0], validRoi[1]);
		std::cout << "�����Ѿ����浽:" << cameraParamsFilename << std::endl;
	}
	catch (cv::Exception e)
	{
		std::cout << "���� " << e.code << e.err << e.msg << e.what() << std::endl;
	}

}
