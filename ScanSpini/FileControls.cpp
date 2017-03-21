/*
*
*	File: FileControls.cpp
*
*	Application: Posture Analyzer
*
*	Copyright (c) 2016-2017 Dorian Sharevich, Richard Voinov
*
*/

#include "FileControls.h"
#include <QtWidgets\qlayout.h>
#include <QtWidgets\qfiledialog.h>
#include <QtWidgets\qprogressdialog.h>
#include <fstream>
#include <Windows.h>
#include "Vector3D.h"
#include "LeftGLWindow.h"

using namespace std;

#define BUFFERSIZE 500000
DWORD g_BytesTransferred = 0;


FileControls::FileControls(Data3D *data3D, GraphicalControls *graphicalControls)
{
	this->data3D = data3D;
	this->graphicalControls = graphicalControls;
	this->setMinimumSize(800, 100);

	QLayout *fileControlsLayout;
	setLayout(fileControlsLayout = new QHBoxLayout);
	
	fileControlsLayout->addWidget(fileDataLabel = new QLabel);
	fileDataLabel->setMinimumWidth(40);
	fileDataLabel->setText("Data file:");
	fileDataLabel->setGeometry(0, 0, 400, 14);
		
	fileControlsLayout->addWidget(fileDataLineEdit = new QLineEdit);
	fileDataLineEdit->setMinimumWidth(450);
	//fileDataLineEdit->setText("C:\\DEVELOPMENT\\Data Files\\spina.stl");
#ifdef _DEBUG
	fileDataLineEdit->setText("C:\\DEVELOPMENT\\Data Files\\r4.stl");
#endif // _DEBUG
	//fileDataLineEdit->setText("C:\\DEVELOPMENT\\Data Files\\box_binary.stl");
	//fileDataLineEdit->setText("C:\\DEVELOPMENT\\Data Files\\detal_binary.stl");
	//fileDataLineEdit->setText("C:\\DEVELOPMENT\\Data Files\\3d-box-binary.stl");
	fileDataLineEdit->setGeometry(0, 0,300, 14);
	
	fileControlsLayout->addWidget(browsePushButton = new QPushButton);
	browsePushButton->setMinimumWidth(80);
	browsePushButton->setText("Browse");
	browsePushButton->setGeometry(0, 0, 80, 14);
	connect(browsePushButton, SIGNAL(released()), this, SLOT(handleBrowseButton()));

	fileControlsLayout->addWidget(startPushButton = new QPushButton);
	startPushButton->setMinimumWidth(80);
	startPushButton->setText("Start");
	startPushButton->setGeometry(0, 0, 80, 14);
	connect(startPushButton, SIGNAL(released()), this, SLOT(handleStartButton()));
}


void FileControls::handleBrowseButton()
{
	// change the text
	//browsePushButton->setText("Example");
	
	filePath = QFileDialog::getOpenFileName(
		this,
		tr("Open file"),
		"C:\\",
		"STL file (*.stl);;All file (*.*)"
		);

	fileDataLineEdit->setText(filePath);
}

VOID CALLBACK FileIOCompletionRoutine(
	__in  DWORD dwErrorCode,
	__in  DWORD dwNumberOfBytesTransfered,
	__in  LPOVERLAPPED lpOverlapped
);

VOID CALLBACK FileIOCompletionRoutine(
	__in  DWORD dwErrorCode,
	__in  DWORD dwNumberOfBytesTransfered,
	__in  LPOVERLAPPED lpOverlapped)
{
	//_tprintf(TEXT("Error code:\t%x\n"), dwErrorCode);
	//_tprintf(TEXT("Number of bytes:\t%x\n"), dwNumberOfBytesTransfered);
	g_BytesTransferred = dwNumberOfBytesTransfered;
}


void FileControls::handleStartButton()
{
	startPushButton->setText("Pushed");
	
	QString filePath = fileDataLineEdit->text();

	// save path in data3D to make output file name
	data3D->setInputFilePath(filePath);

	const unsigned int lengthPath = filePath.length();

	wchar_t filePathWChar[255];

	filePath.toWCharArray(filePathWChar);

	filePathWChar[lengthPath] = '\0';

	//readStlFile(filePathWChar);
	
	ReadInChunks(filePathWChar);
	//readStlFile(L"C:\\DEVELOPMENT\\Data Files\\box_binary.stl");

	// scheck if the file is exists



	// display progress bar

	// read file to memmory

	// process surface to 3d points
	

	// process surface to horizont sections

}




