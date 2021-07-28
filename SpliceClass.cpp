/*! \file SpliceClass.cpp
*	\brief Splice parser class definition.
*	\details Splice parser class definition CPP file. CPP file for definition of class SpliceClass whose declaration is in SpliceClass.hpp.
*	\author https://github.com/satyamsameer
*	\version 1.0.0.0
*	\date 27-07-2021
*/

#include "SpliceClass.hpp"
#include <stdexcept>      //for std::invalid_argument
#include <bitset>		// for bit string conversion
#include <sstream>		//for std::stringstream
#include <algorithm>	//for string std::remove_if

/*! \brief scte35 namespace for grouping implementation in one place
*/
namespace scte35{

#pragma region CommonUtil

bool IsSpace(unsigned char ch) {
	return (ch==' ' || ch=='\n' || ch=='\r' || ch=='\t' || ch=='\v' || ch=='\f');
}

void CSpliceClass::RemoveSpaces(std::string& sParam) {
	sParam.erase(std::remove_if(sParam.begin(),sParam.end(),IsSpace),sParam.end());
}


std::string CSpliceClass::Base64ToHexa(const std::string& str)
{
	std::string finalStr = "";
	std::string refData = "0123456789abcdef";

	//Check valid b64 string
	if (str.size() % 2 != 0)
	{
		return finalStr;
	}

	//Number of bytes for hexadecimals
	int BytesCount = str.size() / 2;
	int MainCount = (int)str.length();

	unsigned long long hVal = 0;
	//Iterate every 2 chars
	for (int i = 0; i<MainCount; i += 2) 
	{
		for (int j = 0; j<2; j++)
		{
			hVal <<= 6; //Shift 6 bits to the left

			//For range of A-Z
			if (str[i + j] >= 0x41 && str[i + j] <= 0x5a)
			{
				hVal |= str[i + j] - 0x41;
			}
			//For range of a-z
			else if (str[i + j] >= 0x61 && str[i + j] <= 0x7a)
			{
				hVal |= str[i + j] - 0x47;
			}
			//For range of 0-9
			else if (str[i + j] >= 0x30 && str[i + j] <= 0x39)
			{
				hVal |= str[i + j] + 0x04;
			}
			//If value is a '+'
			else if (str[i + j] == 0x2b)
			{
				hVal |= 0x3e;
			}
			//If value is a '/'
			else if (str[i + j] == 0x2f)
			{
				hVal |= 0x3f;
			}
			//If value is a '='
			else if (str[i + j] == 0x3d)
			{
				if (MainCount - (i + j) == 1)
				{
					finalStr += refData[0xf & (hVal >> 8)];
				}
			}
		}
		//Shift to the right by 8 bits
		finalStr += refData[0xf & (hVal >> 8)];
		//Shift to the right by 4 bits
		finalStr += refData[0xf & (hVal >> 4)];
		finalStr += refData[0xf & hVal];
	}

	//Fix - pay attention in future
	int nBinLength = finalStr.length();
	if (nBinLength % 2 != 0) {//should be of even length
		finalStr.append("0");
	}

	return finalStr;
}


const char* CSpliceClass::hexa2binary(char ch)
{
	switch (toupper(ch))
	{
		case '0': return "0000";
		case '1': return "0001";
		case '2': return "0010";
		case '3': return "0011";
		case '4': return "0100";
		case '5': return "0101";
		case '6': return "0110";
		case '7': return "0111";
		case '8': return "1000";
		case '9': return "1001";
		case 'A': return "1010";
		case 'B': return "1011";
		case 'C': return "1100";
		case 'D': return "1101";
		case 'E': return "1110";
		case 'F': return "1111";
		default:
			return "0000";
			break;
	}
}

std::string CSpliceClass::HexaStr2BinaryStr(const std::string& hex)
{
	std::string binary;
	for (unsigned i = 0; i != hex.length(); ++i) {
		binary += hexa2binary(hex[i]);
	}
	return binary;
}

bool CSpliceClass::SpacesNotPresent(const std::string& sParam) {
	if (sParam.empty()) {
		return false;
	}

	std::size_t found = sParam.find(" ");
	if (found == std::string::npos) {
		return true;
	}
	return false;
}

uint32_t CSpliceClass::BinStrToNumber_32bit(const std::string& sInput) {
	try {
		return (uint32_t)std::bitset<32>(sInput).to_ullong();
	}
	catch (...) {
		return PARAMETER_DEF_VAL;
	}	
}

uint64_t CSpliceClass::BinStrToNumber_64bit(const std::string& sInput) {
	try {
		return (uint64_t)std::bitset<64>(sInput).to_ullong();
	}
	catch (...) {
		return PARAMETER_DEF_VAL;
	}
}

uint32_t CSpliceClass::GetValue_info_32bit(const std::string& sInput, const int& iStart, const bool& bUpdateStart, const int& iLength, bool& bSuccess) {
	bSuccess = false;
	if (sInput.empty()) {
		return PARAMETER_DEF_VAL;
	}

	std::size_t iLocalLength = sInput.length();
	if (iLocalLength<(std::size_t)(iStart + iLength)) {
		return PARAMETER_DEF_VAL;
	}

	std::string sTemp;
	sTemp = sInput.substr(iStart, iLength);
	if (sTemp.empty()) {
		return PARAMETER_DEF_VAL;
	}
	if (bUpdateStart) {
		iCurrentBitPoint += iLength;
	}

	bSuccess = true;
	return BinStrToNumber_32bit(sTemp);

}

uint64_t CSpliceClass::GetValue_info_64bit(const std::string& sInput, const int& iStart, const bool& bUpdateStart, const int& iLength, bool& bSuccess) {
	bSuccess = false;
	if (sInput.empty()) {
		return PARAMETER_DEF_VAL;
	}

	std::size_t iLocalLength = sInput.length();
	if (iLocalLength<(std::size_t)(iStart + iLength)) {
		return PARAMETER_DEF_VAL;
	}

	std::string sTemp;
	sTemp = sInput.substr(iStart, iLength);
	if (sTemp.empty()) {
		return PARAMETER_DEF_VAL;
	}
	if (bUpdateStart) {
		iCurrentBitPoint += iLength;
	}

	bSuccess = true;
	return BinStrToNumber_64bit(sTemp);
}

uint32_t CSpliceClass::GetValue_Desc_32bit(const std::string& sInput, int& iStart, const bool& bUpdateStart, const int& iLength, bool& bSuccess) {

	bSuccess = false;
	if (sInput.empty()) {
		return PARAMETER_DEF_VAL;
	}

	std::size_t iLocalLength = sInput.length();
	if (iLocalLength<(std::size_t)(iStart + iLength)) {
		return PARAMETER_DEF_VAL;
	}

	std::string sTemp;
	sTemp = sInput.substr(iStart, iLength);
	if (sTemp.empty()) {
		return PARAMETER_DEF_VAL;
	}

	if (bUpdateStart) {
		iStart += iLength;
	}

	bSuccess = true;

	return BinStrToNumber_32bit(sTemp);

}

uint64_t CSpliceClass::GetValue_Desc_64bit(const std::string& sInput, int& iStart, const bool& bUpdateStart, const int& iLength, bool& bSuccess) {

	bSuccess = false;
	if (sInput.empty()) {
		return PARAMETER_DEF_VAL;
	}

	std::size_t iLocalLength = sInput.length();
	if (iLocalLength<(std::size_t)(iStart + iLength)) {
		return PARAMETER_DEF_VAL;
	}

	std::string sTemp;
	sTemp = sInput.substr(iStart, iLength);
	if (sTemp.empty()) {
		return PARAMETER_DEF_VAL;
	}

	if (bUpdateStart) {
		iStart += iLength;
	}

	bSuccess = true;

	return BinStrToNumber_64bit(sTemp);
}
#pragma endregion

void SInfoData::ClearValues() {
	table_id = PARAMETER_DEF_VAL;
	section_syntax_indicator = PARAMETER_DEF_VAL;
	private_indicator = PARAMETER_DEF_VAL;
	reserved = PARAMETER_DEF_VAL;
	section_length = PARAMETER_DEF_VAL;
	protocol_version = PARAMETER_DEF_VAL;
	encrypted_packet = PARAMETER_DEF_VAL;
	encryption_algorithm = PARAMETER_DEF_VAL;
	pts_adjustment = PARAMETER_DEF_VAL;
	cw_index = PARAMETER_DEF_VAL;
	tier = PARAMETER_DEF_VAL;
	splice_command_length = PARAMETER_DEF_VAL;
	splice_command_type = scte35Cmd::unknown;
	descriptor_loop_length = PARAMETER_DEF_VAL;
	alignment_stuffing = PARAMETER_DEF_VAL;
	E_CRC_32 = PARAMETER_DEF_VAL;
	CRC_32 = PARAMETER_DEF_VAL;
}

CSpliceClass::CSpliceClass()
{
	ResetValue();
}

CSpliceClass::~CSpliceClass()
{
	ResetValue();
}

void CSpliceClass::ResetValue() {
	sNextLine = ",\r\n";
	sCommaOnly = ", ";
	iCurrentBitPoint = 0;
	sBinData = "";
	sDescriptorData = "";

	Clear_Command_Type();
	Clear_Splice_Descriptor();
	m_SInfoData.ClearValues();
}

bool CSpliceClass::ParseSInfoDataFirst() {

	bool bSuccess = false;

	m_SInfoData.table_id = GetValue_info_32bit(sBinData, iCurrentBitPoint, false, SInfoLength::table_id, bSuccess);
	if (!bSuccess) {
		return false;
	}
	iCurrentBitPoint += SInfoLength::table_id;

	m_SInfoData.section_syntax_indicator = GetValue_info_32bit(sBinData, iCurrentBitPoint, false, SInfoLength::section_syntax_indicator, bSuccess);
	if (!bSuccess) {
		return false;
	}
	iCurrentBitPoint += SInfoLength::section_syntax_indicator;

	m_SInfoData.private_indicator = GetValue_info_32bit(sBinData, iCurrentBitPoint, false, SInfoLength::private_indicator, bSuccess);
	if (!bSuccess) {
		return false;
	}
	iCurrentBitPoint += SInfoLength::private_indicator;

	m_SInfoData.reserved = GetValue_info_32bit(sBinData, iCurrentBitPoint, false, SInfoLength::reserved, bSuccess);
	if (!bSuccess) {
		return false;
	}
	iCurrentBitPoint += SInfoLength::reserved;

	m_SInfoData.section_length = GetValue_info_32bit(sBinData, iCurrentBitPoint, false, SInfoLength::section_length, bSuccess);
	if (!bSuccess) {
		return false;
	}
	iCurrentBitPoint += SInfoLength::section_length;

	m_SInfoData.protocol_version = GetValue_info_32bit(sBinData, iCurrentBitPoint, false, SInfoLength::protocol_version, bSuccess);
	if (!bSuccess) {
		return false;
	}
	iCurrentBitPoint += SInfoLength::protocol_version;

	m_SInfoData.encrypted_packet = GetValue_info_32bit(sBinData, iCurrentBitPoint, false, SInfoLength::encrypted_packet, bSuccess);
	if (!bSuccess) {
		return false;
	}
	iCurrentBitPoint += SInfoLength::encrypted_packet;


	m_SInfoData.encryption_algorithm = GetValue_info_32bit(sBinData, iCurrentBitPoint, false, SInfoLength::encryption_algorithm, bSuccess);
	if (!bSuccess) {
		return false;
	}
	iCurrentBitPoint += SInfoLength::encryption_algorithm;


	m_SInfoData.pts_adjustment = GetValue_info_64bit(sBinData, iCurrentBitPoint, false, SInfoLength::pts_adjustment, bSuccess);
	if (!bSuccess) {
		return false;
	}
	iCurrentBitPoint += SInfoLength::pts_adjustment;

	m_SInfoData.cw_index = GetValue_info_32bit(sBinData, iCurrentBitPoint, false, SInfoLength::cw_index, bSuccess);
	if (!bSuccess) {
		return false;
	}
	iCurrentBitPoint += SInfoLength::cw_index;


	m_SInfoData.tier = GetValue_info_32bit(sBinData, iCurrentBitPoint, false, SInfoLength::tier, bSuccess);
	if (!bSuccess) {
		return false;
	}
	iCurrentBitPoint += SInfoLength::tier;

	m_SInfoData.splice_command_length = GetValue_info_32bit(sBinData, iCurrentBitPoint, false, SInfoLength::splice_command_length, bSuccess);
	if (!bSuccess) {
		return false;
	}
	iCurrentBitPoint += SInfoLength::splice_command_length;


	m_SInfoData.splice_command_type = GetValue_info_32bit(sBinData, iCurrentBitPoint, false, SInfoLength::splice_command_type, bSuccess);
	if (!bSuccess) {
		return false;
	}
	iCurrentBitPoint += SInfoLength::splice_command_type;

	return true;
}
std::stringstream CSpliceClass::GetTextSInfoDataFirst(const bool& bIfSingleLine) {
	std::stringstream sLocalStr;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	sLocalStr << "table_id=" << m_SInfoData.table_id << sSepLocal;
	sLocalStr << "section_syntax_indicator=" << m_SInfoData.section_syntax_indicator << sSepLocal;
	sLocalStr << "private_indicator=" << m_SInfoData.private_indicator << sSepLocal;
	sLocalStr << "reserved=" << m_SInfoData.reserved << sSepLocal;
	sLocalStr << "section_length=" << m_SInfoData.section_length << sSepLocal;
	sLocalStr << "protocol_version=" << m_SInfoData.protocol_version << sSepLocal;
	sLocalStr << "encrypted_packet=" << m_SInfoData.encrypted_packet << sSepLocal;
	sLocalStr << "encryption_algorithm=" << m_SInfoData.encryption_algorithm << sSepLocal;
	sLocalStr << "pts_adjustment=" << m_SInfoData.pts_adjustment << sSepLocal;
	sLocalStr << "cw_index=" << m_SInfoData.cw_index << sSepLocal;
	sLocalStr << "tier=" << m_SInfoData.tier << sSepLocal;
	sLocalStr << "splice_command_length=" << m_SInfoData.splice_command_length << sSepLocal;
	sLocalStr << "splice_command_type=" << m_SInfoData.splice_command_type << sSepLocal;

	return sLocalStr;
}

bool CSpliceClass::ParseSInfoDataSecond() {

	bool bSuccess = false;

	m_SInfoData.descriptor_loop_length = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 16, bSuccess);
	if (!bSuccess) { return false; }

