#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("图片查看器");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    m_pictureFilePath = QFileDialog::getOpenFileName(this,"请选择图片","E://workSpase//QTProject//MyPictureViewer//data");
    if(m_pictureFilePath.isEmpty()){
        QMessageBox::warning(this,"选择图片","您没有选择图片！");
        return;
    }
    //加载图片
    m_pixmap = new QPixmap(m_pictureFilePath);
    if(!m_pixmap->load(m_pictureFilePath)){
        QMessageBox::warning(this,"图片加载","图片加载失败！");
        return;
    }
    //更新视图
    ui->widget->setPicture(m_pixmap);
    ui->widget->update();
}

