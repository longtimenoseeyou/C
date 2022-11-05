#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
	//载入原图
	Mat src = imread("0001.bmp");
    //定义一个最终显示结果的矩阵
    Mat result;
    //将BGR图像转换为YCBCR
    Mat ycrcb;
    cvtColor(src, ycrcb, COLOR_BGR2YCrCb);
    //将ycrcb拆分成三个不同的通道，可以单独拿到Y分量
    vector<Mat> channels;
    split(ycrcb, channels);
    //直方图均衡，只均衡Y分量
    equalizeHist(channels[0], channels[0]);
    //将均衡后的y分量合并到Ycrcb图像中
    merge(channels, ycrcb);
    //将ycrcb转换为BGR
    cvtColor(ycrcb, result, COLOR_YCrCb2BGR);
    //显示原图
    namedWindow("src", WINDOW_NORMAL);
    imshow("src", src);
    //显示均衡后的图像
    namedWindow("equalizeHist", WINDOW_NORMAL);
    imshow("equalizeHist", result);
    src.release();
    ycrcb.release();
    result.release();
    waitKey(0);
    destroyWindow("equalizeHist");
	getchar();
}