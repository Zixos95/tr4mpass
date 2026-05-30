#ifndef TR4MPASS_LIBIRECOVERY_COMPAT_H
#define TR4MPASS_LIBIRECOVERY_COMPAT_H

#include <unistd.h>

#include <libirecovery.h>

/*
 * libirecovery < 1.0.6: irecv_send_file() third argument was notify=1
 * instead of an options bitfield.  Value 1 is identical to the modern
 * IRECV_SEND_OPT_DFU_NOTIFY_FINISH flag.
 */
#ifndef IRECV_SEND_OPT_NONE
#define IRECV_SEND_OPT_NONE 0
#endif

#ifndef IRECV_SEND_OPT_DFU_NOTIFY_FINISH
#define IRECV_SEND_OPT_DFU_NOTIFY_FINISH 1
#endif

/*
 * tr4mpass_irecv_usb_pid -- Return the USB product ID for an open client.
 *
 * Modern libirecovery exposes pid in irecv_device_info; older releases
 * only provide irecv_get_mode(), which returns the same value.
 */
static inline int
tr4mpass_irecv_usb_pid(irecv_client_t client, unsigned *pid_out)
{
    int mode = 0;

    if (!client || !pid_out)
        return -1;

    if (irecv_get_mode(client, &mode) != IRECV_E_SUCCESS)
        return -1;

    *pid_out = (unsigned)mode;
    return 0;
}

/*
 * tr4mpass_irecv_open_with_ecid_and_attempts -- Retry wrapper around
 * irecv_open_with_ecid() for distros that lack the upstream helper.
 */
static inline irecv_error_t
tr4mpass_irecv_open_with_ecid_and_attempts(irecv_client_t *client,
                                            uint64_t ecid, int attempts)
{
    int i;

    if (!client || attempts < 1)
        return IRECV_E_INVALID_INPUT;

    for (i = 0; i < attempts; i++) {
        if (*client) {
            irecv_close(*client);
            *client = NULL;
        }
        if (irecv_open_with_ecid(client, ecid) == IRECV_E_SUCCESS)
            return IRECV_E_SUCCESS;
        if (i + 1 < attempts)
            sleep(1);
    }

    return IRECV_E_UNABLE_TO_CONNECT;
}

#endif /* TR4MPASS_LIBIRECOVERY_COMPAT_H */
