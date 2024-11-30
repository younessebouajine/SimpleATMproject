
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<iomanip>
using namespace std;
struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

enum enMainMenueOptions
{
	eQucikWithdraw = 1, eNormalWithDraw = 2, eDeposit = 3, eChangePinCode ,
	eCheckBalance = 5, eExit = 6
};
const string ClientsFileName = "Clients.txt";
sClient CurrentClient;
void ShowMainMenue();
void Login();
void ShowQuickWithdrawScreen();
void ShowNormalWithDrawScreen();
void ShowDepositScreen();
vector<string> SplitString(string  S1, string Delim)
{

	vector<string> vString;
	short pos = 0;
	string sWord; // define a string variable  // use find() function to get the position of the delimiters  
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos); // store the word  
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
	}
	if (S1 != "")
	{
		vString.push_back(S1); // it adds last word of the string. 
	}
	return vString;
}
sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
	sClient Client; vector<string> vClientData;
	vClientData = SplitString(Line, Seperator);
	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];

	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);//cast string to double
	return Client;
}
string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}
vector <sClient> LoadCleintsDataFromFile(string FileName)
{

	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLinetoRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}
bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient& Client)
{
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	for (sClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
		{
			Client = C;
			return true;
		}
	}
	return false;
}
vector <sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite
	string DataLine;
	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{
	char Answer = 'n';
	cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		for (sClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveCleintsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
				return true;
			}
		}
		return false;
	}
}
bool ChangePinCodeinTheFile(string AccountNumber, string PinCode , vector <sClient> & vClients)
{
	char Answer = 'n';
	cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		for (sClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				//C.AccountBalance += Amount;
				C.PinCode = PinCode;
				SaveCleintsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfully. New PinCode is: " << C.PinCode;
				return true;
			}
		}
		return false;
	}
}
short ReadQuickWithdrawOption()
{
	short Choice = 0;
	while (Choice < 1 || Choice>9)
	{
		cout << "\nChoose what to do from [1] to [9] ? ";
		cin >> Choice;
	}
	return Choice;
}
short getQuickWithDrawAmount(short QuickWithDrawOption)
{
	switch (QuickWithDrawOption)
	{
	case 1:
		return  20;
	case 2:
		return 50;
	case 3:
		return 100;
	case 4:
		return 200;
	case 5:
		return 400;
	case 6:
		return 600;
	case 7:
		return 800;
	case 8:
		return 1000;
	default:
		return 0;
	}
}
void PerfromQuickWithdrawOption(short QuickWithDrawOption)
{
	if (QuickWithDrawOption == 9)//exit 
		return;
	short WithDrawBalance = getQuickWithDrawAmount(QuickWithDrawOption);
	if (WithDrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		cout << "Press Anykey to continue...";
		system("pause>0");
		ShowQuickWithdrawScreen(); return;
	}
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);
	CurrentClient.AccountBalance -= WithDrawBalance;
}
double ReadDepositAmount()
{
	double Amount;
	cout << "\nEnter a positive Deposit Amount? ";
	cin >> Amount;
	while (Amount <= 0)
	{
		cout << "\nEnter a positive Deposit Amount? ";
		cin >> Amount;
	}
	return Amount;
}
void PerfromDepositOption()
{
	double DepositAmount = ReadDepositAmount();
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
	CurrentClient.AccountBalance += DepositAmount;
}
void ShowDepositScreen() {
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tDeposit Screen\n";
	cout << "===========================================\n";
	PerfromDepositOption();
}
void ShowCheckBalanceScreen()
{
	system("cls");

	cout << "===========================================\n";
	cout << "\t\tCheck Balance Screen\n";
	cout << "===========================================\n";
	cout << "Your Balance is " << CurrentClient.AccountBalance << "\n";
}
int ReadWithDrawAmont()
{
	int Amount;
	cout << "\nEnter an amount multiple of 5's ? ";
	cin >> Amount;
	while (Amount % 5 != 0)
	{
		cout << "\nEnter an amount multiple of 5's ? ";
		cin >> Amount;
	}
	return Amount;
}
void PerfromNormalWithdrawOption()
{
	int WithDrawBalance = ReadWithDrawAmont();
	if (WithDrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		cout << "Press Anykey to continue...";
		system("pause>0");
		ShowNormalWithDrawScreen();
		return;
	}
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);
	CurrentClient.AccountBalance -= WithDrawBalance;
}
void ShowNormalWithDrawScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tNormal Withdraw Screen\n";
	cout << "===========================================\n";
	PerfromNormalWithdrawOption();
}
void ShowQuickWithdrawScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tQucik Withdraw\n";
	cout << "===========================================\n";
	cout << "\t[1] 20\t\t[2] 50\n";
	cout << "\t[3] 100\t\t[4] 200\n";
	cout << "\t[5] 400\t\t[6] 600\n";
	cout << "\t[7] 800\t\t[8] 1000\n";
	cout << "\t[9] Exit\n";
	cout << "===========================================\n";
	cout << "Your Balance is " << CurrentClient.AccountBalance;
	PerfromQuickWithdrawOption(ReadQuickWithdrawOption());
}

