#pragma once

#include <TopoDS_Shape.hxx>
#include <string>

namespace occutils::step_export {

/**
 * Export a shape to a file
 * Units: M, MM, KM, INCH, FT, MI, MIL, UM, CM, UIN
 *
 * @throws std::invalid_argument in case of null shape
 * @throws std::logic_error in case of transfer or write error
 */
void ExportSTEP(const TopoDS_Shape& shape, const std::string& filename,
                const std::string& unit = "MM");

}  // namespace occutils::step_export
