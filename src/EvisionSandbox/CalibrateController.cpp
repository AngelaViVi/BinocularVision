#include "CalibrateController.h"
#include "StereoCalibrate.h"
#include <QMessageBox>
#include <QFileDialog>
#include <opencv.hpp>

CalibrateController::CalibrateController(QObject *parent)
	: QObject(parent)
{
	m_calib_entity = CalibrateParamEntity::getInstance();
}

CalibrateController::~CalibrateController()
{
}
//����:Ĭ�ϱ궨����
void CalibrateController::setDefaultCalibParamCommand()
{
	m_calib_entity->setBoardWidth(9);
	m_calib_entity->setBoardHeight(6);
	m_calib_entity->setSquareSize(25);
	m_calib_entity->setCALIB_SAME_FOCAL_LENGTH(true);
	m_calib_entity->setCALIB_RATIONAL_MODEL(true);
	m_calib_entity->setCALIB_FIX_K3(true);
	m_calib_entity->setCALIB_FIX_K4(true);
	m_calib_entity->setCALIB_FIX_K5(true);
}
//����:�궨
void CalibrateController::CalibrateCommand()
{
	//1.��֤����
	if (m_calib_entity->getBoardWidth()*m_calib_entity->getBoardHeight()*m_calib_entity->getSquareSize() <= 0)
	{
		//δͨ��������������֤
		QMessageBox::information(NULL, QStringLiteral("����"), QStringLiteral("��������!����!"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	//2.ѡ���ļ�
	QFileDialog * fileDialog = new QFileDialog();
	fileDialog->setWindowTitle(QStringLiteral("��ѡ��������ͷ�����ͼƬ�ļ�����"));
	fileDialog->setNameFilter("ͼƬ�ļ�(*.jpg *.png *.jpeg *.bmp)");
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	if (fileDialog->exec() == QDialog::Accepted)
	{
		ImageListL = fileDialog->selectedFiles();
		fileDialog->setWindowTitle(QStringLiteral("��ѡ��������ͷ�����ͼƬ�ļ�����"));
		if (fileDialog->exec() == QDialog::Accepted)
		{
			ImageListR = fileDialog->selectedFiles();
			//�����ļ��б������ȷ��
			//��֤����
			if (ImageListL.size() != ImageListR.size() || ImageListL.size() == 0 || ImageListR.size() == 0)
			{
				//��һ��ͼ����ĿΪ0���������ͼ��������һ��,�˳�
				QMessageBox::information(NULL, QStringLiteral("����"), QStringLiteral("������ͼ��������Ȼ�Ϊ��"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				return;
			}
			else
			{
				/*
				 *3.һ������,���Խ��д�ͼƬ�궨
				 */
				std::vector<std::string>* imagelistL = new std::vector<std::string>();
				std::vector<std::string>* imagelistR = new std::vector<std::string>();
				for (int i = 0; i < std::min(ImageListL.size(), ImageListR.size()); ++i)
				{
					imagelistL->push_back(ImageListL.at(i).toStdString());
					imagelistR->push_back(ImageListR.at(i).toStdString());
				}
				m_calib_entity->clearItemMap();
				_stereoCalib = new StereoCalibrate(imagelistL, imagelistR);
				connect(_stereoCalib, SIGNAL(openMessageBox(QString, QString)), this, SLOT(onOpenMessageBox(QString, QString)));
				_stereoCalib->start();
			}
		}
		else
		{
			//ѡ����ͼ��ʱ���˳�
			return;
		}
	}
	else
	{
		//ѡ����ͼ��ʱ���˳�
		return;
	}
}
//����:�Ѳ������浽�ļ���
void CalibrateController::SaveParamsToFileCommand()
{
	if (_stereoCalib!=NULL)
	{
		_stereoCalib->SaveCameraParamsToFile();
	}
}

//��Ϣ��Ӧ:�����Ի���
void CalibrateController::onOpenMessageBox(QString title, QString msg)
{
	QMessageBox::information(NULL, title, msg);
}
