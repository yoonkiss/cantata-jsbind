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

/**
 * js bind for Tizen::System API
 *
 * @author Gyeongseok Seo {@literal <gseok.seo@gmail.com>}
 *
 */
void NODE_EXTERN System::Init(v8::Handle<v8::Object> target) {
    AppLog("entered System::Init");

    v8::HandleScope scope;
    v8::Local<v8::FunctionTemplate> funcTemplate = v8::Local<v8::FunctionTemplate>::New(v8::FunctionTemplate::New(System::New));
    v8::Local<v8::FunctionTemplate> _getStorage;

    //funcTemplate->InstanceTemplate() -> SetInternalFieldCount( 1 );
    funcTemplate->SetClassName(v8::String::NewSymbol("System"));

    //NODE_SET_PROTOTYPE_METHOD(funcTemplate, "getStorage", getStorage);
    funcTemplate -> Set( v8::String::NewSymbol( "getStorage" ),
            ( _getStorage = v8::Local<v8::FunctionTemplate>::New( v8::FunctionTemplate::New( getStorage ) ) ) -> GetFunction() );
    funcTemplate -> Set( v8::String::NewSymbol( "getMemoryInfo" ), v8::FunctionTemplate::New( getMemoryInfo ) -> GetFunction() );
    funcTemplate -> Set( v8::String::NewSymbol( "getPhoneNumber" ), v8::FunctionTemplate::New( getPhoneNumber ) -> GetFunction() );
    funcTemplate -> Set( v8::String::NewSymbol( "getCPUUsages" ), v8::FunctionTemplate::New( getCPUUsages ) -> GetFunction() );


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
     * key reference
     * https://developer.tizen.org/help/topic/org.tizen.native.appprogramming/html/guide/system/runtime_info.htm
     */
    long long storage_internal_available = 0; //in bytes
//    long long storage_internal_available_media = 0;
    long long storage_internal_audio_allocated = 0;
    long long storage_internal_application_allocated = 0;
    long long storage_internal_download_allocated = 0;
    long long storage_internal_image_allocated = 0;
    long long storage_internal_video_allocated = 0;
    long long storage_internal_total = 0;

    long long storage_external_available = 0; // in bytes
    long long storage_external_audio_allocated = 0;
    long long storage_external_application_allocated = 0;
    long long storage_external_download_allocated = 0;
    long long storage_external_image_allocated = 0;
    long long storage_external_video_allocated = 0;
    long long storage_external_total = 0;

    // internal storage
    Tizen::System::RuntimeInfo::GetValue(L"http://tizen.org/runtime/storage.available.internal",
            storage_internal_available);
    Tizen::System::RuntimeInfo::GetValue(
            L"http://tizen.org/runtime/storage.allocated.internal.application",
            storage_internal_application_allocated);
    Tizen::System::RuntimeInfo::GetValue(
            L"http://tizen.org/runtime/storage.allocated.internal.audio",
            storage_internal_audio_allocated);
    Tizen::System::RuntimeInfo::GetValue(
            L"http://tizen.org/runtime/storage.allocated.internal.download",
            storage_internal_download_allocated);
    Tizen::System::RuntimeInfo::GetValue(
            L"http://tizen.org/runtime/storage.allocated.internal.image",
            storage_internal_image_allocated);
    Tizen::System::RuntimeInfo::GetValue(
            L"http://tizen.org/runtime/storage.allocated.internal.video",
            storage_internal_video_allocated);
//    Tizen::System::RuntimeInfo::GetValue(
//            L"http://tizen.org/runtime/storage.available.internal.media",
//            storage_internal_available_media);

    storage_internal_total = storage_internal_available +
//                             storage_internal_available_media +
                             storage_internal_audio_allocated +
                             storage_internal_application_allocated +
                             storage_internal_download_allocated +
                             storage_internal_image_allocated +
                             storage_internal_video_allocated;

    // external storage
    Tizen::System::RuntimeInfo::GetValue(L"http://tizen.org/runtime/storage.available.external",
            storage_external_available);
    Tizen::System::RuntimeInfo::GetValue(L"http://tizen.org/runtime/storage.allocated.external.application",
            storage_external_application_allocated);
    Tizen::System::RuntimeInfo::GetValue(L"http://tizen.org/runtime/storage.allocated.external.audio",
            storage_external_audio_allocated);
    Tizen::System::RuntimeInfo::GetValue(L"http://tizen.org/runtime/storage.allocated.external.download",
            storage_external_download_allocated);
    Tizen::System::RuntimeInfo::GetValue(L"http://tizen.org/runtime/storage.allocated.external.image",
            storage_external_image_allocated);
    Tizen::System::RuntimeInfo::GetValue(L"http://tizen.org/runtime/storage.allocated.external.video",
            storage_external_video_allocated);

    storage_external_total = storage_external_available +
                             storage_external_audio_allocated +
                             storage_external_application_allocated +
                             storage_external_download_allocated +
                             storage_external_image_allocated +
                             storage_external_video_allocated;

    v8::HandleScope scope;
    v8::Local<v8::Object> storageInfo = v8::Object::New();

    // for experiments.
    {
        char buf[512];

        // internal
        sprintf(buf, "%lld", storage_internal_total);
        storageInfo->Set(v8::String::New("inTotalSize"), v8::String::New(buf));
        sprintf(buf, "%lld", storage_internal_total - storage_internal_available);
        storageInfo->Set(v8::String::New("inTotalUsage"), v8::String::New(buf));
        sprintf(buf, "%lld", storage_internal_application_allocated);
        storageInfo->Set(v8::String::New("inAppUsage"), v8::String::New(buf));
        sprintf(buf, "%lld", storage_internal_audio_allocated);
        storageInfo->Set(v8::String::New("inAudioUsage"), v8::String::New(buf));
        sprintf(buf, "%lld", storage_internal_download_allocated);
        storageInfo->Set(v8::String::New("inDownloadUsage"), v8::String::New(buf));
        sprintf(buf, "%lld", storage_internal_image_allocated);
        storageInfo->Set(v8::String::New("inImageUsage"), v8::String::New(buf));
        sprintf(buf, "%lld", storage_internal_video_allocated);
        storageInfo->Set(v8::String::New("inVideoUsage"), v8::String::New(buf));
        sprintf(buf, "%lld", storage_internal_available);
        storageInfo->Set(v8::String::New("inRemainder"), v8::String::New(buf));

        // external
        sprintf(buf, "%lld", storage_external_total);
        storageInfo->Set(v8::String::New("exTotalSize"), v8::String::New(buf));
        sprintf(buf, "%lld", storage_external_total - storage_external_available);
        storageInfo->Set(v8::String::New("exTotalUsage"), v8::String::New(buf));
        sprintf(buf, "%lld", storage_external_application_allocated);
        storageInfo->Set(v8::String::New("exAppUsage"), v8::String::New(buf));
        sprintf(buf, "%lld", storage_external_audio_allocated);
        storageInfo->Set(v8::String::New("exAudioUsage"), v8::String::New(buf));
        sprintf(buf, "%lld", storage_external_download_allocated);
        storageInfo->Set(v8::String::New("exDownloadUsage"), v8::String::New(buf));
        sprintf(buf, "%lld", storage_external_image_allocated);
        storageInfo->Set(v8::String::New("exImageUsage"), v8::String::New(buf));
        sprintf(buf, "%lld", storage_external_video_allocated);
        storageInfo->Set(v8::String::New("exVideoUsage"), v8::String::New(buf));
        sprintf(buf, "%lld", storage_external_available);
        storageInfo->Set(v8::String::New("exRemainder"), v8::String::New(buf));
    }
    return scope.Close(storageInfo);
}

