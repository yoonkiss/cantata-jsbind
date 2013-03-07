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

#define STRING_MAX 128

void NODE_EXTERN Contacts::Init(v8::Handle<v8::Object> target) {
    AppLog("Entered Contacts::Init");

    v8::Local<v8::FunctionTemplate> funcTemplate = v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(Contacts::New));

    funcTemplate->SetClassName(v8::String::NewSymbol("Contacts"));
    funcTemplate->Set(v8::String::NewSymbol("list"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(list))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("addCategory"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(addCategory))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("removeCategory"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(removeCategory))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("renameCategory"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(renameCategory))->GetFunction());
    funcTemplate->Set(v8::String::NewSymbol("isExistCategory"), v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(isExistCategory))->GetFunction());
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
	AppLogTag("Contacts", "Entered Contacts::list (args length:%d)", args.Length());

    v8::HandleScope scope;
    AddressbookManager* pAddressbookManager = AddressbookManager::GetInstance();
    IList* pCategoryList = pAddressbookManager->GetAllCategoriesN();
    AppLogTag("Contacts", "TEST1");
    Category* pCategory = null;
    Contact* pContact = null;
    AppLogTag("Contacts", "TEST2");
    IEnumerator* pCategoryEnum = pCategoryList->GetEnumeratorN();
    v8::Local<v8::Object> categoryObject = v8::Object::New();

    while (pCategoryEnum->MoveNext() == E_SUCCESS) {
    	AppLogTag("Contacts", "TEST");
        pCategory = static_cast<Category*>(pCategoryEnum->GetCurrent());
        char category[STRING_MAX];
        IList* pContactList = pAddressbookManager->GetContactsByCategoryN(pCategory->GetRecordId());
        IEnumerator* pContactEnum = pContactList->GetEnumeratorN();
        v8::Local<v8::Array> personList = v8::Array::New();

        AppLogTag("Contacts", "Fetching category: %ls", pCategory->GetName().GetPointer());
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

            AppLogTag("Contacts", "Getting person: %ls", fullName.GetPointer());
            IList* pPhoneNumberList = pContact->GetValuesN(CONTACT_MPROPERTY_ID_PHONE_NUMBERS);
            v8::Local<v8::Array> phoneList = v8::Array::New();
            if (pPhoneNumberList != null) {
                IEnumerator* pEnum = pPhoneNumberList->GetEnumeratorN();
                int number_cnt = 0;
                while (pEnum->MoveNext() == E_SUCCESS) {
                    PhoneNumber* pPhoneNumber = static_cast<PhoneNumber*>(pEnum->GetCurrent());
                    String number = pPhoneNumber->GetPhoneNumber();
                    AppLogTag("Contacts", "Getting person's phonenumber: %ls", number.GetPointer());

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
	AppLogTag("Contacts", "Entered Contacts::addCategory (args length:%d)", args.Length());

//	if (args.Length() < 1 || Util::isArgumentNull(args[0])) {
    if (args.Length() < 1) {
    	AppLogTag("Contacts", "Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;

    String newCategory = null;
    if(args[0]->IsString())
    {
    	newCategory = UNWRAP_STRING(args[0]).c_str();
    	AppLogTag("Contacts","Now add new Category:%ls", newCategory.GetPointer());
    }

    if(newCategory == null)
    {
    	AppLogTag("Contacts","category name is null");
    }

    Category category;
    category.SetName(newCategory);

    AddressbookManager* pAddressbookManager = AddressbookManager::GetInstance();
    Addressbook* pAddressbook = pAddressbookManager->GetAddressbookN(DEFAULT_ADDRESSBOOK_ID);

    pAddressbook->AddCategory(category);

    if (IsFailed(GetLastResult())) {
        return scope.Close(v8::Boolean::New(false));
    } else {
        return scope.Close(v8::Boolean::New(true));
    }
}

v8::Handle<v8::Value> Contacts::removeCategory(const v8::Arguments& args) {
    AppLog("Entered Contacts::removeCategory (args length:%d)", args.Length());

    if (args.Length() < 2 || Util::isArgumentNull(args[0]) || Util::isArgumentNull(args[1])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;

    String category = UNWRAP_STRING(args[0]).c_str();
    String force = UNWRAP_STRING(args[1]).c_str();

    AddressbookManager* pAddressbookManager = AddressbookManager::GetInstance();
    Addressbook* pAddressbook = pAddressbookManager->GetAddressbookN(DEFAULT_ADDRESSBOOK_ID);

    IList* pCategoryList = pAddressbook->GetAllCategoriesN();

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

                r = pAddressbook->RemoveCategory(*pCategory);
                if (IsFailed(r)) {
                    AppLog("Fail to remove category: %s", GetErrorMessage(r));
                    return scope.Close(v8::Boolean::New(false));
                } else {
                    AppLog("Succeed to remove category");
                    break;
                }
            }
        }
    }

    return scope.Close(v8::Boolean::New(true));
}

v8::Handle<v8::Value> Contacts::isExistCategory(const v8::Arguments& args) {
	AppLogTag("Contacts", "Entered Contacts::isExistCategory (args: length:%d)", args.Length());

    if (args.Length() < 1 || Util::isArgumentNull(args[0])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    String name = null;
    v8::HandleScope scope;
    if(args[0]->IsString())
    {
    	name = UNWRAP_STRING(args[0]).c_str();
    	AppLogTag("Contacts","check Category:%ls", name.GetPointer());
    }

    if(name == null)
    {
    	AppLogTag("Contacts","category name is null");
    	return scope.Close(v8::Boolean::New(false));
    }

    AddressbookManager* pAddressbookManager = AddressbookManager::GetInstance();
    Addressbook* pAddressbook = pAddressbookManager->GetAddressbookN(DEFAULT_ADDRESSBOOK_ID);

    IList* pCategoryList = pAddressbook->GetAllCategoriesN();

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
            if (pCategory->GetName().Equals(name)) {
            	AppLog("It is existed category");
            	return scope.Close(v8::Boolean::New(true));
            }
        }
    }

    AppLog("Non-exist category");
    return scope.Close(v8::Boolean::New(false));
}

v8::Handle<v8::Value> Contacts::renameCategory(const v8::Arguments& args) {
    AppLog("Entered Contacts::renameCategory (args length:%d)", args.Length());

    if (args.Length() < 2 || Util::isArgumentNull(args[0])) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;

    String category = null;
    String newcategory = null;
    if(args[0]->IsString())
    {
    	category = UNWRAP_STRING(args[0]).c_str();
    	AppLogTag("Contacts","old Category:%ls", category.GetPointer());
    }
    if(args[1]->IsString())
    {
    	newcategory = UNWRAP_STRING(args[1]).c_str();
    	AppLogTag("Contacts","new Category:%ls", newcategory.GetPointer());
    }

    if(category == null || newcategory == null)
	{
		AppLogTag("Contacts","category name is null");
		return scope.Close(v8::Boolean::New(false));
	}

    AddressbookManager* pAddressbookManager = AddressbookManager::GetInstance();
    Addressbook* pAddressbook = pAddressbookManager->GetAddressbookN(DEFAULT_ADDRESSBOOK_ID);

    IList* pCategoryList = pAddressbookManager->GetAllCategoriesN();

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
                    AppLog("Fail to update category: %s", GetErrorMessage(r));
                    return scope.Close(v8::Boolean::New(false));
                } else {
                    AppLog("Succeed to update category");
                    break;
                }
            }
        }
    }

    return scope.Close(v8::Boolean::New(true));
}

