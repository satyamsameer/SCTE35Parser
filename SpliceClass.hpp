/*! \file SpliceClass.hpp
*	\brief Splice parser class declaration.
*	\details Splice parser class declaration header file. Header file for declaration of class SpliceClass whose definition is in SpliceClass.cpp.
*	\author https://github.com/satyamsameer
*	\version 1.0.0.0
*	\date 27-07-2021
*/

#ifndef _SPLICECLASS_HPP_
/*! \brief SpliceClass.hpp header guard define.
*/
#define _SPLICECLASS_HPP_

#include <string>
#include <vector>

/*! \namespace scte35
*	\brief scte35 namespace to keep clean the global scope.
*/
namespace scte35 {

/*! \def PARAMETER_DEF_VAL
*	\brief Default values for all the variables currently set to 0.
*/
#define PARAMETER_DEF_VAL 0

/*! \relates SpliceClass
*	\brief Global function to return check for special characters
*	\param ch The character to check as input
*	\returns boolean true if special character is present and false if not
*/
bool IsSpace(unsigned char ch);

/*! \struct scte35Cmd
*	\brief scte35Cmd structure to hold splice command values.
*	
*	This structure contains the enum values Unknown, Splice Null, Splice Insert, Splice Schedule, Time Signal, Bandwidth Reservation and Private Command
*	splice command values which are possible in current SCTE-35 standard.
*/
struct scte35Cmd {
	/*! 
	*	\brief A type definition for type (Splice Commands)
	*/
	typedef enum {
		unknown = 999,					/*!<Unknown Command (Not in standard but high number taken as garbage/default value)*/
		splice_null = 0x00,				/*!<Splice Null 0x00*/
		/*! \brief Splice Schedule 0x04 (Reserved from 0x01 to 0x03)
		*/
		splice_schedule = 0x04,			
		splice_insert = 0x05,			/*!<Splice Insert 0x05*/
		time_signal = 0x06,				/*!<Time Signal 0x06*/
		bandwidth_reservation = 0x07,	/*!<Bandwidth Reservation 0x07*/
		/*!	\brief Private Command 0xff (Reserved from 0x08 to 0xfe)
		*/
		private_command = 0xff
	}type;
};

/*!	\struct scte35Desc
*	\brief scte35Desc structure to hold splice descriptor tag values.
*	
*	This structure contains the enum values Unknown, Avail Descriptor, DTMF Descriptor, Segmentation Descriptor, Time Descriptor, Audio Descriptor and reserved minimum and maximum
*	splice descriptor tag values which are possible in current SCTE-35 standard. Descriptors for Identifier "CUEI".
*/
struct scte35Desc {
	/*! \brief A type definition for type (Splice Descriptors).
	*/
	typedef enum {
		unknown = 999,					/*!<Unknown Descriptor (Not in standard but high number taken as garbage/default value)*/
		avail_descriptor = 0x00,		/*!<Avail Descriptor 0x00*/
		dtmf_descriptor = 0x01,			/*!<DTMF Descriptor 0x01*/
		segmentation_descriptor = 0x02,	/*!<Segmentation Descriptor 0x02*/
		time_descriptor = 0x03,			/*!<Time Descriptor 0x03*/
		audio_descriptor = 0x04,		/*!<Audio Descriptor 0x04*/
		reserved_min = 0x05,			/*!<Reserved Descriptor Minimum 0x05*/
		reserved_max = 0xff				/*!<Reserved Descriptor Maximum 0xff*/
	}type;
};

/*!	\class SInfoData
*	\brief #SInfoData class to hold base parameters.
*	
*	#SInfoData class holds the base parameters which are always present in SCTE-35 data.
*/
class SInfoData {
public:
	uint32_t table_id;					/*!<8-bit : table_id should be 252 (oxfc).*/
	uint32_t section_syntax_indicator;	/*!<1-bit : field that should always be set to 0 indicating that MPEG short sections are to be used.*/
	uint32_t private_indicator;			/*!<1-bit : Value either will be 0 or 1*/
	uint32_t reserved;					/*!<2-bit : Reserved bits*/
	uint32_t section_length;			/*!<12-bit : Number of remaining bytes in the splice_info_section, immediately following the section_length field up to the end of the splice_info_section. The value in this field shall not exceed 4093.*/
	uint32_t protocol_version;			/*!<8-bit : Its function is to allow, in the future, this table type to carry parameters that may be structured differently than those defined in the current protocol. At present, the only valid value for protocol_version is 0. Non-zero values of protocol_version may be used by a future version of SCTE-35 to indicate structurally different tables.*/
	uint32_t encrypted_packet;			/*!<1-bit : Value is 1 if encrypted packet and 0 if not*/
	/*!	\brief 6-bit : Encryption algorithm values
	*
	*	<table><caption id="multi_row">Encryption Algorithm Values</caption><tr><th>encryption_algorithm value</th><th>Encryption Algorithm</th></tr><tr><td>0</td><td>No encryption</td></tr><tr><td>1</td><td>DES – ECB mode</td></tr><tr><td>2</td><td>DES – CBC mode</td></tr><tr><td>3</td><td>Triple DES EDE3 – ECB mode</td></tr><tr><td>4 - 31</td><td>Reserved</td></tr><tr><td>32 - 63</td><td>User private</td></tr></table>
	*/
	uint32_t encryption_algorithm;		
	uint64_t pts_adjustment;			/*!<33-bit : A 33 bit unsigned integer that appears in the clear and that shall be used by a splicing device as an offset to be added to the (sometimes) encrypted pts_time field(s) throughout this message to obtain the intended splice time(s). When this field has a zero value, the pts_time field(s) shall be used without an offset. The pts_adjustment shall, at all times, be the proper value to use for conversion of the pts_time field to the current time-base. The conversion is done by adding the two fields. In the presence of a wrap or overflow condition the carry shall be ignored.*/
	uint32_t cw_index;					/*!<8-bit : An 8 bit unsigned integer that conveys which control word (key) is to be used to decrypt the message. The splicing device may store up to 256 keys previously provided for this purpose. When the encrypted_packet bit is zero, this field is present but undefined.*/
	uint32_t tier;						/*!<12-bit : A 12-bit value used by the SCTE 35 message provider to assign messages to authorization tiers. This field may take any value between 0x000 and 0xFFF. The value of 0xFFF provides backwards compatibility and shall be ignored by downstream equipment. When using tier, the message provider should keep the entire message in a single transport stream packet.*/
	uint32_t splice_command_length;		/*!<12-bit : A 16-bit number representing the number of bytes following the splice_command_type up to, but not including the descriptor_loop_length.*/
	
	/*!	\brief 8-bit : Splice command value
	*
	*	<table><caption id="multi_row">Splice Command Values</caption><tr><th>Command</th><th>splice_command_type value</th><th>XML Element</th></tr><tr><td>splice_null</td><td class="aligncenter">0x00</td><td>SpliceNull</td></tr><tr><td>Reserved</td><td class="aligncenter">0x01</td><td></td></tr><tr><td>Reserved</td><td class="aligncenter">0x02</td><td></td></tr><tr><td>Reserved</td><td class="aligncenter">0x03</td><td></td></tr><tr><td>splice_schedule</td><td class="aligncenter">0x04</td><td>SpliceSchedule</td></tr><tr><td>splice_insert</td><td class="aligncenter">0x05</td><td>SpliceInsert</td></tr><tr><td>time_signal</td><td class="aligncenter">0x06</td><td>TimeSignal</td></tr><tr><td>bandwidth_reservation</td><td class="aligncenter">0x07</td><td>BandwidthReservation</td></tr><tr><td>Reserved</td><td class="aligncenter">0x08 - 0xfe</td><td></td></tr><tr><td>private_command</td><td class="aligncenter">0xff</td><td>PrivateCommand</td></tr></table>
	*/
	uint32_t splice_command_type;
	uint32_t descriptor_loop_length;	/*!<16-bit : A 16-bit unsigned integer specifying the number of bytes used in the splice descriptor loop immediately following.*/
	uint32_t alignment_stuffing;		/*!<8-bit : For alignment stuffing mostly value is null*/
	uint32_t E_CRC_32;					/*!<32-bit : For encryption checking. Re-computed CRC value to verify CRC_32. It SHOULD be identical to CRC_32 attribute value.*/
	uint32_t CRC_32;					/*!<32-bit : 32-bit CRC value, by processing the entire splice_info_section, which includes the table_id field to but not including the CRC_32 field.*/

