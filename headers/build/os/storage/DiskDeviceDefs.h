/*
 * Copyright 2003-2006, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _DISK_DEVICE_DEFS_H
#define _DISK_DEVICE_DEFS_H


#include <SupportDefs.h>


typedef int32 partition_id;
typedef int32 disk_system_id;
typedef int32 disk_job_id;

// partition flags
enum {
	B_PARTITION_IS_DEVICE			= 0x01,
	B_PARTITION_FILE_SYSTEM			= 0x02,
	B_PARTITION_PARTITIONING_SYSTEM	= 0x04,
	B_PARTITION_READ_ONLY			= 0x08,
	B_PARTITION_MOUNTED				= 0x10,	// needed?
	B_PARTITION_BUSY				= 0x20,
	B_PARTITION_DESCENDANT_BUSY		= 0x40,
};

// partition statuses
enum {
	B_PARTITION_VALID,
	B_PARTITION_CORRUPT,
	B_PARTITION_UNRECOGNIZED,
	B_PARTITION_UNINITIALIZED,	// Only when uninitialized manually.
								// When not recognized while scanning it's
								// B_PARTITION_UNRECOGNIZED.
};

// partition change flags
enum {
	B_PARTITION_CHANGED_OFFSET				= 0x000001,
	B_PARTITION_CHANGED_SIZE				= 0x000002,
	B_PARTITION_CHANGED_CONTENT_SIZE		= 0x000004,
	B_PARTITION_CHANGED_BLOCK_SIZE			= 0x000008,
	B_PARTITION_CHANGED_STATUS				= 0x000010,
	B_PARTITION_CHANGED_FLAGS				= 0x000020,
	B_PARTITION_CHANGED_VOLUME				= 0x000040,
	B_PARTITION_CHANGED_NAME				= 0x000080,
	B_PARTITION_CHANGED_CONTENT_NAME		= 0x000100,
	B_PARTITION_CHANGED_TYPE				= 0x000200,
	B_PARTITION_CHANGED_CONTENT_TYPE		= 0x000400,
	B_PARTITION_CHANGED_PARAMETERS			= 0x000800,
	B_PARTITION_CHANGED_CONTENT_PARAMETERS	= 0x001000,
	B_PARTITION_CHANGED_CHILDREN			= 0x002000,
	B_PARTITION_CHANGED_DESCENDANTS			= 0x004000,
	B_PARTITION_CHANGED_DEFRAGMENTATION		= 0x008000,
	B_PARTITION_CHANGED_CHECK				= 0x010000,
	B_PARTITION_CHANGED_REPAIR				= 0x020000,
	B_PARTITION_CHANGED_INITIALIZATION		= 0x040000,
};

// disk device flags
enum {
	B_DISK_DEVICE_REMOVABLE		= 0x01,
	B_DISK_DEVICE_HAS_MEDIA		= 0x02,
	B_DISK_DEVICE_READ_ONLY		= 0x04,
	B_DISK_DEVICE_WRITE_ONCE	= 0x08,
};

// disk system flags
enum {
	B_DISK_SYSTEM_IS_FILE_SYSTEM								= 0x0001,

	// flags common for both file and partitioning systems
	B_DISK_SYSTEM_SUPPORTS_CHECKING								= 0x0002,
	B_DISK_SYSTEM_SUPPORTS_REPAIRING							= 0x0004,
	B_DISK_SYSTEM_SUPPORTS_RESIZING								= 0x0008,
	B_DISK_SYSTEM_SUPPORTS_MOVING								= 0x0010,
	B_DISK_SYSTEM_SUPPORTS_SETTING_CONTENT_NAME					= 0x0020,
	B_DISK_SYSTEM_SUPPORTS_SETTING_CONTENT_PARAMETERS			= 0x0040,

	// file system specific flags
	B_DISK_SYSTEM_SUPPORTS_DEFRAGMENTING						= 0x0100,
	B_DISK_SYSTEM_SUPPORTS_DEFRAGMENTING_WHILE_MOUNTED			= 0x0200,
	B_DISK_SYSTEM_SUPPORTS_CHECKING_WHILE_MOUNTED				= 0x0400,
	B_DISK_SYSTEM_SUPPORTS_REPAIRING_WHILE_MOUNTED				= 0x0800,
	B_DISK_SYSTEM_SUPPORTS_RESIZING_WHILE_MOUNTED				= 0x1000,
	B_DISK_SYSTEM_SUPPORTS_MOVING_WHILE_MOUNTED					= 0x2000,
	B_DISK_SYSTEM_SUPPORTS_SETTING_CONTENT_NAME_WHILE_MOUNTED	= 0x4000,
	B_DISK_SYSTEM_SUPPORTS_SETTING_CONTENT_PARAMETERS_WHILE_MOUNTED	= 0x8000,

	// partitioning system specific flags
	B_DISK_SYSTEM_SUPPORTS_RESIZING_CHILD						= 0x0100,
	B_DISK_SYSTEM_SUPPORTS_MOVING_CHILD							= 0x0200,
	B_DISK_SYSTEM_SUPPORTS_SETTING_NAME							= 0x0400,
	B_DISK_SYSTEM_SUPPORTS_SETTING_TYPE							= 0x0800,
	B_DISK_SYSTEM_SUPPORTS_SETTING_PARAMETERS					= 0x1000,
	B_DISK_SYSTEM_SUPPORTS_CREATING_CHILD						= 0x2000,
	B_DISK_SYSTEM_SUPPORTS_DELETING_CHILD						= 0x4000,
	B_DISK_SYSTEM_SUPPORTS_INITIALIZING							= 0x8000,
};

// disk device job types
enum {
	B_DISK_DEVICE_JOB_BAD_TYPE,
	B_DISK_DEVICE_JOB_DEFRAGMENT,
	B_DISK_DEVICE_JOB_REPAIR,
	B_DISK_DEVICE_JOB_RESIZE,
	B_DISK_DEVICE_JOB_MOVE,
	B_DISK_DEVICE_JOB_SET_NAME,
	B_DISK_DEVICE_JOB_SET_CONTENT_NAME,
	B_DISK_DEVICE_JOB_SET_TYPE,
	B_DISK_DEVICE_JOB_SET_PARMETERS,
	B_DISK_DEVICE_JOB_SET_CONTENT_PARMETERS,
	B_DISK_DEVICE_JOB_INITIALIZE,
	B_DISK_DEVICE_JOB_UNINITIALIZE,
	B_DISK_DEVICE_JOB_CREATE,
	B_DISK_DEVICE_JOB_DELETE,
	B_DISK_DEVICE_JOB_SCAN,
};

// disk device job statuses
enum {
	B_DISK_DEVICE_JOB_UNINITIALIZED,
	B_DISK_DEVICE_JOB_SCHEDULED,
	B_DISK_DEVICE_JOB_IN_PROGRESS,
	B_DISK_DEVICE_JOB_SUCCEEDED,
	B_DISK_DEVICE_JOB_FAILED,
	B_DISK_DEVICE_JOB_CANCELED,
};

// disk device job progress info
typedef struct disk_device_job_progress_info {
	uint32	status;
	uint32	interrupt_properties;
	int32	task_count;
	int32	completed_tasks;
	float	current_task_progress;
	char	current_task_description[256];
} disk_device_job_progress_info;

// disk device job interrupt properties
enum {
	B_DISK_DEVICE_JOB_CAN_CANCEL			= 0x01,
	B_DISK_DEVICE_JOB_STOP_ON_CANCEL		= 0x02,
	B_DISK_DEVICE_JOB_REVERSE_ON_CANCEL		= 0x04,
	B_DISK_DEVICE_JOB_CAN_PAUSE				= 0x08,
};

// string length constants, all of which include the NULL terminator
#define B_DISK_DEVICE_TYPE_LENGTH B_FILE_NAME_LENGTH
#define B_DISK_DEVICE_NAME_LENGTH B_FILE_NAME_LENGTH
#define B_DISK_SYSTEM_NAME_LENGTH B_PATH_NAME_LENGTH

// max size of parameter string buffers, including NULL terminator
#define B_DISK_DEVICE_MAX_PARAMETER_SIZE (32 * 1024)

#endif	// _DISK_DEVICE_DEFS_H