void ChangPinCode()
{
	string NewPinCode;
	cout << "\n So , enter a new PinCode : ";
	cin >> NewPinCode;
	vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
	ChangePinCodeinTheFile(CurrentClient.AccountNumber, NewPinCode, vClients);
	CurrentClient.PinCode = NewPinCode;
	//cout << "\n Done Successfully , New Pin Code is : " << CurrentClient.PinCode;
}

void ShowChangPinCoceScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tChange Pin Code \n";
	cout << "===========================================\n";
	cout << " Your Pin Code is : " << CurrentClient.PinCode;
	cout << "\n Are you sure do you want change your Pin Code : (Y/N) ? ";
	char AN;
	cin >> AN;
	if (AN == 'y' || AN == 'Y')
	{
		ChangPinCode();
	}
} 

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowMainMenue();
}
short ReadMainMenueOption()
{
	cout << "Choose what do you want to do? [1 to 5]? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}
void PerfromMainMenueOption(enMainMenueOptions MainMenueOption) {
	switch (MainMenueOption)
	{


	case enMainMenueOptions::eQucikWithdraw:
	{
		system("cls");
		ShowQuickWithdrawScreen();
		GoBackToMainMenue();
		break;
	}
	case enMainMenueOptions::eNormalWithDraw:
		system("cls");
		ShowNormalWithDrawScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eDeposit:

		system("cls");
		ShowDepositScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eChangePinCode:
		
		ShowChangPinCoceScreen();
		GoBackToMainMenue();
	case enMainMenueOptions::eCheckBalance:
		system("cls");
		ShowCheckBalanceScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eExit:
		system("cls");
		Login();
		break;
	}
}
void ShowMainMenue()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tATM Main Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit\n";
	cout << "\t[4] Change Pin Code\n";
	cout << "\t[5] Check Balance.\n";
	cout << "\t[6] Logout.\n";
	cout << "===========================================\n";
	PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}
bool  LoadClientInfo(string AccountNumber, string PinCode)
{
	if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
		return true;
	else
		return false;
}
void Login()
{
	bool LoginFaild = false;
	string AccountNumber, PinCode;
	do
	{
		system("cls");

		cout << "\n---------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n---------------------------------\n";
		if (LoginFaild)
		{
			cout << "Invlaid Account Number/PinCode!\n";
		}
		cout << "Enter Account Number? ";
		cin >> AccountNumber;
		cout << "Enter Pin? ";
		cin >> PinCode;
		LoginFaild = !LoadClientInfo(AccountNumber, PinCode);
	} while (LoginFaild);
	ShowMainMenue();
}
int main()
{
	Login();
	system("pause>0");
	return 0;
}
























