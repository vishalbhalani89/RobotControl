#pragma once
#include <string>
#include <qsettings.h>
#include <qfile.h>
#include <qdir.h>

using namespace std;

class Cparameter
{
public:
	Cparameter(void);
	~Cparameter(void);
	void setLogFileName(QString name);
	QString getLogFileName();
	void setLogFileDirectory(QString path);
	QString getLogFileDirectory();
	void setIpAddress(QString name);
	QString getIpAddress();
	void setPort(int num);
	int getPort();
	void setPclDirectory(QString path);
	QString getPclDirectory();
	void setResolution(QString resolution);
	QString getResolution();
	void setFrames(int num);
	int getFrames();
	void setHorizontalFocalLength(float );
	float getHorizontalFocallength();
	void setVerticalFocalLength(float );
	float getVerticalFocallength();
	void setAlpha(float );
	float getAlpha();
	void setCoordinatex(float );
	float getCoordinatex();
	void setCoordinatey(float );
	float getCoordinatey();
	void setDistortion_1(float );
	float getDistortion_1();
	void setDistortion_2(float );
	float getDistortion_2();
	void setDistortion_3(float );
	float getDistortion_3();
	void setDistortion_4(float );
	float getDistortion_4();
	void setDistortion_5(float );
	float getDistortion_5();
	void initParameters();
	void readFromFile();
	void writeToFile();

private:
	QString mLogFileName;
	QString mLogfileDirectory;
	QString mIpAddress;
	int mPort;
	QString mPclDirectory;
	QString mResolution;
	int mFrames;
	QSettings *settings;
	float mFcHorizontal;
	float mFcVertical;
	float mAlpha_c;
	float mCoordinatex;
	float mCoordinatey;
	float mDistortion1;
	float mDistortion2;
	float mDistortion3;
	float mDistortion4;
	float mDistortion5;
};


