// ============================================
// Name: Hina Shehzad
// Programming
// Project: Payroll System
//
// Description:
// This program processes driver payroll data using file input.
// It reads driver details (ID, name, and rate per mile) from a file
// and calculates monthly pay based on miles driven from multiple
// monthly files (Jan, Feb, Mar 2026).
//
// The system includes:
// - File handling for reading driver and payroll data
// - Use of vector data structure to store driver records
// - Functions for processing payroll and searching drivers
// - Case-insensitive ID matching
// - Tax calculation based on annual income rules
// - Output of results to console and output files
//
// The program also allows users to:
// - View payroll details for all drivers
// - Search for a specific driver and calculate total pay across months
// ============================================


#include <iostream>// Allows input and output operations (e.g., cout, cin)
#include <vector>  // Provides the vector container to store multiple Driver objects
#include <string>  // Allows use of string data type for names and IDs
#include <fstream> // Allows reading from and writing to files
#include <iomanip> // (Input / Output Manipulators)Allows formatting of output e.g setting decimal places for money 
#include <algorithm> //To convert driver IDs to uppercase for case-insensitive matching.
using namespace std; 

//--------------------------------
// File paths
//--------------------------------
const string DRIVERS_FILE = "drivers.txt"; //using constants because file names are fixed and should not be changed during program execution.
const string JAN_FILE = "jan26.txt";
const string FEB_FILE = "feb26.txt";
const string MAR_FILE = "mar26.txt";
//--------------------------------
// Constants for tax calculations
//--------------------------------
const double TAX_RATE = 0.20;//if tax rules change, i only update this.
const double TAX_FREE_ALLOWANCE = 12570;
//--------------------------------
// Maximum input size for user input validation
//--------------------------------
const int MAX_INPUT = 1000;//Ignore or throw away characters still left in the keyboard input buffer.
//------------------------------
// Driver class to hold driver details such as ID, name, and rate per mile
//------------------------------
class Driver 
{
public://Means these members can be accessed outside the class.
    string id;
    string name;
	double ratePerMile = 0.0; // Stores the pay rate per mile for the driver,defaqult starting value
};
//--------------------------------
// Function to process payroll for a given month for all drivers
//--------------------------------
// Pass drivers vector by reference to avoid copying and improve performance
void processPayroll(string filename, vector<Driver>& drivers)
{
	ifstream payFile(filename);// Open the payroll file for the specified month

    if (!payFile)
    {
        cout << "Error opening " << filename << endl;
        return;
    }

	string outputName = filename.substr(0, filename.find("."));// Extract the base name of the file (e.g., "jan26" from "jan26.txt")
	outputName += "_output.txt";// Create the output file name by appending "_output.txt" to the base name (e.g., "jan26_output.txt")

	ofstream outputFile(outputName);// Open the output file to write the processed payroll results

	outputFile << fixed << setprecision(2);// Format all decimal numbers to 2 decimal places for currency formatting
	//------------------------------
	// Variables to hold data read from the payroll file
	//------------------------------
    string payID;
    double miles;
	// Display the name of the payroll file being processed for clarity in the console output
    cout << "\033[35m";
	string shortName = filename.substr(filename.find_last_of("\\") + 1);// Extracts the file name from the full path for display

    cout << "\n==============================\n";
    cout << "   PROCESSING: " << shortName << endl;
    cout << "==============================\n";
    cout << "\033[0m";
    // Read each record from the payroll file (driver ID and miles driven) until end of file
    while (payFile >> payID >> miles)
    {
		bool found = false;//Assumes driver not found at first, will be set to true if a matching driver ID is found in the drivers list

		for (int i = 0; i < drivers.size(); i++)// Loop through the list of drivers to find a matching ID
        {
			string driverID = drivers[i].id;// Get current driver's ID from the vector
			string inputID = payID;// Store payroll file ID for comparison

            // convert both to uppercase
            transform(driverID.begin(), driverID.end(), driverID.begin(), ::toupper);
			// Convert the input ID to uppercase for case-insensitive comparison
            transform(inputID.begin(), inputID.end(), inputID.begin(), ::toupper);
            //Compare both IDs after converting to uppercase
            if (driverID == inputID)
            {
				found = true;// If a match is found, calculate the pay and tax
				// Calculate monthly pay based on miles driven and rate per mile
                double monthlyPay = miles * drivers[i].ratePerMile;
				double annualPay = monthlyPay * 12;// Calculate annual pay to determine tax liability
				double monthlyTax = 0;// Initialize monthly tax to zero, some drivers may not owe tax if their annual pay is below the tax-free allowance

                if (annualPay > TAX_FREE_ALLOWANCE)
                {
					double taxableIncome = annualPay - TAX_FREE_ALLOWANCE;// Calculate taxable income by subtracting the tax-free allowance from annual pay
                    double annualTax = taxableIncome * TAX_RATE;//Calculate annual tax (20% of taxable income)
                    monthlyTax = annualTax / 12; // Convert annual tax into monthly tax
                }
                //Calculate final pay after deducting tax
                double payAfterTax = monthlyPay - monthlyTax;
                // Write results to output file (ID, pay before tax, pay after tax)
                outputFile << drivers[i].id << " "<< monthlyPay << " " << payAfterTax << endl;
                // Display results on console in a readable format
                cout << "ID: " << drivers[i].id << endl;
                cout << "Monthly Pay (Before Tax): " << monthlyPay << endl;
                cout << "Monthly Pay (After Tax): " << payAfterTax << endl;
                cout << "-----------------------------\n";
            }
        }

        if (!found)
        {
            cout << "Error: Driver ID " << payID << " not found." << endl;
        }
    }
	// Close both the payroll file and the output file after processing is complete because we are done with them and to free up system resources.
    payFile.close();
    outputFile.close();
}
//--------------------------------
// Function to search for a specific one driver in the payroll file and display their pay details
//--------------------------------
// Pass drivers vector by reference to avoid copying and improve performance
double searchPayroll(string filename, vector<Driver>& drivers, string searchID)
{
    ifstream payFile(filename);

    if (!payFile)
    {
        cout << "Error opening " << filename << endl;
        return 0;  
    }
	// Variables to hold data read from the payroll file
    string payID;
    double miles;
	//short file name for display
	string shortName = filename.substr(filename.find_last_of("\\") + 1);// Extracts the file name from the full path for display

    cout << "\n--- " << shortName << " ---\n";
    // Loop through the payroll file to find matching driver records
	while (payFile >> payID >> miles)//loop through each record in the payroll file, reading the driver ID and miles driven until the end of the file is reached
    {
        string fileID = payID;//ID from monthly file
		string inputID = searchID;//ID entered by user for search

		transform(fileID.begin(), fileID.end(), fileID.begin(), ::toupper);// Convert the file ID to uppercase for case-insensitive comparison
		transform(inputID.begin(), inputID.end(), inputID.begin(), ::toupper);// Convert the input ID to uppercase for case-insensitive comparison

        if (fileID == inputID)
        {
			for (int i = 0; i < drivers.size(); i++)// Loop through the list of drivers to find a matching ID and calculate pay details
            {
				string driverID = drivers[i].id;// ID from drivers list
				string driverFileID = payID;// ID from monthly file

                transform(driverID.begin(), driverID.end(), driverID.begin(), ::toupper);
                transform(fileID.begin(), fileID.end(), fileID.begin(), ::toupper);

				if (driverID == fileID)// If a matching driver is found, calculate and display their pay details
                {
                    double monthlyPay = miles * drivers[i].ratePerMile;
                    double annualPay = monthlyPay * 12;
                    double monthlyTax = 0;

					if (annualPay > TAX_FREE_ALLOWANCE)// Calculate tax only if annual pay exceeds the tax-free allowance
                    {
                        double taxableIncome = annualPay - TAX_FREE_ALLOWANCE;
                        double annualTax = taxableIncome * TAX_RATE;
                        monthlyTax = annualTax / 12;
                    }

					double payAfterTax = monthlyPay - monthlyTax;// Calculate final pay after tax deduction

                    cout << "Monthly Pay (After Tax): " << payAfterTax << endl;

					return payAfterTax;  // Return the pay after tax for this driver to be added to the total in main()
                }
            }
        }
    }

	payFile.close();// Close the file after processing

    return 0;  
}
int main()
{
    //welcome message

    cout << "\033[33m";  // yellow

    cout << "              ______________________" << endl;
    cout << "     ________/|_||_\\`.__" << endl;
    cout << "    (   _    _    _ _\\   )*LOGISTICS*" << endl;
    cout << "    =`-(_)--(_)-(_)--(_)-'--------------" << endl;
    cout << endl;

    cout << "\033[35m";  // Magenta 

    cout << "==================================================" << endl;
    cout << "        LOGISTICS DRIVER PAYROLL SYSTEM           " << endl;
    cout << "==================================================" << endl;

    cout << "\033[33m";  // Yellow

    cout << "          Payroll Processing Application          " << endl;
    cout << "            Academic Year 2025 - 2026             " << endl;
    cout << "           Developed by: Hina Shehzad             " << endl;

    cout << "\033[35m";  
    cout << "==================================================" << endl;

    cout << endl;

    cout << "\033[33m";   // Yellow
    cout << "Press Enter to continue...";
    cout << "\033[0m";    // Reset colour
    cin.get();
    system("cls");

	//--------------------------------
    //Formats all decimal numbers to 2 decimal places
	//--------------------------------
    cout << fixed << setprecision(2);


	// Load driver data from file
	vector<Driver> drivers;// Create a vector to store driver records read from the file

	// Open the drivers file and check if it opened successfully
    ifstream inputFile(DRIVERS_FILE);
    if (!inputFile)
    {
        cout << "Error opening drivers.txt" << endl;
        return 1;//End the program and indicate that an error happened,reyurn 1 means Program ended because of a problem / error.
    }
	// Temporary variable to hold driver data while reading from file
    Driver temp;
	// Read driver data from file and store in vector
    while (inputFile >> temp.id >> temp.name >> temp.ratePerMile) 
    {
        drivers.push_back(temp);
    }

	inputFile.close();// Close the file after reading to free up system resources
	// Display loaded driver data
    cout << "\033[35m";
    cout << "Drivers Loaded Successfully!\n" << endl;
    cout << "\033[0m";
	for (int i = 0; i < drivers.size(); i++) // Loop through the list of drivers and display their details
    {
        cout << "ID: " << drivers[i].id << endl;
        cout << "Name: " << drivers[i].name << endl;
        cout << "Rate: " << drivers[i].ratePerMile << endl;
        cout << "----------------------" << endl;
    }
    system("cls");
	// Display main menu and get user choice
    int choice;  // variable to store user choice
    system("cls");  // clear screen before showing menu
    cout << "\033[35m";
    cout << "==============================\n";
    cout << "        MAIN MENU\n";
    cout << "==============================\n";
    cout << "\033[33m";
    cout << "1. View all drivers\n";
    cout << "2. Search for a driver\n";
    cout << "\033[35m";
    cout << "Enter your choice: ";
    cout << "\033[0m";    // Reset colour
    cin >> choice;

	// Validate user input to ensure it's a number and within expected range
    if (cin.fail())
    {
        cin.clear(); // clear error
        cin.ignore(MAX_INPUT, '\n'); // remove bad input
        cout << "Invalid input! Please enter a number.\n";
        return 0;
    }
	// Check if choice is either 1 or 2, if not display error message and exit
    if (choice < 1 || choice > 2)
    {
        cout << "Invalid choice!" << endl;
        return 0;
    }
    system("cls");
    if (choice == 1)
    {
        system("cls");  // clear menu show results on new screen
        // View all drivers run all payroll files

        processPayroll(JAN_FILE, drivers);
        processPayroll(FEB_FILE, drivers);
        processPayroll(MAR_FILE, drivers);

    }
    else if (choice == 2)
    {
        system("cls");

		string searchID;// Variable to store the driver ID entered by the user for searching
        cout << "Enter Driver ID: ";
		cin >> searchID;// Get the driver ID to search for from user input

        system("cls");

        cout << "\nDriver: " << searchID << endl;
		double total = 0;// Variable to accumulate total pay across all months for the searched driver

        // Store all monthly files in a list
        vector<string> payFiles = { JAN_FILE, FEB_FILE, MAR_FILE };

        // Loop through each file
        for (int i = 0; i < payFiles.size(); i++)
        {
            total += searchPayroll(payFiles[i], drivers, searchID);
        }

        cout << "\nTOTAL PAY: " << total << endl;
    }

    else
    {
        cout << "Invalid choice!" << endl;
    }
	
}