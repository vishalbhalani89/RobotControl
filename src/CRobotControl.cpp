#include<iostream>
#include "CRobotControl.h"
#include<pcl/io/obj_io.h>
#include<pcl/surface/vtk_smoothing/vtk_utils.h>
#include <pcl/io/pcd_io.h>
#include<pcl/io/obj_io.h>
#include<QtEndian>

//For appending request Id to the every request of firebrand
long CRobotControl::requestId;

/*
    Constructor of the class will establish the connection with firebrand and
    shows the proper message on Ui consol.
*/
CRobotControl::CRobotControl ()
{
    //initialize the static variable
    requestId=0;

    //For Firebrand Connection
    //this->ipAddress="localhost";
    this->ipAddress="141.100.109.160";
    this->port=23000;

    //For raw TCP connection with unity
    //this->rawTcpIpAddress="141.100.109.160";
    this->rawTcpIpAddress="localhost";
    this->rawTcpPort=30000;

    //Connect to Firebrand
    establishConnection();
}

// Connection with firebrand
void
CRobotControl::establishConnection ()
{
    QString message;
    client.connectToHost(ipAddress,port);

    //wait for connection in milliseconds
    if( client.waitForConnected(5000) )
    {
        message="Connection established:";
        connected=true;
    }
    else
    {
        message="\n Connection failed with :";
        connected=false;
    }
    message.append(ipAddress+":");
    message.append(QString::number(this->port));
    //qDebug()<<message;
}

//For connection with unity for raw TCP connection
void
CRobotControl::rawTCPConnection ()
{
    QString message;

    //make the requestXML to firebrand to open raw tcp socket for point cloud data
    QString requestXML;
    requestXML=this->makeXML(QString::number(requestId++),"requestId");
    requestXML.append(this->makeXML("openrawdatasocket","system"));
    requestXML.append(this->makeXML("unity","destination"));
    requestXML.append(this->makeXML("30000","port"));
    requestXML.append(this->makeXML("99000","length"));
    requestXML.append(this->makeXML("9000","buffersize"));
    requestXML = this->makeXML(requestXML,"kinectmotion");

    //Send the request
    this->notifyServer(requestXML);

    //wait for connection and to unity to open the raw tcp connection port
    this->sleep(10);

    //Check the response of the rawtcp socket request
    /*if(!processXml(responseXML))
    //{
    //  qDebug()<<"Error While parsing Response XML";
    //}
    //QString response=this->kinectResponse.value("response");
    //QString response="true";
    //if(response=="true")
    //{*/

    //Every thing is fine now we can connect to unity via raw tcp connection
    rawTcpServer.connectToHost(rawTcpIpAddress,rawTcpPort);

    //wait for connection in milliseconds
    if( rawTcpServer.waitForConnected(5000) )
    {
        message="Connection established:";
    }
    else
    {
        message="\n Connection failed with :";
    }

    message.append(rawTcpIpAddress+":");
    message.append(QString::number(this->rawTcpPort));
    //qDebug()<<message;
}

/*
    getters and setter of all the members of this class
*/
void
CRobotControl::setConnected (bool connect)
{
    connected=connect;
}
bool
CRobotControl::getConnected (void)
{
    return connected;
}
void
CRobotControl::setRobotCommandParameter (RobotCommandParameter parameter)
{
    mRobotCommandParameter=parameter;
}

RobotCommandParameter
CRobotControl::getRobotCommandParameter (void)
{
    return mRobotCommandParameter;
}


/*
    This function will take the tag and content and returns the simple XML format
*/
QString
CRobotControl::makeXML (QString content,QString tag)
{
    return "<"+tag+">"+content+"</"+tag+">";
}

/*
    This function will validate the request parameters and returns true if the request format is
    as expected else return false
*/
bool
CRobotControl::validateCommand (RobotCommandParameter parameter)
{
    // in future more validation rules can be added
    if(parameter.getCommand()=="")
    {
        return false;
    }
    return true;
}

