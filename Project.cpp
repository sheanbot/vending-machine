#include <iostream>
#include <iomanip> // For setprecision
#include <limits>  // For numeric_limits
#include <fstream> // For file handling
#include <ctime> // For date and time 
using namespace std;

// Define a struct for drinks
struct Drink {
    string code; 
    string name; 
    double price; 
    int quantity;
};

// Function: Display coffee menu
void display_coffee_menu(const Drink coffee[], int size) {
    cout << "Drinks Menu:" << endl;
    for (int i = 0; i < size; ++i) {
        cout << coffee[i].code << " = " << coffee[i].name << " (RM" << fixed << setprecision(2) << coffee[i].price << ")" << endl;
    }
}

// Function: Get more items
bool add_on_items() {
    char choice;

    while (true) {
        cout << "Anything else you would like to add on? (Y/N): ";
        cin >> choice;

        if (choice == 'Y' || choice == 'y') {
            return true;
        } else if (choice == 'N' || choice == 'n') {
            return false;
        } else {
            cout << "Invalid input, please enter (Y/N) to add on items or not..." << endl;
            cin.clear(); //Clear the error state flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
        }
    }
}

// Function: Get quantity
int get_quantity() {
    int quantity;

    do {
        cout << "Enter quantity: ";
        cin >> quantity;

        if (quantity <= 0) {
            cout << "Invalid quantity, please a valid positive quantity..." << endl;
            cin.clear(); //Clear the error state flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
        }
    } while (quantity <= 0);
    return quantity;
}

// Function: Print receipt
void print_receipt(const Drink coffee[], int size, double totalAmount, double payment, double change) {
    ofstream receiptFile("receipt.txt"); // Open receipt from txt file

    // Set time-zone to Malaysia
    putenv("TZ=Asia/Kuala_Lumpur");

    // Get current date and time
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);
    char dateTime[100];
    strftime(dateTime, sizeof(dateTime), "%c", localTime);

    receiptFile << "Receipt: " << dateTime << endl; // Include date and time
    receiptFile << "----------------------------------------" << endl;
    receiptFile << "Drinks Ordered:" << endl;

    for (int i = 0; i < size; ++i) {
        if (coffee[i].quantity > 0) {
            receiptFile << coffee[i].name << " x" << coffee[i].quantity << " = RM" << fixed << setprecision(2) << (coffee[i].price * coffee[i].quantity) << endl;
        }
    }

    receiptFile << "\nTotal Amount: RM" << fixed << setprecision(2) << totalAmount << endl;
    receiptFile << "Payment: RM" << fixed << setprecision(2) << payment << endl;
    receiptFile << "Change: RM" << fixed << setprecision(2) << change << endl;

    cout << "Thank you for your purchase! Here is your change: RM" << fixed << setprecision(2) << change << endl;
    cout << "Do enjoy your coffee, have a nice day!" << endl;

    receiptFile.close(); // Close the file
}

// Function: Calculate payment
void handle_payment(double totalAmount, const Drink coffee[], int size) {
    double payment, imbalance = totalAmount;

    while (imbalance > 0) {

        for (int i = 0; i < size; ++i) {
        if (coffee[i].quantity > 0) {
            cout << coffee[i].name << " x" << coffee[i].quantity << " = RM" << fixed << setprecision(2) << (coffee[i].price * coffee[i].quantity) << endl;
        }
    }

        cout << "Enter payment amount RM: ";
        cin >> payment;

        if (cin.fail() || payment <= 0) {
            cout << "Invalid payment, please enter a valid positive amount." << endl;
            cin.clear(); // Clear the error state flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
        } else {
            imbalance -= payment;
            if (imbalance > 0) {
                cout << "Insufficient payment! You still have to pay RM" << fixed << setprecision(2) << imbalance << " in order to purchase your coffee." << endl;
            }
        }
    }

    // Calculate change
    double change = -imbalance;

    // Generate receipt from 'Function: print receipt'
    print_receipt(coffee, size, totalAmount, totalAmount - imbalance, change);
}

// Function: Find a drink by code
void input_drink_code(Drink coffee[], int size) {
    double totalAmount = 0.0;

    do {
        string userCode;
        bool validCode = false;

        do {
            cout << "Enter your coffee by following the menu code:" << endl;
            cin >> userCode;

            for (int i = 0; i < size; ++i) {
                if (coffee[i].code == userCode) {
                    cout << "You selected " << coffee[i].name << " (RM" << fixed << setprecision(2) << coffee[i].price << ")" << endl;
                    validCode = true;                 
                    // Ask quantity using 'Function: Get quantity'
                    int quantity = get_quantity();
                    // Calculate total amount
                    totalAmount += coffee[i].price * quantity; 
                    // Update quantity in the coffee array
                    coffee[i].quantity += quantity; 
                }
            }
            if (!validCode) {
                cout << "Invalid code, please enter the code by following the menu..." << endl;
                cin.clear(); // Clear the error state flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            }
        } while (!validCode);
        // Ask add more items using 'Function: Get more items'
    } while (add_on_items());

    cout << "Total amount is RM: " << fixed << setprecision(2) << totalAmount << endl;

    handle_payment(totalAmount, coffee, size);
}

int main() {
    Drink coffee[] = {
        {"H1", "Hot-Light Coffee", 5.0},
        {"H2", "Hot-Medium Coffee", 5.0},
        {"H3", "Hot-Dark Coffee", 5.0},
        {"H4", "Hot-Latte", 8.0},
        {"H5", "Hot-Mocha", 10.0},
        
        {"C1", "Ice-Light Coffee", 7.0},
        {"C2", "Ice-Medium Coffee", 7.0},
        {"C3", "Ice-Dark Coffee", 7.0},
        {"C4", "Ice-Latte", 10.0},
        {"C5", "Ice-Mocha", 12.0}
    };

    display_coffee_menu(coffee, sizeof(coffee) / sizeof(coffee[0]));
    input_drink_code(coffee, sizeof(coffee) / sizeof(coffee[0]));

    return 0;
}