/*! \file main.cpp
*	\brief Main console file.
*	\details CPP file containing the main entry point of the console program.
*	\author https://github.com/satyamsameer
*	\version 1.0.0.0
*	\date 27-07-2021
*/
#include <iostream>
#include "SpliceClass.hpp"
#include <string>
#include <sstream>

/*!	\brief Function main of the console program.
*
*	Options provided (0 to exit) :
*	1. Time_Signal - Placement Opportunity Start
*	2. Splice_Insert
*	3. Time_Signal - Placement Opportunity End
*	4. Time_Signal - Program Start/End
*	5. Time_Signal - Program Overlap Start
*	6. Time_Signal - Program Blackout Override / Program End
*	7. Time_Signal - Program End
*	8. Time_Signal - Program Start/End - Placement Opportunity End
*	9. Custom SCTE-35 hexa-decimal string data
*/
int main(){

    scte35::CSpliceClass objLocal;

	int choice = -1;
	do {
		std::cout << "Select any one index hexa-decimal data string to parse - " << std::endl;
		std::cout << "0. Exit" << std::endl;
		std::cout << "1. Time_Signal - Placement Opportunity Start" << std::endl;
		std::cout << "2. Splice_Insert" << std::endl;
		std::cout << "3. Time_Signal - Placement Opportunity End" << std::endl;
		std::cout << "4. Time_Signal - Program Start/End" << std::endl;
		std::cout << "5. Time_Signal - Program Overlap Start" << std::endl;
		std::cout << "6. Time_Signal - Program Blackout Override / Program End" << std::endl;
		std::cout << "7. Time_Signal - Program End" << std::endl;
		std::cout << "8. Time_Signal - Program Start/End - Placement Opportunity End" << std::endl;
		std::cout << "9. Custom SCTE-35 hexa-decimal string data" << std::endl;

		std::cout << "Enter your choice : ";
		std::cin >> choice;

		std::string sTestData="";

		switch (choice)
		{
		case 0: {
			//Exit
		}break;
		case 1: {
			sTestData = "FC3034000000000000FFFFF00506FE72BD0050001E021C435545494800008E7FCF0001A599B00808000000002CA0A18A3402009AC9D17E";
		}break;
		case 2: {
			sTestData = "FC302F000000000000FFFFF014054800008F7FEFFE7369C02EFE0052CCF500000000000A0008435545490000013562DBA30A";
		}break;
		case 3: {
			sTestData = "FC302F000000000000FFFFF00506FE746290A000190217435545494800008E7F9F0808000000002CA0A18A350200A9CC6758";
		}break;
		case 4: {
			sTestData = "FC3048000000000000FFFFF00506FE7A4D88B60032021743554549480000187F9F0808000000002CCBC344110000021743554549480000197F9F0808000000002CA4DBA01000009972E343";
		}break;
		case 5: {
			sTestData = "FC302F000000000000FFFFF00506FEAEBFFF640019021743554549480000087F9F0808000000002CA56CF5170000951DB0A8";
		}break;
		case 6: {
			sTestData = "FC3048000000000000FFFFF00506FE932E380B00320217435545494800000A7F9F0808000000002CA0A1E3180000021743554549480000097F9F0808000000002CA0A18A110000B4217EB0";
		}break;
		case 7: {
			sTestData = "FC302F000000000000FFFFF00506FEAEF17C4C0019021743554549480000077F9F0808000000002CA56C97110000C4876A2E";
		}break;
		case 8: {
			sTestData = "FC3061000000000000FFFFF00506FEA8CD44ED004B021743554549480000AD7F9F0808000000002CB2D79D350200021743554549480000267F9F0808000000002CB2D79D110000021743554549480000277F9F0808000000002CB2D7B31000008A18869F";
		}break;
		case 9: {
			std::cout<<"Enter hexa-decimal data : ";
			std::getline(std::cin>>std::ws,sTestData);//Take string with white spaces
		}break;
		default:
			break;
		}

		objLocal.RemoveSpaces(sTestData);
		if(!sTestData.empty()) {
			if (objLocal.ParseHexa(sTestData)) {
				std::cout << "======================================================== Success ===================================================" << std::endl;
			}
			else {
				std::cout << "====================================================== Failed =======================================================" << std::endl;
			}
			std::cout << objLocal.GetText(false);
			std::cout << "====================================================================================================================" << std::endl << std::endl;
		}

	} while ((choice>0) && (choice<=10));

    //std::cin.get();//Not required
}