/*
    This slot will be responsible for firebrand related command
    this will validate the request, form it in required format,
    send it over TCP connection and process the response
*/
QMap<QString,QString>
CRobotControl::moveTo (RobotCommandParameter parameter)
{
    this->setRobotCommandParameter(parameter);

    //Make the request in XML format for firebrand
    QString requestXML="";
    if(connected)
    {
        if(this->validateCommand(this->mRobotCommandParameter)==true)
        {
            // Request format for moveJoint command
            if(this->mRobotCommandParameter.getCommand() == "movejoint" )
            {
                requestXML=this->makeXML(QString::number(requestId++),"requestid");
                requestXML.append(this->makeXML(this->mRobotCommandParameter.getCommand(),"command"));
                requestXML.append(this->makeXML(this->mRobotCommandParameter.getName(),"name"));
                requestXML.append(this->makeXML(QString::number(this->mRobotCommandParameter.getDelta()),"delta"));
                //converting bool to string
                QString simulate="false";
                if(this->mRobotCommandParameter.getSimulateFirst()==1)
                    simulate="true";
                requestXML.append(this->makeXML(simulate,"simulatefirst"));
                requestXML = this->makeXML(requestXML,"kinectmotion");
            }

            //Request format for moveWaypoint Command
            else if(this->mRobotCommandParameter.getCommand() == "movewaypoint")
            {
                qDebug()<<"m in movewaypoint command";
                requestXML=this->makeXML(QString::number(requestId++),"requestid");
                requestXML.append(this->makeXML(this->mRobotCommandParameter.getCommand(),"command"));
                requestXML.append(this->makeXML(this->mRobotCommandParameter.getReferenceFrame(),"referenceframe"));
                QString simulate="false";
                //Converting bool to string
                if(this->mRobotCommandParameter.getSimulateFirst()==1)
                    simulate="true";
                requestXML.append(this->makeXML(simulate,"simulatefirst"));
                requestXML.append(this->makeXML(QString::number(this->mRobotCommandParameter.getTx()),"tx"));
                requestXML.append(this->makeXML(QString::number(this->mRobotCommandParameter.getTy()),"ty"));
                requestXML.append(this->makeXML(QString::number(this->mRobotCommandParameter.getTz()),"tz"));
                requestXML.append(this->makeXML(QString::number(this->mRobotCommandParameter.getRx()),"rx"));
                requestXML.append(this->makeXML(QString::number(this->mRobotCommandParameter.getRy()),"ry"));
                requestXML.append(this->makeXML(QString::number(this->mRobotCommandParameter.getRz()),"rz"));
                requestXML = this->makeXML(requestXML,"kinectmotion");
            }

            //Request format for movetoknownwp command
            else if(this->mRobotCommandParameter.getCommand() == "movetoknownwp")
            {
                requestXML=this->makeXML(QString::number(requestId++),"requestid");
                requestXML.append(this->makeXML(this->mRobotCommandParameter.getCommand(),"command"));
                requestXML.append(this->makeXML(this->mRobotCommandParameter.getName(),"name"));
                QString simulate="false";
                //converting bool to string
                if(this->mRobotCommandParameter.getSimulateFirst()==1)
                    simulate="true";
                requestXML.append(this->makeXML(simulate,"simulatefirst"));
                requestXML = this->makeXML(requestXML,"kinectmotion");
            }

            //Request format for isreachable_wp command
            else if(this->mRobotCommandParameter.getCommand() == "isreachable_wp")
            {
                requestXML=this->makeXML(QString::number(requestId++),"requestid");
                requestXML.append(this->makeXML(this->mRobotCommandParameter.getCommand(),"command"));
                requestXML.append(this->makeXML(QString::number(this->mRobotCommandParameter.getTx()),"tx"));
                requestXML.append(this->makeXML(QString::number(this->mRobotCommandParameter.getTy()),"ty"));
                requestXML.append(this->makeXML(QString::number(this->mRobotCommandParameter.getTz()),"tz"));
                requestXML.append(this->makeXML(QString::number(this->mRobotCommandParameter.getRx()),"rx"));
                requestXML.append(this->makeXML(QString::number(this->mRobotCommandParameter.getRy()),"ry"));
                requestXML.append(this->makeXML(QString::number(this->mRobotCommandParameter.getRz()),"rz"));
                requestXML = this->makeXML(requestXML,"kinectmotion");
            }

            //request format for where command
            else if(this->mRobotCommandParameter.getCommand() == "where" )
            {
                requestXML=this->makeXML(QString::number(requestId++),"requestid");
                requestXML.append(this->makeXML(this->mRobotCommandParameter.getCommand(),"command"));
                requestXML = this->makeXML(requestXML,"kinectmotion");
            }
            //Request format for isreachable_joint command
            else if(this->mRobotCommandParameter.getCommand() == "isreachable_joint")
            {
                requestXML=this->makeXML(QString::number(requestId++),"requestid");
                requestXML.append(this->makeXML(this->mRobotCommandParameter.getCommand(),"command"));
                requestXML.append(this->makeXML(this->mRobotCommandParameter.getName(),"name"));
                requestXML.append(this->makeXML(QString::number(this->mRobotCommandParameter.getDelta()),"delta"));
                requestXML = this->makeXML(requestXML,"kinectmotion");
            }

            //For unknown Command Error
            else
            {
                //std::cout<<"Unknown command for firebrand detected:";//<<this->mRobotCommandParameter.getCommand();
            }

            //Send the request and get the response
            QString responseXML = this->sendRequest(requestXML);

            if(!processXml(responseXML))
            {
                //Can not parse the XML response
                //qDebug()<<"Error While parsing Response XML";
            }
        }
        else
        {
            //validation fails
            //std::cout<<"Command parameters validation failed";
        }
    }
    else
    {
        //connect to firebrand
        establishConnection();

        if(connected)
        {
            //call the same function if connection is established
            this->moveTo(mRobotCommandParameter);
        }
        else
        {
            //Connection with firebrand fails
            //std::cout<<"Not connected to firebrand";
        }
    }
    return kinectResponse;
}

