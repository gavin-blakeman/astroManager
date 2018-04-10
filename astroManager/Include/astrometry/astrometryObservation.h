#ifndef ASTROMETRYOBSERVATION
#define ASTROMETRYOBSERVATION

#include "../qtExtensions/qt.h"

#include <memory>
#include <vector>

#include <ACL>

namespace AstroManager
{
  namespace astrometry
  {
    class CAstrometryObservation : public ACL::CAstrometryObservation
    {
    public:
      QGraphicsItemGroup *group;
      QGraphicsSimpleTextItem *text;

      CAstrometryObservation();
      CAstrometryObservation(ACL::CAstrometryObservation const &);
      CAstrometryObservation(std::string const &);
      CAstrometryObservation(ACL::SPTargetAstronomy &);

      virtual ~CAstrometryObservation();

      friend std::ostream &operator << (std::ostream &, CAstrometryObservation &);
    };
    typedef std::shared_ptr<CAstrometryObservation> PAstrometryObservation;

    typedef std::vector<PAstrometryObservation> DAstrometryObservationStore;
    typedef std::vector<PAstrometryObservation> DTaggedObjectsStore;

  } // namespace astrometry
} // namespace AstroManager

#endif // ASTROMETRYOBSERVATION