//#include<iostream>
//#include<fstream>
//#include<string>
//#include<vector>
//#include<iomanip>
//using namespace std;
//struct sClient
//{
//	string AccountNumber;
//	string PinCode;
//	string Name;
//	string Phone;
//	double AccountBalance;
//	bool MarkForDelete = false; 
//};
//
//enum enMainMenueOptions
//{
//	eQucikWithdraw = 1, eNormalWithDraw = 2, eDeposit = 3,
//	eCheckBalance = 4, eExit = 5 
//}; 
//const string ClientsFileName = "Clients.txt";
//sClient CurrentClient;
//void ShowMainMenue();
//void Login();
//void ShowQuickWithdrawScreen();
//void ShowNormalWithDrawScreen(); 
//void ShowDepositScreen();
//vector<string> SplitString(string  S1, string Delim)
//{
//
//	vector<string> vString;
//	short pos = 0; 
//	string sWord; // define a string variable  // use find() function to get the position of the delimiters  
//	while ((pos = S1.find(Delim)) != std::string::npos) 
//	{       
//		sWord = S1.substr(0, pos); // store the word  
//		if (sWord != "")    
//		{  
//			vString.push_back(sWord);    
//		}
//		S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */   
//	}
//	if (S1 != "")  
//	{     
//		vString.push_back(S1); // it adds last word of the string. 
//	}
//	return vString;
//} 
//sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
//{
//	sClient Client; vector<string> vClientData;   
//	vClientData = SplitString(Line, Seperator);  
//	Client.AccountNumber = vClientData[0];    
//	Client.PinCode = vClientData[1];  
//	Client.Name = vClientData[2];  
//
//	Client.Phone = vClientData[3]; 
//	Client.AccountBalance = stod(vClientData[4]);//cast string to double
//	return Client;
//} 
// string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
// {
//	 string stClientRecord = "";  
//	 stClientRecord += Client.AccountNumber + Seperator; 
//	 stClientRecord += Client.PinCode + Seperator;  
//	 stClientRecord += Client.Name + Seperator;    
//	 stClientRecord += Client.Phone + Seperator;   
//	 stClientRecord += to_string(Client.AccountBalance);
//	 return stClientRecord;
// }
// vector <sClient> LoadCleintsDataFromFile(string FileName)
// {
//
//		vector <sClient> vClients;
//		fstream MyFile;   
//		MyFile.open(FileName, ios::in);//read Mode
//		if (MyFile.is_open())   
//		{ 
//			string Line; 
//			sClient Client;
//			while (getline(MyFile, Line))    
//			{   
//				Client = ConvertLinetoRecord(Line);
//				vClients.push_back(Client);   
//			}    
//			MyFile.close(); 
//		}
//		return vClients;
// } 
// bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient & Client) 
// {
//	 vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName); 
//	 for (sClient C : vClients)
//	 { 
//		 if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
//		 {
//			 Client = C;
//			 return true;
//		 } 
//	 }
//	 return false; 
// }
// vector <sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients) 
// {
//			fstream MyFile;  
//			MyFile.open(FileName, ios::out);//overwrite
//			string DataLine;
//			if (MyFile.is_open())   
//			{
//				for (sClient C : vClients)  
//				{
//					if (C.MarkForDelete == false)  
//					{
//						DataLine = ConvertRecordToLine(C); 
//						MyFile << DataLine << endl;   
//					}     
//				}    
//				MyFile.close();   
//			}
//			return vClients; 
// } 
// bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>&vClients)
// {
//	 char Answer = 'n';   
//	 cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";   
//	 cin >> Answer; 
//	 if (Answer == 'y' || Answer == 'Y') 
//	 {
//		 for (sClient& C : vClients)
//		 {
//			 if (C.AccountNumber == AccountNumber)
//			 {
//				 C.AccountBalance += Amount; 
//				 SaveCleintsDataToFile(ClientsFileName, vClients); 
//				 cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance; 
//				 return true; 
//			 }
//		 }
//		 return false; 
//	 }
// }
// short ReadQuickWithdrawOption()
// {
//	 short Choice = 0;
//	 while (Choice < 1 || Choice>9)
//	 {
//		 cout << "\nChoose what to do from [1] to [9] ? ";   
//		 cin >> Choice; 
//	 }
//	 return Choice;
// }
// short getQuickWithDrawAmount(short QuickWithDrawOption)
// {
// switch (QuickWithDrawOption)
// {
// case 1: 
// return  20;
// case 2:
// return 50;
// case 3:
// return 100;
// case 4:
// return 200;
// case 5: 
// return 400;
// case 6:
// return 600;
// case 7: 
// return 800;
// case 8: 
// return 1000;
// default:
//	 return 0;
// }
// }
// void PerfromQuickWithdrawOption(short QuickWithDrawOption) 
// {
//	 if (QuickWithDrawOption == 9)//exit 
//		 return;
//	 short WithDrawBalance = getQuickWithDrawAmount(QuickWithDrawOption); 
//	 if (WithDrawBalance > CurrentClient.AccountBalance) 
//	 {      
//		 cout << "\nThe amount exceeds your balance, make another choice.\n";    
//		 cout << "Press Anykey to continue...";    
//		 system("pause>0");     
//		 ShowQuickWithdrawScreen(); return;   
//	 }
//	 vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
//	 DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);   
//	 CurrentClient.AccountBalance -= WithDrawBalance;
// }
// double ReadDepositAmount() 
// { 
//	 double Amount; 
// cout << "\nEnter a positive Deposit Amount? ";  
// cin >> Amount;
// while (Amount <= 0)   
// {    
//	 cout << "\nEnter a positive Deposit Amount? ";   
//	 cin >> Amount;  
// }
// return Amount; 
// } 
//  void PerfromDepositOption()
//		  {
//			  double DepositAmount = ReadDepositAmount();
//			  vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
//			  DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
//			  CurrentClient.AccountBalance += DepositAmount;
//  } 
//  void ShowDepositScreen() {
//  system("cls");   
//  cout << "===========================================\n";   
//  cout << "\t\tDeposit Screen\n";  
//  cout << "===========================================\n";  
//  PerfromDepositOption();
//  }
//  void ShowCheckBalanceScreen() 
//  {
//	  system("cls"); 
//
//	  cout << "===========================================\n";  
//	  cout << "\t\tCheck Balance Screen\n";  
//	  cout << "===========================================\n";   
//	  cout << "Your Balance is " << CurrentClient.AccountBalance << "\n"; 
//  }
//  int ReadWithDrawAmont()
//  {
//	  int Amount;   
//	  cout << "\nEnter an amount multiple of 5's ? ";  
//	  cin >> Amount;
//	  while (Amount % 5 != 0) 
//	  {
//		  cout << "\nEnter an amount multiple of 5's ? ";  
//		  cin >> Amount;
//	  } 
//	  return Amount; 
//  }
//	void PerfromNormalWithdrawOption() 
//	{ 
//		int WithDrawBalance = ReadWithDrawAmont();
//		if (WithDrawBalance > CurrentClient.AccountBalance)
//		{
//			cout << "\nThe amount exceeds your balance, make another choice.\n";    
//			cout << "Press Anykey to continue...";      
//			system("pause>0"); 
//			ShowNormalWithDrawScreen();
//			return; 
//		} 
//		vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
//		DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);  
//		CurrentClient.AccountBalance -= WithDrawBalance; 
//	} 
//	void ShowNormalWithDrawScreen() 
//	{ 
//		system("cls");
//	cout << "===========================================\n";   
//	cout << "\t\tNormal Withdraw Screen\n";  
//	cout << "===========================================\n";   
//	PerfromNormalWithdrawOption(); 
//	}
//	void ShowQuickWithdrawScreen()
//	{
//		system("cls"); 
//		cout << "===========================================\n";  
//		cout << "\t\tQucik Withdraw\n";   
//		cout << "===========================================\n"; 
//		cout << "\t[1] 20\t\t[2] 50\n";  
//		cout << "\t[3] 100\t\t[4] 200\n";  
//		cout << "\t[5] 400\t\t[6] 600\n";  
//		cout << "\t[7] 800\t\t[8] 1000\n";   
//		cout << "\t[9] Exit\n";  
//		cout << "===========================================\n"; 
//		cout << "Your Balance is " << CurrentClient.AccountBalance;   
//		PerfromQuickWithdrawOption(ReadQuickWithdrawOption()); 
//	}
//	void GoBackToMainMenue() 
//	{
//		cout << "\n\nPress any key to go back to Main Menue...";  
//		system("pause>0");   
//		ShowMainMenue();
//	}
//	short ReadMainMenueOption() 
//	{
//		cout << "Choose what do you want to do? [1 to 5]? "; 
//		short Choice = 0;   
//		cin >> Choice;
//		return Choice; 
//	}
//	void PerfromMainMenueOption(enMainMenueOptions MainMenueOption) {
//		 switch (MainMenueOption) 
//		 {
//
//
//			 case enMainMenueOptions::eQucikWithdraw:  
//			 {   
//				 system("cls");  
//				 ShowQuickWithdrawScreen();  
//				 GoBackToMainMenue();
//				 break;  
//			 }
//			 case enMainMenueOptions::eNormalWithDraw :
//				 system("cls");  
//				 ShowNormalWithDrawScreen();   
//				 GoBackToMainMenue(); 
//				 break;
//			 case enMainMenueOptions::eDeposit:  
//					 
//				system("cls");   
//				ShowDepositScreen();  
//				GoBackToMainMenue(); 
//				break; 
//			case enMainMenueOptions::eCheckBalance:  
//				system("cls");   
//				ShowCheckBalanceScreen();  
//				GoBackToMainMenue();
//				break;
//			 case enMainMenueOptions::eExit:  
//				 system("cls");  
//				 Login();
//				 break;
//		 }
//	 }
//	void ShowMainMenue()
//	{ system("cls");  
//	cout << "===========================================\n"; 
//	cout << "\t\tATM Main Menue Screen\n";    
//	cout << "===========================================\n";  
//	cout << "\t[1] Quick Withdraw.\n";    
//	cout << "\t[2] Normal Withdraw.\n";   
//	cout << "\t[3] Deposit\n";   
//	cout << "\t[4] Check Balance.\n";  
//	cout << "\t[5] Logout.\n";   
//	cout << "===========================================\n";
//	PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption()); 
//	}
//	bool  LoadClientInfo(string AccountNumber, string PinCode) 
//	{
//		if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient)) 
//			return true; 
//		else 
//			return false; 
//	}
//	void Login() 
//	{
//		bool LoginFaild = false;
//		string AccountNumber, PinCode; do 
//		{
//			system("cls");    
//
//			cout << "\n---------------------------------\n";    
//			cout << "\tLogin Screen";     
//			cout << "\n---------------------------------\n";
//			if (LoginFaild)
//			{
//				cout << "Invlaid Account Number/PinCode!\n";
//			}      
//			cout << "Enter Account Number? ";  
//			cin >> AccountNumber;      
//			cout << "Enter Pin? ";    
//			cin >> PinCode;       
//			LoginFaild = !LoadClientInfo(AccountNumber, PinCode); 
//		} while (LoginFaild);  
//		ShowMainMenue();
//	}
//	int main()
//	{ 
//		Login(); 
//	system("pause>0"); 
//	return 0;
//	} 																																				
	
	




















