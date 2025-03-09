#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global variables for dimensions and paths
int STU_ROWS=0;
int STU_COLS=0;
int UNI_ROWS=0;
int UNI_COLS=0;
int NRMP_COLS=0;

#define NRMP_ROWS 67          // Maximum seats in a course + 1
#define MAX_CORE_COURSE 3     // Maximum no of courses that can be allotted to a student
#define MAX_ELEC_COURSE 1
#define MAX_LINE_LENGTH 1024
#define MAX_LINE_LENGTH_CS 200
#define MAX_LINE_LENGTH_DCS 100
#define MAX_FIELDS 40
#define MAX_CORE 4
#define MAX_ELECTIVE 2

int **stu;
int **uni;
int **nrmp;

//course selection student structure
typedef struct {
    int student_id;              // Field for student ID
    int core[MAX_CORE];               // Array to store core course IDs
    int elective[MAX_ELECTIVE];       // Array to store elective course IDs
    int num_core;                     // Number of core courses added
    int num_elective;                 // Number of elective courses added
} Student;

//course recommendation course structure
typedef struct {
    int course_id;
    int popularity;
} Course;

//courses selected course structure
typedef struct {
    int course_id;
    int student_ids[1000];
    int student_count;
} Courses;

//login student structure
typedef struct{
    int student_id; 
char password[30]; 
    char dob[20];
} stud;
stud login_s[800];

//login faculty structure
typedef struct{
    int fac_id; 
    char password[30];
} fac;
fac login_f[50];

//course available courses structure
typedef struct{
    int id;
    char name[100];
    int credits;
    char type;
    int sem;
    int prereq;
    float cgpa;
} courses_available;
courses_available courses[50];

//course vacancy structure
typedef struct {
    int course_id;
    int seats;
} CourseVacancy;

//allotment structure if less than 60 applicants
typedef struct {
    int course_id;
    int student_ids[100]; // Adjust size as needed
    int student_count;
} Allotment;
Allotment allotments[100]; // Adjust size as needed
int allotment_count=0;

// Function to parse a line into fields
int parse_line(char *line, char *fields[]){
    char *token;
    int field_count=0;

    token=strtok(line, ",");
    while (token!=NULL && field_count<MAX_FIELDS){
        fields[field_count++]=token;
        token=strtok(NULL, ",");
    }

    return field_count;
}

//Function to check whether given string is digit
int is_digit(char str[50]){
    for (int i=0; i<strlen(str); i++){
        if (isdigit(str[i]))
            return 1;
    }
    return 0;
}

//Function to check whether given string is uppercase
int is_upper(char str[50]){
    for (int i=0; i<strlen(str); i++){
        if (isupper(str[i]))
            return 1;
    }
    return 0;
}

//Function to check whether given string is a special character
int is_spl(char str[50]){
    for (int i=0; i<strlen(str); i++){
        if (!isalnum(str[i]) && !isspace(str[i]))
            return 1;
    }
    return 0;
}

//Fucntion to read student file
int read_studentfile(FILE *file){
    char line[MAX_LINE_LENGTH];
    int count=0;
    while (fgets(line, sizeof(line), file)!=NULL){
        char *token;
        token=strtok(line, ",");
        if (token!=NULL){
            login_s[count].student_id=atoi(token);
            token=strtok(NULL, ",");
            if (token!=NULL){
                strcpy(login_s[count].password, token);
                token=strtok(NULL, ",");
                if (token!=NULL){
                    strcpy(login_s[count].dob, token);
                    count++;
                }
            }
        }
    }
    fclose(file);
    return 0;
}

//Funtion to read admin file
int read_adminfile(FILE *file){
    char line[MAX_LINE_LENGTH];
    int count=0;
    while (fgets(line, sizeof(line), file)!=NULL){
        char *token;
        token=strtok(line, ",");
        if (token!=NULL){
            login_f[count].fac_id=atoi(token);
            token=strtok(NULL, ",");
            if (token!=NULL){
                token[strcspn(token, "\n")]=0;
                strcpy(login_f[count].password, token);
                token=strtok(NULL, ",");
                count++;
            }
        }
    }
    fclose(file);
    return 0;
}

//Login module
int Login(FILE *file, int x){
    int i=0, j=0, a=0, forgot=0, found=0, flag=0, username1=0, flagf=0;
    char check[50], password1[50], dob1[11], update[29];

    if (x==2){
        read_adminfile(file);
        for (j=0; j<3; j++){
            printf("\nEnter username: ");
            scanf("%d",&username1);
            printf ("Enter password: ");
            scanf("%s",&password1);
            for (i=0;i<38;i++){
                if (login_f[i].fac_id==username1 && strcmp(login_f[i].password,password1)==0){
                    printf("*******************\nLogin Successful\n*******************\n");
                    flagf=1;
                    return username1;
                }
            }
            if (flagf==0)
                printf("Username/Password doesn't match. Try again!\n");

        }
        if (flagf==0){
            printf("Can't login further.\n");
            flagf=2;
        }
    }
    
    else if (x==1){
        read_studentfile(file);
        for (j=0; j<3; j++){
            if (flag==0){
                printf("\nEnter username: ");
                scanf("%d",&username1);
                printf ("Forgot password? (0->no / 1->yes): ");
                scanf("%d",&forgot);
                if (forgot==1){
                    j--;
                    file=fopen("login.csv", "w");
                    if (file==NULL){
                        printf("Error opening file.\n");
                        return 1;
                    }
                    printf("Enter date of birth as dd/mm/yyyy: ");
                    getchar();
                    fgets(dob1, sizeof(dob1), stdin);
                    strcat(dob1,"\n");
                    for (i=0;i<750;i++){
                        if (strcmp(login_s[i].dob, dob1)==0){
                            printf("Password must be atleast 8 characters long and contain atleast 1 number, 1 special character other than space and 1 uppercase letter.\nEnter new password: ");
                            scanf("%s",&password1);
                            if (strlen(password1)<8){
                                printf("Password has less than 8 characters. Try again!\n");
                                fprintf(file,"%d,%s,%s",login_s[i].student_id,login_s[i].password,login_s[i].dob);
                                found=2;
                            }
                            if (is_digit(password1)==0){
                                printf("Password doesn't have a number. Try again!\n");
                                fprintf(file,"%d,%s,%s",login_s[i].student_id,login_s[i].password,login_s[i].dob);
                                found=2;
                            }
                            else if (is_upper(password1)==0){
                                printf("Password doesn't have an uppercase letter. Try again!\n");
                                fprintf(file,"%d,%s,%s",login_s[i].student_id,login_s[i].password,login_s[i].dob);
                                found=2;
                            }     
                            else if (is_spl(password1)==0){
                                printf("Password doesn't have a special case character. Try again!\n");
                                fprintf(file,"%d,%s,%s",login_s[i].student_id,login_s[i].password,login_s[i].dob);
                                found=2;
                            }
                            else{
                                printf("Confirm new password: ");
                                scanf("%s",&check);
                                if (strcmp(check,password1)==0){                        
                                    strcpy(login_s[i].password,password1);
                                    fprintf(file,"%d,%s,%s",login_s[i].student_id,login_s[i].password,login_s[i].dob);
                                    found=1;
                                }
                                else{
                                    printf("New password doesn't match. Try again!\n");
                                    fprintf(file,"%d,%s,%s",login_s[i].student_id,login_s[i].password,login_s[i].dob);
                                    found=2;
                                }
                            }
                        }
                        else{
                            fprintf(file,"%d,%s,%s",login_s[i].student_id,login_s[i].password,login_s[i].dob);
                        }
                    }
                    fclose(file);
                    if (found==1)
                        printf("Password reset successful!\n");
                    else if (found==2)
                        continue;
                    else if (strcmp(login_s[i].dob, dob1)!=0)
                        printf("Date of Birth didn't match\n");
                    else
                        printf("Couldn't find record.\n");
                }

                else{
                    printf("Enter password: ");
                    scanf("%s",&password1);
                    for (i=0;i<750;i++){
                        if (login_s[i].student_id==username1 && strcmp(login_s[i].password,password1)==0){
                            printf("Login Successful\n");
                            flag=1;
                            break;
                        }
                    }
                }   
            }
            if (flag==1)
                break;
            else{
                if (forgot!=1){
                    if (j<2)
                        printf("\nUsername/Password doesn't match. Try again\n");
                    else{
                        printf("Cannot login further\n");
                        flag=2;
                    }
                }
            }
        }
        if (flag!=2 || flagf!=2)
            return username1;
    }
}

