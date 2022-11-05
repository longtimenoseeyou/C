#include "opencv2/imgproc.hpp"  
#include "opencv2/highgui.hpp"  
#include <iostream>  
#include <cmath>
using namespace cv;
using namespace std;
/*
RGBת���ɻҶ�ͼ���һ�����ù�ʽ�ǣ�
Gray = R*0.299 + G*0.587 + B*0.114
*/
//******************�Ҷ�ת������*************************  
//��һ������image����Ĳ�ɫRGBͼ������ã�  
//�ڶ�������imageGray��ת��������ĻҶ�ͼ������ã�  
//*******************************************************
void ConvertRGB2GRAY(const Mat &image, Mat &imageGray);
 
 
//****************����һά��˹��Ȩֵ����*****************
//��һ������size�Ǵ���ľ���˵ı߳��Ĵ�С
//�ڶ�������sigma��ʾ����sigma�Ĵ�С
//*******************************************************
double *getOneGuassionArray(int size, double sigma);
 
//****************��˹�˲�������ʵ��*****************
//��һ������srcImage�Ǵ���������ԭͼ
//�ڶ�������dst��ʾ���������ͼ
//����������size��ʾ���Ǿ���˵ı߳��Ĵ�С
//*******************************************************
void MyGaussianBlur(Mat &srcImage, Mat &dst, int size);
 
 
//******************Sobel������Ӽ���X��Y�����ݶȺ��ݶȷ����********************  
//��һ������imageSourcԭʼ�Ҷ�ͼ��  
//�ڶ�������imageSobelX��X�����ݶ�ͼ��  
//����������imageSobelY��Y�����ݶ�ͼ��  
//���ĸ�����pointDrection���ݶȷ��������ָ��  
//*************************************************************  
void SobelGradDirction(Mat &imageSource, Mat &imageSobelX, Mat &imageSobelY, double *&pointDrection);
 
//******************����Sobel��X��Y�����ݶȷ�ֵ*************************  
//��һ������imageGradX��X�����ݶ�ͼ��  
//�ڶ�������imageGradY��Y�����ݶ�ͼ��  
//����������SobelAmpXY�������X��Y�����ݶ�ͼ���ֵ  
//*************************************************************  
void SobelAmplitude(const Mat imageGradX, const Mat imageGradY, Mat &SobelAmpXY);
 
//******************�ֲ�����ֵ����*************************  
//��һ������imageInput�����Sobel�ݶ�ͼ��  
//�ڶ�������imageOutPut������ľֲ�����ֵ����ͼ��  
//����������pointDrection��ͼ����ÿ������ݶȷ�������ָ��  
//*************************************************************  
void LocalMaxValue(const Mat imageInput, Mat &imageOutput, double *pointDrection);
 
//******************˫��ֵ����*************************  
//��һ������imageInput���������ĵ�Sobel�ݶȷ�ֵͼ��  
//�ڶ�������lowThreshold�ǵ���ֵ  
//����������highThreshold�Ǹ���ֵ  
//******************************************************  
void DoubleThreshold(Mat &imageIput, double lowThreshold, double highThreshold);
 
//******************˫��ֵ�м��������Ӵ���*********************  
//��һ������imageInput���������ĵ�Sobel�ݶȷ�ֵͼ��  
//�ڶ�������lowThreshold�ǵ���ֵ  
//����������highThreshold�Ǹ���ֵ  
//*************************************************************  
void DoubleThresholdLink(Mat &imageInput, double lowThreshold, double highThreshold);
int main()
{
	const Mat srcImage = imread("1.jpg");
	if (!srcImage.data)
	{
		printf("could not load image...\n");
		return -1;
	}
	imshow("srcImage", srcImage);
	Mat srcGray;
	ConvertRGB2GRAY(srcImage, srcGray);
	Mat GaussianRes;
	MyGaussianBlur(srcGray, GaussianRes, 3);
	Mat imageSobelX;
	Mat imageSobelY;
	double *pointDirection = new double[(GaussianRes.cols - 2)*(GaussianRes.rows - 2)];  //�����ݶȷ�������� 
	SobelGradDirction(GaussianRes, imageSobelX, imageSobelY, pointDirection);  //����X��Y�����ݶȺͷ���� 
	Mat imageSobleXY;
	SobelAmplitude(imageSobelX, imageSobelY, imageSobleXY);
	Mat localMaxImage;
	LocalMaxValue(imageSobleXY, localMaxImage, pointDirection);
	imshow("Non-Maximum Image", localMaxImage);
	DoubleThreshold(localMaxImage, 60, 100);
	imshow("DoubleThr", localMaxImage);
	DoubleThresholdLink(localMaxImage, 60, 100);
	imshow("Canny Image", localMaxImage);
	imshow("srcGray", srcGray);
	imshow("GaussianRes", GaussianRes);
	imshow("SobleX", imageSobelX);
	imshow("SobleY", imageSobelY);
	imshow("SobleXY", imageSobleXY);
	
	waitKey(0);
	return 0;
}
 
