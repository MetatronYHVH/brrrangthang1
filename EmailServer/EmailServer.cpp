// EmailServer.cpp
// [Evan Davis]
// [07/30/17]
// Project 1
// [This program is an email server]
//*************************************************************
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "stdafx.h"

void MainMenu();
void Login();
void ShowInbox(std::string username, std::ostream & sout);
void LoggedInMenu(std::string username);
void RegisterNewUser();
void DeleteUser();
std::string GetUserFile(std::string username);
std::string GetInboxFile(std::string username);
std::string GetLine(std::istream & sin);
bool DoesUserExist(std::string username);
int GetInboxCount(std::string username);
std::string FormatEmailString(std::string from, std::string to, std::string subject, std::string msg);
void SendEmail(std::string username);
std::string GetPassword(std::string username);



//@include(main)
//@addRule(commentAll)
int main()
{

	MainMenu();
	std::cin.get();
	return 0;


	// make sure your int main function returns an int
}
//@removeRule(commentAll)

void MainMenu()
{
	std::string input = "";
	int menuItem = 0;

	while (input != "99")
	{
		std::cout << " 1) Login" << "\n";
		std::cout << " 2) Register new user" << "\n";
		std::cout << " 3) Delete user" << "\n";
		std::cout << "99) Exit" << "\n";
		std::cout << "Selection: ";

		std::cin >> input;
		try
		{
			menuItem = stoi(input);
			switch (menuItem)
			{
			case 1:
				Login();
				break;
			case 2:
				RegisterNewUser();
				break;
			case 3:
				DeleteUser();
				break;
			}
		}
		catch (std::invalid_argument & ia)
		{
			std::cout << "Bad option!" << std::endl;
			std::cout << ia.what() << std::endl;
		}
	}
}

// Now you fill in the rest
void Login()
{
	std::string username;
	std::string password;

	std::string user;
	std::string pwd;

	std::cout << "Enter username: ";
	std::cin >> username;

	if (!DoesUserExist(username))
	{
		std::cout << "User not found\n";
		return;
	}

	// now, prompt for their password, and see if the password
	// in the file matches the one they enter.

	std::cout << "Enter a password:";
	std::cin >> password;

	// use getPassword for this user to see if they match

	if (password == GetPassword(username))
	{
		LoggedInMenu(username);
	}
	else
	{
		std::cout << "Incorrect password...\n";
		return;
	}
}//end login

void ShowInbox(std::string username, std::ostream & sout)
{

	// while the user is LoggedIn
	// std::cout FileInbox
	//std::cin.ignore();
	std::string header;
	std::string from;
	std::string to;
	std::string subject;
	std::string msg;
	std::string line;
	std::ifstream UserInboxFile(username);
	if (UserInboxFile.is_open())
	{
		while (!UserInboxFile.eof())
		{
			header = GetLine(UserInboxFile);
			if (header == "#email")
			{
				from = GetLine(UserInboxFile);
				to = GetLine(UserInboxFile);
				subject = GetLine(UserInboxFile);
				msg = GetLine(UserInboxFile);
				sout << FormatEmailString(from, to, subject, msg);
			}
		}
		UserInboxFile.close();
	}
	else std::cout << "Unable to open file";
}

void LoggedInMenu(std::string username)
{

	int selection;

	do {
		std::cout << " 1) Inbox[" << GetInboxCount(username) << "]\n";
		std::cout << " 2) Send Email\n";
		std::cout << "99) Logout\n";
		std::cout << "Selection ";
		std::cin >> selection;

		switch (selection)
		{
		case 1:
			ShowInbox(GetInboxFile(username), std::cout);
			break;

		case 2:
			SendEmail(username);
			break;

		case 99:
			return;

		}
	} while (selection != 99);

}

