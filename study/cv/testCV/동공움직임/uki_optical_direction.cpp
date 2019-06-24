#include "opencv2/video/tracking.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
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
            //uki-stop
            // if (x == cvRound(x+fxy.x) && y == cvRound(y+fxy.y))
            //   cout << ">> 가만히 쳐다보는 중" << endl;
            // else
            if (y < cvRound(y+fxy.y))
              cout << ">> 위에 쳐다봤음" << endl;
            else if (y > cvRound(y+fxy.y))
              cout << ">> 아래 쳐다봤음" << endl;
            else if (x < cvRound(x+fxy.x))
              cout << ">> 오른쪽 쳐다봤음" << endl;
            else if (x > cvRound(x+fxy.x))
              cout << ">> 왼쪽 쳐다봤음" << endl;
            else {
              // cout << ">> 복잡한 상태임" << endl;
            }
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

    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);

    Mat flow, cflow, frame;
    UMat gray, prevgray, uflow;
    namedWindow("flow", 1);

    for(;;)
    {
        cap >> frame;
        // Mat ROI = frame(Rect(100, 100, 320, 240));
        // cvtColor(ROI, gray, COLOR_BGR2GRAY);
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        if( !prevgray.empty() )
        {
            calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
            cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
            uflow.copyTo(flow);
            //flow(방향성이 담긴 결과)와 cflow(흑백을 다시 컬러로 변환 시킨 원본)
            //flow는 움직인 방향 정보가 담겨있음.
            drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));//원본에다가 방향성이 담긴 정보를 담을 것임.
            imshow("flow", frame);
        }
        if(waitKey(30)>=0)
            break;
        std::swap(prevgray, gray);
    }
    return 0;
}
