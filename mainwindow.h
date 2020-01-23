#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDir>
#include "qfiledialog.h"
#include "unet_nucleus_seg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:  // slots
    void button_browse_0_clicked();
    void button_browse_1_clicked();
    void button_save_clicked();
    void button_last_clicked();
    void button_next_clicked();
    void button_help_clicked();
    void button_exit_clicked();

private:  //functions
    Ui::MainWindow *ui;
    void Init(void);
    void process_image(int image_num);

private:  //parameters
    QString model_path="unet_nucleus_seg_test.pb";  // path of UNet_nucleus_seg_test.pb file
    QStringList image_paths;  // paths of all image files
    int image_i=-1;  // image index
    DNN_SEG dnn_model;  // UNet model class
    QString image_folder_str="nucleus_images";  //string style of image folder
    int image_num=0;  // all image numbers
    QImage QImage_img;
    QImage QImage_pred;
};

#endif // MAINWINDOW_H