//This function will send the request and wail till the response has been received
QString
CRobotControl::sendRequest (QString requestXML)
{

    //qDebug()<<requestXML;
    requestXML.append("\n");
    QByteArray block (requestXML.toStdString().c_str());;
    QDataStream out(&block, QIODevice::WriteOnly);

    try
    {
        client.write(block,requestXML.length());
        if (!client.waitForBytesWritten(-1))
        {
            //qDebug()<<"Data sent";
        }
    }

    catch(...)
    {
        //qDebug()<<"Exception has been occured";
    }

    //Clear the client
    client.flush();

    //Wait for the response
    responseXML="";
    while(responseXML.compare("")==0 && (this->client.state() == QTcpSocket::ConnectedState))
    {
        //qDebug()<<"Waiting for response"<<responseXML;
    }
    //qDebug()<<"Response is not null :"<<responseXML;
    return responseXML;
}

//Parse the xml
bool
CRobotControl::processXml (QString responseXML)
{
    if(responseXML!="")
    {
        QMap<QString,QString> myMap;
        if(responseXML.contains("<responseid>"))
        {
            kinectResponse.clear();
            myMap = kinectResponse;
        }
        QXmlStreamReader xml(responseXML);
        QString content,tag;
        while (!xml.atEnd())
        {
            switch (xml.readNext())
            {
            case QXmlStreamReader::StartElement:
                break;
            case QXmlStreamReader::Characters:
                content=xml.text().toString();
                break;
            case QXmlStreamReader::EndElement:
                tag=xml.name().toString();
                myMap.insert(tag,content);
                break;
            default:
                break;
            }
        }
        if(myMap.contains("responseid"))
        {
            QMap<QString, QString>::iterator i;
            for (i = myMap.begin(); i != myMap.end(); ++i)
                kinectResponse.insert(i.key(),i.value());
            //qDebug()<<"Response of "<<myMap.value("responseid")<<" is received";
        }
        else
        {
            myMap.remove("kinectmotion");// remove the main tag
            kinectCommand.push_back(myMap);
            qDebug()<<"size of command is:"<<kinectCommand.size();
            /*foreach (QString key, myMap.keys())
            {
                qDebug() << "Command in the commandinterpreter is:";
                qDebug() << myMap.value(key);
            }*/
        }
        return true;
    }
    else
    {
        //qDebug()<<"Cant parse blank XML String";
    }
}

