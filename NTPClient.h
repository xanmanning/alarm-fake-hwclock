/**
 * @file        NTPClient.h
 * @brief       NTP client
 * @author      Viveris Technologies
 * @date        2024-07-01
 * @copyright
 *    Copyright (C) 2024, Liebherr-Mining Equipment
 */
#pragma once

#include <stdint.h>
#include <stdio.h>

#include <string>

#include <exception>
#include <string>

#include <stdarg.h>

class Exception : public std::exception
{
public:
	/** Simple exception without formatting. */
	Exception(const std::string& what) noexcept
	    : fWhat(what)
	{
	}

	/** Exception with formatting.
	 *
	 * Formatting the string requires a memory allocation. This can fail if no memory is available.
	 * In that case, the raw format string will be used, so we still get some message.
	 */
	Exception(const char* const format, ...) noexcept __attribute__((format(printf, 2, 3)))
	{
		va_list args;
		va_start(args, format);
		char* tempBuffer = NULL;
		int ret = vasprintf(&tempBuffer, format, args);
		va_end(args);

		if ((ret <= 0) || (tempBuffer == NULL)) {
			/* Failed to format the string. Use the unformatted one, it is the best thing we have */
			fWhat = format;
		} else {
			fWhat = tempBuffer;
			free(tempBuffer);
		}
	}

	const char* what() const noexcept override
	{
		return fWhat.c_str();
	}

private:
	std::string fWhat;
};

class NTPClient
{
public:
	/**
	 * Structure that represent the NTP header
	 */
	struct NtpPacketRequest {
		// 8 bits: li(2), vn(3), and mode(3).
		uint8_t fLiVnMode;
		// 8 bits: Response bit(1), error bit(1), more bit(1), opcode(5)
		uint8_t fOpcode;
		uint16_t fSequenceNumber;
		// The status of the NTP server (synchronized or not)
		// if the bits from 6 -> 8 are set to 110 it mean that the NTP is synchronized
		uint16_t fStatus;
		// It define each server in the NTP configuration
		uint16_t fAssociationId;
		// This is a 16-bit integer indicating the offset, in octets, of the first octet in the data
		// area
		uint16_t fOffset;
		// The size of the NTP data
		uint16_t fCount;
	};

	enum CommandType
	{
		// Send a NTP command to get the associations id of all the servers
		kReadStatus = 0x01,
		// Send a NTP command to get the offset of the server on which we are synchronized
		kReadVariable = 0x02
	};

public:
	NTPClient();
	virtual ~NTPClient();

	/**
	 * Create NTP command
	 * Depending on the value of "opcode", a read status or read variable command will be generated
	 **/
	void CreateNtpHeader(NtpPacketRequest& ntpPacketIn, const uint8_t& opcode);

	/**
	 * Process the answer to read status command and update the association id value of the server
	 * on which we are synchronized
	 **/
	bool IsSynchronizedWithServer(char ntpDataIn[1024]);

	void SetSocket();

	void CloseSocket();

	const int& GetNtpSocket() const
	{
		return fUdpSocket;
	}

private:
	int fUdpSocket;
	uint16_t fSequenceNumber;
	uint16_t fCurrentAssociationId;
};