	/*! \brief #SInfoData class default constructor with intializer list.
	*/
	SInfoData() :
	table_id(PARAMETER_DEF_VAL),
	section_syntax_indicator(PARAMETER_DEF_VAL),
	private_indicator(PARAMETER_DEF_VAL),
	reserved(PARAMETER_DEF_VAL),
	section_length(PARAMETER_DEF_VAL),
	protocol_version(PARAMETER_DEF_VAL),
	encrypted_packet(PARAMETER_DEF_VAL),
	encryption_algorithm(PARAMETER_DEF_VAL),
	pts_adjustment(PARAMETER_DEF_VAL),
	cw_index(PARAMETER_DEF_VAL),
	tier(PARAMETER_DEF_VAL),
	splice_command_length(PARAMETER_DEF_VAL),
	splice_command_type(PARAMETER_DEF_VAL),
	descriptor_loop_length(PARAMETER_DEF_VAL),
	alignment_stuffing(PARAMETER_DEF_VAL),
	E_CRC_32(PARAMETER_DEF_VAL),
	CRC_32(PARAMETER_DEF_VAL)
	{
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #SInfoData class parameters.
	*/
	void ClearValues();
};

//------------------------------------- Command Types ------------------------------------------//

/*!	\class break_duration
*	\brief #break_duration class to hold break related informations.
*	
*	The #break_duration class specifies the duration of the commercial break(s). It may be used to give the splicer an indication of when the break will be over and when the network In Point will occur.
*/
class break_duration {
public:
	uint32_t auto_return;				/*!<1-bit : Auto return value for break duration either 0 or 1.*/
	uint32_t reserved;					/*!<6-bit : Reserved bits for break duration.*/
	/*!	\brief 33-bit : pts_time is in the resolution of 90kHz (1/90000 of a second).
	*
	*	pts_time is 33-bit unsigned integer in 90kHz. Max: 2^33 /90kHz (seconds) = 26.512 (hours). Rollover happens within this time.
	*/				
	uint64_t duration;

	/*! \brief #break_duration class default constructor with intializer list.
	*/
	break_duration():
	auto_return(PARAMETER_DEF_VAL),
	reserved(PARAMETER_DEF_VAL),
	duration(PARAMETER_DEF_VAL)
	{
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #break_duration class parameters.
	*/
	void ClearValues();
};

/*!	\class splice_time
*	\brief #splice_time class to hold time related informations.
*	
*	The #splice_time class, when modified by pts_adjustment, specifies the time of the splice event.
*/
class splice_time {
public:
	uint32_t time_specified_flag;		/*!<1-bit : Value will be either 0 or 1*/
	uint32_t reserved_1;				/*!<6-bit : Conditional reserved bits in splice_time if time_specified_flag value is 1*/
	uint64_t pts_time;					/*!<33-bit : A 33-bit field that indicates time in terms of ticks of the program’s 90 kHz clock. This field, when modified by pts_adjustment, represents the time of the intended splice point.*/
	uint32_t reserved_2;				/*!<7-bit : Conditional reserved bits in splice_time if time_specified_flag value is 0*/

	/*! \brief #splice_time class default constructor with intializer list.
	*/
	splice_time():
	time_specified_flag(PARAMETER_DEF_VAL),
	reserved_1(PARAMETER_DEF_VAL),
	pts_time(PARAMETER_DEF_VAL),
	reserved_2(PARAMETER_DEF_VAL)
	{
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #splice_time class parameters.
	*/
	void ClearValues();
};

/*!	\class SSchComponent
*	\brief #SSchComponent class to hold components related informations.
*/
class SSchComponent {
public:
	/*!	\brief 8-bit : An 8-bit value that identifies the elementary PID stream.
	*
	*	An 8-bit value that identifies the elementary PID stream containing the Splice Point specified by the value of splice_time that follows. The value shall be the same as the value used in the stream_identifier_descriptor to identify that elementary PID stream.
	*/
	uint32_t component_tag;

	/*!	\brief 32-bit : Time of the signaled splice event.
	*
	*	A 32-bit unsigned integer quantity representing the time of the signaled splice event as the number of seconds since 00 hours UTC, January 6th, 1980, with the count of intervening leap seconds included.
	*/
	uint32_t utc_splice_time;

	/*! \brief #SSchComponent class default constructor with intializer list.
	*/
	SSchComponent() :
	component_tag(PARAMETER_DEF_VAL),
	utc_splice_time(PARAMETER_DEF_VAL)
	{
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #SSchComponent class parameters.
	*/
	void ClearValues();
};

/*!	\class SpliceScheduleChild
*	\brief #SpliceScheduleChild class to hold single splice schedule related informations.
*/
class SpliceScheduleChild {
public:
	uint32_t splice_event_id;						/*!<32-bit : A 32-bit unique splice event identifier.*/
	/*!	\brief 1-bit : Value will be either 1 or 0.
	*
	*	A 1-bit flag that, when set to ‘1’, indicates that a previously sent splice event, identified by splice_event_id, has been cancelled. A value of TRUE shall be equivalent to a value of ‘1’ and FALSE shall be equivalent to a value of ‘0’. If omitted, set splice_event_cancel_indicator to 0 when generating an SCTE 35 splice_schedule message.
	*/
	uint32_t splice_event_cancel_indicator;
	uint32_t reserved_1;							/*!<7-bit : Reserved SpliceScheduleChild bits.*/
	/*!	\brief 1-bit : Value will be either 1 or 0.
	*
	*	A 1-bit flag that, when set to ‘1’, indicates that the splice event is an opportunity to exit from the network feed and that the value of utc_splice_time shall refer to an intended Out Point or Program Out Point. When set to ‘0’, the flag indicates that the splice event is an opportunity to return to the network feed and that the value of utc_splice_time shall refer to an intended In Point or Program In Point. A value of TRUE shall be equivalent to a value of ‘1’ for out_of_network_indicator in Bit Stream Format.
	*/
	uint32_t out_of_network_indicator;
	/*!	\brief 1-bit : Value will be either 1 or 0.
	*	
	*	A 1-bit flag that, when set to ‘1’, indicates that the message refers to a Program Splice Point and that the mode is the Program Splice Mode whereby all PIDs/components of the program are to be spliced. When set to ‘0’, this field indicates that the mode is the Component Splice Mode whereby each component that is intended to be spliced will be listed separately by the syntax that follows.
	*
	*	There is no entry in the XML schema for program_splice_flag. The value of program_splice_flag shall be set to ‘1’ when converting an XML representation of the splice_schedule to Bit Stream Format if the Program Element in the Event Element is specified; otherwise, the value of program_splice_flag shall be set to ‘0’.
	*/
	uint32_t program_splice_flag;
	uint32_t duration_flag;							/*!<1-bit : A 1-bit flag that indicates the presence of the break_duration field.*/
	uint32_t reserved_2;							/*!<5-bit : Reserved SpliceScheduleChild bits.*/
	uint32_t utc_splice_time;						/*!<32-bit : Value will be considered only if program_splice_flag parameter value is 1.*/
	uint32_t component_count;						/*!<8-bit : Value will be considered only when program_splice_flag parameter value is 0. Number of components to parse.*/
	std::vector<SSchComponent> v_Component_childs;	/*!<Holds list of splice schedule components found.*/
	break_duration m_breakD;						/*!<break_duration value will be considered only when duration_flag parameter value is 1.*/
	uint32_t unique_program_id;						/*!<16-bit : This value should provide a unique identification for a viewing event within the service.*/
	uint32_t avail_num;								/*!<8-bit : This field provides an identification for a specific avail within one unique_program_id.*/
	uint32_t avails_expected;						/*!<8-bit : This field provides a count of the expected number of individual avails within the current viewing event. When this field is zero, it indicates that the avail_num field has no meaning.*/

