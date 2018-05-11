// VisaExample.cpp : Defines the entry point for the console application.
// Additional Dependency: C:\Program Files\IVI Foundation\VISA\Win64\lib\msc;
//

#include "stdafx.h"
#include "visa.h"

#define MAX_CNT 200

int bclear(ViChar*, int);

int main()
{
	ViStatus    status;					/* For checking errors         */
	ViSession   defaultRM, instr;		/* Communication channels      */
	ViUInt32    retCount = 0;			/* Return count from string I/O */
	const int	bufferSz = 256;
	ViChar		buffer[bufferSz];			/* Buffer for string I/O       */
	ViUInt32	numInstrs = 0;
	ViFindList	fList = 0;
	ViChar		desc[VI_FIND_BUFLEN];
	ViChar		errorText[256];
	ViUInt16	iManf;
	ViString	iModelStr[256];
	int i,j,k,l; //counter ints
	char*		deviceIDs[256];
	int			maxChoice = 0;
	int			instNum = 999;
	ViPUInt32	retCnt = 0;
	char		cmd[128];
	
	for (i = 0; i < 256; i++) {
		deviceIDs[i] = (char*)malloc(256 * (sizeof(deviceIDs)));
	}

	status = viOpenDefaultRM(&defaultRM);
	if (status < VI_SUCCESS) {
		/* Error Initializing VISA...exiting*/
		return -1;
	}
	status = viFindRsrc(defaultRM, "?*INSTR", &fList, &numInstrs, desc);
	strcpy_s(deviceIDs[0], 256, desc);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf(errorText);
		printf("viFindRsrc: %d", status);
		return -1;
	}
	printf("Found %d instruments\n", numInstrs);
	printf("Description desc: %s\n", desc);
	for(int i=numInstrs-1; i>0; i--){
		viFindNext(fList, desc);
		strcpy_s(deviceIDs[i], 256, desc);
		maxChoice++;
	}
	for (i = 0; i < numInstrs; i++) {
		printf("inst %d: %s\n", i, deviceIDs[i]);
	}
	printf("Choose which instrument(999 to quit) : ");
	scanf("%d", &instNum);
	if (0 <= instNum && instNum <= maxChoice) {

	}
	else if (instNum == 999) {
		printf("Quitting\n");
		return 0;
	}
	else {
		printf("Invalid choice!! Seriously?  sigh....");
		return 1;
	}
	viOpen(defaultRM, deviceIDs[instNum], VI_NULL, VI_NULL, &instr);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf(errorText);
		printf("viFindRsrc: %d", status);
		return -1;
	}
	bclear(buffer, bufferSz);
	sprintf(cmd, "*idn?\n");
	status = viQueryf(instr, (ViConstString)cmd, "%64c", buffer);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf("%x: %s\n",status, errorText);
		return -1;
	}
	printf("ID: %s", buffer);
	
	bclear(buffer, bufferSz);
	sprintf(cmd, "ch1:outp on\n");
	status = viWrite(instr, (ViConstBuf)cmd, strlen(cmd), retCnt);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf("%x: %s\n", status, errorText);
		return -1;
	}
	sprintf(cmd, "ch1:bswv frq, 1kHz\n");
	status = viWrite(instr, (ViConstBuf)cmd, strlen(cmd), retCnt);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf("%x: %s\n", status, errorText);
		return -1;
	}

	status = viClose(defaultRM);
	if (status < VI_SUCCESS) {
		viStatusDesc(defaultRM, status, errorText);
		printf("%x: %s\n", status, errorText);
		return -1;
	}
	return 0;
}

int bclear(ViChar* buff, int length) {
	int i;
	for (i = 0; i < length; i++) {
		buff[i] = 0;
	}
	return 0;
}