//#include <iostream>
//#include <iomanip>
//#include <string>
//#include<vector>
//#include<math.h>
//#include<fstream>
//
//using namespace std;
//
//const string FileName = "MyFile.txt";
//
//void ShowQuickDrawScreen();
//void ShowMainMenue();
//void Login();
//void GoBackToMainMenue();
//void ShowNormalWithDrawScreen();
//
//enum enMainMenueOptions // Waht the chosse
//{
//	eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4,
//	eFindClient = 5, eShowTransactionsMenue = 6,
//	eManageUsers = 7, eExit = 8
//};
//
//enum ATMMenueOptions
//{
//	enQuickDraw = 1  , enNormalDraw = 2 , enDeposit = 3 , enCheckPalace  = 4 , enLogOut = 5
//};
//
//enum enChooseWithDraw
//{
//	anOne = 1 , enTwo = 2, enTrue = 3 , enFour = 4, enFifth = 5, enSix = 6, enSeven = 7,
//	enOuit = 8 , enExit = 9
//};
//
//struct sClient
//{
//	string AcountNumber;
//	string PinCode;
//	string Name;
//	string Phone;
//	double AcountBalance;
//	bool MarkClient = false;
//};
//
//sClient CurrentUser;
//
//vector <string> SplitString(string S1, string Delim)
//{
//	vector <string> vString;
//	string sWord;
//	short pos;
//	while ((pos = S1.find(Delim)) != std::string::npos)
//	{
//		sWord = S1.substr(0, pos);
//		if (sWord != " ")
//		{
//			vString.push_back(sWord);
//		}
//		S1.erase(0, pos + Delim.length());
//	}
//	if (S1 != " ")
//	{
//		vString.push_back(S1);
//	}
//	return vString;
//}
//
//sClient ConvertLineToRecord(string Line, string Operater = "#//#")
//{
//	sClient Client;
//	vector<string> vString;
//	vString = SplitString(Line, Operater);
//	Client.AcountNumber = vString[0];
//	Client.PinCode = vString[1];
//	Client.Name = vString[2];
//	Client.Phone = vString[3];
//	Client.AcountBalance = stod(vString[4]);
//	return Client;
//}
//
//string ConvertRecordToLine(sClient Client, string Operater = "#//#")
//{
//	string ConvertLine;
//	ConvertLine += Client.AcountNumber + Operater;
//	ConvertLine += Client.PinCode + Operater;
//	ConvertLine += Client.Name + Operater;
//	ConvertLine += Client.Phone + Operater;
//	ConvertLine += to_string(Client.AcountBalance);
//	return ConvertLine;
//}
//
//bool ClientExistsByAccountNumber(string AcountNumber, string FileName)
//{
//	vector <sClient> vClient;
//	fstream MyFile;
//	MyFile.open(FileName, ios::in);
//	if (MyFile.is_open())
//	{
//		string Line;
//		sClient Client;
//		while (getline(MyFile, Line))
//		{
//			Client = ConvertLineToRecord(Line);
//			if (Client.AcountNumber == AcountNumber)
//			{
//				MyFile.close();
//				return true;
//			}
//			vClient.push_back(Client);
//		}
//		MyFile.close();
//	}
//	return false;
//}
//
//vector <sClient> LoadCleintsDataFromFile(string FileName)
//{
//	vector<sClient> vClient;
//	fstream MyFile;
//	MyFile.open(FileName, ios::in);
//	if (MyFile.is_open())
//	{
//		sClient Client;
//		string Line;
//		while (getline(MyFile, Line))
//		{
//			Client = ConvertLineToRecord(Line);
//			vClient.push_back(Client);
//		}
//		MyFile.close();
//	}
//	return vClient;
//}
//
//bool MarkAcountClient(string AcountNumber, vector <sClient>& vClient)
//{
//	for (sClient& C : vClient)
//	{
//		if (C.AcountNumber == AcountNumber)
//		{
//			C.MarkClient = true;
//			return true;
//		}
//	}
//	return false;
//}
//
//bool FindClientByAcountNumberAndPinCode(string AcountNumber, string PinCode, sClient& Client) 
//{ 
//	vector <sClient> vClient = LoadCleintsDataFromFile(FileName);
//	for (sClient&  C: vClient)
//	{
//		if (C.AcountNumber == AcountNumber && C.PinCode == PinCode)
//		{
//			Client = C;
//			//if (MarkAcountClient(AcountNumber, vClient));
//			return true;
//		}
//	}
//	return false;
//}
//
//vector<sClient> SaveRecordAfterDeleteClient(string FileName, vector<sClient>vClient)
//{
//	fstream MyFile;
//	MyFile.open(FileName, ios::out);
//	string DateLine;
//	if (MyFile.is_open())
//	{
//		for (sClient& C : vClient)
//		{
//			if (C.MarkClient == false)
//			{
//				DateLine = ConvertRecordToLine(C);
//				MyFile << DateLine << endl;
//			}
//		}
//		MyFile.close();
//	}
//	return vClient;
//}
//
//bool FindAcountNumberFromRecordClient(string AcountNumber, vector<sClient> vClient, sClient& Client)
//{
//	for (sClient C : vClient)
//	{
//		if (C.AcountNumber == AcountNumber)
//		{
//			Client = C;
//			return true;
//		}
//	}
//	return false;
//}
//
// bool LoadClientInfo(string AcountNumber, string PinCode )
//{
//	if (FindClientByAcountNumberAndPinCode(AcountNumber, PinCode, CurrentUser))
//	{
//
//		return true;
//	}
//	return false;
//}
//
// void AcountBalance()
// {
//	 cout << "\n Your Balance is : " << CurrentUser.AcountBalance;
// }
//
// short ReadWithDraw()
// {
//	 cout << "\n Choose what to withdraw from [1] to [9] ? : ";
//	 short Chosse;
//	 cin >> Chosse;
//	 return Chosse;
// }
//
// void Perform1(short Number)
// {
//	 vector <sClient> vClient = LoadCleintsDataFromFile(FileName);
//	 sClient Client;
//	 char answer = 'n';
//	 short AcountBalance = 0;
//	 while (Number > CurrentUser.AcountBalance)
//	 {
//		 cout << "\n The amount exceeds your balance , Make anouther chosse \n ";
//		 cout << "Press any key to countine ...";
//		 system("pause>0");
//		 ShowQuickDrawScreen();
//	 }
//	 cout << "\n Are you sure do you want perforem this transaction : (y/n) : ";
//	 cin >> answer;
//	 if (answer == 'y' || answer == 'Y')
//	 {
//		 CurrentUser.AcountBalance -= Number ;
//		 SaveRecordAfterDeleteClient(FileName, vClient);
//		 cout << "\n Done successfully , New Blance is : " << CurrentUser.AcountBalance;
//	 }
//	 
// }
//
// void ChoosWithDraw(enChooseWithDraw Perferm)
// {
//	
//	 switch (Perferm)
//	 {
//	 case enChooseWithDraw::anOne:
//	 {
//		 Perform1(20);
//		 GoBackToMainMenue();
//	 }
//	 case enChooseWithDraw::enTwo:
//	 {
//		 Perform1(50);
//		 GoBackToMainMenue();
//	 }
//	 case enChooseWithDraw::enTrue:
//	 {
//		 Perform1(100);
//		 GoBackToMainMenue();
//	 }
//	 case enChooseWithDraw::enFour:
//	 {
//		 Perform1(200);
//		 GoBackToMainMenue();
//	 }
//	 case enChooseWithDraw::enFifth:
//	 {
//		 Perform1(400);
//		 GoBackToMainMenue();
//	 }
//	 case enChooseWithDraw::enSix:
//	 {
//		 Perform1(600);
//		 GoBackToMainMenue();
//	 }
//	 case enChooseWithDraw::enSeven:
//	 {
//		Perform1(800);
//		 GoBackToMainMenue();
//	 }
//	 case enChooseWithDraw::enOuit:
//	 {
//		  Perform1(1000);
//		 GoBackToMainMenue();
//	 }
//	 case enChooseWithDraw::enExit:
//	 {
//		 GoBackToMainMenue();
//	 }
//	 }
// }
//
// void ShowQuickDrawScreen()
// {
//	 system("cls");
//	 cout << "\n===================================================";
//	 cout << "\n\t\t Quick WithDraw Screen";
//	 cout << "\n===================================================";
//	 cout << "\n\t [1] 20 \t\t [2] 50\n \t [3] 100 \t\t [4] 200\n \t [5] 400 \t\t [6] 600\n ";
//	 cout << "\t [7] 800 \t\t [8] 1000\n \t [9] Exit \n";
//	 cout << "===================================================";
//	 AcountBalance();
//	 ChoosWithDraw(enChooseWithDraw(ReadWithDraw()));
// }
//
// void NormalWithAdraw()
// {
//	 vector <sClient> vClient = LoadCleintsDataFromFile(FileName);
//	 sClient Client;
//	 char answer = 'n';
//	 int AcountBalance = 0;
//	 cout << "\n Enter an amount multiple of 5 ' s : ";
//	 cin >> AcountBalance;
//	 while (AcountBalance % 5 != 0)
//	 {
//		 cout << "\n Enter an amount multiple of 5 ' s : ";
//		 cin >> AcountBalance; 
//	 }
//	 while (AcountBalance > CurrentUser.AcountBalance)
//	 {
//		 cout << "\n The amount exceeds your balance , Make anouther chosse \n ";
//		 cout << "Press any key to countine ...";
//		 system("pause >0");
//		 system("cls");
//
//		 ShowNormalWithDrawScreen();
//		/* cout << "\n Enter an amount multiple of 5 ' s : ";
//		 cin >> AcountBalance;*/
//	 }
//	 cout << "\n Are you sure do you want perforem this transaction : (y/n) : ";
//	 cin >> answer;
//	 if (answer == 'y' || answer == 'Y')
//	 {
//		 CurrentUser.AcountBalance -= AcountBalance;
//		 SaveRecordAfterDeleteClient(FileName, vClient);
//		 cout << "\n Done successfully , New Blance is : " << CurrentUser.AcountBalance;
//	 }
// }
//
// void ShowNormalWithDrawScreen()
// {
//	 system("cls");
//	 cout << "\n===================================================";
//	 cout << "\n\t\t Normal WithDraw Screen";
//	 cout << "\n===================================================";
//	 NormalWithAdraw();
// }
//
// void ShowTotalBalancesScreen()
// {
//	 cout << "\n===================================================";
//	 cout << "\n\t\t Check Balance Screen ";
//	 cout << "\n===================================================";
//	 vector <sClient> vClient = LoadCleintsDataFromFile(FileName);
//	 sClient Client;
//	
//	 AcountBalance();
// }
//
// void ShowDepositScreen()
// {
//	 cout << "\n===================================================";
//	 cout << "\n\t\t Check Balance Screen ";
//	 cout << "\n===================================================";
//	 int Amount;
//	 vector <sClient> vClient = LoadCleintsDataFromFile(FileName);
//	 cout << "\n Enter a posistve Deposit amount ? ";
//	 cin >> Amount;
//	 char answr;
//	 cout << "\ Are you sure do you want deosit this amunt : ";
//	 cin >> answr;
//	 if (answr == 'Y' || answr == 'y')
//	 {
//		 CurrentUser.AcountBalance += Amount;
//		 SaveRecordAfterDeleteClient(FileName, vClient);
//		 cout << "\n Done successfully , New Blance is : " << CurrentUser.AcountBalance;
//	 }
// }
//
// short ReadATMMenueOption()
// {
//	 cout << "\n Please what do you want to do : [1] to [5] ? ";
//	 short Number;
//	 cin >> Number;
//	 return Number;
// }
//
// void GoBackToMainMenue()
//{
//	cout << "\n\nPress any key to go back to Main Menue...";
//	system("pause>0");
//	ShowMainMenue();
//}
//
// void PerfromAtmMenueOption(ATMMenueOptions TransactionMenueOption)
//{
//	switch (TransactionMenueOption)
//	{
//	case ATMMenueOptions::enQuickDraw:
//	{
//		system("cls");
//		ShowQuickDrawScreen();
//		GoBackToMainMenue();
//		break;
//	}
//	case ATMMenueOptions::enNormalDraw:
//	{
//		system("cls");
//		ShowNormalWithDrawScreen();
//		GoBackToMainMenue();
//		break;
//	}
//	case ATMMenueOptions::enDeposit:
//	{
//		system("cls");
//		ShowDepositScreen();
//		GoBackToMainMenue();
//		break;
//	}
//	case ATMMenueOptions::enCheckPalace:
//	{
//		system("cls");
//		ShowTotalBalancesScreen();
//		GoBackToMainMenue();
//	}
//	case ATMMenueOptions::enLogOut:
//	{
//		Login();
//	}
//	}
//}
//
// void ShowMainMenue()
// {
//
//	 system("cls");
//	 cout << "===========================================\n";
//	 cout << "\t\tATM main Menue Screen\n";
//	 cout << "===========================================\n";
//	 cout << "\t[1] Quick withraw.\n";
//	 cout << "\t[2] Normal wirhraw.\n";
//	 cout << "\t[3] Deposit.\n";
//	 cout << "\t[4] Check Palance.\n";
//	 cout << "\t[5] LogOut.\n";
//	 cout << "===========================================\n";
//	 PerfromAtmMenueOption(ATMMenueOptions(ReadATMMenueOption()));
// }
//
//void Login()
//{
//	bool LoginFaild = false;
//
//	string AcountNumber, PinCode;
//	do
//	{
//		system("cls");
//		cout << "\n-------------------------------------------\n";
//		cout << "\t Login Screen";
//		cout << "\n-------------------------------------------\n";
//
//		if (LoginFaild)
//		{
//			cout << "\n Invalid AcountNumber / PinCode !\n";
//		}
//		cout << "\n Enter Acount Number : ";
//		cin >> AcountNumber;
//		cout << "\n Enter PassWord : ";
//		cin >> PinCode;
//
//		LoginFaild = !LoadClientInfo(AcountNumber, PinCode);
//	} while (LoginFaild);
//
//
//	ShowMainMenue();
//}
//
//int main()
//{
//	Login();
//	system("pause > 0");
//	return 0;
//}