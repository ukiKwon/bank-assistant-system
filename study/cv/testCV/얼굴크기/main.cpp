
/*
  ver1.0 : faceLandMakr + face_eye_detect
*/
//opencv
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "drawLandmarks.hpp"
#include <opencv2/face.hpp>

//c++
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <cstring>

using namespace std;
using namespace cv;
using namespace cv::face;

/* Module_1_facial feature detection */
void detectFaces(Mat&, vector<Rect_<int> >&);
void detectEyes(Mat&, vector<Rect_<int> >&);
void drawFaceNEyes(Mat& img, const vector<Rect_<int> > faces);
void drawFaceLandmark(Mat& frame);

//cascade
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
//FaceLandmark
Ptr<Facemark> facemark = FacemarkLBF::create();// Create an instance of Facemark
//Head Change
const float threshold_head = 0.5;
typedef pair <float, float > pType;
pType headChange;
vector <pType> headXYhistory;
vector <int> occuredHeadChange;
float total_x, total_y;
void addHeadChange(vector<Rect_<int> >&face);
void calDiffOfHead();
bool sortingByY(pType& a, pType& b);
void clearData();
void fileProcess(float _x_var, float _y_var);
void drawCanvasPoint();
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
      facemark->loadModel("./lbfmodel.yaml");// Load landmark detector

      int display_mode = 0;
      VideoCapture cap(0);
      cap.set(CAP_PROP_FRAME_WIDTH, 640);
      cap.set(CAP_PROP_FRAME_HEIGHT, 480);

      // Load image and cascade classifier files
      Mat frame;
      while (cap.read(frame)) {
          vector<Rect_<int> > faces;
          static int stage = 0;
          // display output
          switch (display_mode)
          {
              case 0: //superpixel contours
                  //Part1. 얼굴 찾고 얼굴 안에서 눈 찾고 사각 처리{Detect faces and facial features}
                  detectFaces(frame, faces);
                  drawFaceNEyes(frame, faces);
                  addHeadChange(faces);
                  imshow("Result", frame);
                  break;
              case 1: //mask
                  //Part2.
                  Mat landmark_image = frame;
                  drawFaceLandmark(landmark_image);
                  imshow("Result", landmark_image);
                  break;
          }
          //toggle mode
          char c = waitKey(1);
          if( c == 'q' || c == 'Q' || c == 27 ) {
              cap.release();
              break;
          }
          else if( c == ' ' ) {
              //display_mode = (display_mode + 1) % 1;
              //HeadChange
              cout << "\n\n #### [ Stage " << ++stage << " ] start #### \n";
              calDiffOfHead();
              cout << "#### [ Stage " << stage << " ] end #### \n";
          }
      }
      //drawCanvasPoint();
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
                // circle(ROI, Point(e.x+e.width/2, e.y+e.height/2), 3, Scalar(0, 255, 255), -1, 8);
                circle(ROI, Point(e.x+e.width/2, e.y+e.height/2), 3, Scalar(0, 255, 0), -1, 8);
                // rectangle(ROI, Point(e.x, e.y), Point(e.x+e.width, e.y+e.height), Scalar(0, 255, 0), 1, 4);
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
void drawFaceLandmark(Mat& frame)
{
  // Variable to store a video frame and its grayscale
  Mat gray;
  // Find face
  vector<Rect> faces;
  // Convert frame to grayscale because
  // faceDetector requires grayscale image.
  cvtColor(frame, gray, COLOR_BGR2GRAY);

  // Detect faces
  face_cascade.detectMultiScale(gray, faces);

  // Variable for landmarks.
  // Landmarks for one face is a vector of points
  // There can be more than one face in the image. Hence, we
  // use a vector of vector of points.
  vector< vector<Point2f> > landmarks;

  // Run landmark detector
  bool success = facemark->fit(frame,faces,landmarks);

  if(success)
  {
    // If successful, render the landmarks on the face
    for(int i = 0; i < landmarks.size(); i++)
    {
      // drawLandmarks(frame, landmarks[i]);
      drawNetLandmarks(frame, landmarks[i]);
    }
  }
}
//아직까지는 Head 정보 출력
/*
  const float threshold_head = 0.5;
  typedef pair <float, float > pType;
  pType headChange = 0;
  vector <int> occuredHeadChange;
  vector <pType> headXYhistory;
  vector <float> headChangehistory;
*/
void addHeadChange(vector<Rect_<int> > &face)
{
    /* 현재 head 정보 */
    const int i = 0;
    float curHead = 0, newHead = 0;
    // if (i != 0) {
    //     curHead = headChange.first * headChange.y;
          //cout << ">> head size : " << curHead << endl;
    // }
    // float total_x = 0,
    //       total_y = 0;
    //get head
    for (uint i = 0; i < face.size(); ++i) {
        Rect f = face[i];
        total_x += f.x;
        total_y += f.y;
        headXYhistory.push_back(make_pair(f.x, f.y));
    }
    //한시적으로 여기에 넣어둠.
    // headChangehistory[0] = make_pair(total_x, total_y);
}
void calDiffOfHead()
{
    cout << " >> Working on calDiffOfHead...." << endl;
    //setting variables;
    // float total_x = headChangehistory[0].first,
    //       total_y = headChangehistory[0].second;
    /* head 변화율 계산 */
    //표준 편차 계산
    //cal head
    //1. headChange
    uint total_n = headXYhistory.size();
    cout << " >> sample : " << total_n << endl;
    float mean_x = total_x / total_n,
        mean_y = total_y / total_n;
    //get variance
    float x_variance = 0,
            y_variance = 0;
    for (uint i = 0; i < headXYhistory.size(); ++i) {
        pType e = headXYhistory[i];
        x_variance += pow(e.first - mean_x, 2);
        y_variance += pow(e.second - mean_y, 2);
    }
    x_variance = sqrt(x_variance / total_n);
    y_variance = sqrt(y_variance / total_n);
    cout << " >> x_variance : " << x_variance << endl;
    cout << " >> y_variance : " << y_variance << endl;
    fileProcess(x_variance, y_variance);
}
void clearData()
{
    total_x = 0;
    total_y = 0;
    headXYhistory.clear();
}
void fileProcess(float _x_var, float _y_var)
{
    static int findex = 0;
    //헤드 위치 좌표 각 계산
    cout << "\n >> File Processing now ..." << endl;
    cout << "> Each file is added !!!" << endl;
    const String file_name ="eachHeadXYChange";
    stringstream ss;
    ss << file_name << "_"<<  findex++ << ".md";
    ofstream each_history;
    each_history.open(ss.str());
    for (uint i = 0; i != headXYhistory.size(); ++i) {
      pType element = headXYhistory[i];
        each_history << '[' << element.first << ", " << element.second << "],\n";
    }
    each_history.close();

      //전체 표준편차 계산
    cout << "> file element added !!!" << endl;
    ofstream result_history;
    result_history.open("headchange.md", ios::out | ios_base::app);
    if (result_history.fail())
        throw std::ios_base::failure(std::strerror(errno));
    result_history << headXYhistory.size() << ' ' << _x_var << ' ' << _y_var << endl;
    result_history.close();
    clearData();
    return;
}
bool sortingByY(pType& a, pType& b) { return a.second > b.second;}
void drawCanvasPoint() {
    //sorting by largest Y-dimension
    sort(headXYhistory.begin(), headXYhistory.end(), sortingByY);

    char ptag = '.';
    //print them
    cout << "|";
    for (vector<pType>::iterator it = headXYhistory.begin();
      it != headXYhistory.end(); ++it)
      {
          float top = it->second;
      }
}
