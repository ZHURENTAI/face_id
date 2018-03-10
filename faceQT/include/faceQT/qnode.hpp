/**
 * @file /include/faceQT/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef faceQT_QNODE_HPP_
#define faceQT_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <string>
#include <QThread>
#include <QStringListModel>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sensor_msgs/image_encodings.h>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"


/*****************************************************************************
** Namespaces
*****************************************************************************/
   extern  std::string name;
 extern   cv::Mat facepic;
  extern   cv::Mat frame;
namespace faceQT {

/*****************************************************************************
** Class
*****************************************************************************/

class QNode : public QThread {
    Q_OBJECT

public:

	QNode(int argc, char** argv );
       QStringListModel logging_model;
	virtual ~QNode();
	bool init();
	bool init(const std::string &master_url, const std::string &host_url);
	void run();

	/*********************
	** Logging
	**********************/
	enum LogLevel {
	         Debug,
	         Info,
	         Warn,
	         Error,
	         Fatal
	 };

    QStringListModel* loggingModel() { return &logging_model; }


Q_SIGNALS:
	void loggingUpdated();
    void rosShutdown();

private:
	int init_argc;
	char** init_argv;
	ros::Publisher chatter_publisher;


public Q_SLOTS:
    void log( );
};

}  // namespace faceQT

#endif /* faceQT_QNODE_HPP_ */