	return true;
}

std::stringstream CSpliceClass::GetTextSInfoDataSecond(const bool& bIfSingleLine) {
	std::stringstream sLocalStr;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	sLocalStr << "descriptor_loop_length=" << m_SInfoData.descriptor_loop_length << sSepLocal;

	return sLocalStr;
}

void CSpliceClass::Clear_Command_Type() {
	switch (m_SInfoData.splice_command_type)
	{
		case scte35Cmd::splice_null: {
			//Nothing to do
		}break;
		case scte35Cmd::splice_schedule: {
			m_objSpliceSchedule.ClearValues();
		}break;
		case scte35Cmd::splice_insert: {
			m_objSpliceInsert.ClearValues();
		}break;
		case scte35Cmd::time_signal: {
			m_objTimeSignal.ClearValues();
		}break;
		case scte35Cmd::bandwidth_reservation: {
			//Nothing to do
		}break;
		case scte35Cmd::private_command: {
			m_objPrivateCommand.ClearValues();
		}break;
		default: {
			//Nothing to do
		}
		break;
	}
}

bool CSpliceClass::Parse_Command_Type() {
	
	switch (m_SInfoData.splice_command_type)
	{
		case scte35Cmd::splice_null: {
			if (!Parse_Splice_Null()) {
				return false;
			}
		}break;
		case scte35Cmd::splice_schedule: {
			if (!Parse_Splice_Schedule()) {
				return false;
			}
		}break;
		case scte35Cmd::splice_insert: {
			if (!Parse_Splice_Insert()) {
				return false;
			}
		}break;
		case scte35Cmd::time_signal: {
			if (!Parse_Time_Signal()) {
				return false;
			}
		}break;
		case scte35Cmd::bandwidth_reservation: {
			if (!Parse_BW_Reservation()) {
				return false;
			}
		}break;
		case scte35Cmd::private_command: {
			if (!Parse_Private_Command(m_SInfoData.splice_command_length)) {
				return false;
			}
		}break;
		default: {
			return false;//Unknown data
		}
		break;
	}
	return true;
}

std::stringstream CSpliceClass::GetText_Command_Type(const bool& bIfSingleLine) {
	std::stringstream sLocalStr;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	switch (m_SInfoData.splice_command_type)
	{
		case scte35Cmd::splice_null: {
			return GetText_Splice_Null(bIfSingleLine);
		}break;
		case scte35Cmd::splice_schedule: {
			return GetText_Splice_Schedule(bIfSingleLine);
		}break;
		case scte35Cmd::splice_insert: {
			return GetText_Splice_Insert(bIfSingleLine);
		}break;
		case scte35Cmd::time_signal: {
			return GetText_Time_Signal(bIfSingleLine);
		}break;
		case scte35Cmd::bandwidth_reservation: {
			return GetText_BW_Reservation(bIfSingleLine);
		}break;
		case scte35Cmd::private_command: {
			return GetText_Private_Command(bIfSingleLine);
		}break;
		default: {
			sLocalStr << "unknown()"<< sSepLocal;
			return sLocalStr;//Unknown data
		}
		break;
	}

	return sLocalStr;
}

bool CSpliceClass::ParseHexa(const std::string& sInput) {
	
	if (sInput.empty()) {
		return false;
	}

	int nBinLength = sInput.length();
	if (nBinLength % 2 != 0) {//should be of even length
		return false;
	}

	if (!SpacesNotPresent(sInput)) {
		return false;
	}

	std::string sLocalBinary;
	sLocalBinary = HexaStr2BinaryStr(sInput);

	if (ParseBinary(sLocalBinary)) {
		return true;
	}
	return false;
}

bool CSpliceClass::ParseBinary(const std::string& sInput) {
	if (sInput.empty()) {
		return false;
	}

	int nBinLength = sInput.length();
	if (nBinLength % 2 != 0) {//should be of even length
		return false;
	}

	if (!SpacesNotPresent(sInput)) {
		return false;
	}

	ResetValue();
	
	sBinData = sInput;

	if (!ParseSInfoDataFirst()) {
		return false;
	}

	int iOldPoint = iCurrentBitPoint;

	if (!Parse_Command_Type()) {
		return false;
	}

	if (m_SInfoData.section_length == (iCurrentBitPoint - iOldPoint)) {
		//Successfully parsed command section
	}

	//For parsing descriptor section
	if (!ParseSInfoDataSecond()) {
		return false;
	}

	//================================== Descriptors =========================================//

	int iDescLocal = (int)m_SInfoData.descriptor_loop_length * 8;

	if (nBinLength<(iCurrentBitPoint + iDescLocal)) {
		return false;
	}

	sDescriptorData = sInput.substr(iCurrentBitPoint, iDescLocal);
	if (sDescriptorData.empty()) {//May be not present
		//return false;//do nothing
	}
	else {
		if (!Parse_Splice_Descriptor(m_SInfoData.descriptor_loop_length)) {
			return false;
		}
	}

	//================================== Descriptors =========================================//

	iCurrentBitPoint += iDescLocal;

	bool bSuccess = false;

	if (m_SInfoData.encrypted_packet==1) {
		m_SInfoData.E_CRC_32 = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 32, bSuccess);
		if (!bSuccess) { return false; }
	}

	m_SInfoData.CRC_32 = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 32, bSuccess);
	if (!bSuccess) { return false; }

	return true;
}