	/*! \brief #SpliceScheduleChild class default constructor with intializer list.
	*/
	SpliceScheduleChild():
	splice_event_id(PARAMETER_DEF_VAL),
	 splice_event_cancel_indicator(PARAMETER_DEF_VAL),
	 reserved_1(PARAMETER_DEF_VAL),
	 out_of_network_indicator(PARAMETER_DEF_VAL),
	 program_splice_flag(PARAMETER_DEF_VAL),
	 duration_flag(PARAMETER_DEF_VAL),
	 reserved_2(PARAMETER_DEF_VAL),
	 utc_splice_time(PARAMETER_DEF_VAL),
	 component_count(PARAMETER_DEF_VAL),
	 unique_program_id(PARAMETER_DEF_VAL),
	 avail_num(PARAMETER_DEF_VAL),
	 avails_expected(PARAMETER_DEF_VAL)
	{
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #SpliceScheduleChild class parameters.
	*/
	void ClearValues();
};

/*!	\class SpliceScheduleData
*	\brief #SpliceScheduleData class to hold multiple splice schedule related informations.
*/
class SpliceScheduleData {
public:
	uint32_t splice_count;						/*!<8-bit : An 8-bit unsigned integer that indicates the number of splice events specified in the loop.*/
	std::vector<SpliceScheduleChild> v_childs;	/*!<Holds list of Splice Schedule data found on parsing*/

	/*! \brief #SpliceScheduleData class default constructor with intializer list.
	*/
	SpliceScheduleData():
	splice_count(PARAMETER_DEF_VAL)
	{
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #SpliceScheduleData class parameters.
	*/
	void ClearValues();
};

/*!	\class SInComponent
*	\brief #SInComponent class to hold Splice Insert components related informations.
*/
class SInComponent {
public:
	/*!	\brief 8-bit : An 8-bit value that identifies the elementary PID stream.
	*
	*	An 8-bit value that identifies the elementary PID stream containing the Splice Point specified by the value of splice_time that follows. The value shall be the same as the value used in the stream_identifier_descriptor to identify that elementary PID stream.
	*/
	uint32_t component_tag;
	splice_time m_spliceT;						/*!<#splice_time value considered only when splice_immediate_flag parameter value is 0.*/
	
	/*! \brief #SInComponent class default constructor with intializer list.
	*/
	SInComponent():
	component_tag(PARAMETER_DEF_VAL)
	{
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #SInComponent class parameters.
	*/
	void ClearValues();
};

/*!	\class SpliceInsertData
*	\brief #SpliceInsertData class to hold Splice Insert related informations.
*/
class SpliceInsertData {
public:
	uint32_t splice_event_id;					/*!<32-bit : A 32-bit unique splice event identifier.*/
	/*!	\brief 1-bit : Value will be either 1 or 0.
	*	
	*	A 1-bit flag that, when set to ‘1’, indicates that a previously sent splice event, identified by splice_event_id, has been cancelled.
	*
	*	A value of TRUE shall be equivalent to a value of ‘1’ and FALSE shall be equivalent to a value of ‘0’. If omitted, set splice_event_cancel_indicator to 0 when generating an SCTE 35 splice_insert message.
	*/
	uint32_t splice_event_cancel_indicator;
	uint32_t reserved_1;						/*!<7-bit : Reserved bits of Splice Insert data.*/

	/*!	\brief 1-bit : Value will be either 1 or 0.
	*	
	*	A 1-bit flag that, when set to ‘1’, indicates that the splice event is an opportunity to exit from the network feed and that the value of splice_time, as modified by pts_adjustment, shall refer to an intended Out Point or Program Out Point. When set to ‘0’, the flag indicates that the splice event is an opportunity to return to the network feed and that the value of splice_time, as modified by pts_adjustment, shall refer to an intended In Point or Program In Point.
	*/
	uint32_t out_of_network_indicator;

	/*!	\brief 1-bit : Value will be either 1 or 0.
	*	
	*	A 1-bit flag that, when set to ‘1’, indicates that the message refers to a Program Splice Point and that the mode is the Program Splice Mode whereby all PIDs/components of the program are to be spliced. When set to ‘0’, this field indicates that the mode is the Component Splice Mode whereby each component that is intended to be spliced will be listed separately by the syntax that follows.
	*
	*	There is no entry in the XML schema for program_splice_flag. The value of program_splice_flag shall be set to ‘1’ when converting an XML representation of the splice_schedule to Bit Stream Format if the Program Element in the Event Element is specified; otherwise, the value of program_splice_flag shall be set to ‘0’.
	*/
	uint32_t program_splice_flag;

	uint32_t duration_flag;						/*!<1-bit : A 1-bit flag that indicates the presence of the break_duration field.*/	
	
	/*!	\brief 1-bit : Value will be either 1 or 0.
	*	
	*	When this flag is ‘1’, it indicates the absence of the #splice_time field and that the splice mode shall be the Splice Immediate Mode, whereby the splicing device shall choose the nearest opportunity in the stream, relative to the splice information packet, to splice. When this flag is ‘0’, it indicates the presence of the splice_time field in at least one location within the splice_insert command.
	*/
	uint32_t splice_immediate_flag;
	uint32_t reserved_2;						/*!<4-bit : Reserved bits for Splice Insert data.*/

	splice_time m_spliceT;						/*!<Holds splice_time value if splice_immediate_flag parameter value is 0.*/

	uint32_t component_count;					/*!<8-bit : Value will be considered only when program_splice_flag parameter value is 0. Number of components to parse.*/
	std::vector<SInComponent> v_ChildComps;		/*!<Holds list of splice insert components found.*/
	break_duration m_breakD;					/*!<#break_duration value will be considered only when duration_flag parameter value is 1.*/
	uint32_t unique_program_id;					/*!<16-bit : This value should provide a unique identification for a viewing event within the service.*/
	uint32_t avail_num;							/*!<8-bit : This field provides an identification for a specific avail within one unique_program_id.*/
	uint32_t avails_expected;					/*!<8-bit : This field provides a count of the expected number of individual avails within the current viewing event. When this field is zero, it indicates that the avail_num field has no meaning.*/

	/*! \brief #SpliceInsertData class default constructor with intializer list.
	*/
	SpliceInsertData() :
	 splice_event_id(PARAMETER_DEF_VAL),
	 splice_event_cancel_indicator(PARAMETER_DEF_VAL),
	 reserved_1(PARAMETER_DEF_VAL),
	 out_of_network_indicator(PARAMETER_DEF_VAL),
	 program_splice_flag(PARAMETER_DEF_VAL),
	 duration_flag(PARAMETER_DEF_VAL),
	 splice_immediate_flag(PARAMETER_DEF_VAL),
	 reserved_2(PARAMETER_DEF_VAL),
	 component_count(PARAMETER_DEF_VAL),
	 unique_program_id(PARAMETER_DEF_VAL),
	 avail_num(PARAMETER_DEF_VAL),
	 avails_expected(PARAMETER_DEF_VAL)
	{
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #SpliceInsertData class parameters.
	*/
	void ClearValues();

};

/*!	\class private_command
*	\brief #private_command class to hold private command related informations.
*/
class private_command {
public:

	/*!	\brief 32-bit : The identifier is a 32-bit field as defined in ISO/IEC 13818-1 [MPEG Systems], for the registration_descriptor format_identifier.
	*/
	uint32_t identifier;
	uint32_t private_length;					/*!<Holds the private data length values.*/
	std::vector<uint32_t> private_byte;			/*!<8-bit : Multiple private bytes according to private_length parameter value.*/

	/*! \brief #private_command class default constructor with intializer list.
	*/
	private_command():
	 identifier(PARAMETER_DEF_VAL),
	 private_length(PARAMETER_DEF_VAL)
	 {
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #private_command class parameters.
	*/
	void ClearValues();
};



//-------------------------------------------------- Descriptors ----------------------------------------------------//

/*!	\class SDBase
*	\brief #SDBase class base for all the descriptors classes.
*/
class SDBase {
public:
	/*!	\brief 8-bit : This 8-bit number defines the syntax for the private bytes that make up the body of this descriptor.
	*	
	*	The descriptor tags are defined by the owner of the descriptor, as registered using the identifier.
	*
	*	There is no entry in the XML schema for splice_descriptor_tag. The value is implicit when transforming to or from an XML representation of the splice_descriptor based on the specific descriptor Element supplied.
	*/
	uint32_t splice_descriptor_tag;

	/*!	\brief 8-bit : This 8-bit number gives the length, in bytes, of the descriptor following this field.
	*
	*	Descriptors are limited to 256 bytes, so this value is limited to 254. Since some descriptors have optional elements, care should be used while decoding a descriptor to never attempt to decode beyond the descriptor length.
	*	There is no entry in the XML schema for descriptor_length. The value shall be derived when converting an XML representation of the specific splice_descriptor to Bit Stream Format.
	*/
	uint32_t descriptor_length;

	/*!	\brief 32-bit : The identifier is a 32-bit field as defined in ISO/IEC 13818-1 [MPEG Systems], for the registration_descriptor's format_identifier.
	*
	*	Only identifier values registered and recognized by SMPTE Registration Authority, LLC should be used (See [SMPTE RA]). Its use in this descriptor shall scope and identify only the private information contained within this descriptor. This 32-bit number is used to identify the owner of the descriptor. The code 0x43554549 (ASCII “CUEI”) for descriptors defined in this specification has been registered with SMPTE.
	*/
	uint32_t identifier;

	/*! \brief #SDBase class default constructor with intializer list.
	*/
	SDBase():
	 splice_descriptor_tag(PARAMETER_DEF_VAL),
	 descriptor_length(PARAMETER_DEF_VAL),
	 identifier(PARAMETER_DEF_VAL)
	{

	}
};

/*!	\class SegDescComponent
*	\brief #SegDescComponent class to hold segmentation descriptor component related informations.
*/
class SegDescComponent {
public:
	/*!	\brief 8-bit : An 8-bit unsigned integer that specifies the number of instances of elementary PID stream data in the loop that follows.
	*
	*	Components are equivalent to elementary PID streams. If program_segmentation_flag == ‘0’ then the value of component_count shall be greater than or equal to 1.
	*	There is no entry in the XML schema for component_count. For Component Splice Mode, the value shall be derived when converting an XML representation of the segmentation_descriptor to Bit Stream Format. component_count shall be set to the count of Component Elements supplied within the SegmentationDescriptor Element in the XML document.
	*/
	uint32_t component_tag;

	uint32_t reserved;							/*!<7-bit : Reserved bit data in segmentation descriptor component.*/

	/*!	\brief 33-bit : A 33-bit unsigned integer that shall be used by a splicing device as an offset to be added to the pts_time, as modified by pts_adjustment, in the #time_signal message to obtain the intended splice time(s).
	*
	*	When this field has a zero value, then the pts_time field(s) shall be used without an offset. If #splice_time time_specified_flag = 0 or if the command this descriptor is carried with does not have a #splice_time field, this field shall be used to offset the derived immediate splice time.
	*/
	uint32_t pts_offset;

	/*! \brief #SegDescComponent class default constructor with intializer list.
	*/
	SegDescComponent():
	component_tag(PARAMETER_DEF_VAL),
	reserved(PARAMETER_DEF_VAL),
	pts_offset(PARAMETER_DEF_VAL)
	{
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #SegDescComponent class parameters.
	*/
	void ClearValues();
};

/*!	\class AudioDescComponent
*	\brief #AudioDescComponent class to hold audio descriptor component related informations.
*/
class AudioDescComponent {
public:
	/*!	\brief 8-bit : An optional 8-bit value that identifies the elementary PID stream containing the audio channel that follows.
	*	
	*	 If used, the value shall be the same as the value used in the stream_identifier_descriptor to identify that elementary PID stream. If this is not used, the value shall be 0xFF and the stream order shall be inferred from the PMT audio order.
	*/
	uint32_t component_tag;

	/*!	\brief 24-bit : This field is a 3-byte language code defining the language of this audio service which shall correspond to a registered language code contained in the Code column of the [ISO 639-2] registry.
	*/
	uint32_t ISO_code;

	/*!	\brief 3-bit : As per ATSC.
	*/
	uint32_t Bit_Stream_Mode;

	/*!	\brief 4-bit : As per ATSC.
	*/
	uint32_t Num_Channels;

	/*!	\brief 1-bit : From ATSC. This is a 1-bit field that indicates if this audio service is a full service suitable for presentation, or a partial service which should be combined with another audio service before presentation.
	*
	*	This bit should be set to a ‘1’ if this audio service is sufficiently complete to be presented to the listener without being combined with another audio service.
	*	This bit should be set to a ‘0’ if the service is not sufficiently complete to be presented without being combined with another audio service.
	*/
	uint32_t Full_Srvc_Audio;

	/*! \brief #AudioDescComponent class default constructor with intializer list.
	*/
	AudioDescComponent() :
	 component_tag(PARAMETER_DEF_VAL),
	 ISO_code(PARAMETER_DEF_VAL),
	 Bit_Stream_Mode(PARAMETER_DEF_VAL),
	 Num_Channels(PARAMETER_DEF_VAL),
	 Full_Srvc_Audio(PARAMETER_DEF_VAL)
	{
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #AudioDescComponent class parameters.
	*/
	void ClearValues();
};

/*!	\class AvailDescriptor
*	\brief #AvailDescriptor class to hold avail descriptor related informations.
*/
class AvailDescriptor : public SDBase {
public:
	/*!	\brief 32-bit : This 32-bit number provides information that a receiving device may utilize to alter its behavior during or outside of an avail. It may be used in a manner similar to analog cue tones.
	*
	*	An example would be a network directing an affiliate or a head-end to black out a sporting event.
	*/
	uint32_t provider_avail_id;

	/*! \brief #AvailDescriptor class default constructor with intializer list.
	*/
	AvailDescriptor():
	provider_avail_id(PARAMETER_DEF_VAL)
	 {
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #AvailDescriptor class parameters.
	*/
	void ClearValues();
};

/*!	\class DTMFDescriptor
*	\brief #DTMFDescriptor class to hold DTMF descriptor related informations.
*/
class DTMFDescriptor : public SDBase {
public:
	/*!	\brief 8-bit : This 8-bit number is the time the DTMF is presented to the analog output of the device in tenths of seconds.
	*
	*	 This gives a pre-roll range of 0 to 25.5 seconds. The splice info section shall be sent at least two seconds earlier then this value. The minimum suggested pre-roll is 4.0 seconds.
	*/
	uint32_t preroll;
	/*!	\brief 3-bit : This value of this flag is the number of DTMF characters the device is to generate.
	*
	*	There is no entry in the XML schema for dtmf_count. The value shall be derived when converting an XML representation of the #DTMF_descriptor to Bit Stream Format based on the number of chars.
	*/
	uint32_t dtmf_count;
	/*!	\brief 5-bit : Reserved bits in DTMF Descriptor data.
	*/
	uint32_t reserved;
	/*!	\brief 8-bit each : Holds the dtmf_count number of DTMF characters as list.
	*/
	std::vector<uint32_t> v_DTMF_Char;