v8::Handle<v8::Value> System::getMemoryInfo(const v8::Arguments& args) {
    AppLog("entered System::getMemoryInfo");

    int memory_available = 0; // in bytes
    int memory_allocated = 0;
    Tizen::System::RuntimeInfo::GetValue(L"http://tizen.org/runtime/memory.available",
            memory_available);
    Tizen::System::RuntimeInfo::GetValue(L"http://tizen.org/runtime/memory.allocated",
            memory_allocated);

    v8::HandleScope scope;
    v8::Local<v8::Object> memoryInfo = v8::Object::New();

    // for experiments.
    {
        char buf[512];
        sprintf(buf, "%d", memory_allocated);
        memoryInfo->Set(v8::String::New("total"), v8::String::New(buf));
        sprintf(buf, "%d", memory_allocated - memory_available);
        memoryInfo->Set(v8::String::New("usage"), v8::String::New(buf));
        sprintf(buf, "%d", memory_available);
        memoryInfo->Set(v8::String::New("remainder"), v8::String::New(buf));
    }
    return scope.Close(memoryInfo);
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
    const wchar_t * pwPhoneNumber = pSimInfo->GetPhoneNumber().GetPointer();
    int nLen = wcslen( pwPhoneNumber );
    char buf[128];
    memset( buf, 0x00, sizeof(buf) );
    wcstombs( buf, pwPhoneNumber, nLen );

    delete pSimInfo;
    return scope.Close( v8::String::New( buf ) );
}

v8::Handle<v8::Value> System::getCPUUsages(const v8::Arguments& args) {
    AppLog("entered System::getCPUUsages");

    int cpu_usage = 0; // in percentage
    Tizen::System::RuntimeInfo::GetValue(
            L"http://tizen.org/runtime/cpu.core.all.usage",
            cpu_usage
            );

    v8::HandleScope scope;

    v8::Local<v8::Object> cpuInfo = v8::Object::New();

    // for experiments.
    {
        char buf[512];
        sprintf(buf, "%d", cpu_usage);
        cpuInfo->Set(v8::String::New("cpuUsage"), v8::String::New(buf));
    }
    return scope.Close( cpuInfo );
}