void
CRobotControl::run ()
{
    while(this->client.state() == QTcpSocket::ConnectedState)
    {
        QString xmlCommand="";
        char message[30000] = {};
        if(client.waitForReadyRead(5000))
        {
            client.read(message,client.bytesAvailable());
        }
        xmlCommand = (QString) message;
        xmlCommand.remove("\n");
        xmlCommand.remove("heartbeat");
        if(xmlCommand!="" && !(xmlCommand.contains("responseid")))
        {
            xmlCommand.append("\n");
            bool flag=this->processXml(xmlCommand);
            if(flag==true)
            {
                qDebug()<<"Kinect Command size is:"<<kinectCommand.size();
                //qDebug()<<"Command xml has been parsed";
            }
            else
            {
                qDebug()<<"can not parse the Command xml";
            }
        }
        else if(xmlCommand!="" && (xmlCommand.contains("responseid")))
        {
            responseXML.append(xmlCommand);
        }
        else
        {
            //qDebug()<<"unknown string or null string detected";
        }
    }
}

QMap<QString,QString>
CRobotControl::popCommand ()
{
    QMap<QString,QString> command;
    if(this->kinectCommand.size()>0)
    {
        qDebug()<<"I m in the pop Command";
        command=kinectCommand.front();
        /*foreach (QString key, command.keys())
        {
            qDebug() << "Command in the commandinterpreter is:";
            qDebug() << command.value(key);
        }*/
        kinectCommand.pop_front();
    }
    else
    {
        command.insert("nodata","nodata");
    }
    return command;
}

void
CRobotControl::generateInitialPointCloud ()
{
    QMap<QString,QString> response;
    RobotCommandParameter robotCommandParameter;

    //go to the initial position
    robotCommandParameter.setCommand("movewaypoint");
    robotCommandParameter.setReferenceFrame("base");
    robotCommandParameter.setSimulateFirst(false);
    robotCommandParameter.setTx(-0.860);
    robotCommandParameter.setTy(-0.110);
    robotCommandParameter.setTz(1.20);
    robotCommandParameter.setRx(180);
    robotCommandParameter.setRy(60);
    robotCommandParameter.setRz(0);
    response=this->moveTo(robotCommandParameter);

    if(response.size()>1)
        if(response.value("response","false") == "true")
        {
            robotCommandParameter.setCommand("movejoint");
            robotCommandParameter.setType("joint");
            robotCommandParameter.setName("0");
            robotCommandParameter.setSimulateFirst(false);
            for(int i=0;i<36;i++)
            {
                qDebug()<<"value of i is:"<<i;
                robotCommandParameter.setDelta(-10);
                response=this->moveTo(robotCommandParameter);
                if(response.value("response","false") == "true")
                {
                    qDebug()<<"Robot moved successfully";
                }
            }
        }
    this->notifyServer("<kinectmotion><destination>unity<destination><status>ready<status><description>KM is read for new order<description><kinectmotion>");
}