void FileControls::ReadInChunks(const WCHAR *pszFileName) 
{
	// Open the file that we want to map.
	HANDLE hFile = CreateFile(pszFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	// Create a file-mapping object for the file.
	HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_WRITECOPY,
		0, 0, NULL);
	// Map a copy-on-write view of the file; the system will commit
	// enough physical storage from the paging file to accommodate
	// the entire file. All pages in the view will initially have
	// PAGE_WRITECOPY access.
	PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_COPY,
		0, 0, 0);
	// Read a byte from the mapped view.
	BYTE bSomeByte = pbFile[0];
	// When reading, the system does not touch the committed pages in
	// the paging file. The page keeps its PAGE_WRITECOPY attribute.
	// Write a byte to the mapped view.

	unsigned long int currPosInBuffer = 0;

	float y = (float)pbFile[0];

	char *header = NULL;
	header = (char*)malloc(80);
	if (!header)
	{
		fprintf(stderr, "ERROR: Could not allocate space for destination\n");
		return;
	}
	memcpy(header, pbFile, 80);
	currPosInBuffer += 80;



	//const char *buffer = "foo bar baz...";
	unsigned long *numbOfTriangles = NULL;

	/* note we do not multiply by sizeof(char), which is unnecessary */
	/* we should cast the result, if we're in C++ */
	numbOfTriangles = (unsigned long *)malloc(4);

	/* error checking */
	if (!numbOfTriangles)
	{
		fprintf(stderr, "ERROR: Could not allocate space for destination\n");
		return;
	}

	/* copy bytes and terminate */
	memcpy(numbOfTriangles, pbFile + currPosInBuffer, 4);
	currPosInBuffer += 4;
	const int numbTriangl = *numbOfTriangles;

	//// Preparation for loop
	float *normVectX = NULL;
	float *normVectY = NULL;
	float *normVectZ = NULL;
	float *Vert1x = NULL;
	float *Vert1y = NULL;
	float *Vert1z = NULL;
	float *Vert2x = NULL;
	float *Vert2y = NULL;
	float *Vert2z = NULL;
	float *Vert3x = NULL;
	float *Vert3y = NULL;
	float *Vert3z = NULL;
	UINT16 *byteCount = NULL;		//UINT16 – Attribute byte coun

	int sizeOfFloat = sizeof(float);

	QProgressDialog progress("Load data form file...", NULL, 0, numbTriangl, this);
	progress.setWindowModality(Qt::WindowModal);
	//progress.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);
	progress.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);

	data3D->resetData();

	//// LOOP STARTS HERE
	for (int i = 0; i < numbTriangl; i++)
	{
		if (i % 4 == 0)
		{
			progress.setValue(i);
			if (progress.wasCanceled())
				break;

		}
		
		normVectX = (float *)malloc(4);
		normVectY = (float *)malloc(4);
		normVectZ = (float *)malloc(4);

		Vert1x = (float *)malloc(4);
		Vert1y = (float *)malloc(4);
		Vert1z = (float *)malloc(4);

		Vert2x = (float *)malloc(4);
		Vert2y = (float *)malloc(4);
		Vert2z = (float *)malloc(4);

		Vert3x = (float *)malloc(4);
		Vert3y = (float *)malloc(4);
		Vert3z = (float *)malloc(4);

		byteCount = (UINT16 *)malloc(2);

		if (!normVectX || !normVectY || !normVectZ ||
			!Vert1x || !Vert1y || !Vert1z ||
			!Vert2x || !Vert2y || !Vert2z ||
			!Vert3x || !Vert3y || !Vert3z ||
			!byteCount)
		{
			fprintf(stderr, "ERROR: Could not allocate space for destination\n");
			return;
		}

		memcpy(normVectX, pbFile + currPosInBuffer, 4);
		currPosInBuffer += 4;
		memcpy(normVectY, pbFile + currPosInBuffer, 4);
		currPosInBuffer += 4;
		memcpy(normVectZ, pbFile + currPosInBuffer, 4);
		currPosInBuffer += 4;

		memcpy(Vert1x, pbFile + currPosInBuffer, 4);
		currPosInBuffer += 4;
		memcpy(Vert1y, pbFile + currPosInBuffer, 4);
		currPosInBuffer += 4;
		memcpy(Vert1z, pbFile + currPosInBuffer, 4);
		currPosInBuffer += 4;

		memcpy(Vert2x, pbFile + currPosInBuffer, 4);
		currPosInBuffer += 4;
		memcpy(Vert2y, pbFile + currPosInBuffer, 4);
		currPosInBuffer += 4;
		memcpy(Vert2z, pbFile + currPosInBuffer, 4);
		currPosInBuffer += 4;

		memcpy(Vert3x, pbFile + currPosInBuffer, 4);
		currPosInBuffer += 4;
		memcpy(Vert3y, pbFile + currPosInBuffer, 4);
		currPosInBuffer += 4;
		memcpy(Vert3z, pbFile + currPosInBuffer, 4);
		currPosInBuffer += 4;

		/* copy bytes and terminate */
		memcpy(byteCount, pbFile + currPosInBuffer, 2);
		currPosInBuffer += 2;
		int byteCountInt = *byteCount;

		Vector3D *normVector = new Vector3D(*normVectX, *normVectY, *normVectZ);
		Vector3D *vertexVector1 = new Vector3D(*Vert1x, *Vert1y, *Vert1z);
		Vector3D *vertexVector2 = new Vector3D(*Vert2x, *Vert2y, *Vert2z);
		Vector3D *vertexVector3 = new Vector3D(*Vert3x, *Vert3y, *Vert3z);

		data3D->insertVector(*normVector);
		data3D->insertVector(*vertexVector1);
		data3D->insertVector(*vertexVector2);
		data3D->insertVector(*vertexVector3);

		//data3D->insertVectorPoints(*vertexVector1);
		//data3D->insertVectorPoints(*vertexVector2);
		//data3D->insertVectorPoints(*vertexVector3);
	}
	/* don't forget to free malloc'ed variables at the end of your program, to prevent memory leaks */
	free(header);
	free(numbOfTriangles);
	free(normVectX);
	free(normVectY);
	free(normVectZ);
	free(Vert1x);
	free(Vert1y);
	free(Vert1z);
	free(Vert2x);
	free(Vert2y);
	free(Vert2z);
	free(Vert3x);
	free(Vert3y);
	free(Vert3z);
	free(byteCount);



	/// REFACTORING to FOR LOOP

	// When writing for the first time, the system grabs a committed
	// page from the paging file, copies the original contents of the
	// page at the accessed memory address, and maps the new page
	// (the copy) into the process' address space. The new page has
	// an attribute of PAGE_READWRITE.
	// Write another byte to the mapped view.
	
	////pbFile[1] = 0;

	// Because this byte is now in a PAGE_READWRITE page, the system
	// simply writes the byte to the page (backed by the paging file).
	// When finished using the file's mapped view, unmap it.
	// UnmapViewOfFile is discussed in the next section.
	UnmapViewOfFile(pbFile);
	// The system decommits the physical storage from the paging file.
	// Any writes to the pages are lost.
	// Clean up after ourselves.
	CloseHandle(hFileMapping);
	CloseHandle(hFile);

		

	// Offsets must be a multiple of the system's allocation granularity.  We
	// guarantee this by making our view size equal to the allocation granularity.
	//SYSTEM_INFO sysinfo = { 0 };
	//::GetSystemInfo(&sysinfo);
	//DWORD cbView = sysinfo.dwAllocationGranularity;


	graphicalControls->reInitilizeLeftGLWindow();


	//settingsControls->enableMakeSectionsButton();
	//unsigned int points = data3D->getNumberOfPoints();
}



void FileControls::DisplayError(LPTSTR lpszFunction)
// Routine Description:
// Retrieve and output the system error message for the last-error code
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	lpDisplayBuf =
		(LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf)
			+ lstrlen((LPCTSTR)lpszFunction)
			+ 40) // account for format string
			* sizeof(TCHAR));

	//if (FAILED(StringCchPrintf((LPTSTR)lpDisplayBuf,
	//	LocalSize(lpDisplayBuf) / sizeof(TCHAR),
	//	TEXT("%s failed with error code %d as follows:\n%s"),
	//	lpszFunction,
	//	dw,
	//	lpMsgBuf)))
	//{
	//	printf("FATAL ERROR: Unable to output error code.\n");
	//}

	//_tprintf(TEXT("ERROR: %s\n"), (LPCTSTR)lpDisplayBuf);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}


FileControls::~FileControls()
{
}