//Function to display student profile and return semester
int display_profile(int student_id, char *cgpa){
    FILE *file;
    char line[MAX_LINE_LENGTH_CS];
    char *fields[MAX_FIELDS];
    int field_count;
    int is_header=1;

    // Open the student CSV file for reading
    file=fopen("student.csv", "r");
    if (file==NULL){
        printf("Error opening file.\n");
        return -1;
    }
    
    while (fgets(line, sizeof(line), file)){
        if (is_header){
            is_header=0; 
            continue; 
        }

        field_count=parse_line(line, fields);
        if (student_id==atoi(fields[0])){
            printf("**********************\nStudent Profile\n**********************\n");
            printf("STUDENT ID: %s\n", fields[0]);
            printf("NAME: %s\n", fields[1]);
            printf("SEMESTER: %s", fields[4]);  
            printf("CGPA: %s\n", fields[3]);
            strcpy(cgpa, fields[3]);
            fclose(file);
            return atoi(fields[4]);
        }
    }

    // Close the file
    fclose(file);

    printf("Student ID not found.\n");
    return -1;
}

//Function to read data for student course selection
int read_data(const char *filename, Student students[], int *student_count){
    FILE *file=fopen(filename, "r");
    if (!file){
        perror("Unable to open file");
        return 0;
    }
    char line[MAX_LINE_LENGTH];
    *student_count=0;
    while (fgets(line, sizeof(line), file)){
        size_t length=strlen(line);
        if (length>0 && line[length - 1]=='\n'){
            line[length - 1]='\0';
        }
        char *token=strtok(line, ",");
        students[*student_count].student_id=atoi(token);
        for (int i=0; i<MAX_CORE; i++){
            token=strtok(NULL, ",");
            students[*student_count].core[i]=atoi(token);
        }
        for (int i=0; i<MAX_ELECTIVE; i++){
            token=strtok(NULL, ",");
            students[*student_count].elective[i]=atoi(token);
        }
        students[*student_count].num_core=MAX_CORE;
        students[*student_count].num_elective=MAX_ELECTIVE;
        (*student_count)++;
    }
    fclose(file);
    return 1;
}

//Function to write data of student course selection
int write_data(const char *filename, Student students[], int student_count){
    FILE *file=fopen(filename, "w");
    if (!file){
        perror("Unable to open file");
        return 0;
    }
    for (int i=0; i<student_count; i++){
        fprintf(file, "%d", students[i].student_id);
        for (int j=0; j<MAX_CORE; j++) 
            fprintf(file, ",%d", students[i].core[j]);
        for (int j=0; j<MAX_ELECTIVE; j++) 
            fprintf(file, ",%d", students[i].elective[j]);
        fprintf(file, "\n");
    }
    fclose(file);
    return 1;
}

//Function to check if a course has been attended
int has_attended_course(int student_id, const char *course_id){
    FILE *file;
    char line[MAX_LINE_LENGTH_CS];
    char *fields[MAX_FIELDS];
    int index=0;
    int is_header=1;
    int field_count;
    // Open the courses attended CSV file for reading
    file=fopen("courses_attended.csv", "r");
    if (file==NULL){
        printf("Error opening file.\n");
        return 0;
    }
    while (fgets(line, sizeof(line), file)!=NULL){
        if (is_header){
            field_count=parse_line(line, fields);
            is_header=0;
            for (int i=1; i <field_count; i++){
                index+=1;
                if (strcmp(course_id, fields[i])==0){
                    break;
                }
            continue;
            }
        }
        field_count=parse_line(line, fields);
        if (student_id==atoi(fields[0])){
            for (int i=0; i <field_count; i++){
                if (strncmp(fields[index],"1",1)==0){
                    fclose(file);
                    return 1; // Course found and attended
                }
            }   
        }
    }
    fclose(file);
    return 0; // Course not found
}

//Functiont to calculate course popularity
void calculate_course_popularity(Course core_courses[], int *num_core_courses, Course elective_courses[], int *num_elective_courses){
    FILE *file=fopen("courses_recommended.csv", "r");
    if (file==NULL){
        printf("Error opening file.\n");
        return;
    }
    char line[MAX_LINE_LENGTH];
    char *fields[MAX_FIELDS];
    int is_header=1;
    int core_count=0, elective_count=0;

    while (fgets(line, sizeof(line), file)){
        if (is_header){
            is_header=0;
            continue;
        }
        int field_count=parse_line(line, fields);

        for (int i=0; i<4; i++){
            int course_id=atoi(fields[i]);
            int found=0;
            for (int j=0; j<core_count; j++){
                if (core_courses[j].course_id==course_id){
                    core_courses[j].popularity += 4 - i;
                    found=1;
                    break;
                }
            }
            if (!found){
                core_courses[core_count].course_id=course_id;
                core_courses[core_count].popularity=4 - i;
                core_count++;
            }
        }

        for (int i=4; i<6; i++){
            int course_id=atoi(fields[i]);
            int found=0;
            for (int j=0; j<elective_count; j++){
                if (elective_courses[j].course_id==course_id){
                    elective_courses[j].popularity += 2 - (i - 4);
                    found=1;
                    break;
                }
            }
            if (!found){
                elective_courses[elective_count].course_id=course_id;
                elective_courses[elective_count].popularity=2 - (i - 4);
                elective_count++;
            }
        }
    }

    *num_core_courses=core_count;
    *num_elective_courses=elective_count;

    fclose(file);
}

//Function to compare courses
int compare_courses(const void *a, const void *b){
    Course *courseA=(Course *)a;
    Course *courseB=(Course *)b;
    return courseB->popularity - courseA->popularity;
}

