#include "stdafx.h"

#include <GameBackbone/Core/BackboneBaseExceptions.h>
#include <GameBackbone/Util/Array2D.h>
#include <GameBackbone/Util/FileReader.h>
#include <GameBackbone/Util/FileWriter.h>

using namespace GB;


BOOST_AUTO_TEST_SUITE(FileWriter_Tests)

struct ReusableObjects {
	ReusableObjects() {
		outputArray = Array2D<std::string>(5, 4);
		outputArray[0][0] = "fruit";
		outputArray[0][1] = "red";
		outputArray[0][2] = "yellow";
		outputArray[0][3] = "blue";
		outputArray[1][0] = "apple";
		outputArray[1][1] = "255";
		outputArray[1][2] = "0";
		outputArray[1][3] = "0";
		outputArray[2][0] = "orange";
		outputArray[2][1] = "255";
		outputArray[2][2] = "255";
		outputArray[2][3] = "0";
		outputArray[3][0] = "banana";
		outputArray[3][1] = "0";
		outputArray[3][2] = "255";
		outputArray[3][3] = "0";
		outputArray[4][0] = "plantain";
		outputArray[4][1] = "0";
		outputArray[4][2] = "255";
		outputArray[4][3] = "255";
	}
	~ReusableObjects() {
	}

	Array2D<std::string> outputArray;
};

BOOST_AUTO_TEST_SUITE(FileWriter_writeStringTests)

BOOST_FIXTURE_TEST_CASE(FileWriter_writeString, ReusableObjects) {

	FileWriter testWriter;

	std::string filePath = TestFileLocation + "banana.bin";
	testWriter.writeString("ba\nna\nna", filePath);

	FileReader testReader;
	std::string outFile = testReader.readFile(filePath);

	BOOST_CHECK(outFile == "ba\nna\nna");
	remove(filePath.c_str());
}

BOOST_AUTO_TEST_SUITE_END() //end FileReader_writeStringTests

BOOST_AUTO_TEST_SUITE(FileWriter_createWriteStringTests)

BOOST_FIXTURE_TEST_CASE(FileWriter_createWriteString, ReusableObjects) {
	//Initialize all the necessary data
	FileWriter testWriter;
	std::string filePath = TestFileLocation + "banana.bin";
	ReusableObjects currentReusableObject = ReusableObjects();

	//pass outputString into createWritableString
	std::string outputString = testWriter.createWritableString(&currentReusableObject.outputArray, ',');

	//write outputString into banana.bin
	testWriter.writeString(outputString, filePath);

	FileReader testReader;
	std::string outFile = testReader.readFile(filePath);
	Array2D<std::string> testArray = testReader.createArray2D(outFile, 5, 4, ',');
	// Ensure that the output array is what is in the file.
	for (int ii = 0; ii < 3; ++ii) {
		for (int jj = 0; jj < 3; ++jj) {
			BOOST_CHECK(testArray[ii][jj] == outputArray[ii][jj]);
		}
	}

	remove(filePath.c_str());
}

BOOST_AUTO_TEST_SUITE_END() //end FileWriter_createWriteStringTests

BOOST_AUTO_TEST_SUITE_END() // end FileWriter_Tests