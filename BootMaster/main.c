/*
 * BootMaster/main.c
 * Main code for the boot menu
 *
 * Copyright (c) 2006-2010 Christoph Pfisterer
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *  * Neither the name of Christoph Pfisterer nor the names of the
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * Modifications copyright (c) 2012-2021 Roderick W. Smith
 *
 * Modifications distributed under the terms of the GNU General Public
 * License (GPL) version 3 (GPLv3), or (at your option) any later version.
 */
/*
 * Modified for RefindPlus
 * Copyright (c) 2020-2022 Dayo Akanji (sf.net/u/dakanji/profile)
 * Portions Copyright (c) 2021 Joe van Tunen (joevt@shaw.ca)
 *
 * Modifications distributed under the preceding terms.
 */

#include "global.h"
#include "config.h"
#include "screenmgt.h"
#include "launch_legacy.h"
#include "lib.h"
#include "icns.h"
#include "menu.h"
#include "mok.h"
#include "scan.h"
#include "apple.h"
#include "install.h"
#include "mystrings.h"
#include "launch_efi.h"
#include "driver_support.h"
#include "security_policy.h"
#include "../include/refit_call_wrapper.h"
#include "../libeg/efiConsoleControl.h"
#include "../libeg/efiUgaDraw.h"
#include "../include/version.h"
#include "../libeg/libeg.h"

#ifndef __MAKEWITH_GNUEFI
#define LibLocateProtocol EfiLibLocateProtocol
#endif

INT16 NowYear   = 0;
INT16 NowMonth  = 0;
INT16 NowDay    = 0;
INT16 NowHour   = 0;
INT16 NowMinute = 0;
INT16 NowSecond = 0;

REFIT_MENU_SCREEN *MainMenu = NULL;