bool CSpliceClass::ParseBase64(const std::string& sInput) {
	if (sInput.empty()) {
		return false;
	}

	std::string sRetLocal = Base64ToHexa(sInput);
	if (sRetLocal.empty()) {
		return false;
	}
	
	if (ParseHexa(sRetLocal)) {
		return true;
	}

	return false;
}

std::string CSpliceClass::GetText(const bool& bIfSingleLine) {
	std::stringstream sMainString;
	std::string sSepLocal= sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	sMainString << "splice_info_section()"<<sSepLocal;
	sMainString << GetTextSInfoDataFirst(bIfSingleLine).str();

	//Splice Command
	sMainString << GetText_Command_Type(bIfSingleLine).str();

	sMainString << GetTextSInfoDataSecond(bIfSingleLine).str();

	//Descriptors data
	sMainString << GetText_Splice_Descriptor(bIfSingleLine).str();

	if (m_SInfoData.encrypted_packet==1) {
		sMainString << "E_CRC_32="<<m_SInfoData.E_CRC_32 << sSepLocal;
	}

	sMainString << "CRC_32=" << m_SInfoData.CRC_32 << sSepLocal;

	return sMainString.str();
}

void break_duration::ClearValues() {
	 auto_return = PARAMETER_DEF_VAL;
	 reserved = PARAMETER_DEF_VAL;
	 duration = PARAMETER_DEF_VAL;
}


bool CSpliceClass::Parse_Splice_Null() {
	
	//Nothing to do

	return true;

}
std::stringstream CSpliceClass::GetText_Splice_Null(const bool& bIfSingleLine) {
	std::stringstream slocalStr_1;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	slocalStr_1 << "splice_null()" << sSepLocal;
	return slocalStr_1;
}

void SSchComponent::ClearValues() {
	 component_tag = PARAMETER_DEF_VAL;
	 utc_splice_time = PARAMETER_DEF_VAL;
}
void SpliceScheduleChild::ClearValues() {
	 splice_event_id = PARAMETER_DEF_VAL;
	 splice_event_cancel_indicator = PARAMETER_DEF_VAL;
	 reserved_1 = PARAMETER_DEF_VAL;
	 out_of_network_indicator = PARAMETER_DEF_VAL;
	 program_splice_flag = PARAMETER_DEF_VAL;
	 duration_flag = PARAMETER_DEF_VAL;
	 reserved_2 = PARAMETER_DEF_VAL;
	 utc_splice_time = PARAMETER_DEF_VAL; 
	 for (uint32_t i = 0;i < (int)component_count;i++) {
		 v_Component_childs[i].ClearValues();
	 }
	 component_count = PARAMETER_DEF_VAL;
	 v_Component_childs.clear();
	 m_breakD.ClearValues();
	 unique_program_id = PARAMETER_DEF_VAL;
	 avail_num = PARAMETER_DEF_VAL;
	 avails_expected = PARAMETER_DEF_VAL;
}
void SpliceScheduleData::ClearValues() {
	for (uint32_t i = 0;i < splice_count;i++) {
		v_childs[i].ClearValues();
	}
	splice_count = PARAMETER_DEF_VAL;
	v_childs.clear();
}
bool CSpliceClass::Parse_Splice_Schedule() {
	bool bSuccess = false;

	m_objSpliceSchedule.splice_count= GetValue_info_32bit(sBinData, iCurrentBitPoint,true, 8, bSuccess);
	if (!bSuccess) { return false; }

	m_objSpliceSchedule.v_childs.clear();

	if (m_objSpliceSchedule.splice_count>0) {
		for (uint32_t i = 0;i < m_objSpliceSchedule.splice_count;i++) {
			SpliceScheduleChild objLocalSchChild;

			objLocalSchChild.splice_event_id= GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 32, bSuccess);
			if (!bSuccess) { return false; }
			
			objLocalSchChild.splice_event_cancel_indicator = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 1, bSuccess);
			if (!bSuccess) { return false; }

			objLocalSchChild.reserved_1 = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 7, bSuccess);
			if (!bSuccess) { return false; }

			if (objLocalSchChild.splice_event_cancel_indicator == 0) {

				objLocalSchChild.out_of_network_indicator = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 1, bSuccess);
				if (!bSuccess) { return false; }

				objLocalSchChild.program_splice_flag = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 1, bSuccess);
				if (!bSuccess) { return false; }

				objLocalSchChild.duration_flag = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 1, bSuccess);
				if (!bSuccess) { return false; }

				objLocalSchChild.reserved_2 = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 5, bSuccess);
				if (!bSuccess) { return false; }

				if (objLocalSchChild.program_splice_flag == 1) {
					objLocalSchChild.utc_splice_time = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 32, bSuccess);
					if (!bSuccess) { return false; }
				}
				else {
				
					objLocalSchChild.component_count = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 8, bSuccess);
					if (!bSuccess) { return false; }

					if (objLocalSchChild.component_count>0) {
						for (uint32_t j = 0;j < objLocalSchChild.component_count;j++) {
							SSchComponent objLocal;

							objLocal.component_tag= GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 8, bSuccess);
							if (!bSuccess) { return false; }

							objLocal.utc_splice_time = GetValue_info_32bit(sBinData, iCurrentBitPoint, true,32, bSuccess);
							if (!bSuccess) { return false; }

							objLocalSchChild.v_Component_childs.emplace_back(objLocal);
						}
					}
					
				}

				if (objLocalSchChild.duration_flag) {

					objLocalSchChild.m_breakD.auto_return = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 1, bSuccess);
					if (!bSuccess) { return false; }

					objLocalSchChild.m_breakD.reserved = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 6, bSuccess);
					if (!bSuccess) { return false; }

					objLocalSchChild.m_breakD.duration = GetValue_info_64bit(sBinData, iCurrentBitPoint, true, 32, bSuccess);
					if (!bSuccess) { return false; }

				}

				objLocalSchChild.unique_program_id = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 16, bSuccess);
				if (!bSuccess) { return false; }

				objLocalSchChild.avail_num = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 8, bSuccess);
				if (!bSuccess) { return false; }

				objLocalSchChild.avails_expected = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 8, bSuccess);
				if (!bSuccess) { return false; }

			}

			m_objSpliceSchedule.v_childs.emplace_back(objLocalSchChild);
		}
	}

	return true;
}
std::stringstream CSpliceClass::GetText_Splice_Schedule(const bool& bIfSingleLine) {
	std::stringstream slocalStr_1;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	slocalStr_1 << "splice_schedule()" << sSepLocal;
	slocalStr_1 << "splice_count="<<m_objSpliceSchedule.splice_count << sSepLocal;

	if (m_objSpliceSchedule.splice_count > 0) {
		for (uint32_t i = 0;i < m_objSpliceSchedule.splice_count;i++) {
			
			slocalStr_1 << "ssch_sub[" << i + 1 << "]" << sSepLocal;
			slocalStr_1 << "splice_event_id=" << m_objSpliceSchedule.v_childs[i].splice_event_id << sSepLocal;
			slocalStr_1 << "splice_event_cancel_indicator=" << m_objSpliceSchedule.v_childs[i].splice_event_cancel_indicator << sSepLocal;
			slocalStr_1 << "reserved_1=" << m_objSpliceSchedule.v_childs[i].reserved_1 << sSepLocal;
			if (m_objSpliceSchedule.v_childs[i].splice_event_cancel_indicator == 0) {
				slocalStr_1 << "out_of_network_indicator=" << m_objSpliceSchedule.v_childs[i].out_of_network_indicator << sSepLocal;
				slocalStr_1 << "program_splice_flag=" << m_objSpliceSchedule.v_childs[i].program_splice_flag << sSepLocal;
				slocalStr_1 << "duration_flag=" << m_objSpliceSchedule.v_childs[i].duration_flag << sSepLocal;
				slocalStr_1 << "reserved_2=" << m_objSpliceSchedule.v_childs[i].reserved_2 << sSepLocal;

				if (m_objSpliceSchedule.v_childs[i].program_splice_flag == 1) {
					slocalStr_1 << "utc_splice_time=" << m_objSpliceSchedule.v_childs[i].utc_splice_time << sSepLocal;
				}
				else {
					slocalStr_1 << "component_count=" << m_objSpliceSchedule.v_childs[i].component_count << sSepLocal;
					if (m_objSpliceSchedule.v_childs[i].component_count>0) {
						for (uint32_t j = 0;j < m_objSpliceSchedule.v_childs[i].component_count;j++) {
							slocalStr_1 << "ssch_comp[" << j+1<<"]" << sSepLocal;
							slocalStr_1 << "component_tag=" << m_objSpliceSchedule.v_childs[i].v_Component_childs[j].component_tag << sSepLocal;
							slocalStr_1 << "utc_splice_time=" << m_objSpliceSchedule.v_childs[i].v_Component_childs[j].utc_splice_time << sSepLocal;
						}
					}
				}

				if (m_objSpliceSchedule.v_childs[i].duration_flag) {
					slocalStr_1 << "auto_return=" << m_objSpliceSchedule.v_childs[i].m_breakD.auto_return << sSepLocal;
					slocalStr_1 << "reserved=" << m_objSpliceSchedule.v_childs[i].m_breakD.reserved << sSepLocal;
					slocalStr_1 << "duration=" << m_objSpliceSchedule.v_childs[i].m_breakD.duration << sSepLocal;
				}

				slocalStr_1 << "unique_program_id=" << m_objSpliceSchedule.v_childs[i].unique_program_id << sSepLocal;
				slocalStr_1 << "avail_num=" << m_objSpliceSchedule.v_childs[i].avail_num << sSepLocal;
				slocalStr_1 << "avails_expected=" << m_objSpliceSchedule.v_childs[i].avails_expected << sSepLocal;
			}

		}
	}


	return slocalStr_1;
}