//TODO: Is it possible to set additional information?
v8::Handle<v8::Value> Contacts::add(const v8::Arguments& args) {
    AppLog("Entered Contacts::addContact (args length:%d)", args.Length());

//    if (args.Length() < 1 || Util::isArgumentNull(args[0])) {
    if (args.Length() < 1) {
        AppLog("Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;

    String category;
	String name;
	String number;

	v8::Local<v8::Object> obj = args[0]->ToObject();
    if(args[0]->IsObject())
    {
		v8::Local<v8::Value> obj_category = obj->Get(v8::String::New("category"));
		if(obj_category->IsString())
		{
			category = UNWRAP_STRING(obj_category).c_str();
			AppLogTag("Contacts","Add Contacts: [Category:%ls]", category.GetPointer());
		}

		v8::Local<v8::Value> obj_name = obj->Get(v8::String::New("name"));
		if(obj_name->IsString())
		{
			name = UNWRAP_STRING(obj_name).c_str();
			AppLogTag("Contacts","Add Contacts: [Name:%ls]", name.GetPointer());
		}
	}

    if(category == null || name == null)
    {
    	AppLogTag("Contacts","Failed to add Contact");
    	return scope.Close(v8::Boolean::New(false));
    }

    //CREATE CONTACT
    Contact contact;
    contact.SetValue(CONTACT_PROPERTY_ID_FIRST_NAME, name);

    v8::Local<v8::Value> obj_number = obj->Get(v8::String::New("number"));

	if(!obj_number->IsNull() && obj_number->IsString())
	{
		number = UNWRAP_STRING(obj_number).c_str();
		AppLogTag("Contacts","Add Contacts: [Number:%ls]", number.GetPointer());
		PhoneNumber phoneNumber;
		phoneNumber.SetPhoneNumber(number);
		contact.AddPhoneNumber(phoneNumber);
	}

	AddressbookManager* pAddressbookManager = AddressbookManager::GetInstance();
	Addressbook* pAddressbook = pAddressbookManager->GetAddressbookN(DEFAULT_ADDRESSBOOK_ID);

    //GET CATEGORIES TO ADD A CONTACT
	IList* pCategoryList = pAddressbook->GetAllCategoriesN();

	result r = GetLastResult();
	if (IsFailed(r)) {
		AppLogTag("Contacts", "Failed to get categories: %s", GetErrorMessage(r));
		return scope.Close(v8::Boolean::New(false));
	}

	if (pCategoryList != null && pCategoryList->GetCount() > 0) {
		IEnumerator* pCategoryEnum = pCategoryList->GetEnumeratorN();
		Category* pCategory = null;

		while (pCategoryEnum->MoveNext() == E_SUCCESS) {
			pCategory = static_cast<Category*>(pCategoryEnum->GetCurrent());
			if (pCategory->GetName().Equals(category)) {

				pAddressbook->AddContact(contact);
				pAddressbook->AddMemberToCategory(pCategory->GetRecordId(), contact.GetRecordId());

				if (IsFailed(GetLastResult())) {
					return scope.Close(v8::Boolean::New(false));
				} else {
					AppLogTag("Contacts", "%d", contact.GetRecordId());
					return scope.Close(v8::Boolean::New(true));
				}
			}
		}
	}

	AppLogTag("Contacts","No Categories");
	return scope.Close(v8::Boolean::New(false));
}

//CONTACT ID
v8::Handle<v8::Value> Contacts::remove(const v8::Arguments& args) {
	AppLogTag("Contacts", "Entered Contacts::renameCategory (args length:%d)", args.Length());

//	if (args.Length() < 1 || Util::isArgumentNull(args[0])) {
    if (args.Length() < 1 ) {
    	AppLogTag("Contacts", "Bad parameters");
        return v8::ThrowException(v8::String::New("Bad parameters"));
    }
    v8::HandleScope scope;

    int contactId;
    if(args[0]->IsInt32())
	{
    	contactId =  args[0]->Int32Value();
		AppLogTag("Contacts","contactId :%d", contactId);
	}

    AddressbookManager* pAddressbookManager = AddressbookManager::GetInstance();
    Addressbook* pAddressbook = pAddressbookManager->GetAddressbookN(DEFAULT_ADDRESSBOOK_ID);

    pAddressbook->RemoveContact(contactId);
	if (IsFailed(GetLastResult())) {
		AppLogTag("Contacts", "Fail to remove contact");
		return scope.Close(v8::Boolean::New(false));
	} else {
		AppLogTag("Contacts", "Succeed to remove contact");
		return scope.Close(v8::Boolean::New(true));
	}
}


