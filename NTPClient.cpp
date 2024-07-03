/**
 * @file        NTPClient.cpp
 * @brief       NTP client
 * @author      Viveris Technologies
 * @date        2024-07-01
 * @copyright
 *    Copyright (C) 2024, Liebherr-Mining Equipment
 */
#include "NTPClient.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>

#include <cmath>
#include <cstring>
#include <sstream>
#include <vector>

const size_t SERVER_PROPERTIES_SIZE = 2;
const int CURRENT_SYNCHRONISATION_SOURCE = 0x0600;


NTPClient::NTPClient()
    : fUdpSocket(-1)
    , fSequenceNumber(1)
    , fCurrentAssociationId(0)
{
}

NTPClient::~NTPClient()
{
	CloseSocket();
}

void NTPClient::CloseSocket()
{
	close(fUdpSocket);
	fUdpSocket = -1;
}

void NTPClient::SetSocket()
{
	// Creating socket file descriptor
	fUdpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (fUdpSocket < 0) {
		throw Exception("socket creation failed: errno %d : %s", errno, strerror(errno));
	}

	struct sockaddr_in servAddr = {};
	// Filling server information
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(123);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(fUdpSocket, reinterpret_cast<struct sockaddr*>(&servAddr), sizeof(servAddr)) < 0) {
		throw Exception("Cannot connect the NTP socket: errno %d : %s", errno, strerror(errno));
	}
}

bool NTPClient::IsSynchronizedWithServer(char ntpDataIn[1024])
{
	bool moreBit = false;
	// Copy the 12 first element of the packets.
	NTPClient::NtpPacketRequest* header = reinterpret_cast<NTPClient::NtpPacketRequest*>(ntpDataIn);
	header->fCount = ntohs(header->fCount);
	printf("The size of the NTP payload: %d\n", header->fCount);

	const uint8_t opcodeMask = 0x1F;
	// Check if this is the answer to the read status
	if ((header->fOpcode & opcodeMask) != CommandType::kReadStatus) {
		// Not the expected message, exit the function
		return false;
	}

	// After the header, the ntpDataIn contains a list of server properties with the following description:
	//   - association Id: uint16_t
	//   - peer status: uint16_t

	// Math the number of server properties
	int nbServer = header->fCount / SERVER_PROPERTIES_SIZE;

	// Find the beginning of ther server properties table as described before
	const uint16_t* serverProperties = reinterpret_cast<const uint16_t*>(
		    ntpDataIn + sizeof(NTPClient::NtpPacketRequest));

	// Loop over the list of server and check if one is designated as NTP reference
	for (int index = 0; index < nbServer; index += SERVER_PROPERTIES_SIZE) {

		// Get association id
		uint16_t associationId = serverProperties[index];
		associationId = ntohs(associationId);

		// Get peer status
		uint16_t peerStatus = serverProperties[index + 1];
		peerStatus = ntohs(peerStatus);

		printf("Peer status = %d, associationId = %d\n", peerStatus, associationId);

		// Use a mask to check in the peer status if the selection flag designate the server as the
		// NTP reference server
		if ((peerStatus & 0x0700) == CURRENT_SYNCHRONISATION_SOURCE) {
			// We are synchronized on this server, save the last association id
			fCurrentAssociationId = associationId;
			printf("We have found a server on which the board is synchronized\n");
			return true;
		} else {
			printf("Not synchronized on this server: %d\n", associationId);
		}
	}

	return false;
}

void NTPClient::CreateNtpHeader(NtpPacketRequest& ntpPacketIn, const uint8_t& opcode)
{
	ntpPacketIn.fLiVnMode = 0x16;
	ntpPacketIn.fOpcode = opcode;
	ntpPacketIn.fSequenceNumber = htons(fSequenceNumber);
	ntpPacketIn.fStatus = 0;
	ntpPacketIn.fAssociationId = htons(fCurrentAssociationId);
	ntpPacketIn.fOffset = 0;
	ntpPacketIn.fCount = 0;
	fSequenceNumber++;
}
