
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
#include <vector>

using namespace std;
using namespace cv;
using namespace cv::face;

/* Module_1_facial feature detection */
void detectFaces(Mat&, vector<Rect_<int> >&);
void detectEyes(Mat&, vector<Rect_<int> >&);
void drawFaceNEyes(Mat& img, const vector<Rect_<int> > faces);
void drawFaceLandmark(Mat& frame);
void calDiffOfHead(vector<Rect_<int>&);
//cascade
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
//FaceLandmark
Ptr<Facemark> facemark = FacemarkLBF::create();// Create an instance of Facemark

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
          // display output
          switch (display_mode)
          {
              case 0: //superpixel contours
                  //Part1. 얼굴 찾고 얼굴 안에서 눈 찾고 사각 처리{Detect faces and facial features}
                  detectFaces(frame, faces);
                  drawFaceNEyes(frame, faces);
                  calDiffOfHead(faces);
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
void calDiffOfHead(vector<Rect_<int>& face)
{
    //현재 head 정보
    //
}
