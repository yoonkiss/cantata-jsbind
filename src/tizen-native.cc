#include <v8.h>
#include <node.h>

/*
#include <FSclAddressbookManager.h>
#include <FSclAddressbook.h>
#include <FSclCategory.h>
#include <FSclContact.h>
*/

class Contacts : node::ObjectWrap
{
	public:
	static v8::Persistent<v8::FunctionTemplate> funcTemplate;

	// require에 의해 로드될 때 호출되는 메쏘드
	static void NODE_EXTERN Init( v8::Handle<v8::Object> target )
	{
		v8::HandleScope scope;

		// function() {};
		v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New( New );

		// GC되지 않도록 함
		funcTemplate = v8::Persistent<v8::FunctionTemplate>::New( t );
		funcTemplate->InstanceTemplate() -> SetInternalFieldCount( 1 );

		// 클래스 명 할당( Contacts = function() {}; )
		funcTemplate->SetClassName( v8::String::NewSymbol( "Contacts" ) );

		// Contacts.prototype.list 을 호출하면 List로 Callback
		NODE_SET_PROTOTYPE_METHOD( funcTemplate, "list", List );


		// Contacts.prototype.Contacts 에 생성자 할당
		target -> Set( v8::String::NewSymbol( "Contacts" ), funcTemplate -> GetFunction() );
	}

	// new
	static v8::Handle<v8::Value> New( const v8::Arguments& args )
	{
		v8::HandleScope scope;
		Contacts* contacts = new Contacts();
		contacts -> Wrap( args.This() );
		return args.This();
	}

	// list
	static v8::Handle<v8::Value> List( const v8::Arguments& args )
	{
		v8::HandleScope scope;

		/*
		Tizen::Social::AddressbookManager* addressbookManager = Tizen::Social::AddressbookManager::GetInstance();
		Tizen::Social::Addressbook* addressbook = addressbookManager -> GetAddressbookN( Tizen::Social::DEFAULT_ADDRESSBOOK_ID );

		Tizen::Base::Collection::IList* pCategoryList = addressbook -> GetAllCategoriesN();
		if (pCategoryList == null)
		{
			return;
		}

		Tizen::Social::Category* pCategory = null;
		Tizen::Social::Contact* pContact = null;
		Tizen::Base::Collection::IList* pContactList = null;
		Tizen::Base::Collection::IEnumerator* pContactEnum = null;
		Tizen::Base::String firstName;
		Tizen::Base::String lastName;
		Tizen::Base::Collection::IEnumerator* pCategoryEnum = pCategoryList -> GetEnumeratorN();
		while ( pCategoryEnum->MoveNext() == E_SUCCESS )
		{
			pCategory = static_cast<Tizen::Social::Category*>( pCategoryEnum->GetCurrent() );

			pContactList = addressbook -> GetContactsByCategoryN( pCategory -> GetRecordId() );
			if (pContactList == null)
			{
				continue;
			}

			pContactEnum = pContactList -> GetEnumeratorN();
			while ( pContactEnum->MoveNext() == E_SUCCESS )
			{
				pContact = static_cast<Tizen::Social::Contact*>( pContactEnum->GetCurrent() );

				//Gets the name.
				pContact -> GetValue( Tizen::Social::CONTACT_PROPERTY_ID_FIRST_NAME, firstName );
				pContact -> GetValue( Tizen::Social::CONTACT_PROPERTY_ID_LAST_NAME, lastName );

				// Displays the first name and last name.

			}
			delete pContactEnum;
			pContactList -> RemoveAll ( true);
			delete pContactList;
		}
		pCategoryList -> RemoveAll( true );
		delete pCategoryList;
		*/

		v8::Local<v8::Array> aPhones = v8::Array::New( 2 );
		aPhones -> Set( 0, v8::String::New( "000-0000-0001" ) );
		aPhones -> Set( 1, v8::String::New( "000-0000-0001" ) );
		v8::Local<v8::Object> aContact = v8::Object::New();
		aContact -> Set( v8::String::New( "A" ), aPhones );


		v8::Local<v8::Array> bPhones = v8::Array::New( 1 );
		bPhones -> Set( 0, v8::String::New( "000-0000-0003" ) );
		v8::Local<v8::Object> bContact = v8::Object::New();
		bContact -> Set( v8::String::New( "B" ), bPhones );


		v8::Local<v8::Array> friendsContacts = v8::Array::New( 2 );
		friendsContacts -> Set( 0, aContact );
		friendsContacts -> Set( 1, bContact );
		
		v8::Local<v8::Object> friendsCategory =  v8::Object::New();
		friendsCategory -> Set( v8::String::New( "Native-Friends" ), friendsContacts );



		v8::Local<v8::Array> cPhones = v8::Array::New( 1 );
		cPhones -> Set( 0, v8::String::New( "000-0000-0004" ) );
		v8::Local<v8::Object> cContact = v8::Object::New();
		cContact -> Set( v8::String::New( "C" ), cPhones );

		v8::Local<v8::Array> familyContacts = v8::Array::New( 1 );
		familyContacts -> Set( 0, cContact );
		
		v8::Local<v8::Object> familyCategory =  v8::Object::New();
		familyCategory -> Set( v8::String::New( "Native-Family" ), familyContacts );

		v8::Local<v8::Array> result = v8::Array::New( 2 );
		result -> Set( 0, friendsCategory );
		result -> Set( 1, familyCategory );

		return scope.Close( result );
	}


	Contacts()
	{
	}

	~Contacts()
	{
	}

	
};

v8::Persistent<v8::FunctionTemplate> Contacts::funcTemplate;

extern "C" {
	void NODE_EXTERN init( v8::Handle<v8::Object> target )
	{
		Contacts::Init( target );
	}
	NODE_MODULE( contacts, init );
}


