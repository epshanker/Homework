#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "hidapi.h"

#define MAX_STR 255

short accX[200], accY[200], accZ[200];

int main(int argc, char* argv[])
{
		int res;
		unsigned char buf[65];
		wchar_t wstr[MAX_STR];
		hid_device *handle;
		int i;

		// Initialize the hidapi library
		res = hid_init();

		// Open the device using the VID, PID,
		// and optionally the Serial number.
		handle = hid_open(0x4d8, 0x3f, NULL);

		// Read the Manufacturer String
		res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
		wprintf(L"Manufacturer String: %s\n", wstr);

		// Read the Product String
		res = hid_get_product_string(handle, wstr, MAX_STR);
		wprintf(L"Product String: %s\n", wstr);

		// Read the Serial Number String
		res = hid_get_serial_number_string(handle, wstr, MAX_STR);
		wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);

		// Read Indexed String 1
		res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
		wprintf(L"Indexed String 1: %s\n", wstr);


		int request,row;
		char message[62];
		scanf("%d %s", &row, &message);
		for (request=0;request<200;request++) {



		// Toggle LED (cmd 0x80). The first byte is the report number (0x0).
		int ii=0;
		buf[0] = 0x0;
		buf[1] = 0x80;
		//scanf("%d %s", &row, &message);
		buf[2] = row;
		//scanf("%c", &message);
		for (ii=0;ii<62;ii++) {
			buf[ii+3] = message[ii];
		}
		res = hid_write(handle, buf, 65);

		// Request state (cmd 0x81). The first byte is the report number (0x0).
		buf[0] = 0x0;
		buf[1] = 0x81;
		res = hid_write(handle, buf, 65);

		// Read requested state
		res = hid_read(handle, buf, 65);

		// buffer 0-->read, buffer 1-->x high, buffer 2-->x low, buffer 3-->y high, buffer 4-->y low, buffer 5-->z high, buffer 6-->z low
		short accels[3];
		accels[0] = (buf[1]<<8)|buf[2];
		accels[1] = (buf[3]<<8)|buf[4];
		accels[2] = (buf[5]<<8)|buf[6];

		// Print out the returned buffer.
		//for (i = 0; i <= 6; i++) {
		//	printf("buf[%d]: %d\n", i, buf[i]);
		//}
		//printf("***READING #%d***\nx accel: %d\ny accel: %d\nz accel: %d\n\n",(request+1),accels[0],accels[1],accels[2]);

		// Finalize the hidapi library
		res = hid_exit();

		// ADD CODE THAT SAVES X Y AND Z DATA TO ARRAYS

		accX[request] = accels[0];
		accY[request] = accels[1];
		accZ[request] = accels[2];

	}
		printf("***%d SUCCESSFUL READS***\n",(request));

		int jj;
		FILE *dataFile;
		dataFile = fopen("accels.txt","w");
		for (jj=0;jj<200;jj++) {
			fprintf(dataFile,"%d    %d    %d\r\n",accX[jj],accY[jj],accZ[jj]);
		}
		fclose(dataFile);

		printf("data (hopefully) sent to some file\n\n");

		return 0;
}