void ConvertRGB2GRAY(const Mat &image, Mat &imageGray)
{
	if (!image.data || image.channels() != 3)
	{
		return;
	}
	
	imageGray = Mat::zeros(image.size(), CV_8UC1);
	
	uchar *pointImage = image.data;
	uchar *pointImageGray = imageGray.data;
	
	size_t stepImage = image.step;
	size_t stepImageGray = imageGray.step;
	for (int i = 0; i < imageGray.rows; i++)
	{
		for (int j = 0; j < imageGray.cols; j++)
		{
			pointImageGray[i*stepImageGray + j] = (uchar)(0.114*pointImage[i*stepImage + 3 * j] + 0.587*pointImage[i*stepImage + 3 * j + 1] + 0.299*pointImage[i*stepImage + 3 * j + 2]);
		}
	}
}
 
 
 
 
 
double *getOneGuassionArray(int size, double sigma)
{
	double sum = 0.0;
	
	int kerR = size / 2;
 
	
	double *arr = new double[size];
	for (int i = 0; i < size; i++)
	{
 
		
		arr[i] = exp(-((i - kerR)*(i - kerR)) / (2 * sigma*sigma));
		sum += arr[i];//�����е�ֵ�������
 
	}
	
	for (int i = 0; i < size; i++)
	{
		arr[i] /= sum;
		cout << arr[i] << endl;
	}
	return arr;
}
 
void MyGaussianBlur(Mat &srcImage, Mat &dst, int size)
{
	CV_Assert(srcImage.channels() == 1 || srcImage.channels() == 3); // ֻ����ͨ��������ͨ��ͼ��
	int kerR = size / 2;
	dst = srcImage.clone();
	int channels = dst.channels();
	double* arr;
	arr = getOneGuassionArray(size, 1);//�������˹����
 
									   
	for (int i = kerR; i < dst.rows - kerR; i++)
	{
		for (int j = kerR; j < dst.cols - kerR; j++)
		{
			double GuassionSum[3] = { 0 };
			
			for (int k = -kerR; k <= kerR; k++)
			{
 
				if (channels == 1)//���ֻ�ǵ�ͨ��
				{
					GuassionSum[0] += arr[kerR + k] * dst.at<uchar>(i, j + k);//�в��䣬�б任������ˮƽ����ľ��
				}
				else if (channels == 3)//�������ͨ�������
				{
					Vec3b bgr = dst.at<Vec3b>(i, j + k);
					auto a = arr[kerR + k];
					GuassionSum[0] += a*bgr[0];
					GuassionSum[1] += a*bgr[1];
					GuassionSum[2] += a*bgr[2];
				}
			}
			for (int k = 0; k < channels; k++)
			{
				if (GuassionSum[k] < 0)
					GuassionSum[k] = 0;
				else if (GuassionSum[k] > 255)
					GuassionSum[k] = 255;
			}
			if (channels == 1)
				dst.at<uchar>(i, j) = static_cast<uchar>(GuassionSum[0]);
			else if (channels == 3)
			{
				Vec3b bgr = { static_cast<uchar>(GuassionSum[0]), static_cast<uchar>(GuassionSum[1]), static_cast<uchar>(GuassionSum[2]) };
				dst.at<Vec3b>(i, j) = bgr;
			}
 
		}
	}
 
	
	for (int i = kerR; i < dst.rows - kerR; i++)
	{
		for (int j = kerR; j < dst.cols - kerR; j++)
		{
			double GuassionSum[3] = { 0 };
			//����������ɸ�˹��ƽ��
			for (int k = -kerR; k <= kerR; k++)
			{
 
				if (channels == 1)//���ֻ�ǵ�ͨ��
				{
					GuassionSum[0] += arr[kerR + k] * dst.at<uchar>(i + k, j);//�б䣬�в�����������ֱ����ľ��
				}
				else if (channels == 3)//�������ͨ�������
				{
					Vec3b bgr = dst.at<Vec3b>(i + k, j);
					auto a = arr[kerR + k];
					GuassionSum[0] += a*bgr[0];
					GuassionSum[1] += a*bgr[1];
					GuassionSum[2] += a*bgr[2];
				}
			}
			for (int k = 0; k < channels; k++)
			{
				if (GuassionSum[k] < 0)
					GuassionSum[k] = 0;
				else if (GuassionSum[k] > 255)
					GuassionSum[k] = 255;
			}
			if (channels == 1)
				dst.at<uchar>(i, j) = static_cast<uchar>(GuassionSum[0]);
			else if (channels == 3)
			{
				Vec3b bgr = { static_cast<uchar>(GuassionSum[0]), static_cast<uchar>(GuassionSum[1]), static_cast<uchar>(GuassionSum[2]) };
				dst.at<Vec3b>(i, j) = bgr;
			}
 
		}
	}
	delete[] arr;
}
 
 
 
