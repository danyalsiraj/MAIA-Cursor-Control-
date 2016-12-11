#include "helper.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <windows.h>
//#include <pthread.h>
using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay2(Mat frame);
Point pupilDetect(Mat gray, Mat background);
void cursor_movement2(Point pupil);
void left_click2();

/** Global variables */
String face_cascade_name2 = "haarcascade_frontalface_alt2.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade2;
CascadeClassifier eyes_cascade;
string window_name2 = "Capture - Face detection";
string face_window_name2 = "Detected face";
string eye_window_name = "leyes";
Mat faceROI2;
Point pupil_previous;
int frame_w2= 0 , frame_h2 = 0;
int counter2;

float xmin, xmax, ymin, ymax;
Mat frame2;

/** @function main */

void pupil_tracking(int cam_id)
{
	cout << "eye working" << endl;

	//CvCapture* capture;

	//-- 1. Load the cascades
	if (!face_cascade2.load(face_cascade_name2)) { printf("--(!)Error loading face\n");}

	if (!eyes_cascade.load(eyes_cascade_name)) { printf("--(!)Error loading eye\n");}

	//-- 2. Read the video stream
	//capture = cvCaptureFromCAM(-1);
	VideoCapture capture(cam_id);
	if (capture.isOpened())
	{
		capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
		//capture.set(CV_CAP_PROP_FPS, 30);
		while (true)
		{
			//frame = cvQueryFrame(capture);
			capture.read(frame2);
			frame_w2 = frame2.cols;
			frame_h2 = frame2.rows;

			flip(frame2, frame2, 1);
			Size s = frame2.size();
			int rows = s.height;
			int column = s.width;
			//cout<< rows;
			//cout<<column;
			//-- 3. Apply the classifier to the frame
			if (!frame2.empty())
			{
				detectAndDisplay2(frame2);
			}
			else
			{
				printf(" --(!) No captured frame -- Break!"); break;
			}

			int c = waitKey(10);
			if ((char)c == 'c') { break; }
		}
	}
	
}

/** @function detectAndDisplay */
void detectAndDisplay2(Mat frame)
{
	vector<Rect> faces;
	Mat frame_gray;
	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	//imshow("hist", frame_gray);
	//-- Detect faces
	//face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	face_cascade2.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_FIND_BIGGEST_OBJECT, Size(200, 200));

	for (size_t i = 0; i < faces.size(); i++)
	{

		rectangle(frame, faces[i], 123);

		faceROI2 = frame_gray(faces[0]);
		vector<Rect> eyes;
		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale(faceROI2, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 40));
		if (eyes.size() == 2)
		{
			Rect eye_region(faces[0].x + eyes[0].x, faces[0].y + eyes[0].y + 10, (eyes[1].x + eyes[1].width - eyes[0].x), eyes[0].height - 15);
			rectangle(frame, eye_region, 123);
			//Mat eye(frame_gray, eye_region);
			//pupilDetect2(eye, frame_gray);
		}

		for (size_t j = 0; j < eyes.size(); j++)
		{

			Point center(faces[0].x + eyes[j].x + eyes[j].width*0.5, faces[0].y + eyes[j].y + eyes[j].height*0.5);
			float radius = (eyes[j].width + eyes[j].height)*0.25;
			cout << "# of eyes " << eyes.size() << endl;
			//if (radius > 35 && radius < 70)
			{
				circle(frame, center, radius, 200);
				//Rect roi = boundingRect(contours_final[i]);
				xmin = center.x - radius;
				xmax = center.x + radius;
				ymin = center.y - radius;
				ymax = center.y + radius;

				///corners and middle on face not on frame
				Point leftCorner;
				leftCorner.x = xmin;
				leftCorner.y = (ymin + ymax) / 2;
				Point rightCorner;
				rightCorner.x = xmax;
				rightCorner.y = (ymin + ymax) / 2;
				Point Middle;
				Middle = (rightCorner + leftCorner) / 2;
				//cout << "max middle"<<rightCorner.x - Middle.x << endl;

				circle(frame, leftCorner, 2, 200);
				circle(frame, rightCorner, 2, 200);
				circle(frame, Middle, 2, 200);

				////////////////////////
				//Mat eye(frame_gray, Rect(xmin, ymin, 2 * radius, 2 * radius));
				Mat eye(frame_gray,eyes[0]);

				//Mat eye;
				//eye=eyes[0];

				imshow(eye_window_name, eye);

				Point coor = pupilDetect(eye, frame_gray);
				Point pupil;
				pupil.x = coor.x + faces[i].x + eyes[j].x;
				pupil.y = coor.y + faces[i].y + eyes[j].y;
				cursor_movement2(pupil);
				//cout << "coor" << pupil << endl;

			}
			//-- Show what you got
			imshow(face_window_name2, faceROI2);

			imshow(window_name2, frame);
		}
	}
}
void cursor_movement2(Point pupil)
{
	Point cursor_pos;
	//screencoor.x = coor.x *(1280 / 25);
	//screencoor.y = coor.y *(720 / 15);

	Rect boundry;
	boundry.x = xmin + 5;
	boundry.y = ymin + 15;
	boundry.width = xmax - boundry.x - 5;
	boundry.height = ymax - boundry.y - 12;
	rectangle(frame2, boundry, 123);
	int xscale = frame_w2 / boundry.width;
	int yscale = frame_h2 / boundry.height;

	if (pupil.x > boundry.x && pupil.x<(boundry.x + boundry.width) && pupil.y>boundry.y && pupil.y < (boundry.y + boundry.height))
	{
		pupil.x = pupil.x - boundry.x;
		pupil.y = pupil.y - boundry.y;

	}
	if (abs(pupil.x - pupil_previous.x) > 4)
	{
		cursor_pos.x = pupil.x*xscale;
		counter2 = 0;
	}
	if (abs(pupil.y - pupil_previous.y) > 4)
	{
		cursor_pos.y = pupil.y *yscale;
		counter2 = 0;
	}

	SetCursorPos(cursor_pos.x, cursor_pos.y);
	counter2++;
	//if (counter2 % 30 == 0)
	//	left_click2();
	pupil_previous = pupil;


}
void left_click2()
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
Point pupilDetect(Mat gray2, Mat background)
{

	Moments mu;
	int xCen;
	int yCen;
	//cout << "size of eyebox " << gray.size() << endl;

	Mat gray;
	gray = gray2;
	threshold(gray, gray, 26, 28, cv::THRESH_TOZERO);
	//imshow("thresh1", gray);
	threshold(gray, gray, 0, 254, cv::THRESH_BINARY);
	//imshow("thresh2", gray);

	int morph_operator = 1;
	int morph_size = 2;

	int operation = morph_operator + 2;

	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));

	/// Apply the specified morphology operation
	morphologyEx(gray, gray, operation, element);
	//imshow("morph", gray);

	erode(gray, gray, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	cv::imshow("erode2", gray);

	mu = moments(gray);
	xCen = mu.m10 / mu.m00;
	yCen = mu.m01 / mu.m00;
	//cout << xCen << "\t";
	//cout << yCen << endl;
	Point coor;
	coor.x = xCen;
	coor.y = yCen;
	circle(gray, coor, 5, 1234);


	cv::imshow("image", gray);
	return coor;
}
