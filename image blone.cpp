#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<opencv2\core\core.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

void zhang_thinimage_improve(Mat& srcimage)//单通道、二值化后的图像  
{
    vector<Point> deletelist1;
    int Zhangmude[9];//目标点及其8领域
    int nl = srcimage.rows;//行
    int nc = srcimage.cols;//列
    while (true)
    {
        for (int j = 1; j < (nl - 1); j++)//遍历每行
        {
            //获取邻域指针，指向的是()中的第row行的起点
            uchar* data_last = srcimage.ptr<uchar>(j - 1);//获取这个点的像素值
            uchar* data = srcimage.ptr<uchar>(j);
            uchar* data_next = srcimage.ptr<uchar>(j + 1);
            for (int i = 1; i < (nc - 1); i++)//遍历每列
            {
                if (data[i] == 255)//目标点及8领域像素值判断
                {
                    Zhangmude[0] = 1;
                    if (data_last[i] == 255) Zhangmude[1] = 1;
                    else  Zhangmude[1] = 0;
                    if (data_last[i + 1] == 255) Zhangmude[2] = 1;
                    else  Zhangmude[2] = 0;
                    if (data[i + 1] == 255) Zhangmude[3] = 1;
                    else  Zhangmude[3] = 0;
                    if (data_next[i + 1] == 255) Zhangmude[4] = 1;
                    else  Zhangmude[4] = 0;
                    if (data_next[i] == 255) Zhangmude[5] = 1;
                    else  Zhangmude[5] = 0;
                    if (data_next[i - 1] == 255) Zhangmude[6] = 1;
                    else  Zhangmude[6] = 0;
                    if (data[i - 1] == 255) Zhangmude[7] = 1;
                    else  Zhangmude[7] = 0;
                    if (data_last[i - 1] == 255) Zhangmude[8] = 1;
                    else  Zhangmude[8] = 0;
                    int whitepointtotal = 0;
                    for (int k = 1; k < 9; k++)
                    {
                        //得到像素值为255的个数
                        whitepointtotal = whitepointtotal + Zhangmude[k];
                    }
                    if ((whitepointtotal >= 2) && (whitepointtotal <= 6))//非零邻点个数，条件1.1
                    {
                        //得到0->1的个数
                        int ap = 0;
                        if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;
                        if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;
                        if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;
                        if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;
                        if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;
                        if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;
                        if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;
                        if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;
                        //计算bp，位运算，二进制转换
                        int bp = 0;
                        bp += Zhangmude[1];
                        bp += Zhangmude[2] << 1;
                        bp += Zhangmude[3] << 2;
                        bp += Zhangmude[4] << 3;
                        bp += Zhangmude[5] << 4;
                        bp += Zhangmude[6] << 5;
                        bp += Zhangmude[7] << 6;
                        bp += Zhangmude[8] << 7;
                        //判断条件1.2
                        if (ap == 1 || bp == 65 || bp == 5 || bp == 20 || bp == 80 || bp == 13 || bp == 22 || bp == 52 || bp == 133 || bp == 141 || bp == 54)
                        {   //判断条件1.3和1.4
                            if ((Zhangmude[1] * Zhangmude[7] * Zhangmude[5] == 0) && (Zhangmude[3] * Zhangmude[5] * Zhangmude[7] == 0))
                            {
                                deletelist1.push_back(Point(i, j));  //在链表的最后插入（满足条件，去除该点）
                            }
                        }
                    }
                }
            }
        }
        if (deletelist1.size() == 0) break;//无满足条件点
        //链表内标记点，像素值置0
        for (size_t i = 0; i < deletelist1.size(); i++)
        {
            Point tem;
            tem = deletelist1[i];
            uchar* data = srcimage.ptr<uchar>(tem.y);
            data[tem.x] = 0;
        }
        deletelist1.clear();//清楚内容

        //判断条件2
        for (int j = 1; j < (nl - 1); j++)
        {
            uchar* data_last = srcimage.ptr<uchar>(j - 1);
            uchar* data = srcimage.ptr<uchar>(j);
            uchar* data_next = srcimage.ptr<uchar>(j + 1);
            for (int i = 1; i < (nc - 1); i++)
            {
                if (data[i] == 255)
                {
                    Zhangmude[0] = 1;
                    if (data_last[i] == 255) Zhangmude[1] = 1;
                    else  Zhangmude[1] = 0;
                    if (data_last[i + 1] == 255) Zhangmude[2] = 1;
                    else  Zhangmude[2] = 0;
                    if (data[i + 1] == 255) Zhangmude[3] = 1;
                    else  Zhangmude[3] = 0;
                    if (data_next[i + 1] == 255) Zhangmude[4] = 1;
                    else  Zhangmude[4] = 0;
                    if (data_next[i] == 255) Zhangmude[5] = 1;
                    else  Zhangmude[5] = 0;
                    if (data_next[i - 1] == 255) Zhangmude[6] = 1;
                    else  Zhangmude[6] = 0;
                    if (data[i - 1] == 255) Zhangmude[7] = 1;
                    else  Zhangmude[7] = 0;
                    if (data_last[i - 1] == 255) Zhangmude[8] = 1;
                    else  Zhangmude[8] = 0;
                    int whitepointtotal = 0;
                    for (int k = 1; k < 9; k++)
                    {
                        whitepointtotal = whitepointtotal + Zhangmude[k];
                    }
                    if ((whitepointtotal >= 2) && (whitepointtotal <= 6))
                    {
                        int ap = 0;
                        if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;
                        if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;
                        if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;
                        if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;
                        if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;
                        if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;
                        if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;
                        if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;
                        int bp = 0;
                        bp += Zhangmude[1];
                        bp += Zhangmude[2] << 1;
                        bp += Zhangmude[3] << 2;
                        bp += Zhangmude[4] << 3;
                        bp += Zhangmude[5] << 4;
                        bp += Zhangmude[6] << 5;
                        bp += Zhangmude[7] << 6;
                        bp += Zhangmude[8] << 7;
                        if (ap == 1 || bp == 65 || bp == 5 || bp == 20 || bp == 80 || bp == 13 || bp == 22 || bp == 52 || bp == 133 || bp == 141 || bp == 54)
                        {
                            if ((Zhangmude[1] * Zhangmude[3] * Zhangmude[5] == 0) && (Zhangmude[3] * Zhangmude[1] * Zhangmude[7] == 0))
                            {
                                deletelist1.push_back(Point(i, j));
                            }
                        }
                    }
                }
            }
        }
        if (deletelist1.size() == 0) break;
        for (size_t i = 0; i < deletelist1.size(); i++)
        {
            Point tem;
            tem = deletelist1[i];
            uchar* data = srcimage.ptr<uchar>(tem.y);
            data[tem.x] = 0;
        }
        deletelist1.clear();
    }
}

int main()
{
    //Mat img = imread("opencv.jpg",0);//flag=0，单色通道读取;flag=1,三色通道读取
    Mat img = imread("image.jpg", 0);
    imshow("灰度图", img);
    Mat result;
    //二值化取白区的骨架用THRESH_BINARY,像素灰度值小于阈值全为0,大于阈值全为255
    //二值化取黑区骨架用THRESH_BINARY_INV，像素灰度值小于阈值全为255,大于阈值全为0
    //threshold(img, result, 190, 255, THRESH_BINARY_INV);//image-55,opencv-190
    threshold(img, result, 55, 255, THRESH_BINARY);
    zhang_thinimage_improve(result);
    imshow("thinning", result);
    imwrite("./output.png", result);
    waitKey(0);
    return 0;
}
