#ifndef CONFIG_H
#define CONFIG_H

#include "Config/Inireader.h"
#include <irrlicht.h>
#include <vector>
#include <sstream>



class Config
{
public:
    struct PlayerStruct
    {
    public:
        inline double runSpeed() const {return m_runSpeed;}
        inline double walkSpeed() const {return m_walkSpeed;}

        inline void setRunSpeed(double value) {m_runSpeed = value;}
        inline void setWalkSpeed(double value) {m_walkSpeed = value;}
    private:
        double m_runSpeed = 0.0;
        double m_walkSpeed = 0.0;
    };

    struct MonsterStruct
    {
    public:
        inline double atackSpeed() const {return m_atackSpeed;}
        inline double walkSpeed() const {return m_walkSpeed;}

        inline void setAtackSpeed(double value) {m_atackSpeed = value;}
        inline void setWalkSpeed(double value) {m_walkSpeed = value;}
    private:
        double m_atackSpeed = 0.0;
        double m_walkSpeed = 0.0;
    };



    struct Respawn
    {
    public:
        template<typename Out>
        static void split(const std::string &s, char delim, Out result) {
            std::stringstream ss(s);
            std::string item;
            while (std::getline(ss, item, delim)) {
                *(result++) = item;
            }
        }

        static std::vector<std::string> split(const std::string &s, char delim) {
            std::vector<std::string> elems;
            split(s, delim, std::back_inserter(elems));
            return elems;
        }

        struct Point
        {
            Point(const std::string& str)
            {
                std::vector<std::string> list = split(str,',');
                m_x = std::stod(list[0]);
                m_y = std::stod(list[1]);
                m_z = std::stod(list[2]);
            }
            Point(double x, double y, double z) : m_x(x), m_y(y), m_z(z){}
            double m_x = 0.0;
            double m_y = 0.0;
            double m_z = 0.0;
        };
        std::vector<Point>& points() {return m_points;}

        inline int count() const {return m_count;}

        inline void setCount(int value) {m_count = value;}
    private:
        std::vector<Point> m_points;
        int m_count = 0;
    };

    Config();
    bool load(const std::string& filename);
    // getters
    inline bool fog() const {return m_fog;}
    inline bool freeFly() const {return m_freeFly;}
    inline bool normals() const {return m_normals;}
    inline int count() const {return m_count;}
    inline irr::SIrrlichtCreationParameters params() const {return m_params;}
    inline PlayerStruct& player() {return m_player;}
    inline MonsterStruct& monster() {return m_monster;}
    inline Respawn& respawn() {return m_respawn;}
    // setters
    inline void setFog(bool fog) {m_fog = fog;}
    inline void setFreeFly(bool freeFly) {m_freeFly = freeFly;}
    inline void setNormals(bool normals) {m_normals = normals;}
    inline void setCount(int value) {m_count = value;}
private:
    irr::SIrrlichtCreationParameters m_params;
    bool m_freeFly = false;
    bool m_fog = false;
    bool m_normals = false;
    int m_count = 0;
    PlayerStruct m_player;
    MonsterStruct m_monster;
    Respawn m_respawn;

};

#endif // CONFIG_H
