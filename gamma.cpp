#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void GammaTransform(const Mat& srcImage, Mat& dstImage, double gamma);

int main(int argc, char* argv[])
{
    Mat srcImage = imread("test1.jpg");
    if (!srcImage.data)
    {
        cerr << "fail to load image" << endl;
        exit(-1);
    }
    Mat dstImage, dstImage1;
    GammaTransform(srcImage, dstImage, 0.5);
    GammaTransform(srcImage, dstImage1, 4.0);
    imshow("srcImage", srcImage);
    imshow("gamma 0.5", dstImage);
    imshow("gamma 4.0", dstImage1);
    imwrite("gamma.jpg", dstImage);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
void GammaTransform(const Mat& srcImage, Mat& dstImage, double gamma)
{
    unsigned char lut[256];
    for (int i = 0; i < 256; i++)
    {
        lut[i] = saturate_cast<uchar>(pow((float)i / 255.0, gamma) * 255.0f);
    }
    dstImage = srcImage.clone();
    int channels = srcImage.channels();
    switch (channels)
    {
    case 1:
    {
        MatIterator_<uchar> it = dstImage.begin<uchar>();
        MatIterator_<uchar> end = dstImage.end<uchar>();
        while (it != end)
        {
            *it = lut[(*it)];
            it++;
        }
        break;
    }
    case 3:
    {
        MatIterator_<Vec3b> it = dstImage.begin<Vec3b>();
        MatIterator_<Vec3b> end = dstImage.end<Vec3b>();
        while (it != end)
        {
            (*it)[0] = lut[(*it)[0]];
            (*it)[1] = lut[(*it)[1]];
            (*it)[2] = lut[(*it)[2]];
            it++;
        }
        break;
    }
    default:
        break;
    }
}