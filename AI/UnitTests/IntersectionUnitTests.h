#if !defined(INTERSECTIONUNITTESTS_H)
#define INTERSECTIONUNITTESTS_H

#include "cpptest.h"

class IntersectionUnitTests : public Test::Suite
{
public:
  IntersectionUnitTests();

private:
    void IntersectionTest();
    void IntersectionTestFail();
    void IntersectionTestBoundaryPass();




};

#endif
