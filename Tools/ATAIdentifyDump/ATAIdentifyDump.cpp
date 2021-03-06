#include "pch.h"

int main()
{
	printf("Dumping data for all disks.\n");
	printf("\n");
	for (int driveNumber = 0; driveNumber < 16; driveNumber++)
	{
		char deviceNameBuffer[64] = { 0 };
		sprintf_s(deviceNameBuffer, "\\\\.\\PhysicalDrive%d", driveNumber);

		HANDLE diskHandle = CreateFileA(
			deviceNameBuffer,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			0,
			OPEN_EXISTING,
			0,
			0
		);

		if (diskHandle == INVALID_HANDLE_VALUE)
		{
			printf("====================\n");
			printf("ERROR: Failed to open handle to %s. Last error: %d\n", deviceNameBuffer, GetLastError());
			printf("====================\n");
			continue;
		}

		const unsigned int IdentifyBufferSize = 512;
		const BYTE IdentifyCommandID = 0xEC;
		unsigned char Buffer[IdentifyBufferSize + sizeof(ATA_PASS_THROUGH_EX)] = { 0 };
		ATA_PASS_THROUGH_EX & pte = *reinterpret_cast<ATA_PASS_THROUGH_EX*>(Buffer);
		pte.Length = sizeof(pte);
		pte.TimeOutValue = 10;
		pte.DataTransferLength = 512;
		pte.DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX);

		IDEREGS* regs = (IDEREGS*)pte.CurrentTaskFile;
		regs->bCommandReg = IdentifyCommandID;
		regs->bSectorCountReg = 1;

		pte.AtaFlags = ATA_FLAGS_DATA_IN | ATA_FLAGS_DRDY_REQUIRED;

		DWORD br = 0;
		BOOL ioctlSuccess = DeviceIoControl(
			diskHandle,
			IOCTL_ATA_PASS_THROUGH,
			&pte,
			sizeof(Buffer),
			&pte,
			sizeof(Buffer),
			&br,
			0
		);

		if (!ioctlSuccess)
		{
			printf("====================\n");
			printf("ATA pass through IOCTL failed for drive %s. Last error: %d\n", deviceNameBuffer, GetLastError());
			printf("====================\n");
			CloseHandle(diskHandle);
			continue;
		}

		IDENTIFY_DEVICE_DATA *idd = reinterpret_cast<IDENTIFY_DEVICE_DATA*>(Buffer + sizeof(ATA_PASS_THROUGH_EX));

		printf("\n");
		printf("====================\n");
		printf("BEGIN IDENTIFY_DEVICE_DATA for %s\n", deviceNameBuffer);
		printf("====================\n");
		printf("GeneralConfiguration.Reserved1 = %hu\n", idd->GeneralConfiguration.Reserved1);
		printf("GeneralConfiguration.Retired3 = %hu\n", idd->GeneralConfiguration.Retired3);
		printf("GeneralConfiguration.ResponseIncomplete = %hu\n", idd->GeneralConfiguration.ResponseIncomplete);
		printf("GeneralConfiguration.Retired2 = %hu\n", idd->GeneralConfiguration.Retired2);
		printf("GeneralConfiguration.FixedDevice = %hu\n", idd->GeneralConfiguration.FixedDevice);
		printf("GeneralConfiguration.RemovableMedia = %hu\n", idd->GeneralConfiguration.RemovableMedia);
		printf("GeneralConfiguration.Retired1 = %hu\n", idd->GeneralConfiguration.Retired1);
		printf("GeneralConfiguration.DeviceType = %hu\n", idd->GeneralConfiguration.DeviceType);
		printf("NumCylinders = %hu\n", idd->NumCylinders);
		printf("ReservedWord2 = %hu\n", idd->ReservedWord2);
		printf("NumHeads = %hu\n", idd->NumHeads);
		printf("Retired1[0] = %hu\n", idd->Retired1[0]);
		printf("Retired1[1] = %hu\n", idd->Retired1[1]);
		printf("NumSectorsPerTrack = %hu\n", idd->NumSectorsPerTrack);
		printf("VendorUnique1[0] = %hu\n", idd->VendorUnique1[0]);
		printf("VendorUnique1[1] = %hu\n", idd->VendorUnique1[1]);
		printf("VendorUnique1[2] = %hu\n", idd->VendorUnique1[2]);
		printf("SerialNumber = \"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\"\n", idd->SerialNumber[1], idd->SerialNumber[0], idd->SerialNumber[3], idd->SerialNumber[2], idd->SerialNumber[5], idd->SerialNumber[4], idd->SerialNumber[7], idd->SerialNumber[6], idd->SerialNumber[9], idd->SerialNumber[8], idd->SerialNumber[11], idd->SerialNumber[10], idd->SerialNumber[13], idd->SerialNumber[12], idd->SerialNumber[15], idd->SerialNumber[14], idd->SerialNumber[17], idd->SerialNumber[16], idd->SerialNumber[19], idd->SerialNumber[18]);
		printf("Retired2[0] = %hu\n", idd->Retired2[0]);
		printf("Retired2[1] = %hu\n", idd->Retired2[1]);
		printf("Obsolete1 = %hu\n", idd->Obsolete1);
		printf("FirmwareRevision = \"%c%c%c%c%c%c%c%c\"\n", idd->FirmwareRevision[1], idd->FirmwareRevision[0], idd->FirmwareRevision[3], idd->FirmwareRevision[2], idd->FirmwareRevision[5], idd->FirmwareRevision[4], idd->FirmwareRevision[7], idd->FirmwareRevision[6]);
		printf("ModelNumber = \"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\"\n", idd->ModelNumber[1], idd->ModelNumber[0], idd->ModelNumber[3], idd->ModelNumber[2], idd->ModelNumber[5], idd->ModelNumber[4], idd->ModelNumber[7], idd->ModelNumber[6], idd->ModelNumber[9], idd->ModelNumber[8], idd->ModelNumber[11], idd->ModelNumber[10], idd->ModelNumber[13], idd->ModelNumber[12], idd->ModelNumber[15], idd->ModelNumber[14], idd->ModelNumber[17], idd->ModelNumber[16], idd->ModelNumber[19], idd->ModelNumber[18], idd->ModelNumber[21], idd->ModelNumber[20], idd->ModelNumber[23], idd->ModelNumber[22], idd->ModelNumber[25], idd->ModelNumber[24], idd->ModelNumber[27], idd->ModelNumber[26], idd->ModelNumber[29], idd->ModelNumber[28], idd->ModelNumber[31], idd->ModelNumber[30], idd->ModelNumber[33], idd->ModelNumber[32], idd->ModelNumber[35], idd->ModelNumber[34], idd->ModelNumber[37], idd->ModelNumber[36], idd->ModelNumber[39], idd->ModelNumber[38]);
		printf("MaximumBlockTransfer = %d\n", idd->MaximumBlockTransfer);
		printf("VendorUnique2 = %d\n", idd->VendorUnique2);
		printf("ReservedWord48 = %hu\n", idd->ReservedWord48);
		printf("Capabilities.ReservedByte49 = %d\n", idd->Capabilities.ReservedByte49);
		printf("Capabilities.DmaSupported = %d\n", idd->Capabilities.DmaSupported);
		printf("Capabilities.LbaSupported = %d\n", idd->Capabilities.LbaSupported);
		printf("Capabilities.IordyDisable = %d\n", idd->Capabilities.IordyDisable);
		printf("Capabilities.IordySupported = %d\n", idd->Capabilities.IordySupported);
		printf("Capabilities.Reserved1 = %d\n", idd->Capabilities.Reserved1);
		printf("Capabilities.StandybyTimerSupport = %d\n", idd->Capabilities.StandybyTimerSupport);
		printf("Capabilities.Reserved2 = %d\n", idd->Capabilities.Reserved2);
		printf("Capabilities.ReservedWord50 = %hu\n", idd->Capabilities.ReservedWord50);
		printf("ObsoleteWords51[0] = %hu\n", idd->ObsoleteWords51[0]);
		printf("ObsoleteWords51[1] = %hu\n", idd->ObsoleteWords51[1]);
		printf("TranslationFieldsValid = %hu\n", idd->TranslationFieldsValid);
		printf("Reserved3 = %hu\n", idd->Reserved3);
		printf("NumberOfCurrentCylinders = %hu\n", idd->NumberOfCurrentCylinders);
		printf("NumberOfCurrentHeads = %hu\n", idd->NumberOfCurrentHeads);
		printf("CurrentSectorsPerTrack = %hu\n", idd->CurrentSectorsPerTrack);
		printf("CurrentSectorCapacity = %lu\n", idd->CurrentSectorCapacity);
		printf("CurrentMultiSectorSetting = %d\n", idd->CurrentMultiSectorSetting);
		printf("MultiSectorSettingValid = %d\n", idd->MultiSectorSettingValid);
		printf("ReservedByte59 = %d\n", idd->ReservedByte59);
		printf("UserAddressableSectors = %lu\n", idd->UserAddressableSectors);
		printf("ObsoleteWord62 = %hu\n", idd->ObsoleteWord62);
		printf("MultiWordDMASupport = %hu\n", idd->MultiWordDMASupport);
		printf("MultiWordDMAActive = %hu\n", idd->MultiWordDMAActive);
		printf("AdvancedPIOModes = %hu\n", idd->AdvancedPIOModes);
		printf("ReservedByte64 = %hu\n", idd->ReservedByte64);
		printf("MinimumMWXferCycleTime = %hu\n", idd->MinimumMWXferCycleTime);
		printf("RecommendedMWXferCycleTime = %hu\n", idd->RecommendedMWXferCycleTime);
		printf("MinimumPIOCycleTime = %hu\n", idd->MinimumPIOCycleTime);
		printf("MinimumPIOCycleTimeIORDY = %hu\n", idd->MinimumPIOCycleTimeIORDY);
		printf("ReservedWords69[0] = %hu\n", idd->ReservedWords69[0]);
		printf("ReservedWords69[1] = %hu\n", idd->ReservedWords69[1]);
		printf("ReservedWords69[2] = %hu\n", idd->ReservedWords69[2]);
		printf("ReservedWords69[3] = %hu\n", idd->ReservedWords69[3]);
		printf("ReservedWords69[4] = %hu\n", idd->ReservedWords69[4]);
		printf("ReservedWords69[5] = %hu\n", idd->ReservedWords69[5]);
		printf("QueueDepth = %hu\n", idd->QueueDepth);
		printf("ReservedWord75 = %hu\n", idd->ReservedWord75);
		printf("ReservedWords76[0] = %hu\n", idd->ReservedWords76[0]);
		printf("ReservedWords76[1] = %hu\n", idd->ReservedWords76[1]);
		printf("ReservedWords76[2] = %hu\n", idd->ReservedWords76[2]);
		printf("ReservedWords76[3] = %hu\n", idd->ReservedWords76[3]);
		printf("MajorRevision = %hu\n", idd->MajorRevision);
		printf("MinorRevision = %hu\n", idd->MinorRevision);
		printf("CommandSetSupport.SmartCommands = %hu\n", idd->CommandSetSupport.SmartCommands);
		printf("CommandSetSupport.SecurityMode = %hu\n", idd->CommandSetSupport.SecurityMode);
		printf("CommandSetSupport.RemovableMediaFeature = %hu\n", idd->CommandSetSupport.RemovableMediaFeature);
		printf("CommandSetSupport.PowerManagement = %hu\n", idd->CommandSetSupport.PowerManagement);
		printf("CommandSetSupport.Reserved1 = %hu\n", idd->CommandSetSupport.Reserved1);
		printf("CommandSetSupport.WriteCache = %hu\n", idd->CommandSetSupport.WriteCache);
		printf("CommandSetSupport.LookAhead = %hu\n", idd->CommandSetSupport.LookAhead);
		printf("CommandSetSupport.ReleaseInterrupt = %hu\n", idd->CommandSetSupport.ReleaseInterrupt);
		printf("CommandSetSupport.ServiceInterrupt = %hu\n", idd->CommandSetSupport.ServiceInterrupt);
		printf("CommandSetSupport.DeviceReset = %hu\n", idd->CommandSetSupport.DeviceReset);
		printf("CommandSetSupport.HostProtectedArea = %hu\n", idd->CommandSetSupport.HostProtectedArea);
		printf("CommandSetSupport.Obsolete1 = %hu\n", idd->CommandSetSupport.Obsolete1);
		printf("CommandSetSupport.WriteBuffer = %hu\n", idd->CommandSetSupport.WriteBuffer);
		printf("CommandSetSupport.ReadBuffer = %hu\n", idd->CommandSetSupport.ReadBuffer);
		printf("CommandSetSupport.Nop = %hu\n", idd->CommandSetSupport.Nop);
		printf("CommandSetSupport.Obsolete2 = %hu\n", idd->CommandSetSupport.Obsolete2);
		printf("CommandSetSupport.DownloadMicrocode = %hu\n", idd->CommandSetSupport.DownloadMicrocode);
		printf("CommandSetSupport.DmaQueued = %hu\n", idd->CommandSetSupport.DmaQueued);
		printf("CommandSetSupport.Cfa = %hu\n", idd->CommandSetSupport.Cfa);
		printf("CommandSetSupport.AdvancedPm = %hu\n", idd->CommandSetSupport.AdvancedPm);
		printf("CommandSetSupport.Msn = %hu\n", idd->CommandSetSupport.Msn);
		printf("CommandSetSupport.PowerUpInStandby = %hu\n", idd->CommandSetSupport.PowerUpInStandby);
		printf("CommandSetSupport.ManualPowerUp = %hu\n", idd->CommandSetSupport.ManualPowerUp);
		printf("CommandSetSupport.Reserved2 = %hu\n", idd->CommandSetSupport.Reserved2);
		printf("CommandSetSupport.SetMax = %hu\n", idd->CommandSetSupport.SetMax);
		printf("CommandSetSupport.Acoustics = %hu\n", idd->CommandSetSupport.Acoustics);
		printf("CommandSetSupport.BigLba = %hu\n", idd->CommandSetSupport.BigLba);
		printf("CommandSetSupport.DeviceConfigOverlay = %hu\n", idd->CommandSetSupport.DeviceConfigOverlay);
		printf("CommandSetSupport.FlushCache = %hu\n", idd->CommandSetSupport.FlushCache);
		printf("CommandSetSupport.FlushCacheExt = %hu\n", idd->CommandSetSupport.FlushCacheExt);
		printf("CommandSetSupport.Resrved3 = %hu\n", idd->CommandSetSupport.Resrved3);
		printf("CommandSetSupport.SmartErrorLog = %hu\n", idd->CommandSetSupport.SmartErrorLog);
		printf("CommandSetSupport.SmartSelfTest = %hu\n", idd->CommandSetSupport.SmartSelfTest);
		printf("CommandSetSupport.MediaSerialNumber = %hu\n", idd->CommandSetSupport.MediaSerialNumber);
		printf("CommandSetSupport.MediaCardPassThrough = %hu\n", idd->CommandSetSupport.MediaCardPassThrough);
		printf("CommandSetSupport.StreamingFeature = %hu\n", idd->CommandSetSupport.StreamingFeature);
		printf("CommandSetSupport.GpLogging = %hu\n", idd->CommandSetSupport.GpLogging);
		printf("CommandSetSupport.WriteFua = %hu\n", idd->CommandSetSupport.WriteFua);
		printf("CommandSetSupport.WriteQueuedFua = %hu\n", idd->CommandSetSupport.WriteQueuedFua);
		printf("CommandSetSupport.WWN64Bit = %hu\n", idd->CommandSetSupport.WWN64Bit);
		printf("CommandSetSupport.URGReadStream = %hu\n", idd->CommandSetSupport.URGReadStream);
		printf("CommandSetSupport.URGWriteStream = %hu\n", idd->CommandSetSupport.URGWriteStream);
		printf("CommandSetSupport.ReservedForTechReport = %hu\n", idd->CommandSetSupport.ReservedForTechReport);
		printf("CommandSetSupport.IdleWithUnloadFeature = %hu\n", idd->CommandSetSupport.IdleWithUnloadFeature);
		printf("CommandSetSupport.Reserved4 = %hu\n", idd->CommandSetSupport.Reserved4);
		printf("CommandSetActive.SmartCommands = %hu\n", idd->CommandSetActive.SmartCommands);
		printf("CommandSetActive.SecurityMode = %hu\n", idd->CommandSetActive.SecurityMode);
		printf("CommandSetActive.RemovableMediaFeature = %hu\n", idd->CommandSetActive.RemovableMediaFeature);
		printf("CommandSetActive.PowerManagement = %hu\n", idd->CommandSetActive.PowerManagement);
		printf("CommandSetActive.Reserved1 = %hu\n", idd->CommandSetActive.Reserved1);
		printf("CommandSetActive.WriteCache = %hu\n", idd->CommandSetActive.WriteCache);
		printf("CommandSetActive.LookAhead = %hu\n", idd->CommandSetActive.LookAhead);
		printf("CommandSetActive.ReleaseInterrupt = %hu\n", idd->CommandSetActive.ReleaseInterrupt);
		printf("CommandSetActive.ServiceInterrupt = %hu\n", idd->CommandSetActive.ServiceInterrupt);
		printf("CommandSetActive.DeviceReset = %hu\n", idd->CommandSetActive.DeviceReset);
		printf("CommandSetActive.HostProtectedArea = %hu\n", idd->CommandSetActive.HostProtectedArea);
		printf("CommandSetActive.Obsolete1 = %hu\n", idd->CommandSetActive.Obsolete1);
		printf("CommandSetActive.WriteBuffer = %hu\n", idd->CommandSetActive.WriteBuffer);
		printf("CommandSetActive.ReadBuffer = %hu\n", idd->CommandSetActive.ReadBuffer);
		printf("CommandSetActive.Nop = %hu\n", idd->CommandSetActive.Nop);
		printf("CommandSetActive.Obsolete2 = %hu\n", idd->CommandSetActive.Obsolete2);
		printf("CommandSetActive.DownloadMicrocode = %hu\n", idd->CommandSetActive.DownloadMicrocode);
		printf("CommandSetActive.DmaQueued = %hu\n", idd->CommandSetActive.DmaQueued);
		printf("CommandSetActive.Cfa = %hu\n", idd->CommandSetActive.Cfa);
		printf("CommandSetActive.AdvancedPm = %hu\n", idd->CommandSetActive.AdvancedPm);
		printf("CommandSetActive.Msn = %hu\n", idd->CommandSetActive.Msn);
		printf("CommandSetActive.PowerUpInStandby = %hu\n", idd->CommandSetActive.PowerUpInStandby);
		printf("CommandSetActive.ManualPowerUp = %hu\n", idd->CommandSetActive.ManualPowerUp);
		printf("CommandSetActive.Reserved2 = %hu\n", idd->CommandSetActive.Reserved2);
		printf("CommandSetActive.SetMax = %hu\n", idd->CommandSetActive.SetMax);
		printf("CommandSetActive.Acoustics = %hu\n", idd->CommandSetActive.Acoustics);
		printf("CommandSetActive.BigLba = %hu\n", idd->CommandSetActive.BigLba);
		printf("CommandSetActive.DeviceConfigOverlay = %hu\n", idd->CommandSetActive.DeviceConfigOverlay);
		printf("CommandSetActive.FlushCache = %hu\n", idd->CommandSetActive.FlushCache);
		printf("CommandSetActive.FlushCacheExt = %hu\n", idd->CommandSetActive.FlushCacheExt);
		printf("CommandSetActive.Resrved3 = %hu\n", idd->CommandSetActive.Resrved3);
		printf("CommandSetActive.SmartErrorLog = %hu\n", idd->CommandSetActive.SmartErrorLog);
		printf("CommandSetActive.SmartSelfTest = %hu\n", idd->CommandSetActive.SmartSelfTest);
		printf("CommandSetActive.MediaSerialNumber = %hu\n", idd->CommandSetActive.MediaSerialNumber);
		printf("CommandSetActive.MediaCardPassThrough = %hu\n", idd->CommandSetActive.MediaCardPassThrough);
		printf("CommandSetActive.StreamingFeature = %hu\n", idd->CommandSetActive.StreamingFeature);
		printf("CommandSetActive.GpLogging = %hu\n", idd->CommandSetActive.GpLogging);
		printf("CommandSetActive.WriteFua = %hu\n", idd->CommandSetActive.WriteFua);
		printf("CommandSetActive.WriteQueuedFua = %hu\n", idd->CommandSetActive.WriteQueuedFua);
		printf("CommandSetActive.WWN64Bit = %hu\n", idd->CommandSetActive.WWN64Bit);
		printf("CommandSetActive.URGReadStream = %hu\n", idd->CommandSetActive.URGReadStream);
		printf("CommandSetActive.URGWriteStream = %hu\n", idd->CommandSetActive.URGWriteStream);
		printf("CommandSetActive.ReservedForTechReport = %hu\n", idd->CommandSetActive.ReservedForTechReport);
		printf("CommandSetActive.IdleWithUnloadFeature = %hu\n", idd->CommandSetActive.IdleWithUnloadFeature);
		printf("CommandSetActive.Reserved4 = %hu\n", idd->CommandSetActive.Reserved4);
		printf("UltraDMASupport = %hu\n", idd->UltraDMASupport);
		printf("UltraDMAActive = %hu\n", idd->UltraDMAActive);
		printf("ReservedWord89[0] = %hu\n", idd->ReservedWord89[0]);
		printf("ReservedWord89[1] = %hu\n", idd->ReservedWord89[1]);
		printf("ReservedWord89[2] = %hu\n", idd->ReservedWord89[2]);
		printf("ReservedWord89[3] = %hu\n", idd->ReservedWord89[3]);
		printf("HardwareResetResult = %hu\n", idd->HardwareResetResult);
		printf("CurrentAcousticValue = %hu\n", idd->CurrentAcousticValue);
		printf("RecommendedAcousticValue = %hu\n", idd->RecommendedAcousticValue);
		printf("ReservedWord95[0] = %hu\n", idd->ReservedWord95[0]);
		printf("ReservedWord95[1] = %hu\n", idd->ReservedWord95[1]);
		printf("ReservedWord95[2] = %hu\n", idd->ReservedWord95[2]);
		printf("ReservedWord95[3] = %hu\n", idd->ReservedWord95[3]);
		printf("ReservedWord95[4] = %hu\n", idd->ReservedWord95[4]);
		printf("Max48BitLBA[0] = %lu\n", idd->Max48BitLBA[0]);
		printf("Max48BitLBA[1] = %lu\n", idd->Max48BitLBA[1]);
		printf("StreamingTransferTime = %hu\n", idd->StreamingTransferTime);
		printf("ReservedWord105 = %hu\n", idd->ReservedWord105);
		printf("PhysicalLogicalSectorSize.LogicalSectorsPerPhysicalSector = %hu\n", idd->PhysicalLogicalSectorSize.LogicalSectorsPerPhysicalSector);
		printf("PhysicalLogicalSectorSize.Reserved0 = %hu\n", idd->PhysicalLogicalSectorSize.Reserved0);
		printf("PhysicalLogicalSectorSize.LogicalSectorLongerThan256Words = %hu\n", idd->PhysicalLogicalSectorSize.LogicalSectorLongerThan256Words);
		printf("PhysicalLogicalSectorSize.MultipleLogicalSectorsPerPhysicalSector = %hu\n", idd->PhysicalLogicalSectorSize.MultipleLogicalSectorsPerPhysicalSector);
		printf("PhysicalLogicalSectorSize.Reserved1 = %hu\n", idd->PhysicalLogicalSectorSize.Reserved1);
		printf("InterSeekDelay = %hu\n", idd->InterSeekDelay);
		printf("WorldWideName[0] = %hu\n", idd->WorldWideName[0]);
		printf("WorldWideName[1] = %hu\n", idd->WorldWideName[1]);
		printf("WorldWideName[2] = %hu\n", idd->WorldWideName[2]);
		printf("WorldWideName[3] = %hu\n", idd->WorldWideName[3]);
		printf("ReservedForWorldWideName128[0] = %hu\n", idd->ReservedForWorldWideName128[0]);
		printf("ReservedForWorldWideName128[1] = %hu\n", idd->ReservedForWorldWideName128[1]);
		printf("ReservedForWorldWideName128[2] = %hu\n", idd->ReservedForWorldWideName128[2]);
		printf("ReservedForWorldWideName128[3] = %hu\n", idd->ReservedForWorldWideName128[3]);
		printf("ReservedForTlcTechnicalReport = %hu\n", idd->ReservedForTlcTechnicalReport);
		printf("WordsPerLogicalSector[0] = %hu\n", idd->WordsPerLogicalSector[0]);
		printf("WordsPerLogicalSector[1] = %hu\n", idd->WordsPerLogicalSector[1]);
		printf("CommandSetSupportExt.ReservedForDrqTechnicalReport = %hu\n", idd->CommandSetSupportExt.ReservedForDrqTechnicalReport);
		printf("CommandSetSupportExt.WriteReadVerifySupported = %hu\n", idd->CommandSetSupportExt.WriteReadVerifySupported);
		printf("CommandSetSupportExt.Reserved01 = %hu\n", idd->CommandSetSupportExt.Reserved01);
		printf("CommandSetSupportExt.Reserved1 = %hu\n", idd->CommandSetSupportExt.Reserved1);
		printf("CommandSetActiveExt.ReservedForDrqTechnicalReport = %hu\n", idd->CommandSetActiveExt.ReservedForDrqTechnicalReport);
		printf("CommandSetActiveExt.WriteReadVerifyEnabled = %hu\n", idd->CommandSetActiveExt.WriteReadVerifyEnabled);
		printf("CommandSetActiveExt.Reserved01 = %hu\n", idd->CommandSetActiveExt.Reserved01);
		printf("CommandSetActiveExt.Reserved1 = %hu\n", idd->CommandSetActiveExt.Reserved1);
		printf("ReservedForExpandedSupportandActive[0] = %hu\n", idd->ReservedForExpandedSupportandActive[0]);
		printf("ReservedForExpandedSupportandActive[1] = %hu\n", idd->ReservedForExpandedSupportandActive[1]);
		printf("ReservedForExpandedSupportandActive[2] = %hu\n", idd->ReservedForExpandedSupportandActive[2]);
		printf("ReservedForExpandedSupportandActive[3] = %hu\n", idd->ReservedForExpandedSupportandActive[3]);
		printf("ReservedForExpandedSupportandActive[4] = %hu\n", idd->ReservedForExpandedSupportandActive[4]);
		printf("ReservedForExpandedSupportandActive[5] = %hu\n", idd->ReservedForExpandedSupportandActive[5]);
		printf("MsnSupport = %hu\n", idd->MsnSupport);
		printf("ReservedWord1274 = %hu\n", idd->ReservedWord1274);
		printf("SecurityStatus.SecuritySupported = %hu\n", idd->SecurityStatus.SecuritySupported);
		printf("SecurityStatus.SecurityEnabled = %hu\n", idd->SecurityStatus.SecurityEnabled);
		printf("SecurityStatus.SecurityLocked = %hu\n", idd->SecurityStatus.SecurityLocked);
		printf("SecurityStatus.SecurityFrozen = %hu\n", idd->SecurityStatus.SecurityFrozen);
		printf("SecurityStatus.SecurityCountExpired = %hu\n", idd->SecurityStatus.SecurityCountExpired);
		printf("SecurityStatus.EnhancedSecurityEraseSupported = %hu\n", idd->SecurityStatus.EnhancedSecurityEraseSupported);
		printf("SecurityStatus.Reserved0 = %hu\n", idd->SecurityStatus.Reserved0);
		printf("SecurityStatus.SecurityLevel = %hu\n", idd->SecurityStatus.SecurityLevel);
		printf("SecurityStatus.Reserved1 = %hu\n", idd->SecurityStatus.Reserved1);
		printf("ReservedWord129[0] = %hu\n", idd->ReservedWord129[0]);
		printf("ReservedWord129[1] = %hu\n", idd->ReservedWord129[1]);
		printf("ReservedWord129[2] = %hu\n", idd->ReservedWord129[2]);
		printf("ReservedWord129[3] = %hu\n", idd->ReservedWord129[3]);
		printf("ReservedWord129[4] = %hu\n", idd->ReservedWord129[4]);
		printf("ReservedWord129[5] = %hu\n", idd->ReservedWord129[5]);
		printf("ReservedWord129[6] = %hu\n", idd->ReservedWord129[6]);
		printf("ReservedWord129[7] = %hu\n", idd->ReservedWord129[7]);
		printf("ReservedWord129[8] = %hu\n", idd->ReservedWord129[8]);
		printf("ReservedWord129[9] = %hu\n", idd->ReservedWord129[9]);
		printf("ReservedWord129[10] = %hu\n", idd->ReservedWord129[10]);
		printf("ReservedWord129[11] = %hu\n", idd->ReservedWord129[11]);
		printf("ReservedWord129[12] = %hu\n", idd->ReservedWord129[12]);
		printf("ReservedWord129[13] = %hu\n", idd->ReservedWord129[13]);
		printf("ReservedWord129[14] = %hu\n", idd->ReservedWord129[14]);
		printf("ReservedWord129[15] = %hu\n", idd->ReservedWord129[15]);
		printf("ReservedWord129[16] = %hu\n", idd->ReservedWord129[16]);
		printf("ReservedWord129[17] = %hu\n", idd->ReservedWord129[17]);
		printf("ReservedWord129[18] = %hu\n", idd->ReservedWord129[18]);
		printf("ReservedWord129[19] = %hu\n", idd->ReservedWord129[19]);
		printf("ReservedWord129[20] = %hu\n", idd->ReservedWord129[20]);
		printf("ReservedWord129[21] = %hu\n", idd->ReservedWord129[21]);
		printf("ReservedWord129[22] = %hu\n", idd->ReservedWord129[22]);
		printf("ReservedWord129[23] = %hu\n", idd->ReservedWord129[23]);
		printf("ReservedWord129[24] = %hu\n", idd->ReservedWord129[24]);
		printf("ReservedWord129[25] = %hu\n", idd->ReservedWord129[25]);
		printf("ReservedWord129[26] = %hu\n", idd->ReservedWord129[26]);
		printf("ReservedWord129[27] = %hu\n", idd->ReservedWord129[27]);
		printf("ReservedWord129[28] = %hu\n", idd->ReservedWord129[28]);
		printf("ReservedWord129[29] = %hu\n", idd->ReservedWord129[29]);
		printf("ReservedWord129[30] = %hu\n", idd->ReservedWord129[30]);
		printf("CfaPowerModel.MaximumCurrentInMA2 = %hu\n", idd->CfaPowerModel.MaximumCurrentInMA2);
		printf("CfaPowerModel.CfaPowerMode1Disabled = %hu\n", idd->CfaPowerModel.CfaPowerMode1Disabled);
		printf("CfaPowerModel.CfaPowerMode1Required = %hu\n", idd->CfaPowerModel.CfaPowerMode1Required);
		printf("CfaPowerModel.Reserved0 = %hu\n", idd->CfaPowerModel.Reserved0);
		printf("CfaPowerModel.Word160Supported = %hu\n", idd->CfaPowerModel.Word160Supported);
		printf("ReservedForCfaWord161[0] = %hu\n", idd->ReservedForCfaWord161[0]);
		printf("ReservedForCfaWord161[1] = %hu\n", idd->ReservedForCfaWord161[1]);
		printf("ReservedForCfaWord161[2] = %hu\n", idd->ReservedForCfaWord161[2]);
		printf("ReservedForCfaWord161[3] = %hu\n", idd->ReservedForCfaWord161[3]);
		printf("ReservedForCfaWord161[4] = %hu\n", idd->ReservedForCfaWord161[4]);
		printf("ReservedForCfaWord161[5] = %hu\n", idd->ReservedForCfaWord161[5]);
		printf("ReservedForCfaWord161[6] = %hu\n", idd->ReservedForCfaWord161[6]);
		printf("ReservedForCfaWord161[7] = %hu\n", idd->ReservedForCfaWord161[7]);
		printf("DataSetManagementFeature.SupportsTrim = %hu\n", idd->DataSetManagementFeature.SupportsTrim);
		printf("DataSetManagementFeature.Reserved0 = %hu\n", idd->DataSetManagementFeature.Reserved0);
		printf("ReservedForCfaWord170[0] = %hu\n", idd->ReservedForCfaWord170[0]);
		printf("ReservedForCfaWord170[1] = %hu\n", idd->ReservedForCfaWord170[1]);
		printf("ReservedForCfaWord170[2] = %hu\n", idd->ReservedForCfaWord170[2]);
		printf("ReservedForCfaWord170[3] = %hu\n", idd->ReservedForCfaWord170[3]);
		printf("ReservedForCfaWord170[4] = %hu\n", idd->ReservedForCfaWord170[4]);
		printf("ReservedForCfaWord170[5] = %hu\n", idd->ReservedForCfaWord170[5]);
		printf("CurrentMediaSerialNumber[0] = %hu\n", idd->CurrentMediaSerialNumber[0]);
		printf("CurrentMediaSerialNumber[1] = %hu\n", idd->CurrentMediaSerialNumber[1]);
		printf("CurrentMediaSerialNumber[2] = %hu\n", idd->CurrentMediaSerialNumber[2]);
		printf("CurrentMediaSerialNumber[3] = %hu\n", idd->CurrentMediaSerialNumber[3]);
		printf("CurrentMediaSerialNumber[4] = %hu\n", idd->CurrentMediaSerialNumber[4]);
		printf("CurrentMediaSerialNumber[5] = %hu\n", idd->CurrentMediaSerialNumber[5]);
		printf("CurrentMediaSerialNumber[6] = %hu\n", idd->CurrentMediaSerialNumber[6]);
		printf("CurrentMediaSerialNumber[7] = %hu\n", idd->CurrentMediaSerialNumber[7]);
		printf("CurrentMediaSerialNumber[8] = %hu\n", idd->CurrentMediaSerialNumber[8]);
		printf("CurrentMediaSerialNumber[9] = %hu\n", idd->CurrentMediaSerialNumber[9]);
		printf("CurrentMediaSerialNumber[10] = %hu\n", idd->CurrentMediaSerialNumber[10]);
		printf("CurrentMediaSerialNumber[11] = %hu\n", idd->CurrentMediaSerialNumber[11]);
		printf("CurrentMediaSerialNumber[12] = %hu\n", idd->CurrentMediaSerialNumber[12]);
		printf("CurrentMediaSerialNumber[13] = %hu\n", idd->CurrentMediaSerialNumber[13]);
		printf("CurrentMediaSerialNumber[14] = %hu\n", idd->CurrentMediaSerialNumber[14]);
		printf("CurrentMediaSerialNumber[15] = %hu\n", idd->CurrentMediaSerialNumber[15]);
		printf("CurrentMediaSerialNumber[16] = %hu\n", idd->CurrentMediaSerialNumber[16]);
		printf("CurrentMediaSerialNumber[17] = %hu\n", idd->CurrentMediaSerialNumber[17]);
		printf("CurrentMediaSerialNumber[18] = %hu\n", idd->CurrentMediaSerialNumber[18]);
		printf("CurrentMediaSerialNumber[19] = %hu\n", idd->CurrentMediaSerialNumber[19]);
		printf("CurrentMediaSerialNumber[20] = %hu\n", idd->CurrentMediaSerialNumber[20]);
		printf("CurrentMediaSerialNumber[21] = %hu\n", idd->CurrentMediaSerialNumber[21]);
		printf("CurrentMediaSerialNumber[22] = %hu\n", idd->CurrentMediaSerialNumber[22]);
		printf("CurrentMediaSerialNumber[23] = %hu\n", idd->CurrentMediaSerialNumber[23]);
		printf("CurrentMediaSerialNumber[24] = %hu\n", idd->CurrentMediaSerialNumber[24]);
		printf("CurrentMediaSerialNumber[25] = %hu\n", idd->CurrentMediaSerialNumber[25]);
		printf("CurrentMediaSerialNumber[26] = %hu\n", idd->CurrentMediaSerialNumber[26]);
		printf("CurrentMediaSerialNumber[27] = %hu\n", idd->CurrentMediaSerialNumber[27]);
		printf("CurrentMediaSerialNumber[28] = %hu\n", idd->CurrentMediaSerialNumber[28]);
		printf("CurrentMediaSerialNumber[29] = %hu\n", idd->CurrentMediaSerialNumber[29]);
		printf("ReservedWord206 = %hu\n", idd->ReservedWord206);
		printf("ReservedWord207[0] = %hu\n", idd->ReservedWord207[0]);
		printf("ReservedWord207[1] = %hu\n", idd->ReservedWord207[1]);
		printf("BlockAlignment.AlignmentOfLogicalWithinPhysical = %hu\n", idd->BlockAlignment.AlignmentOfLogicalWithinPhysical);
		printf("BlockAlignment.Word209Supported = %hu\n", idd->BlockAlignment.Word209Supported);
		printf("BlockAlignment.Reserved0 = %hu\n", idd->BlockAlignment.Reserved0);
		printf("WriteReadVerifySectorCountMode3Only[0] = %hu\n", idd->WriteReadVerifySectorCountMode3Only[0]);
		printf("WriteReadVerifySectorCountMode3Only[1] = %hu\n", idd->WriteReadVerifySectorCountMode3Only[1]);
		printf("WriteReadVerifySectorCountMode2Only[0] = %hu\n", idd->WriteReadVerifySectorCountMode2Only[0]);
		printf("WriteReadVerifySectorCountMode2Only[1] = %hu\n", idd->WriteReadVerifySectorCountMode2Only[1]);
		printf("NVCacheCapabilities.NVCachePowerModeEnabled = %hu\n", idd->NVCacheCapabilities.NVCachePowerModeEnabled);
		printf("NVCacheCapabilities.Reserved0 = %hu\n", idd->NVCacheCapabilities.Reserved0);
		printf("NVCacheCapabilities.NVCacheFeatureSetEnabled = %hu\n", idd->NVCacheCapabilities.NVCacheFeatureSetEnabled);
		printf("NVCacheCapabilities.Reserved1 = %hu\n", idd->NVCacheCapabilities.Reserved1);
		printf("NVCacheCapabilities.NVCachePowerModeVersion = %hu\n", idd->NVCacheCapabilities.NVCachePowerModeVersion);
		printf("NVCacheCapabilities.NVCacheFeatureSetVersion = %hu\n", idd->NVCacheCapabilities.NVCacheFeatureSetVersion);
		printf("NVCacheSizeLSW = %hu\n", idd->NVCacheSizeLSW);
		printf("NVCacheSizeMSW = %hu\n", idd->NVCacheSizeMSW);
		printf("NominalMediaRotationRate = %hu\n", idd->NominalMediaRotationRate);
		printf("ReservedWord218 = %hu\n", idd->ReservedWord218);
		printf("NVCacheOptions.NVCacheEstimatedTimeToSpinUpInSeconds = %d\n", idd->NVCacheOptions.NVCacheEstimatedTimeToSpinUpInSeconds);
		printf("NVCacheOptions.Reserved = %d\n", idd->NVCacheOptions.Reserved);
		printf("ReservedWord220[0] = %hu\n", idd->ReservedWord220[0]);
		printf("ReservedWord220[1] = %hu\n", idd->ReservedWord220[1]);
		printf("ReservedWord220[2] = %hu\n", idd->ReservedWord220[2]);
		printf("ReservedWord220[3] = %hu\n", idd->ReservedWord220[3]);
		printf("ReservedWord220[4] = %hu\n", idd->ReservedWord220[4]);
		printf("ReservedWord220[5] = %hu\n", idd->ReservedWord220[5]);
		printf("ReservedWord220[6] = %hu\n", idd->ReservedWord220[6]);
		printf("ReservedWord220[7] = %hu\n", idd->ReservedWord220[7]);
		printf("ReservedWord220[8] = %hu\n", idd->ReservedWord220[8]);
		printf("ReservedWord220[9] = %hu\n", idd->ReservedWord220[9]);
		printf("ReservedWord220[10] = %hu\n", idd->ReservedWord220[10]);
		printf("ReservedWord220[11] = %hu\n", idd->ReservedWord220[11]);
		printf("ReservedWord220[12] = %hu\n", idd->ReservedWord220[12]);
		printf("ReservedWord220[13] = %hu\n", idd->ReservedWord220[13]);
		printf("ReservedWord220[14] = %hu\n", idd->ReservedWord220[14]);
		printf("ReservedWord220[15] = %hu\n", idd->ReservedWord220[15]);
		printf("ReservedWord220[16] = %hu\n", idd->ReservedWord220[16]);
		printf("ReservedWord220[17] = %hu\n", idd->ReservedWord220[17]);
		printf("ReservedWord220[18] = %hu\n", idd->ReservedWord220[18]);
		printf("ReservedWord220[19] = %hu\n", idd->ReservedWord220[19]);
		printf("ReservedWord220[20] = %hu\n", idd->ReservedWord220[20]);
		printf("ReservedWord220[21] = %hu\n", idd->ReservedWord220[21]);
		printf("ReservedWord220[22] = %hu\n", idd->ReservedWord220[22]);
		printf("ReservedWord220[23] = %hu\n", idd->ReservedWord220[23]);
		printf("ReservedWord220[24] = %hu\n", idd->ReservedWord220[24]);
		printf("ReservedWord220[25] = %hu\n", idd->ReservedWord220[25]);
		printf("ReservedWord220[26] = %hu\n", idd->ReservedWord220[26]);
		printf("ReservedWord220[27] = %hu\n", idd->ReservedWord220[27]);
		printf("ReservedWord220[28] = %hu\n", idd->ReservedWord220[28]);
		printf("ReservedWord220[29] = %hu\n", idd->ReservedWord220[29]);
		printf("ReservedWord220[30] = %hu\n", idd->ReservedWord220[30]);
		printf("ReservedWord220[31] = %hu\n", idd->ReservedWord220[31]);
		printf("ReservedWord220[32] = %hu\n", idd->ReservedWord220[32]);
		printf("ReservedWord220[33] = %hu\n", idd->ReservedWord220[33]);
		printf("ReservedWord220[34] = %hu\n", idd->ReservedWord220[34]);
		printf("Signature = %hu\n", idd->Signature);
		printf("CheckSum = %hu\n", idd->CheckSum);
		printf("====================\n");
		printf("END IDENTIFY_DEVICE_DATA for %s\n", deviceNameBuffer);
		printf("====================\n");
		printf("\n");

		CloseHandle(diskHandle);
	}

	return 0;
}
