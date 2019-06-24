#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include "opencv2/face/facemark_train.hpp"
#include <vector>

using namespace std;
using namespace cv;
using namespace cv::face;

int main()
{
      //1.카메라 읽기
      VideoCapture cap(0);
      if (!cap.isOpened()) {
          cerr << "Camera open failed" << endl;
          return -1;
      }
      //2.카메라 담기
      Mat frame;
      //3.facemark 모델
      // Create an instance of Facemark
      Ptr<Facemark> facemark = FacemarkLBF::create();
      // Load landmark detector
      facemark->loadModel("./lbfmodel.yaml");

      while (true) {
          cap >> frame;
          if (frame.empty()) {
              break;
          }
          // imshow("frame", frame);
          //얼굴 가져오기
          vector< cv::Rect> faces;
          face::CParams params("haarcascade_frontalface_alt.xml");
          cv::face::getFaces(frame, faces, &params);
          for(int j=0;j<faces.size();j++){
            cv::rectangle(frame, faces[j], cv::Scalar(255,0,255));
          }
          imshow("detection", frame);
          if (waitKey(10) == 27) //ESC waitKey
          {
            cap.release();
            break;
          }
          // vector< vector<Point2f> > landmarks;
          // facemark->getFaces(frame, faces);
          // facemark->fit(frame, faces, landmarks);
          // for(int j=0;j<rects.size();j++){
          //   face::drawFacemarks(frame, landmarks[j], Scalar(0,0,255));
          // }

      }
      destroyAllWindows();
      return 0;
}
