#include"Test.h"

Test::Test()
{
    std::cout<<"Test Obj created";
    pcl::PointCloud<pcl::PointXYZ>::Ptr basic_cloud_ptr (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr point_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGB>);
    uint8_t r(255), g(15), b(15);

    pcl::PointXYZ basic_point;
    basic_point.x = 0.5;
    basic_point.y = 0.5;
    basic_point.z = 0.5;
    basic_cloud_ptr->points.push_back(basic_point);
    pcl::PointXYZRGB point;
    point.x = basic_point.x;
    point.y = basic_point.y;
    point.z = basic_point.z;
    uint32_t rgb = (static_cast<uint32_t>(r) << 16 |
                    static_cast<uint32_t>(g) << 8 | static_cast<uint32_t>(b));
    point.rgb = *reinterpret_cast<float*>(&rgb);
    point_cloud_ptr->points.push_back (point);

    basic_point.x = -0.5;
    basic_point.y = -0.5;
    basic_point.z = -0.5;

    rgb = (static_cast<uint32_t>(15) << 16 |
                    static_cast<uint32_t>(255) << 8 | static_cast<uint32_t>(15));
    point.rgb = *reinterpret_cast<float*>(&rgb);
    point_cloud_ptr->points.push_back (point);

    //basic_cloud_ptr->width = (int) basic_cloud_ptr->points.size ();
    //basic_cloud_ptr->height = 1;
    //point_cloud_ptr->width = (int) point_cloud_ptr->points.size ();
    //point_cloud_ptr->height = 1;
    pcl::visualization::CloudViewer viz("My cloud viewer");
    viz.showCloud(point_cloud_ptr);
    //viz.spin();
    while(!viz.wasStopped())
    {
    }
}
