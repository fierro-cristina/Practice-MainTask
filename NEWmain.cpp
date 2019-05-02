#include <opencv2\opencv.hpp>
#include <opencv2\core\utility.hpp>
#include "opencv2\imgproc.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\highgui.hpp"
#include <vector>
#include <iostream> 
#include <stdio.h>

using namespace std;
using namespace cv;

void process(Mat src)
{
    ofstream file;
    file.open("PictureInfo.txt");
    Mat canny_output;
    Canny(src, canny_output, 100, 200, 3);
    dilate(canny_output, canny_output, Mat(), Point(-1, -1));

    /*namedWindow("Canny edge detection", WINDOW_AUTOSIZE);
    imshow("Canny edge detection", canny_output);*/


    vector<vector<Point>> contours;
   vector<Vec4i> hierarchy;
    findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);

    vector<Point> approxShape;

    for (size_t i = 0; i < contours.size(); i++)
    {
        Scalar white = Scalar(255, 255, 255);
        approxPolyDP(contours[i], approxShape, arcLength(Mat(contours[i]), true)*0.04, true);
        drawContours(drawing, contours, (int)i, white, CV_FILLED);
    }

    vector<double> areas(contours.size());
    vector<Point2d> centres(contours.size());

    int cnt = 1;

    for (int i = 0; i < contours.size(); i++)
    {
        areas[i] = contourArea(contours[i]);
        const Moments mu = moments(contours[i], false);
        centres[i] = Point2d(mu.m10 / mu.m00, mu.m01 / mu.m00);
        if (areas[i] >= 50)
        {
            file << "Area " << cnt << ": " << areas[i] << "\n";
            file << "Center " << cnt << ": " << centres[i].x << ", " << centres[i].y << "\n";
            cnt++;
        }
    }

    file << "No. of particles: " << cnt - 1 << "\n";
    file.close();
}


int main(int argc, const char** argv)
{
	Mat src = imread("C:/Users/Jazmin/Documents/UNIVERSITY/RICE/2.jpeg", CV_LOAD_IMAGE_GRAYSCALE);

	if (!src.data) 
	{
		cout << "Image not found!" << endl;
		return -1;
	}

	threshold(src, src, 100, 255, CV_THRESH_BINARY);

    process(src);

	waitKey(0);
	system("pause");
}
