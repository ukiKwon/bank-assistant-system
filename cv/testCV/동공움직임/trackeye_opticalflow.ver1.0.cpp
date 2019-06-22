
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

/* Module_1_facial feature detection */
void detectFaces(Mat&, vector<Rect_<int> >&);
void detectEyes(Mat&, vector<Rect_<int> >&);
void drawFaceNEyes(Mat& img, const vector<Rect_<int> > faces);

//cascade
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
//optical flow
Mat flow, cflow, frame;
UMat gray, prevgray, uflow;
vector <int> optical_flow;
void processOpticalFlow(Mat& eyeRegion, const Rect e);
void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
                    double, const Scalar& color);
int main(int argc, char** argv)
{
      //detection
      face_cascade.load("/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_alt2.xml");
      eyes_cascade.load("/usr/local/share/opencv4/haarcascades/haarcascade_eye.xml");
      // Check if everything is ok
      if (face_cascade.empty() || eyes_cascade.empty()) {
        cout << "File missing" << endl;
		    return 1;
      }
      //FaceLandmark
      // facemark->loadModel("./lbfmodel.yaml");// Load landmark detector

      int display_mode = 0;
      VideoCapture cap(0);
      cap.set(CAP_PROP_FRAME_WIDTH, 640);
      cap.set(CAP_PROP_FRAME_HEIGHT, 480);

      // Load image and cascade classifier files
      Mat frame;
      while (cap.read(frame)) {
          vector<Rect_<int> > faces;
          // display output
          switch (display_mode)
          {
              case 0: //superpixel contours
                  //Part1. 얼굴 찾고 얼굴 안에서 눈 찾고 사각 처리{Detect faces and facial features}
                  cout << " Tracking on" << endl;
                  detectFaces(frame, faces);
                  drawFaceNEyes(frame, faces);
                  imshow("Result", frame);
                  break;
              case 1: //mask
                  //Part2.
                  // Mat landmark_image = frame;
                  // drawFaceLandmark(landmark_image);
                  // imshow("Result", landmark_image);
                  cout << " Tracking off" << endl;
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
void drawFaceNEyes(Mat& img, const vector<Rect_<int> >faces)
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
          if(!eyes_cascade.empty())
          {
              //3.1. 얼굴에서 눈 찾기
              vector<Rect_<int> > eyes;
              detectEyes(ROI, eyes);
              //3.2. 눈 부분 사각 처리 {Mark points corresponding to the centre of the eyes}
              for(unsigned int j = 0; j < eyes.size(); ++j)
              {
                  Rect e = eyes[j];
                  //1. 단순히 동공찍기
                  // circle(ROI, Point(e.x+e.width/2, e.y+e.height/2), 3, Scalar(0, 255, 0), -1, 8);
                  rectangle(ROI, Point(e.x, e.y), Point(e.x+e.width, e.y+e.height), Scalar(0, 255, 0), 1, 4);
                  //2. 동공 방향찍기
                  // processOpticalFlow(img(Rect(e.x, e.y, e.width, e.height)));
                  processOpticalFlow(img, e);
              }
          }
        }
}
void detectFaces(Mat& img, vector<Rect_<int> >& faces)
{
  if (!face_cascade.empty())
    face_cascade.detectMultiScale(img, faces, 1.15, 3, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
  return;
}
void detectEyes(Mat& img, vector<Rect_<int> >& eyes)
{
    if (!eyes_cascade.empty())
        eyes_cascade.detectMultiScale(img, eyes, 1.20, 5, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
    return;
}
void processOpticalFlow(Mat& src, const Rect e)
{
    Mat ROI = src(Rect(e.x, e.y, e.width, e.height));
    cvtColor(ROI, gray, COLOR_BGR2GRAY);
    if( !prevgray.empty() )
    {
        calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
        cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
        uflow.copyTo(flow);
        //flow(방향성이 담긴 결과)와 cflow(흑백을 다시 컬러로 변환 시킨 원본)
        //flow는 움직인 방향 정보가 담겨있음.
        drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));//원본에다가 방향성이 담긴 정보를 담을 것임.
        imshow("flow", cflow);
    }
    std::swap(prevgray, gray);
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