void SInComponent::ClearValues() {
	 component_tag=PARAMETER_DEF_VAL;
	 m_spliceT.ClearValues();
}
void SpliceInsertData::ClearValues() {
	 splice_event_id = PARAMETER_DEF_VAL;
	 splice_event_cancel_indicator = PARAMETER_DEF_VAL;
	 reserved_1 = PARAMETER_DEF_VAL;

	 out_of_network_indicator = PARAMETER_DEF_VAL;
	 program_splice_flag = PARAMETER_DEF_VAL;
	 duration_flag = PARAMETER_DEF_VAL;
	 splice_immediate_flag = PARAMETER_DEF_VAL;
	 reserved_2 = PARAMETER_DEF_VAL;

	 m_spliceT.ClearValues();

	 for (uint32_t j = 0;j < component_count;j++) {
		 v_ChildComps[j].ClearValues();
	 }
	 component_count = PARAMETER_DEF_VAL;
	 v_ChildComps.clear();

	 m_breakD.ClearValues();

	 unique_program_id = PARAMETER_DEF_VAL;
	 avail_num = PARAMETER_DEF_VAL;
	 avails_expected = PARAMETER_DEF_VAL;
}
bool CSpliceClass::Parse_Splice_Insert() {
	
	bool bSuccess = false;

	m_objSpliceInsert.splice_event_id= GetValue_info_32bit(sBinData, iCurrentBitPoint, true,32, bSuccess);
	if (!bSuccess) { return false; }

	m_objSpliceInsert.splice_event_cancel_indicator = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 1, bSuccess);
	if (!bSuccess) { return false; }

	m_objSpliceInsert.reserved_1 = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 7, bSuccess);
	if (!bSuccess) { return false; }

	m_objSpliceInsert.v_ChildComps.clear();

	if (m_objSpliceInsert.splice_event_cancel_indicator==0) {
		m_objSpliceInsert.out_of_network_indicator = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 1, bSuccess);
		if (!bSuccess) { return false; }
		m_objSpliceInsert.program_splice_flag = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 1, bSuccess);
		if (!bSuccess) { return false; }

		m_objSpliceInsert.duration_flag = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 1, bSuccess);
		if (!bSuccess) { return false; }

		m_objSpliceInsert.splice_immediate_flag = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 1, bSuccess);
		if (!bSuccess) { return false; }

		m_objSpliceInsert.reserved_2 = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 4, bSuccess);
		if (!bSuccess) { return false; }

		if (m_objSpliceInsert.program_splice_flag==1 && m_objSpliceInsert.splice_immediate_flag==0) {
			
			m_objSpliceInsert.m_spliceT.time_specified_flag= GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 1, bSuccess);
			if (!bSuccess) { return false; }

			if (m_objSpliceInsert.m_spliceT.time_specified_flag==1) {
				m_objSpliceInsert.m_spliceT.reserved_1= GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 6, bSuccess);
				if (!bSuccess) { return false; }

				m_objSpliceInsert.m_spliceT.pts_time = GetValue_info_64bit(sBinData, iCurrentBitPoint, true, 33, bSuccess);
				if (!bSuccess) { return false; }
			}
			else {
				m_objSpliceInsert.m_spliceT.reserved_2 = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 7, bSuccess);
				if (!bSuccess) { return false; }
			}

		}

		if (m_objSpliceInsert.program_splice_flag==0) {
			m_objSpliceInsert.component_count= GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 8, bSuccess);
			if (!bSuccess) { return false; }

			if (m_objSpliceInsert.component_count>0) {
				for (uint32_t i = 0;i < m_objSpliceInsert.component_count;i++) {
					SInComponent objLocal;
					objLocal.component_tag= GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 8, bSuccess);
					if (!bSuccess) { return false; }

					if (m_objSpliceInsert.splice_immediate_flag==0) {			
						objLocal.m_spliceT.time_specified_flag = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 1, bSuccess);
						if (!bSuccess) { return false; }

						if (objLocal.m_spliceT.time_specified_flag == 1) {
							objLocal.m_spliceT.reserved_1 = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 6, bSuccess);
							if (!bSuccess) { return false; }

							objLocal.m_spliceT.pts_time = GetValue_info_64bit(sBinData, iCurrentBitPoint, true, 33, bSuccess);
							if (!bSuccess) { return false; }
						}
						else {
							objLocal.m_spliceT.reserved_2 = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 7, bSuccess);
							if (!bSuccess) { return false; }
						}
					}

					m_objSpliceInsert.v_ChildComps.emplace_back(objLocal);
				}
			}
		}

		if(m_objSpliceInsert.duration_flag==1) {
			m_objSpliceInsert.m_breakD.auto_return= GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 1, bSuccess);
			if (!bSuccess) { return false; }

			m_objSpliceInsert.m_breakD.reserved= GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 6, bSuccess);
			if (!bSuccess) { return false; }

			m_objSpliceInsert.m_breakD.duration = GetValue_info_64bit(sBinData, iCurrentBitPoint, true, 33, bSuccess);
			if (!bSuccess) { return false; }
		}

		m_objSpliceInsert.unique_program_id= GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 16, bSuccess);
		if (!bSuccess) { return false; }

		m_objSpliceInsert.avail_num = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 8, bSuccess);
		if (!bSuccess) { return false; }

		m_objSpliceInsert.avails_expected = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 8, bSuccess);
		if (!bSuccess) { return false; }

	}

	return true;
}
std::stringstream CSpliceClass::GetText_Splice_Insert(const bool& bIfSingleLine) {
	std::stringstream slocalStr_1;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	slocalStr_1 << "splice_insert()" << sSepLocal;

	slocalStr_1 << "splice_event_id=" << m_objSpliceInsert.splice_event_id << sSepLocal;
	slocalStr_1 << "splice_event_cancel_indicator=" << m_objSpliceInsert.splice_event_cancel_indicator << sSepLocal;
	slocalStr_1 << "reserved_1=" << m_objSpliceInsert.reserved_1 << sSepLocal;

	if (m_objSpliceInsert.splice_event_cancel_indicator == 0) {
		slocalStr_1 << "out_of_network_indicator=" << m_objSpliceInsert.out_of_network_indicator << sSepLocal;
		slocalStr_1 << "program_splice_flag=" << m_objSpliceInsert.program_splice_flag << sSepLocal;
		slocalStr_1 << "duration_flag=" << m_objSpliceInsert.duration_flag << sSepLocal;
		slocalStr_1 << "splice_immediate_flag=" << m_objSpliceInsert.splice_immediate_flag << sSepLocal;
		slocalStr_1 << "reserved_2=" << m_objSpliceInsert.reserved_2 << sSepLocal;
		
		if (m_objSpliceInsert.program_splice_flag == 1 && m_objSpliceInsert.splice_immediate_flag == 0) {
			slocalStr_1 << "time_specified_flag=" << m_objSpliceInsert.m_spliceT.time_specified_flag << sSepLocal;

			if(m_objSpliceInsert.m_spliceT.time_specified_flag == 1) {
				slocalStr_1 << "reserved_1=" << m_objSpliceInsert.m_spliceT.reserved_1 << sSepLocal;
				slocalStr_1 << "pts_time=" << m_objSpliceInsert.m_spliceT.pts_time << sSepLocal;
			}
			else {
				slocalStr_1 << "reserved_2=" << m_objSpliceInsert.m_spliceT.reserved_2 << sSepLocal;
			}
		}

		if (m_objSpliceInsert.program_splice_flag == 0) {
			slocalStr_1 << "component_count=" << m_objSpliceInsert.component_count << sSepLocal;
			if (m_objSpliceInsert.component_count > 0) {
				for (uint32_t i = 0;i < m_objSpliceInsert.component_count;i++) {
					slocalStr_1 << "sin_comp["<<i+1<<"]"<< sSepLocal;
					if (m_objSpliceInsert.splice_immediate_flag == 0) {
						slocalStr_1 << "time_specified_flag=" << m_objSpliceInsert.v_ChildComps[i].m_spliceT.time_specified_flag << sSepLocal;
						if (m_objSpliceInsert.v_ChildComps[i].m_spliceT.time_specified_flag==1) {
							slocalStr_1 << "reserved_1=" << m_objSpliceInsert.v_ChildComps[i].m_spliceT.reserved_1 << sSepLocal;
							slocalStr_1 << "pts_time=" << m_objSpliceInsert.v_ChildComps[i].m_spliceT.pts_time << sSepLocal;
						} else {
							slocalStr_1 << "reserved_2=" << m_objSpliceInsert.v_ChildComps[i].m_spliceT.reserved_2 << sSepLocal;
						}
					}
				}
			}
		}
		if (m_objSpliceInsert.duration_flag == 1) {
			slocalStr_1 << "auto_return=" << m_objSpliceInsert.m_breakD.auto_return << sSepLocal;
			slocalStr_1 << "reserved=" << m_objSpliceInsert.m_breakD.reserved << sSepLocal;
			slocalStr_1 << "duration=" << m_objSpliceInsert.m_breakD.duration << sSepLocal;
		}

		slocalStr_1 << "unique_program_id=" << m_objSpliceInsert.unique_program_id << sSepLocal;
		slocalStr_1 << "avail_num=" << m_objSpliceInsert.avail_num << sSepLocal;
		slocalStr_1 << "avails_expected=" << m_objSpliceInsert.avails_expected << sSepLocal;

	}
	return slocalStr_1;
}

