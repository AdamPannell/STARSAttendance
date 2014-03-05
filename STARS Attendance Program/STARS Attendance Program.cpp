// STARS Attendance Program.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "STARS Attendance Program.h"
#include <fstream>
#include <iostream>
#include <string>
#include <regex>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
//bool bDrawLine = false;							//Used to control drawing
//bool bDrawEllipse = false;						//Used to control drawing
bool displayProjects = false;					//Used to control display of project check boxes
int submitCounter = 0;							//Counts number of times submit button has been pressed
const int size = 100;							//Allows quick change of all arrays
HWND box[size];									//Holds handles for each window/control
HWND chBox[size];								//Holds Handles for check boxes for projects
std::string projectTitle[size];					//Holds project title labels for projects
const std::string projectFileName = "Project_List.txt";	//Holds name of file to be used for reading project titles
const std::string versionName = "Version 3.0.0 BF3";  //Holds version number/name of program
wchar_t fullName[500];							//Holds Full Name   //Created as global so they are not created many times as people sign in
wchar_t email[500];								//Holds Email Address
wchar_t studentID[500];							//Holds CPCC Student ID number
std::ofstream fout;								//Create file writer object	
std::ifstream fin;								//Create file reader object

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
HWND				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void				createSecondPage(HWND);
void				createFirstPage(HWND);
void				destroyPage();
std::string			uniToAnsi(wchar_t*);
wchar_t*			ansiToUni(std::string);				//Converts string objects to uncode c-strings and returns it
void				instructMe();
std::string			wordToString(WORD);
std::string			getDate();
bool				validateIDNumber(std::string);
void				idError();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	//Ensures all arrays are NULL
	for(int i = 0; i < size; i++)
	{
		box[i] = NULL;
		chBox[i] = NULL;
	}

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_STARSATTENDANCEPROGRAM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);        //define line 67

	// Perform application initialization:
	HWND hWnd = InitInstance (hInstance, nCmdShow);
	if(!hWnd) //Check if window was sussesfully created
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STARSATTENDANCEPROGRAM));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			if(!IsDialogMessage(hWnd, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)  //Used line 37
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STARSATTENDANCEPROGRAM));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);						//Change color of background  (COLOR_WINDOW+1)
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_STARSATTENDANCEPROGRAM);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION /*| WS_SYSMENU */| WS_THICKFRAME /*| WS_MINIMIZEBOX | WS_MAXIMIZEBOX*/,
      CW_USEDEFAULT, 0, 400, 370, NULL, NULL, hInstance, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	/*case WM_LBUTTONDOWN:             //Examples
		{
			int iPosX = LOWORD(lParam);
			int iPosY = HIWORD(lParam);
			wchar_t waCoord[20];
			wsprintf(waCoord, _T("{%i, %i}"), iPosX, iPosY);
			::MessageBox(hWnd, waCoord, _T("LMB Click"), MB_OK);
			break;
		}
	case WM_RBUTTONDOWN:
		{
			int iPosX = LOWORD(lParam);
			int iPosY = HIWORD(lParam);
			wchar_t waCoord[20];
			wsprintf(waCoord, _T("{%i, %i}"), iPosX, iPosY);
			::MessageBox(hWnd, waCoord, _T("RMB Click"), MB_OK);
			break;
		}
	case WM_CHAR:
		{
			switch (wParam)
			{
			case _T('a'):
				::MessageBox(hWnd, _T("a"), _T("Key Press"), MB_OK);
				break;
			case _T('A'):
				::MessageBox(hWnd, _T("A"), _T("Key Press"), MB_OK);
				break;
			}
			break;
		}
	case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_LEFT:
				::MessageBox(hWnd, _T("Left Arrow"), _T("Key Press"), MB_OK);
				break;
			case 'X':
				::MessageBox(hWnd, _T("X"), _T("Key Press"), MB_OK);
				break;
			default:
				break;
			}
			break;
		}
	case WM_KEYUP:
		{
			switch (wParam)
			{
			case VK_UP:
				::MessageBox(hWnd, _T("Up Arrow"), _T("Key Press"), MB_OK);
				break;
			default:
				break;
			}
			break;
		}*/
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_INSTRUCT:
			instructMe();
			break;
		case IDC_SUBMIT_BUTTON:
			//Code here when button pressed
			if(submitCounter == 0) //Fisrt page admins/officers only
			{
				//Holds data from forms
				wchar_t eventName[500];
				wchar_t hours[3];
				wchar_t description[3000];
				
				//Read data from controls
				GetDlgItemText(hWnd, IDC_EVENT_NAME, eventName, 500);
				GetDlgItemText(hWnd, IDC_HOURS, hours, 3);
				GetDlgItemText(hWnd, IDC_DESCRIPTION, description, 3000);

				wchar_t fileName[600] = L"STARS Attedance File ";
				wcscat(fileName, eventName);
				wcscat(fileName, L" ");
				wcscat(fileName, ansiToUni(getDate()));
				wcscat(fileName, L".txt");

				//Create file
				fout.open(fileName);

				//Write Info to file
				fout << versionName << std::endl;  //Version control
				fout << uniToAnsi(eventName) << std::endl;
				fout << getDate() << std::endl;
				fout << uniToAnsi(hours) << std::endl;
				fout << uniToAnsi(description) << std::endl;
				fout << "----------" << std::endl;          //Marks end of description 

				//Change pages
				destroyPage();			 //Destroy admin page
				createSecondPage(hWnd);  //Create the user page
			}
			else if(submitCounter > 0)  //Sign in Pages
			{
				//Read data from controls
				GetDlgItemText(hWnd, IDC_FULL_NAME, fullName, 500);
				GetDlgItemText(hWnd, IDC_EMAIL_ADDRESS, email, 500);
				GetDlgItemText(hWnd, IDC_STUDENT_ID, studentID, 500);

				if(wcscmp(fullName, L"XXX") == 0 )  //Exit Condition
				{
					fout.close();
					DestroyWindow(hWnd);
				}

				//Data Validation
				if(validateIDNumber(uniToAnsi(studentID)))
				{

					//Write Info to file
					fout << uniToAnsi(fullName) << std::endl;  //Writes data to file after converting from unicode to ansi so that notepad and excel can read it
					fout << uniToAnsi(email) << std::endl;
					fout << uniToAnsi(studentID) << std::endl;

					//If projects are shown get values of check boxes
					if(displayProjects)
					{
						//Loop through check boxes
						int i = 0;  //Intialize loop control variable
						while(chBox[i] != NULL)  
						{
							if(IsDlgButtonChecked(hWnd, IDC_CHECKBOX + i) == BST_CHECKED)  //Checks if checkbox is checked if checked do {}
							{
								fout << projectTitle[i] << std::endl;
							}
						
							//Clear checkbox
							CheckDlgButton(hWnd, IDC_CHECKBOX + i, BST_UNCHECKED);  //Unchecks checkbox

							i++;  //Increment loop control variable
						}//End Loop

					}//End projects IF

					fout << "----------" << std::endl;          //Marks end of users data

					//Clear Controls     //Prep for next entry  //Hide sensitive data
					SetDlgItemText(hWnd, IDC_FULL_NAME, L"");
					SetDlgItemText(hWnd, IDC_EMAIL_ADDRESS, L"");
					SetDlgItemText(hWnd, IDC_STUDENT_ID, L"");

					//Clear Variables   //Prevents multiple duplicate submissions
					wcscpy(fullName, L"");
					wcscpy(email, L"");
					wcscpy(studentID, L"");
				}
				else
				{
					idError();
					break;
				}//End Validation

			}
			submitCounter++;
			break;
		case IDC_SUBMIT_PROJECTS:
			//Code here when button pressed
			if(submitCounter == 0) //Fisrt page admins/officers only
			{
				//Holds data from forms
				wchar_t eventName[500];
				wchar_t hours[3];
				wchar_t description[3000];
				
				//Read data from controls
				GetDlgItemText(hWnd, IDC_EVENT_NAME, eventName, 500);
				GetDlgItemText(hWnd, IDC_HOURS, hours, 3);
				GetDlgItemText(hWnd, IDC_DESCRIPTION, description, 3000);

				wchar_t fileName[600] = L"STARS Attedance File ";
				wcscat(fileName, eventName);
				wcscat(fileName, L" ");
				wcscat(fileName, ansiToUni(getDate()));
				wcscat(fileName, L".txt");

				//Create file
				fout.open(fileName);

				//Write Info to file
				fout << versionName << std::endl;  //Version control
				fout << uniToAnsi(eventName) << std::endl;
				fout << getDate() << std::endl;
				fout << uniToAnsi(hours) << std::endl;
				fout << uniToAnsi(description) << std::endl;
				fout << "----------" << std::endl;

				displayProjects = true;

				//Change pages
				destroyPage();			 //Destroy admin page
				createSecondPage(hWnd);  //Create the user page
			}
			submitCounter++;
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		/*
		case IDM_LINE:
			bDrawLine = !bDrawLine;
			InvalidateRect(hWnd, 0, TRUE);
			break;
		case IDM_ELLIPSE:
			bDrawEllipse = !bDrawEllipse;
			InvalidateRect(hWnd, 0, TRUE);
			break;
		*/
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:    
		createFirstPage(hWnd);  //Create the admin page

		/*Format  for createing a window/box
		HWND hwnd = CreateWindowEx(
			0,                              // Optional window styles.
	        CLASS_NAME,                     // Window class
		    L"Learn to Program Windows",    // Window text
			WS_OVERLAPPEDWINDOW,            // Window style
	
		    // Size and position
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,     //X-coordnate, y-coordniate, x-size, y-size  
	
		    NULL,       // Parent window    
			NULL,       // Menu
			hInstance,  // Instance handle
			NULL        // Additional application data
			);

			if (hwnd == NULL)   //Check if window was created
			{
				return 0;
			}
		*/
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		/*HPEN hPenOld;

		if(bDrawLine)
		{
			//Draw a red line
			HPEN hLinePen;
			COLORREF qLineColor = RGB(255, 0, 0);
			hLinePen = CreatePen(PS_SOLID, 7, qLineColor);
			hPenOld = (HPEN)SelectObject(hdc, hLinePen);

			MoveToEx(hdc, 100, 100, NULL);
			LineTo(hdc, 500, 250);

			SelectObject(hdc, hPenOld);
			DeleteObject(hLinePen);
		}

		if(bDrawEllipse)
		{
			//Draw a Blue Ellipse
			HPEN hEllipsePen;
			COLORREF qEllipseColor = RGB(0, 0, 255);
			hEllipsePen = CreatePen(PS_SOLID, 3, qEllipseColor);
			hPenOld = (HPEN)SelectObject(hdc, hEllipsePen);

			Arc(hdc, 100, 100, 500, 250, 0, 0, 0, 0);

			SelectObject(hdc, hPenOld);
			DeleteObject(hEllipsePen);
		}*/
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void createFirstPage(HWND hWnd)  //Creates admin/officer page
{
	int distanceBetweenControlls = 47;	//Used to add the same amount of space between controls on window
	
	//Event Name Row
	box[0] = CreateWindowEx(NULL, L"EDIT", NULL, WS_VISIBLE|WS_CHILD|WS_BORDER|WS_TABSTOP|WS_HSCROLL|ES_AUTOHSCROLL, 125, (10 + distanceBetweenControlls * 0), 150, 37, hWnd, (HMENU)IDC_EVENT_NAME, hInst, NULL);   //TextBox1
	box[1] = CreateWindowEx(NULL, L"Static", L"Event Name", WS_VISIBLE|WS_CHILD|WS_BORDER, 25, (10 + distanceBetweenControlls * 0), 100, 20, hWnd, NULL, hInst, NULL);   //Label1
	//Hours Row
	box[2] = CreateWindowEx(NULL, L"EDIT", NULL, WS_VISIBLE|WS_CHILD|WS_BORDER|WS_TABSTOP|WS_HSCROLL|ES_AUTOHSCROLL, 125, (10 + distanceBetweenControlls * 1), 150, 37, hWnd, (HMENU)IDC_HOURS, hInst, NULL);   //TextBox2
	box[3] = CreateWindowEx(NULL, L"Static", L"Hours", WS_VISIBLE|WS_CHILD|WS_BORDER, 25, (10 + distanceBetweenControlls * 1), 100, 20, hWnd, NULL, hInst, NULL);   //Label2
	//Description Row
	box[4] = CreateWindowEx(NULL, L"EDIT", NULL, WS_VISIBLE|WS_CHILD|WS_BORDER|WS_VSCROLL|WS_HSCROLL|ES_MULTILINE|ES_AUTOHSCROLL|ES_AUTOVSCROLL|WS_TABSTOP|ES_WANTRETURN, 125, (10 + distanceBetweenControlls * 2), 230, 100, hWnd, (HMENU)IDC_DESCRIPTION, hInst, NULL);   //TextBox3
	box[5] = CreateWindowEx(NULL, L"Static", L"Description", WS_VISIBLE|WS_CHILD|WS_BORDER, 25, (10 + distanceBetweenControlls * 2), 100, 20, hWnd, NULL, hInst, NULL);   //Label3
	
	//Submit Button
	box[6] = CreateWindowEx(NULL, L"Button", L"Submit", WS_VISIBLE|WS_CHILD|WS_BORDER|BS_DEFPUSHBUTTON|WS_TABSTOP, 160, 261, 90, 35, hWnd, (HMENU)IDC_SUBMIT_BUTTON, hInst, NULL);      //Button1
	box[7] = CreateWindowEx(NULL, L"Button", L"Submit Projects", WS_VISIBLE|WS_CHILD|WS_BORDER|BS_DEFPUSHBUTTON|WS_TABSTOP, 25, 261, 125, 35, hWnd, (HMENU)IDC_SUBMIT_PROJECTS, hInst, NULL);      //Button2
}

