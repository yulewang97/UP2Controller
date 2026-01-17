// PCIE_FUNDAMENTAL.cpp : Defines the entry point for the console application.
//
#pragma warning(disable:4996)

#include "FPGAController.h"

static PCIE_HANDLE hPCIE;
static void *lib_handle;

int FPGA_Open()
{
	std::cout << "== Terasic: c5p Start ==" << std::endl;

	lib_handle = PCIE_Load();
	if (!lib_handle) {
		std::cout << "PCIE_Load failed!\r\n" << std::endl;
		return -1;
	}

	hPCIE = PCIE_Open(DEFAULT_PCIE_VID, DEFAULT_PCIE_DID, 0);
	if (!hPCIE) {
		std::cout << "PCIE_Open failed\r\n" << std::endl;
		return -1;
	}

	return 0;
}

void FPGA_Close()
{
	PCIE_Close(hPCIE);
	PCIE_Unload(lib_handle);
}

void FPGA_Test()
{
	if (FPGA_Open() < 0) {
		std::cout << "Can't open FPGA" << std::endl;
		while (1);
		return;
	}
	std::cout << "Open FPGA successful" << std::endl;
	const int ImgSize = IMG_SIZE;
	const PCIE_LOCAL_ADDRESS FPGAWriteAddr = PCIE_IMG_SOURCE_ADDR;
	const PCIE_LOCAL_ADDRESS FPGAReadAddr = PCIE_IMG_DEST_ADDR;
	char *pWrite = (char *)malloc(ImgSize);
	char *pRead = (char *)malloc(ImgSize);
	strcpy(pWrite, "cbcdefghijklmnopq");
	if (PCIE_DmaWrite(hPCIE, FPGAWriteAddr, pWrite, ImgSize)) {
		std::cout << "Write FPGA successful" << std::endl;
	}
	else {
		std::cout << "Write FPGA Fail" << std::endl;
	}
	if (PCIE_DmaRead(hPCIE, FPGAWriteAddr, pRead, ImgSize)) {
		std::cout << pRead << std::endl;
	}
	else {
		std::cout << "Read FPGA Fail" << std::endl;
	}
	if (PCIE_DmaRead(hPCIE, FPGAReadAddr, pRead, ImgSize)) {
		std::cout << pRead << std::endl;
	}
	else {
		std::cout << "Read FPGA Fail" << std::endl;
	}
	FPGA_Close();
	while (1);
	return;
}

bool FPGA_DmaWrite(uint32_t FPGAaddr, char *pWrite, int size)
{
	const PCIE_LOCAL_ADDRESS FPGAWriteAddr = FPGAaddr;
	if (!PCIE_DmaWrite(hPCIE, FPGAWriteAddr, pWrite, size)) {
		std::cout << "FPGA_DmaWrite::Write FPGA Fail" << std::endl;
		return false;
	}
	return true;
}

bool FPGA_DmaRead(uint32_t FPGAaddr, char *pRead, int size)
{
	const PCIE_LOCAL_ADDRESS FPGAReadAddr = FPGAaddr;
	if (!PCIE_DmaRead(hPCIE, FPGAReadAddr, pRead, size)) {
		std::cout << "FPGA_DmaRead::Read FPGA Fail" << std::endl;
		return false;
	}
	return true;
}

bool FPGA_WriteSourceImg(char *pWrite, int ImgSize)
{
	const PCIE_LOCAL_ADDRESS FPGAWriteAddr = PCIE_IMG_SOURCE_ADDR;
	if (!PCIE_DmaWrite(hPCIE, FPGAWriteAddr, pWrite, ImgSize)) {
		std::cout << "FPGA_WriteSourceImg::Write FPGA Fail" << std::endl;
		return false;
	}
	return true;
}

bool FPGA_ReadDestImg(char *pRead, int ImgSize)
{
	const PCIE_LOCAL_ADDRESS FPGAReadAddr = PCIE_IMG_DEST_ADDR;
	if (!PCIE_DmaRead(hPCIE, FPGAReadAddr, pRead, ImgSize)) {
		std::cout << "FPGA_ReadDestImg::Read FPGA Fail" << std::endl;
		return false;
	}
	return true;
}
