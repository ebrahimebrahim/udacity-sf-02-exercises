#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>


#include "dataStructures.h"
#include "structIO.hpp"

using namespace std;

double minXWithinLane(const std::vector<LidarPoint> & lidarPoints, double laneWidthHalf) {
    double minX = 1e9;
    for (auto it = lidarPoints.begin(); it != lidarPoints.end(); ++it)
    {
        if ((it->y < -laneWidthHalf) || (it->y > laneWidthHalf))
            continue;
        minX = minX > it->x ? it->x : minX;
    }
    return minX;
}

void computeTTCLidar(std::vector<LidarPoint> &lidarPointsPrev,
                     std::vector<LidarPoint> &lidarPointsCurr, double &TTC)
{
    // auxiliary variables
    double dT = 0.1;        // time between two measurements in seconds
    double laneWidth = 4.0; // assumed width of the ego lane

    // find closest distance to Lidar points within ego lane
    double minXPrev = minXWithinLane(lidarPointsPrev, laneWidth/2.0);
    double minXCurr = minXWithinLane(lidarPointsCurr, laneWidth/2.0);

    // compute TTC from both measurements
    TTC = minXCurr * dT / (minXPrev - minXCurr);
}

int main()
{

    std::vector<LidarPoint> currLidarPts, prevLidarPts;
    readLidarPts("../dat/C22A5_currLidarPts.dat", currLidarPts);
    readLidarPts("../dat/C22A5_prevLidarPts.dat", prevLidarPts);


    double ttc;
    computeTTCLidar(prevLidarPts, currLidarPts, ttc);
    cout << "ttc = " << ttc << "s" << endl;
}