//Function to recommend courses
void recommended_courses(int sem){
    Course core_courses[20]={};
    Course elective_courses[12]={};
    int num_core_courses=0, num_elective_courses=0;

    calculate_course_popularity(core_courses, &num_core_courses, elective_courses, &num_elective_courses);
    qsort(core_courses, num_core_courses, sizeof(Course), compare_courses);
    qsort(elective_courses, num_elective_courses, sizeof(Course), compare_courses);
    printf("**********************\nRecommendations\n**********************\n");
    printf("Recommended Core Courses:\n");
    for (int i=0; i<num_core_courses; i++){
        int core_sem=core_courses[i].course_id / 100;
        if (core_sem==sem)
            printf("COURSE ID: %d, Popularity score: %d\n", core_courses[i].course_id, core_courses[i].popularity);
    }
    printf("\nRecommended Elective Courses:\n");
    for (int i=0; i<num_elective_courses; i++){
        int elective_sem=elective_courses[i].course_id / 100;
        if (elective_sem==sem)
            printf("COURSE ID: %d, Popularity score: %d\n", elective_courses[i].course_id, elective_courses[i].popularity);
    }

}

//Function to display courses available according to semester
void display_courses(int student_id, char *cgpa,int core[],int elective[],int sem){
    if (sem==-1){
        return; // Exit if student ID is not found
    }
    recommended_courses(sem);
    FILE *file;
    char line[MAX_LINE_LENGTH_CS];
    char *fields[MAX_FIELDS];
    int field_count;
    int is_header=1;
    int prerequisite=0,i=0,j=0;
    // Open the courses CSV file for reading
    file=fopen("courses.csv", "r");
    if (file==NULL){
        printf("Error opening file.\n");
        return;
    }

    printf("\nCourses available for semester %d:\n", sem);
    printf("***************************************************\n");

    while (fgets(line, sizeof(line), file)){
        if (is_header){
            is_header=0; 
            continue; 
        }

        field_count=parse_line(line, fields);
        if (atoi(fields[4])==sem){ 
            if (strcmp(fields[5],"0")==0)
                prerequisite=1;
            else
                prerequisite=has_attended_course(student_id,fields[5]);
            // Check if the CGPA meets the requirement and the prerequisite course has been attended
            if (atof(cgpa)>=atof(fields[6]) && prerequisite){
                printf("COURSE ID: %s\n", fields[0]);
                printf("COURSE NAME: %s\n", fields[1]);
                printf("NUMBER OF CREDITS: %s\n", fields[2]);  
                printf("TYPE: %s\n", fields[3]);
                printf("***************************************************\n");
                if (strncmp(fields[3],"C",1)==0)
                    core[i++]=atoi(fields[0]);
                else
                    elective[j++]=atoi(fields[0]);
            }
        }
    }

    // Close the file
    fclose(file);
}

//Function for selecting the courses
void course_selection(Student *student,int sem, char cgpa[]){
    int core[4]={};
    int elective[2]={};
    display_courses(student->student_id,cgpa,core,elective,sem);
    int nc, ne,check;
    int lc=0,le=0;
    for (int i=0;i<4;i++){
        if (core[i]!=0){
            lc++;
        }
    }
    for (int i=0;i<2;i++){
        if (core[i]!=0){
            le++;
        }
    }
    
    printf("ADDING COURSES\n");
    if (lc==4){
        printf("Enter the number of core courses you'd like to add: ");
        scanf("%d", &nc);
        while (nc<3 || nc>4){
            if (nc<3){
                printf("Less than the minimum number of core courses you can add\n");
            }
            else if (nc>4){
                printf("Greater than the maximum number of core courses you can add\n");
            }
            printf("Please enter again: ");
            scanf("%d", &nc);
        }
    }
    else
        nc=lc;
    student->num_core=nc;
    printf("Please enter the CORE courses in order of your priority\n");
    for (int i=0; i<nc; i){
        printf("Enter the course_id: ");
        scanf("%d", &check);
        int found=0;
        for (int a=0;a<lc;a++){
            if (core[a]==check){
                student->core[i]=check;
                found=1;
                i++;
                break;
            }
        }
        if (found==0){
            printf("Course not available, please enter again\n");
            continue;
        }
    }

    if (le==2){
        printf("Enter the number of ELECTIVE courses you'd like to add: ");
        scanf("%d", &ne);
        while (ne<1 || ne>2){
            if (ne<1){
                printf("Less than the minimum number of elective courses you can add\n");
            }
            else if (ne>2){
                printf("Greater than the maximum number of elective courses you can add\n");
            }
            printf("Please enter again: ");
            scanf("%d", &ne);
        }
    }
    else
        ne=le;
    student->num_elective=ne;
    printf("Please enter the ELECTIVE courses in order of your priority\n");
    for (int i=0; i<ne; i){
        printf("Enter the course_id: ");
        scanf("%d", &check);
        int found=0;
        for (int a=0;a<le;a++){
            if (elective[a]==check){
                student->elective[i]=check;
                found=1;
                i++;
                break;
            }
        }
        if (found==0){
            printf("Course not available, please enter again\n");
            continue;
        }
    }
}

//Function for course selection module
int course_selection_student(int student_id,int sem,char cgpa[]){
    char filename[30];
    sprintf(filename, "courses_selected_sem%d.csv", sem);
    Student students[100];
    int student_count;
    if (!read_data(filename, students, &student_count)){
        return 1;
    }
    int found=0;
    Student *student=NULL;
    for (int i=0; i<student_count; i++){
        if (students[i].student_id==student_id){
            student=&students[i];
            found=1;
            break;
        }
    }
    if (!found){
        student=&students[student_count];
        student->student_id=student_id;
        student_count++;
    }
    course_selection(student, sem, cgpa);
    if (!write_data(filename, students, student_count)){
        return 1;
    }
    printf("Thank you, your priority order has been taken into consideration.\nPlease wait for a 2-week time period to view results.\n\n");
    return 0;
}

//Function to check for the course to be dropped
void drop_course(Student students[], int student_count, int student_id, int course_id, int *found_course){
    for (int i=0; i<student_count; i++){
        if (students[i].student_id==student_id){
            for (int j=0; j<MAX_CORE; j++){
                if (students[i].core[j]==course_id){
                    *found_course=1;
                    students[i].core[j]=0;
                    students[i].num_core--;
                }
            }
            for (int j=0; j<MAX_ELECTIVE; j++){
                if (students[i].elective[j]==course_id){
                    *found_course=1;
                    students[i].elective[j]=0;
                    students[i].num_elective--;
                }
            }
            if (students[i].num_core<3)
                printf("Minimum criteria of 3 core courses not attained. Please add more courses.\n");
            if (students[i].num_elective<1) 
                printf("Minimum criteria of 1 elective course not attained. Please add more courses.\n");
            break;
        }
    }
}

//Function to display the available courses that can be dropped
void display_courses_drop(Student students[], int student_count, int student_id){
    for (int i=0; i<student_count; i++){
        if (students[i].student_id==student_id){
            printf("---------------------------------------\n");
            printf("Core Courses: ");
            for (int j=0; j<MAX_CORE; j++){
                if (students[i].core[j]!=0) 
                    printf("%d ", students[i].core[j]);
            }
            printf("\nElective Courses: ");
            for (int j=0; j<MAX_ELECTIVE; j++){
                if (students[i].elective[j]!=0) 
                    printf("%d ", students[i].elective[j]);
            }
            printf("\n---------------------------------------\n");
            break;
        }
    }
}

