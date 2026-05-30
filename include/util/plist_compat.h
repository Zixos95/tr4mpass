#ifndef TR4MPASS_PLIST_COMPAT_H
#define TR4MPASS_PLIST_COMPAT_H

#include <stdlib.h>
#include <plist/plist.h>

/*
 * libplist < 2.2 does not declare plist_mem_free().  Buffers from
 * plist_to_xml() are plain malloc() allocations; free() is correct on
 * every libplist version (plist_mem_free() is a thin wrapper around it).
 */
#ifndef plist_mem_free
#define plist_mem_free(ptr) free(ptr)
#endif

#endif /* TR4MPASS_PLIST_COMPAT_H */