REFIT_CONFIG GlobalConfig = {
    /* DirectBoot = */ FALSE,
    /* CustomScreenBG = */ FALSE,
    /* TextOnly = */ FALSE,
    /* ScanAllLinux = */ TRUE,
    /* DeepLegacyScan = */ FALSE,
    /* RescanDXE = */ TRUE,
    /* RansomDrives = */ FALSE,
    /* EnableAndLockVMX = */ FALSE,
    /* FoldLinuxKernels = */ TRUE,
    /* EnableMouse = */ FALSE,
    /* EnableTouch = */ FALSE,
    /* HiddenTags = */ TRUE,
    /* UseNvram = */ FALSE,
    /* TransientBoot = */ FALSE,
    /* HiddenIconsIgnore = */ FALSE,
    /* HiddenIconsExternal = */ FALSE,
    /* HiddenIconsPrefer = */ FALSE,
    /* UseTextRenderer = */ FALSE,
    /* PassUgaThrough = */ FALSE,
    /* ProvideConsoleGOP = */ FALSE,
    /* ReloadGOP = */ TRUE,
    /* UseDirectGop = */ FALSE,
    /* ContinueOnWarning = */ FALSE,
    /* ForceTRIM = */ FALSE,
    /* DisableCompatCheck = */ FALSE,
    /* DisableAMFI = */ FALSE,
    /* SupplyNVME = */ TRUE,
    /* SupplyAPFS = */ TRUE,
    /* SupplyUEFI = */ TRUE,
    /* SilenceAPFS = */ TRUE,
    /* SyncAPFS = */ TRUE,
    /* ProtectNVRAM = */ TRUE,
    /* PanicFilter = */ TRUE,
    /* ScanAllESP = */ FALSE,
    /* TagsHelp = */ TRUE,
    /* TextHelp = */ TRUE,
    /* NormaliseCSR = */ FALSE,
    /* ShutdownAfterTimeout = */ FALSE,
    /* Install = */ FALSE,
    /* WriteSystemdVars = */ FALSE,
    /* UnicodeCollation = */ FALSE,
    /* SupplyAppleFB = */ TRUE,
    /* RequestedScreenWidth = */ 0,
    /* RequestedScreenHeight = */ 0,
    /* BannerBottomEdge = */ 0,
    /* RequestedTextMode = */ DONT_CHANGE_TEXT_MODE,
    /* HideUIFlags = */ 0,
    /* MaxTags = */ 0,
    /* GraphicsFor = */ GRAPHICS_FOR_OSX,
    /* LegacyType = */ LEGACY_TYPE_MAC,
    /* ScanDelay = */ 0,
    /* MouseSpeed = */ 4,
    /* IconSizes = */ {
        DEFAULT_BIG_ICON_SIZE / 4,
        DEFAULT_SMALL_ICON_SIZE,
        DEFAULT_BIG_ICON_SIZE,
        DEFAULT_MOUSE_SIZE
    },
    /* BannerScale = */ BANNER_NOSCALE,
    /* ScreensaverTime = */ 0,
    /* Timeout = */ 0,
    /* ScaleUI = */ 0,
    /* DynamicCSR = */ 0,
    /* LogLevel = */ 0,
    /* IconRowMove = */ 0,
    /* IconRowTune = */ 0,
    /* ScreenR = */ -1,
    /* ScreenG = */ -1,
    /* ScreenB = */ -1,
    /* *DiscoveredRoot = */ NULL,
    /* *SelfDevicePath = */ NULL,
    /* *ScreenBackground = */ NULL,
    /* *BannerFileName = */ NULL,
    /* *ConfigFilename = */ CONFIG_FILE_NAME,
    /* *SelectionSmallFileName = */ NULL,
    /* *SelectionBigFileName = */ NULL,
    /* *DefaultSelection = */ NULL,
    /* *AlsoScan = */ NULL,
    /* *DontScanVolumes = */ NULL,
    /* *DontScanDirs = */ NULL,
    /* *DontScanFiles = */ NULL,
    /* *DontScanTools = */ NULL,
    /* *DontScanFirmware = */ NULL,
    /* *WindowsRecoveryFiles = */ NULL,
    /* *MacOSRecoveryFiles = */ NULL,
    /* *DriverDirs = */ NULL,
    /* *IconsDir = */ NULL,
    /* *SetBootArgs = */ NULL,
    /* *ExtraKernelVersionStrings = */ NULL,
    /* *SpoofOSXVersion = */ NULL,
    /* CsrValues = */ NULL,
    /* ShowTools = */ {
        TAG_SHELL,
        TAG_MEMTEST,
        TAG_GDISK,
        TAG_RECOVERY_APPLE,
        TAG_RECOVERY_WINDOWS,
        TAG_MOK_TOOL,
        TAG_ABOUT,
        TAG_HIDDEN,
        TAG_SHUTDOWN,
        TAG_REBOOT,
        TAG_FIRMWARE,
        TAG_FWUPDATE_TOOL,
        0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};

#define NVRAMCLEAN_FILES L"\\EFI\\BOOT\\tools_x64\\x64_CleanNvram.efi,\\EFI\\BOOT\\tools_x64\\CleanNvram_x64.efi,\
\\EFI\\BOOT\\tools_x64\\CleanNvram.efi,\\EFI\\BOOT\\tools\\x64_CleanNvram.efi,\\EFI\\BOOT\\tools\\CleanNvram_x64.efi,\
\\EFI\\BOOT\\tools\\CleanNvram.efi,\\EFI\\tools\\x64_CleanNvram.efi,\\EFI\\tools\\CleanNvram_x64.efi,\
\\EFI\\tools\\CleanNvram.efi,\\EFI\\tools_x64\\x64_CleanNvram.efi,\\EFI\\tools_x64\\CleanNvram_x64.efi,\
\\EFI\\tools_x64\\CleanNvram.efi,\\EFI\\x64_CleanNvram.efi,\\EFI\\CleanNvram_x64.efi,\\EFI\\CleanNvram.efi,\
\\x64_CleanNvram.efi,\\CleanNvram_x64.efi,\\CleanNvram.efi"

UINTN                  AppleFramebuffers    = 0;
CHAR16                *VendorInfo           = NULL;
CHAR16                *gHiddenTools         = NULL;
BOOLEAN                IsBoot               = FALSE;
BOOLEAN                SetSysTab            = FALSE;
BOOLEAN                ConfigWarn           = FALSE;
BOOLEAN                OverrideSB           = FALSE;
BOOLEAN                BlockRescan          = FALSE;
BOOLEAN                NativeLogger         = FALSE;
BOOLEAN                ranCleanNvram        = FALSE;
BOOLEAN                AppleFirmware        = FALSE;
BOOLEAN                AllowTweakUEFI       = FALSE;
BOOLEAN                FlushFailedTag       = FALSE;
BOOLEAN                FlushFailReset       = FALSE;
BOOLEAN                WarnVersionEFI       = FALSE;
BOOLEAN                WarnRevisionUEFI     = FALSE;
BOOLEAN                WarnMissingQVInfo    = FALSE;
BOOLEAN                SecureBootFailure    = FALSE;
EFI_GUID               RefindPlusGuid       = REFINDPLUS_GUID;
EFI_GUID               RefindGuid           = REFIND_GUID_VALUE;
EFI_SET_VARIABLE       OrigSetVariableRT;
EFI_OPEN_PROTOCOL      OrigOpenProtocolBS;


extern VOID              InitBooterLog (VOID);

extern EFI_STATUS        AmendSysTable (VOID);
extern EFI_STATUS        RP_ApfsConnectDevices (VOID);
extern EFI_STATUS EFIAPI NvmExpressLoad (
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
);

#ifdef __MAKEWITH_TIANO
extern VOID              OcUnblockUnmountedPartitions (VOID);
#endif

// Link to Cert GUIDs in mok/guid.c
extern EFI_GUID                      X509_GUID;
extern EFI_GUID                      RSA2048_GUID;
extern EFI_GUID                      PKCS7_GUID;
extern EFI_GUID                      EFI_CERT_SHA256_GUID;

extern EFI_FILE                     *gVarsDir;

extern BOOLEAN                       ForceRescanDXE;

extern EFI_GRAPHICS_OUTPUT_PROTOCOL *GOPDraw;

//
// Misc functions
//


static
VOID InitMainMenu (VOID) {
    REFIT_MENU_SCREEN MainMenuSrc = {
        L"Main Menu",
        NULL, 0,
        NULL, 0,
        NULL, 0,
        L"Automatic Boot",
        SUBSCREEN_HINT1,
        L"Press 'Insert', 'Tab', or 'F2' for More Options and 'Esc' or 'Backspace' to Refresh"
    };

    FreeMenuScreen (&MainMenu);
    MainMenu = CopyMenuScreen (&MainMenuSrc);
    MainMenu->TimeoutSeconds = GlobalConfig.Timeout;
} // static VOID InitMainMenu()

static
EFI_STATUS GetHardwareNvramVariable (
    IN  CHAR16    *VariableName,
    IN  EFI_GUID  *VendorGuid,
    OUT VOID     **VariableData,
    OUT UINTN     *VariableSize  OPTIONAL
) {
    VOID        *TmpBuffer    = NULL;
    UINTN        BufferSize   = 0;
    EFI_STATUS   Status       = EFI_LOAD_ERROR;

    // Pass in a zero-size buffer to find the required buffer size.
    // If the variable exists, the status should be EFI_BUFFER_TOO_SMALL and BufferSize updated.
    // Any other status means the variable does not exist.
    Status = REFIT_CALL_5_WRAPPER(
        gRT->GetVariable, VariableName,
        VendorGuid, NULL,
        &BufferSize, TmpBuffer
    );
    if (Status != EFI_BUFFER_TOO_SMALL) {
        return EFI_NOT_FOUND;
    }

    TmpBuffer = AllocateZeroPool (BufferSize);
    if (!TmpBuffer) {
        return EFI_OUT_OF_RESOURCES;
    }

    // Retry with the correct buffer size.
    Status = REFIT_CALL_5_WRAPPER(
        gRT->GetVariable, VariableName,
        VendorGuid, NULL,
        &BufferSize, TmpBuffer
    );
    if (EFI_ERROR(Status)) {
        MY_FREE_POOL(TmpBuffer);
        *VariableData = NULL;
        *VariableSize = 0;

        return EFI_LOAD_ERROR;
    }

    *VariableData = TmpBuffer;
    *VariableSize = (BufferSize) ? BufferSize : 0;

    return EFI_SUCCESS;
} // static EFI_STATUS GetHardwareNvramVariable()

static
EFI_STATUS SetHardwareNvramVariable (
    IN  CHAR16    *VariableName,
    IN  EFI_GUID  *VendorGuid,
    IN  UINT32     Attributes,
    IN  UINTN      VariableSize,
    IN  VOID      *VariableData
) {
    EFI_STATUS   Status;
    VOID        *OldBuf;
    UINTN        OldSize;
    BOOLEAN      SettingMatch;

    Status = GetHardwareNvramVariable (
        VariableName, VendorGuid,
        &OldBuf, &OldSize
    );
    if (EFI_ERROR(Status)) {
        SettingMatch = FALSE;
    }
    else {
        // Check for settings match
        SettingMatch = (
            VariableSize == OldSize &&
            CompareMem (VariableData, OldBuf, VariableSize) == 0
        );
    }
    MY_FREE_POOL(OldBuf);

    if (Status == EFI_OUT_OF_RESOURCES) {
        return Status;
    }
    if (SettingMatch) {
        // Early Return
        return EFI_ALREADY_STARTED;
    }

    if (Status != EFI_NOT_FOUND) {
        // Clear the current value
        OrigSetVariableRT (
            VariableName, VendorGuid,
            Attributes, 0, NULL
        );
    }

    // Store the new value
    Status = OrigSetVariableRT (
        VariableName, VendorGuid,
        Attributes, VariableSize, VariableData
    );

    return Status;
} // EFI_STATUS SetHardwareNvramVariable()


static
EFI_STATUS EFIAPI gRTSetVariableEx (
    IN  CHAR16    *VariableName,
    IN  EFI_GUID  *VendorGuid,
    IN  UINT32     Attributes,
    IN  UINTN      VariableSize,
    IN  VOID      *VariableData
) {
    EFI_STATUS     Status;
    EFI_GUID       WinGuid               = MICROSOFT_VENDOR_GUID;
    EFI_GUID       X509Guid              = X509_GUID;
    EFI_GUID       PKCS7Guid             = PKCS7_GUID;
    EFI_GUID       Sha001Guid            = EFI_CERT_SHA1_GUID;
    EFI_GUID       Sha224Guid            = EFI_CERT_SHA224_GUID;
    EFI_GUID       Sha256Guid            = EFI_CERT_SHA256_GUID;
    EFI_GUID       Sha384Guid            = EFI_CERT_SHA384_GUID;
    EFI_GUID       Sha512Guid            = EFI_CERT_SHA512_GUID;
    EFI_GUID       RSA2048Guid           = RSA2048_GUID;
    EFI_GUID       RSA2048Sha1Guid       = EFI_CERT_RSA2048_SHA1_GUID;
    EFI_GUID       RSA2048Sha256Guid     = EFI_CERT_RSA2048_SHA256_GUID;
    EFI_GUID       TypeRSA2048Sha256Guid = EFI_CERT_TYPE_RSA2048_SHA256_GUID;

    BOOLEAN BlockAppleKP = FALSE;
    BOOLEAN BlockMore = FALSE;
    BOOLEAN BlockCert = (
        AppleFirmware &&
        (
            GuidsAreEqual (VendorGuid, &WinGuid) ||
            GuidsAreEqual (VendorGuid, &X509Guid) ||
            GuidsAreEqual (VendorGuid, &PKCS7Guid) ||
            GuidsAreEqual (VendorGuid, &Sha001Guid) ||
            GuidsAreEqual (VendorGuid, &Sha224Guid) ||
            GuidsAreEqual (VendorGuid, &Sha256Guid) ||
            GuidsAreEqual (VendorGuid, &Sha384Guid) ||
            GuidsAreEqual (VendorGuid, &Sha512Guid) ||
            GuidsAreEqual (VendorGuid, &RSA2048Guid) ||
            GuidsAreEqual (VendorGuid, &RSA2048Sha1Guid) ||
            GuidsAreEqual (VendorGuid, &RSA2048Sha256Guid) ||
            GuidsAreEqual (VendorGuid, &TypeRSA2048Sha256Guid)
        )
    );

    #if REFIT_DEBUG > 0
    BOOLEAN CheckMute   = FALSE;
    BOOLEAN ForceNative = FALSE;

    MY_MUTELOGGER_SET;
    #endif
    if (!BlockCert) {
        BlockMore = (
            AppleFirmware &&
            (
                (
                    FoundSubStr (VariableName, L"UnlockID")
                ) || (
                    CompareGuid (VendorGuid, &gEfiGlobalVariableGuid) &&
                    (
                        MyStriCmp (VariableName, L"PK")  || /* EFI_PLATFORM_KEY_NAME     */
                        MyStriCmp (VariableName, L"KEK")    /* EFI_KEY_EXCHANGE_KEY_NAME */
                    )
                ) || (
                    CompareGuid (VendorGuid, &gEfiImageSecurityDatabaseGuid) &&
                    (
                        MyStriCmp (VariableName, L"db")  || /* EFI_IMAGE_SECURITY_DATABASE0 */
                        MyStriCmp (VariableName, L"dbx") || /* EFI_IMAGE_SECURITY_DATABASE1 */
                        MyStriCmp (VariableName, L"dbt") || /* EFI_IMAGE_SECURITY_DATABASE2 */
                        MyStriCmp (VariableName, L"dbr")    /* EFI_IMAGE_SECURITY_DATABASE3 */
                    )
                )
            )
        );

        if (!BlockMore && GlobalConfig.PanicFilter) {
            EFI_GUID AppleGUID = APPLE_GUID;
            BlockAppleKP = (
                AppleFirmware &&
                CompareGuid (VendorGuid, &AppleGUID) &&
                FoundSubStr (VariableName, L"AAPL,PanicInfo")
            );
        }
    }

    Status = (BlockCert || BlockMore || BlockAppleKP)
    ? EFI_SUCCESS
    : SetHardwareNvramVariable (
        VariableName, VendorGuid,
        Attributes, VariableSize, VariableData
    );

    #if REFIT_DEBUG > 0
    MY_MUTELOGGER_OFF;

    /* Enable Forced Native Logging */
    MY_NATIVELOGGER_SET;

    // Log Outcome
    CHAR16 *LogStatus = PoolPrint (
        L"%r",
        (BlockCert || BlockMore || BlockAppleKP)
            ? EFI_ACCESS_DENIED : Status
    );

    MY_MUTELOGGER_SET;
    LimitStringLength (LogStatus, 13);
    MY_MUTELOGGER_OFF;

    // Do not free LogName
    CHAR16 *LogName = NULL;
    if (0);
    else if (StrCmp (VariableName, L"db") )   LogName = L"EFI_IMAGE_SECURITY_DATABASE0";
    else if (StrCmp (VariableName, L"dbx"))   LogName = L"EFI_IMAGE_SECURITY_DATABASE1";
    else if (StrCmp (VariableName, L"dbt"))   LogName = L"EFI_IMAGE_SECURITY_DATABASE2";
    else if (StrCmp (VariableName, L"dbr"))   LogName = L"EFI_IMAGE_SECURITY_DATABASE3";
    else if (StrCmp (VariableName, L"KEK"))   LogName = L"EFI_KEY_EXCHANGE_KEY_NAME"   ;
    else if (StrCmp (VariableName, L"PK") )   LogName = L"EFI_PLATFORM_KEY_NAME"       ;

    CHAR16 *MsgStr = PoolPrint (
        L"In Hardware NVRAM ... %13s %s:- '%s%s'",
        LogStatus,
        NVRAM_LOG_SET,
        (BlockCert)
            ? L"Certificate  :::  "
            : (BlockMore)
                ? L"SecurityTag  :::  "
                : (BlockAppleKP)
                    ? L"KernelPanic  :::  "
                    : L"",
        (LogName) ? LogName : VariableName
    );
    LOG_MSG("%s", MsgStr);
    LOG_MSG("\n");
    MY_FREE_POOL(MsgStr);
    MY_FREE_POOL(LogStatus);

    /* Disable Forced Native Logging */
    MY_NATIVELOGGER_OFF;
    #endif

    /* Clear any previously saved instance of blocked variable */
    /* BlockAppleKP is excluded as has dynamic naming */
    if (BlockCert || BlockMore) {
        OrigSetVariableRT (
            VariableName, VendorGuid,
            Attributes, 0, NULL
        );
    }

    return Status;
} // VOID gRTSetVariableEx()

static
VOID SetProtectNvram (
    IN EFI_SYSTEM_TABLE *SystemTable,
    IN BOOLEAN           Activate
) {
    static BOOLEAN ProtectActive = FALSE;

    if (!GlobalConfig.ProtectNVRAM) {
        // Early Return
        return;
    }

    if (Activate) {
        if (!ProtectActive) {
            ProtectActive                             =             TRUE;
            RT->SetVariable                           = gRTSetVariableEx;
            gRT->SetVariable                          = gRTSetVariableEx;
            SystemTable->RuntimeServices->SetVariable = gRTSetVariableEx;
            gRT->Hdr.CRC32                            =                0;
            REFIT_CALL_3_WRAPPER(
                gBS->CalculateCrc32, gRT,
                gRT->Hdr.HeaderSize, &gRT->Hdr.CRC32
            );
        }
    }
    else {
        if (ProtectActive) {
            ProtectActive                             =             FALSE;
            RT->SetVariable                           = OrigSetVariableRT;
            gRT->SetVariable                          = OrigSetVariableRT;
            SystemTable->RuntimeServices->SetVariable = OrigSetVariableRT;
            gRT->Hdr.CRC32                            =                 0;
            REFIT_CALL_3_WRAPPER(
                gBS->CalculateCrc32, gRT,
                gRT->Hdr.HeaderSize, &gRT->Hdr.CRC32
            );
        }
    }
} // static VOID SetProtectNvram()

static
EFI_STATUS FilterCSR (VOID) {
    EFI_STATUS Status = EFI_NOT_STARTED;

    if (GlobalConfig.NormaliseCSR) {
        // Filter out the 'APPLE_INTERNAL' CSR bit if present
        Status = NormaliseCSR();
    }

    #if REFIT_DEBUG > 0
    if (EFI_ERROR(Status)) {
        CHAR16 *MsgStr = PoolPrint (
            L"Normalise CSR ... %r",
            Status
        );
        ALT_LOG(1, LOG_THREE_STAR_MID, L"%s", MsgStr);
        LOG_MSG("%s    * %s", OffsetNext, MsgStr);
        MY_FREE_POOL(MsgStr);
    }
    #endif

    return Status;
} // static EFI_STATUS FilterCSR()

static
VOID AlignCSR (VOID) {
    UINT32  CsrStatus;
    BOOLEAN RotateCsr = FALSE;

    if ((GlobalConfig.DynamicCSR == 0) ||
        (GlobalConfig.DynamicCSR != -1 && GlobalConfig.DynamicCSR != 1)
    ) {
        // Early return if improperly set or configured not to set CSR
        return;
    }

    // Prime 'Status' for logging
    #if REFIT_DEBUG > 0
    EFI_STATUS Status = EFI_ALREADY_STARTED;
    #endif

    // Try to get current CSR status
    if (GetCsrStatus (&CsrStatus) != EFI_SUCCESS) {
        // Early Return
        return;
    }

    // Record CSR status in the 'gCsrStatus' variable
    RecordgCsrStatus (CsrStatus, FALSE);

    // Check 'gCsrStatus' variable for 'Enabled' term
    BOOLEAN CsrEnabled = (MyStrStr (gCsrStatus, L"Enabled")) ? TRUE : FALSE;

    if (GlobalConfig.DynamicCSR == -1) {
        // Set to always disable
        //
        // Seed the log buffer
        #if REFIT_DEBUG > 0
        LOG_MSG("INFO: Disable");
        #endif

        if (CsrEnabled) {
            // Switch SIP/SSV off as currently enabled
            RotateCsr = TRUE;
        }
    }
    else {
        // Set to always enable ... GlobalConfig.DynamicCSR == 1
        //
        // Seed the log buffer
        #if REFIT_DEBUG > 0
        LOG_MSG("INFO: Enable");
        #endif

        if (!CsrEnabled) {
            // Switch SIP/SSV on as currently disbled
            RotateCsr = TRUE;
        }
    }

    if (RotateCsr) {
        // Toggle SIP/SSV from current setting
        RotateCsrValue ();

        // Set 'Status' to 'Success'
        #if REFIT_DEBUG > 0
        Status = EFI_SUCCESS;
        #endif
    }

    // Finalise and flush the log buffer
    #if REFIT_DEBUG > 0
    LOG_MSG(" SIP/SSV ... %r", Status);
    LOG_MSG("\n\n");
    #endif
} // static VOID AlignCSR()

#if REFIT_DEBUG > 0
static
VOID LogDisableCheck (
    IN CHAR16     *TypStr,
    IN EFI_STATUS  Result
) {
    CHAR16 *MsgStr = PoolPrint (
        L"%s ... %r",
        TypStr, Result
    );
    ALT_LOG(1, LOG_LINE_NORMAL, L"%s", MsgStr);
    LOG_MSG("%s    * %s", OffsetNext, MsgStr);
    MY_FREE_POOL(MsgStr);
} // static VOID LogDisableCheck()
#endif

static
VOID SetBootArgs (VOID) {
    EFI_STATUS   Status;
    EFI_GUID     AppleGUID  = APPLE_GUID;
    CHAR16      *NameNVRAM  = L"boot-args";
    CHAR16      *BootArg    = NULL;
    CHAR8       *DataNVRAM  = NULL;

    #if REFIT_DEBUG > 0
    CHAR16  *MsgStr                = NULL;
    BOOLEAN  LogDisableAMFI        = FALSE;
    BOOLEAN  LogDisableCompatCheck = FALSE;
    #endif

    if (!GlobalConfig.SetBootArgs || GlobalConfig.SetBootArgs[0] == L'\0') {
        #if REFIT_DEBUG > 0
        Status = EFI_INVALID_PARAMETER;

        MsgStr = PoolPrint (
            L"Reset Boot Args ... %r",
            Status
        );
        ALT_LOG(1, LOG_LINE_NORMAL, L"%s", MsgStr);
        LOG_MSG("%s    * %s", OffsetNext, MsgStr);
        MY_FREE_POOL(MsgStr);
        #endif

        // Early Return
        return;
    }

    if (MyStrStr (GlobalConfig.SetBootArgs, L"amfi_get_out_of_my_way")) {
        #if REFIT_DEBUG > 0
        if (GlobalConfig.DisableAMFI) {
            // Ensure Logging
            LogDisableAMFI = TRUE;
        }
        #endif

        // Do not duplicate 'amfi_get_out_of_my_way=1'
        GlobalConfig.DisableAMFI = FALSE;
    }

    if (MyStrStr (GlobalConfig.SetBootArgs, L"-no_compat_check")) {
        #if REFIT_DEBUG > 0
        if (GlobalConfig.DisableCompatCheck) {
            // Ensure Logging
            LogDisableCompatCheck = TRUE;
        }
        #endif

        // Do not duplicate '-no_compat_check'
        GlobalConfig.DisableCompatCheck = FALSE;
    }

    if (GlobalConfig.DisableAMFI &&
        GlobalConfig.DisableCompatCheck
    ) {
        // Combine Args with DisableAMFI and DisableCompatCheck
        BootArg = PoolPrint (
            L"%s amfi_get_out_of_my_way=1 -no_compat_check",
            GlobalConfig.SetBootArgs
        );
    }
    else if (GlobalConfig.DisableAMFI) {
        // Combine Args with DisableAMFI
        BootArg = PoolPrint (
            L"%s amfi_get_out_of_my_way=1",
            GlobalConfig.SetBootArgs
        );
    }
    else if (GlobalConfig.DisableCompatCheck) {
        // Combine Args with DisableCompatCheck
        BootArg = PoolPrint (
            L"%s -no_compat_check",
            GlobalConfig.SetBootArgs
        );
    }
    else {
        // Use Args Alone
        BootArg = StrDuplicate (GlobalConfig.SetBootArgs);
    }

    DataNVRAM = AllocatePool (
        (StrLen (BootArg) + 1) * sizeof (CHAR8)
    );
    if (!DataNVRAM) {
        MY_FREE_POOL(BootArg);

        #if REFIT_DEBUG > 0
        Status = EFI_OUT_OF_RESOURCES;

        MsgStr = PoolPrint (
            L"Reset Boot Args ... %r",
            Status
        );
        ALT_LOG(1, LOG_LINE_NORMAL, L"%s", MsgStr);
        LOG_MSG("%s    * %s", OffsetNext, MsgStr);
        MY_FREE_POOL(MsgStr);
        #endif

        // Early Return
        return;
    }

    VOID *VarData = NULL;
    GetHardwareNvramVariable (
        NameNVRAM, &AppleGUID,
        &VarData, NULL
    );

    if (VarData && MyStriCmp (BootArg, VarData)) {
        #if REFIT_DEBUG > 0
        Status = EFI_ALREADY_STARTED;

        MsgStr = PoolPrint (
            L"%r When Setting Boot Args:- '%s'",
            Status, BootArg
        );
        ALT_LOG(1, LOG_LINE_NORMAL, L"%s", MsgStr);
        LOG_MSG("%s    ** %s", OffsetNext, MsgStr);
        MY_FREE_POOL(MsgStr);
        #endif

        MY_FREE_POOL(VarData);
        MY_FREE_POOL(BootArg);

        // Early Return
        return;
    }

    // Convert Unicode String 'BootArg' to Ascii String 'DataNVRAM'
    UnicodeStrToAsciiStr (BootArg, DataNVRAM);

    Status = EfivarSetRaw (
        &AppleGUID, NameNVRAM,
        DataNVRAM, AsciiStrSize (DataNVRAM), TRUE
    );

    #if REFIT_DEBUG > 0
    if (LogDisableAMFI || GlobalConfig.DisableAMFI) {
        LogDisableCheck (L"Disable AMFI Checks", Status);
    }

    if (LogDisableCompatCheck || GlobalConfig.DisableCompatCheck) {
        LogDisableCheck (L"Disable Compat Check", Status);
    }

    MsgStr = PoolPrint (
        L"%r When Setting Boot Args:- '%s'",
        Status, GlobalConfig.SetBootArgs
    );
    ALT_LOG(1, LOG_LINE_NORMAL, L"%s", MsgStr);
    LOG_MSG("%s    ** %s", OffsetNext, MsgStr);
    MY_FREE_POOL(MsgStr);
    #endif

    MY_FREE_POOL(BootArg);
    MY_FREE_POOL(DataNVRAM);
} // static VOID SetBootArgs()

static
EFI_STATUS NoCheckCompat (VOID) {
    EFI_STATUS   Status     = EFI_SUCCESS;
    EFI_GUID     AppleGUID  = APPLE_GUID;
    CHAR16      *NameNVRAM  = L"boot-args";
    CHAR16      *ArgData    = L"-no_compat_check";
    CHAR16      *BootArg    = NULL;
    CHAR8       *DataNVRAM  = NULL;
    VOID        *VarData    = NULL;

    if (!GlobalConfig.DisableCompatCheck) {
        // Early Return ... Do Not Log
        return EFI_NOT_STARTED;
    }

    GetHardwareNvramVariable (
        NameNVRAM, &AppleGUID,
        &VarData, NULL
    );

    if (!VarData) {
        BootArg = StrDuplicate (ArgData);
    }
    else {
        CHAR16 *BootArgCheck = PoolPrint (L"%s", VarData);
        if (FoundSubStr (BootArgCheck, ArgData)) {
            Status = EFI_ALREADY_STARTED;
        }
        else {
            BootArg = PoolPrint (
                L"%s %s",
                VarData, ArgData
            );
        }
        MY_FREE_POOL(BootArgCheck);
    }

    if (!EFI_ERROR(Status)) {
        DataNVRAM = AllocatePool (
            (StrLen (BootArg) + 1) * sizeof (CHAR8)
        );
        if (!DataNVRAM) {
            Status = EFI_OUT_OF_RESOURCES;
        }

        if (!EFI_ERROR(Status)) {
            // Convert Unicode String 'BootArg' to Ascii String 'DataNVRAM'
            UnicodeStrToAsciiStr (BootArg, DataNVRAM);

            Status = EfivarSetRaw (
                &AppleGUID, NameNVRAM,
                DataNVRAM, AsciiStrSize (DataNVRAM), TRUE
            );
        }
    }
    MY_FREE_POOL(BootArg);

    #if REFIT_DEBUG > 0
    LogDisableCheck (L"Disable Compat Check", Status);
    #endif

    return Status;
} // static EFI_STATUS NoCheckCompat()

static
EFI_STATUS NoCheckAMFI (VOID) {
    EFI_STATUS   Status     = EFI_SUCCESS;
    EFI_GUID     AppleGUID  = APPLE_GUID;
    CHAR16      *NameNVRAM  = L"boot-args";
    CHAR16      *ArgData    = L"amfi_get_out_of_my_way";
    CHAR16      *BootArg    = NULL;
    CHAR8       *DataNVRAM  = NULL;
    VOID        *VarData    = NULL;

    if (!GlobalConfig.DisableAMFI) {
        // Early Return ... Do Not Log
        return EFI_NOT_STARTED;
    }

    GetHardwareNvramVariable (
        NameNVRAM, &AppleGUID,
        &VarData, NULL
    );

    if (!VarData) {
        BootArg = StrDuplicate (ArgData);
    }
    else {
        CHAR16 *BootArgCheck = PoolPrint (L"%s", VarData);
        if (FoundSubStr (BootArgCheck, ArgData)) {
            Status = EFI_ALREADY_STARTED;
        }
        else {
            BootArg = PoolPrint (
                L"%s %s=1",
                VarData, ArgData
            );
        }
        MY_FREE_POOL(BootArgCheck);
    }

    if (!EFI_ERROR(Status)) {
        DataNVRAM = AllocatePool (
            (StrLen (BootArg) + 1) * sizeof (CHAR8)
        );
        if (!DataNVRAM) {
            Status = EFI_OUT_OF_RESOURCES;
        }

        if (!EFI_ERROR(Status)) {
            // Convert Unicode String 'BootArg' to Ascii String 'DataNVRAM'
            UnicodeStrToAsciiStr (BootArg, DataNVRAM);

            Status = EfivarSetRaw (
                &AppleGUID, NameNVRAM,
                DataNVRAM, AsciiStrSize (DataNVRAM), TRUE
            );
        }
    }
    MY_FREE_POOL(BootArg);

    #if REFIT_DEBUG > 0
    LogDisableCheck (L"Disable AMFI Checks", Status);
    #endif

    return Status;
} // static EFI_STATUS NoCheckAMFI()

static
EFI_STATUS TrimCoerce (VOID) {
    if (!GlobalConfig.ForceTRIM) {
        // Early Return
        return EFI_NOT_STARTED;
    }

    EFI_GUID   AppleGUID    = APPLE_GUID;
    CHAR16    *NameNVRAM    = L"EnableTRIM";
    CHAR8      DataNVRAM[1] = {0x01};
    EFI_STATUS Status = EfivarSetRaw (
        &AppleGUID, NameNVRAM,
        DataNVRAM, sizeof (DataNVRAM), TRUE
    );

    #if REFIT_DEBUG > 0
    CHAR16 *MsgStr = PoolPrint (
        L"Forcefully Enable TRIM ... %r",
        Status
    );
    ALT_LOG(1, LOG_LINE_NORMAL, L"%s", MsgStr);
    LOG_MSG("%s    * %s", OffsetNext, MsgStr);
    MY_FREE_POOL(MsgStr);
    #endif

    return Status;
} // static EFI_STATUS TrimCoerce()


// Extended 'OpenProtocol'
// Ensures GOP Interface for Boot Loading Screen
static
EFI_STATUS EFIAPI OpenProtocolEx (
    IN   EFI_HANDLE    Handle,
    IN   EFI_GUID     *Protocol,
    OUT  VOID        **Interface OPTIONAL,
    IN   EFI_HANDLE    AgentHandle,
    IN   EFI_HANDLE    ControllerHandle,
    IN   UINT32        Attributes
) {
    EFI_STATUS   Status;
    UINTN        i              = 0;
    UINTN        HandleCount    = 0;
    EFI_HANDLE  *HandleBuffer   = NULL;

    Status = REFIT_CALL_6_WRAPPER(
        OrigOpenProtocolBS, Handle,
        Protocol, Interface,
        AgentHandle, ControllerHandle, Attributes
    );
    if (Status == EFI_UNSUPPORTED) {
        // Early Return
        return EFI_UNSUPPORTED;
    }

    if (!GuidsAreEqual (&gEfiGraphicsOutputProtocolGuid, Protocol)) {
        // Early Return
        return Status;
    }

    if (GOPDraw != NULL) {
        *Interface = GOPDraw;

        // Early Return
        return EFI_SUCCESS;
    }

    Status = REFIT_CALL_5_WRAPPER(
        gBS->LocateHandleBuffer, ByProtocol,
        &gEfiGraphicsOutputProtocolGuid, NULL,
        &HandleCount, &HandleBuffer
    );
    if (EFI_ERROR(Status)) {
        // Early Return
        return EFI_UNSUPPORTED;
    }

    for (i = 0; i < HandleCount; i++) {
        if (HandleBuffer[i] != gST->ConsoleOutHandle) {
            Status = REFIT_CALL_6_WRAPPER(
                OrigOpenProtocolBS, HandleBuffer[i],
                &gEfiGraphicsOutputProtocolGuid, *Interface,
                AgentHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
            );
            if (!EFI_ERROR(Status) && *Interface != NULL) {
                break;
            }
        }
    } // for

    if (EFI_ERROR(Status) || *Interface == NULL) {
        Status = EFI_UNSUPPORTED;
    }

    MY_FREE_POOL(HandleBuffer);

    return Status;
} // EFI_STATUS OpenProtocolEx()


// Extended 'HandleProtocol'
// Routes 'HandleProtocol' to 'OpenProtocol'
static
EFI_STATUS EFIAPI HandleProtocolEx (
    IN   EFI_HANDLE   Handle,
    IN   EFI_GUID    *Protocol,
    OUT  VOID       **Interface
) {
    EFI_STATUS Status;

    Status = REFIT_CALL_6_WRAPPER(
        gBS->OpenProtocol, Handle,
        Protocol, Interface,
        gImageHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
    );

    return Status;
} // EFI_STATUS HandleProtocolEx()

static
VOID ReMapOpenProtocol (VOID) {
    if (!GOPDraw) {
        // Early Return if GOP is Absent
        return;
    }
    if (!AllowTweakUEFI) {
        // Early Return on Invalid UEFI Versions
        return;
    }

    if (AppleFirmware) {
        if (!DevicePresence) {
            // Early Return on Compact Macs
            return;
        }
        if (AppleFramebuffers == 0) {
            // Early Return on Macs without AppleFramebuffers
            return;
        }
    }

    // Amend EFI_BOOT_SERVICES.OpenProtocol
    gBS->OpenProtocol = OpenProtocolEx;
    gBS->Hdr.CRC32    = 0;
    REFIT_CALL_3_WRAPPER(
        gBS->CalculateCrc32, gBS,
        gBS->Hdr.HeaderSize, &gBS->Hdr.CRC32
    );
} // ReMapOpenProtocol()

static
BOOLEAN ShowCleanNvramInfo (
    CHAR16 *ToolPurpose
) {
    REFIT_MENU_SCREEN *CleanNvramInfoMenu = AllocateZeroPool (sizeof (REFIT_MENU_SCREEN));
    if (CleanNvramInfoMenu == NULL) {
        // Early Return
        return FALSE;
    }

    CleanNvramInfoMenu->TitleImage = BuiltinIcon (BUILTIN_ICON_TOOL_NVRAMCLEAN);
    CleanNvramInfoMenu->Title      = StrDuplicate (L"CleanNvram");
    CleanNvramInfoMenu->Hint1      = StrDuplicate (L"Press 'ESC', 'BackSpace' or 'SpaceBar' to Return to Main Menu");
    CleanNvramInfoMenu->Hint2      = StrDuplicate (L"");

    AddMenuInfoLineAlt (CleanNvramInfoMenu, PoolPrint (L"A Tool to %s", ToolPurpose));
    AddMenuInfoLine (CleanNvramInfoMenu, L"");
    AddMenuInfoLine (CleanNvramInfoMenu, L"The binary must be placed in one of the paths below");
    AddMenuInfoLine (CleanNvramInfoMenu, L" - The first file found in the order listed will be used");
    AddMenuInfoLine (CleanNvramInfoMenu, L" - You will be returned to the main menu if not found");
    AddMenuInfoLine (CleanNvramInfoMenu, L"");

    UINTN k = 0;
    CHAR16 *FilePath = NULL;
    while ((FilePath = FindCommaDelimited (NVRAMCLEAN_FILES, k++)) != NULL) {
        AddMenuInfoLineAlt (CleanNvramInfoMenu, StrDuplicate (FilePath));
        MY_FREE_POOL(FilePath);
    }

    AddMenuInfoLine (CleanNvramInfoMenu, L"");

    REFIT_MENU_ENTRY *MenuEntryCleanNvram = AllocateZeroPool (sizeof (REFIT_MENU_ENTRY));
    if (MenuEntryCleanNvram == NULL) {
        FreeMenuScreen (&CleanNvramInfoMenu);

        // Early Return
        return FALSE;
    }

    MenuEntryCleanNvram->Title = StrDuplicate (ToolPurpose);
    MenuEntryCleanNvram->Tag   = TAG_GENERIC;
    AddMenuEntry (CleanNvramInfoMenu, MenuEntryCleanNvram);

    BOOLEAN RetVal = GetReturnMenuEntry (&CleanNvramInfoMenu);
    if (!RetVal) {
        FreeMenuScreen (&CleanNvramInfoMenu);

        // Early Return
        return FALSE;
    }

    // DA_TAG: Investigate This
    //         Delete the directory tree instead?
    //         Hook into Shell for 'rm -fr' if so?
    //         Initial Assessment: Not worth bloat
    //         Find native way to achieve same??
    //
    // Clear Emulated NVRAM
    #if REFIT_DEBUG > 0
    BOOLEAN CheckMute = FALSE;
    MY_MUTELOGGER_SET;
    #endif
    EfivarSetRaw (
        &RefindPlusGuid, L"PreviousBoot",
        NULL, 0, TRUE
    );
    EfivarSetRaw (
        &RefindPlusGuid, L"HiddenTags",
        NULL, 0, TRUE
    );
    EfivarSetRaw (
        &RefindPlusGuid, L"HiddenTools",
        NULL, 0, TRUE
    );
    EfivarSetRaw (
        &RefindPlusGuid, L"HiddenLegacy",
        NULL, 0, TRUE
    );
    EfivarSetRaw (
        &RefindPlusGuid, L"HiddenFirmware",
        NULL, 0, TRUE
    );

    // Try to handle rEFInd as well
    EfivarSetRaw (
        &RefindGuid, L"PreviousBoot",
        NULL, 0, TRUE
    );
    EfivarSetRaw (
        &RefindGuid, L"HiddenTags",
        NULL, 0, TRUE
    );
    EfivarSetRaw (
        &RefindGuid, L"HiddenTools",
        NULL, 0, TRUE
    );
    EfivarSetRaw (
        &RefindGuid, L"HiddenLegacy",
        NULL, 0, TRUE
    );
    EfivarSetRaw (
        &RefindGuid, L"HiddenFirmware",
        NULL, 0, TRUE
    );
    #if REFIT_DEBUG > 0
    MY_MUTELOGGER_OFF;
    #endif

    INTN               DefaultEntry   = 1;
    MENU_STYLE_FUNC    Style          = GraphicsMenuStyle;
    REFIT_MENU_ENTRY  *ChosenEntry    = NULL;
    UINTN MenuExit = RunGenericMenu (CleanNvramInfoMenu, Style, &DefaultEntry, &ChosenEntry);

    #if REFIT_DEBUG > 0
    ALT_LOG(1, LOG_LINE_NORMAL,
        L"Returned '%d' (%s) from RunGenericMenu Call on '%s' in 'ShowCleanNvramInfo'",
        MenuExit, MenuExitInfo (MenuExit), ChosenEntry->Title
    );
    LOG_MSG("Received Input:");
    LOG_MSG("%s  - %s", OffsetNext, ChosenEntry->Title);
    #endif

    RetVal = TRUE;
    if (MenuExit != MENU_EXIT_ENTER || ChosenEntry->Tag != TAG_GENERIC) {
        RetVal = FALSE;
    }

    FreeMenuScreen (&CleanNvramInfoMenu);

    return RetVal;
} // static BOOLEAN ShowCleanNvramInfo()

static
VOID AboutRefindPlus (VOID) {
    REFIT_MENU_SCREEN *AboutMenu = AllocateZeroPool (sizeof (REFIT_MENU_SCREEN));
    if (AboutMenu == NULL) {
        // Early Return
        return;
    }

    #if REFIT_DEBUG > 0
    ALT_LOG(1, LOG_LINE_THIN_SEP, L"Creating 'About RefindPlus' Screen");
    #endif

    UINT32   CsrStatus;
    CHAR16  *VendorString = StrDuplicate (VendorInfo);

    // More than ~65 causes empty info page on 800x600 display
    LimitStringLength (VendorString, MAX_LINE_LENGTH);

    AboutMenu->TitleImage = BuiltinIcon (BUILTIN_ICON_FUNC_ABOUT);
    AboutMenu->Title      = StrDuplicate (L"About RefindPlus");
    AboutMenu->Hint1      = StrDuplicate (L"Press 'Enter' to Return to Main Menu");
    AboutMenu->Hint2      = StrDuplicate (L"");

    AboutMenu->InfoLines  = (CHAR16 **) AllocateZeroPool (sizeof (CHAR16 *));
    if (AboutMenu->InfoLines == NULL) {
        FreeMenuScreen (&AboutMenu);
        MY_FREE_POOL(VendorString);

        // Early Return
        return;
    }

    AddMenuInfoLineAlt (
        AboutMenu,
        PoolPrint (
            L"RefindPlus v%s",
            REFINDPLUS_VERSION
        )
    );

    AddMenuInfoLine (AboutMenu, L"");
    AddMenuInfoLine (AboutMenu, L"Copyright (c) 2020-2022 Dayo Akanji and Others");
    AddMenuInfoLine (AboutMenu, L"Portions Copyright (c) 2012-2021 Roderick W. Smith");
    AddMenuInfoLine (AboutMenu, L"Portions Copyright (c) 2006-2010 Christoph Pfisterer");
    AddMenuInfoLine (AboutMenu, L"Portions Copyright (c) The Intel Corporation and Others");
    AddMenuInfoLine (AboutMenu, L"Distributed under the terms of the GNU GPLv3 license");
    AddMenuInfoLine (AboutMenu, L"");

#if defined (__MAKEWITH_GNUEFI)
    AddMenuInfoLine (AboutMenu, L"Built with GNU-EFI");
#else
    AddMenuInfoLine (AboutMenu, L"Built with TianoCore EDK II");
#endif

    AddMenuInfoLine (AboutMenu, L"");

    AddMenuInfoLineAlt (
        AboutMenu,
        PoolPrint (
            L"Firmware Vendor: %s",
            VendorString
        )
    );

#if defined (EFI32)
    AddMenuInfoLine (AboutMenu, L"Platform: x86 (32 bit)");
#elif defined (EFIX64)
    AddMenuInfoLine (AboutMenu, L"Platform: x86_64 (64 bit)");
#elif defined (EFIAARCH64)
    AddMenuInfoLine (AboutMenu, L"Platform: ARM (64 bit)");
#else
    AddMenuInfoLine (AboutMenu, L"Platform: Unknown");
#endif

    AddMenuInfoLineAlt (
        AboutMenu,
        PoolPrint (
            L"EFI Revision: %s %d.%02d",
            ((gST->Hdr.Revision >> 16) == 1) ? L"EFI" : L"UEFI",
            gST->Hdr.Revision >> 16,
            gST->Hdr.Revision & ((1 << 16) - 1)
        )
    );
    AddMenuInfoLineAlt (
        AboutMenu,
        PoolPrint (
            L"Secure Boot: %s",
            secure_mode() ? L"Active" : L"Inactive"
        )
    );

    if (GetCsrStatus (&CsrStatus) == EFI_SUCCESS) {
        RecordgCsrStatus (CsrStatus, FALSE);
        AddMenuInfoLine (AboutMenu, gCsrStatus);
    }

    AddMenuInfoLineAlt (
        AboutMenu,
        PoolPrint(
            L"Screen Output: %s",
            egScreenDescription()
        )
    );

    AddMenuInfoLine (AboutMenu, L"");
    AddMenuInfoLine (AboutMenu, L"RefindPlus is a variant of rEFInd");
    AddMenuInfoLine (AboutMenu, L"https://github.com/dakanji/RefindPlus");
    AddMenuInfoLine (AboutMenu, L"");
    AddMenuInfoLine (AboutMenu, L"For information on rEFInd, visit:");
    AddMenuInfoLine (AboutMenu, L"http://www.rodsbooks.com/refind");

    BOOLEAN RetVal = GetReturnMenuEntry (&AboutMenu);
    if (!RetVal) {
        FreeMenuScreen (&AboutMenu);

        // Early Return
        return;
    }

    RunMenu (AboutMenu, NULL);

    FreeMenuScreen (&AboutMenu);
    MY_FREE_POOL(VendorString);
} // static VOID AboutRefindPlus()

// Record the loader's name/description in the "PreviousBoot" UEFI variable
// if different from what is already stored there.
VOID StoreLoaderName (
    IN CHAR16 *Name
) {
    // Clear current PreviousBoot if TransientBoot is active
    if (GlobalConfig.TransientBoot) {
        EfivarSetRaw (
            &RefindPlusGuid, L"PreviousBoot",
            NULL, 0, TRUE
        );

        return;
    }

    if (!Name) {
        // Early Return
        return;
    }

    EfivarSetRaw (
        &RefindPlusGuid, L"PreviousBoot",
        Name, StrLen (Name) * 2 + 2, TRUE
    );
} // VOID StoreLoaderName()

// Rescan for boot loaders
VOID RescanAll (
    BOOLEAN Reconnect
) {
    #if REFIT_DEBUG > 0
    BOOLEAN ForceNative = FALSE;
    #endif

    #if REFIT_DEBUG > 0
    CHAR16 *MsgStr = L"R E S C A N   A L L   I T E M S";
    ALT_LOG(1, LOG_STAR_SEPARATOR, L"%s", MsgStr);
    LOG_MSG("I N F O :   %s", MsgStr);
    LOG_MSG("\n\n");
    #endif

    #if REFIT_DEBUG > 0
    /* Enable Forced Native Logging */
    MY_NATIVELOGGER_SET;
    #endif

    // Reset MainMenu
    InitMainMenu();

    // ConnectAllDriversToAllControllers() can cause system hangs with some
    // buggy filesystem drivers, so do it only if necessary.
    if (Reconnect) {
        // Always rescan for DXE drivers when connecting drivers here
        ForceRescanDXE = TRUE;
        ConnectAllDriversToAllControllers (FALSE);

        ScanVolumes();
    }

    ReadConfig (GlobalConfig.ConfigFilename);
    if (OverrideSB) {
        GlobalConfig.Timeout    = 0;
        GlobalConfig.TextOnly   = TRUE;
        GlobalConfig.DirectBoot = FALSE;
    }

    SetVolumeIcons();
    ScanForBootloaders();

    #if REFIT_DEBUG > 0
    /* Disable Forced Native Logging */
    MY_NATIVELOGGER_OFF;
    #endif

    ScanForTools();
} // VOID RescanAll()

#ifdef __MAKEWITH_TIANO
// Minimal initialisation function
static
VOID InitializeLib (
    IN EFI_HANDLE         ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
) {
    gImageHandle = ImageHandle;
    gST          = SystemTable;
    gBS          = SystemTable->BootServices;
    gRT          = SystemTable->RuntimeServices;

    EfiGetSystemConfigurationTable (
        &gEfiDxeServicesTableGuid,
        (VOID **) &gDS
    );

    // Upgrade EFI_BOOT_SERVICES.HandleProtocol
    gBS->HandleProtocol = HandleProtocolEx;
    gBS->Hdr.CRC32      = 0;
    REFIT_CALL_3_WRAPPER(
        gBS->CalculateCrc32, gBS,
        gBS->Hdr.HeaderSize, &gBS->Hdr.CRC32
    );
} // static VOID InitializeLib()
#endif

// Set up our own Secure Boot extensions.
// Returns TRUE on success, FALSE otherwise
static
BOOLEAN SecureBootSetup (VOID) {
    if (secure_mode() && ShimLoaded()) {
        if (security_policy_install() == EFI_SUCCESS) {
            return TRUE;
        }

        SecureBootFailure = TRUE;

        #if REFIT_DEBUG > 0
        LOG_MSG("** FATAL ERROR: Failed to Install MOK Secure Boot Extensions");
        LOG_MSG("\n\n");
        #endif
    }

    return FALSE;
} // VOID static SecureBootSetup()

// Remove our own Secure Boot extensions.
// Returns TRUE on success, FALSE otherwise
static
BOOLEAN SecureBootUninstall (VOID) {
    EFI_STATUS  Status;
    BOOLEAN     Success   =  TRUE;

    #if REFIT_DEBUG > 0
    BOOLEAN CheckMute = FALSE;
    #endif

    if (secure_mode()) {
        Status = security_policy_uninstall();
        if (Status != EFI_SUCCESS) {
            Success = FALSE;
            BeginTextScreen (L"Secure Boot Policy Failure");

            CHAR16 *MsgStr = L"Failed to Uninstall MOK Secure Boot Extensions ... Forcing Shutdown in 9 Seconds";

            #if REFIT_DEBUG > 0
            LOG_MSG("%s", MsgStr);
            LOG_MSG("\n <<----- * ----->>\n\n");

            #endif

            #if REFIT_DEBUG > 0
            MY_MUTELOGGER_SET;
            #endif
            REFIT_CALL_2_WRAPPER(gST->ConOut->SetAttribute, gST->ConOut, ATTR_ERROR);
            PrintUglyText (MsgStr, NEXTLINE);
            REFIT_CALL_2_WRAPPER(gST->ConOut->SetAttribute, gST->ConOut, ATTR_BASIC);
            #if REFIT_DEBUG > 0
            MY_MUTELOGGER_OFF;
            #endif

            PauseSeconds (9);

            REFIT_CALL_4_WRAPPER(
                gRT->ResetSystem, EfiResetShutdown,
                EFI_SUCCESS, 0, NULL
            );
        }
    }

    return Success;
} // static BOOLEAN SecureBootUninstall()

// Sets the global configuration filename; will be CONFIG_FILE_NAME unless the
// "-c" command-line option is set, in which case that takes precedence.
// If an error is encountered, leaves the value alone (it should be set to
// CONFIG_FILE_NAME when GlobalConfig is initialized).
static
VOID SetConfigFilename (
    EFI_HANDLE ImageHandle
) {
    EFI_STATUS         Status;
    CHAR16            *Options;
    CHAR16            *FileName;
    CHAR16            *SubString;
    CHAR16            *MsgStr = NULL;
    EFI_LOADED_IMAGE  *Info;

    #if REFIT_DEBUG > 0
    BOOLEAN CheckMute = FALSE;
    #endif

    Status = REFIT_CALL_3_WRAPPER(
        gBS->HandleProtocol, ImageHandle,
        &LoadedImageProtocol, (VOID **) &Info
    );
    if ((Status == EFI_SUCCESS) && (Info->LoadOptionsSize > 0)) {
        Options   = (CHAR16 *) Info->LoadOptions;
        SubString = MyStrStr (Options, L" -c ");
        if (SubString) {
            #if REFIT_DEBUG > 0
            LOG_MSG("Set Config Filename from Command Line Option:");
            LOG_MSG("\n");
            #endif

            FileName = StrDuplicate (&SubString[4]);

            if (FileName) {
                LimitStringLength (FileName, 256);
            }

            if (FileExists (SelfDir, FileName)) {
                GlobalConfig.ConfigFilename = FileName;

                #if REFIT_DEBUG > 0
                LOG_MSG("  - Config File:- '%s'", FileName);
                LOG_MSG("\n\n");
                #endif
            }
            else {
                MsgStr = StrDuplicate (L"** WARN: Specified Config File Not Found");
                #if REFIT_DEBUG > 0
                LOG_MSG("%s", MsgStr);
                LOG_MSG("\n");
                #endif

                #if REFIT_DEBUG > 0
                MY_MUTELOGGER_SET;
                #endif
                REFIT_CALL_2_WRAPPER(gST->ConOut->SetAttribute, gST->ConOut, ATTR_ERROR);
                PrintUglyText (MsgStr, NEXTLINE);
                #if REFIT_DEBUG > 0
                MY_MUTELOGGER_OFF;
                #endif

                MY_FREE_POOL(MsgStr);
                MsgStr = StrDuplicate (L"         Try Default:- 'config.conf / refind.conf'");
                #if REFIT_DEBUG > 0
                LOG_MSG("%s", MsgStr);
                LOG_MSG("\n\n");
                #endif

                #if REFIT_DEBUG > 0
                MY_MUTELOGGER_SET;
                #endif
                PrintUglyText (MsgStr, NEXTLINE);
                REFIT_CALL_2_WRAPPER(gST->ConOut->SetAttribute, gST->ConOut, ATTR_BASIC);
                #if REFIT_DEBUG > 0
                MY_MUTELOGGER_OFF;
                #endif

                PauseSeconds (9);
                MY_FREE_POOL(MsgStr);
            } // if/else FileExists (SelfDir, FileName

            MY_FREE_POOL(FileName);
        } // if SubString
    } // if (Status == EFI_SUCCESS) && Info->LoadOptionsSize
} // static VOID SetConfigFilename()

// Adjust the GlobalConfig.DefaultSelection variable: Replace all "+" elements with the
//  PreviousBoot variable, if it is available. If it is not available, delete that element.
static
VOID AdjustDefaultSelection (VOID) {
    EFI_STATUS  Status;
    UINTN       i                 = 0;
    CHAR16     *Element           = NULL;
    CHAR16     *NewCommaDelimited = NULL;
    CHAR16     *PreviousBoot      = NULL;
    BOOLEAN     FormatLog;
    BOOLEAN     Ignore;

    #if REFIT_DEBUG > 0
    CHAR16     *MsgStr            = NULL;
    BOOLEAN     LoggedOnce        = FALSE;

    LOG_MSG("A D J U S T   D E F A U L T   S E L E C T I O N");
    #endif

    while ((Element = FindCommaDelimited (
        GlobalConfig.DefaultSelection, i++
    )) != NULL) {
        Ignore    = FALSE;
        FormatLog = FALSE;

        if (MyStriCmp (Element, L"+")) {
            if (GlobalConfig.TransientBoot &&
                StrLen (GlobalConfig.DefaultSelection) > 1
            ) {
                Ignore = TRUE;
            }

            if (!Ignore) {
                FormatLog = TRUE;
                MY_FREE_POOL(Element);

                Status = EfivarGetRaw (
                    &RefindPlusGuid, L"PreviousBoot",
                    (VOID **) &PreviousBoot, NULL
                );
                if (Status == EFI_SUCCESS) {
                    Element = PreviousBoot;
                }
            }
        }

        if (!Ignore && Element && StrLen (Element)) {
            #if REFIT_DEBUG > 0
            if (!LoggedOnce) {
                LoggedOnce = TRUE;
                if (FormatLog) {
                    BRK_MIN("\n");
                    MsgStr = StrDuplicate (L"Changed to Previous Selection");
                }
                else {
                    BRK_MOD("\n");
                    MsgStr = StrDuplicate (L"Changed to Preferred Selection");
                }
                LOG_MSG("%s:- '%s'", MsgStr, Element);
            }
            #endif

            MergeStrings (&NewCommaDelimited, Element, L',');
        }

        MY_FREE_POOL(Element);
    } // while

    #if REFIT_DEBUG > 0
    if (!LoggedOnce) {
        LOG_MSG("\n");
        LOG_MSG("INFO: No Change to Default Selection");
    }
    LOG_MSG("\n\n");
    #endif

    MY_FREE_POOL(GlobalConfig.DefaultSelection);
    GlobalConfig.DefaultSelection = NewCommaDelimited;
} // static VOID AdjustDefaultSelection()


#if REFIT_DEBUG > 0
static
VOID LogRevisionInfo (
    EFI_TABLE_HEADER *Hdr,
    CHAR16           *Name,
    UINT16            CompileSize,
    BOOLEAN           DoEFICheck
) {
    static BOOLEAN FirstRun = TRUE;

    (FirstRun) ? LOG_MSG("\n\n") : LOG_MSG("\n");
    FirstRun = FALSE;

    LOG_MSG(
        "%s:- '%-4s %d.%02d'",
        Name,
        DoEFICheck ? ((Hdr->Revision >> 16 == 1) ? L"EFI" : L"UEFI") : L"Ver",
        Hdr->Revision >> 16,
        Hdr->Revision & 0xffff
    );

    (Hdr->HeaderSize == CompileSize)
        ? LOG_MSG(" (HeaderSize %d)", Hdr->HeaderSize)
        : LOG_MSG(" (HeaderSize %d ... %d CompileSize)", Hdr->HeaderSize, CompileSize);
} // static VOID LogRevisionInfo()
#endif

// Log basic information (RefindPlus version, EFI version, etc.) to the log file.
// Also sets some variables that may be needed later
static
VOID LogBasicInfo (VOID) {
    UINTN  EfiMajorVersion = gST->Hdr.Revision >> 16;

#if REFIT_DEBUG > 0
    EFI_STATUS  Status;
    CHAR16     *MsgStr = NULL;
    UINT64      MaximumVariableSize;
    UINT64      MaximumVariableStorageSize;
    UINT64      RemainingVariableStorageSize;
    EFI_GUID    ConsoleControlProtocolGuid = EFI_CONSOLE_CONTROL_PROTOCOL_GUID;

    LogRevisionInfo (&gST->Hdr, L"    System Table", sizeof (*gST), TRUE);
    LogRevisionInfo (&gBS->Hdr, L"   Boot Services", sizeof (*gBS), TRUE);
    LogRevisionInfo (&gRT->Hdr, L"Runtime Services", sizeof (*gRT), TRUE);
    LogRevisionInfo (&gDS->Hdr, L"    DXE Services", sizeof (*gDS), FALSE);
    LOG_MSG("\n\n");
#endif


    // Get AppleFramebuffer Count
    AppleFramebuffers = egCountAppleFramebuffers();


    WarnVersionEFI = WarnRevisionUEFI = FALSE;
    if (((gST->Hdr.Revision >> 16) != EfiMajorVersion) ||
        ((gBS->Hdr.Revision >> 16) != EfiMajorVersion) ||
        ((gRT->Hdr.Revision >> 16) != EfiMajorVersion)
    ) {
        WarnVersionEFI = TRUE;
    }
    else if (
        ((gST->Hdr.Revision & 0xffff) != (gBS->Hdr.Revision & 0xffff)) ||
        ((gST->Hdr.Revision & 0xffff) != (gRT->Hdr.Revision & 0xffff)) ||
        ((gBS->Hdr.Revision & 0xffff) != (gRT->Hdr.Revision & 0xffff))
    ) {
        WarnRevisionUEFI = TRUE;
    }

    AllowTweakUEFI = TRUE;
    if (WarnVersionEFI || WarnRevisionUEFI) {
        if (WarnVersionEFI) {
            AllowTweakUEFI = FALSE;

#if REFIT_DEBUG > 0
            LOG_MSG("** WARN: Inconsistent EFI Versions Detected");
            LOG_MSG("%s         Program Behaviour is *NOT* Defined",   OffsetNext);
            LOG_MSG("%s         Priority = Stability Over Features",   OffsetNext);
            LOG_MSG("%s         * Disabled:- 'ProvideConsoleGOP'",     OffsetNext);
            LOG_MSG("%s         * Disabled:- 'ReMapOpenProtocol'",     OffsetNext);
            LOG_MSG("%s         * Disabled:- 'UseTextRenderer'",       OffsetNext);
            LOG_MSG("%s         * Disabled:- 'PassUgaThrough'",        OffsetNext);
            LOG_MSG("%s         * Disabled:- 'UseDirectGop'",          OffsetNext);
            LOG_MSG("%s         * Disabled:- 'ProtectNVRAM'",          OffsetNext);
            LOG_MSG("%s         * Disabled:- 'SilenceAPFS'",           OffsetNext);
            LOG_MSG("%s         * Disabled:- 'SupplyUEFI'",            OffsetNext);
            LOG_MSG("%s         * Disabled:- 'SupplyAPFS'",            OffsetNext);
            LOG_MSG("%s         * Disabled:- 'ReloadGOP'",             OffsetNext);
#endif
        }
        else {
#if REFIT_DEBUG > 0
            LOG_MSG("** WARN: Inconsistent UEFI Revisions Detected");
#endif
        } // if/else WarnVersionEFI

#if REFIT_DEBUG > 0
        LOG_MSG("\n\n");
#endif
    } // if/else WarnVersionEFI || WarnRevisionUEFI

#if REFIT_DEBUG > 0
    /* NVRAM Storage Info */
    BOOLEAN QVInfoSupport = FALSE;
    LOG_MSG("Non-Volatile Storage:");
#endif

    if (gRT->Hdr.Revision >> 16 > 1 && !AppleFirmware) {
        if ((gRT->Hdr.HeaderSize <
            MY_OFFSET_OF(EFI_RUNTIME_SERVICES, QueryVariableInfo) + sizeof(gRT->QueryVariableInfo))
        ) {
            WarnMissingQVInfo = TRUE;

#if REFIT_DEBUG > 0
            LOG_MSG("\n\n");
            LOG_MSG("** WARN: Inconsistent UEFI 2.x Implementation Detected");
            LOG_MSG("%s         Program Behaviour is *NOT* Defined", OffsetNext);
        }
        else {
            Status = REFIT_CALL_4_WRAPPER(
                gRT->QueryVariableInfo, EFI_VARIABLE_NON_VOLATILE,
                &MaximumVariableStorageSize, &RemainingVariableStorageSize, &MaximumVariableSize
            );
            if (EFI_ERROR(Status)) {
                LOG_MSG("\n\n");
                LOG_MSG("** WARN: Could Not Retrieve Non-Volatile Storage Info");
            }
            else {
                LOG_MSG("%s  - Total Storage         : %ld", OffsetNext, MaximumVariableStorageSize);
                LOG_MSG("%s  - Remaining Available   : %ld", OffsetNext, RemainingVariableStorageSize);
                LOG_MSG("%s  - Maximum Variable Size : %ld", OffsetNext, MaximumVariableSize);
            }
            QVInfoSupport = TRUE;
#endif
        }
    } // if gRT->Hdr.Revision

/**
 * Function ends here on RELEASE Builds
**/

#if REFIT_DEBUG > 0
    if (!QVInfoSupport) {
        // QueryVariableInfo is not supported on Apple or EFI 1.x Firmware
        LOG_MSG("%s*** QueryVariableInfo is Unavailable ***", OffsetNext);
    }
    LOG_MSG("\n\n");

    /**
     * Report which video output devices are natively available. We do not actually
     * use them, so just use MsgStr as a throwaway pointer to the protocol.
    **/
    LOG_MSG("ConsoleOut Modes:");

    Status = LibLocateProtocol (&ConsoleControlProtocolGuid, (VOID **) &MsgStr);
    LOG_MSG("%s  - Mode = Text           : %s", OffsetNext, EFI_ERROR(Status) ? L" NO" : L"YES");
    MY_FREE_POOL(MsgStr);

    Status = REFIT_CALL_3_WRAPPER(
        gBS->HandleProtocol, gST->ConsoleOutHandle,
        &gEfiUgaDrawProtocolGuid, (VOID **) &MsgStr
    );
    LOG_MSG("%s  - Mode = Graphics (UGA) : %s", OffsetNext, EFI_ERROR(Status) ? L" NO" : L"YES");
    MY_FREE_POOL(MsgStr);

    Status = REFIT_CALL_3_WRAPPER(
        gBS->HandleProtocol, gST->ConsoleOutHandle,
        &gEfiGraphicsOutputProtocolGuid, (VOID **) &MsgStr
    );
    LOG_MSG("%s  - Mode = Graphics (GOP) : %s", OffsetNext, EFI_ERROR(Status) ? L" NO" : L"YES");
    LOG_MSG("\n\n");
    MY_FREE_POOL(MsgStr);

    /* Secure Boot */
    LOG_MSG("Shim:- '%s'", ShimLoaded()         ? L"Present" : L"Absent");
    LOG_MSG("\n");
    LOG_MSG("Secure Boot:- '%s'", secure_mode() ? L"Active"  : L"Inactive");
    LOG_MSG("\n");
    LOG_MSG("Apple Framebuffers:- '%d'", AppleFramebuffers);
    LOG_MSG("\n");

    /* CSM Type */
    switch (GlobalConfig.LegacyType) {
        case LEGACY_TYPE_MAC:  MsgStr = StrDuplicate (L"Mac-Style");  break;
        case LEGACY_TYPE_UEFI: MsgStr = StrDuplicate (L"UEFI-Style"); break;
        case LEGACY_TYPE_NONE: MsgStr = StrDuplicate (L"Absent");     break;
        default:               MsgStr = StrDuplicate (L"Unknown");    break; // Just in case
    }
    LOG_MSG("Compat Support Module:- '%s'", MsgStr);
    LOG_MSG("\n\n");
    MY_FREE_POOL(MsgStr);
#endif
} // static VOID LogBasicInfo()

//
// main entry point
//
EFI_STATUS EFIAPI efi_main (
    EFI_HANDLE         ImageHandle,
    EFI_SYSTEM_TABLE  *SystemTable
) {
    EFI_STATUS  Status;

    BOOLEAN  MainLoopRunning =  TRUE;
    BOOLEAN  MokProtocol     = FALSE;

    #if REFIT_DEBUG > 0
    BOOLEAN CheckMute   = FALSE;
    BOOLEAN ForceNative = FALSE;
    #endif

    REFIT_MENU_ENTRY  *ChosenEntry    = NULL;
    LOADER_ENTRY      *ourLoaderEntry = NULL;
    LEGACY_ENTRY      *ourLegacyEntry = NULL;

    UINTN  i        = 0;
    UINTN  k        = 0;
    INTN   MenuExit = 0;

    CHAR16    *MsgStr             = NULL;
    CHAR16    *SelectionName      = NULL;
    EG_PIXEL   BGColor = COLOR_LIGHTBLUE;

    /* Init Bootstrap */
    InitializeLib (ImageHandle, SystemTable);
    Status = InitRefitLib (ImageHandle);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    /* Stash SystemTable */
    EFI_RUNTIME_SERVICES *OrigRT   =   gRT;
    EFI_BOOT_SERVICES    *OrigBS   =   gBS;
    EFI_SYSTEM_TABLE     *OrigST   =   gST;
    OrigSetVariableRT   = gRT->SetVariable;
    OrigOpenProtocolBS = gBS->OpenProtocol;

    /* Remove any recovery boot flags */
    ClearRecoveryBootFlag();

    /* Other Preambles */
    EFI_TIME Now;
    REFIT_CALL_2_WRAPPER(gRT->GetTime, &Now, NULL);
    NowYear   = Now.Year;
    NowMonth  = Now.Month;
    NowDay    = Now.Day;
    NowHour   = Now.Hour;
    NowMinute = Now.Minute;
    NowSecond = Now.Second;

    if (MyStrStr (gST->FirmwareVendor, L"Apple")) {
        AppleFirmware = TRUE;
        VendorInfo = StrDuplicate (L"Apple");
    }
    else {
        VendorInfo = PoolPrint (
            L"%s %d.%02d",
            gST->FirmwareVendor,
            gST->FirmwareRevision >> 16,
            gST->FirmwareRevision & ((1 << 16) - 1)
        );
    }

    /* Set Legacy Boot Type */
    FindLegacyBootType();

    /* Set Default Scan Pattern */
    (GlobalConfig.LegacyType == LEGACY_TYPE_MAC)
        ? CopyMem (GlobalConfig.ScanFor, "ihebocm    ", NUM_SCAN_OPTIONS)
        : CopyMem (GlobalConfig.ScanFor, "ieom       ", NUM_SCAN_OPTIONS);


    /* Init Logging */
    // DA-TAG: Also on RELEASE builds as we need the timer
    //         - Without logging output on RELEASE builds
    InitBooterLog();

    #if REFIT_DEBUG > 0
    /* Enable Forced Native Logging */
    MY_NATIVELOGGER_SET;
    #endif

    #if REFIT_DEBUG > 0
    /* Start Logging */
    LOG_MSG("B E G I N   B O O T S T R A P");
    LOG_MSG("\n");
    LOG_MSG(
        "Loading RefindPlus v%s on %s Firmware",
        REFINDPLUS_VERSION, VendorInfo
    );
    LOG_MSG("\n");

    /* Architecture */
    LOG_MSG("Arch/Type:- ");
#if defined(EFI32)
    LOG_MSG("'x86 (32 bit)'");
#elif defined(EFIX64)
    LOG_MSG("'x86 (64 bit)'");
#elif defined(EFIAARCH64)
    LOG_MSG("'ARM (64 bit)'");
#else
    LOG_MSG("'Not Known'");
#endif
    LOG_MSG("\n");

    /* Build Engine */
    LOG_MSG("Made With:- ");
#if defined(__MAKEWITH_GNUEFI)
    LOG_MSG("'GNU-EFI'");
#else
    LOG_MSG("'TianoCore EDK II'");
#endif
    LOG_MSG("\n");

    /* TimeStamp */
    CHAR16 *CurDateStr = NULL;
    if (Now.TimeZone == EFI_UNSPECIFIED_TIMEZONE) {
        CurDateStr = PoolPrint (
            L"%d-%02d-%02d %02d:%02d:%02d (GMT)",
            NowYear, NowMonth,
            NowDay, NowHour,
            NowMinute, NowSecond
        );
    }
    else {
        CurDateStr = PoolPrint (
            L"%d-%02d-%02d %02d:%02d:%02d (GMT%s%02d:%02d)",
            NowYear, NowMonth,
            NowDay, NowHour,
            NowMinute, NowSecond,
            (Now.TimeZone > 0 ? L"-" : L"+"),
            ((ABS(Now.TimeZone)) / 60),
            ((ABS(Now.TimeZone)) % 60)
        );
    }
    LOG_MSG("Timestamp:- '%s'", CurDateStr);
    MY_FREE_POOL(CurDateStr);
    #endif

    /* Log System Details */
    LogBasicInfo ();

    /* Read Configuration */
    SetConfigFilename (ImageHandle);

    /* Set Secure Boot Up */
    MokProtocol = SecureBootSetup();

    #if REFIT_DEBUG > 0
    Status = (MokProtocol) ? EFI_SUCCESS : EFI_NOT_STARTED;
    LOG_MSG("INFO: MOK Protocol ... %r", Status);
    LOG_MSG("\n\n");
    #endif

    // First scan volumes by calling ScanVolumes() to find "SelfVolume",
    //    SelfVolume is required by LoadDrivers() and ReadConfig();
    // A second call is needed later to enumerate volumes as well as
    //    to register new filesystem(s) accessed by drivers.
    ScanVolumes();

    /* Get/Set Config File ... Prefer RefindPlus Configuration File Naame */
    if (!FileExists (SelfDir, GlobalConfig.ConfigFilename)) {
        ConfigWarn = TRUE;

        #if REFIT_DEBUG > 0
        LOG_MSG("** WARN: Could not find RefindPlus configuration file:- 'config.conf'\n");
        LOG_MSG("         Trying rEFInd's configuration file instead:- 'refind.conf'\n"  );
        LOG_MSG("         Provide a 'config.conf' file to silence this warning\n"        );
        LOG_MSG("         You can rename 'refind.conf' as 'config.conf'\n"               );
        LOG_MSG("         NB: Will not contain all RefindPlus settings\n\n"              );
        #endif

        GlobalConfig.ConfigFilename = L"refind.conf";
    }

    /* Load config tokens */
    ReadConfig (GlobalConfig.ConfigFilename);

    /* Unlock partitions if required */
    #ifdef __MAKEWITH_TIANO
    // DA-TAG: Limit to TianoCore
    if (GlobalConfig.RansomDrives) {
        UninitRefitLib();
        OcUnblockUnmountedPartitions();
        ReinitRefitLib();
    }
    #endif

    /* Init MainMenu */
    InitMainMenu();

    /* Adjust Default Menu Selection */
    AdjustDefaultSelection();

    /* Align 'AllowTweakUEFI' */
    if (!AllowTweakUEFI) {
        // DA-TAG: Investigate This
        //         Items that may conflict with EFI version mismatch
        GlobalConfig.ProvideConsoleGOP   = FALSE;
        GlobalConfig.UseTextRenderer     = FALSE;
        GlobalConfig.PassUgaThrough      = FALSE;
        GlobalConfig.UseDirectGop        = FALSE;
        GlobalConfig.ProtectNVRAM        = FALSE;
        GlobalConfig.SilenceAPFS         = FALSE;
        GlobalConfig.SupplyUEFI          = FALSE;
        GlobalConfig.SupplyAPFS          = FALSE;
        GlobalConfig.ReloadGOP           = FALSE;
    }

    #if REFIT_DEBUG > 0
    LOG_MSG("P R O G R E S S   B O O T S T R A P");
    LOG_MSG("\n");

    #define TAG_ITEM_A(Item) OffsetNext,  Item
    #define TAG_ITEM_B(Item) OffsetNext, (Item) ? L"YES" : L"NO"
    #define TAG_ITEM_C(Item) OffsetNext, (Item) ? L"Active" : L"Inactive"

    LOG_MSG("INFO: RefitDBG:- '%d'",       REFIT_DEBUG                             );
    LOG_MSG("%s      LogLevel:- '%d'",     TAG_ITEM_A(GlobalConfig.LogLevel       ));
    LOG_MSG("%s      ScanDelay:- '%d'",    TAG_ITEM_A(GlobalConfig.ScanDelay      ));
    LOG_MSG("%s      ReloadGOP:- '%s'",    TAG_ITEM_B(GlobalConfig.ReloadGOP      ));
    LOG_MSG("%s      SyncAPFS:- '%s'",     TAG_ITEM_C(GlobalConfig.SyncAPFS       ));
    LOG_MSG("%s      TagsHelp:- '%s'",     TAG_ITEM_C(GlobalConfig.TagsHelp       ));
    LOG_MSG("%s      TextOnly:- '%s'",     TAG_ITEM_C(GlobalConfig.TextOnly       ));
    LOG_MSG("%s      RescanDXE:- '%s'",    TAG_ITEM_C(GlobalConfig.RescanDXE      ));
    LOG_MSG("%s      DirectGOP:- '%s'",    TAG_ITEM_C(GlobalConfig.UseDirectGop   ));
    LOG_MSG("%s      DirectBoot:- '%s'",   TAG_ITEM_C(GlobalConfig.DirectBoot     ));
    LOG_MSG("%s      ScanAllESP:- '%s'",   TAG_ITEM_C(GlobalConfig.ScanAllESP     ));

    LOG_MSG("%s      PanicFilter:- ",      OffsetNext);
    (!AppleFirmware                                                          )
        ? LOG_MSG("'Disabled'"                                               )
        : LOG_MSG("'%s'", GlobalConfig.PanicFilter ? L"Active" : L"Inactive" );
    LOG_MSG("%s      ProtectNVRAM:- ",     OffsetNext);
    (!AppleFirmware                                                          )
        ? LOG_MSG("'Disabled'"                                               )
        : LOG_MSG("'%s'", GlobalConfig.ProtectNVRAM ? L"Active" : L"Inactive");
    LOG_MSG(
        "%s      TextRenderer:- '%s'",
        OffsetNext,
        GlobalConfig.NormaliseCSR ? L"Active" : L"Inactive"
    );
    LOG_MSG(
        "%s      NormaliseCSR:- '%s'",
        OffsetNext,
        GlobalConfig.NormaliseCSR ? L"Active" : L"Inactive"
    );
    LOG_MSG("%s      RansomDrives:- ",  OffsetNext);
    (AppleFirmware                                                           )
        ? LOG_MSG("'Disabled'"                                               )
        : LOG_MSG("'%s'", GlobalConfig.RansomDrives ? L"Active" : L"Inactive");
    LOG_MSG("%s      SupplyAppleFB:- ",     OffsetNext);
    (!AppleFirmware                                                          )
        ? LOG_MSG("'Disabled'"                                               )
        : LOG_MSG("'%s'", GlobalConfig.SupplyAppleFB ? L"Active" : L"Inactive");
    LOG_MSG(
        "%s      TransientBoot:- '%s'",
        OffsetNext,
        GlobalConfig.TransientBoot ? L"Active" : L"Inactive"
    );
    LOG_MSG("\n\n");

    // DA-TAG: Prime Status for SupplyUEFI
    //         Here to accomodate GNU-EFI
    Status = EFI_NOT_STARTED;
    #endif

    #ifdef __MAKEWITH_TIANO
    // DA-TAG: Limit to TianoCore
    if (GlobalConfig.SupplyUEFI) {
        Status = AmendSysTable();
    }
    #endif

    #if REFIT_DEBUG > 0
    LOG_MSG("INFO: Supply Support:- 'UEFI  :  %r'", Status);

    // DA-TAG: Prime Status for SupplyNVME
    //         Here to accomodate GNU-EFI
    Status = EFI_NOT_STARTED;
    #endif

    #ifdef __MAKEWITH_TIANO
    // DA-TAG: Limit to TianoCore
    if (GlobalConfig.SupplyNVME) {
        Status = NvmExpressLoad (ImageHandle, SystemTable);
    }
    #endif

    #if REFIT_DEBUG > 0
    LOG_MSG("%s      Supply Support:- 'NVME  :  %r'", OffsetNext, Status);

    // DA-TAG: Prime Status for SupplyAPFS
    //         Here to accomodate GNU-EFI
    Status = EFI_NOT_STARTED;
    #endif

    #ifdef __MAKEWITH_TIANO
    // DA-TAG: Limit to TianoCore
    if (GlobalConfig.SupplyAPFS) {
        Status = RP_ApfsConnectDevices();
    }
    #endif

    #if REFIT_DEBUG > 0
    LOG_MSG("%s      Supply Support:- 'APFS  :  %r'", OffsetNext, Status);
    #endif

    // Load Drivers
    LoadDrivers();

    // Restore SystemTable if previously amended and not emulating UEFI 2.x
    if (SetSysTab) {
        if (!GlobalConfig.SupplyUEFI) {
            SetSysTab =  FALSE;
            gBS       = OrigBS;
            gRT       = OrigRT;
            gST       = OrigST;
            gBS->Hdr.CRC32 = 0;
            gRT->Hdr.CRC32 = 0;
            gST->Hdr.CRC32 = 0;
            REFIT_CALL_3_WRAPPER(
                gBS->CalculateCrc32, gBS,
                gBS->Hdr.HeaderSize, &gBS->Hdr.CRC32
            );
            REFIT_CALL_3_WRAPPER(
                gBS->CalculateCrc32, gRT,
                gRT->Hdr.HeaderSize, &gRT->Hdr.CRC32
            );
            REFIT_CALL_3_WRAPPER(
                gBS->CalculateCrc32, gST,
                gST->Hdr.HeaderSize, &gST->Hdr.CRC32
            );
        }

        #if REFIT_DEBUG > 0
        Status = (GlobalConfig.SupplyUEFI) ? EFI_NOT_STARTED : EFI_SUCCESS;
        MsgStr = PoolPrint (L"Restore System Table ... %r", Status);
        ALT_LOG(1, LOG_STAR_SEPARATOR, L"%s", MsgStr);
        LOG_MSG("%s      %s", OffsetNext, MsgStr);
        MY_FREE_POOL(MsgStr);
        #endif
    }

    // Second call to ScanVolumes() to enumerate volumes and
    //   register any new filesystem(s) accessed by drivers.
    ScanVolumes();

    if (GlobalConfig.SpoofOSXVersion &&
        GlobalConfig.SpoofOSXVersion[0] != L'\0'
    ) {
        Status = SetAppleOSInfo();

        #if REFIT_DEBUG > 0
        LOG_MSG("INFO: Spoof MacOS Version ... %r", Status);
        LOG_MSG("\n\n");
        #endif
    }

    /* Validate DirectBoot */
    if (GlobalConfig.DirectBoot) {
        // Override ScreensaverTime
        if (GlobalConfig.ScreensaverTime != 0) {
            GlobalConfig.ScreensaverTime = 300;
        }

        if (SecureBootFailure || WarnVersionEFI) {
            // Override DirectBoot if a warning needs to be shown
            //   for ALL build targets
            OverrideSB                     = TRUE;
            GlobalConfig.ContinueOnWarning = TRUE;
        }

        #if REFIT_DEBUG > 0
        if (WarnMissingQVInfo || ConfigWarn) {
            // Override DirectBoot if a warning needs to be shown
            //   for DBG and NPT build targets
            OverrideSB                     = TRUE;
            GlobalConfig.ContinueOnWarning = TRUE;
        }

        LOG_MSG("C O N F I R M   D I R E C T    B O O T");
        if (OverrideSB) {
            LOG_MSG("\n");
            LOG_MSG("Load Error or Warning Present");
        }
        #endif

        if (!OverrideSB) {
            CHAR16 KeyAsString[2];
            EFI_INPUT_KEY     key;

            Status = REFIT_CALL_2_WRAPPER(gST->ConIn->ReadKeyStroke, gST->ConIn, &key);
            if (!EFI_ERROR(Status)) {
                KeyAsString[0] = key.UnicodeChar;
                KeyAsString[1] = 0;

                if (key.ScanCode == SCAN_ESC || key.UnicodeChar == CHAR_BACKSPACE) {
                    OverrideSB = TRUE;
                    MainMenu->TimeoutSeconds = 0;
                }
            }

            #if REFIT_DEBUG > 0
            LOG_MSG("\n");
            LOG_MSG("Read Buffered Keystrokes ... %r", Status);
            #endif
        }

        // DA-TAG: Do not merge with block above
        if (OverrideSB) {
            BlockRescan = TRUE;
            GlobalConfig.Timeout = 0;
            GlobalConfig.DirectBoot = FALSE;

            #if REFIT_DEBUG > 0
            MsgStr = StrDuplicate (L"Overriding");
            #endif
        }
        else {
            #if REFIT_DEBUG > 0
            MsgStr = StrDuplicate (L"Maintaining");
            #endif

            GlobalConfig.TextOnly = TRUE;
        }

        #if REFIT_DEBUG > 0
        LOG_MSG("%s - %s DirectBoot", OffsetNext, MsgStr);
        LOG_MSG("\n\n");
        MY_FREE_POOL(MsgStr);
        #endif
    } // if GlobalConfig.DirectBoot

    #if REFIT_DEBUG > 0
    MsgStr = StrDuplicate (L"I N I T I A L I S E   G R A P H I C S");
    ALT_LOG(1, LOG_LINE_SEPARATOR, L"%s", MsgStr);
    LOG_MSG("%s", MsgStr);
    LOG_MSG("\n");
    MY_FREE_POOL(MsgStr);
    #endif

    InitScreen();
    WarnIfLegacyProblems();

    // Disable the EFI watchdog timer
    REFIT_CALL_4_WRAPPER(
        gBS->SetWatchdogTimer, 0x0000,
        0x0000, 0x0000, NULL
    );

    // Further bootstrap (now with config available)
    SetupScreen();

    #if REFIT_DEBUG > 0
    /* Disable Forced Native Logging */
    MY_NATIVELOGGER_OFF;
    #endif

    // Show Secure Boot Failure Notice and Shut Down
    if (SecureBootFailure) {
        #if REFIT_DEBUG > 0
        MsgStr = StrDuplicate (L"Secure Boot Failure");
        ALT_LOG(1, LOG_LINE_SEPARATOR, L"Display %s Warning", MsgStr);
        LOG_MSG("INFO: User Warning:- '%s ... Forcing Shutdown'", MsgStr);
        LOG_MSG("\n\n");
        MY_FREE_POOL(MsgStr);
        #endif

        #if REFIT_DEBUG > 0
        MY_MUTELOGGER_SET;
        #endif
        SwitchToText (FALSE);
        REFIT_CALL_2_WRAPPER(gST->ConOut->SetAttribute, gST->ConOut, ATTR_ERROR);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"              Failure Setting Secure Boot Up              ", NEXTLINE);
        PrintUglyText (L"              Forcing Shutdown in 9 Seconds!              ", NEXTLINE);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"                                                          ", NEXTLINE);
        REFIT_CALL_2_WRAPPER(gST->ConOut->SetAttribute, gST->ConOut, ATTR_BASIC);
        PauseSeconds (9);
        #if REFIT_DEBUG > 0
        MY_MUTELOGGER_OFF;
        #endif

        REFIT_CALL_4_WRAPPER(
            gRT->ResetSystem, EfiResetShutdown,
            EFI_SUCCESS, 0, NULL
        );
    }

    // Apply Scan Delay if set
    if (GlobalConfig.ScanDelay > 0) {
        MsgStr = StrDuplicate (L"Paused for Scan Delay");

        UINTN Trigger = 3;
        if (GlobalConfig.ScanDelay > Trigger) {
            CHAR16 *PartMsg = PoolPrint (L"%s ... Please Wait", MsgStr);
            egDisplayMessage (
                PartMsg, &BGColor, CENTER,
                0, NULL
            );

            MY_FREE_POOL(PartMsg);
        }

        #if REFIT_DEBUG > 0
        ALT_LOG(1, LOG_LINE_THIN_SEP, L"Scan Delay");
        ALT_LOG(1, LOG_LINE_NORMAL, L"%s", MsgStr);
        LOG_MSG("%s:", MsgStr);
        LOG_MSG("\n");
        #endif

        MY_FREE_POOL(MsgStr);

        for (i = 0; i < GlobalConfig.ScanDelay; ++i) {
            #if REFIT_DEBUG > 0
            ALT_LOG(1, LOG_THREE_STAR_MID, L"Loading Paused for 1 Second");
            #endif

            // Wait 1 second
            REFIT_CALL_1_WRAPPER(gBS->Stall, 250000);
            REFIT_CALL_1_WRAPPER(gBS->Stall, 250000);
            REFIT_CALL_1_WRAPPER(gBS->Stall, 250000);
            REFIT_CALL_1_WRAPPER(gBS->Stall, 250000);
        }

        #if REFIT_DEBUG > 0
        MsgStr = PoolPrint (L"Resuming After a %d Second Pause", i);
        ALT_LOG(1, LOG_LINE_NORMAL, L"%s", MsgStr);
        LOG_MSG("  - %s", MsgStr);
        LOG_MSG("\n");
        MY_FREE_POOL(MsgStr);
        #endif

        if (GlobalConfig.ScanDelay > Trigger) {
            BltClearScreen (TRUE);
        }
    }

    // Continue Bootstrap
    SetVolumeIcons();
    ScanForBootloaders();
    ScanForTools();

    if (GlobalConfig.ShutdownAfterTimeout) {
        MainMenu->TimeoutText = StrDuplicate (L"Shutdown");
    }

    // Prime ForceContinue
    BOOLEAN ForceContinue = FALSE;

    // Show EFI Version Mismatch Warning
    if (WarnVersionEFI) {
        SwitchToText (FALSE);

        #if REFIT_DEBUG > 0
        LOG_MSG("D I S P L A Y   U S E R   W A R N I N G");
        MsgStr = StrDuplicate (L"Inconsistent EFI Versions");
        ALT_LOG(1, LOG_LINE_SEPARATOR, L"Display %s Warning", MsgStr);
        LOG_MSG("INFO: User Warning:- '%s'", MsgStr);
        MY_FREE_POOL(MsgStr);
        #endif

        ForceContinue = (GlobalConfig.ContinueOnWarning) ? TRUE : FALSE;
        GlobalConfig.ContinueOnWarning = TRUE;
        #if REFIT_DEBUG > 0
        MY_MUTELOGGER_SET;
        #endif
        REFIT_CALL_2_WRAPPER(gST->ConOut->SetAttribute, gST->ConOut, ATTR_ERROR);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"            Inconsistent EFI Versions Detected            ", NEXTLINE);
        PrintUglyText (L"             Program Behaviour is Undefined!!             ", NEXTLINE);
        PrintUglyText (L"                                                          ", NEXTLINE);

        REFIT_CALL_2_WRAPPER(gST->ConOut->SetAttribute, gST->ConOut, ATTR_BASIC);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"        Disabled All Features Involving UEFI Changes      ", NEXTLINE);