void SobelGradDirction(Mat &imageSource, Mat &imageSobelX, Mat &imageSobelY, double *&pointDrection)
{
	
	pointDrection = new double[(imageSource.rows - 2)*(imageSource.cols - 2)];
	
	for (int i = 0; i < (imageSource.rows - 2)*(imageSource.cols - 2); i++)
	{
		pointDrection[i] = 0;
	}
	imageSobelX = Mat::zeros(imageSource.size(), CV_32SC1);
	imageSobelY = Mat::zeros(imageSource.size(), CV_32SC1);
	
	uchar *P = imageSource.data;
	uchar *PX = imageSobelX.data;
	uchar *PY = imageSobelY.data;
 
	//ȡ��ÿ����ռ�ݵ��ֽ���
	int step = imageSource.step;
	int stepXY = imageSobelX.step;
 
	int index = 0;//�ݶȷ���ǵ�����
	for (int i = 1; i < imageSource.rows - 1; ++i)
	{
		for (int j = 1; j < imageSource.cols - 1; ++j)
		{
			//ͨ��ָ�����ͼ����ÿһ������   
			double gradY = P[(i + 1)*step + j - 1] + P[(i + 1)*step + j] * 2 + P[(i + 1)*step + j + 1] - P[(i - 1)*step + j - 1] - P[(i - 1)*step + j] * 2 - P[(i - 1)*step + j + 1];
			PY[i*stepXY + j*(stepXY / step)] = abs(gradY);
 
			double gradX = P[(i - 1)*step + j + 1] + P[i*step + j + 1] * 2 + P[(i + 1)*step + j + 1] - P[(i - 1)*step + j - 1] - P[i*step + j - 1] * 2 - P[(i + 1)*step + j - 1];
			PX[i*stepXY + j*(stepXY / step)] = abs(gradX);
			if (gradX == 0)
			{
				gradX = 0.00000000000000001;  //��ֹ����Ϊ0�쳣  
			}
			pointDrection[index] = (atan(gradY / gradX)*180)/CV_PI;//����ת��Ϊ�� �Ƕȵķ�Χ��[-90,90] 
			pointDrection[index] += 90;//���Ƕȵķ�Χת��Ϊ[0,180]�����ڼ���
			index++;
			
		}
	}
	
	convertScaleAbs(imageSobelX, imageSobelX);
	convertScaleAbs(imageSobelY, imageSobelY);
}
 
 
void SobelAmplitude(const Mat imageGradX, const Mat imageGradY, Mat &SobelAmpXY)
{
	SobelAmpXY = Mat::zeros(imageGradX.size(), CV_32FC1);
	for (int i = 0; i < SobelAmpXY.rows; i++)
	{
		for (int j = 0; j < SobelAmpXY.cols; j++)
		{
			SobelAmpXY.at<float>(i,j)= sqrt(imageGradX.at<uchar>(i, j)*imageGradX.at<uchar>(i, j) + imageGradY.at<uchar>(i, j)*imageGradY.at<uchar>(i, j));
		}
	}
	convertScaleAbs(SobelAmpXY, SobelAmpXY);
}
 
 
 
