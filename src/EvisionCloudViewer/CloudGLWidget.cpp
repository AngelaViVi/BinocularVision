#include <QtGui>
#include <QtOpenGL>
#include <cmath>
#include <iostream>              
#include <string> 
#include <pcl/io/pcd_io.h>       //PCD��д����ص�ͷ�ļ�
#include <pcl/point_types.h>     //PCL��֧�ֵĵ����͵�ͷ�ļ�
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

#include "CloudGLWidget.h"
#include "CloudAnalyzer.h"
#include <pcl/common/time.h>

/*
 * 1. ������Χ��
 * 2. �����ʵ�����
 * 3. ͨ��������ƽ�Ʊ任,�����ķŵ��м�
 * 4. ��ɫ
 *
 * ����Ҫ��ʾ����:
 * ������ʾ
 * ��������
 *
 *
 */
 //����
CloudGLWidget::CloudGLWidget(std::string filename, QWidget *parent)
	: QGLWidget(parent)
{
	//����setFormat����ʹOpenGL����ʾ������֧�ַ�������
	setFormat(QGLFormat(QGL::SampleBuffers));

	//��ʼ��˽�б���
	rotationX = -38.0;
	rotationY = -58.0;
	rotationZ = 0.0;
	scaling = 1.0;
	//ƽ�ƿ���
	transX = 0;
	transY = 0;
	transZ = -10;
	numofPoints = 0;
	this->filename = filename;
	//������䱳����QRadialGradient
	createGradient();
	//����OpenGL����
	createGLObject();

	setAutoBufferSwap(false);
	setAutoFillBackground(false);

	//this->numofPoints = 0;
	

}
//����
CloudGLWidget::~CloudGLWidget()
{
	makeCurrent();
	//ɾ�����캯��������OpenGL���������
	glDeleteLists(glObject, 1);
}

//����,��paintEvent()�д���һ��QPainter,�ڽ��д�OpenGL����ʱ
//����ͻָ���״̬��
//QPainter�Ĺ��캯��(����QPainter::begin())�Զ�����glClear
//����Ԥ�ȵ��ô��ڲ�����setAutoFillBackground(false)
//QPainter����������������QPainter::end()���Զ�����QGLWidget::swapBuffers()
//�л��ɼ�������������棬����Ԥ�ȵ���setAutoBufferSwap(false).
//��QPainter��������ǿ��Խ���ʹ�ô�OpenGL���ֻҪ��ִ�д�OpenGL����֮ǰ����OpenGL״̬��֮��ָ�OpenGL״̬
void CloudGLWidget::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);
	//���Ʊ���
	drawBackground(&painter);
	//�������ƽ���
	painter.end();
	//����������
	drawItem();
	//���ƿ�ʼ
	painter.begin(this);
	//ʹ��HTML��������QTextDocument����
	drawLegend(&painter);
	painter.end();
	swapBuffers();
}

//��갴���¼�:��¼����λ��
void CloudGLWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
}

//����ƶ��¼�:���ڿ���ƽ�ƺ���ת
void CloudGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
	GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();

	if (event->buttons() & Qt::LeftButton) {//���
		rotationX -= 180 * dy;
		rotationY -= 180 * dx;
		update();
	}
	else if (event->buttons() & Qt::RightButton) {//�Ҽ�
		transX += dx;
		transY -= dy;
		//rotationX += 180 * dy;
		//rotationZ += 180 * dx;
		update();
	}
	else if (event->buttons() & Qt::MiddleButton)//�м�
	{
		//transZ += (5 / scaling)*std::min(dx,dy);
		////rotationX += 180 * dy;
		////rotationZ += 180 * dx;
		//update();
	}
	lastPos = event->pos();
}

//���������¼�:���ڿ�������
void CloudGLWidget::wheelEvent(QWheelEvent *event)
{
	double numDegrees = -event->delta() / 8.0;
	double numSteps = numDegrees / 15.0;
	scaling *= std::pow(1.125, numSteps);
	update();
}

//ʹ����ɫ����ɫ����QRadialGradient
void CloudGLWidget::createGradient()
{
	//ȷ��ָ�������ĺͽ���������ݴ��ڲ�����С���е���
	gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
	//λ����0��1֮��ĸ�������ʾ��0��Ӧ�������꣬1��ӦԲ�ı�Ե
	gradient.setCenter(0.45, 0.50);
	gradient.setFocalPoint(0.40, 0.45);
	gradient.setColorAt(0.0, QColor(105, 146, 182));
	gradient.setColorAt(0.4, QColor(81, 113, 150));
	gradient.setColorAt(0.8, QColor(16, 56, 121));
}

