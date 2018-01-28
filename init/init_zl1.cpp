/*
 * Copyright (c) 2017 Paranoid Android
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following
 *   disclaimer in the documentation and/or other materials provided
 *   with the distribution.
 * Neither the name of The Linux Foundation nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "vendor_init.h"
#include "property_service.h"
#include "init_msm.h"

#define DEVINFO_FILE "/dev/block/bootdevice/by-name/devinfo"

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void init_msm_properties(unsigned long msm_id, unsigned long msm_ver, char *board_type) {
    char device[PROP_VALUE_MAX];
    int isLEX720 = 0, isLEX722 = 0, isLEX727 = 0;

    if (read_file2(DEVINFO_FILE, device, sizeof(device)))
    {
        if (!strncmp(device, "le_zl0_whole_netcom", 19))
        {
            isLEX722 = 1;
        }
        else if (!strncmp(device, "le_zl1_oversea", 14))
        {
            isLEX727 = 1;
        }
        else if (!strncmp(device, "le_zl1", 6))
        {
            isLEX720 = 1;
        }
    }

    if (isLEX720 == 1)
    {
        // Set the main properties for the Chinese variant.
        property_set("persist.multisim.config", "dsds");
        property_set("persist.radio.multisim.config", "dsds");
        property_set("ro.telephony.default_network", "22,22");
        property_set("ro.product.model", "LEX720");
        property_set("ro.product.customize", "whole-netcom");
    }
    else if (isLEX727 == 1) 
    {
        // Set the main properties for the USA variant.
        property_set("persist.multisim.config", "NA");
        property_set("persist.radio.multisim.config", "NA");
        property_set("ro.telephony.default_network", "9");
        property_set("ro.product.model", "LEX727");
        property_set("ro.product.customize", "oversea");
    }
    else if (isLEX722 == 1) 
    {
        // Set the main properties for the 722 variant.
        property_override("ro.product.device", "le_zl0");
        property_set("persist.multisim.config", "dsds");
        property_set("persist.radio.multisim.config", "dsds");
        property_set("ro.telephony.default_network", "22,22");
        property_set("ro.product.model", "LEX722");
        property_set("ro.product.customize", "whole-netcom");
        // Power profile
        property_set("ro.power_profile.override", "power_profile_zl0");
        // Fingerprint
        property_override("ro.build.description", "le_zl0-user 6.0.1 WIXCNFN5802001232S eng.letv.20170123.152935 release-keys");
        property_override("ro.build.fingerprint", "LeEco/ZL1_CN/le_zl0:6.0.1/WIXCNFN5802001232S/letv01231534:user/release-keys");
    }
    else
    {
        property_override("ro.product.model", "UNKNOWN");
    }

    // Set the expected 'le_zl1' properties used in our blobs.
    property_set("ro.config.product", "le_zl1");
}
