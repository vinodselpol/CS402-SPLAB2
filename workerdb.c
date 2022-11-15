

#include <stdio.h>
#include <string.h>
#include "readfile.h"
#define MAXNAME 64

extern FILE *file;
struct employee
{
    char firstName[MAXNAME];
    char lastName[MAXNAME];
    int empID;
    int salary;
};
struct employee db[1024];
int total_employees = 0;
//char term;

//used to print out the DB
void PrintDB (struct employee emp[], int total_emp)
{
    int i;

    printf("\nName\t\t\t\tSALARY\t   ID\n");
    printf("----------------------------------------------\n");
    for (i = 0; i < total_emp; i++)
    {
        printf("%-15s\t%-15s\t%d\t%d\n", emp[i].firstName,emp[i].lastName,emp[i].salary, emp[i].empID);
    }
    printf("----------------------------------------------\n");
    printf(" Number of Employees (%d)\n", total_emp);
}

//Linear search employee by ID
int searchID (struct employee emp[], int total_emp, int id)
{
    int i;
    for (i = 0; i < total_emp; i++)
    {
        if (emp[i].empID == id)
        {
            return i;
        }
    }
    return -1;
}



//last name search
int searchLName (struct employee emp[], int total_emp, char* name)
{
    int i;
    for (i = 0; i < total_emp; i++)
    {
        if (strcmp(emp[i].lastName, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

//add new employee
int addEmp(struct employee emp[])
{
    char newFirstName[MAXNAME];
    char newLastName[MAXNAME];
    int newSalary;
    int confirm;

    printf("\nEnter first name: ");
    scanf("%s", newFirstName);
    if (strlen(newFirstName) > 64)
    {
        printf("Too Long. Try again.\n");
        return 0;
    }
    printf("Enter last name: ");
    scanf("%s", newLastName);
    if (strlen(newFirstName) > 64)
    {
        printf("Try a shorter name. \n");
        return 0;
    }
    printf("Enter employee's salary (30000 to 150000): ");
    scanf("%d", &newSalary);
    if (newSalary < 30000 || newSalary > 150000)
    {
        printf("Enter a salary within range. \n");
        return 0;
    }
    printf("\nAre you sure you want to add the employee?\n");
    printf("\t%s %s, \tSalary: %d\n", newFirstName, newLastName, newSalary);
    printf("\nEnter 1 - yes \n 0 - no: ");
    scanf("%d", &confirm);

    if (confirm == 1)
    {
        strcpy(db[total_employees].firstName, newFirstName);
        strcpy(db[total_employees].lastName, newLastName);
        db[total_employees].salary = newSalary;
        db[total_employees].empID = db[total_employees -1].empID + 1;
        total_employees++;
        printf("Added new employee successfully. \n");
    }
    return 1;
}

//create db
void populateDB(char* datafile)
{
    if (open_file(datafile) == 0)
    {
        char buffer[135];
        int ret;
        while (fgets(buffer, sizeof(buffer), file))
        {
            ret = read_int(buffer, 1, &db[total_employees].empID);
            ret = read_int(buffer, 2, &db[total_employees].salary);
            ret = read_string(buffer, 1, &db[total_employees].firstName);
            ret = read_string(buffer, 2, &db[total_employees].lastName);
            total_employees++;
        }

        if (feof(file)) // end of file
        {
            close_file(file);
        }
    }
}

//update the emplopyee"s information
int updateEmployee(int employeeDbIndexToUpdate)
{
    char newFirst[MAXNAME];
    char newLast[MAXNAME];
    int newSalary;
    int confirm;

    printf("\nEnter first name (input 0 to skip): ");
    scanf("%s", newFirst);
    if (strlen(newFirst) > 64)
    {
        printf("Too Long. Try again.\n");
        return 0;
    }
    
    printf("Enter last name (input 0 to skip): ");
    scanf("%s", newLast);
    if (strlen(newFirst) > 64)
    {
        printf("Too Long. Try again.\n");
        return 0;
    }
    printf("Enter employee's salary (30000 to 150000) (input 0 to skip): ");
    scanf("%d", &newSalary);
    if (newSalary < 30000 || newSalary > 150000)
    {
        if (newSalary != 0) {
            printf("Error, out of range.\n");
            return 0;
        }
    }
    printf("\nAre you sure you want to add the employee?\n");
    printf("\t%s %s, \tSalary: %d\n", newFirst, newLast, newSalary);
    printf("\nEnter 1 for yes, 0 for no: ");
    scanf("%d", &confirm);

    if (confirm != 1)
    {
        return 1;
    }

    if (strncmp(newFirst, "0", 1) != 0) {
        strcpy(db[employeeDbIndexToUpdate].firstName, newFirst);
        printf("Updated first name\n");
    } else {
        printf("Skipping update to first name\n");
    }

    if (strncmp(newLast, "0", 1) != 0) {
        strcpy(db[employeeDbIndexToUpdate].lastName, newLast);
        printf("Updated last name\n");
    } else {
        printf("Skipping update to last name\n");
    }
    
    if (newSalary != 0) {
        db[employeeDbIndexToUpdate].salary = newSalary;
        printf("Updated new salary\n");
    } else {
        printf("Skipping update to salary\n");
    }
    
    printf("employee update completed successfully\n");
    
    return 1;
}

void Swap(struct employee* xp, struct employee* yp)
{
    struct employee temp = *xp;
    *xp = *yp;
    *yp = temp;
}
  
//sort
void sort(struct employee emp[], int n)
{
    int i, j, min;
  
     
    for (i = 0; i < n - 1; i++) {
  
        
        min = i;
        for (j = i + 1; j < n; j++)
            if (emp[j].empID < emp[min].empID)
                min = j;
  
        
        Swap(&emp[min], &emp[i]);
    }
}

//sort using the salary
void sortSalary(struct employee emp[], int n)
{
    int i, j, min;
  
     
    for (i = 0; i < n - 1; i++) {
  
        
        min = i;
        for (j = i + 1; j < n; j++)
            if (emp[j].salary > emp[min].salary)
                min = j;
  
        
        Swap(&emp[min], &emp[i]);
    }
}

//remove an employee
void removeEmployee(int employeeToRemoveIndex) {
    int lastEmployeeIndex = total_employees - 1;
    Swap(&db[employeeToRemoveIndex], &db[lastEmployeeIndex]);
    total_employees = total_employees - 1;
    sort(db, total_employees);
}

//employees with highest salaries
void outputTopSalaries(int numberOfSalaries) {
    struct employee dbTemp[1000];
    memcpy(&dbTemp, &db, sizeof(dbTemp));
    sortSalary(dbTemp, total_employees);
    PrintDB(dbTemp, numberOfSalaries);
}

//find all employees with matching name
void findAllByLastName(struct employee emp[], int m, char* name) {
    struct employee dbTemp[1000];
    int found = 0;
    for (int i = 0; i < m; i++)
    {
        if (strcasecmp(emp[i].lastName, name) == 0)
        {
            strcpy(dbTemp[found].firstName, emp[i].firstName);
            strcpy(dbTemp[found].lastName, emp[i].lastName);
            dbTemp[found].salary = emp[i].salary;
            dbTemp[found].empID = emp[i].empID;
            found++;
        }
    }
    PrintDB(dbTemp, found);
}


int main(int argc, char *argv[])
{
    if ( argc != 2 )
    {
        printf( "usage: %s filename\n", argv[0] );
    }
    else
    {
        populateDB(argv[1]);
        sort(db, total_employees);

        int choice;
        int x = 0;
        
        while (1) {
            printf("\nEmployee DB Menu:\n");
            printf("----------------------------------\n");
            printf("\t(1) Print the Database\n");
            printf("\t(2) Lookup by ID\n");
            printf("\t(3) Lookup by Last Name\n");
            printf("\t(4) Add an Employee\n");
            printf("\t(5) Quit\n");
            printf("\t(6) Remove an Employee\n");
            printf("\t(7) Update an employee's information\n");
            printf("\t(8) Print the M employees with the highest salaries\n");
            printf("\t(9) Find all employees with matching last name\n");
            printf("----------------------------------\n");
            printf("Enter your choice: ");

            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    PrintDB(db, total_employees);
                    break;
                case 2: ;
                    int id;
                    printf("\nPlease enter an employee ID: ");
                    scanf("%d", & id);
                    x = searchID(db, total_employees, id);
                    if (x == -1)
                    {
                        printf("\nUser with employee ID '%d' not found\n",id);
                    }
                    else
                    {
                        printf("\nName\t\t\t\tSALARY\t   ID\n");
                        printf("----------------------------------------------\n");
                        printf("%-15s\t%-15s\t%d\t%d\n", db[x].firstName,db[x].lastName,db[x].salary, db[x].empID);
                        printf("----------------------------------------------\n");
                    }
                    
                    break;
                case 3: ;
                    char target_name[MAXNAME];
                    printf("\nPlease enter an employee's last name: ");
                    scanf("%s", target_name);
                    x = searchLName(db, total_employees, target_name);
                    if (x == -1)
                    {
                        printf("\nUser with last name '%s' not found\n", target_name);
                    }
                    else
                    {
                        printf("\nName\t\t\t\tSALARY\t   ID\n");
                        printf("----------------------------------------------\n");
                        printf("%-15s\t%-15s\t%d\t%d\n", db[x].firstName,db[x].lastName,db[x].salary, db[x].empID);
                        printf("----------------------------------------------\n");
                    }
                    break;
                case 4:
                    addEmp(db);
                    break;
                case 5:
                    printf("\n Goodbye! \n");
                    return 0;
                    break;
                case 6:; // remove an employee
                    int employeeIdToDelete;
                    printf("\nPlease enter an employee ID: ");
                    scanf("%d", &employeeIdToDelete);
                    int employeeDbIndexToDelete = searchID(db, total_employees, employeeIdToDelete);
                    if (employeeDbIndexToDelete == -1)
                    {
                        printf("\nUser with employee ID '%d' not found\n", employeeIdToDelete);
                    }
                    else
                    {
                        removeEmployee(employeeDbIndexToDelete);
                    }
                    break;
                case 7:; // Update an employee's information
                    int employeeIdToUpdate;
                    printf("\nPlease enter an employee ID: ");
                    scanf("%d", &employeeIdToUpdate);
                    int employeeDbIndexToUpdate = searchID(db, total_employees, employeeIdToUpdate);
                    updateEmployee(employeeDbIndexToUpdate);
                    break;
                case 8:; // print the M employees with the highest salaries
                    int numberOfSalaries;
                    printf("\nTop number of salaries to print?: ");
                    scanf("%d", &numberOfSalaries);
                    outputTopSalaries(numberOfSalaries);
                    break;
                case 9:; // find all employees with matching last name
                    char lastNameToSearch[MAXNAME]; //target name to search
                    printf("\nPlease enter an employee's last name: ");
                    scanf("%s", lastNameToSearch);
                    findAllByLastName(db, total_employees, lastNameToSearch);
                    break;
                default:
                    printf( "\n Invalid option. Please try again.\n" );
                    break;
            }
        }
    }
}