//Function to add course from students side
void add_course(Student students[], int student_count, int student_id, int course_id, int is_core){
    for (int i=0; i<student_count; i++){
        if (students[i].student_id==student_id){
            if (is_core){
                for (int j=0; j<MAX_CORE; j++){
                    if (students[i].core[j]==0){
                        students[i].core[j]=course_id;
                        students[i].num_core++;
                        printf("Core course %d added.\n", course_id);
                        return;
                    }
                }
            }else {
                for (int j=0; j<MAX_ELECTIVE; j++){
                    if (students[i].elective[j]==0){
                        students[i].elective[j]=course_id;
                        students[i].num_elective++;
                        printf("Elective course %d added.\n", course_id);
                        return;
                    }
                }
            }
        }
            return;
        }
    printf("Student ID not found.\n");
}

//Function for add courses module
int add_course_student(int student_id, int sem){
    char filename[30];
    sprintf(filename, "courses_selected_sem%d.csv", sem);
    Student students[100];
    int student_count;
    if (!read_data(filename, students, &student_count)) 
        return 1;
    display_courses_drop(students, student_count, student_id);
    int course_id, is_core;
    printf("Enter course ID to add: ");
    scanf("%d", &course_id);
    
    printf("Is it a core course? (1 for Yes, 0 for No): ");
    scanf("%d", &is_core); 
    add_course(students, student_count, student_id, course_id, is_core);
    
    if (!write_data(filename, students, student_count))
        return 1;
    
    printf("Updated course selections for student %d:\n", student_id);
    display_courses_drop(students, student_count, student_id);
    
    return 0;
}

