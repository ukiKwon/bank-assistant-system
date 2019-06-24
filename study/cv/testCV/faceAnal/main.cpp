
/*
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

static void help()
{
    cout << "\nThis program demonstrates the smile detector.\n"
            "Usage:\n"
            "./smiledetect [--cascade=<cascade_path> this is the frontal face classifier]\n"
            "   [--smile-cascade=[<smile_cascade_path>]]\n"
            "   [--scale=<image scale greater or equal to 1, try 2.0 for example. The larger the faster the processing>]\n"
            "   [--try-flip]\n"
            "   [video_filename|camera_index]\n\n"
            "Example:\n"
            "./smiledetect --cascade=\"data/haarcascades/haarcascade_frontalface_alt.xml\" --smile-cascade=\"data/haarcascades/haarcascade_smile.xml\" --scale=2.0\n\n"
            "During execution:\n\tHit any key to quit.\n"
            "\tUsing OpenCV version " << CV_VERSION << "\n" << endl;
}

void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip );

string cascadeName;
string nestedCascadeName;

int main( int argc, const char** argv )
{
    VideoCapture capture;
    Mat frame, image;
    string inputName;
    bool tryflip;

    help();

    CascadeClassifier cascade, nestedCascade;
    double scale;
    cv::CommandLineParser parser(argc, argv,
        "{help h||}{scale|1|}"
        "{cascade|data/haarcascades/haarcascade_frontalface_alt.xml|}"
        "{smile-cascade|data/haarcascades/haarcascade_smile.xml|}"
        "{try-flip||}{@input||}");
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    cascadeName = samples::findFile(parser.get<string>("cascade"));
    nestedCascadeName = samples::findFile(parser.get<string>("smile-cascade"));
    tryflip = parser.has("try-flip");
    inputName = parser.get<string>("@input");
    scale = parser.get<int>("scale");
    if (!parser.check())
    {
        help();
        return 1;
    }
    if (scale < 1)
        scale = 1;
    if( !cascade.load( cascadeName ) )
    {
        cerr << "ERROR: Could not load face cascade" << endl;
        help();
        return -1;
    }
    if( !nestedCascade.load( nestedCascadeName ) )
    {
        cerr << "ERROR: Could not load smile cascade" << endl;
        help();
        return -1;
    }
    if( inputName.empty() || (isdigit(inputName[0]) && inputName.size() == 1) )
    {
        int c = inputName.empty() ? 0 : inputName[0] - '0' ;
        if(!capture.open(c))
            cout << "Capture from camera #" <<  c << " didn't work" << endl;
    }
    else if( inputName.size() )
    {
        inputName = samples::findFileOrKeep(inputName);
        if(!capture.open( inputName ))
            cout << "Could not read " << inputName << endl;
    }

    if( capture.isOpened() )
    {
        cout << "Video capturing has been started ..." << endl;
        cout << endl << "NOTE: Smile intensity will only be valid after a first smile has been detected" << endl;

        for(;;)
        {
            capture >> frame;
            if( frame.empty() )
                break;

            Mat frame1 = frame.clone();
            detectAndDraw( frame1, cascade, nestedCascade, scale, tryflip );

            char c = (char)waitKey(10);
            if( c == 27 || c == 'q' || c == 'Q' )
                break;
        }
    }
    else
    {
        cerr << "ERROR: Could not initiate capture" << endl;
        help();
        return -1;
    }

    return 0;
}

void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip)
{
    vector<Rect> faces, faces2;
    const static Scalar colors[] =
    {
        Scalar(255,0,0),
        Scalar(255,128,0),
        Scalar(255,255,0),
        Scalar(0,255,0),
        Scalar(0,128,255),
        Scalar(0,255,255),
        Scalar(0,0,255),
        Scalar(255,0,255)
    };
    Mat gray, smallImg;

    cvtColor( img, gray, COLOR_BGR2GRAY );

    double fx = 1 / scale;
    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT );
    equalizeHist( smallImg, smallImg );

    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(30, 30) );
    if( tryflip )
    {
        flip(smallImg, smallImg, 1);
        cascade.detectMultiScale( smallImg, faces2,
                                 1.1, 2, 0
                                 //|CASCADE_FIND_BIGGEST_OBJECT
                                 //|CASCADE_DO_ROUGH_SEARCH
                                 |CASCADE_SCALE_IMAGE,
                                 Size(30, 30) );
        for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r )
        {
            faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
        }
    }

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Rect r = faces[i];
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;

        double aspect_ratio = (double)r.width/r.height;
        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
        {
            center.x = cvRound((r.x + r.width*0.5)*scale);
            center.y = cvRound((r.y + r.height*0.5)*scale);
            radius = cvRound((r.width + r.height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
        else
            rectangle( img, Point(cvRound(r.x*scale), cvRound(r.y*scale)),
                       Point(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)),
                       color, 3, 8, 0);

        const int half_height=cvRound((float)r.height/2);
        r.y=r.y + half_height;
        r.height = half_height-1;
        smallImgROI = smallImg( r );
        nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
            1.1, 0, 0
            //|CASCADE_FIND_BIGGEST_OBJECT
            //|CASCADE_DO_ROUGH_SEARCH
            //|CASCADE_DO_CANNY_PRUNING
            |CASCADE_SCALE_IMAGE,
            Size(30, 30) );

        // The number of detected neighbors depends on image size (and also illumination, etc.). The
        // following steps use a floating minimum and maximum of neighbors. Intensity thus estimated will be
        //accurate only after a first smile has been displayed by the user.
        const int smile_neighbors = (int)nestedObjects.size();
        static int max_neighbors=-1;
        static int min_neighbors=-1;
        if (min_neighbors == -1) min_neighbors = smile_neighbors;
        max_neighbors = MAX(max_neighbors, smile_neighbors);

        // Draw rectangle on the left side of the image reflecting smile intensity
        float intensityZeroOne = ((float)smile_neighbors - min_neighbors) / (max_neighbors - min_neighbors + 1);
        int rect_height = cvRound((float)img.rows * intensityZeroOne);
        Scalar col = Scalar((float)255 * intensityZeroOne, 0, 0);
        rectangle(img, Point(0, img.rows), Point(img.cols/10, img.rows - rect_height), col, -1);
    }

    imshow( "result", img );
}
*/
/*
  * 백그라운드에 대해서 배운다는 의미가 정확히 어떤건지는 모르곘음
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <stdio.h>
#include <string>

using namespace std;
using namespace cv;

static void help()
{
    printf("\n"
            "This program demonstrated a simple method of connected components clean up of background subtraction\n"
            "When the program starts, it begins learning the background.\n"
            "You can toggle background learning on and off by hitting the space bar.\n"
            "Call\n"
            "./segment_objects [video file, else it reads camera 0]\n\n");
}

static void refineSegments(const Mat& img, Mat& mask, Mat& dst)
{
    int niters = 3;

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Mat temp;

    dilate(mask, temp, Mat(), Point(-1,-1), niters);
    erode(temp, temp, Mat(), Point(-1,-1), niters*2);
    dilate(temp, temp, Mat(), Point(-1,-1), niters);

    findContours( temp, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE );

    dst = Mat::zeros(img.size(), CV_8UC3);

    if( contours.size() == 0 )
        return;

    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    int idx = 0, largestComp = 0;
    double maxArea = 0;

    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
        const vector<Point>& c = contours[idx];
        double area = fabs(contourArea(Mat(c)));
        if( area > maxArea )
        {
            maxArea = area;
            largestComp = idx;
        }
    }
    Scalar color( 0, 0, 255 );
    drawContours( dst, contours, largestComp, color, FILLED, LINE_8, hierarchy );
}


int main(int argc, char** argv)
{
    VideoCapture cap;
    bool update_bg_model = true;

    CommandLineParser parser(argc, argv, "{help h||}{@input||}");
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    string input = parser.get<std::string>("@input");
    if (input.empty())
        cap.open(0);
    else
        cap.open(samples::findFileOrKeep(input));

    if( !cap.isOpened() )
    {
        printf("\nCan not open camera or video file\n");
        return -1;
    }

    Mat tmp_frame, bgmask, out_frame;

    cap >> tmp_frame;
    if(tmp_frame.empty())
    {
        printf("can not read data from the video source\n");
        return -1;
    }

    namedWindow("video", 1);
    namedWindow("segmented", 1);

    Ptr<BackgroundSubtractorMOG2> bgsubtractor=createBackgroundSubtractorMOG2();
    bgsubtractor->setVarThreshold(10);

    for(;;)
    {
        cap >> tmp_frame;
        if( tmp_frame.empty() )
            break;
        bgsubtractor->apply(tmp_frame, bgmask, update_bg_model ? -1 : 0);
        refineSegments(tmp_frame, bgmask, out_frame);
        imshow("video", tmp_frame);
        imshow("segmented", out_frame);
        char keycode = (char)waitKey(30);
        if( keycode == 27 )
            break;
        if( keycode == ' ' )
        {
            update_bg_model = !update_bg_model;
            printf("Learn background is in state = %d\n",update_bg_model);
        }
    }

    return 0;
}
*/

