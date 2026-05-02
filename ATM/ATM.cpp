#include <iostream>
#include <string>
#include <fstream> //file handling
#include <conio.h> //getch
using namespace std;

class Bank_Account
{
private: //variables stored
	int accnum;
	string ownername;
	double balance;
	int pin;

public: //methods
	void saveToFile(ofstream& outFile) { //data into file
		outFile << ownername << endl;
		outFile << balance << endl;
	}

	void loadFromFile(ifstream& inFile) { //data from file
		inFile >> ownername >> balance;
	}

	void logTransaction(string type, double amount) {
		ofstream logFile("transactions.csv", ios::app); //data to file without removing previous logs - append function
		logFile 
			<< ownername << ","
			<<accnum<<","
			<< type << ","
			<< amount << ","
			<< balance << endl;
		logFile.close();
	}

	Bank_Account(int num, string name, double s_balance, int userpin) //assigns values to variables upon entering into program - constructor 
	{
		ownername = name;
		balance = s_balance;
		pin = userpin;
		accnum = num;

	}
	
	bool checkpin(int pincode) //checking pin
	{
		return pincode == pin;
	}

	void showbalance() //showing present balance
	{
		cout << ownername << endl << balance << endl;
	}
	bool deposit(double amount) //entering deposit 
	{
		if (amount <= 0)
		{
			cout << "invalid deposit";
			return false;
		}
		else
		{
			balance = amount + balance;
			

			cout << "deposited amount: " << amount << endl << "New Balance is now : " << balance << endl;
			return true;
		}
	}
	bool withdraw(double amount) //withdrawal
	{
		if (amount>balance) // for overdrafting
		{
			cout << "insufficent funds" << endl;
			return false;
		}
		
		if (amount <= 0) //amount less than 0 not credible
			{
				cout << " Invalid amount" << endl;
				return false;
			}	
		balance = balance - amount;	
		cout << "You withdrew amount :" << amount << endl << "Your remaining Balance is :" << balance << endl;
		return true;
	}
	string getname() { //getter for owner name
		return ownername;
	}
	int accnumber() { //getter for account number
		return accnum;
	}
	
	


};
class ATM //class for ATM interface
{
private:

	Bank_Account accounts[3] = {
		Bank_Account(1001,"Ali",   1000.0, 1234),
		Bank_Account(1002,"Sarah", 600.0,  5687),
		Bank_Account(1003,"Hira",  7700.0, 9876)



	}; //array for objects of Bank_Account class
	int findaccount(int num) { //comparison with account number to find account
		int f = 0;
		int i = 0;
		while (i < 3)
		{
			if (num == accounts[i].accnumber())
			{
				f = i;
				break;
			}
			i++;
		}
		return f;

	}
//masking - for privacy
	int masking(int pin) {
		bool flag = true;
		int x = 0;
		int i = 0;
		while (flag && i < 4) { //only 4 number pins
			int ch = _getch(); //getch gives characters 
			if (ch <= '9' && ch >= '0') { //limits between 9 and o
				cout << "*";
				x = x * 10 + (ch - '0'); //converting into integer
				i++;
			}
			if (ch == 13) { //exits loop at enter - 13 ASCII for enter
				flag = false;
			}

		}
		cout << endl;
		return x;
	}
// to print receipt, only store the last line of the respective name
	void receipt(int i) {
		ifstream inFile("transactions.csv");
		string line;
		
		string last = "";
		while (getline(inFile, line))
		{
			if (line.find(accounts[i].getname()) == 0) {
				last = line;
			}
		}
		cout << last <<endl;
		inFile.close();
	}//to print statement, loops through all the lines that match the respective name and displayes them
	void statement(int i) {
		ifstream inFile("transactions.csv");
		string line;
		while (getline(inFile, line)) {
			if (line.find(accounts[i].getname()) == 0) {
				cout << line << endl;
			}
		}
		inFile.close();
	}


public:
//methods

	void saveAccounts() { //save information to accounts
		ofstream outFile("accounts.txt");
		for (int i = 0; i < 3; i++) {
			accounts[i].saveToFile(outFile);
		}
		outFile.close();
	}

	void loadAccounts() { //taking information from the file
		ifstream inFile("accounts.txt");
		if (!inFile) return;
		for (int i = 0; i < 3; i++) {
			accounts[i].loadFromFile(inFile);
		}
		inFile.close();
	}
	

	void run() { //main interface code, runs the menu
		loadAccounts();
		int option;
		int enteredPin = 0;  //taking in PIN
		int check = 0;
		int index;
		int accnum;
		cout << "Enter your account number:" << endl;
		cin >> accnum;
		index = findaccount(accnum);
		if (index < 0 || index > 2) //checking for account in the presaved array
		{
			cout << "Account not registered." << endl;
			return;
		}

		cout << "Enter Your Pin: " << endl;

		while (check < 3) // loop runs thrice only
		{
			enteredPin = masking(enteredPin);
			if (!enteredPin) // does not allow unknown or empty pin
			{
				cout << "Invalid Pin, Try again" << endl;
				return;
			}

			if (!accounts[index].checkpin(enteredPin)) //checking if the PIN is correct
			{
				cout << "Wrong Pin, Access Denied!" << endl;

				check++;

				cout << "Attempts remaining: " << 3 - check << endl;
			}
			else {

				cout << "Welcome" << endl;
				break;
			}
		}
		if (check == 3) // blocks more attempts
		{
			cout << " Your card has been blocked. Contact the bank for queries" << endl;
			return;
		}

		while (true) //runs menu if Access is granted
		{
			cout << "--MENU--" << endl;
			cout << "choose your option: " << endl
				<< "1. Check Balance" << endl
				<< "2. Deposit Amount" << endl
				<< "3. Withdraw Amount " << endl
				<< "4. Print Receipt" << endl
				<< "5.Request Bank Statement" << endl
				<< "6. Exit Menu" << endl;

			cin >> option;
			switch (option)
			{
			case 1:
			{

				accounts[index].showbalance();
				break;
			}

			case 2:
			{
				double amount;
				cout << "Enter amount: " << endl;
				cin >> amount;
				if (accounts[index].deposit(amount)) {
					saveAccounts();
					accounts[index].logTransaction("deposit", amount);
				}
				break;
			}

			case 3:
			{
				double amount;
				cout << "Enter amount: " << endl;
				cin >> amount;
				if (accounts[index].withdraw(amount)) {
					saveAccounts();
					accounts[index].logTransaction("withdraw", amount);
				}
				break;

			}
			case 4:
			{
				cout << "------Transaction Receipt------" << endl;
				receipt(index);
				cout << endl;
				break;
			}
			case 5:
			{
				cout << "------Bank Statement------" << endl;
				statement(index);
				cout << endl;
				break;
			
			}
			
			case 6:
			{
				cout << "Thank you for choosing Bank ---. Goodbye!" << endl;
				return;
				break;
			}
			default:
			{
				cout << "Invalid option, Try again.";
			}

			}
		}
	}
};

	int main() {
		ATM atm;
		atm.run();
		return 0;
	}
