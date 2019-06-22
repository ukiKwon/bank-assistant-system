//face-eye detect
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


//c++
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace cv;

/* Module_1_facial feature detection */
static void detectFaces(Mat&, vector<Rect_<int> >&, string);
static void detectEyes(Mat&, vector<Rect_<int> >&, string);
static int detectEyes2(Mat& im, Mat& tpl, Rect& rect);
static void trackEyes(Mat& img, Mat& tpl, Rect& rect);
static void drawFaceNEyes(Mat& img, const vector<Rect_<int> > faces, string eye_cascade);

CascadeClassifier face_cascade;
CascadeClassifier eye_cascade;

int main(int argc, char** argv)
{
    face_cascade.load("/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_alt2.xml");
    eye_cascade.load("/usr/local/share/opencv4/haarcascades/haarcascade_eye.xml");
    // Check if everything is ok
	   if (face_cascade.empty() || eye_cascade.empty()) {
        cout << "File missing" << endl;
		    return 1;
      }
      VideoCapture cap(0);
      Mat src;
      // Set video to 320x240
      // cap.set(3, 630);
      // cap.set(4, 540);
      while (cap.read(src)) {
          // Invert the source image and convert to grayscale
        	Mat gray;
        	cvtColor(~src, gray, COLOR_BGR2GRAY);

        	// Convert to binary image by thresholding it
        	threshold(gray, gray, 220, 255, cv::THRESH_BINARY);

        	// Find all contours (외곽선의 점들을 뽑아냄)
        	vector<std::vector<cv::Point> > contours;
        	findContours(gray.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

        	// Fill holes in each contour
        	drawContours(gray, contours, -1, CV_RGB(255,255,255), -1);

        	for (int i = 0; i < contours.size(); i++)
        	{
        		double area = cv::contourArea(contours[i]);
        		Rect rect = cv::boundingRect(contours[i]);
        		int radius = rect.width/2;

        		// If contour is big enough and has round shape
        		// Then it is the pupil
        		if (area >= 30 &&
        		    abs(1 - ((double)rect.width / (double)rect.height)) <= 0.2 &&
        				abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
        		{
        			cv::circle(src, cv::Point(rect.x + radius, rect.y + radius), radius, CV_RGB(255,0,0), 2);
        		}
        	}
        	imshow("image", src);
          if (waitKey(1) == 27) break;
      }
    // Load image and cascade classifier files
    // Mat image;
    // while (cap.read(image)) {
    //     //Part1. 얼굴 찾고 얼굴 안에서 눈 찾고 사각 처리{Detect faces and facial features}
    //     vector<Rect_<int> > faces;
    //     // detectFaces(image, faces, face_cascade_path);
    //     detectFaces(image, faces, "/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_alt2.xml");
    //     // drawFaceNEyes(image, faces, eye_cascade_path);
    //     drawFaceNEyes(image, faces, "/usr/local/share/opencv4/haarcascades/haarcascade_eye_tree_eyeglasses.xml");
    //
    //     imshow("Result", image);
    //     if (waitKey(1) == 27) break;
    // }

    // eyeTracking
    // Mat frame, eye_tpl;
    // Rect eye_bb;
    // while (1)
    // {
    //     cap >> frame;
    //     if (frame.empty())
    //       break;
    //
    //     // Flip the frame horizontally, Windows users might need this
    //     flip(frame, frame, 1);
    //
    //     // Convert to grayscale and
    //     // adjust the image contrast using histogram equalization
    //     Mat gray;
    //     cvtColor(frame, gray, COLOR_BGR2GRAY);
    //
    //     if (eye_bb.width == 0 && eye_bb.height == 0)
    //     {
    //         // Detection stage
    //         // Try to detect the face and the eye of the user
    //         detectEyes2(gray, eye_tpl, eye_bb);
    //     }
    //     else
    //     {
    //       // Tracking stage with template matching
    //       trackEyes(gray, eye_tpl, eye_bb);
    //
    //       // Draw bounding rectangle for the eye
    //       rectangle(frame, eye_bb, CV_RGB(0,255,0));
    //     }
    //
    //     // Display video
    //     imshow("video", frame);
    //     if (waitKey(1) == 27) break;
    // }
    return 0;
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
static void trackEyes(Mat& im, Mat& tpl, Rect& rect)
{
  	Size size(rect.width * 2, rect.height * 2);
  	Rect window(rect + size - Point(size.width/2, size.height/2));

  	window &= Rect(0, 0, im.cols, im.rows);

  	Mat dst(window.width - tpl.rows + 1, window.height - tpl.cols + 1, CV_32FC1);
  	matchTemplate(im(window), tpl, dst, 1);

  	double minval, maxval;
  	Point minloc, maxloc;
  	minMaxLoc(dst, &minval, &maxval, &minloc, &maxloc);

  	if (minval <= 0.2)
  	{
  		rect.x = window.x + minloc.x;
  		rect.y = window.y + minloc.y;
  	}
  	else
  		rect.x = rect.y = rect.width = rect.height = 0;
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
static int detectEyes2(Mat& im, Mat& tpl, Rect& rect)
{
	vector<Rect> faces, eyes;
	face_cascade.detectMultiScale(im, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, cv::Size(30,30));

	for (int i = 0; i < faces.size(); i++)
	{
		Mat face = im(faces[i]);
		eye_cascade.detectMultiScale(face, eyes, 1.1, 2, 0|CASCADE_SCALE_IMAGE, cv::Size(20,20));

		if (eyes.size())
		{
			rect = eyes[0] + cv::Point(faces[i].x, faces[i].y);
			tpl  = im(rect);
		}
	}

	return eyes.size();
}