void LocalMaxValue(const Mat imageInput, Mat &imageOutput, double *pointDrection)
{
	//����һ�������ͼ��
	imageOutput = imageInput.clone();
	int k = 0;
	for (int i = 1; i < imageInput.rows - 1; i++)
	{
		for (int j = 1; j < imageInput.cols - 1; j++)
		{
			/*
			value00  value01  value02
			value10  value11  value12
			value20  value21  value22
			*/
			//���ÿ���������ֵ
			int value00 = imageInput.at<uchar>(i - 1, j - 1);
			int value01 = imageInput.at<uchar>(i - 1, j);
			int value02 = imageInput.at<uchar>(i - 1, j + 1);
			int value10 = imageInput.at<uchar>(i , j - 1);
			int value11 = imageInput.at<uchar>(i , j);
			int value12 = imageInput.at<uchar>(i , j + 1);
			int value20 = imageInput.at<uchar>(i + 1, j - 1);
			int value21 = imageInput.at<uchar>(i + 1, j);
			int value22 = imageInput.at<uchar>(i + 1, j + 1);
			//����ݶȽ���[0,45]��֮��Ļ�
			if (pointDrection[k] > 0 && pointDrection[k] <= 45)
			{
				if ((value11 <= (value12 + (value02 - value12)*tan(pointDrection[k]))) || (value11 <= (value10 + (value20 - value10)*tan(pointDrection[k]))))
				{
					imageOutput.at<uchar>(i, j) = 0;
				}
			}
 
			//����ݶȽ���[45,90]��֮��Ļ�
			if (pointDrection[k] > 45 && pointDrection[k] <= 90)
			{
				if ((value11 <= (value01 + (value02 - value01)*tan(pointDrection[k]))) || (value11 <= (value21 + (value20 - value21)*tan(pointDrection[k]))))
				{
					imageOutput.at<uchar>(i, j) = 0;
				}
			}
 
			//����ݶȽ���[90,135]��֮��Ļ�
			if (pointDrection[k] > 90 && pointDrection[k] <= 135)
			{
				if ((value11 <= (value01 + (value00 - value01)*tan(180-pointDrection[k]))) || (value11 <= (value21 + (value22 - value21)*tan(180-pointDrection[k]))))
				{
					imageOutput.at<uchar>(i, j) = 0;
				}
			}
 
			//����ݶȽ���[135,180]��֮��Ļ�
			if (pointDrection[k] > 135 && pointDrection[k] <= 180)
			{
				if ((value11 <= (value10 + (value00 - value10)*tan(180 - pointDrection[k]))) || (value11 <= (value12 + (value22 - value12)*tan(180 - pointDrection[k]))))
				{
					imageOutput.at<uchar>(i, j) = 0;
				}
			}
			k++;
		}
	}
}
 
void DoubleThreshold(Mat &imageIput, double lowThreshold, double highThreshold)
{
	for (int i = 0; i < imageIput.rows; i++)
	{
		for (int j = 0; j < imageIput.cols; j++)
		{
			if (imageIput.at<uchar>(i, j) > highThreshold)
			{
				imageIput.at<uchar>(i, j) = 255;
			}
			if (imageIput.at<uchar>(i, j) < lowThreshold)
			{
				imageIput.at<uchar>(i, j) = 0;
			}
		}
	}
}
 
 
void DoubleThresholdLink(Mat &imageInput, double lowThreshold, double highThreshold)
{
	for (int i = 1; i < imageInput.rows - 1; i++)
	{
		for (int j = 1; j < imageInput.cols - 1; j++)
		{
			//�����ڸߵ���ֵ֮������صĵ�
			if (imageInput.at<uchar>(i, j) > lowThreshold && imageInput.at<uchar>(i, j) < 255)
			{
				if (imageInput.at<uchar>(i - 1, j - 1) == 255 || imageInput.at<uchar>(i - 1, j) == 255
					|| imageInput.at<uchar>(i - 1, j + 1) == 255 || imageInput.at<uchar>(i, j - 1) == 255
					|| imageInput.at<uchar>(i, j + 1) == 255 || imageInput.at<uchar>(i + 1, j - 1) == 255
					|| imageInput.at<uchar>(i + 1, j) == 255 || imageInput.at<uchar>(i + 1, j + 1) == 255)
				{
					imageInput.at<uchar>(i, j) = 255;
					DoubleThresholdLink(imageInput, lowThreshold, highThreshold);//�ݹ����˫��ֵ���Ӻ�����������
				}
				else
				{
					imageInput.at<uchar>(i, j) = 0;
				}
			}
		}
	}
}