/*
  * 저장된 비디오 파일에 대하여 EyeFlow를 찾는것 같은데, 크기가 안 맞는지 동작이 안 됨. (dis_opticalflow.cpp)
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"

using namespace std;
using namespace cv;

static void help()
{
    printf("Usage: dis_optflow <video_file>\n");
}

int main(int argc, char **argv)
{
    VideoCapture cap;

    if (argc < 2)
    {
        help();
        exit(1);
    }

    cap.open(argv[1]);
    if(!cap.isOpened())
    {
        printf("ERROR: Cannot open file %s\n", argv[1]);
        return -1;
    }

    Mat prevgray, gray, rgb, frame;
    Mat flow, flow_uv[2];
    Mat mag, ang;
    Mat hsv_split[3], hsv;
    char ret;

    namedWindow("flow", 1);
    namedWindow("orig", 1);

    Ptr<DenseOpticalFlow> algorithm = DISOpticalFlow::create(DISOpticalFlow::PRESET_MEDIUM);

    while(true)
    {
        cap >> frame;
        if (frame.empty())
            break;

        cvtColor(frame, gray, COLOR_BGR2GRAY);

        if (!prevgray.empty())
        {
            algorithm->calc(prevgray, gray, flow);
            split(flow, flow_uv);
            multiply(flow_uv[1], -1, flow_uv[1]);
            cartToPolar(flow_uv[0], flow_uv[1], mag, ang, true);
            normalize(mag, mag, 0, 1, NORM_MINMAX);
            hsv_split[0] = ang;
            hsv_split[1] = mag;
            hsv_split[2] = Mat::ones(ang.size(), ang.type());
            merge(hsv_split, 3, hsv);
            cvtColor(hsv, rgb, COLOR_HSV2BGR);
            imshow("flow", rgb);
            imshow("orig", frame);
        }

        if ((ret = (char)waitKey(20)) > 0)
            break;
        std::swap(prevgray, gray);
    }

    return 0;
}
*/
/*
  * face_detect_one_target(dbt_face_detection)
#if defined(__linux__) || defined(LINUX) || defined(__APPLE__) || defined(ANDROID) || (defined(_MSC_VER) && _MSC_VER>=1800)

#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>

#include <stdio.h>

using namespace std;
using namespace cv;

const string WindowName = "Face Detection example";

class CascadeDetectorAdapter: public DetectionBasedTracker::IDetector
{
    public:
        CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> detector):
            IDetector(),
            Detector(detector)
        {
            CV_Assert(detector);
        }

        void detect(const cv::Mat &Image, std::vector<cv::Rect> &objects) CV_OVERRIDE
        {
            Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
        }

        virtual ~CascadeDetectorAdapter() CV_OVERRIDE
        {}

    private:
        CascadeDetectorAdapter();
        cv::Ptr<cv::CascadeClassifier> Detector;
 };

int main(int , char** )
{
    namedWindow(WindowName);

    VideoCapture VideoStream(0);

    if (!VideoStream.isOpened())
    {
        printf("Error: Cannot open video stream from camera\n");
        return 1;
    }

    std::string cascadeFrontalfilename = samples::findFile("data/lbpcascades/lbpcascade_frontalface.xml");
    cv::Ptr<cv::CascadeClassifier> cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
    cv::Ptr<DetectionBasedTracker::IDetector> MainDetector = makePtr<CascadeDetectorAdapter>(cascade);
    if ( cascade->empty() )
    {
      printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
      return 2;
    }

    cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
    cv::Ptr<DetectionBasedTracker::IDetector> TrackingDetector = makePtr<CascadeDetectorAdapter>(cascade);
    if ( cascade->empty() )
    {
      printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
      return 2;
    }

    DetectionBasedTracker::Parameters params;
    DetectionBasedTracker Detector(MainDetector, TrackingDetector, params);

    if (!Detector.run())
    {
        printf("Error: Detector initialization failed\n");
        return 2;
    }

    Mat ReferenceFrame;
    Mat GrayFrame;
    vector<Rect> Faces;

    do
    {
        VideoStream >> ReferenceFrame;
        cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
        Detector.process(GrayFrame);
        Detector.getObjects(Faces);

        for (size_t i = 0; i < Faces.size(); i++)
        {
            rectangle(ReferenceFrame, Faces[i], Scalar(0,255,0));
        }

        imshow(WindowName, ReferenceFrame);
    } while (waitKey(30) < 0);

    Detector.stop();

    return 0;
}

#else

#include <stdio.h>
int main()
{
    printf("This sample works for UNIX or ANDROID or Visual Studio 2013+ only\n");
    return 0;
}

#endif
*/
/*
  * 눈동자 플로우  (fback.cpp)
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
    cout <<
            "\nThis program demonstrates dense optical flow algorithm by Gunnar Farneback\n"
            "Mainly the function: calcOpticalFlowFarneback()\n"
            "Call:\n"
            "./fback\n"
            "This reads from video camera 0\n" << endl;
}
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

int main(int argc, char** argv)
{
    cv::CommandLineParser parser(argc, argv, "{help h||}");
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    VideoCapture cap(0);
    help();
    if( !cap.isOpened() )
        return -1;

    Mat flow, cflow, frame;
    UMat gray, prevgray, uflow;
    namedWindow("flow", 1);

    for(;;)
    {
        cap >> frame;
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        if( !prevgray.empty() )
        {
            calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
            cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
            uflow.copyTo(flow);
            drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));
            imshow("flow", cflow);
        }
        if(waitKey(30)>=0)
            break;
        std::swap(prevgray, gray);
    }
    return 0;
}
*/
/* mysample
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "facemark_train.hpp"
#include <vector>

using namespace std;
using namespace cv;

int main()
{
      //1.카메라 읽기
      VideoCapture cap(1);
      if (!cap.isOpened()) {
          cerr << "Camera open failed" << endl;
          return -1;
      }
      //2.카메라 담기
      Mat frame;
      while (true) {
          cap >> frame;
          if (frame.empty()) {
              break;
          }
          // imshow("frame", frame);
          //얼굴 가져오기
          vector< cv::Rect> faces;
          CParams params("haarcascade_frontalface_alt.xml");
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

      }
      //
      // vector< vector<Point2f> > landmarks;
      // facemark->getFaces(img, faces);
      // facemark->fit(img, faces, landmarks);
      // for(int j=0;j<rects.size();j++){
      //     fac e::drawFacemarks(frame, landmarks[j], Scalar(0,0,255));
      // }
      destroyAllWindows();
      return 0;
}
*/
