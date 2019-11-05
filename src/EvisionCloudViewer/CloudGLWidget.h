#ifndef VOWELCUBE_H
#define VOWELCUBE_H

#include <QGLWidget>
#include <QRadialGradient>
//ʹ��OpenGL���������壬ʹ��QPainter���Ʊ���
//�Ľ��䣬����ʹ��renderText����������������ϵ�8��
//Ԫ����ĸ�����ʹ��QPainter��QTextDocument����ͼ����
//�û����Ե������϶��������ת�����壬���ҿ���ʹ�������ֽ��зŴ����С
class CloudGLWidget : public QGLWidget
{
	Q_OBJECT

public:
	CloudGLWidget(std::string filename,QWidget *parent = 0);
	~CloudGLWidget();

protected:
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

private:
	void createGradient();
	void createGLObject();
	void drawBackground(QPainter *painter);
	void drawItem();
	void drawLegend(QPainter *painter);
	std::vector<std::vector<float>> getPointCloud(std::string filename);

	GLfloat getScalingFactor(std::vector<std::vector<float>> bbox);
	void getTranslationV(std::vector<std::vector<float>> bbox);
	void analyzeCloud();

	std::vector<std::vector<float>> bbox;
	std::string filename;
	GLfloat transX;
	GLuint glObject{};
	QRadialGradient gradient;
	GLfloat rotationX;
	GLfloat rotationY;
	GLfloat rotationZ;
	GLfloat transY;
	GLfloat transZ;
	GLfloat scaling;
	QPoint lastPos;
	int numofPoints{};
};

#endif