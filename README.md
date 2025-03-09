
# University Course Management System


This system handles course management for students and admin interfaces, including login, course selection, seat allotment, waitlist management, and course addition/removal.


## Student Interface
1. Login Module
Data Structures:
- `stud_login_s`: Contains student ID, password, date of birth.

CSV Files:
- `login.csv`: Contains student login credentials.

Functions:
- `login()`: Verifies username and password, allows up to 3 attempts. If failed, verifies using date of birth for password reset.

Code Flow:
- Reads `login.csv` and fills the login array.
- Prompts for username and password.
- If the password is forgotten, verifies using date of birth and allow password reset.
- Checks password match, allows up to 3 attempts.

2. Course Selection Module
Data Structures:
- `Students`: Array of structures with student ID, core and elective courses, number of core and elective courses.

CSV Files:
- `courses.csv`: Contains all course details.
- `student_preference.csv`: Records course preferences.
- `courses_attended.csv`: Tracks passed courses for prerequisite checking.
- `courses_selected_sem<2/4/6>.csv`: Contains course selections by semester.

Functions:
- `course_selection_Student()`: Populates students and calls course selection.
- `course_selection()`: Displays eligible courses and records student priorities.
- `display_course()`: Checks prerequisites and displays eligible courses.
- `recommended_course()`: Recommends popular courses per semester.

Code Flow:
- Reads student data, recommends and displays courses, accepts and records course selections.

3. Seat Allotment Module
Data Structures:
- `Student`: Contains student ID and course priorities.
- `Allotments`: Contains course ID, array of student IDs, and student count.

CSV Files:
- `courses_selected.csv`: Contains student IDs and course selections.
- `allotments.csv`: Contains course ID and student allotments.
- `faculty.csv`: Contains faculty details and seat availability.

Functions:
- `read_data()`: Reads course selection data.
- `read_allotments_from_csv()`: Reads and modifies allotment data.
- `allot_seats()`: Manages seat allotment and faculty assignment.
- `handle_faculty()`: Manages faculty assignment.

Code Flow:
- Reads applicant and allotment counts, checks student allotment status, and updates faculty and vacancy data.

4. Waitlist Module
Data Structures:
- `stu`, `uni`, `nrmp`: 2D arrays for student IDs, course preferences, and allotments.

CSV Files:
- `stud_core_pref_<semno>.csv`: Core course preferences.
- `stud_elec_pref_<semno>.csv`: Elective course preferences.
- `uni_core_pref_<semno>.csv`: University core course preferences.
- `uni_elec_pref_<semno>.csv`: University elective course preferences.

Functions:
- `getCSVDimensions()`: Reads file dimensions.
- `readCSV()`: Reads CSV into 2D arrays.
- `create_nrmp()`: Creates NRMP array.
- `writeCSV()`: Writes NRMP array to CSV.
- `sel()`: Implements matching algorithm.
- `performSeatAllocation()`: Manages overall seat allocation.

5. Add/Drop Courses
Data Structures:
- `Students`: Contains student ID and course selections.

CSV Files:
- `courses_selected_sem<2/4/6>.csv`: Contains course selections by semester.

Functions:
- `display_drop_courses()`: Displays selected courses.
- `add_course()`: Adds courses to priority lists.
- `drop_Courses()`: Drops courses from student profile.

Code Flow:
- Reads student data, displays selected courses, and updates course selections after adding or dropping.

## Admin Interface
1. Courses Handled
Data Structures:
- `Courses`: Contains course ID, array of student IDs, and student count.

CSV Files:
- `courses_selected_sem<2/4/6>.csv`: Contains course selection data by semester.

Functions:
- `courses_selected_admin()`: Processes course selection data and displays applicant counts.
- `course_allotment()`: Manages course allotment and vacancy updates.

Code Flow:
- Initializes courses, reads selection data, displays applications, compares demand and supply, and updates allotments and vacancies.

2. Add/Drop Course
Data Structures:
- `Courses`: Contains course ID, student count, and array of student IDs.

CSV Files:
- `courses.csv`: Contains all course details.

Functions:
- `add_course_admin()`: Adds new courses.
- `drop_course_admin()`: Drops existing courses.

Code Flow:
- Drops an existing course, then adds a new course to the database.



## Authors

- Madhangi Karimanal 
- Madussree Ravi
- Nithyasree K
- R Padmashri 

