/*
 * Copyright 2008, Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 * 
 * Authors:
 *		Michael Pfeiffer <laplace@users.sourceforge.net>
 */
#ifndef LEGACY_BOOT_DRIVE_H
#define LEGACY_BOOT_DRIVE_H

#include "BootDrive.h"

#include <SupportDefs.h>

const uint32 kBlockSize = 512;
// The number of blocks required to store the
// MBR including the Haiku boot loader.
const uint32 kNumberOfBootLoaderBlocks = 4;

const uint32 kMBRSignature = 0xAA55;

const int32 kMaxBootMenuItemLength = 70;
	
typedef struct {
	uint8 bootLoader[440];
	uint8 diskSignature[4];
	uint8 reserved[2];
	uint8 partition[64];	
	uint8 signature[2];
} MasterBootRecord;

class LegacyBootDrive : public BootDrive
{
public:
	LegacyBootDrive();
	~LegacyBootDrive();

	bool IsBootMenuInstalled(BMessage* settings);
	status_t ReadPartitions(BMessage* settings);
	status_t WriteBootMenu(BMessage* settings);
	status_t SaveMasterBootRecord(BMessage* settings, BFile* file);
	status_t RestoreMasterBootRecord(BMessage* settings, BFile* file);
	status_t GetDisplayText(const char* text, BString& displayText);

private:
	bool _ConvertToBIOSText(const char* text, BString& biosText);
	bool _GetBiosDrive(const char* device, int8* drive);
	status_t _ReadBlocks(int fd, uint8* buffer, size_t size);
	status_t _WriteBlocks(int fd, const uint8* buffer, size_t size);
	void _CopyPartitionTable(MasterBootRecord* destination,
		const MasterBootRecord* source);
	bool _IsValid(const MasterBootRecord* mbr);
};

#endif	// LEGACY_BOOT_DRIVE_H
