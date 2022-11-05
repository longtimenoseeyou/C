#include "opencv2/imgproc.hpp"  
#include "opencv2/highgui.hpp"  
#include <iostream>  
#include <cmath>
using namespace cv;
using namespace std;
/*
RGB转换成灰度图像的一个常用公式是：
Gray = R*0.299 + G*0.587 + B*0.114
*/
//******************灰度转换函数*************************  
//第一个参数image输入的彩色RGB图像的引用；  
//第二个参数imageGray是转换后输出的灰度图像的引用；  
//*******************************************************
void ConvertRGB2GRAY(const Mat &image, Mat &imageGray);
 
 
//****************计算一维高斯的权值数组*****************
//第一个参数size是代表的卷积核的边长的大小
//第二个参数sigma表示的是sigma的大小
//*******************************************************
double *getOneGuassionArray(int size, double sigma);
 
//****************高斯滤波函数的实现*****************
//第一个参数srcImage是代表的输入的原图
//第二个参数dst表示的是输出的图
//第三个参数size表示的是卷积核的边长的大小
//*******************************************************
void MyGaussianBlur(Mat &srcImage, Mat &dst, int size);
 
 
//******************Sobel卷积因子计算X、Y方向梯度和梯度方向角********************  
//第一个参数imageSourc原始灰度图像；  
//第二个参数imageSobelX是X方向梯度图像；  
//第三个参数imageSobelY是Y方向梯度图像；  
//第四个参数pointDrection是梯度方向角数组指针  
//*************************************************************  
void SobelGradDirction(Mat &imageSource, Mat &imageSobelX, Mat &imageSobelY, double *&pointDrection);
 
//******************计算Sobel的X和Y方向梯度幅值*************************  
//第一个参数imageGradX是X方向梯度图像；  
//第二个参数imageGradY是Y方向梯度图像；  
//第三个参数SobelAmpXY是输出的X、Y方向梯度图像幅值  
//*************************************************************  
void SobelAmplitude(const Mat imageGradX, const Mat imageGradY, Mat &SobelAmpXY);
 
//******************局部极大值抑制*************************  
//第一个参数imageInput输入的Sobel梯度图像；  
//第二个参数imageOutPut是输出的局部极大值抑制图像；  
//第三个参数pointDrection是图像上每个点的梯度方向数组指针  
//*************************************************************  
void LocalMaxValue(const Mat imageInput, Mat &imageOutput, double *pointDrection);
 
//******************双阈值处理*************************  
//第一个参数imageInput输入和输出的的Sobel梯度幅值图像；  
//第二个参数lowThreshold是低阈值  
//第三个参数highThreshold是高阈值  
//******************************************************  
void DoubleThreshold(Mat &imageIput, double lowThreshold, double highThreshold);
 
