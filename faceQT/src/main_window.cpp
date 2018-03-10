/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/
#include <QTextCodec>
#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/faceQT/main_window.hpp"
#include <ros/ros.h>
#include <string>
#include <QThread>
#include <QStringListModel>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <fstream>
#include <string>
#include <sensor_msgs/image_encodings.h>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <std_msgs/String.h>
#include "../include/faceQT/qnode.hpp"
/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace faceQT {

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
	: QMainWindow(parent)
	, qnode(argc,argv)
{
 ros::init(argc,argv,"faceQT");
	ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
    QObject::connect(ui.actionAbout_Qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt())); // qApp is a global variable for the application
    ReadSettings();
	setWindowIcon(QIcon(":/images/icon.png"));
    //ui.tab_manager->setCurrentIndex(0); // ensure the first tab is showing - qt-designer should have this already hardwired, but often loses it (settings?).
    QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));
   ui.take_photo->setEnabled(false);
	/*********************
	** Logging
	**********************/
    //ui.view_logging->setModel(qnode.loggingModel());
    QObject::connect(&qnode, SIGNAL(loggingUpdated()), this, SLOT(updateLoggingView()));

    /*********************
    ** Auto Start
    **********************/
    if ( ui.checkbox_remember_settings->isChecked() ) {
        on_button_connect_clicked(true);
    }
}


MainWindow::~MainWindow() {}

/*****************************************************************************
** Implementation [Slots]
*****************************************************************************/

void MainWindow::showNoMasterMessage() {
	QMessageBox msgBox;
	msgBox.setText("Couldn't find the ros master.");
	msgBox.exec();
    close();
}

/*
 * These triggers whenever the button is clicked, regardless of whether it
 * is already checked or not.
 */




void MainWindow::on_button_connect_clicked(bool check ) {

		if ( ! qnode.init(ui.line_edit_master->text().toStdString(),
                   ui.take_photo_label->text().toStdString()) ) {
			showNoMasterMessage();
		} else {
            ui.take_photo->setEnabled(true);
			ui.button_connect->setEnabled(false);
			ui.line_edit_master->setReadOnly(true);
			ui.line_edit_topic->setReadOnly(true);
	}
}
void MainWindow::on_take_photo_clicked() {


    std::string  facepath="/home/tiger/photo/"+ui.take_photo_label->text().toStdString()+".jpg";
    cv::imwrite(facepath,facepic);
}

void MainWindow::on_checkbox_use_environment_stateChanged(int state) {
	bool enabled;
	if ( state == 0 ) {
		enabled = true;
	} else {
		enabled = false;
	}
	ui.line_edit_master->setEnabled(enabled);

	//ui.line_edit_topic->setEnabled(enabled);
}

/*****************************************************************************
** Implemenation [Slots][manually connected]
*****************************************************************************/

/**
 * This function is signalled by the underlying model. When the model changes,
 * this will drop the cursor down to the last line in the QListview to ensure
 * the user can always see the latest log message.
 */


std::string prename="nobody.jpg";
std::string name1="nobody.jpg";
std::string name2="nobody.jpg";
 cv::Mat image;
void MainWindow::updateLoggingView() {
   QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
   QString timestr = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式
   //显示摄像头
   if(frame.cols>0){
        cv::Mat frame_img=frame;
       cv::cvtColor(frame_img,frame_img,CV_BGR2RGB);
       QImage img;
       img = QImage((const unsigned char*)(frame_img.data),frame_img.cols,frame_img.rows, frame_img.cols*frame_img.channels(),  QImage::Format_RGB888);
       ui.frame_label->setPixmap(QPixmap::fromImage(img).scaled(ui.frame_label->size(), Qt::KeepAspectRatio));


   }

   if(prename !=name&&name !="nobody.jpg")
      {
       //向上滚动
        name1=name2;
        name2=prename;
        //更新第三列
        std::string path="/home/tiger/文档/database/"+name;
        image=cv::imread(path);
         if(image.cols>0){
        cv::cvtColor(image,image,CV_BGR2RGB);
        QImage img3;
        img3 = QImage((const unsigned char*)(image.data),image.cols,image.rows, image.cols*image.channels(),  QImage::Format_RGB888);
       QString  str3 = QString::fromStdString(timestr.toStdString()+"\n"+name);
         ui.piclabel3->clear();
         ui.piclabel3->setPixmap(QPixmap::fromImage(img3).scaled(ui.piclabel3->size(), Qt::KeepAspectRatio));
         ui. namelabel3->setText(str3);
       //更新第二列
      path="/home/tiger/文档/database/"+name2;
      image=cv::imread(path);
      cv::cvtColor(image,image,CV_BGR2RGB);
        QImage img2;
        img2 = QImage((const unsigned char*)(image.data),image.cols,image.rows, image.cols*image.channels(),  QImage::Format_RGB888);

        QString  str2= QString::fromStdString(timestr.toStdString()+"\n"+name2);
         ui.piclabel2->clear();
        ui.piclabel2->setPixmap(QPixmap::fromImage(img2).scaled(ui.piclabel2->size(), Qt::KeepAspectRatio));
        ui. namelabel2->setText(str2);
         //第1列
        path="/home/tiger/文档/database/"+name1;
        image=cv::imread(path);
        cv::cvtColor(image,image,CV_BGR2RGB);
          QImage img1;
          img1 = QImage((const unsigned char*)(image.data),image.cols,image.rows, image.cols*image.channels(),  QImage::Format_RGB888);

          QString  str1= QString::fromStdString(timestr.toStdString()+"\n"+name1);
           ui.piclabel1->clear();
           ui.piclabel1->setPixmap(QPixmap::fromImage(img1).scaled(ui.piclabel1->size(), Qt::KeepAspectRatio));
           ui. namelabel1->setText(str1);

          prename=name;
         }
         else{std::cout<<"this person not exist in data base";}
   }

}

/*****************************************************************************
** Implementation [Menu]
*****************************************************************************/
void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, tr("About ..."),tr("<h2>PACKAGE_NAME Test Program 0.10</h2><p>Copyright Yujin Robot</p><p>This package needs an about description.</p>"));
}

/*****************************************************************************
** Implementation [Configuration]
*****************************************************************************/

void MainWindow::ReadSettings() {
    QSettings settings("Qt-Ros Package", "faceQT");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    QString master_url = settings.value("master_url",QString("http://192.168.1.2:11311/")).toString();
    QString host_url = settings.value("host_url", QString("192.168.1.3")).toString();
    //QString topic_name = settings.value("topic_name", QString("/chatter")).toString();
    ui.line_edit_master->setText(master_url);

    //ui.line_edit_topic->setText(topic_name);
    bool remember = settings.value("remember_settings", false).toBool();
    ui.checkbox_remember_settings->setChecked(remember);
    bool checked = settings.value("use_environment_variables", false).toBool();
    ui.checkbox_use_environment->setChecked(checked);
    if ( checked ) {
    	ui.line_edit_master->setEnabled(false);

    	//ui.line_edit_topic->setEnabled(false);
    }
}

void MainWindow::WriteSettings() {
    QSettings settings("Qt-Ros Package", "faceQT");
    settings.setValue("master_url",ui.line_edit_master->text());

    //settings.setValue("topic_name",ui.line_edit_topic->text());
    settings.setValue("use_environment_variables",QVariant(ui.checkbox_use_environment->isChecked()));
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("remember_settings",QVariant(ui.checkbox_remember_settings->isChecked()));

}

void MainWindow::closeEvent(QCloseEvent *event)
{
	WriteSettings();
	QMainWindow::closeEvent(event);
}

}  // namespace faceQT

