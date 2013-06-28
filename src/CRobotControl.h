#pragma once
#include<iostream>
#include <qthread.h>
#include <qtcpsocket.h>
#include <qdebug.h>
#include <qdom.h>
#include <qxml.h>
//#include<qtreewidget.h>
#include<qxmlstream.h>
#include "RobotCommandParameter.h"
#include<QString>
#include<iostream>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>

/*
This class will provide thread to communicate with firebrand.
It will have method to validate the command and form it in 
required(XML) format and send the request to firebrand and wait 
for the response. It will process the response and return the MAP
of various parameters.
*/
class CRobotControl : public QThread
{
	long static requestId;
private:
	QString ipAddress;
	long port;
    QString rawTcpIpAddress;
    long rawTcpPort;
    bool connected;
	QTcpSocket client;
    QTcpSocket rawTcpServer;//with unity
    QString responseXML;
	RobotCommandParameter mRobotCommandParameter;
	bool validateCommand(RobotCommandParameter parameter);
	QString makeXML(QString content,QString tag);
	QString sendRequest(QString request);
    QMap<QString,QString> kinectResponse;
    QVector< QMap<QString,QString> > kinectCommand;
	void setRobotCommandParameter(RobotCommandParameter parameter);
    bool processXml(QString xmlResponse);
    void notifyServer(QString requestXML,QString connectionType="firebrand");
    void rawTCPConnection();
    //void reconstructMesh(QString stream);

public:
    CRobotControl();
    QMap<QString,QString> popCommand();
	RobotCommandParameter getRobotCommandParameter(void);
	void setConnected(bool inMotion);
	bool getConnected(void);
    void run();
	void establishConnection();
	QMap<QString,QString> moveTo(RobotCommandParameter parameter);
    void generateInitialPointCloud();
    bool sendPointCloud(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud);
	~CRobotControl(void);
};