//Function to drop course from students side
int drop_course_student(int student_id, int sem){
    int course_id;
    char filename[30];
    sprintf(filename, "courses_selected_sem%d.csv", sem);
    Student students[100];
    int student_count;

    if (!read_data(filename, students, &student_count)) 
        return 1;

    display_courses_drop(students, student_count, student_id);

    while (1){
        printf("Enter course ID to drop: ");
        scanf("%d", &course_id);
        int found_course=0;
        drop_course(students, student_count, student_id, course_id, &found_course);
        if (!found_course)
            printf("Course ID not found\n");
        else
            break;
    }

    if (!write_data(filename, students, student_count))
        return 1;

    char line[MAX_LINE_LENGTH];
    FILE *file=fopen("allotments.csv", "r");
    if (file==NULL){
        printf("Error opening file.\n");
        return -1;
    }

    FILE *temp=fopen("temp.csv", "w");
    if (temp==NULL){
        printf("Error opening temporary file.\n");
        fclose(file);
        return -1;
    }

    int flag=0;
    while (fgets(line, sizeof(line), file)){
        int current_course_id, current_student_count;
        sscanf(line, "%d,%d", &current_course_id, &current_student_count);

        if (current_course_id==course_id){
            flag=1;
            fprintf(temp, "%d,%d", current_course_id, current_student_count - 1);
            char *token=strtok(line, ",");
            token=strtok(NULL, ","); // Skip the course_id and student_count fields
            int is_first_student=1;
            token=strtok(NULL, ",");
            fprintf(temp, ",");
            while (token!=NULL){
                int current_student_id=atoi(token);
                if (current_student_id!=student_id){
                    if (!is_first_student){
                        fprintf(temp, ",");
                    }
                    fprintf(temp, "%d", current_student_id);
                    is_first_student=0;
                }
                token=strtok(NULL, ",");
            }
            fprintf(temp, "\n");
        }else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(temp);
    fclose(file);

    if (flag){
        remove("allotments.csv");
        rename("temp.csv", "allotments.csv");
        printf("Course dropped successfully!\n");
    } else {
        remove("temp.csv");
        printf("Course not found!\n");
    }

    printf("Updated course selections for student %d:\n", student_id);
    display_courses_drop(students, student_count, student_id);
    return 0;
}

//Function to drop a course from admin side
int drop_course_admin(){
    char line[MAX_LINE_LENGTH];
    char *fields[MAX_FIELDS];

    FILE *file=fopen("courses.csv", "r");
    if (file==NULL){
        printf("Error opening file.\n");
        return -1;
    }

    FILE *temp=fopen("temp.csv", "w");
    if (temp==NULL){
        printf("Error opening temporary file.\n");
        fclose(file);
        return -1;
    }

    int s_code, is_header=1, field_count, flag=0;
    printf("Enter Course ID of the course to be dropped: ");
    scanf("%d", &s_code);
    getchar();

    while (fgets(line, sizeof(line), file)){
        if (is_header){
            fprintf(temp, "%s", line);
            is_header=0;
            continue;
        }

        field_count=parse_line(line, fields);
        if (atoi(fields[0])!=s_code){
            fprintf(temp, "%s", fields[0]);
            for (int i=1; i<field_count; i++){
                fprintf(temp, ",%s", fields[i]);
            }
        } else {
            flag=1;
        }
    }

    fclose(temp);
    fclose(file);

    if (flag){
        remove("courses.csv");
        rename("temp.csv", "courses.csv");
        printf("Course dropped Successfully!\n");
    } else {
        remove("temp.csv");
        printf("Course not found!\n");
    }

    return s_code;
}

//Function to add a course from admin side
int add_course_admin(){
    char line[MAX_LINE_LENGTH];
    int id, cred, sem, pre1, c_id, is_header=1, field_count;
    char course[100], c_name[100], type;
    char *fields[MAX_FIELDS];
    float cgpa;

    printf("*******************************\nTo add a new course, one of the old courses must be dropped\n*******************************\n");
    printf("Enter the semester in which the course is available: ");
    scanf("%d", &sem);
    getchar();
    printf("Enter whether course to be added is Core or Elective (C/E): ");
    scanf("%c", &type);
    getchar();

    FILE *file=fopen("courses.csv", "r");
    if (!file){
        perror("Could not open file");
        return 1;
    }

    if (type=='C')
        printf("Core courses available for semester %d:\n", sem);
    else
        printf("Elective courses available for semester %d:\n", sem);

    while (fgets(line, sizeof(line), file)){
        if (is_header){
            is_header=0;
            continue;
        }
        field_count=parse_line(line, fields);
        c_id=atoi(fields[0]);
        strcpy(course, fields[1]);

        if (type==fields[3][0] && sem==atoi(fields[4])){
            printf("%d %s\n", c_id, course);
        }
    }
    fclose(file);

    id=drop_course_admin();

    file=fopen("courses.csv", "a");
    if (!file){
        perror("Could not open file");
        return 1;
    }

    printf("Enter Course Name to be added: ");
    fgets(c_name, sizeof(c_name), stdin);
    c_name[strcspn(c_name, "\n")]=0; 
    printf("Enter pre-requisite course to be added(if none, enter 0): ");
    scanf("%d", &pre1);
    printf("Enter the minimum required CGPA for the course to be added: ");
    scanf("%f", &cgpa);

    if (type=='C'){
        cred=3;
        fprintf(file, "\n%d,%s,%d,%c,%d,%d,%.1f\n", id, c_name, cred, type, sem, pre1, cgpa);
        printf("Course Added Successfully\n");
    } else if (type=='E'){
        cred=2;
        fprintf(file, "\n%d,%s,%d,%c,%d,%d,%.1f\n", id, c_name, cred, type, sem, pre1, cgpa);
        printf("Course Added Successfully\n");
    } else {
        printf("Invalid type for course to be added\n");
    }

    fclose(file);
    return 0;
}

// Function to get dimensions (rows and columns) of a CSV file
void getCSVDimensions(const char *filename, int *ROWS, int *COLS){
    FILE *file=fopen(filename, "r");
    if (!file){
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[10240];
    int temp_rows=0;
    int temp_cols=0;

    // Read the file to determine the number of rows and columns
    while (fgets(line, sizeof(line), file)){
        temp_rows++;
        if (temp_cols==0){  // Calculate columns only for the first row
            char *token=strtok(line, ",");
            while(token){
                temp_cols++;
                token=strtok(NULL, ",");
            }
        }
    }

    *ROWS=temp_cols;
    *COLS=temp_rows;

    fclose(file);
}

// Function to read data from a CSV file into a 2D array
void readCSV(const char *input_file, int **arr, int rows, int cols){
    FILE *file=fopen(input_file, "r");
    if (!file){
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[10240];
    int row=0;
    while (fgets(line, sizeof(line), file)){
        char *token=strtok(line, ",");
        int col=0;
        while (token){
            arr[col][row]=atoi(token); // Store the value in the 2D array
            token=strtok(NULL, ",");
            col++;
        }
        row++;
    }

    fclose(file);
}

// Function to create the nrmp array based on university preferences
void create_nrmp(const char *filename, int **nrmp){
    FILE *file=fopen(filename, "r");
    if (!file){
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[10240];
    int row=0;

    // Initialize nrmp array with zeros
    for (int i=0; i<NRMP_ROWS; i++){
        for (int j=0; j<NRMP_COLS; j++){
            nrmp[i][j]=0;
        }
    }

    //Name the columns in nrmp based on the columns in uni
    for (int i=0; i<NRMP_COLS; i++){
        nrmp[0][i]=uni[0][i];
    }


    fclose(file);
}

// Function to write data from a 2D array into a CSV file
void writeCSV(const char *output_file, int **arr, int rows, int cols){
    FILE *file=fopen(output_file, "w");
    if (!file){
        perror("Failed to create file");
        exit(EXIT_FAILURE);
    }

    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            fprintf(file, "%d", arr[i][j]);
            if (j<cols - 1){
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

// Function to perform selection and seat allocation
void sel(int stu_col, int **stu, int **uni, int **nrmp, int start, int *allot, int MAX_COURSE){
    int student=stu[0][stu_col];
    for (int stu_row=1; stu_row<STU_ROWS && *allot!=MAX_COURSE; stu_row++){
        int done=0;
        int stu_pref=stu[stu_row][stu_col];
        for (int uni_col=0; uni_col<UNI_COLS && !done; uni_col++){
            if (uni[0][uni_col]==stu_pref){
                for (int uni_row=1; uni_row<UNI_ROWS && !done; uni_row++){
                    if (uni[uni_row][uni_col]==student){
                        for (int nrmp_col=0; nrmp_col<NRMP_COLS && !done; nrmp_col++){
                            if (nrmp[0][nrmp_col]==stu_pref){
                                for (int nrmp_row=1; nrmp_row<NRMP_ROWS && !done; nrmp_row++){
                                    if (nrmp[nrmp_row][nrmp_col]==0){
                                        nrmp[nrmp_row][nrmp_col]=student;
                                        (*allot)++;
                                        done=1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Function to check if a course_id already exists in the allotments array
int course_id_exists(int course_id){
    for (int i=0; i<allotment_count; i++){
        if (allotments[i].course_id==course_id){
            return 1;
        }
    }
    return 0;
}

// Function to read the data from elec_allotment_sem2.csv and populate the allotments array
void read_elec_allotment_data(const char *filename){
    FILE *file=fopen(filename, "r");
    if (!file){
        perror("Error opening file for reading");
        return;
    }

    char line[MAX_LINE_LENGTH];

    // Read the first line to get course IDs
    if (fgets(line, sizeof(line), file)){
        line[strcspn(line, "\n")]='\0';
        char *token=strtok(line, ",");
        // Parse course IDs and initialize allotments
        while (token!=NULL && allotment_count<100){
            int course_id=atoi(token);
            if (!course_id_exists(course_id)){
                allotments[allotment_count].course_id=course_id;
                allotments[allotment_count].student_count=0;
                allotment_count++;
            }
            token=strtok(NULL, ",");
        }
    }

    // Read subsequent lines to get student IDs
    while (fgets(line, sizeof(line), file)){
        line[strcspn(line, "\n")]='\0';
        char *token=strtok(line, ",");

        int course_idx=0;
        while (token!=NULL && course_idx<allotment_count){
            if (allotments[course_idx].student_count<100){
                allotments[course_idx].student_ids[allotments[course_idx].student_count++]=atoi(token);
            }
            token=strtok(NULL, ",");
            course_idx++;
        }
    }
    fclose(file);
}

// Function to read existing course IDs from allotments.csv
void read_existing_allotments(const char *filename, int existing_course_ids[], int *existing_count){
    FILE *file=fopen(filename, "r");
    if (!file){
        *existing_count=0;
        return;
    }

    char line[MAX_LINE_LENGTH];
    *existing_count=0;

    while (fgets(line, sizeof(line), file)){
        int course_id;
        sscanf(line, "%d", &course_id);
        existing_course_ids[(*existing_count)++]=course_id;
    }

    fclose(file);
}

// Function to check if a course_id already exists in the existing_course_ids array
int existing_course_id_exists(int course_id, int existing_course_ids[], int existing_count){
    for (int i=0; i<existing_count; i++){
        if (existing_course_ids[i]==course_id){
            return 1;
        }
    }
    return 0;
}

// Function to append allotments data to allotments.csv
void append_allotments_to_csv_nrmp(const char *filename){
    int existing_course_ids[100];
    int existing_count;

    read_existing_allotments(filename, existing_course_ids, &existing_count);

    FILE *file=fopen(filename, "a");
    if (!file){
        perror("Error opening file for appending");
        return;
    }

    for (int i=0; i<allotment_count; i++){
        if (!existing_course_id_exists(allotments[i].course_id, existing_course_ids, existing_count)){
            fprintf(file, "%d,%d", allotments[i].course_id, allotments[i].student_count);
            for (int j=0; j<allotments[i].student_count; j++){
                fprintf(file, ",%d", allotments[i].student_ids[j]);
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

// Function to encapsulate the entire seat allocation process
void performSeatAllocation(int max_courses, const char *stu_pref_file, const char *uni_pref_file, const char *output_file){
    // Get dimensions of the CSV files
    getCSVDimensions(stu_pref_file, &STU_ROWS, &STU_COLS);
    getCSVDimensions(uni_pref_file, &UNI_ROWS, &UNI_COLS);
    NRMP_COLS=UNI_COLS;

    // Allocate memory for arrays
    stu=(int **)malloc(STU_ROWS * sizeof(int *));
    for (int i=0; i<STU_ROWS; i++){
        stu[i]=(int *)malloc(STU_COLS * sizeof(int));
    }

    uni=(int **)malloc(UNI_ROWS * sizeof(int *));
    for (int i=0; i<UNI_ROWS; i++){
        uni[i]=(int *)malloc(UNI_COLS * sizeof(int));
    }

    nrmp=(int **)malloc(NRMP_ROWS * sizeof(int *));
    for (int i=0; i<NRMP_ROWS; i++){
        nrmp[i]=(int *)malloc(NRMP_COLS * sizeof(int));
    }

    // Read data from CSV files
    readCSV(stu_pref_file, stu, STU_ROWS, STU_COLS);
    readCSV(uni_pref_file, uni, UNI_ROWS, UNI_COLS);

    // Create nrmp array based on university preferences
    create_nrmp(uni_pref_file, nrmp);

    // Perform seat allocation for each student column
    for (int stu_col=0; stu_col<STU_COLS; stu_col++){
        int start=1;
        int allot=0;
        sel(stu_col, stu, uni, nrmp, start, &allot, max_courses);
    }

    // Write nrmp array to output CSV file
    writeCSV(output_file, nrmp, NRMP_ROWS, NRMP_COLS);
    const char *output_final_file="allotments.csv";
    read_elec_allotment_data(output_file);
    append_allotments_to_csv_nrmp(output_final_file);
    // Free allocated memory
    for (int i=0; i<STU_ROWS; i++){
        free(stu[i]);
    }
    free(stu);

    for (int i=0; i<UNI_ROWS; i++){
        free(uni[i]);
    }
    free(uni);

    for (int i=0; i<NRMP_ROWS; i++){
        free(nrmp[i]);
    }
    free(nrmp);
}

//Function to find the index of course
int find_course_index(Courses courses[], int course_count, int course_id){
    for (int i=0; i<course_count; i++){
        if (courses[i].course_id==course_id){
            return i;
        }
    }
    return -1;
}

//Function to update the vacancy details for a given course
void update_vacancy_file(int course_id, int seats_remaining){
    FILE *file=fopen("vacancy.csv", "r+");
    if (!file){
        perror("Unable to open vacancy.csv");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char temp_filename[]="temp_vacancy.csv";
    FILE *temp_file=fopen(temp_filename, "w");
    if (!temp_file){
        perror("Unable to open temporary file");
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)){
        int file_course_id, file_seats;
        sscanf(line, "%d,%d", &file_course_id, &file_seats);

        if (file_course_id==course_id){
            fprintf(temp_file, "%d,%d\n", file_course_id, seats_remaining);
        } else {
            fputs(line, temp_file);
        }
    }

    fclose(file);
    fclose(temp_file);

    remove("vacancy.csv");
    rename(temp_filename, "vacancy.csv");
}

//Function to read allotments file
void read_allotments_from_csv_normal(const char *filename){
    FILE *file=fopen(filename, "r");
    if (!file){
        perror("Error opening file for reading");
        return;
    }

    allotment_count=0;
    while (fscanf(file, "%d,%d", &allotments[allotment_count].course_id, &allotments[allotment_count].student_count)==2){
        for (int j=0; j<allotments[allotment_count].student_count; j++){
            fscanf(file, ",%d", &allotments[allotment_count].student_ids[j]);
        }
        allotment_count++;
    }

    fclose(file);
}

//Function to write into allotments
void append_allotments_to_csv(const char *filename){
    FILE *file=fopen(filename, "a");
    if (!file){
        perror("Error opening file for appending");
        return;
    }

    for (int i=0; i<allotment_count; i++){
        fprintf(file, "%d,%d", allotments[i].course_id, allotments[i].student_count);
        for (int j=0; j<allotments[i].student_count; j++){
            fprintf(file, ",%d", allotments[i].student_ids[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

//Function for allotment of a course without waitlist
void course_allotment(int course_id, int student_ids[], int student_count){
    int found=0;

    // Check if the course_id already has allotments
    for (int i=0; i<allotment_count; i++){
        if (allotments[i].course_id==course_id){
            // Add students to existing allotment
            for (int j=0; j<student_count; j++){
                int exists=0;
                for (int k=0; k<allotments[i].student_count; k++){
                    if (allotments[i].student_ids[k]==student_ids[j]){
                        exists=1;
                        break;
                    }
                }
                if (!exists){
                    allotments[i].student_ids[allotments[i].student_count++]=student_ids[j];
                }
            }
            found=1;
            break;
        }
    }

    // If course_id doesn't have allotments, create new entry
    if (!found){
        allotments[allotment_count].course_id=course_id;
        allotments[allotment_count].student_count=student_count;
        for (int j=0; j<student_count; j++){
            allotments[allotment_count].student_ids[j]=student_ids[j];
        }
        allotment_count++;
    }
}

//Function to update allotments to file
void update_allotments_to_csv(const char *filename){
    FILE *file=fopen(filename, "r+");
    if (!file){
        perror("Error opening file for reading and writing");
        return;
    }

    // Read the existing allotments into a temporary array
    Allotment existing_allotments[100];
    int existing_allotment_count=0;

    while (fscanf(file, "%d,%d", &existing_allotments[existing_allotment_count].course_id, &existing_allotments[existing_allotment_count].student_count)==2){
        for (int j=0; j<existing_allotments[existing_allotment_count].student_count; j++){
            fscanf(file, ",%d", &existing_allotments[existing_allotment_count].student_ids[j]);
        }
        existing_allotment_count++;
    }

    // Check for new allotments and append if not present
    fseek(file, 0, 2);
    for (int i=0; i<allotment_count; i++){
        int exists=0;
        for (int j=0; j<existing_allotment_count; j++){
            if (allotments[i].course_id==existing_allotments[j].course_id){
                exists=1;
                break;
            }
        }
        if (!exists){
            fprintf(file, "%d,%d", allotments[i].course_id, allotments[i].student_count);
            for (int j=0; j<allotments[i].student_count; j++){
                fprintf(file, ",%d", allotments[i].student_ids[j]);
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

//Function for choice of faculty
void faculty_choice(const char *course_id){
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char *fields[MAX_FIELDS];
    int field_count, found=0, available=0;

    // Open the faculty CSV file for reading
    file=fopen("faculty.csv", "r");
    if (file==NULL){
        printf("Error opening file.\n");
        return;
    }

    while (fgets(line, sizeof(line), file)!=NULL){
        // Remove newline character if present
        line[strcspn(line, "\n")]='\0';

        // Parse the line
        field_count=parse_line(line, fields);
        // Check if the course_id matches
        if (field_count>1 && strcmp(fields[1], course_id)==0){
            if (atoi(fields[4])>0){ // Check if seats are available
                // Print the faculty details
                printf("Faculty ID: %s\n", fields[0]);
                printf("Course ID: %s\n", fields[1]);
                printf("Name of Faculty: %s\n", fields[2]);
                printf("Section: %s\n",fields[5]);
                printf("-------------------------------------------------\n");
                available=1;
            }
            found=1;
        }
    }

    if (!found){
        printf("Invalid course ID\n");
    } else if (!available){
        printf("No faculty available for this course at the moment. Please come back later\n");
    }

    fclose(file);
}

//Function to allot faculty
char* fallot(const char *course_id, const char *faculty_id, int *seats, char *fac_name, char *sec){
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char *fields[MAX_FIELDS];
    char *final_id=malloc(10 * sizeof(char));
    if (final_id==NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }
    strcpy(final_id, " ");

    // Open the faculty CSV file for reading
    file=fopen("faculty.csv", "r");
    if (file==NULL){
        printf("Error opening file.\n");
        return NULL;
    }

    while (fgets(line, sizeof(line), file)!=NULL){
        // Parse the line
        int field_count=parse_line(line, fields);

        // Check if the course_id and faculty_id match
        if (field_count>1 && strcmp(fields[1], course_id)==0 && strcmp(fields[0], faculty_id)==0){
            if (atoi(fields[4])>0){ // Check if seats are available
                *seats=atoi(fields[4]) - 1;
                strcpy(fac_name, fields[2]);
                strcpy(final_id, fields[0]);
                strcpy(sec, fields[5]);
                fclose(file);
                return final_id;
            }
        }
    }

    fclose(file);
    return final_id;
}

//Function to update faculty
void update_faculty(const char *filename, const char *faculty_id, const char *course_id, const char *new_seats){
    FILE *file, *temp_file;
    char line[MAX_LINE_LENGTH];
    char *fields[MAX_FIELDS];
    int field_count;
    int updated=0;
    char temp_filename[L_tmpnam];

    // Generate a unique temporary file name
    tmpnam(temp_filename);

    // Open the CSV file for reading
    file=fopen(filename, "r");
    if (file==NULL){
        perror("Error opening file");
        return;
    }

    // Open a temporary file for writing
    temp_file=fopen(temp_filename, "w");
    if (temp_file==NULL){
        perror("Error opening temporary file");
        fclose(file);
        return;
    }

    // Read and parse each line of the file
    while (fgets(line, sizeof(line), file)!=NULL){
        // Remove newline character if present
        line[strcspn(line, "\n")]='\0';

        // Make a copy of the line to preserve the original line for writing
        char original_line[MAX_LINE_LENGTH];
        strcpy(original_line, line);

        // Parse the line
        field_count=parse_line(line, fields);

        // Check if the faculty_id and course_id match
        if (field_count>2 && strcmp(fields[0], faculty_id)==0 && strcmp(fields[1], course_id)==0){
            // Update the seats_available field
            snprintf(fields[4], MAX_LINE_LENGTH, "%s", new_seats);  // Update the specific field correctly
            updated=1;

            // Write the updated line to the temporary file
            for (int i=0; i<field_count; i++){
                fprintf(temp_file, "%s", fields[i]);
                if (i<field_count - 1){
                    fprintf(temp_file, ",");
                }
            }
            fprintf(temp_file, "\n");
        } else {
            // Write the original line to the temporary file
            fprintf(temp_file, "%s\n", original_line);
        }
    }

    // Close the files
    fclose(file);
    fclose(temp_file);

}

//Function to handle selection of faculty
void handle_faculty_selection(int course_id){
    char faculty_id[10];
    char new_seats[5];
    int seats;
    char cid[10];
    char fac_name[50];
    char sec[3];
    sprintf(cid, "%d", course_id);
    printf("--------------------------------------------------------\n");
    printf("THE FACULTY OPTIONS FOR YOUR COURSE:\n");
    printf("--------------------------------------------------------\n");
    faculty_choice(cid);
    printf("FROM THE DISPLAYED RESULTS PLEASE ENTER FACULTY ID OF THE FACULTY OF YOUR CHOICE: ");
    scanf("%s", faculty_id);

    char *final_faculty_id=fallot(cid, faculty_id, &seats, fac_name, sec);
    while (1){
        if (strcmp(final_faculty_id, " ")!=0){
            printf("--------------------------------------------------------\n");
            printf("Faculty details that you have been allotted:\n");
            printf("COURSE ID: %s\n", cid);
            printf("FACULTY ID: %s\n", final_faculty_id);
            printf("FACULTY NAME: %s\n", fac_name);
            printf("SECTION: %s \n", sec);
            printf("--------------------------------------------------------\n");
            update_faculty("faculty.csv", final_faculty_id, cid, new_seats);
            free(final_faculty_id);
            break;
        } else {
            printf("Invalid faculty ID, please enter again\n");
            printf("--------------------------------------------------------\n");
            printf("THE FACULTY OPTIONS FOR YOUR COURSE:\n");
            printf("--------------------------------------------------------\n");
            faculty_choice(cid);
            printf("FROM THE DISPLAYED RESULTS PLEASE ENTER FACULTY ID OF THE FACULTY OF YOUR CHOICE: ");
            scanf("%s", faculty_id);
            final_faculty_id=fallot(cid, faculty_id, &seats, fac_name, sec);
        }
    }
}

//Function to read allotments 
void read_allotments_from_csv(const char *filename,Allotment allotment[],int *allotment_count){
    FILE *file=fopen(filename, "r");
    if (!file){
        perror("Error opening file for reading");
        return;
    }

    *allotment_count=0;
    while (fscanf(file, "%d,%d", &allotments[*allotment_count].course_id, &allotments[*allotment_count].student_count)==2){
        for (int j=0; j<allotments[*allotment_count].student_count; j++){
            fscanf(file, ",%d", &allotments[*allotment_count].student_ids[j]);
        }
        (*allotment_count)++;
    }

    fclose(file);
}

//Function to check vacancies and allot seats
void allot_seats(int id, int course_id, Student students[], int num_students){
    // Iterate through each student
    for (int i=0; i<num_students; i++){
        if (id==students[i].student_id){
            printf("Student ID: %d\n", students[i].student_id);

            // Handle faculty selection for core courses
            for (int j=0; j<students[i].num_core; j++){
                if (students[i].core[j]==course_id){
                    handle_faculty_selection(course_id);
                    break;
                }
            }

            // Handle faculty selection for elective courses
            for (int j=0; j<students[i].num_elective; j++){
                if (students[i].elective[j]==course_id){
                    handle_faculty_selection(course_id);
                    break;
                }
            }
        }
    }
}

//Function for seat allotment module
void seat_allotment(int student_id, int sem){
    char filename[30];
    sprintf(filename, "courses_selected_sem%d.csv", sem);
    Student students[100];
    int num_students;
    
    read_data(filename, students, &num_students);

    read_allotments_from_csv("allotments.csv", allotments, &allotment_count);

    printf("Allotment status for student ID %d:\n", student_id);

    // Iterate through each allotment
    for (int i=0; i<allotment_count; i++){
        int student_found=0;
        for (int j=0; j<allotments[i].student_count; j++){
            if (allotments[i].student_ids[j]==student_id){
                student_found=1;
                if (allotments[i].student_count<=60){
                    printf("Course ID: %d\n", allotments[i].course_id);
                    allot_seats(student_id, allotments[i].course_id, students, num_students);
                } else {
                    if (j<=60 ){
                        printf("Course ID: %d\n", allotments[i].course_id);
                        allot_seats(student_id, allotments[i].course_id, students, num_students);
                    }
                    else if (j<66){
                        int waitlist_number=j - 60 + 1; // Calculate waitlist number
                        printf("Course ID: %d - WAITLIST NUMBER: %d\n", allotments[i].course_id, waitlist_number);
                        printf("\nPlease wait for further updates on the waitlist\n");
                        printf("---------------------------------------------\n");
                    }
                    else{
                        printf("Course ID: %d - REJECTED\n", allotments[i].course_id);
                        printf("---------------------------------------------\n");
                    }
                    
                }
                break;
            }
        }
    }
}

//Function for course handling from admin side
int courses_selected_admin(int sem){
    int core_start, core_end, elective_start, elective_end, flag=0;
    const char *filename;

    if (sem==2){
        filename="courses_selected_sem2.csv";
        core_start=201;
        core_end=204;
        elective_start=205;
        elective_end=206;
        flag=2;
    } else if (sem==4){
        filename="courses_selected_sem4.csv";
        core_start=401;
        core_end=404;
        elective_start=405;
        elective_end=406;
        flag=4;
    } else if (sem==6){
        filename="courses_selected_sem6.csv";
        core_start=601;
        core_end=604;
        elective_start=605;
        elective_end=606;
        flag=6;
    } else {
        printf("Currently not handling odd semester\n");
        return 0;
    }

    FILE *file=fopen(filename, "r");
    if (!file){
        perror("Unable to open file");
        return 0;
    }

    Courses courses[20];
    int course_count=0;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)){
        size_t length=strlen(line);
        if (length>0 && line[length - 1]=='\n'){
            line[length - 1]='\0';
        }

        char *token=strtok(line, ",");
        int student_id=atoi(token);

        while ((token=strtok(NULL, ","))!=NULL){
            int course_id=atoi(token);
            if (course_id!=0){
                int index=find_course_index(courses, course_count, course_id);
                if (index==-1){
                    index=course_count++;
                    courses[index].course_id=course_id;
                    courses[index].student_count=0;
                }
                courses[index].student_ids[courses[index].student_count++]=student_id;
            }
        }
    }

    fclose(file);

    printf("Number of applicants:\n");
    printf("Core Courses:\n");
    for (int i=core_start; i<=core_end; i++){
        int index=find_course_index(courses, course_count, i);
        if (index!=-1){
            printf("Course ID %d: %d\n", courses[index].course_id, courses[index].student_count);
            if (courses[index].student_count<=60){
                printf("Procceeding with normal allotment\n");
                printf("---------------------------\n");
                read_allotments_from_csv_normal("allotments.csv");
                course_allotment(courses[index].course_id, courses[index].student_ids, courses[index].student_count);
                update_allotments_to_csv("allotments.csv");
                // Calculate remaining seats and update vacancy file if needed
                int seats_remaining=60-courses[index].student_count;
                update_vacancy_file(courses[index].course_id, seats_remaining);
            }
            else{
                printf("Procceeding with waitlist allotment\n");
                printf("---------------------------\n");
                char student_preference[50], university_preference[50], allotment[50];
                sprintf(student_preference, "stud_core_pref_sem%d.csv", flag);    
                sprintf(university_preference, "uni_core_pref_sem%d.csv", flag);  
                sprintf(allotment, "core_allotment_sem%d.csv", flag);             
                performSeatAllocation(MAX_CORE_COURSE, student_preference, university_preference, allotment);
            }
        }
    }

    printf("Elective Courses:\n");
    for (int i=elective_start; i<=elective_end; i++){
        int index=find_course_index(courses, course_count, i);
        if (index!=-1){
            printf("Course ID %d: %d\n", courses[index].course_id, courses[index].student_count);
            if (courses[index].student_count<=60){
                printf("Procceeding with normal allotment\n");
                printf("---------------------------\n");
                read_allotments_from_csv_normal("allotments.csv");
                course_allotment(courses[index].course_id, courses[index].student_ids, courses[index].student_count);
                update_allotments_to_csv("allotments.csv");
                // Calculate remaining seats and update vacancy file if needed
                int seats_remaining=60-courses[index].student_count;
                update_vacancy_file(courses[index].course_id, seats_remaining);
            }
            else{
                printf("Procceeding with waitlist allotment\n");
                printf("---------------------------\n");
                char student_preference[50], university_preference[50], allotment[50];
                sprintf(student_preference, "stud_elec_pref_sem%d.csv", flag);    
                sprintf(university_preference, "uni_elec_pref_sem%d.csv", flag);  
                sprintf(allotment, "elec_allotment_sem%d.csv", flag);             
                performSeatAllocation(MAX_CORE_COURSE, student_preference, university_preference, allotment);
            }
        }
    }

    return 1;
}

//MAIN
int main(){
    FILE *stud_file=fopen("login.csv","r");
    FILE *admin_file=fopen("admin.csv","r");
    int ch, id, n, ch2;
    printf("**************************************************\n\nWELCOME TO UNIVERSITY COURSE REGISTRATION SYSTEM\n\n**************************************************\n\n");
    printf("**************\nLOGIN\n**************");
    printf("\n1. Student\n2. Admin\n**************\nEnter your choice: ");
    scanf("%d",&ch);
    if (ch==1){
        id=Login(stud_file, 1);
        if (id){
            Student student;
            student.student_id=id;
            char cgpa[5];
            int sem=display_profile(id, cgpa);
            int ch2;
            while (1){
                printf("**********************************\nEnter:\n1 to select courses\n2 to drop course\n3 to add course\n4 to view status of allotment\n0 to quit: ");
                scanf("%d",&ch2);
                if (!ch2)
                    break;
                else if (ch2==1)
                    course_selection_student(id,sem,cgpa);
                else if (ch2==2)
                    drop_course_student(id,sem);
                else if (ch2==3){  
                    if (add_course_student(id, sem)!=0) 
                        printf("Error in adding course\n");
                    else
                        printf("Course added succcessfully\n");
                }
                else if (ch2==4)
                    seat_allotment(id,sem);
            }
        }
    }   

    else if (ch==2){
        id=101;
        id=Login(admin_file, 2);
        if (id){
            while (1){
                int ch2;
                printf("Enter:\n1. Courses handled\n2. Add new course\n0 to quit: ");
                scanf("%d",&ch2);
                if (ch2==0)
                    break;  
                else if (ch2==1){
                    printf("Enter semester to look at number of applicants in each course: ");
                    scanf("%d", &n);
                    courses_selected_admin(n);
                }
                else if (ch2==2)
                    add_course_admin();
                else  
                    printf("Invalid choice\n");
            }
        }
    }
    else
        printf("Invalid choice\n"); 
    return 0;
}
