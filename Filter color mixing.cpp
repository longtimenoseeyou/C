#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include<math.h>
using namespace cv;
using namespace std;
//ͼ���������ɫͨ��
#define BLUE 	0	
#define GREEN	1
#define RED 	2

#ifndef SCREEN_XY
#define SCREEN_XY(x, y)  (255 - ((255 - (x))*(255 - (y)) >> 8))//���µ�ͼ����ԭͼ����ɫ���
//xΪԭʼͼ������ֵ��yΪ��ͼ�������ֵ
#endif

Mat dark_light(Mat src)
{
	int size = src.cols * src.rows;//��ȡԭͼ��Ĵ�С
	uchar r = 0, g = 0, b = 0, g_alpha = 0;//���������ʼֵ
	uchar* pSrcData = src.data;//��ȡͼͼ�������ֵ
	for (int i = 0; i < size; i++, pSrcData += 3)//
	{
		//����ɫͨ����ɫ����b��g��rͨ���ֱ���ˣ��õ��µ�ͼ����ɫ
		g_alpha = 255 - pSrcData[GREEN];//����ɫͨ����ɫ
		b = pSrcData[BLUE] * g_alpha >> 8;
		g = pSrcData[GREEN] * g_alpha >> 8;
		r = pSrcData[RED] * g_alpha >> 8;

		//���ϸ�����õ�����ͼ�㣬��ԭʼͼ����ɫ��ϣ���ִ��f��a,b��=1-(1-a)*(1-b)�Ĳ���

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

Mat& imgSharpen(const Mat& img, char* arith)       //arithΪ3*3ģ������
{
	int rows = img.rows;        //ԭͼ����
	int cols = img.cols * img.channels();   //ԭͼ����
	int offsetx = img.channels();       //���ص��ƫ����

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
	high_resolution_clock::time_point t1 = high_resolution_clock::now(); //����ʱ���

	//����һ
	//Mat src = imread("00001.png");
	//Mat src = imread("NPE_89.png");
	Mat src = imread("0001.bmp");
	imshow("ԭͼ", src);
	namedWindow("ԭͼ", WINDOW_NORMAL);
	Mat out1 = dark_light(src);
	imshow("��ǿ���ͼ��", out1);
	namedWindow("��ǿ���ͼ��", WINDOW_NORMAL);

	//Mat out;
	//medianBlur(out1, out, 1);
	//imshow("�˲����ͼ��", out);

	Mat img = out1;
	cout << "rows = " << img.rows << endl;      //��
	cout << "cols = " << img.cols << endl;      //��
	cout << "channels = " << img.channels() << endl;      //ͨ��
	char arith[9] = { 0, -1, 0, -1, 5, -1, 0, -1, 0 };       //ʹ��������˹����
	Mat dst1 = imgSharpen(img, arith);
	namedWindow("��ͼ��", WINDOW_NORMAL);
	imshow("��ͼ��", dst1);

	Mat out;
	medianBlur(dst1, out, 1);
	imshow("�˲����ͼ��", out);
	namedWindow("�˲����ͼ��", WINDOW_NORMAL);

	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	duration<double, std::milli> time_span = t2 - t1;

	std::cout << time_span.count() << std::endl;

	waitKey();

	return 0;
}

