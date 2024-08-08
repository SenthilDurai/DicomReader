#include <iostream>
#include <gtest/gtest.h>
#include "../src/DicomReader.h"
#include <opencv2/opencv.hpp>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmjpeg/djdecode.h>
#include <dcmtk/dcmjpeg/djencode.h>
#include <dcmtk/dcmjpeg/djrplol.h>
#include <dcmtk/dcmjpeg/dipijpeg.h>
#include <dcmtk/dcmdata/dcistrma.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmdata/dcrledrg.h>



TEST(SampleTest, ReadDicomTags) {
    DICOMReader reader("../../data/327.dcm");
    EXPECT_EQ(reader.getPatientName(), "BRUNER^HUEY");
    EXPECT_EQ(reader.getModality(), "CT");
    EXPECT_EQ(reader.getImageWidth(), 512);
    EXPECT_EQ(reader.getImageHeight(), 512);
}

TEST(SampleTest, SavePixelDataAsImage) {
    DICOMReader reader("../../data/327.dcm");
    reader.saveImage("../../data/test_output_image.jpg");	
    cv::Mat img = cv::imread("../../data/test_output_image.jpg", cv::IMREAD_GRAYSCALE);
    ASSERT_FALSE(img.empty());
    EXPECT_EQ(img.cols, 512);
    EXPECT_EQ(img.rows, 512);
}

void initializeCodecs() {
    //
    // Register codecs for JPEG and RLE
    //
    DJDecoderRegistration::registerCodecs();     // JPEG
    DcmRLEDecoderRegistration::registerCodecs(); // RLE
    }

void cleanupCodecs() {
    //
    // Cleanup
    //
    DcmRLEDecoderRegistration::cleanup();
    DJDecoderRegistration::cleanup();
    }

int main(int argc, char** argv) {
	
    /* //  -- debug purpose --
	std::cout << "Press Enter to continue...";
    std::cin.get(); // Waits for the user to press Enter
    */	
    std::cout << "Initializing Google Test" << std::endl;
	initializeCodecs();
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "Running all tests" << std::endl;
	int nRetVal = RUN_ALL_TESTS();
	cleanupCodecs();	
    return(nRetVal);
}