//******************双阈值中间像素连接处理*********************  
//第一个参数imageInput输入和输出的的Sobel梯度幅值图像；  
//第二个参数lowThreshold是低阈值  
//第三个参数highThreshold是高阈值  
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
	double *pointDirection = new double[(GaussianRes.cols - 2)*(GaussianRes.rows - 2)];  //定义梯度方向角数组 
	SobelGradDirction(GaussianRes, imageSobelX, imageSobelY, pointDirection);  //计算X、Y方向梯度和方向角 
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
		sum += arr[i];//将所有的值进行相加
 
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
	CV_Assert(srcImage.channels() == 1 || srcImage.channels() == 3); // 只处理单通道或者三通道图像
	int kerR = size / 2;
	dst = srcImage.clone();
	int channels = dst.channels();
	double* arr;
	arr = getOneGuassionArray(size, 1);//先求出高斯数组
 
									   
	for (int i = kerR; i < dst.rows - kerR; i++)
	{
		for (int j = kerR; j < dst.cols - kerR; j++)
		{
			double GuassionSum[3] = { 0 };
			
			for (int k = -kerR; k <= kerR; k++)
			{
 
				if (channels == 1)//如果只是单通道
				{
					GuassionSum[0] += arr[kerR + k] * dst.at<uchar>(i, j + k);//行不变，列变换，先做水平方向的卷积
				}
				else if (channels == 3)//如果是三通道的情况
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
			//滑窗搜索完成高斯核平滑
			for (int k = -kerR; k <= kerR; k++)
			{
 
				if (channels == 1)//如果只是单通道
				{
					GuassionSum[0] += arr[kerR + k] * dst.at<uchar>(i + k, j);//行变，列不换，再做竖直方向的卷积
				}
				else if (channels == 3)//如果是三通道的情况
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
 
	//取出每行所占据的字节数
	int step = imageSource.step;
	int stepXY = imageSobelX.step;
 
	int index = 0;//梯度方向角的索引
	for (int i = 1; i < imageSource.rows - 1; ++i)
	{
		for (int j = 1; j < imageSource.cols - 1; ++j)
		{
			//通过指针遍历图像上每一个像素   
			double gradY = P[(i + 1)*step + j - 1] + P[(i + 1)*step + j] * 2 + P[(i + 1)*step + j + 1] - P[(i - 1)*step + j - 1] - P[(i - 1)*step + j] * 2 - P[(i - 1)*step + j + 1];
			PY[i*stepXY + j*(stepXY / step)] = abs(gradY);
 
			double gradX = P[(i - 1)*step + j + 1] + P[i*step + j + 1] * 2 + P[(i + 1)*step + j + 1] - P[(i - 1)*step + j - 1] - P[i*step + j - 1] * 2 - P[(i + 1)*step + j - 1];
			PX[i*stepXY + j*(stepXY / step)] = abs(gradX);
			if (gradX == 0)
			{
				gradX = 0.00000000000000001;  //防止除数为0异常  
			}
			pointDrection[index] = (atan(gradY / gradX)*180)/CV_PI;//弧度转换为度 角度的范围是[-90,90] 
			pointDrection[index] += 90;//将角度的范围转换为[0,180]，便于计算
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
	//复制一张输出的图像
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
			//求出每个点的像素值
			int value00 = imageInput.at<uchar>(i - 1, j - 1);
			int value01 = imageInput.at<uchar>(i - 1, j);
			int value02 = imageInput.at<uchar>(i - 1, j + 1);
			int value10 = imageInput.at<uchar>(i , j - 1);
			int value11 = imageInput.at<uchar>(i , j);
			int value12 = imageInput.at<uchar>(i , j + 1);
			int value20 = imageInput.at<uchar>(i + 1, j - 1);
			int value21 = imageInput.at<uchar>(i + 1, j);
			int value22 = imageInput.at<uchar>(i + 1, j + 1);
			//如果梯度角在[0,45]度之间的话
			if (pointDrection[k] > 0 && pointDrection[k] <= 45)
			{
				if ((value11 <= (value12 + (value02 - value12)*tan(pointDrection[k]))) || (value11 <= (value10 + (value20 - value10)*tan(pointDrection[k]))))
				{
					imageOutput.at<uchar>(i, j) = 0;
				}
			}
 
			//如果梯度角在[45,90]度之间的话
			if (pointDrection[k] > 45 && pointDrection[k] <= 90)
			{
				if ((value11 <= (value01 + (value02 - value01)*tan(pointDrection[k]))) || (value11 <= (value21 + (value20 - value21)*tan(pointDrection[k]))))
				{
					imageOutput.at<uchar>(i, j) = 0;
				}
			}
 
			//如果梯度角在[90,135]度之间的话
			if (pointDrection[k] > 90 && pointDrection[k] <= 135)
			{
				if ((value11 <= (value01 + (value00 - value01)*tan(180-pointDrection[k]))) || (value11 <= (value21 + (value22 - value21)*tan(180-pointDrection[k]))))
				{
					imageOutput.at<uchar>(i, j) = 0;
				}
			}
 
			//如果梯度角在[135,180]度之间的话
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
			//处理在高低阈值之间的像素的点
			if (imageInput.at<uchar>(i, j) > lowThreshold && imageInput.at<uchar>(i, j) < 255)
			{
				if (imageInput.at<uchar>(i - 1, j - 1) == 255 || imageInput.at<uchar>(i - 1, j) == 255
					|| imageInput.at<uchar>(i - 1, j + 1) == 255 || imageInput.at<uchar>(i, j - 1) == 255
					|| imageInput.at<uchar>(i, j + 1) == 255 || imageInput.at<uchar>(i + 1, j - 1) == 255
					|| imageInput.at<uchar>(i + 1, j) == 255 || imageInput.at<uchar>(i + 1, j + 1) == 255)
				{
					imageInput.at<uchar>(i, j) = 255;
					DoubleThresholdLink(imageInput, lowThreshold, highThreshold);//递归调用双阈值链接函数进行链接
				}
				else
				{
					imageInput.at<uchar>(i, j) = 0;
				}
			}
		}
	}
}

