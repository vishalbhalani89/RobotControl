#include "Cparameter.h"
#include <qdebug.h>
#include <iostream>
/*!
 * Creates a file of it does not exists
 * Gives the default values for all the parameters 
 */
Cparameter::Cparameter(void)
{
	/* WORK SEQUENCE
	 * Check if the file already exists
	 * if it does not exist create the file and give a default value for all the parameters
	 * else reads the existing file and sets the prameters in the configuration.ini file
	 */
	QFile f(QDir::currentPath()+ "/" + "configurations.ini");
	if(!f.exists())
	{
	settings = new QSettings(QDir::currentPath()+ "/" + "configurations.ini", QSettings::IniFormat);
		initParameters();
	}
	else
	{
		readFromFile();
	}
}

/*!
 * When the program is closed this will save all 
 * the changes made in the parameters in the configurations.ini file
 */
Cparameter::~Cparameter(void)
{
	/* WORK SEQUENCE
	 * opens the ini file 
	 * Gets the current values if changed in the process by some other functions
	 * simply overwrites all the values when the destructor is called
	 */
	QSettings get(QDir::currentPath()+ "/" + "configurations.ini", QSettings::IniFormat);
	get.beginGroup("Parameters");
	QString name = getLogFileName();
	//qDebug() << name ;
	get.setValue("LogFileName", name);

	QString path = getLogFileDirectory();
	get.setValue("LogFileDirectory", path);

	QString ip = getIpAddress();
	get.setValue("IpAddress", ip);

	int port = getPort();
	get.setValue("Port", port);

	QString resolution = getResolution();
	get.setValue("Resolution", resolution);

	int frames = getFrames();
	get.setValue("Frames", frames);

	float HorizontalFcl = settings->value("HorizontalFocalLength").toFloat();

}
/*!
 * This will initialize all the parameters 
 */
void Cparameter::initParameters()
{
	/* WORK SEQUENCE
	 * Sets the defaults values for all the parameters in the ini file
	 */
		settings->beginGroup("Parameters");
		settings->setValue("LogFileName", "configurations.txt");
		settings->setValue("LogFileDirectory", "E:/Harsh/Kinect_motion/trunk");
		settings->setValue("IpAddress", "192.168.1.2");
		settings->setValue("Port", "2");
		settings->setValue("pcldirectory", "E:/Harsh/Kinect_motion");
		settings->setValue("Resolution", "640*480");
		settings->setValue("Framespersecond", "15");
		settings->endGroup();
		settings->beginGroup("Focal_lengths");
		settings->setValue("HorizontalFocalLength", "20.5");
		settings->setValue("VerticalFocalLength","30.5");
		settings->endGroup();
		settings->beginGroup("Angle");
		settings->setValue("Alpha", "0");
		settings->endGroup();
		settings->beginGroup("Image_Centre_point");
		settings->setValue("Coordinatex", "12.45");
		settings->setValue("Coordinatey", "14.5");
		settings->endGroup();
		settings->beginGroup("Distortions");
		settings->setValue("Distortion_1", "0.4");
		settings->setValue("Distortion_2", "0.5");
		settings->setValue("Distortin_3", "0.3");
		settings->setValue("Distortion_4", "0.7");
		settings->setValue("Distortion_5", "0.2");
		settings->endGroup();
		settings->sync();
		
		QString LogFileName = settings->value("LogFileName").toString();
		this->setLogFileName(LogFileName);

		QString LogFileDirectory = settings->value("LogFileDirectory").toString(); 
		this->setLogFileDirectory(LogFileDirectory);
	
		QString IpAddress = settings->value("IpAddress").toString(); 
		this->setIpAddress(IpAddress);

		int port = settings->value("Port").toInt(); 
		this->setPort(port);

		QString pcldirectory = settings->value("pcldirectory").toString(); 
		this->setPclDirectory(pcldirectory);

		QString resolution = settings->value("Resolution").toString(); 
		this->setResolution(resolution);

		int frames = settings->value("Framespersecond").toInt();
		this->setFrames(frames);

		float HorizontalFcl = settings->value("HorizontalFocalLength").toFloat();
		this->setHorizontalFocalLength(HorizontalFcl);

		float VerticalFcl = settings->value("VerticalFocalLength").toFloat();
		this->setVerticalFocalLength(VerticalFcl);

		float Alpha = settings->value("Alpha_c").toFloat();
		this->setAlpha(Alpha);
		
		float Coordinate_X = settings->value("Coordinatex").toFloat();
		this->setCoordinatex(Coordinate_X);

		float Coordinate_Y = settings->value("Coordinatey").toFloat();
		this->setCoordinatey(Coordinate_Y);

		float Distort_1 = settings->value("Distortion_1").toFloat();
		this->setDistortion_1(Distort_1);

		float Distort_2 = settings->value("Distortion_2").toFloat();
		this->setDistortion_2(Distort_2);

		float Distort_3 = settings->value("Distortion_3").toFloat();
		this->setDistortion_3(Distort_3);

		float Distort_4 = settings->value("Distortion_4").toFloat();
		this->setDistortion_4(Distort_4);

		float Distort_5 = settings->value("Distortion_5").toFloat();
		this->setDistortion_5(Distort_5);
}
/*!
 * This will read the current configurations.ini file and set all the parameter values
 */
