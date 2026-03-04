#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define Structures
typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char name[100];
    char phone[100];
    char email[100];
    char address[200];
} Contact;

// Check Birthday Function
void checkbday(Date date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int current_day = tm.tm_mday;
    int current_month = tm.tm_mon + 1;
    if (date.day == current_day && date.month == current_month) {
        printf("Happy Birthday!\n");
    }
}

// Adding Additional Information
void addtionalinformation() {
    long int EN;
    FILE *info;
    info = fopen("ainfo.csv", "a");
    if (info == NULL) {
        printf("Error opening the file\n");
        return;
    }

    printf("Enter EMERGENCY number: ");
    scanf("%li", &EN);
    fprintf(info, "%li\n", EN);
    fclose(info);
    printf("Contact added successfully!\n");
}

// Add Contact
void add_contacts() {
    char name[100], num[100], mail[100], address[200];
    int additionalinfo;
    FILE *cmsmain;

    cmsmain = fopen("CMS.csv", "a");
    if (cmsmain == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("USE __FOR SPACES\n");
    printf("Enter Name: ");
    scanf("%s", name);
    printf("Enter Phone Number: ");
    scanf("%s", num);
    printf("Enter Email: ");
    scanf("%s", mail);
    printf("Enter Address: ");
    scanf("%s", address);

    printf("If you want to add additional information press 1, else press 0!\n");
    scanf("%d", &additionalinfo);
    if (additionalinfo == 1) {
        addtionalinformation();
    } else {
        printf("Thank you!\n");
    }

    fprintf(cmsmain, "%s,%s,%s,%s\n", name, num, mail, address);
    fclose(cmsmain);
    printf("Contact added successfully!\n");
}

// Read Contacts
void read_contacts() {
    FILE *cmsmain;
    char name[100], num[100], mail[100], address[200];
    char search_name[100];
    char line[500];
    int search_option;

    cmsmain = fopen("CMS.csv", "r");
    if (cmsmain == NULL) {
        printf("Error opening file! Ensure the file exists and has data.\n");
        return;
    }

    printf("\nDo you want to view all contacts or search for a specific contact?\n");
    printf("1. View All Contacts\n");
    printf("2. Search Specific Contact\n");
    printf("Enter your choice: ");
    scanf("%d", &search_option);

    if (search_option == 1) {
        // Display all contacts
        printf("\n--- Contact List ---\n");
        printf("Name                Phone Number        Email                    Address\n");
        printf("---------------------------------------------------------------------------\n");

        while (fgets(line, sizeof(line), cmsmain) != NULL) {
            if (sscanf(line, "%99[^,],%99[^,],%99[^,],%199[^\n]", name, num, mail, address) == 4) {
                printf("%-20s%-20s%-25s%-30s\n", name, num, mail, address);
            }
        }
    } else if (search_option == 2) {
        // Search specific contact
        printf("Enter the name of the contact to search: ");
        scanf("%s", search_name);

        int found = 0;
        while (fgets(line, sizeof(line), cmsmain) != NULL) {
            if (sscanf(line, "%99[^,],%99[^,],%99[^,],%199[^\n]", name, num, mail, address) == 4) {
                if (strcmp(name, search_name) == 0) {
                    printf("\nContact Found:\n");
                    printf("Name: %s\nPhone Number: %s\nEmail: %s\nAddress: %s\n", name, num, mail, address);
                    found = 1;
                    break;
                }
            }
        }

        if (!found) {
            printf("Contact with name '%s' not found.\n", search_name);
        }
    } else {
        printf("Invalid option. Returning to main menu.\n");
    }

    fclose(cmsmain);
}

// Edit Contacts
void edit_contacts() {
    char search_name[100], line[500];
    int found = 0;

    FILE *cmsmain = fopen("CMS.csv", "r");
    if (cmsmain == NULL) {
        printf("Error: Could not open file!\n");g
        return;
    }

    FILE *tempfile = fopen("temp.csv", "w");
    if (tempfile == NULL) {
        printf("Error: Could not open temporary file!\n");
        fclose(cmsmain);
        return;
    }

    printf("Enter the name of the contact to edit: ");
    scanf("%s", search_name);

    while (fgets(line, sizeof(line), cmsmain) != NULL) {
        char name[100], num[100], mail[100], address[200];
        if (sscanf(line, "%99[^,],%99[^,],%99[^,],%199[^\n]", name, num, mail, address) == 4) {
            if (strcmp(name, search_name) == 0) {
                found = 1;
                printf("Contact found! Enter new details:\n");
                printf("New Name: ");
                scanf("%s", name);
                printf("New Phone Number: ");
                scanf("%s", num);
                printf("New Email: ");
                scanf("%s", mail);
                printf("New Address: ");
                scanf("%s", address);
            }
            fprintf(tempfile, "%s,%s,%s,%s\n", name, num, mail, address);
        }
    }

    fclose(cmsmain);
    fclose(tempfile);

    if (found) {
        remove("CMS.csv");
        rename("temp.csv", "CMS.csv");
        printf("Contact updated successfully!\n");
    } else {
        remove("temp.csv");
        printf("No contact found with the name \"%s\".\n", search_name);
    }
}

// Delete Contacts
void delete_contacts() {
    char search_name[100], line[500];
    int found = 0;

    FILE *cmsmain = fopen("CMS.csv", "r");
    if (cmsmain == NULL) {
        printf("Error: Could not open file!\n");
        return;
    }

    FILE *tempfile = fopen("temp.csv", "w");
    if (tempfile == NULL) {
        printf("Error: Could not open temporary file!\n");
        fclose(cmsmain);
        return;
    }

    printf("Enter the name of the contact to delete: ");
    scanf("%s", search_name);

    while (fgets(line, sizeof(line), cmsmain) != NULL) {
        char name[100], num[100], mail[100], address[200];
        if (sscanf(line, "%99[^,],%99[^,],%99[^,],%199[^\n]", name, num, mail, address) == 4) {
            if (strcmp(name, search_name) != 0) {
                fprintf(tempfile, "%s,%s,%s,%s\n", name, num, mail, address);
            } else {
                found = 1;
            }
        }
    }

    fclose(cmsmain);
    fclose(tempfile);

    if (found) {
        remove("CMS.csv");
        rename("temp.csv", "CMS.csv");
        printf("Contact deleted successfully!\n");
    } else {
        remove("temp.csv");
        printf("No contact found with the name \"%s\".\n", search_name);
    }
}

// Sort Contacts
void sort_contacts() {
    FILE *cmsmain;
    Contact contacts[100];
    int count = 0;

    cmsmain = fopen("CMS.csv", "r");
    if (cmsmain == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(cmsmain, "%99[^,],%99[^,],%99[^,],%199[^\n]", contacts[count].name, contacts[count].phone, contacts[count].email, contacts[count].address) != EOF) {
        count++;
    }
    fclose(cmsmain);

    Contact temp;
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(contacts[i].name, contacts[j].name) > 0) {
                temp = contacts[i];
                contacts[i] = contacts[j];
                contacts[j] = temp;
            }
        }
    }

    cmsmain = fopen("CMS.csv", "w");
    if (cmsmain == NULL) {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(cmsmain, "%s,%s,%s,%s\n", contacts[i].name, contacts[i].phone, contacts[i].email, contacts[i].address);
    }

    fclose(cmsmain);
    printf("Contacts sorted successfully!\n");
}

// Main Menu
void home() {
    int choice;

    do {
        printf("\n--- Contact Management System ---\n");
        printf("1. Add Contact\n");
        printf("2. View Contacts\n");
        printf("3. Edit Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Sort Contacts\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            add_contacts();
            break;
        case 2:
            read_contacts();
            break;
        case 3:
            edit_contacts();
            break;
        case 4:
            delete_contacts();
            break;
        case 5:
            sort_contacts();
            break;
        case 6:
            printf("Exiting... Goodbye!\n");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 6);
}

// Main Function
int main() {
    printf("WELCOME TO CONTACT MANAGEMENT SYSTEM \nWe help you to do various tasks in your contacts!");
    home();
    return 0;
}