void createSecondPage(HWND hWnd)  //Creates sign in page
{
	int distanceBetweenControlls = 47;	//Used to add the same amount of space between controls on window

	//Full Name Row
	box[0] = CreateWindowEx(NULL, L"EDIT", NULL, WS_VISIBLE|WS_CHILD|WS_BORDER|WS_TABSTOP|WS_HSCROLL|ES_AUTOHSCROLL, 125, (10 + distanceBetweenControlls * 0), 150, 37, hWnd, (HMENU)IDC_FULL_NAME, hInst, NULL);   //TextBox4
	box[1] = CreateWindowEx(NULL, L"Static", L"Full Name", WS_VISIBLE|WS_CHILD|WS_BORDER, 15, (10 + distanceBetweenControlls * 0), 110, 20, hWnd, NULL, hInst, NULL);   //Label4
	//Email Address Row
	box[2] = CreateWindowEx(NULL, L"EDIT", NULL, WS_VISIBLE|WS_CHILD|WS_BORDER|WS_TABSTOP|WS_HSCROLL|ES_AUTOHSCROLL, 125, (10 + distanceBetweenControlls * 1), 150, 37, hWnd, (HMENU)IDC_EMAIL_ADDRESS, hInst, NULL);   //TextBox5
	box[3] = CreateWindowEx(NULL, L"Static", L"Email Address", WS_VISIBLE|WS_CHILD|WS_BORDER, 15, (10 + distanceBetweenControlls * 1), 110, 20, hWnd, NULL, hInst, NULL);   //Label5
	//Student ID # Row
	box[4] = CreateWindowEx(NULL, L"EDIT", NULL, WS_VISIBLE|WS_CHILD|WS_BORDER|WS_TABSTOP|WS_HSCROLL|ES_AUTOHSCROLL, 125, (10 + distanceBetweenControlls * 2), 150, 37, hWnd, (HMENU)IDC_STUDENT_ID, hInst, NULL);   //TextBox6
	box[5] = CreateWindowEx(NULL, L"Static", L"CPCC Student ID #", WS_VISIBLE|WS_CHILD|WS_BORDER, 15, (10 + distanceBetweenControlls * 2), 110, 20, hWnd, NULL, hInst, NULL);   //Label6

	//Project Textboxs and labels
	if(displayProjects)
	{
		//Progmaticaly display projects
		int start = (10 + distanceBetweenControlls * 3);  //Number of units down to start adding check boxes
		int add = 30;   //Number of units between top of each check box
		fin.open(projectFileName);  //Open file
		for(int i = 0; !fin.eof(); i++)
		{
			
			std::getline(fin, projectTitle[i]);
			
			//Create check box with name from file
			chBox[i] = CreateWindowEx(NULL, L"button", ansiToUni(projectTitle[i]) , WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|WS_TABSTOP, 15, (start + add * i), 110, 40, hWnd, (HMENU)(IDC_CHECKBOX + i), hInst, NULL);
		}

	}

	//Submit Button
	box[6] = CreateWindowEx(NULL, L"Button", L"Submit", WS_VISIBLE|WS_CHILD|WS_BORDER|BS_DEFPUSHBUTTON|WS_TABSTOP, 185, 261, 90, 35, hWnd, (HMENU)IDC_SUBMIT_BUTTON, hInst, NULL);      //Button1
}

