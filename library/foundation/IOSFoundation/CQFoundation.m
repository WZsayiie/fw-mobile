#import "CQFoundation.h"
#import "cqfoundationarche.h"

//log:

void cq_log_info(const char *file, int32_t line, const char *message) {
    CQLogInfo(@(file), line, @"%s", message);
}

void cq_log_error(const char *file, int32_t line, const char *message) {
    CQLogError(@(file), line, @"%s", message);
}

//file manager:

const char *cq_document_directory(void) {
    NSString *directory = CQDocumentDirectory();
    return cq_string_store_string(directory.UTF8String);
}

const char *cq_caches_directory(void) {
    NSString *directory = CQCachesDirectory();
    return cq_string_store_string(directory.UTF8String);
}

const char *cq_temporary_directory(void) {
    NSString *directory = CQTemporaryDirectory();
    return cq_string_store_string(directory.UTF8String);
}

const char *cq_append_path(const char *parent, const char *child) {
    NSString *path = CQAppendPath(@(parent), @(child));
    return cq_string_store_string(path.UTF8String);
}

bool cq_directory_exists(const char *path) {
    return [CQFileManager.sharedObject directoryExistsAtPath:@(path)];
}

bool cq_file_exists(const char *path) {
    return [CQFileManager.sharedObject fileExistsAtPath:@(path)];
}

bool cq_create_directory(const char *path, bool intermediate) {
    CQFileManager *n = CQFileManager.sharedObject;
    return [n createDirectoryAtPath:@(path) intermediate:intermediate];
}

void cq_remove_path(const char *path) {
    [CQFileManager.sharedObject removeItemAtPath:@(path)];
}
