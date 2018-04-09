#ifndef FMS_H
#define FMS_H

enum class PlayerState
{
    Stand,
    Duck,
    Walk,
    Run,
    DuckWalk,
    StandShoot,
    DuckShoot,
    WalkShoot,
    RunShoot,
    DuckWalkShoot,
    StandReload,
    DuckReload,
    WalkReload,
    RunReload,
    DuckWalkReload,
    StandDraw,
    DuckDraw,
    WalkDraw,
    RunDraw,
    DuckWalkDraw,
    Count
};

enum class PlayerSignal
{
    Stop,
    Go,
    Run,
    Down,
    Up,
    Shoot,
    StopShoot,
    Reload,
    Draw,
    StopDraw,
    Count
};

class FMS
{
public:
    FMS();

    PlayerState currentState() const {return static_cast<PlayerState>(m_currentState);}
    void setState(const PlayerSignal& signal) {m_currentState = state[m_currentState][static_cast<int>(signal)];}

private:

    int state[static_cast<int>(PlayerState::Count)][static_cast<int>(PlayerSignal::Count)] =
    {
                    //   stop|go|run|down|up|shoot|stopShoot|reload|draw|stopDraw
                    //----------------------------------------------------------------
/*  0          Stand |*/{  0,  2,  3,  1,  0,    5,    0,    10,    15,   0 },
/*  1           Duck |*/{  1,  4,  4,  1,  0,    6,    1,    11,    16,   1 },
/*  2           Walk |*/{  0,  2,  3,  4,  2,    7,    2,    12,    17,   2 },
/*  3            Run |*/{  0,  2,  3,  4,  3,    8,    3,    13,    18,   3 },
/*  4       DuckWalk |*/{  1,  4,  4,  4,  2,    9,    4,    14,    19,   4 },
/*  5     StandShoot |*/{  5,  7,  8,  6,  5,    5,    0,    10,    15,   0 },
/*  6      DuckShoot |*/{  6,  9,  9,  6,  5,    5,    1,    11,    16,   1 },
/*  7      WalkShoot |*/{  5,  7,  8,  9,  7,    7,    2,    12,    17,   2 },
/*  8       RunShoot |*/{  5,  7,  8,  9,  8,    8,    3,    13,    18,   3 },
/*  9  DuckWalkShoot |*/{  6,  9,  9,  9,  7,    9,    4,    14,    19,   4 },
/* 10    StandReload |*/{ 10, 12, 13, 11, 10,   10,   10,    10,    15,   0 },
/* 11     DuckReload |*/{ 11, 14, 14, 11, 10,   11,   11,    11,    16,   1 },
/* 12     WalkReload |*/{ 10, 12, 13, 14, 12,   12,   12,    12,    17,   2 },
/* 13      RunReload |*/{ 10, 12, 13, 14, 13,   13,   13,    13,    18,   3 },
/* 14 DuckWalkReload |*/{ 11, 14, 14, 14, 12,   14,   14,    14,    19,   4 },
/* 15      StandDraw |*/{ 15, 17, 18, 16, 15,   15,   15,    15,    15,   0 },
/* 16       DuckDraw |*/{ 16, 19, 19, 16, 15,   16,   16,    16,    16,   1 },
/* 17       WalkDraw |*/{ 15, 17, 18, 19, 17,   17,   17,    17,    17,   2 },
/* 18        RunDraw |*/{ 15, 17, 18, 19, 18,   18,   18,    18,    18,   3 },
/* 19   DuckWalkDraw |*/{ 16, 19, 19, 19, 17,   19,   19,    19,    19,   4 }
    };

    int m_currentState = 0;

};

#endif // FMS_H
