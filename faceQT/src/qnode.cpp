/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include "../include/faceQT/qnode.hpp"
#include <QTimer>
/*****************************************************************************
** Namespaces
*****************************************************************************/
std::string name="nobody.jpg";
cv::Mat facepic;
cv::Mat frame;
namespace faceQT {

/*****************************************************************************
** Implementation
*****************************************************************************/


QNode::QNode(int argc, char** argv ) :
	init_argc(argc),
	init_argv(argv)
	{}

QNode::~QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
	wait();
}

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{

    cv_bridge::CvImagePtr cv_ptr;
    try
      {
         cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
      }
    catch (cv_bridge::Exception& e)
      {
         ROS_ERROR("cv_bridge exception: %s", e.what());
         return;
      }
    facepic = cv_ptr->image;

}
void imageCallback1(const sensor_msgs::ImageConstPtr& msg)
{

    cv_bridge::CvImagePtr cv_ptr;
    try
      {
         cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
      }
    catch (cv_bridge::Exception& e)
      {
         ROS_ERROR("cv_bridge exception: %s", e.what());
         return;
      }
    frame = cv_ptr->image;

}
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{

                  name=msg->data.c_str();
}

bool QNode::init() {

    start();
    return true;
}

bool QNode::init(const std::string &master_url, const std::string &host_url) {
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(log()));
    timer->start(31);
    start();
  return true;
}

void QNode::run()
{

    QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(   log()));
     timer->start(10);
    ros::NodeHandle n;
  image_transport::ImageTransport it(n);
  image_transport::Subscriber sub3= it.subscribe("/imgmsg", 1, imageCallback1);
  image_transport::Subscriber sub2= it.subscribe("face_img", 1, imageCallback);
  ros::Subscriber sub = n.subscribe("id_name", 1, chatterCallback);
  ros::spin();
}
void QNode::log( ) {
    logging_model.insertRows(logging_model.rowCount(),1);
    std::stringstream logging_model_msg;
            //    ROS_INFO_STREAM(name);
    logging_model_msg  << name+",";

    QVariant new_row(QString(logging_model_msg.str().c_str()));
    logging_model.setData(logging_model.index(logging_model.rowCount()-1),new_row);
    Q_EMIT loggingUpdated(); // used to readjust the scrollbar
}

//	Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}




 // namespace faceQT
