#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
	//����ԭͼ
	Mat src = imread("0001.bmp");
    //����һ��������ʾ����ľ���
    Mat result;
    //��BGRͼ��ת��ΪYCBCR
    Mat ycrcb;
    cvtColor(src, ycrcb, COLOR_BGR2YCrCb);
    //��ycrcb��ֳ�������ͬ��ͨ�������Ե����õ�Y����
    vector<Mat> channels;
    split(ycrcb, channels);
    //ֱ��ͼ���⣬ֻ����Y����
    equalizeHist(channels[0], channels[0]);
    //��������y�����ϲ���Ycrcbͼ����
    merge(channels, ycrcb);
    //��ycrcbת��ΪBGR
    cvtColor(ycrcb, result, COLOR_YCrCb2BGR);
    //��ʾԭͼ
    namedWindow("src", WINDOW_NORMAL);
    imshow("src", src);
    //��ʾ������ͼ��
    namedWindow("equalizeHist", WINDOW_NORMAL);
    imshow("equalizeHist", result);
    src.release();
    ycrcb.release();
    result.release();
    waitKey(0);
    destroyWindow("equalizeHist");
	getchar();
}