#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <iostream>

#include "PCIE.h"

#define MEM_SIZE			(512*1024) //512KB

#define PCIE_USER_BAR			PCIE_BAR4
#define PCIE_IO_LED_ADDR		0x04000010
#define PCIE_IO_BUTTON_ADDR		0x04000020
#define PCIE_MEM_ADDR			0x07000000

#define CONTROLLER_SIZE			(128) //128B
#define IMG_SIZE		     	(512) //512B

#define PCIE_FPGA_UP2_CONFLG    PCIE_IO_LED_ADDR

//#define PCIE_IMG_SOURCE_ADDR	0x40000000
//#define PCIE_IMG_DEST_ADDR		0x40000000
#define PCIE_IMG_SOURCE_ADDR	0x40320000
//#define PCIE_IMG_SOURCE_ADDR	0x40400000
//#define PCIE_IMG_DEST_ADDR		0x407d0000
#define PCIE_IMG_DEST_ADDR		0x40320000
#define OUT_DATA_ADDR		    0x415e0000

struct FPGAOutData {
	int RedBlockAddr;
	int RedLongitude;
	int RedLatitude;
	int maxGreenSum;
	int GreenLongitude;
	int GreenLatitude;
	int maxBlueSum;
	int BlueLongitude;
	int BlueLatitude;
	char padding[92];
};

int FPGA_Open();
void FPGA_Close();
void FPGA_Test();
bool FPGA_DmaWrite(uint32_t FPGAaddr, char *pWrite, int size);
bool FPGA_DmaRead(uint32_t FPGAaddr, char *pRead, int size);
bool FPGA_WriteSourceImg(char *pWrite, int ImgSize);
bool FPGA_ReadDestImg(char *pRead, int ImgSize);