bool
CRobotControl::sendPointCloud (pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud)
{
    this->rawTCPConnection();
    uint16_t temp;
    QByteArray block(reinterpret_cast<const char*>(&temp), sizeof(temp));

    pcl::PointCloud<pcl::Normal>::Ptr new_normals (new pcl::PointCloud<pcl::Normal> ());
    pcl::NormalEstimation<pcl::PointXYZRGB, pcl::Normal> n;

    //Create a KD-Tree
    pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZRGB>);
    tree->setInputCloud(cloud);
    n.setInputCloud(cloud);
    n.setSearchMethod (tree);
    n.setKSearch(50);
    n.compute(*new_normals);

    // normals should not contain the point normals + surface curvatures
    pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr cloud_with_normals (new   pcl::PointCloud<pcl::PointXYZRGBNormal>);
    pcl::concatenateFields(*cloud, *new_normals,*cloud_with_normals);
    // cloud_with_normals = cloud + normals
    // Create search tree*
    pcl::search::KdTree<pcl::PointXYZRGBNormal>::Ptr tree2 (new pcl::search::KdTree<pcl::PointXYZRGBNormal>);
    tree2->setInputCloud (cloud_with_normals);

    // Initialize objects
    pcl::GreedyProjectionTriangulation<pcl::PointXYZRGBNormal> gp3;
    pcl::PolygonMesh triangles;

    // Set the maximum distance between connected points (maximum edge length)
    gp3.setSearchRadius (2);

    // Set typical values for the parameters
    gp3.setMu(2.5);
    gp3.setMaximumNearestNeighbors(200);
    gp3.setMaximumSurfaceAngle(M_PI/4); // 45 degrees
    gp3.setMinimumAngle(M_PI/18); // 10 degrees
    gp3.setMaximumAngle(2*M_PI/3); // 120 degrees
    gp3.setNormalConsistency(false);

    // Get result
    gp3.setInputCloud (cloud_with_normals);
    gp3.setSearchMethod (tree2);
    gp3.reconstruct (triangles);

    //Additional vertex information
    std::vector<int> idOfPoints = gp3.getPartIDs();
    std::vector<int> stateOfPoint = gp3.getPointStates();
    std::cout<<" Ids of points :"<<idOfPoints.size();
    std::cout<<" States of points :"<<stateOfPoint.size();
    std::cout<<" Number of triangles are :"<<triangles.polygons.size();
    std::cout<< std::endl;
    QString stream;
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr rgbCloud(new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::fromROSMsg(triangles.cloud, *rgbCloud);

    // number of points
    int nr_points  = triangles.cloud.width * triangles.cloud.height;
    size_t point_size = triangles.cloud.data.size () / nr_points;

    // mesh size
    int nr_meshes = triangles.polygons.size();
    std::cout << "# Vertices: " << nr_points << std::endl;
    std::cout << "# Faces: " << nr_meshes << std::endl;
    int rgb_index = pcl::getFieldIndex (triangles.cloud, "rgb");
    if(rgb_index != -1)
    {
        std::cout << "\nproperty uchar red"
                     "\nproperty uchar green"
                     "\nproperty uchar blue\n";
    }
    for (size_t i = 0; i < nr_points; ++i)
    {
        int xyz = 0;
        for (size_t d = 0; d < triangles.cloud.fields.size (); ++d)
        {
            int count = triangles.cloud.fields[d].count;
            if (count == 0)
                count = 1;
            int c = 0;

            // adding vertex
            if ((triangles.cloud.fields[d].datatype == sensor_msgs::PointField::FLOAT32) && (
                        triangles.cloud.fields[d].name == "x" ||
                        triangles.cloud.fields[d].name == "y" ||
                        triangles.cloud.fields[d].name == "z"))
            {
                float value;
                memcpy (&value, &triangles.cloud.data[i * point_size + triangles.cloud.fields[d].offset + c * sizeof (float)], sizeof (float));
                uint16_t data = qToBigEndian((uint16_t) floor(value));
                std::cout << value<<" ";
                block.append(reinterpret_cast<const char*>(&data),sizeof(uint16_t));//, sizeof(uint16_t));
                ++xyz;
            }
            std::cout<<" ";
            if (xyz==3 && triangles.cloud.fields[rgb_index].datatype == sensor_msgs::PointField::FLOAT32)
            {
                pcl::RGB color;
                memcpy (&color, &triangles.cloud.data[i * point_size + triangles.cloud.fields[rgb_index].offset + c * sizeof (float)], sizeof (pcl::RGB));
                unsigned int r = (unsigned int) color.r;
                unsigned int g = (unsigned int) color.g;
                unsigned int b = (unsigned int) color.b;
                std::cout << r << " " << g << " " << b;
                block.append(reinterpret_cast<char *>(&r), sizeof(uint8_t));
                block.append(reinterpret_cast<char *>(&g), sizeof(uint8_t));
                block.append(reinterpret_cast<char *>(&b), sizeof(uint8_t));
                break;
            }
        }
        if((i%1000)==0)
        {
            qDebug()<<"block data in Hex is :\n"<<block.toHex();
            QDataStream out(&block, QIODevice::WriteOnly);

            out.device()->seek(0);

            rawTcpServer.write(block);
            if (!client.waitForBytesWritten(-1))
            {
                //qDebug()<<"Data sent";
            }
            rawTcpServer.flush();
            block.clear();
        }
        if (xyz != 3)
        {
            PCL_ERROR ("[pcl::io::savePLYFile] Input point cloud has no XYZ data!\n");
            return (-2);
        }
        std::cout << std::endl;
    }
    qDebug()<<"Data size is:"<<block.size();
    qDebug()<<"block data in Hex is :\n"<<block.toHex();
    QDataStream out(&block, QIODevice::WriteOnly);

    out.device()->seek(0);

    rawTcpServer.write(block);
    if (!client.waitForBytesWritten(-1))
    {
        //qDebug()<<"Data sent";
    }
    rawTcpServer.flush();
    block.clear();
    sleep(10);
    for (size_t i=0; i<triangles.polygons.size(); i++) {
        std::cout<< "FaceId:"<<i <<" ";
        for (size_t j=0; j<triangles.polygons[i].vertices.size(); j++) {
            //qDebug()<<"appending the faces data";
            std::cout <<triangles.polygons[i].vertices[j]<<" ";
            uint32_t verticeId = qToBigEndian(triangles.polygons[i].vertices[j]);
            block.append(reinterpret_cast<const char*>(&verticeId), sizeof(triangles.polygons[i].vertices[j]));
        }
        if((i%750)==0)
        {
            qDebug()<<"In polyGons:";
            qDebug()<<"Data Block of polygons is:"<<block.size();
            qDebug()<<"block data of polygons is :\n"<<block.toHex();
            sleep(10);
            QDataStream out(&block, QIODevice::WriteOnly);
            out.device()->seek(0);
            //out << (quint32)(block.size() - sizeof(quint32));
            rawTcpServer.write(block);
            if (!client.waitForBytesWritten(-1))
            {
                qDebug()<<"Data sent";
            }
            rawTcpServer.flush();
            block.clear();
        }
        std::cout<< std::endl;
    }
    qDebug()<<"In polyGons:";
    qDebug()<<"Final Data Block of polygons is:"<<block.size();
    qDebug()<<"Final block data of polygons is :\n"<<block.toHex();
    rawTcpServer.write(block);
    if (!client.waitForBytesWritten(-1))
    {
        qDebug()<<"Final Data sent";
    }
    rawTcpServer.flush();
    block.clear();
    rawTcpServer.close();
    stream.append("\n");

    pcl::visualization::PCLVisualizer viz("visualizer");
    viz.addPolygonMesh(triangles);
    viz.spin();
    while(!viz.wasStopped())
    {
    }
    return true;
}

