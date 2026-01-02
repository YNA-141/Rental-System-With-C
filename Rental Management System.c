#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TENANTS 100
#define FILE_NAME "tenants.txt"

#define OWNER_USER "owner"
#define OWNER_PASS "12345"

typedef struct
{
    char username[40];
    char password[40];
    char name[100];
    char nid[32];
    char phone[32];
    char flat[16];
    double monthlyRent;
    int dueDay;
} Tenant;

typedef struct
{
    Tenant tenants[MAX_TENANTS];
    int count;
} TenantDB;

TenantDB db;


void loadData()
{
    FILE *fp= fopen(FILE_NAME, "r");
    if (fp == NULL)
    {
        db.count = 0;
        return;
    }
    db.count = 0;

    while (fscanf(fp, "%s %s %s %s %s %s %lf %d", db.tenants[db.count].username, db.tenants[db.count].password, db.tenants[db.count].name, db.tenants[db.count].nid, db.tenants[db.count].phone,
                  db.tenants[db.count].flat, &db.tenants[db.count].monthlyRent, &db.tenants[db.count].dueDay) != EOF)
    {
        db.count++;
    }
    fclose(fp);
}

void saveData() {
FILE *fp = fopen(FILE_NAME, "w");
if (fp == NULL) return;

    for (int i = 0; i < db.count; i++) {
        fprintf(fp, "%s %s %s %s %s %s %.2lf %d\n",
                db.tenants[i].username,
                db.tenants[i].password,
                db.tenants[i].name,
                db.tenants[i].nid,
                db.tenants[i].phone,
                db.tenants[i].flat,
                db.tenants[i].monthlyRent,
                db.tenants[i].dueDay);
    }
    fclose(fp);
}

void addTenant()
{
    if (db.count >= MAX_TENANTS)
    {
        printf("Database full.\n");
        return;
    }

    Tenant t;
    printf("\nAdd New Tenant\n");
    printf("Username: "); scanf("%s", t.username);
    printf("Password: "); scanf("%s", t.password);
    printf("Name: ");scanf("%s", t.name);
    printf("NID: ");scanf("%s", t.nid);
    printf("Phone: ");scanf("%s", t.phone);
    printf("Flat No: ");scanf("%s", t.flat);
    printf("Monthly Rent: "); scanf("%lf", &t.monthlyRent);
    printf("Rent Due Day (1-31): "); scanf("%d", &t.dueDay);

    db.tenants[db.count] = t;
    db.count++;
    saveData();
    printf("Tenant added successfully.\n");
}

void viewAllTenants()
{
    printf("\nAll Tenants\n");
    printf("%-5s %-20s %-10s %-10s\n", "ID", "Name", "Flat", "Rent");
    for(int i=0; i<db.count; i++)
    {
        printf("%-5d %-20s %-10s %.2f\n", i, db.tenants[i].name, db.tenants[i].flat, db.tenants[i].monthlyRent);
    }
}

void editTenant(int index)
{
    printf("\nEditing %s\n", db.tenants[index].name);
    printf("(Note: Use underscores for spaces)\n");

    printf("Enter new Name: ");
    scanf("%s", db.tenants[index].name);

    printf("Enter new Phone: ");
    scanf("%s", db.tenants[index].phone);

    printf("Enter new Rent: ");
    scanf("%lf", &db.tenants[index].monthlyRent);

    saveData();
    printf("Record updated.\n");
}

void deleteTenant(int index)
{

    for(int i = index; i < db.count - 1; i++)
    {
        db.tenants[i] = db.tenants[i+1];
    }
    db.count--;
    saveData();
    printf("Record deleted.\n");
}

void searchTenant(int mode)
{
    char key[100];
    int foundCount = 0;

    printf("Enter Name to search: ");
    scanf("%s", key);

    printf("\nSearch Results\n");
    for(int i=0; i<db.count; i++)
        {
        if(strstr(db.tenants[i].name, key) != NULL)
        {
            printf("Name: %s | Flat: %s\n", db.tenants[i].name, db.tenants[i].flat);
            foundCount++;
        }
        }

    if(foundCount == 0)
    {
        printf("No records found.\n");
        return;
    }


    if(mode == 2 || mode == 3)
    {
        char targetName[100];

        if (mode == 2) printf("\nEnter Exact Name to Edit: ");
        else printf("\nEnter Exact Name to Delete: ");

        scanf("%s", targetName);

        int foundIndex = -1;
        for(int i=0; i<db.count; i++)
        {

            if(strstr(db.tenants[i].name, targetName) != NULL)
            {
                foundIndex = i;
                break;
            }
        }

        if(foundIndex != -1)
        {
            if(mode == 2) editTenant(foundIndex);
            else deleteTenant(foundIndex);
        }
        else
        {
            printf("Tenant name not found.\n");
        }
    }
}

void generateInvoice(int index)
{
    double rent = db.tenants[index].monthlyRent;
    double discount = 0.0;

    if(db.tenants[index].dueDay <= 5)
    {
        discount = rent * 0.05;
    }

    printf("\nINVOICE\n");
    printf("Base Rent: %.2f\n", rent);
    printf("Discount: -%.2f\n", discount);
    printf("Total:     %.2f\n", rent - discount);
}

void tenantMenu(int index)
{
    int choice;
    do {
        printf("\nTENANT MENU (%s)\n", db.tenants[index].name);
        printf("1. View Profile\n");
        printf("2. Generate Invoice\n");
        printf("0. Logout\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                printf("\nName: %s\nFlat: %s\nRent: %.2f\nDue Day: %d\n",
                       db.tenants[index].name, db.tenants[index].flat,
                       db.tenants[index].monthlyRent, db.tenants[index].dueDay);
                break;
            case 2:
                generateInvoice(index);
                break;
            case 0: break;
        }
    } while(choice != 0);
}

void ownerMenu()
{
    int choice;
    do {
        printf("\nOWNER DASHBOARD\n");
        printf("1. Add Tenant\n");
        printf("2. View All Tenants\n");
        printf("3. Search Tenant\n");
        printf("4. Edit Tenant\n");
        printf("5. Delete Tenant\n");
        printf("0. Logout\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1: addTenant(); break;
            case 2: viewAllTenants(); break;
            case 3: searchTenant(1); break;
            case 4: searchTenant(2); break;
            case 5: searchTenant(3); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while(choice != 0);
}


int main()
{
    loadData();
    int choice;
    char u[50], p[50];

    while(1) {
        printf("\nRental Management System\n");
        printf("1. Owner Login\n");
        printf("2. Tenant Login\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            printf("Owner Username: "); scanf("%s", u);
            printf("Owner Password: "); scanf("%s", p);


            if(strcmp(u, OWNER_USER)==0 && strcmp(p, OWNER_PASS)==0)
            {
                ownerMenu();
            }
            else
            {
                printf("Invalid Owner Credentials!\n");
            }
        }
        else if (choice == 2)
      {

            printf("Tenant Username: "); scanf("%s", u);
            printf("Tenant Password: "); scanf("%s", p);
            int found = -1;

            for(int i=0; i<db.count; i++)
                {
                if(strcmp(u, db.tenants[i].username)==0 && strcmp(p, db.tenants[i].password)==0)
               {
                    found = i;
                    break;
               }
                }
            if(found != -1) tenantMenu(found);
            else printf("Invalid Tenant Credentials!\n");
        }
        else if (choice == 0)
       {
            exit(0);
       }
    }
    return 0;
}

