#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Init();

    connect(this->ui->button_browse_0,SIGNAL(clicked(bool)),this,SLOT(button_browse_0_clicked()));
    connect(this->ui->button_browse_1,SIGNAL(clicked(bool)),this,SLOT(button_browse_1_clicked()));
    connect(this->ui->button_last,SIGNAL(clicked(bool)),this,SLOT(button_last_clicked()));
    connect(this->ui->button_next,SIGNAL(clicked(bool)),this,SLOT(button_next_clicked()));
    connect(this->ui->button_save,SIGNAL(clicked(bool)),this,SLOT(button_save_clicked()));
    connect(this->ui->button_help,SIGNAL(clicked(bool)),this,SLOT(button_help_clicked()));
    connect(this->ui->button_exit,SIGNAL(clicked(bool)),this,SLOT(button_exit_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::button_browse_0_clicked()
{
    model_path = QFileDialog::getOpenFileName(NULL,"Choose a model folder",".","*");
    Init();
}

void MainWindow::button_browse_1_clicked()
{
    image_folder_str = QFileDialog::getExistingDirectory(this,"select dir","",QFileDialog::ShowDirsOnly);
    Init();
}

void MainWindow::button_last_clicked()
{
    image_i -= 1;
    if (image_i < 0){
        image_i = image_num-1;
    }
    process_image(image_i);
}

void MainWindow::button_next_clicked()
{
    image_i += 1;
    if (image_i == image_num){
        image_i = 0;
    }
    process_image(image_i);
}

void MainWindow::button_save_clicked()
{
    if (QImage_pred.isNull()){
        QMessageBox::warning(NULL, "Warning", "No Predictions!");
        return;
    }
    QImage_pred.save("result.png", "png", 100);
    QMessageBox::information(NULL, "Image Saved", "Prediction image have saved!");
}

void MainWindow::button_help_clicked()
{
    QMessageBox::information(NULL, "Help", "For more information, please refer to http://yalongliu.github.io");
}

void MainWindow::button_exit_clicked()
{
    this->close();
}

void MainWindow::Init(void){
    if (!model_path.isEmpty()){  //Init model paths
        dnn_model.DNN_Init(model_path.toStdString());
    }
    if (!image_folder_str.isEmpty()){ //Init image paths
        QStringList nameFilters;
        nameFilters << "*.jpg" << "*.png" << "*.bmp";
        QDir image_folder_dir(image_folder_str);
        image_paths = image_folder_dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
        image_num = image_paths.size();
    }
    ui->statusBar->showMessage("UNet Nucleus Segmentation by Yalong Liu");
    ui->lineEdit_modelpath->setText(model_path);
    ui->lineEdit_imagepath->setText(image_folder_str);
}

void MainWindow::process_image(int image_i){
    if (image_paths.isEmpty()){
        QMessageBox::warning(NULL, "Warning", "Wrong ImagePath!");
        return;
    }
    if (model_path.isEmpty()){
        QMessageBox::warning(NULL, "Warning", "Wrong ModelPath!");
        return;
    }
    Mat srcImage = imread(image_folder_str.toStdString()+'\\'+image_paths[image_i].toLatin1().data());//Read image data
    if (srcImage.empty())
    {
        QMessageBox::warning(NULL, "Warning", "Image load Failed!");
        return;
    }
    Mat pred = dnn_model.DNN_inference(srcImage);  // dnn_model inference
    cvtColor(srcImage, srcImage, COLOR_BGR2RGB);//BGR to RGB

    QImage_img = QImage((const unsigned char*)(srcImage.data),srcImage.cols,srcImage.rows,QImage::Format_RGB888);
    ui->label_original_image->setPixmap(QPixmap::fromImage(QImage_img).scaled(256, 256, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));//show original image picture in label
    QImage_pred = QImage((const unsigned char*)(pred.data),pred.cols,pred.rows,QImage::Format_Grayscale8);
    ui->label_prediction->setPixmap(QPixmap::fromImage(QImage_pred));  //show pridiction picture in label

    ui->lineEdit_imagename->setText(image_paths[image_i]);  //show image name
    ui->lineEdit_index->setText(QString::number(image_i+1)+"/"+QString::number(image_num));  //show image name
    ui->lineEdit_time->setText(QString::fromStdString(dnn_model.time));  // show inference time
}
