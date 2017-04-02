#pragma once

#define MAJOR_VERSION   (8)

#define MINOR_VERSION   (11)
#define PATCH_VERSION   (1)

/* using MAJOR.MINOR.PATCH version system */
#define ISSUE_VERSION   (\
                         ((MAJOR_VERSION << 24) & 0xFF000000)|\
                         ((MINOR_VERSION << 16) & 0x00FF0000)|\
                         ((PATCH_VERSION) & 0x0000FFFF)