void Cparameter::readFromFile()
{
	/* WORK SEQUENCE
	 * opens the ini file 
	 * Reads all the values from the ini file
	 * Saves all the values by calling setter functions
	 */
	QSettings set(QDir::currentPath()+ "/" + "configurations.ini", QSettings::IniFormat);
	set.beginGroup("Parameters");

	QString LogFileName = set.value("LogFileName").toString();
	this->setLogFileName(LogFileName);

	QString LogFileDirectory = set.value("LogFileDirectory").toString(); 
	this->setLogFileDirectory(LogFileDirectory);
	
	QString IpAddress = set.value("IpAddress").toString(); 
	this->setIpAddress(IpAddress);

	int port = set.value("Port").toInt(); 
	this->setPort(port);

	QString pcldirectory = set.value("pcldirectory").toString(); 
	this->setPclDirectory(pcldirectory);

	QString resolution = set.value("Resolution").toString(); 
	this->setResolution(resolution);

	int frames = set.value("Framespersecond").toInt();
	this->setFrames(frames);
	set.endGroup();

	set.beginGroup("Focal_lengths");
	float HorizontalFcl = set.value("HorizontalFocalLength").toFloat();
	this->setHorizontalFocalLength(HorizontalFcl);

	float VerticalFcl = set.value("VerticalFocalLength").toFloat();
	this->setVerticalFocalLength(VerticalFcl);
	set.endGroup();
	
	set.beginGroup("Angle");
	float Alpha = set.value("Alpha_c").toFloat();
	this->setAlpha(Alpha);
	settings->endGroup();

	set.beginGroup("Image_Centre_point");
	float Coordinate_X = set.value("Coordinatex").toFloat();
	this->setCoordinatex(Coordinate_X);

	float Coordinate_Y = set.value("Coordinatey").toFloat();
	this->setCoordinatey(Coordinate_Y);
	set.endGroup();

	set.beginGroup("Distortions");
	float Distort_1 = set.value("Distortion_1").toFloat();
	this->setDistortion_1(Distort_1);

	float Distort_2 = set.value("Distortion_2").toFloat();
	this->setDistortion_2(Distort_2);

	float Distort_3 = set.value("Distortion_3").toFloat();
	this->setDistortion_3(Distort_3);

	float Distort_4 = set.value("Distortion_4").toFloat();
	this->setDistortion_4(Distort_4);

	float Distort_5 = set.value("Distortion_5").toFloat();
	this->setDistortion_5(Distort_5);
	set.endGroup();
}

void Cparameter::writeToFile()
{
	QSettings write(QDir::currentPath()+ "/" + "configurations.ini", QSettings::IniFormat);

	write.beginGroup("Parameters");
	write.setValue("LogFileName", mLogFileName);
	write.setValue("LogFileDirectory", mLogfileDirectory);
	write.setValue("IpAddress", mIpAddress);
	write.setValue("Port", mPort);
	write.setValue("pcldirectory", mPclDirectory);
	write.setValue("Resolution", mResolution);
	write.setValue("Framespersecond", mFrames);
	write.endGroup();
	write.beginGroup("Focal_lengths");
	write.setValue("HorizontalFocalLength", mFcHorizontal);
	write.setValue("VerticalFocalLength", mFcVertical);
	write.endGroup();
	write.beginGroup("Angle");
	write.setValue("Alpha", mAlpha_c);
	write.endGroup();
	write.beginGroup("Image_Centre_point");
	write.setValue("Coordinatex", mCoordinatex);
	write.setValue("Coordinatey", mCoordinatey);
	write.endGroup();
	write.beginGroup("Distortions");
	write.setValue("Distortion_1", mDistortion1);
	write.setValue("Distortion_2", mDistortion2);
	write.setValue("Distortin_3", mDistortion3);
	write.setValue("Distortion_4", mDistortion4);
	write.setValue("Distortion_5", mDistortion5);
	write.endGroup();
	write.sync();
}

