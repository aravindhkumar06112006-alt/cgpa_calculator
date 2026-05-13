/*
 * ============================================================
 *   CGPA CALCULATOR
 *   Semester GPA + Cumulative CGPA  |  10-Point Grading Scale
 * ============================================================
 *
 *  Compile : g++ -o cgpa_calculator.cpp main
 *  Run     : ./main
 * ============================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>

using namespace std;

struct Course {
    string name;
    string grade;
    int    gradePoint;
    int    credits;
    double weightedPoints;
};

int gradeToPoint(const string& g) {
    if (g == "O"  || g == "o")  return 10;
    if (g == "A+" || g == "a+") return 9;
    if (g == "A"  || g == "a")  return 8;
    if (g == "B+" || g == "b+") return 7;
    if (g == "B"  || g == "b")  return 6;
    if (g == "C"  || g == "c")  return 5;
    if (g == "U"  || g == "u")  return 0;
    return -1;
}

string getStatus(int gp) {
    if (gp == 10) return "Outstanding";
    if (gp == 9)  return "Excellent";
    if (gp == 8)  return "Very Good";
    if (gp == 7)  return "Good";
    if (gp == 6)  return "Average";
    if (gp == 5)  return "Pass";
    return "FAIL";
}

string getRemark(double cgpa) {
    if (cgpa >= 9.5) return "Exceptional — top of the class!";
    if (cgpa >= 9.0) return "Outstanding performance!";
    if (cgpa >= 8.0) return "Excellent work!";
    if (cgpa >= 7.0) return "Good standing.";
    if (cgpa >= 6.0) return "Satisfactory.";
    if (cgpa >= 5.0) return "Passed — push harder next semester!";
    return "Below passing. Seek academic counselling.";
}

void line(char ch = '-', int w = 68) {
    cout << string(w, ch) << "\n";
}

void clearCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt, int lo, int hi) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v && v >= lo && v <= hi) return v;
        cout << "  [!] Enter a number between " << lo << " and " << hi << ".\n";
        clearCin();
    }
}

double readDouble(const string& prompt) {
    double v;
    while (true) {
        cout << prompt;
        if (cin >> v && v >= 0) return v;
        cout << "  [!] Enter a valid non-negative number.\n";
        clearCin();
    }
}

void showGradeScale() {
    line();
    cout << "  GRADE SCALE  (10-point system)\n";
    line();
    cout << "  O  = 10  Outstanding     A+ =  9  Excellent\n";
    cout << "  A  =  8  Very Good       B+ =  7  Good\n";
    cout << "  B  =  6  Average         C  =  5  Pass\n";
    cout << "  U  =  0  Fail\n";
    line();
}

vector<Course> inputCourses(int n) {
    vector<Course> list;
    for (int i = 0; i < n; i++) {
        Course c;
        cout << "\n  [ Course " << (i+1) << " / " << n << " ]\n";

        cout << "    Name    (no spaces, e.g. OS, DataStructures) : ";
        cin >> c.name;

        while (true) {
            cout << "    Grade   (O / A+ / A / B+ / B / C / U)       : ";
            cin >> c.grade;
            c.gradePoint = gradeToPoint(c.grade);
            if (c.gradePoint != -1) break;
            cout << "  [!] Invalid. Choose from: O  A+  A  B+  B  C  U\n";
        }

        c.credits        = readInt("    Credits (1 - 6)                              : ", 1, 6);
        c.weightedPoints = (double)c.gradePoint * c.credits;
        list.push_back(c);
    }
    return list;
}

void printResults(const vector<Course>& courses,
                  double semCr, double semPts, double gpa,
                  double prevCr, double prevPts, double cgpa)
{
    cout << "\n";
    line('=');
    cout << "  RESULTS\n";
    line('=');

    cout << left
         << "  " << setw(20) << "Course"
         << setw(10) << "Grade"
         << setw(10) << "Credits"
         << setw(14) << "Grd Pts"
         << "Status\n";
    line();

    for (const auto& c : courses) {
        string lbl = c.grade + "(" + to_string(c.gradePoint) + ")";
        cout << left
             << "  " << setw(20) << c.name
             << setw(10) << lbl
             << setw(10) << c.credits
             << setw(14) << fixed << setprecision(1) << c.weightedPoints
             << getStatus(c.gradePoint) << "\n";
    }

    line();
    cout << right
         << setw(40) << "Total Credits       : " << (int)semCr << "\n"
         << setw(40) << "Total Grade Points  : " << fixed << setprecision(2) << semPts << "\n";

    line();
    cout << setw(40) << "Semester GPA        : " << fixed << setprecision(2) << gpa  << " / 10.00\n";

    if (prevCr > 0) {
        cout << setw(40) << "Prev. Credits       : " << (int)prevCr  << "\n"
             << setw(40) << "Prev. Grade Points  : " << fixed << setprecision(2) << prevPts << "\n";
    }

    cout << setw(40) << "Overall CGPA        : " << fixed << setprecision(2) << cgpa << " / 10.00\n";
    line('=');
    cout << "  Remarks : " << getRemark(cgpa) << "\n";
    line('=');
}

void runSemester(double& cumCr, double& cumPts, int semNum) {
    cout << "\n"; line('=');
    cout << "  SEMESTER " << semNum << "\n";
    line('=');

    int n = readInt("\n  Number of courses this semester (1 - 12) : ", 1, 12);
    vector<Course> courses = inputCourses(n);

    double semCr = 0, semPts = 0;
    for (const auto& c : courses) { semCr += c.credits; semPts += c.weightedPoints; }

    double gpa  = semCr > 0 ? semPts / semCr : 0.0;
    double cgpa = (semCr + cumCr) > 0 ? (semPts + cumPts) / (semCr + cumCr) : 0.0;

    printResults(courses, semCr, semPts, gpa, cumCr, cumPts, cgpa);

    cumCr  += semCr;
    cumPts += semPts;
}

int main() {
    cout << "\n";
    line('=');
    cout << "      CGPA CALCULATOR  —  Semester GPA & Cumulative CGPA\n";
    line('=');

    showGradeScale();

    double cumCr = 0, cumPts = 0;
    cout << "\n  Previous semester data available? (1=Yes  0=No) : ";
    int hasPrev = readInt("", 0, 1);
    if (hasPrev == 1) {
        cumCr  = readDouble("  Previous total credits      : ");
        cumPts = readDouble("  Previous total grade points : ");
    }

    int semNum = 1, more = 1;
    while (more == 1) {
        runSemester(cumCr, cumPts, semNum++);
        more = readInt("\n  Add another semester? (1=Yes  0=No) : ", 0, 1);
    }

    cout << "\n";
    line('=');
    cout << "  FINAL SUMMARY\n";
    line('=');
    double finalCGPA = cumCr > 0 ? cumPts / cumCr : 0.0;
    cout << "  Cumulative Credits      : " << (int)cumCr  << "\n"
         << "  Cumulative Grade Points : " << fixed << setprecision(2) << cumPts  << "\n"
         << "  Final CGPA              : " << fixed << setprecision(2) << finalCGPA << " / 10.00\n"
         << "  Remarks                 : " << getRemark(finalCGPA) << "\n";
    line('=');
    cout << "  Good luck!\n\n";

    return 0;
}