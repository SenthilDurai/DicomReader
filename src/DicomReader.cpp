#include "DicomReader.h"
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmjpeg/djdecode.h>
#include <dcmtk/dcmjpeg/djencode.h>
#include <dcmtk/dcmjpeg/djrplol.h>
#include <dcmtk/dcmjpeg/dipijpeg.h>
#include <dcmtk/dcmdata/dcistrma.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmdata/dcrledrg.h>
#include <opencv2/opencv.hpp>



DICOMReader::DICOMReader(const std::string& filePath) {
    dicomFilePath = filePath;    
    readFile(filePath.c_str());
    }

DICOMReader::~DICOMReader() {
    }

void DICOMReader::readFile(const std::string &filename) {
    try
        {    
        dicomFilePath = filename;
        //
        // Load the DICOM file
        //
        DcmFileFormat fileFormat;
        OFCondition status = fileFormat.loadFile(filename.c_str());
        if (!status.good()) {
            std::cerr << "Error reading DICOM file: " << status.text() << std::endl;
            return;
            }

        DcmDataset* pDataset = fileFormat.getDataset();
        if (!pDataset) {
            std::cerr << "Error: unable to get DICOM dataset" << std::endl;
            return;
        }
    
        status = pDataset->findAndGetOFString(DCM_PatientName, sPatientNameOF);
        if (status.good()) {
       // -- for debug purpose --
       //     std::cout << "Patient Name:" << patientNameOF;
       //     patientName = "Test"; //patientNameOF.c_str(); // Convert OFString to std::string
            } else {
            std::cerr << "Error retrieving patient name: " << status.text() << std::endl;
            return;
            }
    
        status = pDataset->findAndGetOFString(DCM_Modality, sModalityOF);
        if (!status.good()) {
            std::cerr << "Error retrieving modality: " << status.text() << std::endl;
            return;
            }
        
        // Get photometric interpretation
        status = pDataset->findAndGetOFString(DCM_PhotometricInterpretation, sPhotometricInterpretation);
        if (!status.good())
            {
            std::cerr << "Error retrieving Photometric Interpretation: " << status.text() << std::endl;
            return;
            }

        Uint16 imageWidthU16 = 0;
        Uint16 imageHeightU16 = 0;
        status = pDataset->findAndGetUint16(DCM_Columns, imageWidthU16);
        if (status.good()) {
            nImageWidth = imageWidthU16;
            } else {
            std::cerr << "Error retrieving image width: " << status.text() << std::endl;
            return;
            }
    
        status = pDataset->findAndGetUint16(DCM_Rows, imageHeightU16);
        if (status.good()) {
            nImageHeight = imageHeightU16;
            } else {
            std::cerr << "Error retrieving image height: " << status.text() << std::endl;
            return;
            }
        }
    catch (const std::exception &e)
        {
        std::cerr << "Exception: " << e.what() << std::endl;
        return;
        }
    }

std::string DICOMReader::getPatientName() const {
    return sPatientNameOF;
    }

std::string DICOMReader::getModality() const {
    return sModalityOF;
    }

int DICOMReader::getImageHeight() const {
    return nImageHeight;
    }

int DICOMReader::getImageWidth() const {
    return nImageWidth;
    }

void DICOMReader::saveImage(const std::string &outputFilePath) {
    DicomImage dicomImage(dicomFilePath.c_str());
    if (dicomImage.getStatus() != EIS_Normal) {
        std::cerr << "Error: cannot load DICOM image (" << DicomImage::getString(dicomImage.getStatus()) << ")" << std::endl;
        return;
        }

    // Convert the DICOM image to 8-bit grayscale
    dicomImage.setMinMaxWindow();
    const int width = dicomImage.getWidth();
    const int height = dicomImage.getHeight();
    //
    // Get pixel data
    //
    const void* pixelData = dicomImage.getOutputData(8);

    if (pixelData == nullptr) {
        std::cerr << "Error: cannot get pixel data from DICOM image" << std::endl;
        return;
        }    
    //
    // Determine number of channels
    //
    int nChannels = 1; // Default to grayscale
    if (sPhotometricInterpretation == "RGB" || sPhotometricInterpretation == "PALETTE COLOR") {
        nChannels = 3;
        }
    else if (sPhotometricInterpretation == "YBR_FULL" || sPhotometricInterpretation == "YBR_FULL_422") {
        nChannels = 3;
        }

    int cvType = (nChannels == 1) ? CV_8UC1 : CV_8UC3;
    //
    // Convert pixel data to OpenCV Mat format
    //
    cv::Mat img(height, width, cvType, const_cast<void*>(pixelData));
    if (img.empty()) {
        std::cerr << "Error converting pixel data to OpenCV Mat format." << std::endl;
        return;
        }
    //
    // Save image using OpenCV
    //
    if (!cv::imwrite(outputFilePath, img)) {
        std::cerr << "Error saving image to file: " << outputFilePath << std::endl;
        }
    std::cout << "Successfully saved image as " << outputFilePath << std::endl;
    }