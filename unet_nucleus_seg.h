#ifndef UNET_NUCLEUS_SEG_H
#define UNET_NUCLEUS_SEG_H

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <io.h>
#include <sstream>
#include <fstream>
#include <direct.h>

using namespace cv;
using namespace dnn;
using namespace std;

Mat tensor2img(const Mat &tensor, const float &threshold);

//extern "C" class __declspec(dllexport) DNN_SEG;
class DNN_SEG
{
public:
    //    ~DNN_SEG();
    void DNN_Init(string model_path);
    Mat DNN_inference(Mat img);

    DNN_SEG();
    ~DNN_SEG();

    float confThreshold, scale;
    cv::Scalar mean;
    bool swapRB;
    int inpWidth, inpHeight;  // i start from 2 to avoid .. or . in file_paths
    String net_input_layer_name, net_output_layer_name, time="--ms";
    Mat dnn_pred; //input image
    dnn::Net net;
};
#endif