#if REFIT_DEBUG > 0
        PrintUglyText (L"                See Debug Log for Details                 ", NEXTLINE);
#else
        PrintUglyText (L"              Run DBG Build File for Details              ", NEXTLINE);
#endif
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PauseForKey();
        #if REFIT_DEBUG > 0
        MY_MUTELOGGER_SET;
        #endif
        GlobalConfig.ContinueOnWarning = ForceContinue;
        ForceContinue = FALSE;

        #if REFIT_DEBUG > 0
        MsgStr = StrDuplicate (L"Warning Acknowledged or Timed Out");
        ALT_LOG(1, LOG_LINE_NORMAL, L"%s", MsgStr);
        ALT_LOG(1, LOG_BLANK_LINE_SEP, L"X");
        LOG_MSG("%s      %s ... ", OffsetNext, MsgStr);
        MY_FREE_POOL(MsgStr);
        #endif
        if (AllowGraphicsMode) {
            #if REFIT_DEBUG > 0
            LOG_MSG("Restore Graphics Mode");
            LOG_MSG("\n\n");
            #endif

            SwitchToGraphicsAndClear (TRUE);
        }
        else {
            #if REFIT_DEBUG > 0
            LOG_MSG("Proceeding");
            LOG_MSG("\n\n");
            #endif
        }
        // Wait 1 second
        REFIT_CALL_1_WRAPPER(gBS->Stall, 250000);
        REFIT_CALL_1_WRAPPER(gBS->Stall, 250000);
        REFIT_CALL_1_WRAPPER(gBS->Stall, 250000);
        REFIT_CALL_1_WRAPPER(gBS->Stall, 250000);
    } // if WarnVersionEFI

    // Show Inconsistent UEFI 2.x Implementation Warning
    #if REFIT_DEBUG > 0
    if (WarnMissingQVInfo) {
        SwitchToText (FALSE);

        LOG_MSG("D I S P L A Y   U S E R   N O T I C E");
        MsgStr = StrDuplicate (L"Inconsistent UEFI 2.x Implementation");
        ALT_LOG(1, LOG_LINE_SEPARATOR, L"Display %s Warning", MsgStr);
        LOG_MSG("INFO: User Warning:- '%s'", MsgStr);
        MY_FREE_POOL(MsgStr);

        ForceContinue = (GlobalConfig.ContinueOnWarning) ? TRUE : FALSE;
        GlobalConfig.ContinueOnWarning = TRUE;
        MY_MUTELOGGER_SET;
        REFIT_CALL_2_WRAPPER(gST->ConOut->SetAttribute, gST->ConOut, ATTR_ERROR);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"           Inconsistent UEFI 2.x Implementation           ", NEXTLINE);
        PrintUglyText (L"                                                          ", NEXTLINE);

        REFIT_CALL_2_WRAPPER(gST->ConOut->SetAttribute, gST->ConOut, ATTR_BASIC);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"             Program Behaviour is Undefined!!             ", NEXTLINE);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PauseForKey();
        MY_MUTELOGGER_OFF;
        GlobalConfig.ContinueOnWarning = ForceContinue;
        ForceContinue = FALSE;

        MsgStr = StrDuplicate (L"Warning Acknowledged or Timed Out");
        ALT_LOG(1, LOG_LINE_NORMAL, L"%s", MsgStr);
        ALT_LOG(1, LOG_BLANK_LINE_SEP, L"X");
        LOG_MSG("%s      %s ...", OffsetNext, MsgStr);
        MY_FREE_POOL(MsgStr);

        if (AllowGraphicsMode) {
            LOG_MSG("Restore Graphics Mode");

            SwitchToGraphicsAndClear (TRUE);
        }
        else {
            LOG_MSG("Proceeding");

            BltClearScreen (FALSE);
        }

        LOG_MSG("\n\n");

        // Wait 1 second
        REFIT_CALL_1_WRAPPER(gBS->Stall, 250000);
        REFIT_CALL_1_WRAPPER(gBS->Stall, 250000);
        REFIT_CALL_1_WRAPPER(gBS->Stall, 250000);
        REFIT_CALL_1_WRAPPER(gBS->Stall, 250000);
    } // if WarnMissingQVInfo
    #endif

    // Show Config Mismatch Warning
    // DA-TAG: Separate REFIT_DEBUG sections for flexibility
    #if REFIT_DEBUG > 0
    if (ConfigWarn) {
        SwitchToText (FALSE);

        LOG_MSG("D I S P L A Y   U S E R   N O T I C E");
        MsgStr = StrDuplicate (L"Missing Config File");
        ALT_LOG(1, LOG_LINE_SEPARATOR, L"Display %s Warning", MsgStr);
        LOG_MSG("INFO: User Warning:- '%s'", MsgStr);
        MY_FREE_POOL(MsgStr);

        ForceContinue = (GlobalConfig.ContinueOnWarning) ? TRUE : FALSE;
        GlobalConfig.ContinueOnWarning = TRUE;
        MY_MUTELOGGER_SET;
        REFIT_CALL_2_WRAPPER(gST->ConOut->SetAttribute, gST->ConOut, ATTR_ERROR);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"      Could not find a RefindPlus 'config.conf' file      ", NEXTLINE);
        PrintUglyText (L"    Trying to load a rEFInd 'refind.conf' file instead    ", NEXTLINE);
        PrintUglyText (L"                                                          ", NEXTLINE);

        REFIT_CALL_2_WRAPPER(gST->ConOut->SetAttribute, gST->ConOut, ATTR_BASIC);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"    Provide a config.conf file to silence this warning    ", NEXTLINE);
        PrintUglyText (L"     You can rename a refind.conf file as config.conf     ", NEXTLINE);
        PrintUglyText (L" * Will not contain all RefindPlus configuration tokens * ", NEXTLINE);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PrintUglyText (L"                                                          ", NEXTLINE);
        PauseForKey();
        MY_MUTELOGGER_OFF;
        GlobalConfig.ContinueOnWarning = ForceContinue;
        ForceContinue = FALSE;

        MsgStr = StrDuplicate (L"Warning Acknowledged or Timed Out");
        ALT_LOG(1, LOG_LINE_NORMAL, L"%s", MsgStr);
        ALT_LOG(1, LOG_BLANK_LINE_SEP, L"X");
        LOG_MSG("%s      %s ...", OffsetNext, MsgStr);
        MY_FREE_POOL(MsgStr);

        if (AllowGraphicsMode) {
            LOG_MSG("Restore Graphics Mode");
            LOG_MSG("\n\n");

            SwitchToGraphicsAndClear (TRUE);
        }
        else {
            LOG_MSG("Proceeding");
            LOG_MSG("\n\n");
        }

        // Wait 0.25 second
        REFIT_CALL_1_WRAPPER(gBS->Stall, 250000);
    } // if ConfigWarn
    #endif

    #if REFIT_DEBUG > 0
    if (GlobalConfig.LogLevel > 1) {
        /* Enable Forced Native Logging */
        MY_NATIVELOGGER_SET;
    }
    #endif

    pdInitialize();

    #if REFIT_DEBUG > 0
    /* Disable Forced Native Logging */
    MY_NATIVELOGGER_OFF;
    #endif

    #if REFIT_DEBUG > 0
    MsgStr = StrDuplicate (L"R U N   M A I N   L O O P");
    ALT_LOG(1, LOG_LINE_SEPARATOR, L"%s", MsgStr);
    LOG_MSG("%s", MsgStr);
    LOG_MSG("\n");
    MY_FREE_POOL(MsgStr);
    #endif

    if (GlobalConfig.DefaultSelection) {
        SelectionName = StrDuplicate (GlobalConfig.DefaultSelection);
    }

    CHAR16   *TypeStr      = NULL;
    CHAR16   *FilePath     = NULL;
    BOOLEAN   FoundTool    = FALSE;
    BOOLEAN   RunOurTool   = FALSE;

    while (MainLoopRunning) {
        // Reset Misc
        IsBoot         = FALSE;
        FoundTool      = FALSE;
        RunOurTool     = FALSE;
        MY_FREE_POOL(FilePath);

        MenuExit = RunMainMenu (MainMenu, &SelectionName, &ChosenEntry);

        // The ESC key triggers a rescan ... if allowed
        if (MenuExit == MENU_EXIT_ESCAPE) {
            #if REFIT_DEBUG > 0
            LOG_MSG("Received Input:");
            LOG_MSG("%s  - Escape Key Pressed ... Rescan All", OffsetNext);
            LOG_MSG("\n\n");
            #endif

            OverrideSB = TRUE;
            RescanAll (TRUE);
            continue;
        }
        BlockRescan = FALSE;

        // Ignore MenuExit if FlushFailedTag is set and not previously reset
        if (FlushFailedTag && !FlushFailReset) {
            #if REFIT_DEBUG > 0
            MsgStr = StrDuplicate (L"FlushFailedTag is Set ... Ignore MenuExit");
            ALT_LOG(1, LOG_STAR_SEPARATOR, L"%s", MsgStr);
            LOG_MSG("INFO: %s", MsgStr);
            LOG_MSG("\n\n");
            MY_FREE_POOL(MsgStr);
            #endif

            FlushFailedTag = FALSE;
            FlushFailReset = TRUE;
            continue;
        }

        if (MenuExit == MENU_EXIT_TIMEOUT &&
            GlobalConfig.ShutdownAfterTimeout
        ) {
            ChosenEntry->Tag = TAG_SHUTDOWN;
        }

        // Reset NVRAM Protection
        SetProtectNvram (SystemTable, FALSE);

        switch (ChosenEntry->Tag) {
            case TAG_INFO_NVRAMCLEAN:    // Clean NVRAM
                TypeStr = L"Clean/Reset NVRAM";

                #if REFIT_DEBUG > 0
                ALT_LOG(1, LOG_LINE_THIN_SEP, L"Creating '%s Info' Screen", TypeStr);

                LOG_MSG("Received Input:");
                LOG_MSG("%s  - %s", OffsetNext, TypeStr);
                LOG_MSG("\n\n");
                #endif

                RunOurTool = ShowCleanNvramInfo (TypeStr);
                if (!RunOurTool) {
                    #if REFIT_DEBUG > 0
                    LOG_MSG("%s    ** Not Running Tool to %s", OffsetNext, TypeStr);
                    LOG_MSG("\n\n");
                    #endif

                    // Early Exit
                    break;
                }

                k = 0;
                while (!FoundTool && (FilePath = FindCommaDelimited (NVRAMCLEAN_FILES, k++)) != NULL) {
                    for (i = 0; i < VolumesCount; i++) {
                        if (Volumes[i]->RootDir != NULL &&
                            FileExists (Volumes[i]->RootDir, FilePath)
                        ) {
                            ourLoaderEntry = AllocateZeroPool (sizeof (LOADER_ENTRY));
                            ourLoaderEntry->me.Title        = StrDuplicate (TypeStr);
                            ourLoaderEntry->me.Tag          = TAG_LOAD_NVRAMCLEAN;
                            ourLoaderEntry->Volume          = CopyVolume (Volumes[i]);
                            ourLoaderEntry->LoaderPath      = StrDuplicate (FilePath);
                            ourLoaderEntry->UseGraphicsMode = FALSE;

                            FoundTool = TRUE;

                            // Break out of 'for' loop
                            break;
                        }
                    } // for

                    if (!FoundTool) {
                        MY_FREE_POOL(FilePath);
                    }
                } // while

                if (!FoundTool) {
                    #if REFIT_DEBUG > 0
                    MsgStr = PoolPrint (L"Could Not Find Tool:- '%s'", TypeStr);
                    ALT_LOG(1, LOG_LINE_NORMAL, L"%s", MsgStr);
                    LOG_MSG("\n\n");
                    LOG_MSG("** WARN : %s ... Return to Main Menu", MsgStr);
                    LOG_MSG("\n\n");
                    MY_FREE_POOL(MsgStr);
                    #endif

                    MY_FREE_POOL(FilePath);

                    // Early Exit
                    break;
                }

                #if REFIT_DEBUG > 0
                LOG_MSG("%s  - Load Tool to %s", OffsetNext, TypeStr);
                LOG_MSG("\n\n");
                #endif

                MY_FREE_POOL(FilePath);

                // No end dash line ... Handled in 'Reboot' below
                StartTool (ourLoaderEntry);

                #if REFIT_DEBUG > 0
                LOG_MSG("INFO: Cleaned NVRAM");
                LOG_MSG("\n\n");
                #endif

            // No Break
            case TAG_REBOOT:    // Reboot
                TypeStr = L"Running System Reset";

                // Terminate Screen
                TerminateScreen();

                #if REFIT_DEBUG > 0
                MsgStr = StrDuplicate (L"R E B O O T   S Y S T E M");
                ALT_LOG(1, LOG_BLANK_LINE_SEP, L"X");
                ALT_LOG(1, LOG_LINE_SEPARATOR, L"%s", MsgStr);
                ALT_LOG(1, LOG_BLANK_LINE_SEP, L"X");
                LOG_MSG("%s", MsgStr);
                LOG_MSG("\n");
                MY_FREE_POOL(MsgStr);

                ALT_LOG(1, LOG_LINE_NORMAL, L"%s", TypeStr);
                LOG_MSG("%s", TypeStr);
                LOG_MSG("\n <<----- * ----->>\n\n");
                #endif

                #if REFIT_DEBUG > 0
                MY_MUTELOGGER_SET;
                #endif
                egDisplayMessage (
                    TypeStr, &BGColor, CENTER,
                    3, L"PauseSeconds"
                );
                #if REFIT_DEBUG > 0
                MY_MUTELOGGER_OFF;
                #endif

                REFIT_CALL_4_WRAPPER(
                    gRT->ResetSystem, EfiResetCold,
                    EFI_SUCCESS, 0, NULL
                );

                // Just in case we get this far
                MainLoopRunning = FALSE;

                #if REFIT_DEBUG > 0
                MsgStr = StrDuplicate (L"System Reset Failed");
                ALT_LOG(1, LOG_LINE_NORMAL, L"%s!!", MsgStr);
                LOG_MSG("WARN: %s", MsgStr);
                MY_FREE_POOL(MsgStr);
                #endif

            break;
            case TAG_SHUTDOWN: // Shut Down
                TypeStr = L"Running System Shutdown";

                // Terminate Screen
                TerminateScreen();

                #if REFIT_DEBUG > 0
                MsgStr = StrDuplicate (L"S H U T   S Y S T E M   D O W N");
                ALT_LOG(1, LOG_BLANK_LINE_SEP, L"X");
                ALT_LOG(1, LOG_LINE_SEPARATOR, L"%s", MsgStr);
                ALT_LOG(1, LOG_BLANK_LINE_SEP, L"X");
                LOG_MSG("%s", MsgStr);
                LOG_MSG("\n");
                MY_FREE_POOL(MsgStr);

                ALT_LOG(1, LOG_LINE_NORMAL, L"%s", TypeStr);
                LOG_MSG("%s", TypeStr);
                LOG_MSG("\n <<----- * ----->>\n\n");
                #endif

                #if REFIT_DEBUG > 0
                MY_MUTELOGGER_SET;
                #endif
                egDisplayMessage (
                    TypeStr, &BGColor, CENTER,
                    3, L"PauseSeconds"
                );
                #if REFIT_DEBUG > 0
                MY_MUTELOGGER_OFF;
                #endif

                REFIT_CALL_4_WRAPPER(
                    gRT->ResetSystem, EfiResetShutdown,
                    EFI_SUCCESS, 0, NULL
                );

                // Just in case we get this far
                MainLoopRunning = FALSE;

                #if REFIT_DEBUG > 0
                MsgStr = StrDuplicate (L"System Shutdown Failed");
                ALT_LOG(1, LOG_LINE_NORMAL, L"%s!!", MsgStr);
                LOG_MSG("WARN: %s", MsgStr);
                MY_FREE_POOL(MsgStr);
                #endif

            break;
            case TAG_ABOUT:    // About RefindPlus
                #if REFIT_DEBUG > 0
                LOG_MSG("Received Input:");
                LOG_MSG("%s  - Show 'About RefindPlus' Page", OffsetNext);
                LOG_MSG("\n\n");
                #endif

                // No end dash line ... Expected to return
                AboutRefindPlus();

                #if REFIT_DEBUG > 0
                LOG_MSG("Received Input:");
                LOG_MSG("%s  - Exit 'About RefindPlus' Page", OffsetNext);
                LOG_MSG("\n\n");
                #endif

            break;
            case TAG_LOADER:   // Boot OS via *.efi File
                ourLoaderEntry = (LOADER_ENTRY *) ChosenEntry;

                // Fix undetected MacOS
                if (!FoundSubStr (ourLoaderEntry->Title, L"MacOS") &&
                    FoundSubStr (ourLoaderEntry->LoaderPath, L"System\\Library\\CoreServices")
                ) {
                    ourLoaderEntry->Title = (FoundSubStr (ourLoaderEntry->Volume->VolName, L"PreBoot"))
                        ? L"MacOS" : L"RefindPlus";
                }

                // Fix undetected Windows
                if (!FoundSubStr (ourLoaderEntry->Title, L"Windows") &&
                    FoundSubStr (ourLoaderEntry->LoaderPath, L"EFI\\Microsoft\\Boot")
                ) {
                    ourLoaderEntry->Title = L"Windows (UEFI)";
                }

                if (FoundSubStr (ourLoaderEntry->Title, L"OpenCore") ||
                    FoundSubStr (ourLoaderEntry->LoaderPath, L"\\OC\\") ||
                    FoundSubStr (ourLoaderEntry->LoaderPath, L"\\OpenCore")
                ) {
                    // Set CSR if required
                    AlignCSR();

                    if (!ourLoaderEntry->UseGraphicsMode) {
                        ourLoaderEntry->UseGraphicsMode = GlobalConfig.GraphicsFor & GRAPHICS_FOR_OPENCORE;
                    }

                    #if REFIT_DEBUG > 0
                    // DA-TAG: Using separate instances of 'Received Input:'
                    LOG_MSG("Received Input:");
                    MsgStr = StrDuplicate (L"Load OpenCore Instance");
                    ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                    LOG_MSG(
                        "%s  - %s:- '%s'",
                        OffsetNext,
                        MsgStr,
                        ourLoaderEntry->LoaderPath
                    );
                    MY_FREE_POOL(MsgStr);
                    #endif

                    // Filter out the 'APPLE_INTERNAL' CSR bit if required
                    FilterCSR();
                }
                else if (FoundSubStr (ourLoaderEntry->Title, L"Clover") ||
                    FoundSubStr (ourLoaderEntry->LoaderPath, L"\\Clover")
                ) {
                    // Set CSR if required
                    AlignCSR();

                    if (!ourLoaderEntry->UseGraphicsMode) {
                        ourLoaderEntry->UseGraphicsMode = GlobalConfig.GraphicsFor & GRAPHICS_FOR_CLOVER;
                    }

                    #if REFIT_DEBUG > 0
                    // DA-TAG: Using separate instances of 'Received Input:'
                    LOG_MSG("Received Input:");
                    MsgStr = StrDuplicate (L"Load Clover Instance");
                    ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                    LOG_MSG(
                        "%s  - %s:- '%s'",
                        OffsetNext,
                        MsgStr,
                        ourLoaderEntry->LoaderPath
                    );
                    MY_FREE_POOL(MsgStr);
                    #endif

                    // Filter out the 'APPLE_INTERNAL' CSR bit if required
                    FilterCSR();
                }
                else if (FoundSubStr (ourLoaderEntry->Title, L"MacOS")
                    || ourLoaderEntry->OSType == 'M'
                ) {
                    // Set CSR if required
                    AlignCSR();

                    #if REFIT_DEBUG > 0
                    // DA-TAG: Using separate instances of 'Received Input:'
                    LOG_MSG("Received Input:");
                    MsgStr = StrDuplicate (L"Load MacOS Instance");
                    ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                    LOG_MSG("%s  - %s", OffsetNext, MsgStr);

                    CHAR16 *DisplayName = NULL;
                    if (!ourLoaderEntry->Volume->VolName) {
                        LOG_MSG(":- '%s'", ourLoaderEntry->LoaderPath);
                    }
                    else {
                        if (GlobalConfig.SyncAPFS) {
                            APPLE_APFS_VOLUME_ROLE VolumeRole = 0;

                            // DA-TAG: Limit to TianoCore
                            #ifndef __MAKEWITH_TIANO
                            Status = EFI_NOT_STARTED;
                            #else
                            Status = RP_GetApfsVolumeInfo (
                                ourLoaderEntry->Volume->DeviceHandle,
                                NULL, NULL,
                                &VolumeRole
                            );
                            #endif

                            if (!EFI_ERROR(Status)) {
                                if (VolumeRole == APPLE_APFS_VOLUME_ROLE_PREBOOT) {
                                    DisplayName = GetVolumeGroupName (
                                        ourLoaderEntry->LoaderPath,
                                        ourLoaderEntry->Volume
                                    );
                                }
                            }
                        }

                        LOG_MSG(
                            " from '%s'",
                            (DisplayName)
                                ? DisplayName
                                : ourLoaderEntry->Volume->VolName
                        );
                    }
                    MY_FREE_POOL(MsgStr);
                    MY_FREE_POOL(DisplayName);
                    #endif

                    // Set Mac boot args if configured to
                    // Also disables AMFI if this is configured
                    // Also disables MacOS compatibility check if this is configured
                    if (GlobalConfig.SetBootArgs && GlobalConfig.SetBootArgs[0] != L'\0') {
                        SetBootArgs();
                    }

                    // Disable AMFI if configured to and not previously disabled
                    NoCheckAMFI();

                    // Disable MacOS compatibility check if configured to and not previously disabled
                    NoCheckCompat();

                    // Filter out the 'APPLE_INTERNAL' CSR bit if required
                    FilterCSR();

                    // Enable TRIM on non-Apple SSDs if configured to
                    TrimCoerce();

                    // Re-Map OpenProtocol
                    ReMapOpenProtocol();

                    if (GlobalConfig.PanicFilter) {
                        // Protect Mac NVRAM from KP Dumps
                        SetProtectNvram (SystemTable, TRUE);
                    }
                }
                else if (FoundSubStr (ourLoaderEntry->Title, L"Windows")
                    || ourLoaderEntry->OSType == 'W'
                ) {
                    // Protect Mac NVRAM from UEFI Windows
                    SetProtectNvram (SystemTable, TRUE);

                    #if REFIT_DEBUG > 0
                    // DA-TAG: Using separate instances of 'Received Input:'
                    LOG_MSG("Received Input:");
                    MsgStr = StrDuplicate (L"Load Windows (UEFI) Instance");
                    ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                    LOG_MSG(
                        "%s  - %s%s '%s'",
                        OffsetNext,
                        MsgStr,
                        (ourLoaderEntry->Volume->VolName)
                            ? L" from" : L":-",
                        (ourLoaderEntry->Volume->VolName)
                            ? ourLoaderEntry->Volume->VolName : ourLoaderEntry->LoaderPath
                    );

                    MY_FREE_POOL(MsgStr);
                    #endif
                }
                else if (FoundSubStr (ourLoaderEntry->Title, L"Grub")
                    || ourLoaderEntry->OSType == 'G'
                ) {
                    #if REFIT_DEBUG > 0
                    MsgStr = StrDuplicate (L"Load Linux Instance via Grub Loader");
                    ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                    // DA-TAG: Using separate instances of 'Received Input:'
                    LOG_MSG("Received Input:");
                    LOG_MSG(
                        "%s  - %s:- '%s'",
                        OffsetNext, MsgStr,
                        ourLoaderEntry->LoaderPath
                    );

                    MY_FREE_POOL(MsgStr);
                    #endif
                }
                else if (FoundSubStr (ourLoaderEntry->LoaderPath, L"vmlinuz")) {
                    #if REFIT_DEBUG > 0
                    MsgStr = StrDuplicate (L"Load Linux Instance via VMLinuz Loader");
                    ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                    // DA-TAG: Using separate instances of 'Received Input:'
                    LOG_MSG("Received Input:");
                    LOG_MSG(
                        "%s  - %s:- '%s'",
                        OffsetNext, MsgStr,
                        ourLoaderEntry->LoaderPath
                    );

                    MY_FREE_POOL(MsgStr);
                    #endif
                }
                else if (FoundSubStr (ourLoaderEntry->LoaderPath, L"bzImage")) {
                    #if REFIT_DEBUG > 0
                    MsgStr = StrDuplicate (L"Load Linux Instance via BZImage Loader");
                    ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                    // DA-TAG: Using separate instances of 'Received Input:'
                    LOG_MSG("Received Input:");
                    LOG_MSG(
                        "%s  - %s:- '%s'",
                        OffsetNext, MsgStr,
                        ourLoaderEntry->LoaderPath
                    );

                    MY_FREE_POOL(MsgStr);
                    #endif
                }
                else if (FoundSubStr (ourLoaderEntry->LoaderPath, L"kernel")) {
                    #if REFIT_DEBUG > 0
                    MsgStr = StrDuplicate (L"Load Linux Instance via Kernel Loader");
                    ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                    // DA-TAG: Using separate instances of 'Received Input:'
                    LOG_MSG("Received Input:");
                    LOG_MSG(
                        "%s  - %s:- '%s'",
                        OffsetNext, MsgStr,
                        ourLoaderEntry->LoaderPath
                    );

                    MY_FREE_POOL(MsgStr);
                    #endif
                }
                else if (FoundSubStr (ourLoaderEntry->Title, L"Linux")
                    || ourLoaderEntry->OSType == 'L'
                ) {
                    #if REFIT_DEBUG > 0
                    // DA-TAG: Using separate instances of 'Received Input:'
                    LOG_MSG("Received Input:");
                    MsgStr = StrDuplicate (L"Load Linux Instance");
                    ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                    LOG_MSG("%s  - %s", OffsetNext, MsgStr);
                    (ourLoaderEntry->Volume->VolName)
                        ? LOG_MSG(" from '%s'", ourLoaderEntry->Volume->VolName)
                        : LOG_MSG(":- '%s'", ourLoaderEntry->LoaderPath);

                    MY_FREE_POOL(MsgStr);
                    #endif
                }
                else if (ourLoaderEntry->OSType == 'R') {
                    if (!ourLoaderEntry->UseGraphicsMode && AllowGraphicsMode) {
                        ourLoaderEntry->UseGraphicsMode = TRUE;
                    }

                    #if REFIT_DEBUG > 0
                    MsgStr = StrDuplicate (L"Run rEFIt Variant");
                    ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                    // DA-TAG: Using separate instances of 'Received Input:'
                    LOG_MSG("Received Input:");
                    LOG_MSG(
                        "%s  - %s:- '%s'",
                        OffsetNext, MsgStr,
                        ourLoaderEntry->LoaderPath
                    );

                    MY_FREE_POOL(MsgStr);
                    #endif
                }
                else {
                    // Some UEFI Windows installers/updaters may not be in the standard path
                    // Activate ProtectNVRAM on any unidentified loaders
                    SetProtectNvram (SystemTable, TRUE);

                    #if REFIT_DEBUG > 0
                    MsgStr = StrDuplicate (L"Run UEFI File");
                    ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                    // DA-TAG: Using separate instances of 'Received Input:'
                    LOG_MSG("Received Input:");
                    LOG_MSG(
                        "%s  - %s:- '%s'",
                        OffsetNext, MsgStr,
                        ourLoaderEntry->LoaderPath
                    );

                    MY_FREE_POOL(MsgStr);
                    #endif
                }

                // No end dash line ... Added in 'IsValidLoader'
                StartLoader (ourLoaderEntry, SelectionName);

            break;
            case TAG_LEGACY:   // Boot legacy OS
                ourLegacyEntry = (LEGACY_ENTRY *) ChosenEntry;

                #if REFIT_DEBUG > 0
                LOG_MSG("Received Input:");
                #endif

                if (MyStrStr (ourLegacyEntry->Volume->OSName, L"Windows")) {
                    #if REFIT_DEBUG > 0
                    MsgStr = PoolPrint (
                        L"Boot %s from '%s'",
                        ourLegacyEntry->Volume->OSName,
                        ourLegacyEntry->Volume->VolName
                    );
                    ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                    LOG_MSG("%s  - %s", OffsetNext, MsgStr);
                    MY_FREE_POOL(MsgStr);
                    #endif
                }
                else {
                    #if REFIT_DEBUG > 0
                    MsgStr = StrDuplicate (L"Load 'Mac-Style' Legacy (BIOS) OS");
                    ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                    LOG_MSG("%s  - %s:- '%s'", OffsetNext, MsgStr, ourLegacyEntry->Volume->OSName);
                    MY_FREE_POOL(MsgStr);
                    #endif
                }

                #if REFIT_DEBUG > 0
                LOG_MSG("\n <<----- * ----->>\n\n");
                #endif

                StartLegacy (ourLegacyEntry, SelectionName);

            break;
            case TAG_LEGACY_UEFI: // Boot a legacy OS on a non-Mac
                ourLegacyEntry = (LEGACY_ENTRY *) ChosenEntry;

                #if REFIT_DEBUG > 0
                MsgStr = StrDuplicate (L"Load 'UEFI-Style' Legacy (BIOS) OS");
                ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                LOG_MSG("Received Input:");
                LOG_MSG(
                    "%s  - %s:- '%s'",
                    OffsetNext, MsgStr,
                    ourLegacyEntry->Volume ? ourLegacyEntry->Volume->OSName : L"NULL Volume"
                );
                MY_FREE_POOL(MsgStr);
                LOG_MSG("\n <<----- * ----->>\n\n");
                #endif

                StartLegacyUEFI (ourLegacyEntry, SelectionName);

            break;
            case TAG_TOOL:     // Start a UEFI tool
                ourLoaderEntry = (LOADER_ENTRY *) ChosenEntry;

                #if REFIT_DEBUG > 0
                MsgStr = StrDuplicate (L"Start UEFI Tool");
                ALT_LOG(1, LOG_LINE_THIN_SEP, L"%s", MsgStr);
                LOG_MSG("Received Input:");
                LOG_MSG("%s  - %s:- '%s'", OffsetNext, MsgStr, ourLoaderEntry->LoaderPath);
                MY_FREE_POOL(MsgStr);
                #endif

                // No end dash line ... Expected to return
                StartTool (ourLoaderEntry);

            break;
            case TAG_FIRMWARE_LOADER:  // Reboot to a loader defined in the NVRAM
                ourLoaderEntry = (LOADER_ENTRY *) ChosenEntry;

                #if REFIT_DEBUG > 0
                LOG_MSG("Received Input:");
                LOG_MSG("%s  - Reboot into Firmware Defined Loader", OffsetNext);
                #endif

                // No end dash line ... Added in 'RebootIntoLoader'
                RebootIntoLoader (ourLoaderEntry);

            break;
            case TAG_HIDDEN:  // Manage hidden tag entries

                #if REFIT_DEBUG > 0
                LOG_MSG("Received Input:");
                LOG_MSG("%s  - Manage Hidden Tag Entries", OffsetNext);
                LOG_MSG("\n\n");
                #endif

                // No end dash line ... Expected to return
                ManageHiddenTags();

                #if REFIT_DEBUG > 0
                LOG_MSG("Received Input:");
                LOG_MSG("%s  - Exit Hidden Tags Page", OffsetNext);
                LOG_MSG("\n\n");
                #endif

            break;
            case TAG_EXIT:    // Exit RefindPlus
                #if REFIT_DEBUG > 0
                LOG_MSG("Received Input:");
                LOG_MSG("%s  - Exit RefindPlus", OffsetNext);
                LOG_MSG("\n <<----- * ----->>\n\n");
                #endif

                if ((MokProtocol) && !SecureBootUninstall()) {
                    // Just in case we get this far
                    MainLoopRunning = FALSE;
                }
                else {
                   BeginTextScreen (L" ");
                   return EFI_SUCCESS;
                }

            break;
            case TAG_FIRMWARE: // Reboot into firmware's user interface
                #if REFIT_DEBUG > 0
                LOG_MSG("Received Input:");
                LOG_MSG("%s  - Reboot into Firmware", OffsetNext);
                #endif

                // No end dash line ... Added in 'RebootIntoFirmware'
                RebootIntoFirmware();

            break;
            case TAG_CSR_ROTATE:
                #if REFIT_DEBUG > 0
                LOG_MSG("Received Input:");
                LOG_MSG("%s  - Toggle Mac SIP", OffsetNext);
                LOG_MSG("\n");
                #endif

                // No end dash line ... Expected to return
                RotateCsrValue();

            break;
            case TAG_INSTALL:
                #if REFIT_DEBUG > 0
                LOG_MSG("Received Input:");
                LOG_MSG("%s  - Install RefindPlus", OffsetNext);
                LOG_MSG("\n\n");
                #endif

                // No end dash line ... Expected to return
                InstallRefindPlus();

            break;
            case TAG_BOOTORDER:
                #if REFIT_DEBUG > 0
                LOG_MSG("Received Input:");
                LOG_MSG("%s  - Manage Firmware Boot Order", OffsetNext);
                LOG_MSG("\n\n");
                #endif

                // No end dash line ... Expected to return
                ManageBootorder();

                #if REFIT_DEBUG > 0
                LOG_MSG("Received Input:");
                LOG_MSG("%s  - Exit Manage Firmware Boot Order Page", OffsetNext);
                LOG_MSG("\n\n");
                #endif

            break;
        } // switch

        // Disable DirectBoot if still active after first loop
        if (GlobalConfig.DirectBoot) {
            GlobalConfig.DirectBoot = FALSE;
            MainMenu->TimeoutSeconds = GlobalConfig.Timeout = 0;
            DrawScreenHeader (MainMenu->Title);
        }
    } // while

    // Things have gone wrong if we end up here ... Try to reboot.
    // Go into a dead loop if the reboot attempt fails.
    #if REFIT_DEBUG > 0
    ALT_LOG(1, LOG_THREE_STAR_SEP, L"Unexpected Main Loop Exit ... Try to Reboot!!");

    LOG_MSG("Fallback: System Restart...");
    LOG_MSG("\n");
    LOG_MSG("Screen Termination:");
    LOG_MSG("\n");
    #endif

    TerminateScreen();

    #if REFIT_DEBUG > 0
    LOG_MSG("System Reset:");
    LOG_MSG("\n\n");
    #endif

    REFIT_CALL_4_WRAPPER(
        gRT->ResetSystem, EfiResetCold,
        EFI_SUCCESS, 0, NULL
    );

    #if REFIT_DEBUG > 0
    ALT_LOG(1, LOG_THREE_STAR_SEP, L"Reset After Unexpected Main Loop Exit:- 'FAILED!!'");
    #endif

    SwitchToText (FALSE);

    MsgStr = StrDuplicate (L"E N T E R I N G   D E A D   L O O P");

    #if REFIT_DEBUG > 0
    MY_MUTELOGGER_SET;
    #endif
    REFIT_CALL_2_WRAPPER(gST->ConOut->SetAttribute, gST->ConOut, ATTR_ERROR);
    PrintUglyText (MsgStr, NEXTLINE);
    REFIT_CALL_2_WRAPPER(gST->ConOut->SetAttribute, gST->ConOut, ATTR_BASIC);
    #if REFIT_DEBUG > 0
    MY_MUTELOGGER_OFF;
    #endif

    #if REFIT_DEBUG > 0
    ALT_LOG(1, LOG_BLANK_LINE_SEP, L"X");
    ALT_LOG(1, LOG_LINE_NORMAL, L"%s", MsgStr);
    LOG_MSG("INFO: %s", MsgStr);
    LOG_MSG("\n <<----- * ----->>\n\n");
    #endif

    MY_FREE_POOL(MsgStr);

    PauseForKey();
    RefitDeadLoop();

    return EFI_SUCCESS;
} // EFI_STATUS EFIAPI efi_main()
