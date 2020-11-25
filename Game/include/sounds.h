#ifndef SOUNDS_H
#define SOUNDS_H

#include <SNDS/irrKlang.h>
#pragma comment(lib,"irrKlang.lib") // link your dll to the program

using namespace irrklang;

class sounds
{
    public:
        sounds();
        virtual ~sounds();
        void playMusic(char *, bool);
        void playSound(char *, bool);
        void stopAllSounds();
        void setSoundVolume(int );
        void pauseSounds(bool);
        int initSounds();
        bool isPlaying = false;
        int volume;
    protected:

    private:
};

#endif // SOUNDS_H
