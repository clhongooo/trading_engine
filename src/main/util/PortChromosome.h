#ifndef PORTCHROMOSOME_H_
#define PORTCHROMOSOME_H_

#include "PCH.h"
#include "Constants.h"
#include "STool.h"
#include "FuturesCtrt.h"
#include "OptionCtrt.h"

using namespace std;
using namespace boost;

class PortChromosome {
  public:
    // enum InterpolationType { LINEAR };
    PortChromosome();
    virtual ~PortChromosome();
    void Reset();

  private:
    //--------------------------------------------------
    // Format:
    //--------------------------------------------------
    map<unsigned long, map<double,double> >                  m_VolSurf;
    map<unsigned long, map<double,boost::shared_mutex> >     m_VolSurfMutex;

};

#endif /* PORTCHROMOSOME_H_ */