	/*! \brief #DTMFDescriptor class default constructor with intializer list.
	*/
	DTMFDescriptor() :
	 preroll(PARAMETER_DEF_VAL),
	 dtmf_count(PARAMETER_DEF_VAL),
	 reserved(PARAMETER_DEF_VAL)
	{
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #DTMFDescriptor class parameters.
	*/
	void ClearValues();
};

/*!	\class SegmentationDescriptor
*	\brief #SegmentationDescriptor class to hold segmentation descriptor related informations.
*/
class SegmentationDescriptor : public SDBase {
public:
	/*!	\brief 32-bit : A 32-bit unique segmentation event identifier. Only one occurrence of a given segmentation_event_id value shall be active at any one time.
	*/
	uint32_t segmentation_event_id;

	/*!	\brief 1-bit : A 1-bit flag that, when set to ‘1’, indicates that a previously sent segmentation event, identified by segmentation_event_id, has been cancelled.
	*
	*	The segmentation_type_id does not need to match between the original/cancelled segmentation event message and the message with the segmentation_event_cancel_indicator true. Once a segmentation event is cancelled, the segmentation_event_id may be reused for content identification or to start a new segment.
	*	A value of TRUE shall be equivalent to a value of ‘1’ and FALSE shall be equivalent to a value of ‘0’. If omitted, set segmentation_event_cancel_indicator to 0 when generating a segmentation_descriptor.
	*/
	uint32_t segmentation_event_cancel_indicator;

	/*! \brief 7-bit : Reserved bits in segmentation descriptor.
	*/
	uint32_t reserved_1;

	/*!	\brief 1-bit : Value can be either 1 or 0.
	*
	*	A 1-bit flag that should be set to ‘1’ indicating that the message refers to a Program Segmentation Point and that the mode is the Program Segmentation Mode whereby all PIDs/components of the program are to be segmented.
	*	When set to ‘0’, this field indicates that the mode is the Component Segmentation Mode whereby each component that is intended to be segmented will be listed separately by the syntax that follows.
	*	The program_segmentation_flag can be set to different states during different descriptors messages within a program.
	*/
	uint32_t program_segmentation_flag;

	/*!	\brief 1-bit : A 1-bit flag that should be set to ‘1’ indicating the presence of segmentation_duration field.
	*
	*	The accuracy of the start time of this duration is constrained by the splice_command_type specified. For example, if a splice_null command is specified, the precise position in the stream is not deterministic.
	*/
	uint32_t segmentation_duration_flag;

	/*!	\brief 1-bit : When this bit has a value of ‘1’, the next five bits are reserved.
	*
	*	When this bit has the value of ‘0’, the following additional information bits shall have the meanings defined below. This bit and the following five bits are provided to facilitate implementations that use methods that are out of scope of this standard to process and manage this segment.
	*/
	uint32_t delivery_not_restricted_flag;

	/*!	\brief 1-bit : Value can be either 1 or 0. Considered only when delivery_not_restricted_flag is set to 0.
	*
	*	This bit shall have the value of ‘1’ when there are no restrictions with respect to web delivery of this segment.
	*	This bit shall have the value of ‘0’ to signal that restrictions related to web delivery of this segment are asserted.
	*/
	uint32_t web_delivery_allowed_flag;

	/*!	\brief 1-bit : Value can be either 1 or 0. Considered only when delivery_not_restricted_flag is set to 0.
	*
	*	This bit shall have the value of ‘1’ when there is no regional blackout of this segment.
	*	This bit shall have the value of ‘0’ when this segment is restricted due to regional blackout rules.
	*/
	uint32_t no_regional_blackout_flag;

	/*!	\brief 1-bit : Value can be either 1 or 0. Considered only when delivery_not_restricted_flag is set to 0.
	*
	*	This bit shall have the value of ‘1’ when there is no assertion about recording this segment.
	*	This bit shall have the value of 0 to signal that restrictions related to recording this segment are asserted.
	*/
	uint32_t archive_allowed_flag;

	/*!	\brief 2-bit : This field signals three pre-defined groups of devices. The population of each group is independent and the groups are non-hierarchical.
	*
	*	<table><caption id="multi_row">Device Restrictions Values</caption><tr><th>device_restrictions (2 bits)</th><th>Segmentation Message</th></tr><tr><td>0</td><td>Segment restricted to device group 0</td></tr><tr><td>1</td><td>Segment restricted to device group 1</td></tr><tr><td>2</td><td>Segment restricted to device group 2</td></tr><tr><td>3</td><td>None</td></tr></table>
	*/
	uint32_t device_restrictions;

	/*!	\brief 5-bit : Reserved bits in segmentation descriptor.
	*/
	uint32_t reserved_2;

	/*!	\brief 8-bit : Value can be either 1 or 0. Considered only when program_segmentation_flag is set to 0.
	*
	*	An 8-bit unsigned integer that specifies the number of instances of elementary PID stream data in the loop that follows. Components are equivalent to elementary PID streams.
	*	If program_segmentation_flag == ‘0’ then the value of component_count shall be greater than or equal to 1.
	*/
	uint32_t component_count;

	/*!	\brief Holds the component_count number of segmentation descriptor components in list.
	*/
	std::vector<SegDescComponent> v_ChildComps;

	/*!	\brief 40-bit : A 40-bit unsigned integer that specifies the duration of the segment in terms of ticks of the program’s 90 kHz clock.
	*
	*	It may be used to give the splicer an indication of when the segment will be over and when the next segmentation message will occur. Shall be 0 for end messages.
	*/
	uint64_t segmentation_duration;

	/*!	\brief 8-bit : There are multiple types allowed to ensure that programmers will be able to use an id that their systems support.
	*
	*	<table><caption id="multi_row">Segmentation UPID Table</caption><tr><th>segmentation_upid_type</th><th>segmentation_upid_length (byte)</th><th>segmentation_upid() (name)</th><th>Description</th></tr><tr><td>0</td><td>0</td><td>Not used</td><td>The segmentation_upid is not defined and is not present in the descriptor.</td></tr><tr><td>3</td><td>12</td><td>Ad-ID</td><td>Defined by the Advertising Digital Identification, LLC group.</td></tr><tr><td>4</td><td>32</td><td>UMID</td><td>See SMPTE 330</td></tr><tr><td>6</td><td>12</td><td>ISAN</td><td>ISO 15706-2 binary encoding</td></tr><tr><td>7</td><td>12</td><td>TID</td><td>Tribune Media Systems Program identifier</td></tr><tr><td>8</td><td>8</td><td>TI</td><td>AiringID (formerly Turner ID) for an airing of a program that is unique within a network.</td></tr><tr><td>9</td><td>variable</td><td>ADI</td><td>CableLabs metadata identifier as defined in Section 10.3.3.2.</td></tr><tr><td>10</td><td>12</td><td>EIDR</td><td>An EIDR represented in Compact Binary encoding as defined in Section 2.1.1</td></tr><tr><td>11</td><td>variable</td><td>ATSC Content ID</td><td>ATSC_content_identifier() structure</td></tr><tr><td>12</td><td>variable</td><td>MPU()</td><td>Managed Private UPID structure as defined in section 10.3.3.3.</td></tr><tr><td>13</td><td>variable</td><td>MID()</td><td>Multiple UPID types structure as defined in section 10.3.3.4.</td></tr><tr><td>14</td><td>variable</td><td>ADS Info</td><td>Advertising information</td></tr><tr><td>15</td><td>variable</td><td>URI</td><td>Universal Resource Identifier</td></tr><tr><td>16 - 255</td><td>variable</td><td>reserved</td><td>Reserved for future standardization</td></tr></table>
	*
	*/
	uint32_t segmentation_upid_type;

