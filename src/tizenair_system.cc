#include <iostream>
#include <fstream>
#include <v8.h>
#include <node.h>
#include <sys/stat.h>
#include <FBase.h>
#include <FSystem.h>
#include <FSocial.h>
#include <FTelephony.h>

#include "tizenair-util.h"
#include "tizenair_system.h"

void NODE_EXTERN System::Init(v8::Handle<v8::Object> target) {
    AppLog("entered System::Init");

    v8::HandleScope scope;
    v8::Local<v8::FunctionTemplate> funcTemplate = v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(System::New));
    v8::Local<v8::FunctionTemplate> _getStorage;

    //funcTemplate->InstanceTemplate() -> SetInternalFieldCount( 1 );
    funcTemplate->SetClassName(v8::String::NewSymbol("System"));

    //NODE_SET_PROTOTYPE_METHOD(funcTemplate, "getStorage", getStorage);
    funcTemplate -> Set( v8::String::NewSymbol( "getStorage" ), ( _getStorage = v8::Local<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( getStorage ) ) ) -> GetFunction() );
    //target->Set(v8::String::NewSymbol("getStorage"), v8::FunctionTemplate::New(getStorage)->GetFunction());
    target->Set( v8::String::NewSymbol( "System" ), funcTemplate -> GetFunction() );
}


v8::Handle<v8::Value> System::New(const v8::Arguments& args) {
    v8::HandleScope scope;
    // nothing to do new()
    return args.This();
}

v8::Handle<v8::Value> System::getStorage(const v8::Arguments& args) {
    AppLog("entered System::getStorage");

/*
    long long storage_internal_available; //in bytes
    long long storage_internal_available_media;
    long long storage_internal_audio_allocated;
    long long storage_internal_application_allocated;
    long long storage_internal_download_allocated;
    long long storage_internal_image_allocated;
    long long storage_internal_video_allocated;

    Tizen::System::RuntimeInfo::GetValue(L"http://tizen.org/runtime/storage.available.internal",
            storage_internal_available);
    Tizen::System::RuntimeInfo::GetValue(
            L"http://tizen.org/runtime/storage.available.internal.media",
            storage_internal_available_media);

    Tizen::System::RuntimeInfo::GetValue(
            L"http://tizen.org/runtime/storage.allocated.internal.audio",
            storage_internal_audio_allocated);
    Tizen::System::RuntimeInfo::GetValue(
            L"http://tizen.org/runtime/storage.allocated.internal.application",
            storage_internal_application_allocated);
    Tizen::System::RuntimeInfo::GetValue(
            L"http://tizen.org/runtime/storage.allocated.internal.download",
            storage_internal_download_allocated);
    Tizen::System::RuntimeInfo::GetValue(
            L"http://tizen.org/runtime/storage.allocated.internal.image",
            storage_internal_image_allocated);
    Tizen::System::RuntimeInfo::GetValue(
            L"http://tizen.org/runtime/storage.allocated.internal.video",
            storage_internal_video_allocated);
*/
    int memory_available; // in bytes
    int memory_allocated;
    Tizen::System::RuntimeInfo::GetValue(L"http://tizen.org/runtime/memory.available",
            memory_available);
    Tizen::System::RuntimeInfo::GetValue(L"http://tizen.org/runtime/memory.allocated",
            memory_allocated);

    v8::HandleScope scope;
    v8::Local<v8::Object> storageInfo = v8::Object::New();

    // for experiments.
    {
        char buf[512];
        sprintf(buf, "%d", memory_allocated);
        storageInfo->Set(v8::String::New("total"), v8::String::New(buf));
        sprintf(buf, "%d", memory_allocated - memory_available);
        storageInfo->Set(v8::String::New("usage"), v8::String::New(buf));
        sprintf(buf, "%d", memory_available);
        storageInfo->Set(v8::String::New("remainder"), v8::String::New(buf));
    }
    return scope.Close(storageInfo);
}

v8::Handle<v8::Value> System::getPhoneNumber(const v8::Arguments& args) {
    AppLog("entered System::getPhoneNumber");

    v8::HandleScope scope;

    Tizen::Telephony::SimInfo *pSimInfo = new Tizen::Telephony::SimInfo();
    result r = pSimInfo->Construct();
    if ( IsFailed(r) ) {
        delete pSimInfo;
        // when SIM is not available 'undefined' return.
        return scope.Close( v8::Undefined() );
    }

    // SIM is available then get phone number
    v8::String phoneNumber = v8::String::New( pSimInfo->GetPhoneNumber() );
    delete pSimInfo;
    return scope.Close( phoneNumber );
}

