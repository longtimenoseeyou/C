#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include<math.h>
using namespace cv;
using namespace std;
//图像的三个颜色通道
#define BLUE 	0	
#define GREEN	1
#define RED 	2

#ifndef SCREEN_XY
#define SCREEN_XY(x, y)  (255 - ((255 - (x))*(255 - (y)) >> 8))//将新的图层与原图做滤色混合
//x为原始图像像素值，y为新图层的像素值
#endif

Mat dark_light(Mat src)
{
	int size = src.cols * src.rows;//获取原图像的大小
	uchar r = 0, g = 0, b = 0, g_alpha = 0;//定义参数初始值
	uchar* pSrcData = src.data;//获取图图像的像素值
	for (int i = 0; i < size; i++, pSrcData += 3)//
	{
		//将绿色通道反色，与b、g、r通道分别相乘，得到新的图层颜色
		g_alpha = 255 - pSrcData[GREEN];//将绿色通道反色
		b = pSrcData[BLUE] * g_alpha >> 8;
		g = pSrcData[GREEN] * g_alpha >> 8;
		r = pSrcData[RED] * g_alpha >> 8;

		//将上个步骤得到的新图层，与原始图做滤色混合，即执行f（a,b）=1-(1-a)*(1-b)的操作

		//pSrcData[BLUE] = SCREEN_XY(pSrcData[BLUE], b);
		//pSrcData[GREEN] = SCREEN_XY(pSrcData[GREEN], g);
		//pSrcData[RED] = SCREEN_XY(pSrcData[RED], r);



		//pSrcData[BLUE] = SCREEN_XY(pSrcData[BLUE], 3*b);
		//pSrcData[GREEN] = SCREEN_XY(pSrcData[GREEN], 3*g);
		//pSrcData[RED] = SCREEN_XY(pSrcData[RED], 3*r);


		pSrcData[BLUE] = SCREEN_XY(pSrcData[BLUE], b);
		pSrcData[GREEN] = SCREEN_XY(pSrcData[GREEN], g);
		pSrcData[RED] = SCREEN_XY(pSrcData[RED], r);


		pSrcData[BLUE] = SCREEN_XY(pSrcData[BLUE], b);
		pSrcData[GREEN] = SCREEN_XY(pSrcData[GREEN], g);
		pSrcData[RED] = SCREEN_XY(pSrcData[RED], r);




	}
	return src;
}

Mat& imgSharpen(const Mat& img, char* arith)       //arith为3*3模板算子
{
	int rows = img.rows;        //原图的行
	int cols = img.cols * img.channels();   //原图的列
	int offsetx = img.channels();       //像素点的偏移量

	static Mat dst = Mat::ones(img.rows - 2, img.cols - 2, img.type());

	for (int i = 1; i < rows - 1; i++)
	{
		const uchar* previous = img.ptr<uchar>(i - 1);
		const uchar* current = img.ptr<uchar>(i);
		const uchar* next = img.ptr<uchar>(i + 1);
		uchar* output = dst.ptr<uchar>(i - 1);
		for (int j = offsetx; j < cols - offsetx; j++)
		{
			output[j - offsetx] =
				saturate_cast<uchar>(previous[j - offsetx] * arith[0] + previous[j] * arith[1] + previous[j + offsetx] * arith[2] +
					current[j - offsetx] * arith[3] + current[j] * arith[4] + current[j + offsetx] * arith[5] +
					next[j - offsetx] * arith[6] + next[j] * arith[7] + next[j - offsetx] * arith[8]);
		}
	}
	return dst;
}



#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std::chrono;
int main()
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now(); //返回时间戳

	//方法一
	//Mat src = imread("00001.png");
	//Mat src = imread("NPE_89.png");
	Mat src = imread("0001.bmp");
	imshow("原图", src);
	namedWindow("原图", WINDOW_NORMAL);
	Mat out1 = dark_light(src);
	imshow("增强后的图像", out1);
	namedWindow("增强后的图像", WINDOW_NORMAL);

	//Mat out;
	//medianBlur(out1, out, 1);
	//imshow("滤波后的图像", out);

	Mat img = out1;
	cout << "rows = " << img.rows << endl;      //行
	cout << "cols = " << img.cols << endl;      //列
	cout << "channels = " << img.channels() << endl;      //通道
	char arith[9] = { 0, -1, 0, -1, 5, -1, 0, -1, 0 };       //使用拉普拉斯算子
	Mat dst1 = imgSharpen(img, arith);
	namedWindow("锐化图像", WINDOW_NORMAL);
	imshow("锐化图像", dst1);

	Mat out;
	medianBlur(dst1, out, 1);
	imshow("滤波后的图像", out);
	namedWindow("滤波后的图像", WINDOW_NORMAL);

	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	duration<double, std::milli> time_span = t2 - t1;

	std::cout << time_span.count() << std::endl;

	waitKey();

	return 0;
}