	/*!	\brief 8-bit : Length in bytes of segmentation_upid.
	*
	*	If there is no segmentation_upid present, segmentation_upid_length shall be set to zero.
	*/
	uint32_t segmentation_upid_length;

	/*!	\brief 8-bit each : Holds the 8-bit segmentation upid data in list.
	*/
	std::vector<uint32_t> v_upid_list;

	/*!	\brief 8-bit : Contains any one value from the mentioned table.
	*
	*	<table><caption id="multi_row">Segmentation Type ID Table</caption><tr><th>Segment Pairing - Start</th><th>Segment Pairing - End</th><th>segmentation_type_id</th></tr><tr><td>Program Start</td><td>Program End (can be overridden by Program Early Termination)</td><td>16/17 (18)</td></tr><tr><td>Program Overlap Start</td><td>Program End</td><td>23/17</td></tr><tr><td>Program Start–In Progress</td><td>Program End (can be overridden by Program Early Termination)</td><td>25/17 (18)</td></tr><tr><td>Program Breakaway</td><td>Program Resumption</td><td>19/20</td></tr><tr><td>Chapter Start</td><td>Chapter End</td><td>32/33</td></tr><tr><td>Break Start</td><td>Break End</td><td>34/35</td></tr><tr><td>Opening Credit Start</td><td>Opening Credit End</td><td>36/37</td></tr><tr><td>Closing Credit Start</td><td>Closing Credit End</td><td>38/39</td></tr><tr><td>Provider Advertisement Start</td><td>Provider Advertisement End</td><td>48/49</td></tr><tr><td>Distributor Advertisement Start</td><td>Distributor Advertisement End</td><td>50/51</td></tr><tr><td>Provider Placement Opportunity Start</td><td>Provider Placement Opportunity End</td><td>52/53</td></tr><tr><td>Distributor Placement Opportunity Start</td><td>Distributor Placement Opportunity End</td><td>54/55</td></tr><tr><td>Provider Overlay Placement Opportunity Start</td><td>Provider Overlay Placement Opportunity Start End</td><td>56/57</td></tr><tr><td>Distributor Overlay Placement Opportunity Start</td><td>Distributor Overlay Placement Opportunity End</td><td>58/59</td></tr><tr><td>Unscheduled Event Start</td><td>Unscheduled Event End</td><td>64/65</td></tr><tr><td>Network Start</td><td>Network End</td><td>80/81</td></tr><tr><td></td><td></td><td></td></tr><tr><th>Not Paired Segments</th><th></th><th></th></tr><tr><td></td><td>Program Runover Planned</td><td>21</td></tr><tr><td></td><td>Program Runover Unplanned</td><td>22</td></tr><tr><td></td><td>Content Identification</td><td>01</td></tr><tr><td></td><td>Not Indicated</td><td>00</td></tr></table>
	*/
	uint32_t segmentation_type_id;

	/*!	\brief 8-bit : This field provides support for numbering segments within a given collection of segments (such as chapters, advertisements or placement opportunities).
	*/
	uint32_t segment_num;
	/*!	\brief 8-bit : This field provides a count of the expected number of individual segments (such as chapters) within a collection of segments.
	*/
	uint32_t segments_expected;
	/*! \brief 8-bit : If specified, this field provides identification for a specific sub-segment within a collection of sub-segments.
	*
	*	This value, when utilized, is expected to be set to one for the first subsegment within a collection of sub-segments. This field is expected to increment by one for each new subsegment within a given collection. If present, descriptor_length shall include sub_segment_num in the byte count and serve as an indication to an implementation that sub_segment_num is present in the descriptor.
	*	If sub_segment_num is provided, sub_segments_expected shall be provided.
	*/
	uint32_t sub_segment_num;
	/*! \brief 8-bit : If specified, this field provides a count of the expected number of individual sub-segments within the collection of sub-segments.
	*
	*	 If present, descriptor_length shall include sub_segments_expected in the byte count and serve as an indication to an implementation that sub_segments_expected is present in the descriptor.
	*/
	uint32_t sub_segments_expected;

	/*! \brief #SegmentationDescriptor class default constructor with intializer list.
	*/
	SegmentationDescriptor():
	 segmentation_event_id(PARAMETER_DEF_VAL),
	 segmentation_event_cancel_indicator(PARAMETER_DEF_VAL),
	 reserved_1(PARAMETER_DEF_VAL),
	 program_segmentation_flag(PARAMETER_DEF_VAL),
	 segmentation_duration_flag(PARAMETER_DEF_VAL),
	 delivery_not_restricted_flag(PARAMETER_DEF_VAL),
	 web_delivery_allowed_flag(PARAMETER_DEF_VAL),
	 no_regional_blackout_flag(PARAMETER_DEF_VAL),
	 archive_allowed_flag(PARAMETER_DEF_VAL),
	 device_restrictions(PARAMETER_DEF_VAL),
	 reserved_2(PARAMETER_DEF_VAL),
	 component_count(PARAMETER_DEF_VAL),
	 segmentation_duration(PARAMETER_DEF_VAL),
	 segmentation_upid_type(PARAMETER_DEF_VAL),
	 segmentation_upid_length(PARAMETER_DEF_VAL),
	 segmentation_type_id(PARAMETER_DEF_VAL),
	 segment_num(PARAMETER_DEF_VAL),
	 segments_expected(PARAMETER_DEF_VAL),
	 sub_segment_num(PARAMETER_DEF_VAL),
	 sub_segments_expected(PARAMETER_DEF_VAL)
	 {
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #SegmentationDescriptor class parameters.
	*/
	void ClearValues();
};

/*!	\class TimeDescriptor
*	\brief #TimeDescriptor class to hold time descriptor related informations.
*/
class TimeDescriptor : public SDBase {
public:
	/*! \brief 48-bit : This 48-bit number is the TAI seconds value.
	*/
	uint64_t TAI_seconds;
	/*! \brief 32-bit : This 32-bit number is the TAI nanoseconds value.
	*/
	uint32_t TAI_ns;
	/*! \brief 16-bit : This 16-bit number shall be used in the conversion from TAI time to UTC or NTP time per the following equations.
	*
	*	UTC seconds = TAI seconds - UTC_offset
	*	NTP seconds = TAI seconds - UTC_offset + 2,208,988,800
	*/
	uint32_t UTC_offset;

	/*! \brief #TimeDescriptor class default constructor with intializer list.
	*/
	TimeDescriptor() :
	 TAI_seconds(PARAMETER_DEF_VAL),
	 TAI_ns(PARAMETER_DEF_VAL),
	 UTC_offset(PARAMETER_DEF_VAL)
	{
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #TimeDescriptor class parameters.
	*/
	void ClearValues();
};

/*!	\class AudioDescriptor
*	\brief #AudioDescriptor class to hold time descriptor related informations.
*/
class AudioDescriptor : public SDBase {
public:
	/*! \brief 4-bit : The value of this flag is the number of audio PIDs in the program.
	*/
	uint32_t audio_count;

	/*! \brief 4-bit : Reserved bits in audio descriptor data.
	*/
	uint32_t reserved;

	/*! \brief Holds the audio_count number of audio descriptor components in list.
	*/
	std::vector<AudioDescComponent> v_audio_comp_list;

	/*! \brief #AudioDescriptor class default constructor with intializer list.
	*/
	AudioDescriptor() :
	 audio_count(PARAMETER_DEF_VAL),
	 reserved(PARAMETER_DEF_VAL)
	{
		ClearValues();
	}

