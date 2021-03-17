#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

void magnitudeSobel()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1gray.png");

    // convert image to grayscale
    cv::Mat imgGray;
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

    // apply smoothing using the GaussianBlur() function from the OpenCV
    // ToDo : Add your code here
    cv::Mat imgSmooth;
    cv::GaussianBlur(imgGray, imgSmooth, cv::Size(7,7), 2.0, 2.0);

    // create filter kernels using the cv::Mat datatype both for x and y
    float sobel_x_data[9] = {-1, 0, +1,
                             -2, 0, +2, 
                             -1, 0, +1};
    float sobel_y_data[9] = {-1, -2, -1,
                              0,  0,  0,    
                             +1, +2, +1};
    cv::Mat sobel_x(3,3, CV_32F, sobel_x_data);
    cv::Mat sobel_y(3,3, CV_32F, sobel_y_data);

    // apply filter using the OpenCv function filter2D()
    // ToDo : Add your code here
    cv::Mat sobel_x_img, sobel_y_img;
    cv::filter2D(imgSmooth, sobel_x_img, -1, sobel_x);
    cv::filter2D(imgSmooth, sobel_y_img, -1, sobel_y);


    

    // compute magnitude image based on the equation presented in the lesson 
    // ToDo : Add your code here
    cv::Mat magnitude(sobel_x_img.rows, sobel_x_img.cols, CV_8U);
    
    std::cout << sobel_x_img.at<uchar>(0,0) << std::endl;
    for (int i=0; i<sobel_x_img.rows; ++i)
        for (int j=0; j< sobel_x_img.cols; ++j)
            magnitude.at<uchar>(i,j) = cv::norm(cv::Vec2f(sobel_x_img.at<uchar>(i,j), sobel_y_img.at<uchar>(i,j)));
    

    // show result
    string windowName = "Gaussian Blurring";
    cv::namedWindow(windowName, 1); // create window
    cv::imshow(windowName, magnitude);
    cv::waitKey(0); // wait for keyboard input before continuing
}

int main()
{
    magnitudeSobel();
}