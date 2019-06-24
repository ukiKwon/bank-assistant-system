//face-eye detect
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

//optical_flow
#include "opencv2/video/tracking.hpp"
#include "opencv2/videoio.hpp"

//c++
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

/* Module_1_facial feature detection */
static void detectFaces(Mat&, vector<Rect_<int> >&, string);
static void detectEyes(Mat&, vector<Rect_<int> >&, string);
static void detectMouth(Mat&, vector<Rect_<int> >&, string);
static void drawFaceNEyes(Mat& img, const vector<Rect_<int> > faces, string eye_cascade);
string input_image_path;
string face_cascade_path, eye_cascade_path, mouth_cascade_path;

/* Module_2_optical_flow */
static void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step, double, const Scalar& color);

int main(int argc, char** argv)
{
    cv::CommandLineParser parser(argc, argv,
            "{eyes||}{nose||}{mouth||}{help h||}{@facexml||}");
    if (parser.has("help"))
    {
        return 0;
    }
    face_cascade_path = parser.get<string>("@facexml");
    eye_cascade_path = parser.has("eyes") ? parser.get<string>("eyes") : "";
    mouth_cascade_path = parser.has("mouth") ? parser.get<string>("mouth") : "";
    if (face_cascade_path.empty())
    {
        cout << "IMAGE or FACE_CASCADE are not specified";
        return 1;
    }
    VideoCapture cap(0);
    // Load image and cascade classifier files
    Mat image;
    while (cap.read(image)) {
        //Part1. 얼굴 찾고 얼굴 안에서 눈 찾고 사각 처리{Detect faces and facial features}
        vector<Rect_<int> > faces;
        detectFaces(image, faces, face_cascade_path);
        drawFaceNEyes(image, faces, eye_cascade_path);
        // imshow("Result", image);

        Mat flow, cflow;
        UMat gray, prevgray, uflow;
        // Create a mask image for drawing purposes
        cvtColor(image, gray, COLOR_BGR2GRAY);
        Mat mask = Mat::zeros(image.size(), image.type());

        if( !prevgray.empty() )
        {
            calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
            cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
            uflow.copyTo(flow);
            drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));
        }
        Mat img;
        add(image, mask, img);
        imshow("Result", img);
        if (waitKey(1) == 27) break;
        std::swap(prevgray, gray);
    }
    return 0;
}
static void findOpticalFlow(Mat& img, const vector <Rect_<int> > eyes)
{
  ;
}
static void drawFaceNEyes(Mat& img, const vector<Rect_<int> >faces, string eye_cascade)
{
    for(unsigned int i = 0; i < faces.size(); ++i)
    {
          //1. 전체 영역에서 얼굴 영역에 대하여 사각형 그리기 {Mark the bounding box enclosing the face}
          Rect face = faces[i];
          rectangle(img, Point(face.x, face.y), Point(face.x+face.width, face.y+face.height),
                  Scalar(255, 0, 0), 1, 4);

          //2. 얼굴 영역을 따로 추출해옴. {Eyes, nose and mouth will be detected inside the face (region of interest)}
          Mat ROI = img(Rect(face.x, face.y, face.width, face.height));
          //3. 얼굴 영역에 대해서 눈 부분을 찾고 사각형 처리(연산을 줄이기 위하여) {Detect eyes if classifier provided by the user}
          if(!eye_cascade.empty())
          {
              //3.1. 얼굴에서 눈 찾기
              vector<Rect_<int> > eyes;
              detectEyes(ROI, eyes, eye_cascade);

              //3.2. 눈 부분 사각 처리 {Mark points corresponding to the centre of the eyes}
              for(unsigned int j = 0; j < eyes.size(); ++j)
              {
                Rect e = eyes[j];
                rectangle(ROI, Point(e.x, e.y), Point(e.x+e.width, e.y+e.height), Scalar(0, 255, 0), 1, 4);
              }
          }
        }
}
static void detectFaces(Mat& img, vector<Rect_<int> >& faces, string cascade_path)
{
  CascadeClassifier face_cascade;
  face_cascade.load(samples::findFile(cascade_path));

  if (!face_cascade.empty())
  face_cascade.detectMultiScale(img, faces, 1.15, 3, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
  return;
}
static void detectEyes(Mat& img, vector<Rect_<int> >& eyes, string cascade_path)
{
    CascadeClassifier eyes_cascade;
    eyes_cascade.load(samples::findFile(cascade_path, !cascade_path.empty()));

    if (!eyes_cascade.empty())
        eyes_cascade.detectMultiScale(img, eyes, 1.20, 5, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
    return;
}
static void detectMouth(Mat& img, vector<Rect_<int> >& mouth, string cascade_path)
{
    CascadeClassifier mouth_cascade;
    mouth_cascade.load(samples::findFile(cascade_path, !cascade_path.empty()));

    if (!mouth_cascade.empty())
        mouth_cascade.detectMultiScale(img, mouth, 1.20, 5, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
    return;
}
//2_1_optical_flow
static void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
                    double, const Scalar& color)
{
    for(int y = 0; y < cflowmap.rows; y += step)
        for(int x = 0; x < cflowmap.cols; x += step)
        {
            const Point2f& fxy = flow.at<Point2f>(y, x);
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                 color);
            circle(cflowmap, Point(x,y), 2, color, -1);
        }
}
