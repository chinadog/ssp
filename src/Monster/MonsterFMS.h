#ifndef MONSTERFMS_H
#define MONSTERFMS_H

#include "Common/Logger.h"

enum class MonsterState
{
    Walk,
    Run,
    Atack,
    Wound,
    Die,
    Draw,
    Count
};

enum class MonsterSignal
{
    Go,
    Run,
    Atack,
    Wound,
    Die,
    Stop,
    Draw,
    Count
};

class MonsterFMS
{
public:
    MonsterFMS();

    MonsterState currentState() const {return static_cast<MonsterState>(m_currentState);}
    void setState(const MonsterSignal& signal)
    {
        TWARNING() << "Send signal" << (int)signal;
        m_currentState = state[m_currentState][static_cast<int>(signal)];
        TWARNING() << "State now is = " << (int)m_currentState;
    }
private:
    int state[static_cast<int>(MonsterState::Count)][static_cast<int>(MonsterSignal::Count)] =
    {
            //   go|run|atack|wound|die|stop|draw
            //-------------------------------------
/*  0  Walk |*/{  0,  1,  2,   3,   4,   0,   5},
/*  1   Run |*/{  0,  1,  2,   3,   4,   0,   5},
/*  2 Atack |*/{  2,  2,  2,   3,   4,   0,   5},
/*  3 Wound |*/{  0,  1,  2,   3,   4,   0,   5},
/*  4   Die |*/{  4,  4,  4,   4,   4,   4,   5},
/*  5  Draw |*/{  5,  5,  5,   5,   5,   0,   5},
    };

    int m_currentState = 5;
};

#endif // MONSTERFMS_H
