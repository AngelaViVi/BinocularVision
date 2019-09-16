#include "ObjectDetectionView.h"
#include <QFileDialog>
#include "EvisionUtils.h"

ObjectDetectionView::ObjectDetectionView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_engine = new ObjectDetectionEngine();
	//�����źŲ�
	m_entity = ObjectDetectionEntity::getInstance();

	connect(m_entity, SIGNAL(paramChanged_DetectionPlayer()), this, SLOT(OnParamChanged_DetectionPlayer()));
	connect(m_entity, SIGNAL(paramChanged_cfgFilename()), this, SLOT(OnParamChanged_cfgFilename()));
	connect(m_entity, SIGNAL(paramChanged_weightsFilename()), this, SLOT(OnParamChanged_weightsFilename()));
	connect(m_entity, SIGNAL(paramChanged_namesFilename()), this, SLOT(OnParamChanged_namesFilename()));
}

ObjectDetectionView::~ObjectDetectionView()
{
}
//ѡ��CFG�ļ�
void ObjectDetectionView::OnBrowseCfg()
{
	QFileDialog * fileDialog = new QFileDialog();
	fileDialog->setWindowTitle(QStringLiteral("��ѡ��CFG"));
	fileDialog->setNameFilter(QStringLiteral("CFG�ļ�(*.cfg)"));
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	if (fileDialog->exec() == QDialog::Accepted)
	{
		m_entity->setCfgFilename(fileDialog->selectedFiles().at(0));
	}
}
//ѡ��Weights�ļ�
void ObjectDetectionView::OnBrowseWeights()
{
	QFileDialog * fileDialog = new QFileDialog();
	fileDialog->setWindowTitle(QStringLiteral("��ѡ��weights�ļ�"));
	fileDialog->setNameFilter(QStringLiteral("weighes�ļ�(*.weights)"));
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	if (fileDialog->exec() == QDialog::Accepted)
	{
		m_entity->setweightsFilename( fileDialog->selectedFiles().at(0));
	}
}
//ѡ��Names�ļ�
void ObjectDetectionView::OnBrowseNames()
{
	QFileDialog * fileDialog = new QFileDialog();
	fileDialog->setWindowTitle(QStringLiteral("��ѡ��names�ļ�"));
	fileDialog->setNameFilter(QStringLiteral("names�ļ�(*.names)"));

	fileDialog->setFileMode(QFileDialog::ExistingFile);
	if (fileDialog->exec() == QDialog::Accepted)
	{
		m_entity->setnamesFilename(fileDialog->selectedFiles().at(0));
	}
}
//Switch
void ObjectDetectionView::OnSwitch()
{
	//��û�п�ʼ����:������,�����߳�
	if (m_engine->isRunning() == false)
	{
		if (!m_entity->getCfgFilename().isEmpty()&& !m_entity->getnamesFilename().isEmpty() && !m_entity->getweightsFilename().isEmpty())
		{
			m_engine->start();
		}
	}else if(m_engine->isRunning() == true)
	{
		m_entity->setRunningState(false);
	}
	
	//�Ѿ���ʼ����:ֹͣ����

}
//��Ӧ֡ˢ��
void ObjectDetectionView::OnParamChanged_DetectionPlayer()
{
	QImage LQImage = EvisionUtils::cvMat2QImage(m_entity->getDetectionPlayer());
	QGraphicsScene *sceneL = new QGraphicsScene;
	sceneL->addPixmap(QPixmap::fromImage(LQImage));
	ui.graphicsView->setScene(sceneL);
	QRectF bounds = sceneL->itemsBoundingRect();
	bounds.setWidth(bounds.width());         // to tighten-up margins
	bounds.setHeight(bounds.height());       // same as above
	ui.graphicsView->fitInView(bounds, Qt::KeepAspectRatio);
	ui.graphicsView->centerOn(0, 0);
	ui.graphicsView->show();
	ui.graphicsView->update();
}

//��ӦcfgFile�ĸ���
void ObjectDetectionView::OnParamChanged_cfgFilename()
{
	ui.lineEdit_Cfg->setText(m_entity->getCfgFilename());
}
//��Ӧweights file�ĸ���
void ObjectDetectionView::OnParamChanged_weightsFilename()
{
	ui.lineEdit_Weight->setText(m_entity->getweightsFilename());
}
//��Ӧnames file�ĸ���
void ObjectDetectionView::OnParamChanged_namesFilename()
{
	ui.lineEdit_Name->setText(m_entity->getnamesFilename());
}
