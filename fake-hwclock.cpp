/**
 * @file        fake-hwclock.cpp
 * @brief       Fake Hardware Clock
 * @author      Viveris Technologies
 * @date        2024-07-01
 * @copyright
 *    Copyright (C) 2024, Liebherr-Mining Equipment
 */

#include <stdio.h>
#include <time.h>
#include <utime.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include <cstring>

#include "NTPClient.h"

/* If the difference between the last known time and current time
 * is greater than this amount, we assume the clock is broken
 * and reset it. */
static time_t max_diff = 60 * 60 * 24 * 365 * 10; /* ten years */

void Write(NTPClient& ntpClient, NTPClient::NtpPacketRequest& ntpPacket)
{
	ssize_t result = write(ntpClient.GetNtpSocket(), reinterpret_cast<char*>(&ntpPacket),
	    sizeof(NTPClient::NtpPacketRequest));
	if (result != sizeof(NTPClient::NtpPacketRequest)) {
		throw Exception("Write to NTP socket failed: %s", strerror(errno));
	}
	printf("Sended this amount of data %d\n", result);
}

/**
 * Send a read status or read variable command
 **/
void SendNtpCommand(NTPClient& ntpClient, const NTPClient::CommandType commandType)
{
	NTPClient::NtpPacketRequest ntpPacketIn;
	// Send a read status command
	ntpClient.CreateNtpHeader(ntpPacketIn, commandType);
	Write(ntpClient, ntpPacketIn);
}

void SendReadStatusCommand(NTPClient& ntpClient)
{
	SendNtpCommand(ntpClient, NTPClient::CommandType::kReadStatus);
}

void ReadFromNtpSocket(NTPClient& ntpClient, char* bufferIn, const size_t size)
{
	ssize_t result = read(ntpClient.GetNtpSocket(), bufferIn, size);
	if (result < 0) {
		throw Exception("Cannot read from NTP socket: %s", strerror(errno));
	}
}

/**
 *  Process the answer to NTP commands.
 *
 *  When NTP is fully synchronized, send a ZeroMQ notification to all subscribers. The
 *  notification is repeated every 2 seconds and contains no data.
 */
bool ProcessNtpPackets(NTPClient& ntpClient, char bufferIn[1024])
{
	return ntpClient.IsSynchronizedWithServer(bufferIn);
}

bool ReadMessageAndCheckIfSynchronized(NTPClient& ntpClient)
{
	char bufferIn[1024];
	ReadFromNtpSocket(ntpClient, bufferIn, sizeof(bufferIn));
	return ProcessNtpPackets(ntpClient, bufferIn);
}

bool isNtpSynchro()
{
	try {
		char fBufferIn[1024];
		NTPClient ntpClient;

		ntpClient.SetSocket();
		SendReadStatusCommand(ntpClient);
		return ReadMessageAndCheckIfSynchronized(ntpClient);
	} catch (Exception e) {
		printf("Cannot check if the board is NTP synchronized: %s\n", e.what());
		return false;
	}
}

void SavingClock(struct stat& temp, const char* timeFile)
{
	struct utimbuf time_update;
	time_update.actime = temp.st_atime;
	time_update.modtime = time(NULL);
	utime(timeFile, &time_update);
}

void SettingClock(struct stat& temp, const time_t& timeStore)
{
	struct timeval sys_update;
	sys_update.tv_sec = timeStore;
	sys_update.tv_usec = 0;
	settimeofday(&sys_update,NULL);
}

int main (int argc, char *argv[])
{
	struct stat temp;
	const char *timeFile;
	/* The time is stored in the creation and modification date of a file.
	 * If no specific file is passed, use the executable itself.
	 */
	if (argc > 1)
		timeFile = argv[1];
	else
		timeFile = argv[0];
	time_t currentTime;
	time_t timeStore;
	time_t diff;

	currentTime = time(NULL);
	if(currentTime == ((time_t) - 1))
		return 1;

	if(stat(timeFile, &temp) < 0)
		return 1;

	timeStore = temp.st_mtime;
	diff = timeStore - currentTime;

	char timeStoreStr[20];
	strftime(timeStoreStr, 20, "%Y-%m-%d %H:%M:%S", localtime(&timeStore));
	char currentTimeStr[20];
	strftime(currentTimeStr, 20, "%Y-%m-%d %H:%M:%S", localtime(&currentTime));

	if (isNtpSynchro()) {
		printf("Saving current time %s.\n", currentTimeStr);
		SavingClock(temp, timeFile);
	} else if(timeStore > currentTime || diff > max_diff || diff < -max_diff) {
		printf("Update time from %s to %s\n", currentTimeStr, timeStoreStr);
		SettingClock(temp, timeStore);
	} else {
		printf("Nothing to do right now\n");
	}

	return 0;
}
