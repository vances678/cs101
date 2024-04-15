#include "p3.h"

#include <iostream>
#include <string>

using namespace std;

Info::Info() {
    this->name = "No Name Set";
    this->value = "No Value Set";
    this->next = nullptr;
}

Info::Info(string name, string value, Info* next) {
    this->name = name;
    this->value = value;
    this->next = next;
}

Contact::Contact() {
    this->first = "No First Set";
    this->last = "No Last Set";
    this->next = nullptr;
    this->headInfoList = nullptr;
}

Contact::Contact(string first, string last, Contact* next) {
    this->first = first;
    this->last = last;
    this->next = next;
    this->headInfoList = nullptr;
}

ContactList::ContactList() {
    this->headContactList = nullptr;
    this->count = 0;
}

int ContactList::getCount() {
    return this->count;
}

Contact* ContactList::findContact(string first, string last) {
    Contact* contact = this->headContactList;
    while (contact != nullptr) {
        if (contact->first == first && contact->last == last) {
            return contact;
        }
        contact = contact->next;
    }
    return nullptr;
}

void ContactList::printContact(Contact* contact) {
    cout << "Contact Name: " << contact->first << " " << contact->last << endl;
    Info* info = contact->headInfoList;
    while (info != nullptr) {
        cout << "    " << info->name << " | " << info->value << endl;
        info = info->next;
    }
}

// print the specified contact and its information
// 1. return false and print nothing if the contact is not in the list
// 2. otherwise return true and print the contact
bool ContactList::printContact(ostream& os, string first, string last) {
    Contact* contact = findContact(first, last);
    if (contact == nullptr) {
        return false;
    }
    printContact(contact);
    return true;
}

// print all contacts and their information
// print nothing if the list is empty
void ContactList::print(ostream& os) {
    Contact* contact = this->headContactList;
    while (contact != nullptr) {
        printContact(contact);
        contact = contact->next;
    }
}

// add a contact to the back of the list
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the back of the list
// - do not forget to update count
bool ContactList::addContact(string first, string last) {
    if (this->headContactList == nullptr) {
        this->headContactList = new Contact(first, last);
    } else {
        Contact* contact = this->headContactList;
        while (contact != nullptr) {
            if (contact->first == first && contact->last == last) {
                return false;
            }
            if (contact->next == nullptr) {
                contact->next = new Contact(first, last);
                break;
            }
            contact = contact->next;
        }
    }
    this->count++;
    return true;
}

// add info to the back of a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and return true
// 3. otherwise add the info to the back of the contact's list and return true
bool ContactList::addInfo(string first, string last, string infoName, string infoVal) {
    Contact* contact = findContact(first, last);
    if (contact == nullptr) {
        return false;
    }
    if (contact->headInfoList == nullptr) {
        contact->headInfoList = new Info(infoName, infoVal);
    } else {
        Info* info = contact->headInfoList;
        while (info != nullptr) {
            if (info->name == infoName) {
                info->value = infoVal;
                return true;
            }
            if (info->next == nullptr) {
                info->next = new Info(infoName, infoVal);
                break;
            }
            info = info->next;
        }
    }
    return true;
}

// add a contact to the list in ascending order by last name
//     if last names are equal, then order by first name ascending
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the list
// - do not forget to update count
// - compare strings with the built-in comparison operators: <, >, ==, etc.
// - a compare method/function is recommended
bool ContactList::addContactOrdered(string first, string last) {
    if (this->headContactList == nullptr) {
        this->headContactList = new Contact(first, last);
    } else {
        Contact* prevContact = nullptr;
        Contact* contact = this->headContactList;
        while (contact != nullptr) {
            if (contact->last.compare(last) > 0 || (contact->last.compare(last) == 0 && contact->first.compare(first) > 0)) {
                if (prevContact == nullptr) {
                    this->headContactList = new Contact(first, last, this->headContactList);
                } else {
                    prevContact->next = new Contact(first, last, prevContact->next);
                }
                break;
            } else if (contact->last.compare(last) == 0) {
                if (contact->first == first) {
                    return false;
                }
            }
            if (contact->next == nullptr) {
                contact->next = new Contact(first, last);
                break;
            }
            prevContact = contact;
            contact = contact->next;
        }
    }
    this->count++;
    return true;
}

