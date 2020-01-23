// UNet Nucleus Segmentation by YalongLiu

#include <io.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <direct.h>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "unet_nucleus_seg.h"

using namespace cv;
using namespace dnn;
using namespace std;

DNN_SEG::DNN_SEG(void){

}

DNN_SEG::~DNN_SEG(void){

}

void DNN_SEG::DNN_Init(string model_path) try {
    confThreshold = 0.9;  //Threshold
    scale = 0.00390625f;  //0-1 to 0-255
    mean = Scalar{ 54,54,54 };  //mean value
    swapRB = true;  //Swap RGB to BGR(tf_model to opencv dnn model)
    inpWidth = 256;  // image width
    inpHeight = 256;  // image height
    net_input_layer_name = "input_1";  // input layer name in tensorflow model
    net_output_layer_name = "outputs";  // ouptut layer name
    net = cv::dnn::readNetFromTensorflow(model_path);// Read and initialize network
}
catch (std::exception & e) {
    std::cerr << e.what() << std::endl;
}

Mat DNN_SEG::DNN_inference(Mat img) {
    // Input
    Mat inputBlob = dnn::blobFromImage(img, scale, Size(inpWidth, inpHeight), Scalar(), true, false);// Create a 4D blob from a image
    net.setInput(inputBlob, net_input_layer_name); // Set input blob
    // DNN Inference
    Mat pred_ori = net.forward();
    dnn_pred = tensor2img(pred_ori, confThreshold);

    // Put efficiency information.
    std::vector<double> layersTimes;
    double freq = getTickFrequency() / 1000;
    double t = net.getPerfProfile(layersTimes) / freq;
    time = format("%.2f ms", t);  //output spend time
    return dnn_pred;
}

Mat tensor2img(const Mat &tensor, const float &threshold) {  //convert DNN output to visible image
    const int rows = tensor.size[2];
    const int cols = tensor.size[3];
    const int chns = tensor.size[1];
    Mat img_color = Mat::zeros(rows, cols, CV_8UC1); //Create a zeros mat
    for (int y = 0; y < rows; y++) {
        uchar* ptr = img_color.ptr<uchar>(y);
        for (int x = 0; x < cols; x++) {
            const float *ptrScore = tensor.ptr<float>(0, 0, y);
            if (ptrScore[x] > threshold) {   //output results by threshold
                ptr[x] = 255;
            }
        }
    }
    return img_color;
}
