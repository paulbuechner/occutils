#pragma once

// std includes
#include <string>

// OCC includes
#include <TopoDS_Shape.hxx>

namespace occutils::step_export {

/**
 * Export a shape to a file
 * Units: M, MM, KM, INCH, FT, MI, MIL, UM, CM, UIN
 *
 * @throws OCCInvalidArgumentException in case of null shape
 * @throws OCCRuntimeException in case of transfer error
 * @throws OCCIOException in case of write error
 */
void ExportSTEP(const TopoDS_Shape& shape, const std::string& filename,
                const std::string& unit = "MM");

}  // namespace occutils::step_export
