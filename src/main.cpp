#include "CRobotControl.h"
#include<iostream>
#include "Cparameter.h"
#include <sstream>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include "CommandInterpreter.h"
#include"Test.h"

int main(int argc, char *argv[])
{

    //Cparameter c1;
    CRobotControl robotControl;
    //std::cout<<"Main thread started";
    robotControl.start();
    //CommandInterpreter interpreter(&robotControl);
    //interpreter.start();
    //robotControl.join();
    RobotCommandParameter robotCommand;
    //robotControl.generateInitialPointCloud();
    robotCommand.setCommand("isReachable_WP");
    robotCommand.setTx(50);
    robotCommand.setTy(5);
    robotCommand.setTz(5);
    robotCommand.setRx(50);
    robotCommand.setRy(12);
    robotCommand.setRz(10);
    //QMap<QString,QString> response = robotControl.moveTo(robotCommand);
    //qDebug()<<response.keys();

    /*pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGB>);
    if (pcl::io::loadPCDFile<pcl::PointXYZRGB> ("/home/vishal/PointClouds/bunny.pcd", *cloud) == -1) //* load the file
    {
        PCL_ERROR ("Couldn't read file test_pcd.pcd \n");
        return (-1);
    }*/

    pcl::PointCloud<pcl::PointXYZ>::Ptr basic_cloud_ptr (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr point_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
    uint8_t r(255), g(15), b(15);
    for (float z(-1.0); z <= 1.0; z += 0.05)
    {
        for (float angle(0.0); angle <= 360.0; angle += 10.0)
        {
            pcl::PointXYZ basic_point;
            basic_point.x = floor(0.5 * cosf (angle*3.14/180)*10);
            basic_point.y = floor(sinf (angle*3.14/180)*10);
            basic_point.z = floor(z*10);
            basic_cloud_ptr->points.push_back(basic_point);
            pcl::PointXYZRGB point;
            point.x = basic_point.x;
            point.y = basic_point.y;
            point.z = basic_point.z;
            uint32_t rgb = (static_cast<uint32_t>(r) << 16 |
                            static_cast<uint32_t>(g) << 8 | static_cast<uint32_t>(b));
            point.rgb = *reinterpret_cast<float*>(&rgb);
            point_cloud_ptr->points.push_back (point);
        }
        if (z < 0.0)
        {
            r -= 12;
            g += 12;
        }
        else
        {
            g -= 12;
            b += 12;
        }
    }
    basic_cloud_ptr->width = (int) basic_cloud_ptr->points.size ();
    basic_cloud_ptr->height = 1;
    point_cloud_ptr->width = (int) point_cloud_ptr->points.size ();
    point_cloud_ptr->height = 1;

    std::cout << "Loaded "
              << point_cloud_ptr->width * point_cloud_ptr->height
              << " data points from test_pcd.pcd with the following fields: "
              << std::endl;

    robotControl.sendPointCloud(point_cloud_ptr);
    robotControl.wait();
    return 0;
}