void destroyPage()  //Destroys a page 
{
	//Loop that destroys and deletes pages/windows/boxes
	for(int i = 0; box[i] != NULL; i++)
	{
		DestroyWindow(box[i]);
		box[i] = NULL;
	}
}

std::string uniToAnsi(wchar_t* uni)  //Converts unicode c-strings to ansi c-strings then returns a string object
{
	char ansi[3000];

	WideCharToMultiByte(CP_ACP, NULL, uni, -1, ansi, 3000, NULL, NULL);

	return static_cast<std::string>(ansi);
}

wchar_t* ansiToUni(std::string stringToConvert) //Converts string objects to uncode c-strings and returns it
{
	//Declare variables
	const int sizeOfStringToConvert = stringToConvert.size() + 1;
	LPWSTR unicodeString = NULL;

	//Dynamicly allocate memory to unicodeString
	unicodeString = (LPWSTR)malloc(sizeOfStringToConvert * 2);  //C function

	//Converts ascii string to unicode string 
	MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS|MB_PRECOMPOSED, stringToConvert.c_str(), -1, unicodeString, sizeOfStringToConvert);  //Win 32 function
	
	/*//Copy dereferanced unicodeString to temporary so that dynamicly allocated memory can be deleted
	wchar_t temporary[3000];
	for(unsigned int i = 0; i < stringToConvert.size() + 1; i++)
	{
		(temporary[i]) = unicodeString[i];  //Assign values in unicodeString to temporary 1 element at a time
	}

	//Deallocate memory to prevent memory leak
	free(unicodeString); //Deallocate memory //C function
	unicodeString = NULL; //Assign unicodeString to NULL to prevent memory accses errors

	//Return temporary
	return temporary;*/

	return unicodeString; //Creates memory leak need to fix
}

