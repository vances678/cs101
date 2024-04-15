#ifndef _P3_H_
#define _P3_H_

#include <iostream>
#include <string>

using namespace std;

class Info {
public:
    string name;
    string value;
    Info* next;

    Info();
    Info(string name, string value, Info* next = nullptr);
};

class Contact {
public:
    string first;
    string last;
    Contact* next;
    Info* headInfoList;

    Contact();
    Contact(string first, string last, Contact* next = nullptr);
};

class ContactList {
private:
    Contact* headContactList;
    int count;

public:
    ContactList();

    int getCount();

    Contact* findContact(string first, string last);

    void printContact(Contact* contact);

    bool printContact(ostream& os, string first, string last);
    void print(ostream& os);

    bool addContact(string first, string last);
    bool addInfo(string first, string last, string infoName, string infoVal);

    bool addContactOrdered(string first, string last);
    bool addInfoOrdered(string first, string last, string infoName, string infoVal);

    bool removeContact(string first, string last);
    bool removeInfo(string first, string last, string infoName);

    void deleteList();

    ~ContactList();
    void deepCopy(const ContactList& src);
    ContactList(const ContactList& src);
    const ContactList& operator=(const ContactList& src);
};

#endif