// add info to a contact's info list in ascending order by infoName
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and return true
// 3. otherwise add the info to the contact's list and return true
bool ContactList::addInfoOrdered(string first, string last, string infoName, string infoVal) {
    Contact* contact = findContact(first, last);
    if (contact == nullptr) {
        return false;
    }
    if (contact->headInfoList == nullptr) {
        contact->headInfoList = new Info(infoName, infoVal);
    } else {
        Info* prevInfo = nullptr;
        Info* info = contact->headInfoList;
        while (info != nullptr) {
            if (info->name > infoName) {
                if (prevInfo == nullptr) {
                    contact->headInfoList = new Info(infoName, infoVal, contact->headInfoList);
                } else {
                    prevInfo->next = new Info(infoName, infoVal, prevInfo->next);
                }
                break;
            } else if (info->name == infoName) {
                info->value = infoVal;
                break;
            }
            if (info->next == nullptr) {
                info->next = new Info(infoName, infoVal);
                break;
            }
            prevInfo = info;
            info = info->next;
        }
    }
    return true;
}

// remove the contact and its info from the list
// 1. return false and do nothing if the contact is not in the list
// 2. otherwise return true and remove the contact and its info
// - do not forget to update count
bool ContactList::removeContact(string first, string last) {
    Contact* prevContact = nullptr;
    Contact* contact = this->headContactList;
    while (contact != nullptr) {
        if (contact->first == first && contact->last == last) {
            if (prevContact == nullptr) {
                this->headContactList = contact->next;
            } else {
                prevContact->next = contact->next;
            }
            delete contact;
            this->count--;
            return true;
        }
        prevContact = contact;
        contact = contact->next;
    }
    return false;
}

// remove the info from a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. return false and do nothing if the info is not in the contact's info list
// 3. otherwise return true and remove the info from the contact's list
bool ContactList::removeInfo(string first, string last, string infoName) {
    Contact* contact = findContact(first, last);
    if (contact == nullptr) {
        return false;
    }
    Info* prevInfo = nullptr;
    Info* info = contact->headInfoList;
    while (info != nullptr) {
        if (info->name == infoName) {
            if (prevInfo == nullptr) {
                contact->headInfoList = info->next;
            } else {
                prevInfo->next = info->next;
            }
            delete info;
            return true;
        }
        prevInfo = info;
        info = info->next;
    }
    return false;
}

void ContactList::deleteList() {
    Contact* contact = this->headContactList;
    while (contact != nullptr) {
        Info* info = contact->headInfoList;
        while (info != nullptr) {
            Info* temp = info->next;
            delete info;
            info = temp;
        }
        Contact* temp = contact->next;
        delete contact;
        contact = temp;
    }
    this->count = 0;
    this->headContactList = nullptr;
}

// destroy the list by removing all contacts and their infos
ContactList::~ContactList() {
    this->deleteList();
}

void ContactList::deepCopy(const ContactList& src) {
    this->count = src.count;
    Contact* srcContact = src.headContactList;
    Contact* prevContact = nullptr;
    Contact* contact = nullptr;
    while (srcContact != nullptr) {
        contact = new Contact(srcContact->first, srcContact->last);
        Info* srcInfo = srcContact->headInfoList;
        Info* prevInfo = nullptr;
        Info* info = nullptr;
        while (srcInfo != nullptr) {
            info = new Info(srcInfo->name, srcInfo->value);
            if (prevInfo == nullptr) {
                contact->headInfoList = info;
            } else {
                prevInfo->next = info;
            }
            prevInfo = info;
            srcInfo = srcInfo->next;
        }
        if (prevContact == nullptr) {
            this->headContactList = contact;
        } else {
            prevContact->next = contact;

        }
        prevContact = contact;
        srcContact = srcContact->next;
    }
}

// deep copy the source list
// - do not forget to update count
ContactList::ContactList(const ContactList& src) {
    deepCopy(src);
}

// remove all contacts and their info then deep copy the source list
// - do not forget to update count
const ContactList& ContactList::operator=(const ContactList& src) {
    if (this != &src) {
        this->deleteList();
        deepCopy(src);
    }
    return *this;
}
