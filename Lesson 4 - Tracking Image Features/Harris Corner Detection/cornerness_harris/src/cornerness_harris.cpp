#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

using namespace std;

void cornernessHarris()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1.png");
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY); // convert to grayscale

    // Detector parameters
    int blockSize = 2;     // for every pixel, a blockSize × blockSize neighborhood is considered
    int apertureSize = 3;  // aperture parameter for Sobel operator (must be odd)
    int minResponse = 100; // minimum value for a corner in the 8bit scaled response matrix
    double k = 0.04;       // Harris parameter (see equation for details)

    // Detect Harris corners and normalize output
    cv::Mat dst, dst_norm, dst_norm_scaled;
    dst = cv::Mat::zeros(img.size(), CV_32FC1);
    cv::cornerHarris(img, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT);
    cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat()); // shifts corner response values into 0-255 range
    cv::convertScaleAbs(dst_norm, dst_norm_scaled); // converts to 8 bit from the original CV_32FC1 (there is no scaling or abs; the name of the fn is confusing)

    // visualize results
    string windowName = "Harris Corner Detector Response Matrix";
    cv::namedWindow(windowName, 4);
    cv::imshow(windowName, dst_norm_scaled);
    cv::waitKey(0);

    // TODO: Your task is to locate local maxima in the Harris response matrix 
    // and perform a non-maximum suppression (NMS) in a local neighborhood around 
    // each maximum. The resulting coordinates shall be stored in a list of keypoints 
    // of the type `vector<cv::KeyPoint>`.


    std::vector<cv::KeyPoint> keypoints; // todo: make this a list initially, for the sake of the iteration you do below.

    for (int i=0; i<dst_norm.rows; ++i)
    {
        for (int j=0; j<dst_norm.cols; ++j) {
            auto & r = dst_norm.at<float>(i,j);
            if (r>minResponse)
                keypoints.emplace_back(float(j),float(i),2*apertureSize,-1,r);

        }
    }

    float repel_dist = 8.0;

    for (auto it = keypoints.begin(); it!=keypoints.end();) {
        bool delete_this_one = false;
        for (const auto & p : keypoints){
            if (dst_norm.at<float>(it->pt.y,it->pt.x) && (cv::norm(p.pt - it->pt) < repel_dist)) {
                if (p.pt == it->pt)
                    continue;
                delete_this_one=true;
                break;
            }
        }
        if (delete_this_one) {
            it = keypoints.erase(it);
        }
        else {
            ++it;
        }
    }

    // visualize keypoints
    windowName = "Harris Corner Detection Results";
    cv::namedWindow(windowName, 1);
    cv::Mat visImage = dst_norm_scaled.clone();
    cv::drawKeypoints(dst_norm_scaled, keypoints, visImage, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    cv::imshow(windowName, visImage);
    cv::waitKey(0);

}

int main()
{
    cornernessHarris();
}