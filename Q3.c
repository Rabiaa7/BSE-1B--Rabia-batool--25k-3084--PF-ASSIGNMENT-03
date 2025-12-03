//25k-3084(employee management)
#include <stdio.h>
#include <string.h>

struct Employee {
    int id;
    char name[50];
    char designation[50];
    float salary;
};

void inputEmployees(struct Employee emp[], int n) {
    for (int i = 0; i < n; i++) {
        printf("\nEnter details for Employee %d:\n", i + 1);
        printf("ID: ");
        scanf("%d", &emp[i].id);
        getchar(); 

        printf("Name: ");
        fgets(emp[i].name, sizeof(emp[i].name), stdin);
        emp[i].name[strcspn(emp[i].name, "\n")] = '\0';

        printf("Designation: ");
        fgets(emp[i].designation, sizeof(emp[i].designation), stdin);
        emp[i].designation[strcspn(emp[i].designation, "\n")] = '\0';

        printf("Salary: ");
        scanf("%f", &emp[i].salary);
    }
}

void displayEmployees(struct Employee emp[], int n) {
    printf("\n%-5s %-20s %-20s %-10s\n", "ID", "Name", "Designation", "Salary");
    printf("-----------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-5d %-20s %-20s %-10.2f\n", emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
    }
}
void findHighestSalary(struct Employee emp[], int n) {
    int index = 0;
    for (int i = 1; i < n; i++) {
        if (emp[i].salary > emp[index].salary) {
            index = i;
        }
    }
    printf("\nEmployee with Highest Salary:\n");
    printf(" ID: %d \n Name: %s \n Designation: %s \n Salary: %.2f\n",
           emp[index].id, emp[index].name, emp[index].designation, emp[index].salary);
}

void searchEmployee(struct Employee emp[], int n) {
    int choice;
    printf("\nSearch by: 1. ID  2. Name\nChoice: ");
    scanf("%d", &choice);
    getchar(); 

    if (choice == 1) {
        int searchId;
        printf("Enter Employee ID to search: ");
        scanf("%d", &searchId);

        for (int i = 0; i < n; i++) {
            if (emp[i].id == searchId) {
                printf("Employee Found:\n ID: %d \n Name: %s \n Designation: %s \n Salary: %.2f\n",
                       emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
                return;
            }
        }
        printf("Employee with ID %d not found.\n", searchId);

    } else if (choice == 2) {
        char searchName[50];
        printf("Enter Employee Name to search: ");
        fgets(searchName, sizeof(searchName), stdin);
        searchName[strcspn(searchName, "\n")] = '\0';

        for (int i = 0; i < n; i++) {
            if (strcmp(emp[i].name, searchName) == 0) {
                printf("Employee Found:\n ID: %d \n Name: %s \n Designation: %s \n Salary: %.2f\n",
                       emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
                return;
            }
        }
        printf("Employee with Name '%s' not found.\n", searchName);

    } else {
        printf("Invalid choice.\n");
    }
}

void giveBonus(struct Employee emp[], int n, float threshold) {
    for (int i = 0; i < n; i++) {
        if (emp[i].salary < threshold) {
            emp[i].salary *= 1.1;
        }
    }
    printf("\nSalaries updated for employees below %.2f\n", threshold);
}

int main() {
    int n;
    printf("Enter number of employees: ");
    scanf("%d", &n);

    struct Employee employees[100]; 

    inputEmployees(employees, n);
    displayEmployees(employees, n);
    findHighestSalary(employees, n);
    searchEmployee(employees, n);
    giveBonus(employees, n, 50000);
    displayEmployees(employees, n);

    return 0;
}
