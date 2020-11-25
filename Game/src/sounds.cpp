#include "sounds.h"
#include <iostream>

using namespace std;

ISoundEngine* engine = createIrrKlangDevice();

sounds::sounds()
{
    //ctor
}

sounds::~sounds()
{
    //dtor
    engine->drop();
}
void sounds::playMusic(char* File, bool loop)
{
    engine->play2D(File,loop);
}

void sounds::playSound(char* File, bool loop)
{
    engine->play2D(File,loop,false);
}

void sounds::setSoundVolume(int setVolume) {
    engine->setSoundVolume(setVolume);
    volume = setVolume;
}

void sounds::stopAllSounds()
{
    engine->stopAllSounds();
}

void sounds::pauseSounds(bool pause) {
    engine->setAllSoundsPaused(pause);
}
int sounds::initSounds()
{
    if(!engine)
    {
       cout<<"Could not Start the engine\n";
       return 0;
    }
    return 1;
}