	/*! \brief Member function to clear or reset all the #AudioDescriptor class parameters.
	*/
	void ClearValues();
};


/*!	\class CSpliceClass
*	\brief #CSpliceClass class is the utility for parsing SCTE-35 data.
*/
class CSpliceClass
{
private:
	/*! \brief String variable to hold the main SCTE-35 textual data.
	*/
	std::string sBinData;
	/*! \brief String variable to hold the SCTE-35 descriptor part textual data.
	*/
	std::string sDescriptorData;
	/*! \brief Integer variable to hold the current bit point till is has been parsed.
	*/
	int iCurrentBitPoint;

	/*! \brief Fixed string value used for next line in output string data.
	*/
	std::string sNextLine;
	/*! \brief Fixed string values used for comma only places in output string data.
	*/
	std::string sCommaOnly;

	//----------------------------------------- Utilities Start ------------------------------------------------------//
	/*! \brief Function to convert base64 string to hexa-decimal string
	*	\param str an input Base64 string data
	*	\returns Hexa-decimal string data or empty string on some failure
	*/
	std::string Base64ToHexa(const std::string& str);
	/*! \brief Function to convert hexa-decimal character to binary string
	*	\param c an input hexa-decimal character data
	*	\returns binary string data or empty string on some failure
	*/
	const char* hexa2binary(char c);
	/*! \brief Function to convert hexa-decimal string to binary string
	*	\param hex an input hexa-decimal string data
	*	\returns binary string data or empty string on some failure
	*/
	std::string HexaStr2BinaryStr(const std::string& hex);
	/*! \brief Function to check if white-spaces are present or not in string data
	*	\param sParam an input string data
	*	\returns true if no white-spaces are present otherwise false
	*/
	bool SpacesNotPresent(const std::string& sParam);

	/*! \brief Function to convert 32-bit binary string data to 32-bit unsigned integer
	*	\param sInput an input 32-bit binary string data
	*	\returns 32-bit unsigned integer value of corresponding input data 
	*/
	uint32_t BinStrToNumber_32bit(const std::string& sInput);
	/*! \brief Function to get 32-bit unsigned integer value starting from a particular bit location to a length from a binary string 
	*	\param sInput an input binary string data
	*	\param iStart starting bit position to get data
	*	\param bUpdateStart	to whether update the current bit position after bit traversing (modifies iCurrentBitPoint variable)
	*	\param iLength the bit length to which the traversing will be done from iStart
	*	\param bSuccess the flag to check for this function success or failure
	*	\returns 32-bit unsigned integer value
	*	\warning Only use this for getting 32-bit or less binary combination values
	*/
	uint32_t GetValue_info_32bit(const std::string& sInput, const int& iStart, const bool& bUpdateStart, const int& iLength, bool& bSuccess);

	/*! \brief Function to convert 64-bit binary string data to 64-bit unsigned integer
	*	\param sInput an input 64-bit binary string data
	*	\returns 64-bit unsigned integer value of corresponding input data 
	*/
	uint64_t BinStrToNumber_64bit(const std::string& sInput);
	/*! \brief Function to get 64-bit unsigned integer value starting from a particular bit location to a length from a binary string 
	*	\param sInput an input binary string data
	*	\param iStart starting bit position to get data
	*	\param bUpdateStart	to whether update the current bit position after bit traversing (modifies iCurrentBitPoint variable)
	*	\param iLength the bit length to which the traversing will be done from iStart
	*	\param bSuccess the flag to check for this function success or failure
	*	\returns 32-bit unsigned integer value
	*	\warning Only use this for getting 64-bit or less and greater than 32-bit binary combination values
	*/
	uint64_t GetValue_info_64bit(const std::string& sInput, const int& iStart, const bool& bUpdateStart, const int& iLength, bool& bSuccess);

	/*! \brief Function to get 32-bit unsigned integer value starting from a particular bit location to a length from a binary string 
	*	\param sInput an input binary string data
	*	\param iStart starting bit position to get data
	*	\param bUpdateStart	to whether update the current bit position after bit traversing (modifies input iStart variable)
	*	\param iLength the bit length to which the traversing will be done from iStart
	*	\param bSuccess the flag to check for this function success or failure
	*	\returns 32-bit unsigned integer value
	*	\warning Only use this for getting 32-bit or less binary combination values
	*/
	uint32_t GetValue_Desc_32bit(const std::string& sInput, int& iStart, const bool& bUpdateStart, const int& iLength, bool& bSuccess);
	/*! \brief Function to get 64-bit unsigned integer value starting from a particular bit location to a length from a binary string 
	*	\param sInput an input binary string data
	*	\param iStart starting bit position to get data
	*	\param bUpdateStart	to whether update the current bit position after bit traversing (modifies input iStart variable)
	*	\param iLength the bit length to which the traversing will be done from iStart
	*	\param bSuccess the flag to check for this function success or failure
	*	\returns 32-bit unsigned integer value
	*	\warning Only use this for getting 64-bit or less and greater than 32-bit binary combination values
	*/
	uint64_t GetValue_Desc_64bit(const std::string& sInput, int& iStart, const bool& bUpdateStart, const int& iLength, bool& bSuccess);

	//----------------------------------------- Utilities End ------------------------------------------------------------//

	//======================= Splice Info Start =============================//

	/*!	\class SInfoLength
	*	\brief #SInfoLength class holding the fixed base parameters bit length values as enums.
	*/
	class SInfoLength {
	public:
		/*! 
		*	\brief A type definition for sil (Base parameters fixed lengths)
		*/
		typedef enum {
			table_id = 8,
			section_syntax_indicator = 1,
			private_indicator = 1,
			reserved = 2,
			section_length = 12,
			protocol_version = 8,
			encrypted_packet = 1,
			encryption_algorithm = 6,
			pts_adjustment = 33,
			cw_index = 8,
			tier = 12,
			splice_command_length = 12,
			splice_command_type = 8,
			descriptor_loop_length = 16,
			alignment_stuffing = 8,
			E_CRC_32 = 32,
			CRC_32 = 32
		}sil;
	};

	/*! \brief #SInfoData object variable for holding SCTE-35 base parameters values.
	*/
	SInfoData m_SInfoData;
	/*! \brief Function to parse the first portion of base parameters from sBinData to m_SInfoData object.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool ParseSInfoDataFirst();
	/*! \brief Function to get the first portion of base parameters as string textual data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns first portion of base parameters textual data as string stream.
	*/
	std::stringstream GetTextSInfoDataFirst(const bool& bIfSingleLine);

	/*!	\brief Function to clear the splice command type object according to splice_command_type value.
	*/
	void Clear_Command_Type();
	/*!	\brief Function to parse sBinData to the corresponding splice command type object according to splice_command_type value.
	*/
	bool Parse_Command_Type();
	/*! \brief Function to get the splice command type object as string textual data according to splice_command_type value.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns splice command type object textual data as string stream according to splice_command_type value.
	*/
	std::stringstream GetText_Command_Type(const bool& bIfSingleLine);

	/*! \brief Function to parse the second portion of base parameters from sBinData to m_SInfoData object.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool ParseSInfoDataSecond();
	/*! \brief Function to get the second portion of base parameters as string textual data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns second portion of base parameters textual data as string stream.
	*/
	std::stringstream GetTextSInfoDataSecond(const bool& bIfSingleLine);

	//======================= Splice Info End =============================//

	//======================= Command Type Start =============================//

	//Splice Null not required
	/*! \brief Function to parse the splice null command parameters from sBinData to null object.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool Parse_Splice_Null();
	/*! \brief Function to get the splice null command parameters as string textual data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns splice null command parameters textual data as string stream.
	*/
	std::stringstream GetText_Splice_Null(const bool& bIfSingleLine);

