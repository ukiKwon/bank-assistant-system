#ifndef _renderFace_H_
#define _renderFace_H_

using namespace cv;
using namespace std;

#define COLOR Scalar(255, 200,0)
#define COLOR_WHITE Scalar(255, 255, 255)

vector< pair <int, int> > netMarks{
  {0, 17}, {0, 36}, {1, 36}, {1, 41}, {2, 41}, {2, 31}, {3, 31}, {3, 48}, {4, 48}, {5, 48}, {5, 59},
  {6, 58}, {6, 59}, {7, 57}, {7, 58}, {8, 56}, {8, 57}, {9, 55}, {9, 56}, {10, 54}, {10, 55},
  {11, 54}, {12, 35}, {12, 54}, {13, 35}, {14, 35}, {14, 45}, {14, 46}, {15, 45}, {16, 26}, {16, 45},
  //
  {17, 36}, {18, 36}, {19, 36}, {19, 37}, {20, 37}, {20, 38}, {21, 38}, {21, 39}, {21, 27}, {21, 22},
  {22, 27}, {23, 42}, {23, 43}, {24, 43}, {24, 43}, {24, 44}, {25, 44}, {25, 45}, {26, 45},
  {27, 39}, {27, 42}, {28, 39}, {28, 42}, {29, 39}, {29, 42}, {29, 40}, {29, 31}, {29, 47}, {29, 35},
  {30, 31}, {30, 32}, {30, 34}, {30, 35}, {31, 40}, {31, 41}, {30, 48}, {30, 49}, {32, 49}, {32, 50},
  {33, 50}, {33, 52}, {34, 52}, {34, 53}, {35, 46}, {35, 47}, {35, 53}, {35, 54},
  //
  {48, 60}, {49, 60}, {49, 61}, {50, 51}, {50, 61}, {51, 61}, {51, 62}, {51, 63}, {52, 63}, {53, 63},
  {53, 64}, {54, 64}, {55, 64}, {56, 64}, {56, 65}, {57, 65}, {57, 66}, {58, 66}, {58, 67}, {59, 67},
  {14, 54}, {2, 48}
};
// drawPolyLine draws a poly line by joining
// successive points between the start and end indices.
void drawPolyline
(
  Mat &im,
  const vector<Point2f> &landmarks,
  const int start,
  const int end,
  bool isClosed = false
)
{
    // Gather all points between the start and end indices
    vector <Point> points;
    for (int i = start; i <= end; i++)
    {
        points.push_back(cv::Point(landmarks[i].x, landmarks[i].y));
    }
    // Draw polylines.
    // polylines(im, points, isClosed, COLOR, 2, 16);
    polylines(im, points, isClosed, COLOR_WHITE, 1, 16);

}
void drawLandmarks(Mat &im, vector<Point2f> &landmarks)
{
    // Draw face for the 68-point model.
    if (landmarks.size() == 68)
    {
      drawPolyline(im, landmarks, 0, 16);           // Jaw line
      drawPolyline(im, landmarks, 17, 21);          // Left eyebrow
      drawPolyline(im, landmarks, 22, 26);          // Right eyebrow
      drawPolyline(im, landmarks, 27, 30);          // Nose bridge
      drawPolyline(im, landmarks, 30, 35, true);    // Lower nose
      drawPolyline(im, landmarks, 36, 41, true);    // Left eye
      drawPolyline(im, landmarks, 42, 47, true);    // Right Eye
      drawPolyline(im, landmarks, 48, 59, true);    // Outer lip
      drawPolyline(im, landmarks, 60, 67, true);    // Inner lip
    }
    else
    { // If the number of points is not 68, we do not know which
      // points correspond to which facial features. So, we draw
      // one dot per landamrk.
      for(int i = 0; i < landmarks.size(); i++)
      {
        circle(im,landmarks[i],3, COLOR, FILLED);
      }
    }

}
//uki408
void drawNetLandmarks(Mat &im, vector<Point2f> &landmarks)
{
    if(landmarks.size() == 68) {
        drawPolyline(im, landmarks, 0, 16);           // Jaw line
        drawPolyline(im, landmarks, 17, 21);          // Left eyebrow
        drawPolyline(im, landmarks, 22, 26);          // Right eyebrow
        drawPolyline(im, landmarks, 27, 30);          // Nose bridge
        drawPolyline(im, landmarks, 30, 35, true);    // Lower nose
        drawPolyline(im, landmarks, 36, 41, true);    // Left eye
        drawPolyline(im, landmarks, 42, 47, true);    // Right Eye
        drawPolyline(im, landmarks, 48, 59, true);    // Outer lip
        drawPolyline(im, landmarks, 60, 67, true);    // Inner lip
        //net_facemark
        for (vector<pair<int, int> >::iterator it=netMarks.begin(); it !=netMarks.end(); ++it) {
            line(im, landmarks[it->first], landmarks[it->second], COLOR_WHITE, 1);
        }
    } else {
        drawLandmarks(im, landmarks);
    }
}
#endif // _renderFace_H_
