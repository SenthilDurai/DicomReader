#ifndef DICOM_READER_H
#define DICOM_READER_H

#include <string>
#include <opencv2/opencv.hpp>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/ofstd/ofstring.h>

class DICOMReader {
public:
    DICOMReader(const std::string& filePath);
    ~DICOMReader();
    void readFile(const std::string &filename);
    std::string getPatientName() const;
    std::string getModality() const;
    int getImageHeight() const;
    int getImageWidth() const;
    void saveImage(const std::string &outputFilePath);

private:
    DcmFileFormat fileFormat;
    DcmDataset* dataset;
    std::string dicomFilePath;

    OFString sPatientNameOF;
    OFString sModalityOF;
    OFString sPhotometricInterpretation;
    int nImageHeight;
    int nImageWidth;
};

#endif // DICOM_READER_H
