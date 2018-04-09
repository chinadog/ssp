#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <irrKlang.h>

using namespace irrklang;

class SoundEngine
{
private:
    SoundEngine();
    SoundEngine( const SoundEngine&);
    SoundEngine& operator=( SoundEngine& );

    ISoundEngine* m_soundEngine = nullptr;
public:
    static SoundEngine& instance() {
        static SoundEngine instance;
        return instance;
    }
    inline ISoundEngine* soundEngine() const {return instance().m_soundEngine;}
};

#define StaticSoundEngine (static_cast<ISoundEngine *>(SoundEngine::instance().soundEngine()))

#endif // SOUNDENGINE_H
