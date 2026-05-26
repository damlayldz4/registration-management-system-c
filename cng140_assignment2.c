// Damla Yıldız
// 2649746
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 50
#define MAC_LEN 20

// Define a structure to store registration data
struct Registration {
    int regID;
    char name[MAX_LEN];
    char surname[MAX_LEN];
    char status[MAX_LEN];
    int year;
    char deviceMACAddress[MAC_LEN];
};

typedef struct Registration Registration;

// Function declarations
Registration* Load_RegistrationData(char*, int*);
void Display_RegistrationData(Registration*, int);
Registration* Add_Registration(Registration*, int*);
void Sort(Registration*, int);
void Statistics(Registration*, int);
void Menu(void);
int isValidMAC(const char*);
int isValidStatus(const char*);

int main(int argc, char *argv[]) {
    char filename[100];
    int recordSize = 0;
    FILE *file;

    // Ensure a filename is provided
    if (argc < 2) {
        printf("Please provide the file name as command-line argument.\n");
        return 1;
    }

    // Try to open the file
    strcpy(filename, argv[1]);
    file = fopen(filename, "r");

    // Keep prompting until a valid file is entered
    while (file == NULL) {
        printf("This file does not exist. Please enter again: ");
        scanf("%s", filename);
        file = fopen(filename, "r");
    }

    fclose(file);

    // Load the data from file
    Registration *data = Load_RegistrationData(filename, &recordSize);
    printf("The registration records file (%s) has been successfully loaded!\n", filename);
    printf("The following records have been loaded:\n\n");
    Display_RegistrationData(data, recordSize);

    // Main menu loop
    int choice;
    do {
        Menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                data = Add_Registration(data, &recordSize);
                break;
            case 2:
                Sort(data, recordSize);
                Display_RegistrationData(data, recordSize);
                break;
            case 3:
                Statistics(data, recordSize);
                break;
            case 4:
                printf("Bye\n");
                break;
            default:
                printf("Wrong input!\n");
        }
    } while (choice != 4);

    // Free dynamically allocated memory
    free(data);
    return 0;
}

// Loads registration data from the given file
Registration* Load_RegistrationData(char* fileName, int* recordSize) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Could not open the file.\n");
        exit(1);
    }

    // Count number of lines in file
    int count = 0;
    char ch = fgetc(file);
    while (ch != EOF) {
        if (ch == '\n') {
            count++;
        }
        ch = fgetc(file);
    }

    *recordSize = count; // includes all data lines
    rewind(file);

    char firstLine[200];
    fgets(firstLine, sizeof(firstLine), file); // skip the header line

    // Allocate memory for data
    Registration *data = (Registration*)malloc(sizeof(Registration) * (*recordSize));
    if (data == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    // Read each record into the array
    for (int i = 0; i < *recordSize; i++) {
        fscanf(file, "%d %s %s %s %d %s",
               &data[i].regID,
               data[i].name,
               data[i].surname,
               data[i].status,
               &data[i].year,
               data[i].deviceMACAddress);
    }

    fclose(file);
    return data;
}

// Displays registration data in a formatted table
void Display_RegistrationData(Registration* data, int size) {
    if (data == NULL || size == 0) {
        printf("No registration data available.\n");
        return;
    }

    printf("REGID  NAME     SURNAME   STATUS    YEAR  DEVICEMACADDRESS\n");
    for (int i = 0; i < size; i++) {
        printf("%-6d %-8s %-9s %-9s %-5d %s\n",
               data[i].regID,
               data[i].name,
               data[i].surname,
               data[i].status,
               data[i].year,
               data[i].deviceMACAddress);
    }
}

// Validates if the given status is allowed
int isValidStatus(const char* status) {
    return strcmp(status, "Approved") == 0 ||
           strcmp(status, "Declined") == 0 ||
           strcmp(status, "Blocked") == 0;
}

// Checks if the given MAC address follows the correct XX:XX:XX:XX:XX:XX format
int isValidMAC(const char* mac) {
    if (strlen(mac) != 17) return 0;

    for (int i = 0; i < 17; i++) {
        if ((i + 1) % 3 == 0) {
            if (mac[i] != ':') return 0;
        }
    }

    return 1;
}

// Adds a new registration to the data array
Registration* Add_Registration(Registration* data, int* recordSize) {
    char name[MAX_LEN], surname[MAX_LEN], status[MAX_LEN], mac[MAC_LEN];
    int year;

    // Collect user input
    printf("Please enter the name: ");
    scanf("%s", name);

    printf("Please enter the surname: ");
    scanf("%s", surname);

    // Validate status
    do {
        printf("Please enter the status: ");
        scanf("%s", status);
        if (!isValidStatus(status))
            printf("This is not valid!\n");
    } while (!isValidStatus(status));

    printf("Please enter the year: ");
    scanf("%d", &year);

    // Validate MAC address format
    do {
        printf("Please enter the mac address: ");
        scanf("%s", mac);
        if (!isValidMAC(mac))
            printf("This is not valid!\n");
    } while (!isValidMAC(mac));

    // Reallocate memory to add new record
    data = realloc(data, (*recordSize + 1) * sizeof(Registration));
    Registration *newReg = &data[*recordSize];
    newReg->regID = *recordSize == 0 ? 1 : data[*recordSize - 1].regID + 1;
    strcpy(newReg->name, name);
    strcpy(newReg->surname, surname);
    strcpy(newReg->status, status);
    newReg->year = year;
    strcpy(newReg->deviceMACAddress, mac);

    (*recordSize)++;
    printf("It is recorded successfully!\n");
    return data;
}

// Sorts registration data by year or surname+name
void Sort(Registration* data, int size) {
    int choice;
    do {
        printf("Which column (1: year, 2: surname)?:");
        scanf("%d", &choice);

        if (choice == 1) {
            // Bubble sort by year
            for (int i = 0; i < size - 1; i++) {
                for (int j = 0; j < size - i - 1; j++) {
                    if (data[j].year > data[j + 1].year) {
                        Registration temp = data[j];
                        data[j] = data[j + 1];
                        data[j + 1] = temp;
                    }
                }
            }
        } else if (choice == 2) {
            // Bubble sort by surname, then name
            for (int i = 0; i < size - 1; i++) {
                for (int j = 0; j < size - i - 1; j++) {
                    int cmpSurname = strcmp(data[j].surname, data[j + 1].surname);
                    int cmpName = strcmp(data[j].name, data[j + 1].name);
                    if (cmpSurname > 0 || (cmpSurname == 0 && cmpName > 0)) {
                        Registration temp = data[j];
                        data[j] = data[j + 1];
                        data[j + 1] = temp;
                    }
                }
            }
        } else {
            printf("Invalid choice!\n");
        }
    } while (choice != 1 && choice != 2);
}

// Displays statistics based on registration status
void Statistics(Registration* data, int size) {
    int approved = 0, blocked = 0, declined = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp(data[i].status, "Approved") == 0) approved++;
        else if (strcmp(data[i].status, "Blocked") == 0) blocked++;
        else if (strcmp(data[i].status, "Declined") == 0) declined++;
    }

    printf("Summary:\n");
    printf("%d Blocked\n", blocked);
    printf("%d Approved\n", approved);
    printf("%d Declined\n", declined);
}

// Displays the main menu options
void Menu(void) {
    printf("\nWhat would you like to do?\n");
    printf("1 - add\n");
    printf("2 - sort\n");
    printf("3 - statistics\n");
    printf("4 - exit\n");
}
