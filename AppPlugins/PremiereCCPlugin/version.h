#pragma once

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0
#define VERSION_BUILD 0

#define xstr(a) str(a)
#define str(a) #a

#define BINVERSION VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_BUILD
#define STRVERSION xstr(VERSION_MAJOR) "." xstr(VERSION_MINOR) "." xstr(VERSION_PATCH) "." xstr(VERSION_BUILD)
