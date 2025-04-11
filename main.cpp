
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <iomanip>

using namespace std;

struct Employee {
    int id;
    string name;
    string position;
    string empType;
    int basicSalary;
    int hourlyWage;
    int totalHours;
    int leaveDays;
    int overtimeHours;
};

const map<string, int> FULL_TIME_BASE_SALARIES = {
    {"Teacher", 4500},
    {"Administrator", 2300},
    {"Janitor", 1700},
    {"Librarian", 2000}
};

const map<int, int> LEAVE_BONUSES = {
    {0, 150},
    {1, 100},
    {2, 50}
};

const float OVERTIME_MULTIPLIER = 2.0f;
const float LEAVE_OVERTIME_DEDUCTION_RATE = 0.01f;
const float INCOME_TAX_RATE = 0.02f;
const int SOCIAL_SECURITY_TAX = 10;

int calculateSalary(const Employee& emp);
void addEmployee(vector<Employee>& employees);
void displayEmployees(const vector<Employee>& employees);
void searchEmployeeById(const vector<Employee>& employees);
void calculateTotalPayroll(const vector<Employee>& employees);
int getValidNumber(const string& prompt, int minVal = 0, int maxVal = INT_MAX);

int main() {
    vector<Employee> employees;
    int choice;

    do {
        cout << "\n===== Employee Salary System =====\n";
        cout << "1. Add Employee\n";
        cout << "2. Display Employees (with Salary)\n";
        cout << "3. Search Employee by ID\n";
        cout << "4. Calculate Total Payroll\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                addEmployee(employees);
                break;
            case 2:
                displayEmployees(employees);
                break;
            case 3:
                searchEmployeeById(employees);
                break;
            case 4:
                calculateTotalPayroll(employees);
                break;
            case 5:
                cout << "Exiting system...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 5);

    return 0;
}

int calculateSalary(const Employee& emp) {
    int baseSalary = emp.basicSalary;
    if (emp.empType == "Part-time" || emp.empType == "Contract") {
        baseSalary = emp.hourlyWage * emp.totalHours;
    }

    int overtimePay = emp.overtimeHours * (emp.hourlyWage * OVERTIME_MULTIPLIER);
    int leaveBonus = (emp.leaveDays <= 2 && LEAVE_BONUSES.count(emp.leaveDays)) ? LEAVE_BONUSES.at(emp.leaveDays) : 0;
    float deductions = (emp.leaveDays > 4 && emp.overtimeHours > 0) ? baseSalary * LEAVE_OVERTIME_DEDUCTION_RATE : 0;

    float grossSalary = baseSalary + overtimePay + leaveBonus - deductions;
    float incomeTax = grossSalary * INCOME_TAX_RATE;
    float netSalary = grossSalary - incomeTax - SOCIAL_SECURITY_TAX;

    return static_cast<int>(netSalary);
}

void addEmployee(vector<Employee>& employees) {
    Employee newEmp;

    cout << "\n=== Add New Employee ===\n";
    cout << "Employee ID: ";
    cin >> newEmp.id;
    cin.ignore();

    cout << "Name: ";
    getline(cin, newEmp.name);

    cout << "Position (Teacher/Administrator/Janitor/Librarian): ";
    getline(cin, newEmp.position);

    cout << "Employment Type (Full-time/Part-time/Contract): ";
    getline(cin, newEmp.empType);

    newEmp.basicSalary = 0;
    newEmp.hourlyWage = 0;
    newEmp.totalHours = 0;

    if (newEmp.empType == "Full-time" && FULL_TIME_BASE_SALARIES.count(newEmp.position)) {
        newEmp.basicSalary = FULL_TIME_BASE_SALARIES.at(newEmp.position);
        cout << "Basic Salary set to: $" << newEmp.basicSalary << " (Full-time)\n";
    } else if (newEmp.empType == "Part-time" || newEmp.empType == "Contract") {
        cout << "Hourly Wage: $";
        cin >> newEmp.hourlyWage;
        cout << "Total Working Hours: ";
        cin >> newEmp.totalHours;
    } else {
        cout << "Warning: Basic salary for this type/position not defined.\n";
    }

    cout << "Leave Days Taken: ";
    cin >> newEmp.leaveDays;
    cout << "Overtime Hours: ";
    cin >> newEmp.overtimeHours;

    employees.push_back(newEmp);
    cout << "\nEmployee added successfully!\n";
}

void displayEmployees(const vector<Employee>& employees) {
    if (employees.empty()) {
        cout << "\nNo employees in the system!\n";
        return;
    }

    cout << "\n=== Employee List ===\n";
    cout << left << setw(8) << "ID" << setw(20) << "Name" << setw(15) << "Position"
         << setw(12) << "Type" << setw(12) << "Base Salary" << setw(10) << "Net Salary" << endl;
    cout << string(77, '-') << endl;

    for (const auto& emp : employees) {
        cout << left << setw(8) << emp.id << setw(20) << emp.name
             << setw(15) << emp.position << setw(12) << emp.empType
             << "$" << setw(11) << emp.basicSalary << "$" << setw(9) << calculateSalary(emp) << endl;
    }
}

void searchEmployeeById(const vector<Employee>& employees) {
    int searchId;
    cout << "Enter Employee ID to search: ";
    cin >> searchId;

    for (const auto& emp : employees) {
        if (emp.id == searchId) {
            cout << "\n--- Employee Found ---\n";
            cout << "ID: " << emp.id << "\nName: " << emp.name
                 << "\nPosition: " << emp.position << "\nType: " << emp.empType
                 << "\nNet Salary: $" << calculateSalary(emp) << endl;
            return;
        }
    }
    cout << "Employee not found.\n";
}

void calculateTotalPayroll(const vector<Employee>& employees) {
    int total = 0;
    for (const auto& emp : employees) {
        total += calculateSalary(emp);
    }
    cout << "\nTotal Payroll (Net Salary for all employees): $" << total << endl;
}