void splice_time::ClearValues() {
	 time_specified_flag= PARAMETER_DEF_VAL;
	 reserved_1=PARAMETER_DEF_VAL;
	 pts_time= PARAMETER_DEF_VAL;
	 reserved_2= PARAMETER_DEF_VAL;
}
bool CSpliceClass::Parse_Time_Signal() {
	bool bSuccess = false;

	m_objTimeSignal.time_specified_flag = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 1, bSuccess);
	if (!bSuccess) { return false; }

	if (m_objTimeSignal.time_specified_flag == 1) {
		m_objTimeSignal.reserved_1 = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 6, bSuccess);
		if (!bSuccess) { return false; }

		m_objTimeSignal.pts_time = GetValue_info_64bit(sBinData, iCurrentBitPoint, true, 33, bSuccess);
		if (!bSuccess) { return false; }
	}
	else {
		m_objTimeSignal.reserved_2 = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 7, bSuccess);
		if (!bSuccess) { return false; }
	}

	return true;
}
std::stringstream CSpliceClass::GetText_Time_Signal(const bool& bIfSingleLine) {
	std::stringstream slocalStr_1;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	slocalStr_1 << "time_signal()" << sSepLocal;

	slocalStr_1 << "time_specified_flag=" << m_objTimeSignal.time_specified_flag << sSepLocal;

	if (m_objTimeSignal.time_specified_flag==1) {
		slocalStr_1 << "reserved_1=" << m_objTimeSignal.reserved_1 << sSepLocal;
		slocalStr_1 << "pts_time=" << m_objTimeSignal.pts_time << sSepLocal;
	}
	else {
		slocalStr_1 << "reserved_2=" << m_objTimeSignal.reserved_2 << sSepLocal;
	}

	return slocalStr_1;
}

bool CSpliceClass::Parse_BW_Reservation() {

	//Nothing to do

	return true;

}
std::stringstream CSpliceClass::GetText_BW_Reservation(const bool& bIfSingleLine) {
	std::stringstream slocalStr_1;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	slocalStr_1 << "bandwidth_reservation()" << sSepLocal;

	return slocalStr_1;
}

void private_command::ClearValues() {
	identifier = PARAMETER_DEF_VAL;
	private_byte.clear();
}
bool CSpliceClass::Parse_Private_Command(const uint32_t& splice_command_length_param) {
	bool bSuccess = false;

	m_objPrivateCommand.identifier = GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 32, bSuccess);
	if (!bSuccess) { return false; }

	m_objPrivateCommand.private_length = splice_command_length_param - 4;
	if (!bSuccess) { return false; }

	m_objPrivateCommand.private_byte.clear();
	for (int i = 0;i < (int)m_objPrivateCommand.private_length;i++) {
		uint32_t uiLocal= GetValue_info_32bit(sBinData, iCurrentBitPoint, true, 8, bSuccess);
		if (!bSuccess) { return false; }
		m_objPrivateCommand.private_byte.emplace_back(uiLocal);
	}

	return true;
}
std::stringstream CSpliceClass::GetText_Private_Command(const bool& bIfSingleLine) {
	std::stringstream slocalStr_1;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	slocalStr_1 << "private_command()" << sSepLocal;

	slocalStr_1 << "identifier=" << m_objPrivateCommand.identifier << sSepLocal;
	slocalStr_1 << "private_length=" << m_objPrivateCommand.private_length << sSepLocal;

	for (int i = 0;i < (int)m_objPrivateCommand.private_length;i++) {
		slocalStr_1 << "private_byte["<<i+1<<"]="<< m_objPrivateCommand.private_byte[i]<<sSepLocal;
	}

	return slocalStr_1;
}

void AvailDescriptor::ClearValues() {
	 splice_descriptor_tag = scte35Desc::unknown;
	 descriptor_length =PARAMETER_DEF_VAL;
	 identifier = PARAMETER_DEF_VAL;
	 provider_avail_id = PARAMETER_DEF_VAL;
}
bool CSpliceClass::Parse_Avail_Descriptor(const std::string& sInData) {
	bool bSuccess = false;

	AvailDescriptor* pobjLocal=new AvailDescriptor();

	int iLocalCur = 0;

	pobjLocal->splice_descriptor_tag = GetValue_Desc_32bit(sInData, iLocalCur,true, 8, bSuccess);
	if (!bSuccess) { goto AVAIL_D_FAILED; }

	pobjLocal->descriptor_length = GetValue_Desc_32bit(sInData, iLocalCur,true, 8, bSuccess);
	if (!bSuccess) { goto AVAIL_D_FAILED; }

	pobjLocal->identifier = GetValue_Desc_32bit(sInData, iLocalCur,true, 32, bSuccess);
	if (!bSuccess) { goto AVAIL_D_FAILED; }

	pobjLocal->provider_avail_id = GetValue_Desc_32bit(sInData, iLocalCur,true, 32, bSuccess);
	if (!bSuccess) { goto AVAIL_D_FAILED; }

	m_objSpliceDescriptor.v_SDChilds.emplace_back(pobjLocal);

	return true;

AVAIL_D_FAILED:
	pobjLocal->ClearValues();
	delete pobjLocal;

	return false;

}
std::stringstream CSpliceClass::GetText_Avail_Descriptor(SDBase* paramData, const bool& bIfSingleLine) {
	std::stringstream slocalStr_2;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	AvailDescriptor* pReadLocal = static_cast<AvailDescriptor*>(paramData);

	slocalStr_2 << "avail_descriptor()" << sSepLocal;
	slocalStr_2 << "splice_descriptor_tag=" << pReadLocal->splice_descriptor_tag << sSepLocal;
	slocalStr_2 << "descriptor_length=" << pReadLocal->descriptor_length << sSepLocal;
	slocalStr_2 << "identifier=" << pReadLocal->identifier << sSepLocal;
	slocalStr_2 << "provider_avail_id=" << pReadLocal->provider_avail_id << sSepLocal;


	return slocalStr_2;
}

void DTMFDescriptor::ClearValues() {
	splice_descriptor_tag = scte35Desc::unknown;
	descriptor_length = PARAMETER_DEF_VAL;
	identifier = PARAMETER_DEF_VAL;
	preroll = PARAMETER_DEF_VAL;
	dtmf_count = PARAMETER_DEF_VAL;
	reserved = PARAMETER_DEF_VAL;
	v_DTMF_Char.clear();
}
bool CSpliceClass::Parse_DTMF_Descriptor(const std::string& sInData) {
	bool bSuccess = false;

	DTMFDescriptor* pobjDTMFLocal=new DTMFDescriptor();

	int iLocalCur=0;

	pobjDTMFLocal->splice_descriptor_tag = GetValue_Desc_32bit(sInData, iLocalCur,true, 8, bSuccess);
	if (!bSuccess) {goto DTMF_D_FAILED; }

	pobjDTMFLocal->descriptor_length = GetValue_Desc_32bit(sInData, iLocalCur,true, 8, bSuccess);
	if (!bSuccess) { goto DTMF_D_FAILED; }

	pobjDTMFLocal->identifier = GetValue_Desc_32bit(sInData, iLocalCur,true, 32, bSuccess);
	if (!bSuccess) { goto DTMF_D_FAILED; }

	pobjDTMFLocal->preroll= GetValue_Desc_32bit(sInData, iLocalCur,true, 8, bSuccess);
	if (!bSuccess) { goto DTMF_D_FAILED; }

	pobjDTMFLocal->dtmf_count = GetValue_Desc_32bit(sInData, iLocalCur,true, 3, bSuccess);
	if (!bSuccess) { goto DTMF_D_FAILED; }

	pobjDTMFLocal->reserved = GetValue_Desc_32bit(sInData, iLocalCur,true, 5, bSuccess);
	if (!bSuccess) { goto DTMF_D_FAILED; }

	for (int a = 0;a < (int)pobjDTMFLocal->dtmf_count;a++) {
		uint32_t ValueLocal = GetValue_Desc_32bit(sInData, iLocalCur,true, 8, bSuccess);
		if (!bSuccess) { goto DTMF_D_FAILED; }
		pobjDTMFLocal->v_DTMF_Char.emplace_back(ValueLocal);
	}

	m_objSpliceDescriptor.v_SDChilds.emplace_back(pobjDTMFLocal);

	return true;

DTMF_D_FAILED:
	pobjDTMFLocal->ClearValues();
	delete pobjDTMFLocal;

	return false;

}
std::stringstream CSpliceClass::GetText_DTMF_Descriptor(SDBase* paramData, const bool& bIfSingleLine) {
	std::stringstream slocalStr_2;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	DTMFDescriptor* pReadLocal = static_cast<DTMFDescriptor*>(paramData);

	slocalStr_2 << "DTMF_descriptor()" << sSepLocal;
	slocalStr_2 << "splice_descriptor_tag=" << pReadLocal->splice_descriptor_tag << sSepLocal;
	slocalStr_2 << "descriptor_length=" << pReadLocal->descriptor_length << sSepLocal;
	slocalStr_2 << "identifier=" << pReadLocal->identifier << sSepLocal;
	slocalStr_2 << "preroll=" << pReadLocal->preroll << sSepLocal;
	slocalStr_2 << "dtmf_count=" << pReadLocal->dtmf_count << sSepLocal;
	slocalStr_2 << "reserved=" << pReadLocal->reserved << sSepLocal;

	for (int a = 0;a < (int)pReadLocal->dtmf_count;a++) {
		slocalStr_2 << "DTMF_char["<<a+1<<"]=" << pReadLocal->v_DTMF_Char[a] << sSepLocal;
	}

	return slocalStr_2;
}

