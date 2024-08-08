#include "DicomReader.h"
#include <iostream>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmjpeg/djdecode.h>
#include <dcmtk/dcmjpeg/djencode.h>
#include <dcmtk/dcmjpeg/djrplol.h>
#include <dcmtk/dcmjpeg/dipijpeg.h>
#include <dcmtk/dcmdata/dcistrma.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmdata/dcrledrg.h>



void initializeCodecs() {
    // Register codecs for JPEG and RLE
    DJDecoderRegistration::registerCodecs();     // JPEG
    DcmRLEDecoderRegistration::registerCodecs(); // RLE
    }

void cleanupCodecs() {
    // cleanup
    DcmRLEDecoderRegistration::cleanup();
    DJDecoderRegistration::cleanup();
    }

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <DICOM file path>  <Output Image file path>" << std::endl;
        return 1;
    }
	
// 	  -- for debug purpose --
//    std::cout << "Press Enter to continue...";
//    std::cin.get(); // Waits for the user to press Enter

    try
        {
        initializeCodecs();         // initialize codecs
        std::string dicomFilePath = argv[1];
        DICOMReader reader(dicomFilePath);

        std::cout << "Patient Name: " << reader.getPatientName() << std::endl;
        std::cout << "Modality: " << reader.getModality() << std::endl;
        std::cout << "Image Height: " << reader.getImageHeight() << std::endl;
        std::cout << "Image Width: " << reader.getImageWidth() << std::endl;

        std::string outputImagePath = argv[2];
        reader.saveImage(outputImagePath);
        cleanupCodecs();            // cleanup codecs
        }
    catch (const std::exception& e)
        {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
        }
    return 0;
}

