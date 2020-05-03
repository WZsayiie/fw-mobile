#include "CObject.hh"
#include "cqjnihelper.hh"

static const int32_t JAVA_OBJECT_MAGIC = 0x4A415641; //"JAVA".

static void del_java_object(void *raw) {
    if (raw != nullptr) {
        cqJNIEnv::env()->DeleteGlobalRef((jobject)raw);
    }
}

extern "C" JNIEXPORT jobject JNICALL Java_src_library_basis_CObject_retainRaw
    (JNIEnv *env, jclass, jobject object, jstring cls)
{
    if (object != nullptr) {
        std::string name = cqJNIType::string(env, cls);
        void *raw = (void *)env->NewGlobalRef(object);
        cq_object *ptr = cq_object_retain_raw(raw, name.c_str(), JAVA_OBJECT_MAGIC, del_java_object);
        return cqJNIType::jniPtr(env, ptr);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_src_library_basis_CObject_retain
    (JNIEnv *env, jclass, jobject _object)
{
    auto object = cqJNIType::ptr<cq_object *>(env, _object);
    cq_object_retain(object);
    return _object;
}

extern "C" JNIEXPORT void JNICALL Java_src_library_basis_CObject_release
    (JNIEnv *env, jclass, jobject _object)
{
    auto object = cqJNIType::ptr<cq_object *>(env, _object);
    cq_object_release(object);
}

extern "C" JNIEXPORT jobject JNICALL Java_src_library_basis_CObject_raw
    (JNIEnv *env, jclass, jobject _raw)
{
    auto raw = cqJNIType::ptr<cq_object *>(env, _raw);
    if (raw == nullptr) {
        return nullptr;
    }

    if (cq_object_magic(raw) != JAVA_OBJECT_MAGIC) {
        //it's not a java object.
        return nullptr;
    }

    jobject object = (jobject)cq_object_raw(raw);
    return env->NewLocalRef(object);
}

extern "C" JNIEXPORT void JNICALL Java_src_library_basis_CObject_listen
    (JNIEnv *env, jclass, jobject _object, jint event, jobject _block)
{
    auto object = cqJNIType::ptr<cq_object *>(env, _object);
    auto block  = cqJNIType::ptr<cq_block  *>(env, _block );

    cq_object_listen(object, (int32_t)event, block);
}

extern "C" JNIEXPORT void JNICALL Java_src_library_basis_CObject_emit
    (JNIEnv *env, jclass, jobject _object, jint event)
{
    auto object = cqJNIType::ptr<cq_object *>(env, _object);
    cq_object_emit(object, (int32_t)event);
}