void SegDescComponent::ClearValues() {
	 component_tag = PARAMETER_DEF_VAL;
	 reserved = PARAMETER_DEF_VAL;
	 pts_offset = PARAMETER_DEF_VAL;
}
void SegmentationDescriptor::ClearValues() {
	splice_descriptor_tag = scte35Desc::unknown;
	descriptor_length = PARAMETER_DEF_VAL;
	identifier = PARAMETER_DEF_VAL;
	 segmentation_event_id = PARAMETER_DEF_VAL;
	 segmentation_event_cancel_indicator = PARAMETER_DEF_VAL;
	 reserved_1 = PARAMETER_DEF_VAL;
	 program_segmentation_flag = PARAMETER_DEF_VAL;
	 segmentation_duration_flag;
	 delivery_not_restricted_flag = PARAMETER_DEF_VAL;
	 web_delivery_allowed_flag= PARAMETER_DEF_VAL;
	 no_regional_blackout_flag= PARAMETER_DEF_VAL;
	 archive_allowed_flag= PARAMETER_DEF_VAL;
	 device_restrictions= PARAMETER_DEF_VAL;
	 reserved_2= PARAMETER_DEF_VAL;

	 for (uint32_t i = 0;i < component_count;i++) {
		 v_ChildComps[i].ClearValues();
	 }
	 v_ChildComps.clear();
	 component_count = PARAMETER_DEF_VAL;

	 segmentation_duration= PARAMETER_DEF_VAL;
	 segmentation_upid_type= PARAMETER_DEF_VAL;
	 segmentation_upid_length= PARAMETER_DEF_VAL;
	 v_upid_list.clear();

	 segmentation_type_id = PARAMETER_DEF_VAL;
	 segment_num = PARAMETER_DEF_VAL;
	 segments_expected = PARAMETER_DEF_VAL;
	 sub_segment_num = PARAMETER_DEF_VAL;
	 sub_segments_expected = PARAMETER_DEF_VAL;
}
bool CSpliceClass::Parse_Segmentation_Descriptor(const std::string& sInData) {
	
	bool bSuccess = false;

	SegmentationDescriptor* pobjLocal = new SegmentationDescriptor();

	int iLocalCur = 0;

	pobjLocal->splice_descriptor_tag= GetValue_Desc_32bit(sInData, iLocalCur,true, 8, bSuccess);
	if (!bSuccess) { goto SEG_D_FAILED; }

	pobjLocal->descriptor_length = GetValue_Desc_32bit(sInData, iLocalCur, true, 8, bSuccess);
	if (!bSuccess) { goto SEG_D_FAILED; }

	pobjLocal->identifier = GetValue_Desc_32bit(sInData, iLocalCur, true, 32, bSuccess);
	if (!bSuccess) { goto SEG_D_FAILED; }

	if (pobjLocal->identifier != 0x43554549) {
		goto SEG_D_FAILED; /*Not SCTE CUEI*/
	}

	pobjLocal->segmentation_event_id = GetValue_Desc_32bit(sInData, iLocalCur, true, 32, bSuccess);
	if (!bSuccess) { goto SEG_D_FAILED; }

	pobjLocal->segmentation_event_cancel_indicator = GetValue_Desc_32bit(sInData, iLocalCur, true, 1, bSuccess);
	if (!bSuccess) { goto SEG_D_FAILED; }

	pobjLocal->reserved_1 = GetValue_Desc_32bit(sInData, iLocalCur, true, 7, bSuccess);
	if (!bSuccess) { goto SEG_D_FAILED; }

	if (pobjLocal->segmentation_event_cancel_indicator==0) {
		pobjLocal->program_segmentation_flag= GetValue_Desc_32bit(sInData, iLocalCur, true, 1, bSuccess);
		if (!bSuccess) { goto SEG_D_FAILED; }

		pobjLocal->segmentation_duration_flag = GetValue_Desc_32bit(sInData, iLocalCur, true, 1, bSuccess);
		if (!bSuccess) { goto SEG_D_FAILED; }

		pobjLocal->delivery_not_restricted_flag = GetValue_Desc_32bit(sInData, iLocalCur, true, 1, bSuccess);
		if (!bSuccess) { goto SEG_D_FAILED; }

		if (pobjLocal->delivery_not_restricted_flag==0) {
			pobjLocal->web_delivery_allowed_flag = GetValue_Desc_32bit(sInData, iLocalCur, true, 1, bSuccess);
			if (!bSuccess) { goto SEG_D_FAILED; }

			pobjLocal->no_regional_blackout_flag = GetValue_Desc_32bit(sInData, iLocalCur, true, 1, bSuccess);
			if (!bSuccess) { goto SEG_D_FAILED; }

			pobjLocal->archive_allowed_flag = GetValue_Desc_32bit(sInData, iLocalCur, true, 1, bSuccess);
			if (!bSuccess) { goto SEG_D_FAILED; }

			pobjLocal->device_restrictions = GetValue_Desc_32bit(sInData, iLocalCur, true, 2, bSuccess);
			if (!bSuccess) { goto SEG_D_FAILED; }
		}
		else {
			pobjLocal->reserved_2 = GetValue_Desc_32bit(sInData, iLocalCur, true, 5, bSuccess);
			if (!bSuccess) { goto SEG_D_FAILED; }
		}


		if (pobjLocal->program_segmentation_flag==0) {
			pobjLocal->component_count = GetValue_Desc_32bit(sInData, iLocalCur, true, 8, bSuccess);
			if (!bSuccess) { goto SEG_D_FAILED; }

			for (int h = 0;h < (int)pobjLocal->component_count;h++) {
				SegDescComponent objSDC;

				objSDC.component_tag = GetValue_Desc_32bit(sInData, iLocalCur, true, 8, bSuccess);
				if (!bSuccess) { goto SEG_D_FAILED; }

				objSDC.reserved = GetValue_Desc_32bit(sInData, iLocalCur, true, 7, bSuccess);
				if (!bSuccess) { goto SEG_D_FAILED; }

				objSDC.pts_offset = GetValue_Desc_32bit(sInData, iLocalCur, true, 33, bSuccess);
				if (!bSuccess) { goto SEG_D_FAILED; }

				pobjLocal->v_ChildComps.emplace_back(objSDC);

			}
			
		}

		if (pobjLocal->segmentation_duration_flag==1) {
			pobjLocal->segmentation_duration = GetValue_Desc_64bit(sInData, iLocalCur, true, 40, bSuccess);
			if (!bSuccess) { goto SEG_D_FAILED; }
		}

		pobjLocal->segmentation_upid_type = GetValue_Desc_32bit(sInData, iLocalCur, true, 8, bSuccess);
		if (!bSuccess) { goto SEG_D_FAILED; }
		
		pobjLocal->segmentation_upid_length = GetValue_Desc_32bit(sInData, iLocalCur, true, 8, bSuccess);
		if (!bSuccess) { goto SEG_D_FAILED; }

		for (int g = 0;g < (int)pobjLocal->segmentation_upid_length;g++) {
			uint32_t uVal= GetValue_Desc_32bit(sInData, iLocalCur, true, 8, bSuccess);
			if (!bSuccess) { goto SEG_D_FAILED; }

			pobjLocal->v_upid_list.emplace_back(uVal);
		}

		pobjLocal->segmentation_type_id = GetValue_Desc_32bit(sInData, iLocalCur, true, 8, bSuccess);
		if (!bSuccess) { goto SEG_D_FAILED; }

		pobjLocal->segment_num = GetValue_Desc_32bit(sInData, iLocalCur, true, 8, bSuccess);
		if (!bSuccess) { goto SEG_D_FAILED; }

		pobjLocal->segments_expected = GetValue_Desc_32bit(sInData, iLocalCur, true, 8, bSuccess);
		if (!bSuccess) { goto SEG_D_FAILED; }

		if (
			pobjLocal->segmentation_type_id==0x34 ||
			pobjLocal->segmentation_type_id==0x36 ||
			pobjLocal->segmentation_type_id==0x38 ||
			pobjLocal->segmentation_type_id==0x3A
			) {
			
			pobjLocal->sub_segment_num = GetValue_Desc_32bit(sInData, iLocalCur, true, 8, bSuccess);
			//if (!bSuccess) { goto SEG_D_FAILED; }

			pobjLocal->sub_segments_expected = GetValue_Desc_32bit(sInData, iLocalCur, true, 8, bSuccess);
			//if (!bSuccess) { goto SEG_D_FAILED; }

		}
	}

	m_objSpliceDescriptor.v_SDChilds.emplace_back(pobjLocal);

	return true;

SEG_D_FAILED:
	pobjLocal->ClearValues();
	delete pobjLocal;
	return false;

}
std::stringstream CSpliceClass::GetText_Segmentation_Descriptor(SDBase* paramData, const bool& bIfSingleLine) {
	std::stringstream slocalStr_2;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	SegmentationDescriptor* pReadLocal = static_cast<SegmentationDescriptor*>(paramData);

	slocalStr_2 << "segmentation_descriptor()" << sSepLocal;
	slocalStr_2 << "splice_descriptor_tag=" << pReadLocal->splice_descriptor_tag << sSepLocal;
	slocalStr_2 << "descriptor_length=" << pReadLocal->descriptor_length << sSepLocal;
	slocalStr_2 << "identifier=" << pReadLocal->identifier << sSepLocal;
	slocalStr_2 << "segmentation_event_id=" << pReadLocal->segmentation_event_id << sSepLocal;
	slocalStr_2 << "segmentation_event_cancel_indicator=" << pReadLocal->segmentation_event_cancel_indicator << sSepLocal;
	slocalStr_2 << "reserved_1=" << pReadLocal->reserved_1 << sSepLocal;

	if (pReadLocal->segmentation_event_cancel_indicator == 0) {
		
		slocalStr_2 << "program_segmentation_flag=" << pReadLocal->program_segmentation_flag << sSepLocal;
		slocalStr_2 << "segmentation_duration_flag=" << pReadLocal->segmentation_duration_flag << sSepLocal;
		slocalStr_2 << "delivery_not_restricted_flag=" << pReadLocal->delivery_not_restricted_flag << sSepLocal;
		
		if (pReadLocal->delivery_not_restricted_flag==0) {
			slocalStr_2 << "web_delivery_allowed_flag=" << pReadLocal->web_delivery_allowed_flag << sSepLocal;
			slocalStr_2 << "no_regional_blackout_flag=" << pReadLocal->no_regional_blackout_flag << sSepLocal;
			slocalStr_2 << "archive_allowed_flag=" << pReadLocal->archive_allowed_flag << sSepLocal;
			slocalStr_2 << "device_restrictions=" << pReadLocal->device_restrictions << sSepLocal;
		}
		else {
			slocalStr_2 << "reserved_2=" << pReadLocal->reserved_2 << sSepLocal;
		}

		if (pReadLocal->program_segmentation_flag == 0) {
			
			slocalStr_2 << "component_count=" << pReadLocal->component_count << sSepLocal;
			for (int h = 0;h < (int)pReadLocal->component_count;h++) {
				
				slocalStr_2 << "segdesc_comp["<<h+1 <<"]"<< sSepLocal;
				slocalStr_2 << "component_tag=" << pReadLocal->v_ChildComps[h].component_tag << sSepLocal;
				slocalStr_2 << "reserved=" << pReadLocal->v_ChildComps[h].reserved << sSepLocal;
				slocalStr_2 << "pts_offset=" << pReadLocal->v_ChildComps[h].pts_offset << sSepLocal;

			}

		}

		if (pReadLocal->segmentation_duration_flag == 1) {
			slocalStr_2 << "segmentation_duration=" << pReadLocal->segmentation_duration << sSepLocal;
		}

		slocalStr_2 << "segmentation_upid_type=" << pReadLocal->segmentation_upid_type << sSepLocal;
		slocalStr_2 << "segmentation_upid_length=" << pReadLocal->segmentation_upid_length << sSepLocal;

		slocalStr_2 << "segmentation_upid()"  << sSepLocal;
		for (int g = 0;g < (int)pReadLocal->segmentation_upid_length;g++) {
			slocalStr_2 << "upid_val[" << g + 1 << "]=" << pReadLocal->v_upid_list[g]<< sSepLocal;
		}

		slocalStr_2 << "segmentation_type_id=" << pReadLocal->segmentation_type_id << sSepLocal;
		slocalStr_2 << "segment_num=" << pReadLocal->segment_num << sSepLocal;
		slocalStr_2 << "segments_expected=" << pReadLocal->segments_expected << sSepLocal;

		if (
			pReadLocal->segmentation_type_id == 0x34 ||
			pReadLocal->segmentation_type_id == 0x36 ||
			pReadLocal->segmentation_type_id == 0x38 ||
			pReadLocal->segmentation_type_id == 0x3A
		) {
			slocalStr_2 << "sub_segment_num=" << pReadLocal->sub_segment_num << sSepLocal;
			slocalStr_2 << "sub_segments_expected=" << pReadLocal->sub_segments_expected << sSepLocal;
		}
	}
	return slocalStr_2;
}

