#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <SharedMem.h>

typedef struct{
    unsigned target_x;
    unsigned target_y;
    float    target_angle;
} __attribute__ ((packed)) TTarget;

class CNavigation
{
public:
    CNavigation();

    void PenaltyDefend(TNavEntry *entry);
    void PenaltyAttack(TNavEntry *entry);

    void GenerateValues(TNavEntry *entry);

    void GenerateStop(TNavEntry *entry);
    void kickerP(TNavEntry *entry);

private:

    bool isNewPath(TAIEntry *ai);
    void savePathState(TAIEntry *ai);
    void setCurrentPos(TNavEntry *entry, int &x, int &y, float &theta, TTeamColor teamColor);

    TTarget m_target;

    float m_ourPos_x;
    float m_ourPos_y;
    float m_ourOrientation;
    int targetId; // ID of the next taret point provided by AI
    int lastPathLen, lastCurrentX, lastCurrentY; // for checking wheather AI has generated a new path

    bool m_isPrevStateSet;
    float m_prevX;
    float m_prevY;
    float m_prevOrientation;
};

#endif // NAVIGATION_H