void
CRobotControl::notifyServer (QString requestXML,QString connectionType)
{

    std::cout<<"Notify xml is :"<<requestXML.toStdString();
    std::cout<<"Connection type is :"<<connectionType.toStdString();
    std::cout<<requestXML.toStdString();
    QTcpSocket *myClient;
    if(connectionType=="unity")
    {
        myClient=&this->rawTcpServer;
    }
    else
    {
        requestXML.append("\n");
        myClient=&this->client;
    }
    QByteArray block (requestXML.toStdString().c_str());
    QDataStream out(&block, QIODevice::WriteOnly);
    try
    {
        myClient->write(block,requestXML.length());
        if (!myClient->waitForBytesWritten(-1))
        {
            //qDebug()<<"Data sent";
        }
    }
    catch(...)
    {
        //qDebug()<<"Exception has been occured";
    }
    myClient->flush();
}

CRobotControl::~CRobotControl ()
{
    //std::cout<<std::endl<<"in the destructor"<<endl;
    QString requestXML="";
    requestXML=this->makeXML(QString::number(requestId++),"requestId");
    requestXML.append(this->makeXML("Client Shutting Down","system"));
    requestXML = this->makeXML(requestXML,"KinectMotion");
    this->notifyServer(requestXML);
    client.close();
    rawTcpServer.close();
}


