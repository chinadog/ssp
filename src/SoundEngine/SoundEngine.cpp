#include "SoundEngine/SoundEngine.h"
#include <iostream>

SoundEngine::SoundEngine()
{
    // start the sound engine with default parameters
    m_soundEngine = irrklang::createIrrKlangDevice();

    if (!m_soundEngine)
    {
        std::cout << "Could not startup engine" << std::endl;
        return; // error starting up the engine
    }
}

SoundEngine::~SoundEngine()
{
    m_soundEngine->drop();
}
