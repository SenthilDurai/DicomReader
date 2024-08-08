# DicomReader

Dicom Reader Application

It is a simple application written in C++ using the DCMTK library that reads dicom tags and 
pixel data from a given DICOM image and saves the pixel data as an jpg image to disk.


Prerequisites: 
--------------
- requires CMAKE tool for configuring and building the code
- requires DCMTK library for reading dicom images
- requires OpenCV library for converting and saving pixel data as an image to disk
- requires GTest library for Unit testing the application functionalities

Project structure:
------------------

-DicomReader
      |
	  |
	  |--- build (debug folder will contain executables after successful build process)
	  |
	  |--- src
	  |	   |
	  |	   --- DicomReader.cpp  (CDicomReader class implementation)
	  |    |
	  |	   --- DicomReader.h    (CDicomReader class declaration)
	  |	   |
	  |	   --- main.cpp         (Application entry point)
	  |
	  |--- test
	  |    |
	  |    --- test_main.cpp  (Unit Test module using "Google Test" framework)
	  |
	  |--- data
	  |    |
	  |	   |
	  |	   --- 327.dcm  (given sample  - CT dicom image)
	  |	   |
	  |	   --- 0002.dcm (another sample - XA dicom image)
      |
      |-- CMakeLists.txt     (CMake configuration file)
      	  



 