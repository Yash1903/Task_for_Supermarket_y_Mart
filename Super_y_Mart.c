/******************************************************************************

Welcome to Y Mart
done by YASH +91 94298 80326
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 100
#define DESC_LEN 100
#define MAX_CUSTOMERS 100
#define MAX_PURCHASES 50

typedef struct {
    int hsn_code;
    char description[DESC_LEN];
    int quantity;
    float mrp;
} Item;

typedef struct {
    char name[DESC_LEN];
    Item purchases[MAX_PURCHASES];
    int purchase_count;
    float total_bill;
} Customer;

Item warehouse[MAX_ITEMS];
Customer customers[MAX_CUSTOMERS];
int item_count = 0;
int customer_count = 0;

void loadWarehouse() {
    FILE *file = fopen("warehouse.txt", "r");
    if (file == NULL) {
        return;
    }
    while (fscanf(file, "%d,%99[^,],%d,%f\n", &warehouse[item_count].hsn_code, warehouse[item_count].description,
                  &warehouse[item_count].quantity, &warehouse[item_count].mrp) != EOF) {
        item_count++;
    }
    fclose(file);
}

void saveWarehouse() {
    FILE *file = fopen("warehouse.txt", "w");
    for (int i = 0; i < item_count; i++) {
        fprintf(file, "%d,%s,%d,%f\n", warehouse[i].hsn_code, warehouse[i].description, warehouse[i].quantity, warehouse[i].mrp);
    }
    fclose(file);
}

void addItem() {
    char add_more;
    do {
        if (item_count >= MAX_ITEMS) {
            printf("Warehouse is full!\n");
            return;
        }

        int hsn_code;
        char description[DESC_LEN];
        int quantity;
        float mrp;

        printf("Enter HSN Code: ");
        scanf("%d", &hsn_code);

        // Check if item already exists
        for (int i = 0; i < item_count; i++) {
            if (warehouse[i].hsn_code == hsn_code) {
                printf("Item with HSN Code %d already exists!\n", hsn_code);
                return;
            }
        }

        printf("Enter Product Description: ");
        scanf(" %[^\n]", description);

        printf("Enter Quantity: ");
        scanf("%d", &quantity);

        printf("Enter MRP: ");
        scanf("%f", &mrp);

        warehouse[item_count].hsn_code = hsn_code;
        strcpy(warehouse[item_count].description, description);
        warehouse[item_count].quantity = quantity;
        warehouse[item_count].mrp = mrp;
        item_count++;

        printf("Item added successfully!\n");

        printf("Would you like to add more items? (y/n): ");
        scanf(" %c", &add_more);

    } while (add_more == 'y' || add_more == 'Y');
    saveWarehouse();
}

void updateItem() {
    int hsn_code;
    int quantity;
    float mrp;

    printf("Enter HSN Code of the item to update: ");
    scanf("%d", &hsn_code);

    int found = 0;
    for (int i = 0; i < item_count; i++) {
        if (warehouse[i].hsn_code == hsn_code) {
            printf("Enter new Quantity: ");
            scanf("%d", &quantity);

            printf("Enter new MRP: ");
            scanf("%f", &mrp);

            warehouse[i].quantity = quantity;
            warehouse[i].mrp = mrp;

            printf("Item updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Item with HSN Code %d not found!\n", hsn_code);
    }
    saveWarehouse();
}

void loadCustomers() {
    FILE *file = fopen("customers.txt", "r");
    if (file == NULL) {
        return;
    }
    while (fscanf(file, "%99[^,],%d,%f\n", customers[customer_count].name, &customers[customer_count].purchase_count, &customers[customer_count].total_bill) != EOF) {
        for (int i = 0; i < customers[customer_count].purchase_count; i++) {
            fscanf(file, "%d,%99[^,],%d,%f\n", &customers[customer_count].purchases[i].hsn_code, customers[customer_count].purchases[i].description,
                   &customers[customer_count].purchases[i].quantity, &customers[customer_count].purchases[i].mrp);
        }
        customer_count++;
    }
    fclose(file);
}

void saveCustomers() {
    FILE *file = fopen("customers.txt", "w");
    for (int i = 0; i < customer_count; i++) {
        fprintf(file, "%s,%d,%f\n", customers[i].name, customers[i].purchase_count, customers[i].total_bill);
        for (int j = 0; j < customers[i].purchase_count; j++) {
            fprintf(file, "%d,%s,%d,%f\n", customers[i].purchases[j].hsn_code, customers[i].purchases[j].description, customers[i].purchases[j].quantity, customers[i].purchases[j].mrp);
        }
    }
    fclose(file);
}

void generateBill() {
    char name[DESC_LEN];
    char add_more;
    printf("Enter Customer Name: ");
    scanf(" %[^\n]", name);

    Customer customer;
    strcpy(customer.name, name);
    customer.purchase_count = 0;
    customer.total_bill = 0.0;

    do {
        int hsn_code, quantity;
        printf("Enter HSN Code: ");
        scanf("%d", &hsn_code);

        int found = 0;
        for (int i = 0; i < item_count; i++) {
            if (warehouse[i].hsn_code == hsn_code) {
                found = 1;
                printf("Product Description: %s\n", warehouse[i].description);
                printf("Enter Quantity: ");
                scanf("%d", &quantity);

                if (quantity > warehouse[i].quantity) {
                    printf("Not enough stock. Available quantity: %d\n", warehouse[i].quantity);
                } else {
                    Item purchase = warehouse[i];
                    purchase.quantity = quantity;
                    customer.purchases[customer.purchase_count++] = purchase;
                    customer.total_bill += purchase.mrp * quantity;
                    warehouse[i].quantity -= quantity;
                    printf("Item added to bill.\n");
                }
                break;
            }
        }

        if (!found) {
            printf("Item with Hsn code %d not found!\n", hsn_code);
        }

        printf("Would you like to add more items? (y/n): ");
        scanf(" %c", &add_more);
    } while (add_more == 'y' || add_more == 'Y');

    customers[customer_count++] = customer;
    saveCustomers();
    saveWarehouse();

    printf("\nBill for %s:\n", customer.name);
    printf("HSN Code\tDescription\t\tQuantity\tMRP\tTotal\n");
    for (int i = 0; i < customer.purchase_count; i++) {
        printf("%d\t\t%s\t\t%d\t\t%.2f\t%.2f\n", customer.purchases[i].hsn_code, customer.purchases[i].description,
               customer.purchases[i].quantity, customer.purchases[i].mrp, customer.purchases[i].mrp * customer.purchases[i].quantity);
    }
    printf("Total Bill: %.2f\n", customer.total_bill);
}

void generateReport() {
    printf("\nSales Report:\n");
    for (int i = 0; i < customer_count; i++) {
        printf("Customer: %s\n", customers[i].name);
        printf("HSN Code\tDescription\t\tQuantity\tMRP\tTotal\n");
        for (int j = 0; j < customers[i].purchase_count; j++) {
            printf("%d\t\t%s\t\t%d\t\t%.2f\t%.2f\n", customers[i].purchases[j].hsn_code, customers[i].purchases[j].description,
                   customers[i].purchases[j].quantity, customers[i].purchases[j].mrp, customers[i].purchases[j].mrp * customers[i].purchases[j].quantity);
        }
        printf("Total Bill: %.2f\n\n", customers[i].total_bill);
    }

    printf("\nRemaining Items in Warehouse:\n");
    printf("HSN Code\tDescription\t\tQuantity\tMRP\n");
    for (int i = 0; i < item_count; i++) {
        printf("%d\t\t%s\t\t%d\t\t%.2f\n", warehouse[i].hsn_code, warehouse[i].description, warehouse[i].quantity, warehouse[i].mrp);
    }
}

int main() {
    loadWarehouse();
    loadCustomers();

    int choice;

    while (1) {
        printf("\nSupermarket Desk\n");
        printf("1. Add New Item\n");
        printf("2. Update Item\n");
        printf("3. Generate Bill\n");
        printf("4. Generate Report\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addItem();
                break;
            case 2:
                updateItem();
                break;
            case 3:
                generateBill();
                break;
            case 4:
                generateReport();
                break;
            case 5:
                saveWarehouse();
                saveCustomers();
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

