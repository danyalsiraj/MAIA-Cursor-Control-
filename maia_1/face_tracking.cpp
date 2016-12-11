#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include "helper.h"
/////
using namespace cv;
using namespace std;
// Function Headers 
void detectAndDisplay(Mat frame);
void cursor_movement(Point current_pos);
void left_click();

// Global variables 
String face_cascade_name = "haarcascade_frontalface_alt2.xml";
CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
string face_window_name = "Detected face";
Mat faceROI;
RNG rng(12345);
int frame_w=0, frame_h=0;
//float xmin, xmax, ymin, ymax;
Mat frame;

Point center;
POINT cursor_pos;
Point previous_center;
int counter;
// @function main 
void face_tracking(int cam_id)
{
	cout << "face working" << endl;

	//-- 1. Load the cascades
	if (!face_cascade.load(face_cascade_name))
	{
		printf("--(!)Error loadings face\n");
	}

	//-- 2. Read the video stream
	VideoCapture capture(cam_id);
	if (capture.isOpened())
	{
		capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
		//capture.set(CV_CAP_PROP_BRIGHTNESS, 40);

		while (true)
		{
			//frame = cvQueryFrame(capture);
			capture.read(frame);
			frame_w = frame.cols;
			frame_h = frame.rows; 

			cout << "frame size " << frame.size() << endl;

			flip(frame, frame, 1);
			Size s = frame.size();
			int rows = s.height;
			int column = s.width;
			//cout<< rows;
			//cout<<column;
			//-- 3. Apply the classifier to the frame
			if (frame.empty())
			{
				printf(" --(!) No captured frame -- Break!"); break;
			}
			else
			{
				detectAndDisplay(frame);

			}

			int c = waitKey(10);
			if ((char)c == 'c') { break; }
		}
	}
	//return 0; 
}

// @function detectAndDisplay 
void detectAndDisplay(Mat frame)
{
	vector<Rect> faces;
	Mat frame_gray;
	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	//imshow("hist", frame_gray);
	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_FIND_BIGGEST_OBJECT, Size(150, 150));

	for (size_t i = 0; i < faces.size(); i++)
	{
		//Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		//ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
		rectangle(frame, faces[0], 500,5);

		//faceROI = frame_gray(faces[0]);
		int width = faces[0].width;
		int height = faces[0].height;
		int middley = faces[0].y + height / 2;
		int middlex = faces[0].x + width / 2;
		center.x = middlex;
		center.y = middley;
		circle(frame, center, 7, 5000);
		//cout << "xxxx "<< center.x - previous_center.x << endl;
		cursor_movement(center);
	}			
	imshow(window_name, frame);
}

void cursor_movement(Point current_pos)
{
	

	Rect boundry;
	boundry.x = (frame_w / 2) - 78;
	boundry.y = (frame_h / 2) - 165;
	boundry.width = 156;
	boundry.height = 130;
	rectangle(frame, boundry, 123,5);
	cout << frame_w << endl;
	int xscale =frame_w/boundry.width ;
	int yscale = frame_h/boundry.height;


	//if (center.x > boundry.x && center.x<(boundry.x + boundry.width)&& center.y>boundry.y && center.y < (boundry.y + boundry.height))
	{
		center.x = center.x - boundry.x;
		center.y = center.y - boundry.y;

	}
	if (abs(center.x - previous_center.x) > 4)
	{
		cursor_pos.x = center.x*xscale;
		counter = 0;
	}
	if (abs(center.y - previous_center.y) > 4)
	{
		cursor_pos.y = center.y *yscale;
		counter = 0;
	}

		SetCursorPos(cursor_pos.x, cursor_pos.y);
		counter++;
		if (counter % 30 == 0)
			left_click();
	previous_center = center;

	/*GetCursorPos(&cursor_pos);

	if ((center.x - previous_center.x) > 5)
	{
		//SetCursorPos(cursor_pos.x + scale, cursor_pos.y);
		cursor_pos.x = cursor_pos.x + xscale;
		cout << "right " << endl;
		counter = 0;

	}
	if ((center.x - previous_center.x) < -5)
	{
		//SetCursorPos(cursor_pos.x - scale, cursor_pos.y);
		cursor_pos.x = cursor_pos.x - xscale;
		cout << "left " << endl;
		counter = 0;

	}
	if ((center.y - previous_center.y) > 5)
	{
		//SetCursorPos(cursor_pos.x, cursor_pos.y+scale);
		cursor_pos.y = cursor_pos.y + yscale;
		counter = 0;

	}
	if ((center.y - previous_center.y) <-5)
	{
		//SetCursorPos(cursor_pos.x, cursor_pos.y - scale);
		cursor_pos.y = cursor_pos.y - yscale;
		counter = 0;
	}
	else
	{
		counter++;
	}
	if (counter % 20 == 0)
	{
		left_click();
		counter = 0;
	}
	//cout << "coor  " << center << endl;
	SetCursorPos(cursor_pos.x, cursor_pos.y);
	previous_center = center;*/

}
void left_click()
{
	INPUT    Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	// left up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}