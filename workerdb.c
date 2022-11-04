

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
                default:            
                    printf( "\n Invalid option. Please try again.\n" );
                    break;
            }
        }
    }
}
