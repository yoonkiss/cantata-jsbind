#include <v8.h>
#include <node.h>
#include <stdlib.h>
#include <iostream>
#include <FIo.h>
#include <FBaseSysLog.h>
#include <FBase.h>
#include <FIo.h>
#include <FSocial.h>

#include "contacts.h"
#include "tizenair-util.h"

using namespace Tizen::Base::Collection;
using namespace Tizen::Social;

void NODE_EXTERN Contacts::Init(v8::Handle<v8::Object> target) {
    AppLog("Entered Contacts::Init");

    v8::Local<v8::FunctionTemplate> funcTemplate = v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(Contacts::New));

    funcTemplate->SetClassName(v8::String::NewSymbol("Contacts"));
    funcTemplate->Set(v8::String::NewSymbol("list"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(list))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("addCategory"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(addCategory))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("removeCategory"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(removeCategory))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("renameCategory"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(renameCategory))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("add"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(add))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("remove"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(remove))->GetFunction());

    target->Set(v8::String::NewSymbol("Contacts"), funcTemplate->GetFunction());
}

v8::Handle<v8::Value> Contacts::New(const v8::Arguments& args) {
    v8::HandleScope scope;
    // nothing to do new()
    return args.This();
}

v8::Handle<v8::Value> Contacts::list(const v8::Arguments& args) {
    AppLog("Entered Contacts::list (args length:%d)", args.Length());

    v8::HandleScope scope;
    AddressbookManager* pAddressbookManager = AddressbookManager::GetInstance();
    IList* pCategoryList = pAddressbookManager->GetAllCategoriesN();
    Category* pCategory = null;
    Contact* pContact = null;
    IEnumerator* pCategoryEnum = pCategoryList->GetEnumeratorN();
    v8::Local<v8::Object> categoryObject = v8::Object::New();

    while (pCategoryEnum->MoveNext() == E_SUCCESS) {
        pCategory = static_cast<Category*>(pCategoryEnum->GetCurrent());
        char category[STRING_MAX];
        IList* pContactList = pAddressbookManager->GetContactsByCategoryN(pCategory->GetRecordId());
        IEnumerator* pContactEnum = pContactList->GetEnumeratorN();
        v8::Local<v8::Array> personList = v8::Array::New();

        AppLog("Fetching category: %ls", pCategory->GetName().GetPointer());
        int person_cnt = 0;
        while (pContactEnum->MoveNext() == E_SUCCESS) {
            pContact = static_cast<Contact*>(pContactEnum->GetCurrent());
            String fullName = L"", firstName, lastName;
            v8::Local<v8::Object> personObject = v8::Object::New();
            char buf[STRING_MAX];

            pContact->GetValue(CONTACT_PROPERTY_ID_FIRST_NAME, firstName);
            pContact->GetValue(CONTACT_PROPERTY_ID_LAST_NAME, lastName);
            fullName.Append(firstName);
            fullName.Append(L" ");
            fullName.Append(lastName);

            AppLog("Getting person: %ls", fullName.GetPointer());
            IList* pPhoneNumberList = pContact->GetValuesN(CONTACT_MPROPERTY_ID_PHONE_NUMBERS);
            v8::Local<v8::Array> phoneList = v8::Array::New();
            if (pPhoneNumberList != null) {
                IEnumerator* pEnum = pPhoneNumberList->GetEnumeratorN();
                int number_cnt = 0;
                while (pEnum->MoveNext() == E_SUCCESS) {
                    PhoneNumber* pPhoneNumber = static_cast<PhoneNumber*>(pEnum->GetCurrent());
                    String number = pPhoneNumber->GetPhoneNumber();
                    AppLog("Getting person's phonenumber: %ls", number.GetPointer());

                    phoneList->Set(number_cnt++, v8::String::New(Util::toAnsi(buf, number.GetPointer(), STRING_MAX)));
                }

                delete pEnum;
                pPhoneNumberList->RemoveAll(true);
                delete pPhoneNumberList;
            }
            personObject->Set(v8::String::New("name"), v8::String::New(Util::toAnsi(buf, fullName.GetPointer(), STRING_MAX)));
            personObject->Set(v8::String::New("phoneNumber"), phoneList);
            personList->Set(person_cnt++, personObject);
        }
        categoryObject->Set(v8::String::New(Util::toAnsi(category, pCategory->GetName(), STRING_MAX)), personList);

        delete pContactEnum;
        pContactList->RemoveAll(true);
        delete pContactList;
    }
   return scope.Close(categoryObject);
}

