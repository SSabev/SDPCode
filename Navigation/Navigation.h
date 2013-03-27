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

    void GenerateValues();
    void PenaltyDefend();
    void PenaltyAttack();
    void GenerateStop();
    void kickerP();
private:

    TTarget m_target;

    float m_ourPos_x;
    float m_ourPos_y;
    float m_ourOrientation;
};

#endif // NAVIGATION_H