void TimeDescriptor::ClearValues() {
	splice_descriptor_tag = scte35Desc::unknown;
	descriptor_length = PARAMETER_DEF_VAL;
	identifier = PARAMETER_DEF_VAL;
	 TAI_seconds = PARAMETER_DEF_VAL;
	 TAI_ns = PARAMETER_DEF_VAL;
	 UTC_offset = PARAMETER_DEF_VAL;
}
bool CSpliceClass::Parse_Time_Descriptor(const std::string& sInData) {
	bool bSuccess = false;

	TimeDescriptor* pobjTimeDesc=new TimeDescriptor();

	int iLocalCur = 0;

	pobjTimeDesc->splice_descriptor_tag = GetValue_Desc_32bit(sInData, iLocalCur,true, 8, bSuccess);
	if (!bSuccess) { goto TIME_D_FAILED; }

	pobjTimeDesc->descriptor_length = GetValue_Desc_32bit(sInData, iLocalCur,true, 8, bSuccess);
	if (!bSuccess) { goto TIME_D_FAILED; }

	pobjTimeDesc->identifier = GetValue_Desc_32bit(sInData, iLocalCur,true, 32, bSuccess);
	if (!bSuccess) { goto TIME_D_FAILED; }

	pobjTimeDesc->TAI_seconds = GetValue_Desc_64bit(sInData, iLocalCur,true, 48, bSuccess);
	if (!bSuccess) { goto TIME_D_FAILED; }

	pobjTimeDesc->TAI_ns = GetValue_Desc_32bit(sInData, iLocalCur,true, 32, bSuccess);
	if (!bSuccess) { goto TIME_D_FAILED; }

	pobjTimeDesc->UTC_offset = GetValue_Desc_32bit(sInData, iLocalCur,true, 16, bSuccess);
	if (!bSuccess) { goto TIME_D_FAILED; }

	m_objSpliceDescriptor.v_SDChilds.emplace_back(pobjTimeDesc);

	return true;

TIME_D_FAILED:
	pobjTimeDesc->ClearValues();
	delete pobjTimeDesc;
	return false;

}
std::stringstream CSpliceClass::GetText_Time_Descriptor(SDBase* paramData, const bool& bIfSingleLine) {
	std::stringstream slocalStr_2;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	TimeDescriptor* pReadLocal = static_cast<TimeDescriptor*>(paramData);

	slocalStr_2 << "time_descriptor()" << sSepLocal;
	slocalStr_2 << "splice_descriptor_tag=" << pReadLocal->splice_descriptor_tag << sSepLocal;
	slocalStr_2 << "descriptor_length=" << pReadLocal->descriptor_length << sSepLocal;
	slocalStr_2 << "identifier=" << pReadLocal->identifier << sSepLocal;
	slocalStr_2 << "TAI_seconds=" << pReadLocal->TAI_seconds << sSepLocal;
	slocalStr_2 << "TAI_ns=" << pReadLocal->TAI_ns << sSepLocal;
	slocalStr_2 << "UTC_offset=" << pReadLocal->UTC_offset << sSepLocal;

	return slocalStr_2;
}

