#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUM_CUSTOMERS 5
#define NUM_RESOURCES 4

int available[NUM_RESOURCES];
int maximum[NUM_CUSTOMERS][NUM_RESOURCES];
int allocation[NUM_CUSTOMERS][NUM_RESOURCES] = {0};
int need[NUM_CUSTOMERS][NUM_RESOURCES];

// Function declarations
void readMaximumResources(const char *filename);
int requestResources(int customerNum, int request[]);
void releaseResources(int customerNum, int release[]);
bool isSafeState();
void displayData();

int main(int argc, char *argv[])
{
    if (argc != NUM_RESOURCES + 1)
    {
        printf("Usage: %s [resource1] [resource2] ... [resourceN]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        available[i - 1] = atoi(argv[i]);
    }

    readMaximumResources("max_request.txt"); // Replace with your file name

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        for (int j = 0; j < NUM_RESOURCES; j++)
        {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }

    // Main loop for handling user commands
    char command[3];
    int customerNum;
    int resources[NUM_RESOURCES];

    while (1)
    {
        printf("Enter command (RQ/RL/*): ");
        char input[100];
        fgets(input, sizeof(input), stdin);

        sscanf(input, "%s", command);

        if (strcmp(command, "RQ") == 0)
        {
            sscanf(input, "%*s %d %d %d %d %d", &customerNum, &resources[0], &resources[1], &resources[2], &resources[3]);
            int result = requestResources(customerNum, resources);
            if (result == 1)
            {
                printf("Request granted.\n");
            }
            else
            {
                printf("Request denied.\n");
            }
        }
        else if (strcmp(command, "RL") == 0)
        {
            sscanf(input, "%*s %d %d %d %d %d", &customerNum, &resources[0], &resources[1], &resources[2], &resources[3]);

            releaseResources(customerNum, resources);
        }
        else if (strcmp(command, "*") == 0)
        {
            displayData();
        }
        else
        {
            printf("Invalid command.\n");
        }
    }

    return 0;
}

// Read Maximum Resources from File
void readMaximumResources(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        for (int j = 0; j < NUM_RESOURCES; j++)
        {
            if (fscanf(file, "%d,", &maximum[i][j]) != 1)
            {
                fprintf(stderr, "Error reading max_request.txt\n");
                exit(1);
            }
        }
    }

    fclose(file);
}

int requestResources(int customerNum, int request[])
{
    if (customerNum < 0 || customerNum >= NUM_CUSTOMERS)
    {
        return -1; // Invalid customer number
    }

    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        if (request[i] > need[customerNum][i] || request[i] > available[i])
        {
            return 0; // Request cannot be granted
        }
    }

    // Tentatively allocate resources
    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        available[i] -= request[i];
        allocation[customerNum][i] += request[i];
        need[customerNum][i] -= request[i];
    }

    printf("isSafeState() = %d\n", isSafeState());

    if (isSafeState())
    {
        return 1; // Request granted
    }
    else
    {
        // Rollback
        for (int i = 0; i < NUM_RESOURCES; i++)
        {
            available[i] += request[i];
            allocation[customerNum][i] -= request[i];
            need[customerNum][i] += request[i];
        }
        return 0; // Request cannot be granted, state not safe
    }
}

void releaseResources(int customerNum, int release[])
{
    if (customerNum < 0 || customerNum >= NUM_CUSTOMERS)
    {
        printf("Invalid customer number.\n");
        return;
    }

    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        if (release[i] > allocation[customerNum][i])
        {
            printf("Release request exceeds allocation.\n");
            printf("Because release[%d] = %d > allocation[%d][%d] = %d\n", i, release[i], customerNum, i, allocation[customerNum][i]);
            return;
        }
        allocation[customerNum][i] -= release[i];
        available[i] += release[i];
        need[customerNum][i] += release[i];
    }
    printf("Resources released.\n");
}

bool isSafeState()
{
    int work[NUM_RESOURCES];
    bool finish[NUM_CUSTOMERS] = {false};

    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        work[i] = available[i];
    }

    while (true)
    {
        bool found = false;
        for (int i = 0; i < NUM_CUSTOMERS; i++)
        {
            if (!finish[i])
            {
                int j;
                for (j = 0; j < NUM_RESOURCES; j++)
                {
                    if (need[i][j] > work[j])
                    {
                        break;
                    }
                }
                if (j == NUM_RESOURCES)
                {
                    for (int k = 0; k < NUM_RESOURCES; k++)
                    {
                        work[k] += allocation[i][k];
                    }
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found)
        {
            break;
        }
    }

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        if (!finish[i])
        {
            return false;
        }
    }
    return true;
}

void displayData()
{
    printf("Available Resources:\n");
    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        printf("%d ", available[i]);
    }
    printf("\nMaximum Resources:\n");
    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        for (int j = 0; j < NUM_RESOURCES; j++)
        {
           
            printf("%d ", maximum[i][j]);
        }
        printf("\n");
    }

    printf("Allocation:\n");
    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        for (int j = 0; j < NUM_RESOURCES; j++)
        {
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }

    printf("Need:\n");
    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        for (int j = 0; j < NUM_RESOURCES; j++)
        {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}
