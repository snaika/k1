#pragma once

// For kernel module
#define VERSION_MODIFIED_CODE	0xE0
#define VERSION_RESET_CODE	0xE1

// For user
#define VERSION_MODIFIED	_IO(0, VERSION_MODIFIED_CODE)
#define VERSION_RESET		_IO(0, VERSION_RESET_CODE)

#define IS_MODIFIED		1
#define IS_NOT_MODIFIED		0