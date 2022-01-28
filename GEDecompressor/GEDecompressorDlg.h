// GEDecompressorDlg.h : header file
//

#pragma once
#include "GECompression.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "rnc_deco.h"
#include "mio0.h"
#include "yay0.h"
#include "yaz0.h"
#include "EDLCompress.h"
#include "CustomRoboCompress.h"
#include "ODTCompress.h"
#include "erzdec.h"
#include "BlitzDecoder.h"
#include "EASportsDecoder.h"
#include "SupermanDecoder.h"
#include "MonacoDecoder.h"
#include "n643docompression.h"
#include "BOFSDecoder.h"
#include "MKMythologiesDecode.h"
#include "FLA2Decoder.h"
#include "VPK0Decoder.h"
#include "n64_cic_nus_6105.h"
#include "BoltDecoder.h"
#include "H20Decoder.h"
#include "TetrisphereDecoder.h"
#include "CommandAndConquerDecoder.h"
#include "MidwayDecoder.h"
#include "MarioTennisDecoder.h"
#include "NintendoEncoder.h"
#include "NaganoDecoder.h"
#include "ViewpointDecoder.h"
#include "HexenDecoder.h"

#include <algorithm>

#define UPDATE_LIST (WM_APP + 1)
#define UPDATE_PROGRESS_BAR (WM_APP + 2)

struct ListUpdateStruct
{
	unsigned long address;
	int fileSizeUncompressed;
	int fileSizeCompressed;
	CString type;
	CString internalName;
	CString tempLocation;
};

// CGEDecompressorDlg dialog
class CGEDecompressorDlg : public CDialog
{
// Construction
public:
	CGEDecompressorDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	enum { IDD = IDD_GEDECOMPRESSOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT AddListEntry(WPARAM, LPARAM);
	afx_msg LRESULT UpdateProgressBar(WPARAM, LPARAM);
	afx_msg void OnBnClickedButton1();
	CComboBox m_gameselection;
	afx_msg void OnBnClickedCompressfilebutton();
	afx_msg void OnBnClickedDecompressgame();
	static UINT DecompressGameThread( LPVOID pParam );
	static void ToUpdateProgressBar(CGEDecompressorDlg* dlg, int curSpot, int romSize);
	static int GetZLibGameName(CString gameNameStr);
	static bool ReadROM(CString gameNameStr, CString fileLocation, unsigned char*& GameBuffer, int& romSize, CString& folderPath);
	static bool AllocateInput(int offset, unsigned char*& Buffer, unsigned char* GameBuffer, int& endSize, int romSize);
	static void WriteASCIIFile(CString filename, unsigned char* outputDecompressed, int fileSize, bool appendFile);
	static void WriteResult(bool genTextFile, CString filename, unsigned char* outputDecompressed, int fileSize, bool appendFile);
	static int DecompressZLibSpot(GECompression* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type, unsigned long printedAddress, bool printBank, unsigned printbankAddress);
	static int DecompressRNCSpot(RncDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type, bool appendFile, unsigned long appendROMLocation, CString appendInternalFileName);
	static int DecompressCommandConquerSpot(CommandAndConquerDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type, bool appendFile, unsigned long appendROMLocation, CString appendInternalFileName, int kind, unsigned long offsetHeader);
	static int DecompressMIO0Spot(MIO0* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type);
	static int DecompressNaganoSpot(CNaganoDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type);
	static int DecompressBOLTSpot(CBoltDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type, int bank, int entry);
	static int DecompressSupermanSpot(SupermanDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type);
	static int DecompressMonacoSpot(MonacoDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type);
	static int DecompressYAY0Spot(YAY0* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type);
	static int DecompressVPK0Spot(CVPK0Decoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type);
	static int DecompressERZSpot(ERZ* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type);
	static int DecompressYAZ0Spot(YAZ0* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type);
	static int DecompressEDLSpot(CEDLCompress* compressed, bool genText, int offset, FILE* in, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type);
	static int DecompressEAGamesSpot(EASportsDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type);
	static int DecompressCustomRoboSpot(CCustomRoboCompress* compressed, bool genText, int offset, FILE* in, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type);
	static int DecompressODTSpot(CODTCompress* compressed, bool genText, int offset, FILE* in, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type);
	static int DecompressFLA2Spot(CFLA2Decoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type);
	static int DecompressBlitzSpot(CBlitzDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type);
	static void WriteBinaryFile(CString filename, unsigned char* outputDecompressed, int fileSize, bool appendFile);
	void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
	static void DecompressZLibFromTable(CString gameNameStr, CGEDecompressorDlg* dlg, CString filein, unsigned long start, unsigned long end, int step, int GAME, unsigned long tblOffset, int shift, int multiplier, int offset);
	static void DecompressConkerFromTable(CGEDecompressorDlg* dlg, CString filein, unsigned char* input, int size, unsigned long start, unsigned long end, int GAME, bool writeFileNumberInstead, int bankNumber);
	static void DecryptBTFile(int fileNumber, unsigned char* input, unsigned char* output, int size);
	static void ReceivedNewROM(CGEDecompressorDlg* dlg, CString fileLocation, unsigned char* GameBuffer, int romSize);
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
	static float CharArrayToFloat(unsigned char* currentSpot);
	static unsigned long Flip32Bit(unsigned long inLong);
	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	static unsigned short Flip16Bit(unsigned short ShortValue);

	static unsigned long StringHexToLong(CString inString);
	static int HexToInt(char inChar);
	unsigned char StringToUnsignedChar(CString inString);

	static unsigned long GetSizeFile(CString filename);
	afx_msg void OnBnClickedButton3();
	CButton m_genTextFiles;
	CListCtrl m_list;
	int fileCount;
	static void AddRowData(CGEDecompressorDlg* dlg, unsigned long address,  int fileSizeCompressed, int fileSizeUncompressed, CString internalName, CString tempLocation, CString type);
	CProgressCtrl m_progressBar;
	afx_msg void OnLvnItemchangedListdecompressedfiles(NMHDR *pNMHDR, LRESULT *pResult);
	unsigned char ReturnASCIIChar(unsigned char asciiByte);
	CString strROMPath;
	CString gameNameStr;
	bool genText;
	CString directory;
	CWinThread* decompressGamethread;
	bool killThread;
	afx_msg void OnBnClickedButtoncancelload();
	void KillDecompressGameThread();
	CButton m_cancelLoad;
	CButton m_injectButton;
	unsigned char* ROM;
	int ROMSize;
	CString romName;
	~CGEDecompressorDlg(void);
	CButton m_saveROMButton;
	afx_msg void OnBnClickedButtonsaverom();
	afx_msg void OnClose();
	BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory);
	BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory, int& errorCode);
	BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory, int& errorCode, int waitTime);
	CEdit m_hexView;
	CEdit mSearch;
	afx_msg void OnBnClickedButtonsearch();
	int lastSearchSpot;
	CEdit mFile;
	CButton mCompressEncryptedButton;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedCompressfilebuttonencrypted();
	CEdit mEncryptedFileNumber;
	unsigned short StringToUnsignedShort(CString inString);
	CStatic mFileNumberStatic;
	CButton mCompressFileButton;
	CButton mDecompressFileButton;
};
