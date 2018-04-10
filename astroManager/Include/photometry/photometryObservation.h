#ifndef PHOTOMETRYOBSERVATION
#define PHOTOMETRYOBSERVATION

#include "../qtExtensions/qt.h"

#include <ACL>

namespace AstroManager
{
  namespace photometry
  {
    class CPhotometryObservation : public ACL::CPhotometryObservation
    {
    public:
      QGraphicsItemGroup *group;
      QGraphicsSimpleTextItem *text;

      CPhotometryObservation();
      CPhotometryObservation(ACL::CPhotometryObservation const &);
      CPhotometryObservation(ACL::SPTargetAstronomy &);

      virtual ~CPhotometryObservation();

      friend std::ostream &operator << (std::ostream &, CPhotometryObservation &);
    };
    typedef std::shared_ptr<CPhotometryObservation> PPhotometryObservation;

    typedef std::vector<photometry::PPhotometryObservation> DPhotometryObservationStore;

  } // namespace photometry

} // namespace AstroManager

#endif // PHOTOMETRYOBSERVATION

