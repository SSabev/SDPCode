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
    void Holonomic();
    void RotateToOrient(float targetOrient);
private:
    void GenerateLinear(TEntry *entry);
    void GenerateAngular(TEntry *entry);
    void GenerateMaxAngular(TEntry *entry);

    TTarget m_target;

    float m_ourPos_x;
    float m_ourPos_y;
    float m_ourOrientation;
};

#endif // NAVIGATION_H
