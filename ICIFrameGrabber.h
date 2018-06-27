#ifndef FRAMEGRABBER_H
#define FRAMEGRABBER_H
#include <libusb-1.0/libusb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <memory>

enum ReturnMessages {
	NO_ERROR = 0,
	NULL_USB_HANDLER,
	NULL_IMAGE_DATA,
	NULL_FPA_LENS_DATA,
	NULL_FILE_POINTER,
	ERROR_START_CAMERA_NOT_CALLED,
	RECEIVED_SHORT_FRAME,
	ERROR_SERIAL_NUMBER,
	CAL_FILE_NOT_FOUND,
	CAL_FILE_NOT_LOADED,
	ERROR_IN_LOADING_CALIBRATION_FILE,
	ERROR_IN_LOADING_AUTONUC_FILE,
	AUTO_NUC_FILE_NOT_FOUND,
	OUT_OF_MEMORY_ERROR,
	PATH_NOT_EXISIS,
	FRAME_RECEIVED_WHILE_SHUTTER_IS_CLOSED,
	NULL_TEMPERATURE_DATA,
	LIBUSB_INITIALIZATION_FAILED,
	LIBUSB_GET_DEVICE_DESCRIPTOR_FAILED,
	LIBUSB_OPEN_DEVICE_FAILED,
	LIBUSB_GET_TOTAL_NUMBER_OF_DEVICES_FAILED,
	LIBUSB_CANNOT_CLAIM_INTERFACE,
	LIBUSB_NULL_BULK_TRANSFER,
	LIBUSB_ERROR_SUBMITTING_USB_TRANSFER,
	NULL_LIBUSB_DEVICE,
	ERROR_DOWNLOAD_CAMERA_FIRMWARE,
	ERROR_FINDING_CAMERA_SERIAL_NUMBER,
	ERROR_SETTING_CAMERA_RESOLUTION    
};



#define PRINTFS

#ifdef PRINTFS
#define VDBG(...) do {\
			printf(__VA_ARGS__);\
			}while(0)
#else
#define VDBG(...)
#endif

class USBHandler {
public:
	struct libusb_device_handle *m_USBHandle;
	struct libusb_transfer *m_BulkTransfer[1];
};

struct TemperatureData {
	float fpa;
	float lens;
	unsigned short fpaState;
};

class ICIFrameGrabber;

class ICICameraFinder {
private:
	struct ICICameraFinderImpl;
	ICICameraFinderImpl  *m_pICICameraFinderImpl;
public:
	int GetConnectedCameras(std::vector<ICIFrameGrabber *> &totalNumberOfConnectedCameras);
	ICICameraFinder();
	~ICICameraFinder();
};

class ICIFrameGrabber {
private:
	struct ICIFrameGrabberImpl;
	ICIFrameGrabberImpl  *m_pICIFrameGrabberImpl;
	friend class ICICameraFinder;
	ICIFrameGrabber();
public:

	~ICIFrameGrabber();
	/*
	 * Description: StartCamera will start the camera and establish connection with the camera. 
	 *
	 * Return value:
	 * On Success, StartCamera return the pointer to the USBHandler class.
	 */
	int StartCamera();

	/*
	 * Description: ShutdownCamera will disconnect the camera from the host application. 
	 *
	 * Arguments:
	 * USBHandler *pUSBHandler: Pointer to the USBHandler class. 
	 *
	 */

	void ShutdownCamera();

	/*
	 * Description: GetCameraSerialNumber returns the camera serial number. User need to call StartCamera before calling this method.
	 *
	 * Return Value:
	 * GetCameraSerial returns ERROR_SERIAL_NUMBER on failure. 
	 */

	long GetCameraSerialNumber();

	/*
	 * Description: GetWidth returns the width of the image. User need to call StartCamera before calling this method. 
	 *
	 * Return Values:
	 * GetWidth returns BAD_WIDTH_HEIGHT on failure. 
	 */
	int GetWidth();

	/*
	 * Description: GetHeight returns the height of the image. User need to call StartCamera before calling this method. 
	 *
	 * Return Value:
	 * GetHeight returns BAD_WIDTH_HEIGHT on Failure. 
	 */
	int GetHeight();


