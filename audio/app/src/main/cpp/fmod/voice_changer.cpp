#include <jni.h>
#include <string>
#include <fmod_common.h>
#include <fmod.hpp>
#include <unistd.h>
#include <android/log.h>
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "keymatch", __VA_ARGS__)
using namespace FMOD;

#define MODE_NORMAL 0
#define MODE_LUOLI 1
#define MODE_DASHU 2
#define MODE_JINGSONG 3
#define MODE_GAOGUAI 4
#define MODE_KONGLING 5

extern "C" {

JNIEXPORT void JNICALL
Java_com_a78_audio_jni_VoiceChangeJni_fix(JNIEnv *env, jobject instance, jstring path_,jint mode) {
    const char *path = env->GetStringUTFChars(path_, 0);
    Sound *sound;
    System *system;
    Channel *channel;  //channelGroup
    DSP *dsp;
    bool isPlaying = true;
    //二级指针
    System_Create(&system);
    system->init(32, FMOD_INIT_NORMAL, NULL);
    system->createSound(path, FMOD_DEFAULT, NULL, &sound);

    try {
        switch (mode) {
            case MODE_NORMAL:
                system->playSound(sound, NULL, false, &channel);
                break;
            case MODE_LUOLI:
                LOGD("luoli");
                //创建音效
                system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT,&dsp);
                //设置值
                dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH,2.5);
                system->playSound(sound, NULL, false, &channel);
                //添加到人声音效
                channel->addDSP(0,dsp);
                break;
            case MODE_DASHU:
                LOGD("dashu1");
                //创建音效
                system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT,&dsp);
                //设置值
                dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH,0.8);
                system->playSound(sound, NULL, false, &channel);
                //添加到人声音效
                channel->addDSP(0,dsp);
                break;

        }

    } catch (...) {
        goto end;
    }

   //真正执行
    system->update();
    //检查是否播放完成
    while (isPlaying) {
        channel->isPlaying(&isPlaying);
        sleep(1);
    }

    end:
    //释放
    system->close();
    system->release();

    env->ReleaseStringUTFChars(path_, path);
}

}


