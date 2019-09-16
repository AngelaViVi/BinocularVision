#pragma once

#include <QObject>
#include <core/mat.hpp>

class CalibrateParamEntity : public QObject
{
	Q_OBJECT

public:
	static CalibrateParamEntity *getInstance();
	~CalibrateParamEntity();
private:
	CalibrateParamEntity(QObject *parent = 0);

private:

#pragma region params
	int BoardWidth = 0;
	int BoardHeight = 0;
	int SquareSize = 0;

	bool CALIB_FIX_PRINCIPAL_POINT = false;	//���Ż�����λ��
	bool CALIB_FIX_ASPECT_RATIO = false;	//�̶���߱�
	bool CALIB_ZERO_TANGENT_DIST = false;	//�����������ϵ��P1,P2
	bool CALIB_SAME_FOCAL_LENGTH = false;	//ǿ����������Ľ������
	bool CALIB_FIX_K1 = false;				//���Ծ������ϵ��K1
	bool CALIB_FIX_K2 = false;				//���Ծ������ϵ��K2
	bool CALIB_FIX_K3 = false;				//���Ծ������ϵ��K3
	bool CALIB_FIX_K4 = false;				//���Ծ������ϵ��K4
	bool CALIB_FIX_K5 = false;				//���Ծ������ϵ��K5
	bool CALIB_FIX_K6 = false;				//���Ծ������ϵ��K6
	bool CALIB_RATIONAL_MODEL = false;		//ʹ������ģ��(8ϵ��)
	bool CALIB_THIN_PRISM_MODEL = false;	//ʹ�ñ��⾵ģ��(12ϵ��)
	bool CALIB_FIX_S1_S2_S3_S4 = false;		//���Ż����⾵����ϵ��S1-S4
	bool CALIB_TILTED_MODEL = false;		//ʹ����б������ģ��(14ϵ��)
	bool CALIB_FIX_TAUX_TAUY = false;		//���Ż���б����������ϵ��(TauX,TauY)

	std::map<QString, cv::Mat> itemMap;//�洢<����,Mat>
	int index = 0;
#pragma endregion 

#pragma region param_Changed_signals
signals:
	void paramChanged_BoardWidth();
	void paramChanged_BoardHeight();
	void paramChanged_SquareSize();
	void paramChanged_CALIB_FIX_PRINCIPAL_POINT();
	void paramChanged_CALIB_FIX_ASPECT_RATIO();
	void paramChanged_CALIB_ZERO_TANGENT_DIST();
	void paramChanged_CALIB_SAME_FOCAL_LENGTH();
	void paramChanged_CALIB_FIX_K1();
	void paramChanged_CALIB_FIX_K2();
	void paramChanged_CALIB_FIX_K3();
	void paramChanged_CALIB_FIX_K4();
	void paramChanged_CALIB_FIX_K5();
	void paramChanged_CALIB_FIX_K6();
	void paramChanged_CALIB_RATIONAL_MODEL();
	void paramChanged_CALIB_THIN_PRISM_MODEL();
	void paramChanged_CALIB_FIX_S1_S2_S3_S4();
	void paramChanged_CALIB_TILTED_MODEL();
	void paramChanged_CALIB_FIX_TAUX_TAUY();
	void paramChanged_InsertToItemMap();
	void paramChanged_ClearItemMap();
#pragma endregion 

#pragma region getter And setter
public:
	int getBoardWidth();
	void setBoardWidth(int value);

	int getBoardHeight();
	void setBoardHeight(int value);

	int getSquareSize();
	void setSquareSize(int value);

	bool getCALIB_FIX_PRINCIPAL_POINT();
	void setCALIB_FIX_PRINCIPAL_POINT(bool value);

	bool getCALIB_FIX_ASPECT_RATIO();
	void setCALIB_FIX_ASPECT_RATIO(bool value);

	bool getCALIB_ZERO_TANGENT_DIST();
	void setCALIB_ZERO_TANGENT_DIST(bool value);

	bool getCALIB_SAME_FOCAL_LENGTH();
	void setCALIB_SAME_FOCAL_LENGTH(bool value);

	bool getCALIB_FIX_K1();
	void setCALIB_FIX_K1(bool value);

	bool getCALIB_FIX_K2();
	void setCALIB_FIX_K2(bool value);

	bool getCALIB_FIX_K3();
	void setCALIB_FIX_K3(bool value);

	bool getCALIB_FIX_K4();
	void setCALIB_FIX_K4(bool value);

	bool getCALIB_FIX_K5();
	void setCALIB_FIX_K5(bool value);

	bool getCALIB_FIX_K6();
	void setCALIB_FIX_K6(bool value);

	bool getCALIB_RATIONAL_MODEL();
	void setCALIB_RATIONAL_MODEL(bool value);

	bool getCALIB_THIN_PRISM_MODEL();
	void setCALIB_THIN_PRISM_MODEL(bool value);

	bool getCALIB_FIX_S1_S2_S3_S4();
	void setCALIB_FIX_S1_S2_S3_S4(bool value);

	bool getCALIB_TILTED_MODEL();
	void setCALIB_TILTED_MODEL(bool value);

	bool getCALIB_FIX_TAUX_TAUY();
	void setCALIB_FIX_TAUX_TAUY(bool value);

	std::map<QString, cv::Mat> & getItemMap();//��������ͼMap
	void insertItem(cv::Mat value);//������ͼMap�в���һ��
	void clearItemMap();//���Map
	int getIndex();//��ȡ���µ�����ͼindex,��ֵ��ֻ����
#pragma endregion 
};