	int GetFPALensTemperatureData(TemperatureData *pTemperatureData);


	/*
	 * Description: GetImageData get single frame from the camera.
	 * 
	 * Arguments:
	 * USBHandler *pUSBHandler: Pointer to the USBHandler class. Pointer to the USBHandler class can be received by calling the StartCamera() method.
	 * unsigned short *pImageData: Pointer of unsigned short to receive the frame data. User would need allocate the memory for pImageData. 
	 * 			       User should allocate width*height*sizeof(unsigned short) memory for pImageData.
	 * int *transferred: Transferred returns the number of bytes received from the camera.
	 *
	 * Return Value:
	 * GetImageData returns one of the values from RTNERRORS. On Success, GetImageData returns NO_ERROR.
	 *
	 */
	int GetRawImage(unsigned short *pImageData, int *transferred);

	/*
	 * Description: ControlShutter will either open or close the shutter. 
	 *
	 * Arguments: 
	 * USBHandler *pUSBHandler: Pointer to the USBHandler class. Pointer to the USBHandler class can be received by calling the StartCamera() method. 
	 * bool bOpen: true to open the shutter, false to close the shutter.
	 *
	 * Return Value:
	 * ControlShutter returns one of the values from RTNERRORS. On Success, ControlShutter returns NO_ERROR.
	 */

	int ControlShutter(bool bOpen);

	/*
	 * Description: PerformNUC will do non uniformity correction. 
	 *
	 * Arguments: 
	 * USBHandler *pUSBHandler: Pointer to the USBHandler class. Pointer to the USBHandler class can be received by calling the StartCamera() method. 
	 *
	 * Return Value:
	 * PerformNUC returns one of the values from RTNERRORS. On Success, PerformNUC returns NO_ERROR.
	 */
	int PerformNUC();

	/*
	 * Description: SaveX16Image that can be loaded in Windows using IR Flash software.
	 *
	 * Arguments
	 * USBHanlder *pUSBHandler: Pointer to the USBHandler class.
	 * const char *path: FullPath to store the file. 
	 */
	int SaveX16Image(const char *path);

	/*
	 * Description: SaveX16Image that can be loaded in Windows using IR Flash software.
	 *
	 * Arguments
	 * USBHanlder *pUSBHandler: Pointer to the USBHandler class.
	 * FILE *pFP: pointer to the FILE. User need to make sure to pass the open file pointer and need to close the file pointer.  
	 */
	int SaveX16Image(FILE *pFP);

	/*
	 * Description: GetRadiometricImage will get the raw Image from the Camera, Convert to Radiometric Data and store the data in the pRadiometricImage
	 * buffer. User have to allocate the memory for the radiometric Image. User should allocate memory to CameraResolution(Width*Height)*sizeof(float). 
	 * Also, user need to call the LoadCalibrationFile method once before calling this method. 
	 * 
	 * Arguments
	 * USBHandler *pUSBHandler: Pointer to the USBHandler class.
	 * float *pRadiometricImage: Pointer of float that store the radiometric data. 
	 * 
	 * Return Value:
	 * GetRadiometricImage returns one of the values from RTNERRORS. On Success, GetRadiometricImage returns NO_ERROR.
	 */
	int GetTemperatureImage(float *pRadiometricImage);

	/*
	 * Description: LoadCalibrationFile load the calibration file of the camera. Calibration File is necessary to convert Raw 14 data to radiometric
	 * data. Calibration Files should be of Cal<CameraSerialNumber>F.bin.
	 * 
	 * Arguments:
	 * USBHandler *pUSBHandler: Pointer to the USBHandler class.
	 * const char *path: Path of the folder that contains the calibration file. 
	 * 
	 * Return Value:
	 * LoadCalibrationFile returns one of the values from RTNERRORS. On Success, LoadCalibrationFile returns NO_ERROR.
	 */
	int LoadCalibrationFile(const char *path);


	/*
	 * GetError will return the error code in string format. 
	 */
	const char *GetError(int nEnumVal);

	/*
	 *GetVersion will return the version of the SDK
	 */
	static const char *GetVersion();



};



#endif
