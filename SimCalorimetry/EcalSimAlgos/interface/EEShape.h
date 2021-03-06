#ifndef EcalSimAlgos_EEShape_h
#define EcalSimAlgos_EEShape_h

#include "SimCalorimetry/EcalSimAlgos/interface/EcalShapeBase.h"

class EEShape : public EcalShapeBase
{
   public:
   EEShape(bool useDB):EcalShapeBase(useDB){if(!useDB)buildMe();} // if useDB = true, then buildMe is executed when setEventSetup and DB conditions are available}
//   EEShape():EcalShapeBase(false){;}

   protected:
      void fillShape(float &time_interval, double &m_thresh, EcalShapeBase::DVec& aVec, const edm::EventSetup* es) const override ;
};
  


#endif
  