/*!
 * Sets the name of the logfile
 */
void Cparameter::setLogFileName(QString name)
{
	mLogFileName = name;
	qDebug() << mLogFileName ;
}

/*!
 * Returns the name of the logfile
 */
QString Cparameter::getLogFileName()
{
	return mLogFileName;
}

/*!
 * Sets the path of the logfileDirectory
 */
void Cparameter::setLogFileDirectory(QString path)
{
	mLogfileDirectory = path;
	qDebug() << mLogfileDirectory ;
}

/*!
 * Gets the path of the logfileDirectory
 */
QString Cparameter::getLogFileDirectory()
{
	return mLogfileDirectory;
}

/*!
 * Sets the IpAddress 
 */
void Cparameter::setIpAddress(QString ip)
{
	mIpAddress = ip;
	qDebug() << mIpAddress ;
}

/*!
 * Returns the IpAddress
 */
QString Cparameter::getIpAddress()
{
	return mIpAddress;
}

/*!
 * Sets the port number
 */
void Cparameter::setPort(int port)
{
	mPort = port;
	qDebug() << mPort ;
}

/*!
 * Returns the Port number
 */
int Cparameter::getPort()
{
	return mPort;
}

/*!
 * Sets the path of the PclDirectory
 */
void Cparameter::setPclDirectory(QString path)
{
	mPclDirectory = path;
	qDebug() << mPclDirectory ;
}

/*!
 * returns the path of the PclDirectory
 */
QString Cparameter::getPclDirectory()
{
	return mPclDirectory;
}

/*!
 * Sets the resoltion 
 */
void Cparameter::setResolution(QString resolution)
{
	mResolution = resolution;
	qDebug() << mResolution ;
}

/*!
 * Returns the resoltion 
 */
QString Cparameter::getResolution()
{
	return mResolution;
}

/*!
 * Sets the frames 
 */
void Cparameter::setFrames(int num)
{
	mFrames = num;
	qDebug() << mFrames ;
}

/*!
 * Returns the frames 
 */
int Cparameter::getFrames()
{
	return mFrames;
}

void Cparameter::setHorizontalFocalLength(float focallength)
{
	mFcHorizontal = focallength;
	qDebug() << mFcHorizontal;
};
float Cparameter::getHorizontalFocallength()
{
	return mFcHorizontal;
};
void Cparameter::setVerticalFocalLength(float focallength)
{
	mFcVertical = focallength;
	qDebug() << mFcVertical;
};
float Cparameter::getVerticalFocallength()
{
	return mFcVertical;
};
void Cparameter::setAlpha(float alpha)
{
	mAlpha_c = alpha; 
	qDebug() << mAlpha_c;
};
float Cparameter::getAlpha()
{
	return mAlpha_c;
};
void Cparameter::setCoordinatex(float coordinate)
{
	mCoordinatex = coordinate;
	qDebug() << mCoordinatex;
};
float Cparameter::getCoordinatex()
{
	return mCoordinatex;
};
void Cparameter::setCoordinatey(float coordinate)
{
	mCoordinatey = coordinate;
	qDebug() << mCoordinatey;
};
float Cparameter::getCoordinatey()
{
	return mCoordinatey;
};
void Cparameter::setDistortion_1(float distortion)
{
	mDistortion1 = distortion;	
	qDebug() << mDistortion1;
};
float Cparameter::getDistortion_1()
{
	return mDistortion1;
};
void Cparameter::setDistortion_2(float distortion)
{
	mDistortion2 = distortion;
	qDebug() << mDistortion2;
};
float Cparameter::getDistortion_2()
{
	return mDistortion2;
};
void Cparameter::setDistortion_3(float distortion)
{
	mDistortion3 = distortion;
	qDebug() << mDistortion3;
};
float Cparameter::getDistortion_3()
{
	return mDistortion3;
};
void Cparameter::setDistortion_4(float distortion)
{
	mDistortion4 = distortion;
	qDebug() << mDistortion4;
};
float Cparameter::getDistortion_4()
{
	return mDistortion4;
};
void Cparameter::setDistortion_5(float distortion)
{
	mDistortion5 = distortion;
	qDebug() << mDistortion5;
};
float Cparameter::getDistortion_5()
{
	return mDistortion5;
};