	/*!	\brief #SpliceScheduleData object variable for holding SCTE-35 Splice Schedule data.
	*/
	SpliceScheduleData m_objSpliceSchedule;
	/*! \brief Function to parse the splice schedule command parameters from sBinData to m_objSpliceSchedule object.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool Parse_Splice_Schedule();
	/*! \brief Function to get the splice null schedule parameters as string textual data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns splice schedule command parameters textual data as string stream.
	*/
	std::stringstream GetText_Splice_Schedule(const bool& bIfSingleLine);

	/*!	\brief #SpliceInsertData object variable for holding SCTE-35 Splice Insert data.
	*/
	SpliceInsertData m_objSpliceInsert;
	/*! \brief Function to parse the splice insert command parameters from sBinData to m_objSpliceInsert object.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool Parse_Splice_Insert();
	/*! \brief Function to get the splice insert schedule parameters as string textual data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns splice insert command parameters textual data as string stream.
	*/
	std::stringstream GetText_Splice_Insert(const bool& bIfSingleLine);

	/*!	\brief #splice_time object variable for holding SCTE-35 Time Signal data.
	*/
	splice_time m_objTimeSignal;
	/*! \brief Function to parse the time signal command parameters from sBinData to m_objTimeSignal object.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool Parse_Time_Signal();
	/*! \brief Function to get the time signal parameters as string textual data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns time signal command parameters textual data as string stream.
	*/
	std::stringstream GetText_Time_Signal(const bool& bIfSingleLine);

	//BW_Reservation not required
	/*! \brief Function to parse the BW reservation command parameters from sBinData to null object.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool Parse_BW_Reservation();
	/*! \brief Function to get the BW reservation parameters as string textual data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns BW reservation command parameters textual data as string stream.
	*/
	std::stringstream GetText_BW_Reservation(const bool& bIfSingleLine);

	/*!	\brief #private_command object variable for holding SCTE-35 Private Command data.
	*/
	private_command m_objPrivateCommand;
	/*! \brief Function to parse the private command parameters from sBinData to m_objPrivateCommand object.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool Parse_Private_Command(const uint32_t& splice_command_length_param);
	/*! \brief Function to get the private command parameters as string textual data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns private command parameters textual data as string stream.
	*/
	std::stringstream GetText_Private_Command(const bool& bIfSingleLine);

	//======================= Command Type End =============================//

	//================================== Descriptors =========================================//

	/*!	\class SpliceDescriptorData
	*	\brief #SpliceDescriptorData class holding multiple SCTE-35 descriptors as list.
	*/
	class SpliceDescriptorData {
	public:
		/*! \brief Holds multiple SDBase data type in list.
		*/
		std::vector<SDBase*> v_SDChilds;
	};
	/*!	\brief #SpliceDescriptorData object variable for holding SCTE-35 decriptors data.
	*/
	SpliceDescriptorData m_objSpliceDescriptor;

	/*!	\brief Function to parse the avail descriptor parameters from sInData and add to a list in m_objSpliceDescriptor object.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool Parse_Avail_Descriptor(const std::string& sInData);
	/*! \brief Function to get the avail descriptor parameters as string textual data.
	*	\param paramData an input of descriptor type data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns avail descriptor parameters textual data as string stream.
	*/
	std::stringstream GetText_Avail_Descriptor(SDBase* paramData, const bool& bIfSingleLine);

	/*!	\brief Function to parse the DTMF descriptor parameters from sInData and add to a list in m_objSpliceDescriptor object.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool Parse_DTMF_Descriptor(const std::string& sInData);
	/*! \brief Function to get the DTMF descriptor parameters as string textual data.
	*	\param paramData an input of descriptor type data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns DTMF descriptor parameters textual data as string stream.
	*/
	std::stringstream GetText_DTMF_Descriptor(SDBase* paramData, const bool& bIfSingleLine);

	/*!	\brief Function to parse the segmentation descriptor parameters from sInData and add to a list in m_objSpliceDescriptor object.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool Parse_Segmentation_Descriptor(const std::string& sInData);
	/*! \brief Function to get the segmentation descriptor parameters as string textual data.
	*	\param paramData an input of descriptor type data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns segmentation descriptor parameters textual data as string stream.
	*/
	std::stringstream GetText_Segmentation_Descriptor(SDBase* paramData, const bool& bIfSingleLine);

	/*!	\brief Function to parse the time descriptor parameters from sInData and add to a list in m_objSpliceDescriptor object.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool Parse_Time_Descriptor(const std::string& sInData);
	/*! \brief Function to get the time descriptor parameters as string textual data.
	*	\param paramData an input of descriptor type data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns time descriptor parameters textual data as string stream.
	*/
	std::stringstream GetText_Time_Descriptor(SDBase* paramData, const bool& bIfSingleLine);

	/*!	\brief Function to parse the audio descriptor parameters from sInData and add to a list in m_objSpliceDescriptor object.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool Parse_Audio_Descriptor(const std::string& sInData);
	/*! \brief Function to get the audio descriptor parameters as string textual data.
	*	\param paramData an input of descriptor type data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns audio descriptor parameters textual data as string stream.
	*/
	std::stringstream GetText_Audio_Descriptor(SDBase* paramData, const bool& bIfSingleLine);

	/*!	\brief Function to clear each descriptor in data list in object m_objSpliceDescriptor according to splice_descriptor_tag parameter.
	*/
	void Clear_Splice_Descriptor();
	/*!	\brief Function to parse each descriptor in data list in object m_objSpliceDescriptor according to splice_descriptor_tag parameter from sDescriptorData. (Bounded to loopLength)
	*	\param loopLength is the length to which bit traversing is performed on sDescriptorData.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool Parse_Splice_Descriptor(const uint32_t& loopLength);
	/*! \brief Function to get each descriptor parameters in data list present in m_objSpliceDescriptor object concatenated as string textual data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns all descriptors parameters textual data as string stream.
	*/
	std::stringstream GetText_Splice_Descriptor(const bool& bIfSingleLine);

	//================================== Descriptors =========================================//


public:

	/*! \brief #CSpliceClass class default constructor.
	*/
	CSpliceClass();
	/*! \brief #CSpliceClass class default destructor.
	*/
	~CSpliceClass();

	/*!	\brief Deleted copy constructor so #CSpliceClass class object cannot be copied.
	*/
	CSpliceClass(const CSpliceClass&)=delete;
	/*!	\brief Deleted assignment operator so #CSpliceClass class object cannot be assigned.
	*/
	void operator=(const CSpliceClass&)=delete;

	//--------------------------------------- Usage Functions Start ----------------------------------------------------//
	/*!	\brief Function to parse binary textual data (without spaces).
	*	\param sInput an input binary string data.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool ParseBinary(const std::string& sInput);

	/*!	\brief Function to parse hexa-decimal textual data (without spaces).
	*	\param sInput an input hexa-decimal string data.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool ParseHexa(const std::string& sInput);

	/*!	\brief Function to parse Base64 textual data (without spaces).
	*	\param sInput an input Base64 string data.
	*	\returns true if successfully parsed and false if any failure.
	*/
	bool ParseBase64(const std::string& sInput);

	//Main function to get data in text format
	/*!	\brief Function to get full SCTE-35 parameters parsed as string textual data.
	*	\param bIfSingleLine to whether get textual data in single-line or multi-line.
	*	\returns full SCTE-35 parameters textual data as string stream.
	*/
	std::string GetText(const bool& bIfSingleLine);

	/*!	\brief Function to reset all the SCTE-35 data parsed previously in parameters to default value.
	*/
	void ResetValue();

	//Function to remove white-spaces in string
	/*!	\brief Function to remove white-spaces from input string data.
	*	\param sParam an input string data (this variable is modified inside this function).
	*/
	void RemoveSpaces(std::string& sParam);
	//--------------------------------------- Usage Functions End -------------------------------------------------------//
};

};

#endif