void RegisterNewUser()
{
	std::string username; // holds the name of the user we're trying to add
	std::string password; // holds the password of the user we want to add

	std::string user; // for reading in from the 
	std::string pwd;  // list of EXISTING users

	std::cin.ignore();
	std::cout << "Enter username: ";
	std::cin >> username;

	// check username length here.  If it's not at least 3, return
	if (username.length() < 3)
	{
		std::cout << "Invalid user name.  Must be at least 3 characters\n";
		return;
	}

	// Check here to see if the user already exists, before we prompt
	// for the password
	if (DoesUserExist(username))
	{
		std::cerr << "User already exists!\n";
		return;
	}


	std::cin.ignore();
	std::cout << "Enter password: ";
	std::cin >> password;

	if (password.length() < 6)
	{
		std::cout << "Invalid user password.  Must be at least 6 characters\n";
		return;
	}

	// At this point, we have prompted for the username, and made sure
	// it was at least 3 characters.  Then we made sure it wasn't already
	// in the EXISTING abc.txt file.  Then we prompted for the password,
	// and made sure it was at least 6 characters long.
	//
	// NOW we can add this username/password pair to the anc.txt file.
	std::string filename = GetUserFile(username);
	std::ofstream usersFileOut(filename, std::ofstream::out | std::ofstream::app); // adds new information to existing file

	if (!usersFileOut.is_open()) // if user file is not open
	{
		std::cerr << "ERROR: File is not open\n"; // display error through console
		return;
	}

	// Put this user's name and password in their file.
	usersFileOut << "username " << username << "\n" <<
		"password " << password << "\n";

	// Start them with their first "welcome" e-mail.
	filename = GetInboxFile(username);
	std::ofstream usersFileInboxOut(filename);
	usersFileInboxOut << "#email\nmailservice\n" << username <<
		"\nIntro\nHello," << username << "! Welcome to our service\n";

	usersFileInboxOut.close();
	usersFileOut.close();
}

void DeleteUser()
{
	std::cin.ignore();
	std::string username;
	std::cout << "Enter name of user to delete: ";
	std::cin >> username;

	// verify user exists
	// if user exists, use delete function to delete username file 
	// including username-inbox file
	if (DoesUserExist(username))
	{
		remove(GetUserFile(username).c_str());
		remove(GetInboxFile(username).c_str());
	}
	else
	{
		std::cout << "User doesn't exist\n ";
	}
}

std::string GetUserFile(std::string username)
{
	return username + ".txt";
}

std::string GetInboxFile(std::string username)
{
	return username + "-inbox.txt";
}

std::string GetLine(std::istream & sin)
{
	char line[256];
	sin.getline(line, 256);
	return line;
}

bool DoesUserExist(std::string username)
{
	// If a user exists, then we've created them a username.txt file.

	std::ifstream usersFileIn(username + ".txt");

	// if we can't open the file, then we've never created the user.
	if (!usersFileIn.is_open()) return false;

	// If we get this far, then the user DOES exist (because we just
	//  opened their file), so we want to CLOSE it and return true

	usersFileIn.close();
	return true;
}

int GetInboxCount(std::string username)
{
	int emailCount = 0;

	// Read through username's mailbox.  Every time we come across a line
	//  of "#email", that's the start of a new email message.  Count 'em!
	//
	std::ifstream mailbox(GetInboxFile(username));
	while (!mailbox.eof())
	{
		std::string lineRead = GetLine(mailbox);
		if (lineRead == "#email")
			emailCount++;
	}
	mailbox.close();
	return emailCount;
}

std::string FormatEmailString(std::string from, std::string to, std::string subject, std::string msg)
{
	// here the email information is formatted
	// from to subject and message all added
	return "\nFrom: " + from + "\n" +
		"To: " + to + "\n" +
		"Subject: " + subject + "\n" +
		"Message: " + msg + "\n";
}

void SendEmail(std::string username)
{
	std::string toUsername;
	std::string message;
	std::cin.ignore();
	std::string subject;
	std::cout << "Send email to who: ";
	toUsername = GetLine(std::cin);

	if (!DoesUserExist(toUsername))
	{
		std::cout << "User " << toUsername << " does not exist\n";
		return;
	}

	std::cout << "Subject: ";
	subject = GetLine(std::cin);

	std::cout << "Message: ";
	message = GetLine(std::cin);
	// After strings are input by the user
	// The information is then "sent" to the desired user-inbox
	std::ofstream fout(GetInboxFile(toUsername), std::ofstream::out | std::ofstream::app);
	// here the information is sent to the user stream
	// details such as from, to, subject, and actual message
	fout << "#email\n" <<
		username << "\n" <<
		toUsername << "\n" <<
		subject << "\n" <<
		message << "\n";


	fout.close();

}

std::string GetPassword(std::string username)
{
	// We will always check to make sure that the username exists before
	// we try to get the password FOR the username, so we know that
	// username.txt exists.  All we have to do is open that file, and
	// read the information.  The format of the file is:
	// "username" <username>
	// "password" <password>
	// So, we want the fourth string from that file.

	std::string pwd;
	std::ifstream usersFileIn(GetUserFile(username));

	usersFileIn >> pwd >> pwd >> pwd >> pwd;
	// If we get this far, then we did NOT find the specified user
	usersFileIn.close();


	return pwd;
}
