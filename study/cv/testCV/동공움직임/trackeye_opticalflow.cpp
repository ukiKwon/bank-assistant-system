/*
  ver1.0 : faceLandMakr + face_eye_detect
*/
//opencv
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/face.hpp>
#include "opencv2/video/tracking.hpp"

//c++
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;
using namespace cv::face;

/* Module_facial feature detection */
void detectFaceFeatures(Mat& src, vector<Rect_<int> >& faces, vector<Rect_<int> >& eyes);
void detectFeature(Mat& src, vector<Rect_<int> >& target, CascadeClassifier& detector);
void drawFaceFeatures(Mat& src, vector<Rect_<int> >& faces, vector<Rect_<int> >& eyes);
void drawFeature(Mat& src, vector<Rect_<int> >& target, const Scalar color);
void detectOpticalDirection(Mat& src, vector<Rect_<int> >& eyesRegion);
//cascade
CascadeClassifier eyes_cascade;
CascadeClassifier face_cascade;

//optical flow
Mat flow, cflow;
UMat gray, prevgray, uflow;
// Mat flow, cflow, frame;
// UMat gray, prevgray, uflow;
// vector <int> optical_flow;
void detectOpticalDerection(Mat& src, vector<Rect_<int> >& eyesRegion);
void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
                    double, const Scalar& color);
int main(int argc, char** argv)
{
      // Load image and cascade classifier files
      face_cascade.load("/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_alt2.xml");
      eyes_cascade.load("/usr/local/share/opencv4/haarcascades/haarcascade_eye_tree_eyeglasses.xml");
      // Check if everything is ok
      if (face_cascade.empty() || eyes_cascade.empty()) {
          cout << ">> File missing" << endl;
  		    return -1;
      }
      //FaceLandmark
      // facemark->loadModel("./lbfmodel.yaml");// Load landmark detector
      int display_mode = 0;
      VideoCapture cap(0);
      cap.set(CAP_PROP_FRAME_WIDTH, 640);
      cap.set(CAP_PROP_FRAME_HEIGHT, 480);
      Mat frame;
      while (cap.read(frame)) {
          vector<Rect_<int> > faces, eyes;
          // display output
          switch (display_mode)
          {
              case 0: //superpixel contours
                  //part1. 얼굴/눈 찾고 추적(Detect and Track face features)
                  detectFaceFeatures(frame, faces, eyes);
                  drawFaceFeatures(frame, faces, eyes);
                  // detectOpticalDirection(frame, eyes);
                  imshow("Result", frame);
                  //part2. 동공 방향

                  break;
              case 1: //mask
                  //Part2.
                  // Mat landmark_image = frame;
                  // drawFaceLandmark(landmark_image);
                  // imshow("Result", landmark_image);
                  break;
          }
          //toggle mode
          char c = waitKey(1);
          if( c == 'q' || c == 'Q' || c == 27 ) {
              cap.release();
              break;
          }
          else if( c == ' ' )
              display_mode = (display_mode + 1) % 2;
      }
      return 0;
}
//각 얼굴 구성 요소 찾기
void detectFaceFeatures(Mat& src, vector<Rect_<int> >& faces, vector<Rect_<int> >& eyes)
{
    vector<Rect_<int> > multi_eyes;
    detectFeature(src, faces, face_cascade);//얼굴 찾기
    //얼굴 안 기준에서 각 구성 요소 찾기
    for (unsigned int i = 0; i < faces.size(); ++i) {
        Rect face = faces[i];
        //ROI 이미지 기준으로 eyes 좌표가 설정됨;전체 위치에서 (100, 100)이라도 (0, 0)식으로 설정됨
        Mat ROI = src(Rect(face.x, face.y, face.width, face.height));
        detectFeature(ROI, eyes, eyes_cascade);
        //전체 frame에 맞게 eyes 위치 재조정
        for (unsigned int j = 0; j < eyes.size(); ++j) {
            eyes[j].x += face.x;
            eyes[j].y += face.y;
        }
        multi_eyes.insert(multi_eyes.end(), eyes.begin(), eyes.end());
    }
    eyes = multi_eyes;
}
//얼굴 구성 요소 찾기
void detectFeature(Mat& src, vector<Rect_<int> >& target, CascadeClassifier& detector)
{
  if (!detector.empty())
    detector.detectMultiScale(src, target, 1.20, 5, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
  return;
}
//각 얼굴 구성 요소 그리기 또는 트랙킹
void drawFaceFeatures(Mat& src, vector<Rect_<int> >& faces, vector<Rect_<int> >& eyes)
{
    drawFeature(src, faces, Scalar(255, 0, 0));
    //얼굴 안 기준으로 각 구성요소 그리기
    for (unsigned int i = 0; i < faces.size(); ++i) {
        // Rect face = faces[i];
        // Mat ROI = src(Rect(face.x, face.y, face.width, face.height));
        // drawFeature(ROI, eyes, Scalar(0, 255, 0));
        drawFeature(src, eyes, Scalar(0, 255, 0));
    }
}
//얼굴 구성 요소 그리기
void drawFeature(Mat& src, vector<Rect_<int> >& target, const Scalar color)
{
    for (unsigned int i = 0; i < target.size(); ++i) {
        Rect t = target[i];
        rectangle(src, Point(t.x, t.y), Point(t.x + t.width, t.y + t.height),
                  color, 1, 4);
    }
}
void detectOpticalDirection(Mat& src, vector<Rect_<int> >& eyesRegion)
{
    for (unsigned int i = 0; i < eyesRegion.size(); ++i) {
        Rect e = eyesRegion[i];
        // Mat ROI = src(Rect(e.x, e.y, e.width, e.height));
        // cvtColor(ROI , gray, COLOR_BGR2GRAY);
        cvtColor(src , gray, COLOR_BGR2GRAY);
        if (!prevgray.empty())
        {
            calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
            cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
            uflow.copyTo(flow);
            //flow(방향성이 담긴 결과)와 cflow(흑백을 다시 컬러로 변환 시킨 원본)
            //flow는 움직인 방향 정보가 담겨있음.
            Mat ROI = cflow(Rect(e.x, e.y, e.width, e.height));
            drawOptFlowMap(flow, ROI, 16, 1.5, Scalar(0, 255, 0));//원본에다가 방향성이 담긴 정보를 담을 것임.
        }
        swap(prevgray, gray);
    }
}
void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
                    double, const Scalar& color)
{
    for(int y = 0; y < cflowmap.rows; y += step)
        for(int x = 0; x < cflowmap.cols; x += step)
        {
            const Point2f& fxy = flow.at<Point2f>(y, x);
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                 color);
            //uki-stop
            if (x == cvRound(x+fxy.x) && y == cvRound(y+fxy.y))
              cout << ">> 가만히 쳐다보는 중" << endl;
            else
            if (y < cvRound(y+fxy.y))
              cout << ">> 위에 쳐다봤음" << endl;
            else if (y > cvRound(y+fxy.y))
              cout << ">> 아래 쳐다봤음" << endl;
            else if (x < cvRound(x+fxy.x))
              cout << ">> 오른쪽 쳐다봤음" << endl;
            else if (x > cvRound(x+fxy.x))
              cout << ">> 왼쪽 쳐다봤음" << endl;
            else {
              cout << ">> 복잡한 상태임" << endl;
            }
            circle(cflowmap, Point(x,y), 2, color, -1);
        }
}
