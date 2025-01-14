
#ifndef FMU4J_NATIVE_JNI_HELPER_HPP
#define FMU4J_NATIVE_JNI_HELPER_HPP

#include "cppfmu/cppfmu_common.hpp"

#include <iostream>
#include <jni.h>

namespace
{

inline void jvm_invoke(JavaVM* jvm, const std::function<void(JNIEnv*)>& f)
{
    JNIEnv* env;
    bool attach = false;
    int getEnvStat = jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_8);
    if (getEnvStat == JNI_EDETACHED) {
        attach = true;
        jvm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
    }

    f(env);

    if (attach) {
        jvm->DetachCurrentThread();
    }
}

jmethodID GetMethodID(JNIEnv* env, jclass cls, const char* name, const char* sig)
{
    jmethodID id = env->GetMethodID(cls, name, sig);
    if (id == nullptr) {
        std::string msg = "[FMU4j native] Unable to locate method '" + std::string(name) + "'!";
        throw cppfmu::FatalError(msg.c_str());
    }
    return id;
}

jmethodID GetStaticMethodID(JNIEnv* env, jclass cls, const char* name, const char* sig)
{
    jmethodID id = env->GetStaticMethodID(cls, name, sig);
    if (id == nullptr) {
        std::string msg = "[FMU4j native] Unable to locate static method '" + std::string(name) + "'!";
        throw cppfmu::FatalError(msg.c_str());
    }
    return id;
}

jclass FindClass(JNIEnv* env, jobject classLoaderInstance, const std::string& name, bool throwOnFailure = true)
{
    const char* cName = name.c_str();
    jstring jName = env->NewStringUTF(cName);

    jclass URLClassLoader = env->FindClass("java/net/URLClassLoader");
    jmethodID loadClass = GetMethodID(env, URLClassLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    auto cls = reinterpret_cast<jclass>(env->CallObjectMethod(classLoaderInstance, loadClass, jName));

    jboolean flag = env->ExceptionCheck();
    if (flag) {
        if (throwOnFailure) {
            env->ExceptionDescribe();
        }
        env->ExceptionClear();
    }

    env->DeleteLocalRef(jName);

    return cls;
}

jobject create_classloader(JNIEnv* env, const std::string& classpath)
{
    std::string path = classpath;
    if (classpath.rfind('/', 0) == 0) {
        path.insert(0, "file:");
    } else {
        path.insert(0, "file:/");
    }
    const char* cClasspath = path.c_str();
    jstring jClasspath = env->NewStringUTF(cClasspath);

    jclass classLoaderCls = env->FindClass("java/net/URLClassLoader");
    jmethodID classLoaderCtor = GetMethodID(env, classLoaderCls, "<init>", "([Ljava/net/URL;Ljava/lang/ClassLoader;)V");

    jclass urlCls = env->FindClass("java/net/URL");
    jmethodID urlCtor = GetMethodID(env, urlCls, "<init>", "(Ljava/lang/String;)V");
    jobject urlInstance = env->NewObject(urlCls, urlCtor, jClasspath);
    jobjectArray urls = env->NewObjectArray(1, urlCls, urlInstance);

    env->DeleteLocalRef(jClasspath);

    return env->NewObject(classLoaderCls, classLoaderCtor, urls, nullptr);
}

JNIEnv* get_or_create_jvm(JavaVM** jvm)
{
    JNIEnv* env;

    jint rc;
    jsize nVms;
    rc = JNI_GetCreatedJavaVMs(jvm, 1, &nVms);
    if (rc == JNI_OK && nVms == 1) {
        rc = (*jvm)->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_8);
        if (rc == JNI_OK) {
            std::cout << "[FMU4j native] Reusing already created JMV." << std::endl;
            return env;
        }
    }

    JavaVMInitArgs args;
    args.version = JNI_VERSION_1_8;
    args.nOptions = 0;
    rc = JNI_CreateJavaVM(jvm, (void**)&env, &args);
    if (rc == JNI_OK) {
        std::cout << "[FMU4j native] Created a new JVM." << std::endl;
    } else {
        std::cout << "[FMU4j native] Unable to launch JVM: " << rc << std::endl;
    }
    return env;
}


} // namespace

#endif //FMU4J_NATIVE_JNI_HELPER_HPP
