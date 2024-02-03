#include "occutils/occutils-io.h"

// std includes
#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>

// OCC includes
#include <IFSelect_ReturnStatus.hxx>
#include <IGESControl_Reader.hxx>
#include <STEPControl_Reader.hxx>
#include <TopoDS_Shape.hxx>
#include <XSControl_Reader.hxx>

// occutils includes
#include "occutils/occutils-exceptions.h"

namespace occutils::io {

TopoDS_Shape Read(const std::string& filename) {
  auto reader = Reader::STEPorIGESReader(filename);
  Reader::ReadFile(reader, filename);
  return Reader::ReadOneShape(reader);
}

namespace Reader {

std::shared_ptr<XSControl_Reader> STEPorIGESReader(
    const std::string& filename) {
  std::shared_ptr<XSControl_Reader> reader;

  std::filesystem::path filepath(filename);
  std::string extension = filepath.extension().string();
  std::transform(extension.begin(), extension.end(), extension.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  if (extension == ".step" || extension == ".stp") {
    reader = STEPReader();
  } else if (extension == ".iges" || extension == ".igs") {
    reader = IGESReader();
  } else {
    throw OCCIOException(
        "Unknown file extension (.stp/.step or .igs/.iges expected): " +
        filename);
  }
  return reader;
}

std::shared_ptr<XSControl_Reader> STEPReader() {
  return std::shared_ptr<XSControl_Reader>(
      dynamic_cast<XSControl_Reader*>(new STEPControl_Reader()));
}

std::shared_ptr<XSControl_Reader> IGESReader() {
  return std::shared_ptr<XSControl_Reader>(
      dynamic_cast<XSControl_Reader*>(new IGESControl_Reader()));
}

/**
 * Convert a IFSelect_ReturnStatus code to string
 */
static std::string _IFSelectReturnStatusToString(IFSelect_ReturnStatus code) {
  switch (code) {
    case IFSelect_RetVoid:
      return "Void";
    case IFSelect_RetDone:
      return "Done";
    case IFSelect_RetError:
      return "Error";
    case IFSelect_RetFail:
      return "Fail";
    case IFSelect_RetStop:
      return "Stop";
    default:
      return "Unknown";
  }
}

void ReadFile(const std::shared_ptr<XSControl_Reader>& reader,
              const std::string& filename) {
  if (auto readStat = reader->ReadFile(filename.c_str());
      readStat != IFSelect_RetDone) {
    throw OCCIOException("Failed to read file, error code: " +
                         _IFSelectReturnStatusToString(readStat));
  }
}

TopoDS_Shape ReadOneShape(const std::shared_ptr<XSControl_Reader>& reader) {
  // Check if there is anything to convert
  if (auto numRoots = reader->NbRootsForTransfer(); numRoots < 1) {
    throw OCCIOException(
        "Failed to read file: No roots to transfer are present");
  }
  // Convert STEP to shape
  if (reader->TransferRoots() < 1) {
    throw OCCIOException("Failed to read file: Failed to transfer any roots");
  }
  return reader->OneShape();
}

}  // namespace Reader

}  // namespace occutils::io
