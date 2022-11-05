#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//边缘检测
int main()
{
    Mat img = imread("jiao.jpg");

    //imshow("original figure ", img);

    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y, dst;

    //求x方向梯度
    Sobel(img, grad_x, CV_16S, 1, 0, 3, 1, 1,BORDER_DEFAULT);
    convertScaleAbs(grad_x, abs_grad_x);
    imshow("x direction of soble", abs_grad_x);

    //求y方向梯度
    Sobel(img, grad_y,CV_16S,0, 1,3, 1, 1, BORDER_DEFAULT);
    convertScaleAbs(grad_y,abs_grad_y);
    imshow("y direction of soble", abs_grad_y);

    //合并梯度
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
    imshow("overall direction soble", dst);
    cv:imwrite("output.jpg", dst);
    waitKey(0);
}
