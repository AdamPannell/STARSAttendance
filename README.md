STARSAttendance
===============
This Program is designed to aid STARS in STEM a club at Central Piedmont Community College in the proccess of taking attendace at its events.
It presents a GUI to the user asking for the event name, hours avalable, and a discription of the event.
It then asks each volunteer to sign in to the system with their name, email address, and CPCC Student ID number.
In addition, it can be used to sign people up for projects run by STARS in STEM.

To Use
===============
System Requierments: 
	Windows XP or later
	Screen and Keyboard
	Read and Write access to the directory it is in
1. Open program
2. Enter the name of the event you wish to track attendance for; in the Event Name text box
3. Enter the number of hours that will be earned by the volunteers at the event; in the Hours text box
4. Enter the description of the event in the Description text box
5. If you want the volunteers to be able to select what project they would like to volunteer for go to line 5a otherwise continue to 6
	5a. Ensure that the file "Project_List.txt" (Case Sensitive) is in the same directory as the "STARS Attendance Program.exe"
	5b. If it is not create a file of that name in the directory and in the file list the name of each project on their own lines see Example
	5c. Click the button marked "Submit Projects" and go to line 7
6. Click the button marked "Submit"
7. The screen changes to the user page
8. Enter your FULL name in the Full Name text box
9. Enter your email address in the Email Address text box
10. Enter your CPCC Student ID number in to the CPCC Student ID text box
	10a. If you do not have a CPCC Student ID number or are an instructor
11. If a list of projects is displayed check the ones you wish to volunteer for
12. Click the button marked "Submit"
13. Have all Volunteers do the same
14. To close the program type "XXX" into the Full Name text box and Click "Submit"
15. A file has been created in the directory from which the "STARS Attendance Program.exe" was launched
	15a. The name of the file is "STARS Attendance File [name of event] [mm-dd-yyyy].txt"
	15b. This file can be used as proof of attendance and can be read by the "STARS Roster.xlsm" Excell spreadsheet

To get Source/Compile
===============
Source is available at https://github.com/AdamPannell/STARSAttendance/
This program was made in Visual Studio Ultimate 2012


Example Project_List.txt
===============
ExampleProject1
Example Project 2
Ex. Project 3
example_project-4


Example Attendance file "STARS Attendance File Example Event 3-5-2014.txt"
===============
Version 3.0.0 BF3
Example Event
3-5-2014
1
Example Description line 1
Example Description line 2
----------
Example Person
person@example.com
12345678
ExampleProject1
Example Project 2
Ex. Project 3
example_project-4
----------
Example Person 2
person2@example.com
12345678
Example Project 2
example_project-4
----------
Example Person 3
person3@example.com

----------