//���������Ƶ�Ŀ��
void CloudGLWidget::createGLObject()
{
	makeCurrent();

	glShadeModel(GL_FLAT);

	glObject = glGenLists(1);
	glNewList(glObject, GL_COMPILE);
	qglColor(QColor(255, 239, 191));
	glLineWidth(1.0);

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

	//�򿪵����ļ�"F:\\pcl\\test\\car6.pcd""C:\\Users\\Anna\\Desktop\\test.pcd"0_cloud
	if (pcl::io::loadPCDFile<pcl::PointXYZ>(this->filename, *cloud) == -1)
	{
		PCL_ERROR("Couldn't read file,the file name is :\n");
		PCL_ERROR(this->filename.c_str());
		return;
	}


	glBegin(GL_POINTS);
	this->numofPoints = cloud->points.size();
	for (size_t i = 0; i < cloud->points.size(); ++i)
	{
		glVertex3f(cloud->points[i].x, cloud->points[i].y, cloud->points[i].z);
	}
	glEnd();

	CloudAnalyzer ca;
	this->bbox = ca.bbox(cloud);//�õ����Χ��

	//	    V4 ---------- V6
	//        |\        |\
	//        | \_______|_\ V7
	//        | V5      | |
	//      V0|_|_ _ _ _|V2
	//        \ |       \ |
	//         \|________\|
	//         V1         V3

	//scaling = getScalingFactor(bbox);//��ȡ����
	getTranslationV(bbox);//����ƽ��
	GLint index_list[][2] = { {0, 1},{2, 3},{4, 5},{6, 7},{0, 2},{1, 3},
							{4, 6},{5, 7},{0, 4},{1, 5},{7, 3},{2, 6} };
	glBegin(GL_LINES);
	for (int i = 0; i < 12; ++i) // 12 ���߶�
	{
		for (int j = 0; j < 2; ++j) // ÿ���߶� 2������
		{
			glVertex3f(bbox[index_list[i][j]][0], bbox[index_list[i][j]][1], bbox[index_list[i][j]][2]);
		}
	}
	glEnd();

	glEndList();
}

//�����Ļ���
void CloudGLWidget::drawBackground(QPainter *painter)
{
	painter->setPen(Qt::NoPen);
	painter->setBrush(gradient);
	painter->drawRect(rect());
}

//��������
void CloudGLWidget::drawItem()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	GLfloat x = 3.0 * GLfloat(width()) / height();
	//glOrtho(-x, +x, -3.0, +3.0, 4.0, 15.0);

	/*
	 * min_px bbox[0][0]
	 * min_py bbox[0][1]
	 * min_pz bbox[0][2]
	 * max_px bbox[1][0]
	 * max_py bbox[2][1]
	 * max_pz bbox[4][2]
	 */
	GLfloat min_i = std::min(std::min(bbox[0][0] - bbox[1][0], bbox[0][1] - bbox[2][1]), bbox[0][2] - bbox[4][2]);
	GLfloat max_i = std::max(std::max(bbox[1][0] - bbox[0][0], bbox[2][1] - bbox[0][1]), bbox[4][2] - bbox[0][2]);

	glOrtho(min_i, max_i, min_i, max_i, min_i, max_i);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glRotatef(rotationX, 1.0, 0.0, 0.0);//��ת
	glRotatef(rotationY, 0.0, 1.0, 0.0);
	glRotatef(rotationZ, 0.0, 0.0, 1.0);
	glScalef(scaling, scaling, scaling);//����
	glTranslatef(transX, transY, transZ);//ƽ��

	//���÷�����
	glEnable(GL_MULTISAMPLE);

	//����������
	glCallList(glObject);

	//����������ɫ������
	setFont(QFont("Times", 18));
	qglColor(QColor(255, 223, 127));

	//�����ַ���renderTextʹ���ֲ�����
	for (int i = 0; i < 8; ++i)
	{
		renderText(bbox[i][0], bbox[i][1], bbox[i][2], QChar(i + 48));
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();
}


//ʹ��HTML��������QTextDocument�����ڰ�͸������ɫ�����ϻ�������
void CloudGLWidget::drawLegend(QPainter *painter)
{
	const int Margin = 11;
	const int Padding = 6;

	QString text = QString().sprintf("<h4 align=\"center\">Point Cloud Viewer</h4>"
		"<p align=\"center\"><table width=\"100%\">"
		"<tr><td>numbers:<td>%d<td>"
		"</table>", numofPoints);

	QTextDocument textDocument;
	textDocument.setDefaultStyleSheet("* { color: #FFEFEF }");
	textDocument.setHtml(text);
	textDocument.setTextWidth(textDocument.size().width());

	QRect rect(QPoint(0, 0), textDocument.size().toSize()
		+ QSize(2 * Padding, 2 * Padding));
	painter->translate(width() - rect.width() - Margin,
		height() - rect.height() - Margin);
	painter->setPen(QColor(255, 239, 239));
	painter->setBrush(QColor(255, 0, 0, 31));
	painter->drawRect(rect);

	painter->translate(Padding, Padding);
	textDocument.drawContents(painter);
}

GLfloat CloudGLWidget::getScalingFactor(std::vector<std::vector<float>> bbox)
{
	//1. ȡ���ڴ�С
	std::cout << width() << " , " << height() << std::endl;

	GLfloat target = std::sqrt(std::pow(width(), 2) + std::pow(height(), 2));
	//
//                
//	    V4 ---------- V6
//        |\        |\
//        | \_______|_\ V7
//        | V5      | |
//      V0|_|_ _ _ _|V2
//        \ |       \ |
//         \|________\|
//         V1         V3

	//2. ����v0��v7�ľ���
	GLfloat dis = std::sqrt(std::pow(bbox[0][0] - bbox[7][0], 2) + std::pow(bbox[0][1] - bbox[7][1], 2)/*+ std::pow(bbox[0][2] - bbox[7][2], 2)*/);

	//3. some magic

	return 0.25*(target / dis);
}

void CloudGLWidget::getTranslationV(std::vector<std::vector<float>> bbox)
{
	//1. ���㼸������
	GLfloat x = (bbox[0][0] + bbox[7][0]) / 2;
	GLfloat y = (bbox[0][1] + bbox[7][1]) / 2;
	GLfloat z = (bbox[0][2] + bbox[7][2]) / 2;
	//Eigen::Vector3f c1 = 0.5f*(min_p.getVector3fMap() + max_p.getVector3fMap());//��������
	//2. ����ƽ������
	this->transX = -x;
	this->transY = -y;
	this->transZ = -z;
}