void instructMe()   //Displays instructions from help menu
{
	::MessageBox(NULL, 
				 _T("Fill in the data. \nEnter your name how you want it in our records and communications with you.\nStudent ID # is requierd for insurance reasons."),  //Help Instructions
				 _T("Instructions"),				//Title
				 MB_OK);							//Button type
}

std::string getDate()  //Returns the date in format mm-dd-yyyy
{
	SYSTEMTIME time; //A struct with the current UTC time

	GetLocalTime(&time); //Converts sturct from UTC time to Local time

	std::string month = wordToString(time.wMonth);
	std::string day = wordToString(time.wDay);
	std::string year = wordToString(time.wYear);

	std::string date = month + "-" + day + "-" + year;

	return date;
	
}

std::string wordToString(WORD temp)  //Converts from the win 32 data type WORD to std::string
{
	int convertToInt = temp;  //Convert WORD to int

	std::string convertToString = std::to_string(convertToInt);  //Convert int to std::string

	return convertToString;
}

bool validateIDNumber(std::string id)
{
	//Create a regular expression to validate Student ID number
	static std::regex regularExpression("[0-9]*");  //Declared as static because regex objects are compiled at runtime //Prevents slowdown of function

	//regex_search returns true if it finds a match for the expression in the string/c-string
	return std::regex_search(id, regularExpression);
}

void idError()   //Displays error message
{
	::MessageBox(NULL, 
				 _T("You must enter your student ID number \nNot your username"),  //Help Instructions
				 _T("Error: Student ID Number"),				//Title
				 MB_OK);							//Button type
}