v8::Handle<v8::Value> Contacts::addCategory(const v8::Arguments& args) {
    AppLog("Entered Contacts::addCategory (args length:%d)", args.Length());

    if (args.Length() < 1 || Util::isArgumentNull(args[0])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;

    Category category;
    category.SetName(UNWRAP_STRING(args[0]).c_str());

    AddressbookManager* pAddressbookManager = AddressbookManager::GetInstance();
    pAddressbookManager->AddCategory(category, DEFAULT_ADDRESSBOOK_ID);

    if (IsFailed(GetLastResult())) {
        return scope.Close(v8::Boolean::New(false));
    } else {
        return scope.Close(v8::Boolean::New(true));
    }
}

/**
 * TODO: what we have to do next if given category is deleted
 */
v8::Handle<v8::Value> Contacts::removeCategory(const v8::Arguments& args) {
    AppLog("Entered Contacts::removeCategory (args length:%d)", args.Length());

    if (args.Length() < 1 || Util::isArgumentNull(args[0])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;

    String category = UNWRAP_STRING(args[0]).c_str();

    AddressbookManager* pAddressbookManager = AddressbookManager::GetInstance();
    IList* pCategoryList = pAddressbookManager->GetAllCategoriesN();

    Addressbook* pAddressbook = pAddressbookManager->GetAddressbookN(DEFAULT_ADDRESSBOOK_ID);
    result r = GetLastResult();
    if (IsFailed(r)) {
        AppLog("Failed to get addressbook: %s", GetErrorMessage(r));
        return scope.Close(v8::Boolean::New(false));
    }
    if (pCategoryList != null && pCategoryList->GetCount() > 0) {
        IEnumerator* pCategoryEnum = pCategoryList->GetEnumeratorN();
        Category* pCategory = null;

        while (pCategoryEnum->MoveNext() == E_SUCCESS) {
            pCategory = static_cast<Category*>(pCategoryEnum->GetCurrent());
            if (pCategory->GetName().Equals(category)) {
                // what's doinn
                r = pAddressbook->RemoveCategory(*pCategory);
                if (IsFailed(r)) {
                    AppLog("Failed to get addressbook: %s", GetErrorMessage(r));
                    return scope.Close(v8::Boolean::New(false));
                } else {
                    AppLog("Deleted addressbook");
                    break;
                }
            }
        }
    }

    return scope.Close(v8::Boolean::New(true));
}

v8::Handle<v8::Value> Contacts::renameCategory(const v8::Arguments& args) {
    AppLog("Entered Contacts::renameCategory (args length:%d)", args.Length());

    if (args.Length() < 2 || Util::isArgumentNull(args[0]) || Util::isArgumentNull(args[1])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;

    String category = UNWRAP_STRING(args[0]).c_str();
    String newcategory = UNWRAP_STRING(args[1]).c_str();

    AddressbookManager* pAddressbookManager = AddressbookManager::GetInstance();
    IList* pCategoryList = pAddressbookManager->GetAllCategoriesN();

    Addressbook* pAddressbook = pAddressbookManager->GetAddressbookN(DEFAULT_ADDRESSBOOK_ID);
    result r = GetLastResult();
    if (IsFailed(r)) {
        AppLog("Failed to get addressbook: %s", GetErrorMessage(r));
        return scope.Close(v8::Boolean::New(false));
    }
    if (pCategoryList != null && pCategoryList->GetCount() > 0) {
        IEnumerator* pCategoryEnum = pCategoryList->GetEnumeratorN();
        Category* pCategory = null;

        while (pCategoryEnum->MoveNext() == E_SUCCESS) {
            pCategory = static_cast<Category*>(pCategoryEnum->GetCurrent());
            if (pCategory->GetName().Equals(category)) {
            	pCategory->SetName(newcategory);

                r = pAddressbook->UpdateCategory(*pCategory);
                if (IsFailed(r)) {
                    AppLog("Failed to get addressbook: %s", GetErrorMessage(r));
                    return scope.Close(v8::Boolean::New(false));
                } else {
                    AppLog("Deleted addressbook");
                    break;
                }
            }
        }
    }

    return scope.Close(v8::Boolean::New(true));
}

v8::Handle<v8::Value> Contacts::add(const v8::Arguments& args) {
    AppLog("Entered Contacts::renameCategory (args length:%d)", args.Length());

    if (args.Length() < 2 || Util::isArgumentNull(args[0]) || Util::isArgumentNull(args[1])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;

    AddressbookManager* pAddressbookManager = AddressbookManager::GetInstance();

    String category = UNWRAP_STRING(args[0]).c_str();
    String firstname = UNWRAP_STRING(args[1]).c_str();
    String lastname = UNWRAP_STRING(args[2]).c_str();

    Contact contact;

    contact.SetValue(CONTACT_PROPERTY_ID_FIRST_NAME, firstname);
    contact.SetValue(CONTACT_PROPERTY_ID_LAST_NAME, lastname);

    if(Util::isArgumentNull(args[3]))
    {
    	String num1 = UNWRAP_STRING(args[3]).c_str();
    	contact.SetValue(CONTACT_MPROPERTY_ID_PHONE_NUMBERS, num1);
    }

    if(Util::isArgumentNull(args[4]))
	{
		String num2 = UNWRAP_STRING(args[4]).c_str();
		contact.SetValue(CONTACT_MPROPERTY_ID_PHONE_NUMBERS, num2);
	}

    //FIND CATEGORY TO ADD AS A MEMBER
    IList* pCategoryList = pAddressbookManager->GetAllCategoriesN();

	Addressbook* pAddressbook = pAddressbookManager->GetAddressbookN(DEFAULT_ADDRESSBOOK_ID);

	result r = GetLastResult();
	if (IsFailed(r)) {
		AppLog("Failed to get addressbook: %s", GetErrorMessage(r));
		return scope.Close(v8::Boolean::New(false));
	}

	if (pCategoryList != null && pCategoryList->GetCount() > 0) {
		IEnumerator* pCategoryEnum = pCategoryList->GetEnumeratorN();
		Category* pCategory = null;

		while (pCategoryEnum->MoveNext() == E_SUCCESS) {
			pCategory = static_cast<Category*>(pCategoryEnum->GetCurrent());
			if (pCategory->GetName().Equals(category)) {

				pAddressbookManager->AddContact(contact, DEFAULT_ADDRESSBOOK_ID);
				pAddressbookManager->AddMemberToCategory(*pCategory->GetRecordId(), contact.GetRecordId());

				  if (IsFailed(GetLastResult())) {
					   return scope.Close(v8::Boolean::New(false));
				   } else {
					   return scope.Close(v8::Boolean::New(true));
				   }
			}
		}
	}
}

//WHAT IS THE UNIQUE KEY? PERSON ID? CONTACT ID?
v8::Handle<v8::Value> Contacts::remove(const v8::Arguments& args) {
    AppLog("Entered Contacts::renameCategory (args length:%d)", args.Length());

    if (args.Length() < 2 || Util::isArgumentNull(args[0])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;

    AddressbookManager* pAddressbookManager = AddressbookManager::GetInstance();

    String personId = UNWRAP_STRING(args[0]).c_str();
    String name = UNWRAP_STRING(args[1]).c_str();

    IList* pContacts  = pAddressbookManager->SearchContactsByNameN(name);

    if (pContacts != null && pContacts->GetCount() > 0) {
    		IEnumerator* pContactEnum = pContacts->GetEnumeratorN();
    		Contact* pContact = null;

    		while (pContactEnum->MoveNext() == E_SUCCESS) {
    			pContact = static_cast<Category*>(pContactEnum->GetCurrent());
    			if (pContact->GetPersonId().Equals(personId)) {
    				pAddressbookManager->RemoveContact(pContact->GetRecordId());

    				  if (IsFailed(GetLastResult())) {
    					   return scope.Close(v8::Boolean::New(false));
    				   } else {
    					   return scope.Close(v8::Boolean::New(true));
    				   }
    			}
    		}
    	}
}

