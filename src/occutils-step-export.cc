#include "occutils/occutils-step-export.h"

// std includes
#include <string>

// OCC includes
#include <IFSelect_ReturnStatus.hxx>
#include <Interface_Static.hxx>
#include <STEPControl_Writer.hxx>
#include <TopoDS_Shape.hxx>

// occutils includes
#include "occutils/occutils-exceptions.h"

namespace occutils::step_export {

void ExportSTEP(const TopoDS_Shape& shape, const std::string& filename,
                const std::string& unit) {
  if (shape.IsNull()) {
    throw OCCInvalidArgumentException("Can't export null shape to STEP");
  }

  STEPControl_Writer writer;
  Interface_Static::SetCVal("xstep.cascade.unit", unit.c_str());
  Interface_Static::SetCVal("write.step.unit", unit.c_str());
  Interface_Static::SetIVal("write.step.nonmanifold", 1);

  if (IFSelect_ReturnStatus transferStatus =
          writer.Transfer(shape, STEPControl_AsIs);
      transferStatus != IFSelect_RetDone) {
    throw OCCRuntimeException("Error while transferring shape to STEP");
  }

  // Write transferred structure to STEP file
  if (IFSelect_ReturnStatus writeStatus = writer.Write(filename.c_str());
      writeStatus != IFSelect_RetDone) {
    throw OCCIOException("Error while writing transferred shape to STEP file");
  }
}

}  // namespace occutils::step_export