void AudioDescComponent::ClearValues() {
	 component_tag = PARAMETER_DEF_VAL;
	 ISO_code = PARAMETER_DEF_VAL;
	 Bit_Stream_Mode = PARAMETER_DEF_VAL;
	 Num_Channels = PARAMETER_DEF_VAL;
	 Full_Srvc_Audio = PARAMETER_DEF_VAL;
}
void AudioDescriptor::ClearValues() {
	splice_descriptor_tag = scte35Desc::unknown;
	descriptor_length = PARAMETER_DEF_VAL;
	identifier = PARAMETER_DEF_VAL;
	reserved = PARAMETER_DEF_VAL;
	for (uint32_t i = 0;i < audio_count;i++) {
		v_audio_comp_list[i].ClearValues();
	}
	v_audio_comp_list.clear();
}
bool CSpliceClass::Parse_Audio_Descriptor(const std::string& sInData) {
	
	bool bSuccess = false;

	AudioDescriptor* pobjAudioDesc=new AudioDescriptor();

	int iLocalCur = 0;

	pobjAudioDesc->splice_descriptor_tag = GetValue_Desc_32bit(sInData, iLocalCur,true, 8, bSuccess);
	if (!bSuccess) { goto AUDIO_D_FAILED; }

	pobjAudioDesc->descriptor_length = GetValue_Desc_32bit(sInData, iLocalCur,true, 8, bSuccess);
	if (!bSuccess) { goto AUDIO_D_FAILED; }

	pobjAudioDesc->identifier = GetValue_Desc_32bit(sInData, iLocalCur,true, 32, bSuccess);
	if (!bSuccess) { goto AUDIO_D_FAILED; }

	pobjAudioDesc->audio_count = GetValue_Desc_32bit(sInData, iLocalCur,true, 4, bSuccess);
	if (!bSuccess) { goto AUDIO_D_FAILED; }

	pobjAudioDesc->reserved = GetValue_Desc_32bit(sInData, iLocalCur,true, 4, bSuccess);
	if (!bSuccess) { goto AUDIO_D_FAILED; }

	pobjAudioDesc->v_audio_comp_list.clear();

	for (int i = 0;i < (int)pobjAudioDesc->audio_count;i++) {
		AudioDescComponent objLocal;

		objLocal.component_tag = GetValue_Desc_32bit(sInData, iLocalCur,true, 8, bSuccess);
		if (!bSuccess) { goto AUDIO_D_FAILED; }

		objLocal.ISO_code = GetValue_Desc_32bit(sInData, iLocalCur,true, 24, bSuccess);
		if (!bSuccess) { goto AUDIO_D_FAILED; }

		objLocal.Bit_Stream_Mode = GetValue_Desc_32bit(sInData, iLocalCur,true, 3, bSuccess);
		if (!bSuccess) { goto AUDIO_D_FAILED; }

		objLocal.Num_Channels = GetValue_Desc_32bit(sInData, iLocalCur,true, 4, bSuccess);
		if (!bSuccess) { goto AUDIO_D_FAILED; }

		objLocal.Full_Srvc_Audio = GetValue_Desc_32bit(sInData, iLocalCur,true, 1, bSuccess);
		if (!bSuccess) { goto AUDIO_D_FAILED; }

		pobjAudioDesc->v_audio_comp_list.emplace_back(objLocal);

	}

	m_objSpliceDescriptor.v_SDChilds.emplace_back(pobjAudioDesc);

	return true;
AUDIO_D_FAILED:
	pobjAudioDesc->ClearValues();
	delete pobjAudioDesc;
	return false;

}
std::stringstream CSpliceClass::GetText_Audio_Descriptor(SDBase* paramData, const bool& bIfSingleLine) {
	std::stringstream slocalStr_2;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	AudioDescriptor* pReadLocal = static_cast<AudioDescriptor*>(paramData);

	slocalStr_2 << "audio_descriptor()" << sSepLocal;
	slocalStr_2 << "splice_descriptor_tag=" << pReadLocal->splice_descriptor_tag << sSepLocal;
	slocalStr_2 << "descriptor_length=" << pReadLocal->descriptor_length << sSepLocal;
	slocalStr_2 << "identifier=" << pReadLocal->identifier << sSepLocal;
	slocalStr_2 << "audio_count=" << pReadLocal->audio_count << sSepLocal;
	slocalStr_2 << "reserved=" << pReadLocal->reserved << sSepLocal;

	for (int i = 0;i < (int)pReadLocal->audio_count;i++) {
		
		slocalStr_2 << "audiodesc_comp["<<i+1<<"]" << sSepLocal;
		slocalStr_2 << "component_tag=" << pReadLocal->v_audio_comp_list[i].component_tag << sSepLocal;
		slocalStr_2 << "ISO_code=" << pReadLocal->v_audio_comp_list[i].ISO_code << sSepLocal;
		slocalStr_2 << "Bit_Stream_Mode=" << pReadLocal->v_audio_comp_list[i].Bit_Stream_Mode << sSepLocal;
		slocalStr_2 << "Num_Channels=" << pReadLocal->v_audio_comp_list[i].Num_Channels << sSepLocal;
		slocalStr_2 << "Full_Srvc_Audio=" << pReadLocal->v_audio_comp_list[i].Full_Srvc_Audio << sSepLocal;

	}

	return slocalStr_2;
}

void CSpliceClass::Clear_Splice_Descriptor() {
	
	std::size_t iLenLocal = m_objSpliceDescriptor.v_SDChilds.size();
	for (std::size_t i = 0;i < iLenLocal;i++) {
		uint32_t iLocalType = m_objSpliceDescriptor.v_SDChilds[i]->splice_descriptor_tag;
		switch (iLocalType)
		{
			case scte35Desc::avail_descriptor: {
				AvailDescriptor* pLocal = static_cast<AvailDescriptor*>(m_objSpliceDescriptor.v_SDChilds[i]);
				pLocal->ClearValues();
				delete pLocal;
			}break;
			case scte35Desc::dtmf_descriptor: {
				DTMFDescriptor* pLocal = static_cast<DTMFDescriptor*>(m_objSpliceDescriptor.v_SDChilds[i]);
				pLocal->ClearValues();
				delete pLocal;
			}break;
			case scte35Desc::segmentation_descriptor: {
				SegmentationDescriptor* pLocal = static_cast<SegmentationDescriptor*>(m_objSpliceDescriptor.v_SDChilds[i]);
				pLocal->ClearValues();
				delete pLocal;
			}break;
			case scte35Desc::time_descriptor: {
				TimeDescriptor* pLocal = static_cast<TimeDescriptor*>(m_objSpliceDescriptor.v_SDChilds[i]);
				pLocal->ClearValues();
				delete pLocal;
			}break;
			case scte35Desc::audio_descriptor: {
				AudioDescriptor* pLocal = static_cast<AudioDescriptor*>(m_objSpliceDescriptor.v_SDChilds[i]);
				pLocal->ClearValues();
				delete pLocal;
			}break;
			default:
			{
				//Nothing to do
			}break;
		}
	}
	m_objSpliceDescriptor.v_SDChilds.clear();
}
bool CSpliceClass::Parse_Splice_Descriptor(const uint32_t& loopLength) {
	bool bSuccess = false;

	if (sDescriptorData.empty()) {
		return false;
	}

	int iLocalCur = 0;
	int iLocalOneTimeStart = 0;

	uint32_t local_splice_descriptor_tag;
	uint32_t local_descriptor_length;

	uint32_t bitsRead = 0;

	std::vector<std::string> v_LocalIndi;
	std::vector<uint32_t> v_tag_list;

	while (bitsRead<loopLength){
		
		iLocalOneTimeStart = iLocalCur;

		local_splice_descriptor_tag = GetValue_Desc_32bit(sDescriptorData, iLocalCur,false, 8, bSuccess);
		if (!bSuccess) { return false; }
		iLocalCur += 8;

		local_descriptor_length = GetValue_Desc_32bit(sDescriptorData, iLocalCur,false, 8, bSuccess);
		if (!bSuccess) { return false; }
		iLocalCur += 8;

		iLocalCur += (local_descriptor_length *8);/* Identifier + Private Bytes */

		std::string sLocal = sDescriptorData.substr(iLocalOneTimeStart,(iLocalCur- iLocalOneTimeStart));

		v_tag_list.emplace_back(local_splice_descriptor_tag);
		v_LocalIndi.emplace_back(sLocal);

		bitsRead += local_descriptor_length + 2;
	}

	std::size_t iVectorSize = v_LocalIndi.size();
	if (iVectorSize>0) {
		
		for (std::size_t k = 0;k < iVectorSize; k++) {

			if (v_tag_list[k] == scte35Desc::avail_descriptor) {
				if (!Parse_Avail_Descriptor(v_LocalIndi[k])) {
					return false;
				}
			}
			else if(v_tag_list[k] == scte35Desc::dtmf_descriptor) {
				if (!Parse_DTMF_Descriptor(v_LocalIndi[k])) {
					return false;
				}
			}
			else if (v_tag_list[k] == scte35Desc::segmentation_descriptor) {
				if (!Parse_Segmentation_Descriptor(v_LocalIndi[k])) {
					return false;
				}
			}
			else if (v_tag_list[k] == scte35Desc::time_descriptor) {
				if(!Parse_Time_Descriptor(v_LocalIndi[k])) {
					return false;
				}
			}
			else if(v_tag_list[k] == scte35Desc::audio_descriptor) {
				if (!Parse_Audio_Descriptor(v_LocalIndi[k])) {
					return false;
				}
			}
			else {
				//Not Supported
			}

		}

	}

	
	return true;


}
std::stringstream CSpliceClass::GetText_Splice_Descriptor(const bool& bIfSingleLine) {
	std::stringstream slocalStr_2;
	std::string sSepLocal = sNextLine;
	if (bIfSingleLine) {
		sSepLocal = sCommaOnly;
	}

	slocalStr_2 << "splice_descriptor()" << sSepLocal;

	for (std::size_t k = 0;k < m_objSpliceDescriptor.v_SDChilds.size(); k++) {

		slocalStr_2 << "sd[" << k + 1 << "]" << sSepLocal;

		switch (m_objSpliceDescriptor.v_SDChilds[k]->splice_descriptor_tag)
		{
			case scte35Desc::avail_descriptor: {
				slocalStr_2 << GetText_Avail_Descriptor(m_objSpliceDescriptor.v_SDChilds[k],bIfSingleLine).str();
			}break;
			case scte35Desc::dtmf_descriptor: {
				slocalStr_2 << GetText_DTMF_Descriptor(m_objSpliceDescriptor.v_SDChilds[k], bIfSingleLine).str();
			}break;
			case scte35Desc::segmentation_descriptor: {
				slocalStr_2 << GetText_Segmentation_Descriptor(m_objSpliceDescriptor.v_SDChilds[k], bIfSingleLine).str();
			}break;
			case scte35Desc::time_descriptor: {
				slocalStr_2 << GetText_Time_Descriptor(m_objSpliceDescriptor.v_SDChilds[k], bIfSingleLine).str();
			}break;
			case scte35Desc::audio_descriptor: {
				slocalStr_2 << GetText_Audio_Descriptor(m_objSpliceDescriptor.v_SDChilds[k], bIfSingleLine).str();
			}break;
			default: {
				slocalStr_2 << "Unknown()" << sSepLocal;
			}
			break;
		}
	}

	return slocalStr_2;
}

};