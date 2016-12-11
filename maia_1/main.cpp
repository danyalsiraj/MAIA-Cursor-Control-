#include "helper.h"

DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
DWORD screenHeigt = GetSystemMetrics(SM_CYSCREEN);

int main()
{
	cout << "Select Camera" << endl;
	cout << "Enter 1 for Internal camera" << endl;
	cout << "Enter 2 for External Camera" << endl;
	int cam_id;
	cin >> cam_id;
	cout << endl; 
	cout << "Select Mode" << endl;
	cout << "Enter 1 for Eye Tracking" << endl;
	cout << "Enter 2 for Face Tracking" << endl;
	int choice;
	cin >> choice;
	cout << endl;

	if (choice == 1)
	{
		cout << screenWidth <<"x" << screenHeigt << endl;
		SetCursorPos(screenWidth /2, screenHeigt /2);
		pupil_tracking(cam_id-1);
	}

	if (choice == 2)
	{
		SetCursorPos(screenWidth / 2, screenHeigt / 2);
		face_tracking(cam_id-1);
	}

}