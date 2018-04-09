#ifndef CONFIG_H
#define CONFIG_H

#include "Config/Inireader.h"
#include <irrlicht.h>

class Config
{
public:
    Config();
    bool load(const std::string& filename);
    // getters
    inline bool fog() const {return m_fog;}
    inline bool freeFly() const {return m_freeFly;}
    inline bool normals() const {return m_normals;}
    inline irr::SIrrlichtCreationParameters params() const {return m_params;}
    // setters
    inline void setFog(bool fog) {m_fog = fog;}
    inline void setFreeFly(bool freeFly) {m_freeFly = freeFly;}
    inline void setNormals(bool normals) {m_normals = normals;}

private:
    irr::SIrrlichtCreationParameters m_params;
    bool m_freeFly = false;
    bool m_fog = false;
    bool m_normals = false;
};

#endif // CONFIG_H
