#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<opencv2\core\core.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

void zhang_thinimage_improve(Mat& srcimage)//��ͨ������ֵ�����ͼ��  
{
    vector<Point> deletelist1;
    int Zhangmude[9];//Ŀ��㼰��8����
    int nl = srcimage.rows;//��
    int nc = srcimage.cols;//��
    while (true)
    {
        for (int j = 1; j < (nl - 1); j++)//����ÿ��
        {
            //��ȡ����ָ�룬ָ�����()�еĵ�row�е����
            uchar* data_last = srcimage.ptr<uchar>(j - 1);//��ȡ����������ֵ
            uchar* data = srcimage.ptr<uchar>(j);
            uchar* data_next = srcimage.ptr<uchar>(j + 1);
            for (int i = 1; i < (nc - 1); i++)//����ÿ��
            {
                if (data[i] == 255)//Ŀ��㼰8��������ֵ�ж�
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
                        //�õ�����ֵΪ255�ĸ���
                        whitepointtotal = whitepointtotal + Zhangmude[k];
                    }
                    if ((whitepointtotal >= 2) && (whitepointtotal <= 6))//�����ڵ����������1.1
                    {
                        //�õ�0->1�ĸ���
                        int ap = 0;
                        if ((Zhangmude[1] == 0) && (Zhangmude[2] == 1)) ap++;
                        if ((Zhangmude[2] == 0) && (Zhangmude[3] == 1)) ap++;
                        if ((Zhangmude[3] == 0) && (Zhangmude[4] == 1)) ap++;
                        if ((Zhangmude[4] == 0) && (Zhangmude[5] == 1)) ap++;
                        if ((Zhangmude[5] == 0) && (Zhangmude[6] == 1)) ap++;
                        if ((Zhangmude[6] == 0) && (Zhangmude[7] == 1)) ap++;
                        if ((Zhangmude[7] == 0) && (Zhangmude[8] == 1)) ap++;
                        if ((Zhangmude[8] == 0) && (Zhangmude[1] == 1)) ap++;
                        //����bp��λ���㣬������ת��
                        int bp = 0;
                        bp += Zhangmude[1];
                        bp += Zhangmude[2] << 1;
                        bp += Zhangmude[3] << 2;
                        bp += Zhangmude[4] << 3;
                        bp += Zhangmude[5] << 4;
                        bp += Zhangmude[6] << 5;
                        bp += Zhangmude[7] << 6;
                        bp += Zhangmude[8] << 7;
                        //�ж�����1.2
                        if (ap == 1 || bp == 65 || bp == 5 || bp == 20 || bp == 80 || bp == 13 || bp == 22 || bp == 52 || bp == 133 || bp == 141 || bp == 54)
                        {   //�ж�����1.3��1.4
                            if ((Zhangmude[1] * Zhangmude[7] * Zhangmude[5] == 0) && (Zhangmude[3] * Zhangmude[5] * Zhangmude[7] == 0))
                            {
                                deletelist1.push_back(Point(i, j));  //������������루����������ȥ���õ㣩
                            }
                        }
                    }
                }
            }
        }
        if (deletelist1.size() == 0) break;//������������
        //�����ڱ�ǵ㣬����ֵ��0
        for (size_t i = 0; i < deletelist1.size(); i++)
        {
            Point tem;
            tem = deletelist1[i];
            uchar* data = srcimage.ptr<uchar>(tem.y);
            data[tem.x] = 0;
        }
        deletelist1.clear();//�������

        //�ж�����2
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
    //Mat img = imread("opencv.jpg",0);//flag=0����ɫͨ����ȡ;flag=1,��ɫͨ����ȡ
    Mat img = imread("image.jpg", 0);
    imshow("�Ҷ�ͼ", img);
    Mat result;
    //��ֵ��ȡ�����ĹǼ���THRESH_BINARY,���ػҶ�ֵС����ֵȫΪ0,������ֵȫΪ255
    //��ֵ��ȡ�����Ǽ���THRESH_BINARY_INV�����ػҶ�ֵС����ֵȫΪ255,������ֵȫΪ0
    //threshold(img, result, 190, 255, THRESH_BINARY_INV);//image-55,opencv-190
    threshold(img, result, 55, 255, THRESH_BINARY);
    zhang_thinimage_improve(result);
    imshow("thinning", result);
    imwrite("./output.png", result);
    waitKey(0);
    return 0;
}
