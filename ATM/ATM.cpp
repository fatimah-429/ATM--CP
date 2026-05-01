#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
using namespace std;

class Bank_Account
{
private: //data at backend
	int accnum;
	string ownername;
	double balance;
	int pin;

public: //functions
	void saveToFile(ofstream& outFile) {
		outFile << ownername << endl;
		outFile << balance << endl;
	}

	void loadFromFile(ifstream& inFile) {
		inFile >> ownername >> balance;
	}

	void logTransaction(string type, double amount) {
		ofstream logFile("transactions.csv", ios::app);
		logFile 
			<< ownername << ","
			<<accnum<<","
			<< type << ","
			<< amount << ","
			<< balance << endl;
		logFile.close();
	}

	Bank_Account(int num, string name, double s_balance, int userpin) //assigns values to variables - constructor 
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
		if (amount>balance)
		{
			cout << "insufficent funds" << endl;
			return false;
		}
		
		if (amount <= 0)
			{
				cout << " Invalid amount" << endl;
				return false;
			}	
		balance = balance - amount;	
		cout << "You withdrew amount :" << amount << endl << "Your remaining Balance is :" << balance << endl;
		return true;
	}
	string getname() {
		return ownername;
	}
	int accnumber() {
		return accnum;
	}
	
	


};
class ATM
{
private:

	Bank_Account accounts[3] = {
		Bank_Account(1001,"Ali",   1000.0, 1234),
		Bank_Account(1002,"Sarah", 600.0,  5687),
		Bank_Account(1003,"Hira",  7700.0, 9876)



	};
	int findaccount(int num) {
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

	int masking(int pin) {
		bool flag = true;
		int x = 0;
		int i = 0;
		while (flag && i < 4) {
			int ch = _getch();
			if (ch <= '9' && ch >= '0') {
				cout << "*";
				x = x * 10 + (ch - '0');
				i++;
			}
			if (ch == 13) {
				flag = false;
			}

		}
		cout << endl;
		return x;
	}

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
	}
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
	

	void saveAccounts() {
		ofstream outFile("accounts.txt");
		for (int i = 0; i < 3; i++) {
			accounts[i].saveToFile(outFile);
		}
		outFile.close();
	}

	void loadAccounts() {
		ifstream inFile("accounts.txt");
		if (!inFile) return;
		for (int i = 0; i < 3; i++) {
			accounts[i].loadFromFile(inFile);
		}
		inFile.close();
	}
	

	void run() {
		loadAccounts();
		int option;
		int enteredPin = 0;  //taking in PIN
		int check = 0;
		int index;
		int accnum;
		cout << "Enter your account number:" << endl;
		cin >> accnum;
		index = findaccount(accnum);
		if (index < 0 || index > 2) //checking for account
		{
			cout << "Account not registered." << endl;
			return;
		}

		cout << "Enter Your Pin: " << endl;

		while (check < 3)
		{
			enteredPin = masking(enteredPin);
			if (!enteredPin)
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
		if (check == 3)
		{
			cout << " Your card has been blocked. Contact the bank for queries" << endl;
			return;
		}

		while (true)
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
