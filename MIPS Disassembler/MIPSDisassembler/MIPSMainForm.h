
#pragma once
#include "DisassemblerConfiguration.h"
#include "ReadXMLConfiguration.h"
//#include "IInstructionSet.h"
#include "ProcessorChoiceDlg.h"
#include "ResultsWindowControl.h"
#include "DisassemblyContents.h"
#include "DisassemblerSharedFunctions.h"
#include "ProgressBarDlg.h"
#include "InputDlg.h"
#include "DisassembleSerialized.h"
#include "FindDlg.h"
#include "GotoAddressDlg.h"


#using <mscorlib.dll>
using namespace System::Runtime::InteropServices; 
using namespace System;
using namespace System::IO;
using System::Runtime::Serialization::Formatters::Binary::BinaryFormatter;
using System::Collections::Generic::Dictionary;
// for DllImportAttribute

namespace SysWin32
{
    [DllImport("user32.dll")]
      extern bool LockWindowUpdate(IntPtr hWndLock);

	[DllImport("user32.dll")] 
	extern int SendMessage(IntPtr hWnd, int Msg, int wParam, int lParam);  

	[DllImport("user32.dll")] 
	extern int GetScrollPos(IntPtr hWnd, int nBar);

	[DllImport("user32.dll")] 
	int SetScrollPos(IntPtr hWnd, int nBar, int nPos, bool bRedraw);

}


using MIPSDisassembler::MIPSDisassemblerControlsControl;

namespace MIPSDisassembler {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Reflection;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MIPSMainForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MIPSMainForm : public System::Windows::Forms::Form
	{
	private:
		disassembler^ configFile;
			 int numberLines;
			 System::Drawing::Size sizeChars;


	List<MIPSDisassembler::MIPSDisassemblerJoinedControl^>^  mipsDisassemblerJoinedControlsList;
	List<MIPSDisassembler::SubroutinesControl^>^  mipsDisassemblerSubroutineControlsList;
	List<MIPSDisassembler::LabelsControl^>^  mipsDisassemblerLabelControlsList;
	List<MIPSDisassembler::FindAllControl^>^ mipsDisassemblerFindAllControlsList;
	 System::Drawing::Point^ lastLoc;

				System::Drawing::Font^ fontUsed;

	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStripTabHeaderRightClick;
	private: System::Windows::Forms::ToolStripMenuItem^  closeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveDisassemblyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  closeDisassemblyToolStripMenuItem;


	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStripRichTextBox;
	private: System::Windows::Forms::ToolStripMenuItem^  cutToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  copyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  pasteToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  renameAddressToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  gotoXREFToolStripMenuItem;
















	List<DisassemblyContents^>^ disassemblyContentsArray;

	int foundIndex;
	String^ foundWord;
	private: System::Windows::Forms::TabControl^  tabControlViews;
	private: System::Windows::Forms::TabPage^  tabPageSubRoutines;
	private: System::Windows::Forms::TabPage^  tabPageLabels;
	private: System::Windows::Forms::TabPage^  tabPageFindAll;
	private: System::Windows::Forms::ToolStripMenuItem^  gotoAddressToolStripMenuItem;
	private: System::Windows::Forms::TabControl^  tabControlDisassembles;
	private: System::Windows::Forms::ToolTip^  toolTipRichTextBox;
	private: System::Windows::Forms::ToolStripMenuItem^  exportDisassemblyTotxtToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripMenuItem^  exportFindResultsTotxtToolStripMenuItem;



			 System::Windows::Forms::RichTextBoxFinds richTextBoxFinds;
		
	public:
		MIPSMainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MIPSMainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuMain;
	protected: 
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  newDisassemblyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openDisassemblyToolStripMenuItem;
	private: System::Windows::Forms::SplitContainer^  splitContainerMain;
	private: System::Windows::Forms::SplitContainer^  splitContainerBottom;
	private: System::ComponentModel::IContainer^  components;

			 
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->menuMain = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->newDisassemblyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->saveDisassemblyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openDisassemblyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exportDisassemblyTotxtToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->closeDisassemblyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->splitContainerMain = (gcnew System::Windows::Forms::SplitContainer());
			this->tabControlDisassembles = (gcnew System::Windows::Forms::TabControl());
			this->splitContainerBottom = (gcnew System::Windows::Forms::SplitContainer());
			this->tabControlViews = (gcnew System::Windows::Forms::TabControl());
			this->tabPageSubRoutines = (gcnew System::Windows::Forms::TabPage());
			this->tabPageLabels = (gcnew System::Windows::Forms::TabPage());
			this->tabPageFindAll = (gcnew System::Windows::Forms::TabPage());
			this->contextMenuStripTabHeaderRightClick = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->closeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuStripRichTextBox = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->cutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->copyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pasteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->renameAddressToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->gotoXREFToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->gotoAddressToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolTipRichTextBox = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->exportFindResultsTotxtToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuMain->SuspendLayout();
			this->splitContainerMain->Panel1->SuspendLayout();
			this->splitContainerMain->Panel2->SuspendLayout();
			this->splitContainerMain->SuspendLayout();
			this->splitContainerBottom->Panel2->SuspendLayout();
			this->splitContainerBottom->SuspendLayout();
			this->tabControlViews->SuspendLayout();
			this->contextMenuStripTabHeaderRightClick->SuspendLayout();
			this->contextMenuStripRichTextBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuMain
			// 
			this->menuMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fileToolStripMenuItem});
			this->menuMain->Location = System::Drawing::Point(0, 0);
			this->menuMain->Name = L"menuMain";
			this->menuMain->Padding = System::Windows::Forms::Padding(5, 2, 0, 2);
			this->menuMain->Size = System::Drawing::Size(1037, 28);
			this->menuMain->TabIndex = 0;
			this->menuMain->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {this->newDisassemblyToolStripMenuItem, 
				this->toolStripSeparator2, this->saveDisassemblyToolStripMenuItem, this->openDisassemblyToolStripMenuItem, this->toolStripSeparator3, 
				this->exportDisassemblyTotxtToolStripMenuItem, this->exportFindResultsTotxtToolStripMenuItem, this->closeDisassemblyToolStripMenuItem, 
				this->toolStripSeparator1, this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(44, 24);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// newDisassemblyToolStripMenuItem
			// 
			this->newDisassemblyToolStripMenuItem->Name = L"newDisassemblyToolStripMenuItem";
			this->newDisassemblyToolStripMenuItem->Size = System::Drawing::Size(249, 24);
			this->newDisassemblyToolStripMenuItem->Text = L"New Disassembly";
			this->newDisassemblyToolStripMenuItem->Click += gcnew System::EventHandler(this, &MIPSMainForm::newDisassemblyToolStripMenuItem_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(246, 6);
			// 
			// saveDisassemblyToolStripMenuItem
			// 
			this->saveDisassemblyToolStripMenuItem->Name = L"saveDisassemblyToolStripMenuItem";
			this->saveDisassemblyToolStripMenuItem->Size = System::Drawing::Size(249, 24);
			this->saveDisassemblyToolStripMenuItem->Text = L"Save Disassembly";
			this->saveDisassemblyToolStripMenuItem->Click += gcnew System::EventHandler(this, &MIPSMainForm::saveDisassemblyToolStripMenuItem_Click);
			// 
			// openDisassemblyToolStripMenuItem
			// 
			this->openDisassemblyToolStripMenuItem->Name = L"openDisassemblyToolStripMenuItem";
			this->openDisassemblyToolStripMenuItem->Size = System::Drawing::Size(249, 24);
			this->openDisassemblyToolStripMenuItem->Text = L"Open Disassembly";
			this->openDisassemblyToolStripMenuItem->Click += gcnew System::EventHandler(this, &MIPSMainForm::openDisassemblyToolStripMenuItem_Click);
			// 
			// toolStripSeparator3
			// 
			this->toolStripSeparator3->Name = L"toolStripSeparator3";
			this->toolStripSeparator3->Size = System::Drawing::Size(246, 6);
			// 
			// exportDisassemblyTotxtToolStripMenuItem
			// 
			this->exportDisassemblyTotxtToolStripMenuItem->Name = L"exportDisassemblyTotxtToolStripMenuItem";
			this->exportDisassemblyTotxtToolStripMenuItem->Size = System::Drawing::Size(249, 24);
			this->exportDisassemblyTotxtToolStripMenuItem->Text = L"Export Disassembly to .txt";
			this->exportDisassemblyTotxtToolStripMenuItem->Click += gcnew System::EventHandler(this, &MIPSMainForm::exportDisassemblyTotxtToolStripMenuItem_Click);
			// 
			// closeDisassemblyToolStripMenuItem
			// 
			this->closeDisassemblyToolStripMenuItem->Name = L"closeDisassemblyToolStripMenuItem";
			this->closeDisassemblyToolStripMenuItem->Size = System::Drawing::Size(249, 24);
			this->closeDisassemblyToolStripMenuItem->Text = L"Close Disassembly";
			this->closeDisassemblyToolStripMenuItem->Click += gcnew System::EventHandler(this, &MIPSMainForm::closeDisassemblyToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(246, 6);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(249, 24);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MIPSMainForm::exitToolStripMenuItem_Click);
			// 
			// splitContainerMain
			// 
			this->splitContainerMain->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerMain->Location = System::Drawing::Point(0, 28);
			this->splitContainerMain->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->splitContainerMain->Name = L"splitContainerMain";
			this->splitContainerMain->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainerMain.Panel1
			// 
			this->splitContainerMain->Panel1->Controls->Add(this->tabControlDisassembles);
			// 
			// splitContainerMain.Panel2
			// 
			this->splitContainerMain->Panel2->Controls->Add(this->splitContainerBottom);
			this->splitContainerMain->Size = System::Drawing::Size(1037, 774);
			this->splitContainerMain->SplitterDistance = 423;
			this->splitContainerMain->TabIndex = 1;
			// 
			// tabControlDisassembles
			// 
			this->tabControlDisassembles->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControlDisassembles->Location = System::Drawing::Point(0, 0);
			this->tabControlDisassembles->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabControlDisassembles->Name = L"tabControlDisassembles";
			this->tabControlDisassembles->SelectedIndex = 0;
			this->tabControlDisassembles->Size = System::Drawing::Size(1037, 423);
			this->tabControlDisassembles->TabIndex = 1;
			this->tabControlDisassembles->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MIPSMainForm::tabControlDisassembles_MouseUp);
			this->tabControlDisassembles->SelectedIndexChanged += gcnew System::EventHandler(this, &MIPSMainForm::tabControlDisassembles_SelectedIndexChanged);
			// 
			// splitContainerBottom
			// 
			this->splitContainerBottom->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerBottom->Location = System::Drawing::Point(0, 0);
			this->splitContainerBottom->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->splitContainerBottom->Name = L"splitContainerBottom";
			// 
			// splitContainerBottom.Panel2
			// 
			this->splitContainerBottom->Panel2->Controls->Add(this->tabControlViews);
			this->splitContainerBottom->Size = System::Drawing::Size(1037, 347);
			this->splitContainerBottom->SplitterDistance = 514;
			this->splitContainerBottom->TabIndex = 0;
			// 
			// tabControlViews
			// 
			this->tabControlViews->Controls->Add(this->tabPageSubRoutines);
			this->tabControlViews->Controls->Add(this->tabPageLabels);
			this->tabControlViews->Controls->Add(this->tabPageFindAll);
			this->tabControlViews->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControlViews->Location = System::Drawing::Point(0, 0);
			this->tabControlViews->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabControlViews->Name = L"tabControlViews";
			this->tabControlViews->SelectedIndex = 0;
			this->tabControlViews->Size = System::Drawing::Size(519, 347);
			this->tabControlViews->TabIndex = 0;
			// 
			// tabPageSubRoutines
			// 
			this->tabPageSubRoutines->Location = System::Drawing::Point(4, 25);
			this->tabPageSubRoutines->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabPageSubRoutines->Name = L"tabPageSubRoutines";
			this->tabPageSubRoutines->Size = System::Drawing::Size(511, 318);
			this->tabPageSubRoutines->TabIndex = 0;
			this->tabPageSubRoutines->Text = L"Subroutines";
			this->tabPageSubRoutines->UseVisualStyleBackColor = true;
			// 
			// tabPageLabels
			// 
			this->tabPageLabels->Location = System::Drawing::Point(4, 25);
			this->tabPageLabels->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabPageLabels->Name = L"tabPageLabels";
			this->tabPageLabels->Size = System::Drawing::Size(511, 314);
			this->tabPageLabels->TabIndex = 1;
			this->tabPageLabels->Text = L"Labels";
			this->tabPageLabels->UseVisualStyleBackColor = true;
			// 
			// tabPageFindAll
			// 
			this->tabPageFindAll->Location = System::Drawing::Point(4, 25);
			this->tabPageFindAll->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tabPageFindAll->Name = L"tabPageFindAll";
			this->tabPageFindAll->Size = System::Drawing::Size(511, 314);
			this->tabPageFindAll->TabIndex = 3;
			this->tabPageFindAll->Text = L"Find All";
			this->tabPageFindAll->UseVisualStyleBackColor = true;
			// 
			// contextMenuStripTabHeaderRightClick
			// 
			this->contextMenuStripTabHeaderRightClick->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->closeToolStripMenuItem});
			this->contextMenuStripTabHeaderRightClick->Name = L"contextMenuStripTabHeaderRightClick";
			this->contextMenuStripTabHeaderRightClick->Size = System::Drawing::Size(142, 28);
			// 
			// closeToolStripMenuItem
			// 
			this->closeToolStripMenuItem->Name = L"closeToolStripMenuItem";
			this->closeToolStripMenuItem->Size = System::Drawing::Size(141, 24);
			this->closeToolStripMenuItem->Text = L"Close Tab";
			this->closeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MIPSMainForm::closeToolStripMenuItem_Click);
			// 
			// contextMenuStripRichTextBox
			// 
			this->contextMenuStripRichTextBox->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->cutToolStripMenuItem, 
				this->copyToolStripMenuItem, this->pasteToolStripMenuItem, this->renameAddressToolStripMenuItem, this->gotoXREFToolStripMenuItem, 
				this->gotoAddressToolStripMenuItem});
			this->contextMenuStripRichTextBox->Name = L"contextMenuStripRichTextBox";
			this->contextMenuStripRichTextBox->Size = System::Drawing::Size(190, 148);
			// 
			// cutToolStripMenuItem
			// 
			this->cutToolStripMenuItem->Name = L"cutToolStripMenuItem";
			this->cutToolStripMenuItem->Size = System::Drawing::Size(189, 24);
			this->cutToolStripMenuItem->Text = L"Cut";
			this->cutToolStripMenuItem->Click += gcnew System::EventHandler(this, &MIPSMainForm::cutToolStripMenuItem_Click);
			// 
			// copyToolStripMenuItem
			// 
			this->copyToolStripMenuItem->Name = L"copyToolStripMenuItem";
			this->copyToolStripMenuItem->Size = System::Drawing::Size(189, 24);
			this->copyToolStripMenuItem->Text = L"Copy";
			this->copyToolStripMenuItem->Click += gcnew System::EventHandler(this, &MIPSMainForm::copyToolStripMenuItem_Click);
			// 
			// pasteToolStripMenuItem
			// 
			this->pasteToolStripMenuItem->Name = L"pasteToolStripMenuItem";
			this->pasteToolStripMenuItem->Size = System::Drawing::Size(189, 24);
			this->pasteToolStripMenuItem->Text = L"Paste";
			this->pasteToolStripMenuItem->Click += gcnew System::EventHandler(this, &MIPSMainForm::pasteToolStripMenuItem_Click);
			// 
			// renameAddressToolStripMenuItem
			// 
			this->renameAddressToolStripMenuItem->Name = L"renameAddressToolStripMenuItem";
			this->renameAddressToolStripMenuItem->Size = System::Drawing::Size(189, 24);
			this->renameAddressToolStripMenuItem->Text = L"Rename Address";
			this->renameAddressToolStripMenuItem->Click += gcnew System::EventHandler(this, &MIPSMainForm::renameAddressToolStripMenuItem_Click);
			// 
			// gotoXREFToolStripMenuItem
			// 
			this->gotoXREFToolStripMenuItem->Name = L"gotoXREFToolStripMenuItem";
			this->gotoXREFToolStripMenuItem->Size = System::Drawing::Size(189, 24);
			this->gotoXREFToolStripMenuItem->Text = L"Goto XREF";
			this->gotoXREFToolStripMenuItem->Click += gcnew System::EventHandler(this, &MIPSMainForm::gotoXREFToolStripMenuItem_Click);
			// 
			// gotoAddressToolStripMenuItem
			// 
			this->gotoAddressToolStripMenuItem->Name = L"gotoAddressToolStripMenuItem";
			this->gotoAddressToolStripMenuItem->Size = System::Drawing::Size(189, 24);
			this->gotoAddressToolStripMenuItem->Text = L"Goto Address";
			this->gotoAddressToolStripMenuItem->Click += gcnew System::EventHandler(this, &MIPSMainForm::gotoAddressToolStripMenuItem_Click);
			// 
			// exportFindResultsTotxtToolStripMenuItem
			// 
			this->exportFindResultsTotxtToolStripMenuItem->Name = L"exportFindResultsTotxtToolStripMenuItem";
			this->exportFindResultsTotxtToolStripMenuItem->Size = System::Drawing::Size(249, 24);
			this->exportFindResultsTotxtToolStripMenuItem->Text = L"Export Find Results to .txt";
			this->exportFindResultsTotxtToolStripMenuItem->Click += gcnew System::EventHandler(this, &MIPSMainForm::exportFindResultsTotxtToolStripMenuItem_Click);
			// 
			// MIPSMainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1037, 802);
			this->Controls->Add(this->splitContainerMain);
			this->Controls->Add(this->menuMain);
			this->MainMenuStrip = this->menuMain;
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"MIPSMainForm";
			this->Text = L"Disassembler by SubDrag (goldeneyevault.com) Sept. 6, 2011";
			this->Load += gcnew System::EventHandler(this, &MIPSMainForm::MIPSMainForm_Load);
			this->menuMain->ResumeLayout(false);
			this->menuMain->PerformLayout();
			this->splitContainerMain->Panel1->ResumeLayout(false);
			this->splitContainerMain->Panel2->ResumeLayout(false);
			this->splitContainerMain->ResumeLayout(false);
			this->splitContainerBottom->Panel2->ResumeLayout(false);
			this->splitContainerBottom->ResumeLayout(false);
			this->tabControlViews->ResumeLayout(false);
			this->contextMenuStripTabHeaderRightClick->ResumeLayout(false);
			this->contextMenuStripRichTextBox->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MIPSMainForm_Load(System::Object^  sender, System::EventArgs^  e) {
		foundIndex = 0;
		foundWord = "";
		disassemblyContentsArray = gcnew List<DisassemblyContents^>();
		mipsDisassemblerJoinedControlsList = gcnew List<MIPSDisassembler::MIPSDisassemblerJoinedControl^>();
		mipsDisassemblerSubroutineControlsList = gcnew List<MIPSDisassembler::SubroutinesControl^>();
		mipsDisassemblerLabelControlsList = gcnew List<MIPSDisassembler::LabelsControl^>();
		mipsDisassemblerFindAllControlsList = gcnew List<MIPSDisassembler::FindAllControl^>();

		String^ appPathString = (Assembly::GetEntryAssembly()->Location); 
		FileInfo^ fileInfo = gcnew FileInfo(appPathString); 
		appPathString = fileInfo->DirectoryName + "\\";

		configFile = nullptr;
		try
		{
			if (System::IO::File::Exists(appPathString + "DisassemblerConfiguration.xml"))
			{
				configFile = ReadXMLConfiguration::ReadDisassembler(appPathString + "DisassemblerConfiguration.xml");
			}
			else
			{
				if (appPathString->Substring(0, appPathString->Length-1)->LastIndexOf('\\') != -1)
				{
					appPathString = appPathString->Substring(0, (appPathString->Substring(0, appPathString->Length-1)->LastIndexOf('\\') + 1));
					configFile = ReadXMLConfiguration::ReadDisassembler(appPathString + "DisassemblerConfiguration.xml");
				}
				else
				{
					MessageBox::Show("Config file not found");
					Close();
				}
			}
		}
		catch (Exception^ e)
		{
			MessageBox::Show(e->ToString(), "Error processing DisassemblyConfiguration.xml");
			configFile = nullptr;
		}
	 }

		bool InitializeBinary(String^ binaryFilename, array<unsigned char>^% binary, long% binaryLength)
		{
		 
			  for (int x = 0; x < disassemblyContentsArray->Count; x++)
			  {
				  for (int y = 0; y < disassemblyContentsArray[x]->binaryData->Count; y++)
				  {
					  if (binaryFilename->Equals(disassemblyContentsArray[x]->binaryData[y]->filename))
					  {
						  tabControlDisassembles->SelectedIndex = x;
						  return false;
					  }
				  }
			  }
			FileStream^ fs = nullptr;
			try
			{

				fs = gcnew FileStream(binaryFilename, FileMode::Open, System::IO::FileAccess::Read);
			  BinaryReader^ br = gcnew BinaryReader(fs);
			  binary = br->ReadBytes(fs->Length);
			  binaryLength = fs->Length;

			  if (((binaryLength % 4) != 0) || (binaryLength == 0))
			  {
				  MessageBox::Show("Invalid binary length");
				  return false;
			  }

			  

			}
			catch (Exception^ e)
			{
			  if (dynamic_cast<FileNotFoundException^>(e))
				  MessageBox::Show(String::Format("File '{0}' not found", binaryFilename));
			  else
				 MessageBox::Show(String::Format("Exception: ({0})", e));
			  return false;
		   }
			finally
			{
				if (fs != nullptr)
				{
					fs->Close();
				}
			}

			 if ((configFile == nullptr) || (configFile->tab == nullptr) || (configFile->processor == nullptr)
						 || (configFile->tab->Count == 0) || (configFile->processor->Rows->Count == 0))
					 {
						 MessageBox::Show("No disassemblers present, cannot disassemble");
						 return false;
					 }

			return true;
		}

private: System::Void newDisassemblyToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			  ProcessorChoiceDlg^ dlg = gcnew ProcessorChoiceDlg(configFile);
			 if (dlg->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			 {
				MIPSDisassembler::disassembler::processorRow^  procRow = dlg->ProcessorReturnRow;
				unsigned long processorStartAddress = dlg->ProcessorReturnStartAddress;
				
				IInstructionSet^ instructionSet = DisassemblerSharedFunctions::ActivateInstructionSet(procRow->DLL, procRow->_class);					
				if (instructionSet == nullptr)
				{

					MessageBox::Show("Error starting processor ");
					return;
				}

					OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
			 
				  openFileDialog->Filter = "bin files (*.bin)|*.bin|All files (*.*)|*.*";
				  openFileDialog->FilterIndex = 1;
				  openFileDialog->FileName = "PD8Meg.bin";
				  openFileDialog->RestoreDirectory = true;
				  openFileDialog->Multiselect = false;

				   if ((openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) && (openFileDialog->FileName != "") && (openFileDialog->FileName != nullptr))
					{
						OpenFileDialog^ openFileDialogTlb = gcnew OpenFileDialog();
			 
					  openFileDialogTlb->Filter = "Tlb bin files (*.bin)|*.bin|All files (*.*)|*.*";
					  openFileDialogTlb->FilterIndex = 1;
					  openFileDialogTlb->FileName = "PDTLB.bin";
					  openFileDialogTlb->RestoreDirectory = true;
					  openFileDialogTlb->Multiselect = false;

					   if (dlg->checkBoxTLB->Checked)
					   {
							if (!((openFileDialogTlb->ShowDialog() == System::Windows::Forms::DialogResult::OK) && (openFileDialogTlb->FileName != "") && (openFileDialogTlb->FileName != nullptr)))
							{
								return;
							}
					   }


					  array<unsigned char>^ binary = nullptr;
					  long binaryLength = 0;
					  if (!InitializeBinary(openFileDialog->FileName, binary, binaryLength))
						  return;

					  List<DisassembledFileDetails^>^ disassemblyDetails = gcnew List<DisassembledFileDetails^>();
					  disassemblyDetails->Add(gcnew DisassembledFileDetails(openFileDialog->FileName, dlg->ProcessorReturnStartAddress, binaryLength, binary));


					  if (dlg->checkBoxTLB->Checked)
					  {
						  array<unsigned char>^ binaryTLB = nullptr;
						  long binaryLengthTLB = 0;
						  if (!InitializeBinary(openFileDialogTlb->FileName, binaryTLB, binaryLengthTLB))
							  return;

						  disassemblyDetails->Insert(0, gcnew DisassembledFileDetails(openFileDialogTlb->FileName, dlg->ProcessorReturnStartAddressTlb, binaryLengthTLB, binaryTLB));
					  }

						 

					DisassemblyContents^ disassemblyContents = gcnew DisassemblyContents();
					// call on new thread
					disassemblyContents->SetDisassemblyVariables(instructionSet, disassemblyDetails, true);

					ProgressBarDlg^ processBarDlg = gcnew ProgressBarDlg(gcnew ProgressRunDelegate(disassemblyContents, &MIPSDisassembler::DisassemblyContents::Disassemble), gcnew GetProgressDelegate(disassemblyContents, &MIPSDisassembler::DisassemblyContents::GetProgress));
						if (processBarDlg->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					{
						AddTab(disassemblyContents);
					}
				}
			}
		 }

		bool MIPSMainForm::IsCommentSpot(MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl)
		{
			if (mipsDisassemblerControl->SelectionStart != -1)
			{
				int start = mipsDisassemblerControl->SelectionStart;
				int length = mipsDisassemblerControl->SelectionLength;

				int lineStart = mipsDisassemblerControl->GetLineFromCharIndex(start);
				int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart);

				int lineEnd = mipsDisassemblerControl->GetLineFromCharIndex(start + length - 1);
				int charEnd = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineEnd);
				
				int charNext = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineEnd + 1);

				String^ currentText = mipsDisassemblerControl->SelectedText;
				if (((currentText->Contains("\r\n")) || (currentText->Contains("\n")) || (currentText->Contains("\r")))
					|| ((charNext == -1) && ((start + length) >= (mipsDisassemblerControl->Text->Length))))
					return false;

				if (lineStart.Equals(lineEnd))
				{
					if (charNext == -1)
					{
						charNext = charStart + mipsDisassemblerControl->Lines[lineStart]->Length - 1;
					}

					try
					{

						// TODO save to instructions
						SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);
						mipsDisassemblerControl->Select(charStart, 100);
						String^ text = mipsDisassemblerControl->SelectedText;
						
						if (text->StartsWith("."))
						{
							return false;
						}

						int poundOffset = GetPoundOffset(mipsDisassemblerControl, text, charNext - charStart);

						if (poundOffset == -1)
							return false;

						if (start > (charStart + poundOffset))
						{
							// editable
							return true;
						}
					}
					finally
					{
						mipsDisassemblerControl->Select(start, length);
						SysWin32::LockWindowUpdate(IntPtr::Zero);
					}
					
				}
			}
			return false;
		}

		int MIPSMainForm::GetPoundOffset(MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl, String^ text, int length)
		{
			int poundOffset = -1;
			for (int x = 0; x < length; x++)
			{
				if (text->Substring(x, 1) == "#")
				{
					poundOffset = x;
					break;				
				}
			}
			return poundOffset;
		}

		bool MIPSMainForm::InsertNewLine(MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl)
		{
			if (mipsDisassemblerControl->SelectionStart != -1)
			{
				int start = mipsDisassemblerControl->SelectionStart;
				int length = mipsDisassemblerControl->SelectionLength;
				
				int lineStart = mipsDisassemblerControl->GetLineFromCharIndex(start);
				int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart);

				int lineEnd = mipsDisassemblerControl->GetLineFromCharIndex(start + length - 1);
				int charEnd = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineEnd);

				int charNext = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineEnd + 1);
				
				if (((mipsDisassemblerControl->SelectedText->Contains("\r\n")) || (mipsDisassemblerControl->SelectedText->Contains("\n")) || (mipsDisassemblerControl->SelectedText->Contains("\r")))
					|| ((charNext == -1) && ((start + length) >= (mipsDisassemblerControl->Text->Length - 1))))
					return true;

				if (lineStart.Equals(lineEnd))
				{
					try
					{
						// TODO save to instructions
						SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);

						mipsDisassemblerControl->SelectedText = "";
						charNext = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineEnd + 1);
						if (charNext == -1)
						{
							charNext = charStart + mipsDisassemblerControl->Lines[lineEnd]->Length - 1;
						}

						mipsDisassemblerControl->Select(start, (charNext - start - 1));
						String^ textToPaste = mipsDisassemblerControl->SelectedText;
						mipsDisassemblerControl->SelectedText = "";

						charNext = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineEnd + 1);
						mipsDisassemblerControl->Select(charStart, (charNext - charStart - 1));
						String^ textCurrent = mipsDisassemblerControl->SelectedText;

						int poundOffset = GetPoundOffset(mipsDisassemblerControl, textCurrent, (charNext - charStart - 1));

						String^ address = textCurrent->Substring(0, 8);
						mipsDisassemblerControl->Select(charNext, 0);
						String^ newText = address + ":\t\t\t#" + textToPaste + "\r\n";
						mipsDisassemblerControl->SelectedText = newText;

						disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[GetAbsoluteLineNumber(lineStart)] =  GetStringFromLine(lineStart, mipsDisassemblerControl);
						disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Insert((mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Value + lineStart + 1), newText);

						mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Maximum++;
						
						return true;
					}
					finally
					{
						int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineEnd + 1);
						mipsDisassemblerControl->Select(charStart, 100);
						String^ textCurrent = mipsDisassemblerControl->SelectedText;

						int poundOffset = GetPoundOffset(mipsDisassemblerControl, textCurrent, 100);

						mipsDisassemblerControl->Select(charStart + poundOffset + 1, 0);
						SysWin32::LockWindowUpdate(IntPtr::Zero);
					}
				}
			}
			return false;
		}

		bool MIPSMainForm::BackspaceLine(MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl)
		{
			if (mipsDisassemblerControl->SelectionStart != -1)
			{
				int start = mipsDisassemblerControl->SelectionStart;
				int length = mipsDisassemblerControl->SelectionLength;

				int lineStart = mipsDisassemblerControl->GetLineFromCharIndex(start);
				int charNext = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart + 1);

				if (((mipsDisassemblerControl->SelectedText->Contains("\r\n")) || (mipsDisassemblerControl->SelectedText->Contains("\n")) || (mipsDisassemblerControl->SelectedText->Contains("\r")))
					|| ((charNext == -1) && ((start + length) >= (mipsDisassemblerControl->Text->Length - 1))))
					return true;

				// if more than one, just delete and return
				if (length > 0)
				{
					return false;
				}

				
				int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart);

				if (charNext == -1)
				{
					charNext = charStart + mipsDisassemblerControl->Lines[lineStart]->Length - 1;
				}

				if (lineStart == 0) // need to see above line
					return true;

				int lineEnd = mipsDisassemblerControl->GetLineFromCharIndex(start + length - 1);
				int charEnd = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineEnd);
				
				

				if (lineStart.Equals(lineEnd))
				{
					try
					{
						SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);

						int charPrev = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineEnd - 1);
						
						// TODO save to instructions
						
						mipsDisassemblerControl->Select(charStart, (charNext - charStart));
						String^ textCurrent = mipsDisassemblerControl->SelectedText;

						int poundOffset = GetPoundOffset(mipsDisassemblerControl, textCurrent, (charNext - charStart));

						if ((start - charStart - 1) != poundOffset)
						{
							mipsDisassemblerControl->Select(start, length);
							return false;
						}

						mipsDisassemblerControl->Select(charPrev, (charStart - charPrev));
						String^ textPrev = mipsDisassemblerControl->SelectedText;

						int poundOffsetPrev = GetPoundOffset(mipsDisassemblerControl, textPrev, (charStart - charPrev));
						if (poundOffsetPrev == -1)
						{
							mipsDisassemblerControl->Select(start, length);
							return true;
						}

						if (textPrev->StartsWith("."))
						{
							return true;
						}

						String^ address = textCurrent->Substring(0, 8);
						String^ addressPrev = textPrev->Substring(0, 8);

						if (address->Equals(addressPrev))
						{
							mipsDisassemblerControl->Select((charStart - 1), poundOffset + 2);
							mipsDisassemblerControl->SelectedText = "";

							int oldSize = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[GetAbsoluteLineNumber(lineStart - 1)]->Length;
							disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[GetAbsoluteLineNumber(lineStart - 1)] = GetStringFromLine((lineStart - 1), mipsDisassemblerControl);
							disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->RemoveAt(GetAbsoluteLineNumber(lineStart));
							if (mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Value > 0)
								mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Value = mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Value - 1;
							DrawRichTextBoxData(mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex], mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Value);

							mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Maximum--;

							int charPrev = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineEnd-1);
							mipsDisassemblerControl->Select((charPrev + oldSize-1), 0);
							return true;
						}
						else if ((start - charStart - 1) == poundOffset)
						{
							mipsDisassemblerControl->Select(start, length);
							return true;
						}
						else
						{
							mipsDisassemblerControl->Select(start, length);
							return false;
						}
					}
					finally
					{
						SysWin32::LockWindowUpdate(IntPtr::Zero);
					}
				}
			}
			return false;
		}

		bool MIPSMainForm::DeleteLine(MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl)
		{
			if (mipsDisassemblerControl->SelectionStart != -1)
			{
				int start = mipsDisassemblerControl->SelectionStart;
				int length = mipsDisassemblerControl->SelectionLength;

				int lineStart = mipsDisassemblerControl->GetLineFromCharIndex(start);
				int charNext = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart + 1);

				if (((mipsDisassemblerControl->SelectedText->Contains("\r\n")) || (mipsDisassemblerControl->SelectedText->Contains("\n")) || (mipsDisassemblerControl->SelectedText->Contains("\r")))
					|| ((charNext == -1) && ((start + length) >= (mipsDisassemblerControl->Text->Length - 1))))
					return true;

				
				// if more than one, just delete and return
				if (length > 0)
				{
					return false;
				}

				int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart);
				if (charNext == -1)
				{
					charNext = charStart + mipsDisassemblerControl->Lines[lineStart]->Length - 1;
				}

				int lineEnd = mipsDisassemblerControl->GetLineFromCharIndex(start + length - 1);
				int charEnd = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineEnd);
				
				charNext = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineEnd + 1);

				if (lineStart.Equals(lineEnd))
				{
					try
					{
						SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);

						int charNext = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineEnd + 1);
						
						if (charNext == (start + length + 1))
						{
							mipsDisassemblerControl->Select(charStart, 100);
							String^ textCurrent = mipsDisassemblerControl->SelectedText;
							
							int poundOffsetCurrent = GetPoundOffset(mipsDisassemblerControl, textCurrent, 100);

							mipsDisassemblerControl->Select(charNext, 100);
							String^ textNext = mipsDisassemblerControl->SelectedText;

							if (textNext->StartsWith("."))
							{
								mipsDisassemblerControl->Select(start, length);
								return true;
							}

							int poundOffsetNext = GetPoundOffset(mipsDisassemblerControl, textNext, 100);

							if (poundOffsetNext == -1)
							{
								mipsDisassemblerControl->Select(start, length);
								return true;
							}

							String^ address = textCurrent->Substring(0, 8);
							String^ addressNext = textNext->Substring(0, 8);

							if (address != addressNext)
							{
								mipsDisassemblerControl->Select(start, length);
								return true;
							}
							else
							{
								int charNextNext = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineEnd + 2);
								// delete /r/n
								//mipsDisassemblerControl->Select((charNextNext - 1), 1);
								//mipsDisassemblerControl->SelectedText = "";

								// delete line
								mipsDisassemblerControl->Select((charNext - 1), poundOffsetNext + 2);
								mipsDisassemblerControl->SelectedText = "";

								disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->RemoveAt(GetAbsoluteLineNumber(lineStart + 1));
								disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[GetAbsoluteLineNumber(lineStart)] = GetStringFromLine(lineStart, mipsDisassemblerControl);
								mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Value = mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Value;
								DrawRichTextBoxData(mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex], mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Value);

								mipsDisassemblerControl->Select(charNext-1, 0);

								mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Maximum--;
								return true;
							}
							// end
						}
						else
						{
							return false;
						}
						// TODO save to instructions
												
					}
					finally
					{
						SysWin32::LockWindowUpdate(IntPtr::Zero);
					}
				}
			}
			return false;
		}

		virtual void MIPSMainForm::KeyUp( Object^ sender, System::Windows::Forms::KeyEventArgs^ e )
		{
			MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl = (MIPSDisassembler::MIPSDisassemblerControlsControl^)sender;

			if (mipsDisassemblerControl->SelectionStart == -1)
				foundIndex = 0;
			else
				foundIndex = GetAbsoluteLineNumber(mipsDisassemblerControl->GetLineFromCharIndex(mipsDisassemblerControl->SelectionStart));

			if ((e->KeyCode == Keys::Left) || (e->KeyCode == Keys::Right) || (e->KeyCode == Keys::Up) || (e->KeyCode == Keys::Down)
				|| (e->KeyCode == Keys::Home) || (e->KeyCode == Keys::End) || (e->KeyCode == Keys::PageDown) || (e->KeyCode == Keys::PageUp)
				/*|| (e->KeyCode == Keys::Delete)  || (e->KeyCode == Keys::Back)  || (e->KeyCode == Keys::Enter)  || (e->KeyCode == Keys::Return)*/)
				Highlight(mipsDisassemblerControl);
				return;

			if ((e->KeyCode == Keys::F3) || (e->KeyCode == Keys::G) || (e->KeyCode == Keys::F))
				return;

			if (e->Control)
			{
				if ((e->KeyCode == Keys::X) || (e->KeyCode == Keys::V) || (e->KeyCode == Keys::Z))
				{
					e->SuppressKeyPress = true;
					e->Handled = true;
					return;
				}
			}


			int start = mipsDisassemblerControl->SelectionStart;
			if (start != -1)
			{
				int length = mipsDisassemblerControl->SelectionLength;

				SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);
				
				int lineSpot = mipsDisassemblerControl->GetLineFromCharIndex(start);
				String^ currentLine = GetStringFromLine(lineSpot, mipsDisassemblerControl);

				disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[GetAbsoluteLineNumber(lineSpot)] = currentLine;

				CommitLineToInstructionArray(GetAbsoluteLineNumber(lineSpot), mipsDisassemblerControl);
				mipsDisassemblerControl->Select(start, length);
				SysWin32::LockWindowUpdate(IntPtr::Zero);
			}
		}

		virtual void MIPSMainForm::KeyDown( Object^ sender, System::Windows::Forms::KeyEventArgs^ e )
		{
			MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl = (MIPSDisassembler::MIPSDisassemblerControlsControl^)sender;
			MIPSDisassembler::MIPSDisassemblerJoinedControl^ mipsDisassemblerJoinedControl = (MIPSDisassembler::MIPSDisassemblerJoinedControl^)mipsDisassemblerControl->Tag;

			if  (e->KeyCode == Keys::PageDown)
			{
				if (mipsDisassemblerControl->SelectionStart != -1)
				{
					MIPSDisassembler::MIPSDisassemblerJoinedControl^ mipsDisassemblerJoinedControl = (MIPSDisassembler::MIPSDisassemblerJoinedControl^)mipsDisassemblerControl->Tag;

					if ((mipsDisassemblerJoinedControl->vScrollBar->Value + mipsDisassemblerJoinedControl->vScrollBar->LargeChange) < 0)
						mipsDisassemblerJoinedControl->vScrollBar->Value = 0;
					else if ((mipsDisassemblerJoinedControl->vScrollBar->Value + mipsDisassemblerJoinedControl->vScrollBar->LargeChange) > (disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count - 1))
						mipsDisassemblerJoinedControl->vScrollBar->Value = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count - 1;
					else
						mipsDisassemblerJoinedControl->vScrollBar->Value = mipsDisassemblerJoinedControl->vScrollBar->Value + mipsDisassemblerJoinedControl->vScrollBar->LargeChange;
					
					int lineStart = mipsDisassemblerControl->GetLineFromCharIndex(mipsDisassemblerControl->SelectionStart);

					int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart);
					int charSameLineOffset = (mipsDisassemblerControl->SelectionStart - charStart);

					DrawRichTextBoxData(mipsDisassemblerJoinedControl, mipsDisassemblerJoinedControl->vScrollBar->Value);

					if (mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart) != -1)
					{
						mipsDisassemblerControl->Select((mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart) + charSameLineOffset), 0);
					}
					else
					{
						mipsDisassemblerControl->Select(charSameLineOffset, 0);
					}
				}

				e->SuppressKeyPress = true;
				e->Handled = true;
				return;
			}
			else if (e->KeyCode == Keys::PageUp)
			{
				if (mipsDisassemblerControl->SelectionStart != -1)
				{
					MIPSDisassembler::MIPSDisassemblerJoinedControl^ mipsDisassemblerJoinedControl = (MIPSDisassembler::MIPSDisassemblerJoinedControl^)mipsDisassemblerControl->Tag;
					
					if ((mipsDisassemblerJoinedControl->vScrollBar->Value - mipsDisassemblerJoinedControl->vScrollBar->LargeChange) < 0)
						mipsDisassemblerJoinedControl->vScrollBar->Value = 0;
					else if ((mipsDisassemblerJoinedControl->vScrollBar->Value - mipsDisassemblerJoinedControl->vScrollBar->LargeChange) > (disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count - 1))
						mipsDisassemblerJoinedControl->vScrollBar->Value = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count - 1;
					else
						mipsDisassemblerJoinedControl->vScrollBar->Value = mipsDisassemblerJoinedControl->vScrollBar->Value - mipsDisassemblerJoinedControl->vScrollBar->LargeChange;

					
					int lineStart = mipsDisassemblerControl->GetLineFromCharIndex(mipsDisassemblerControl->SelectionStart);

					int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart);
					int charSameLineOffset = (mipsDisassemblerControl->SelectionStart - charStart);

					DrawRichTextBoxData(mipsDisassemblerJoinedControl, mipsDisassemblerJoinedControl->vScrollBar->Value);

					if (mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart) != -1)
					{
						mipsDisassemblerControl->Select((mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart) + charSameLineOffset), 0);
					}
					else
					{
						mipsDisassemblerControl->Select(charSameLineOffset, 0);
					}
				}

				e->SuppressKeyPress = true;
				e->Handled = true;
				return;
			}
			else if (e->KeyCode == Keys::Up)
			{
				MIPSDisassembler::MIPSDisassemblerJoinedControl^ mipsDisassemblerJoinedControl = (MIPSDisassembler::MIPSDisassemblerJoinedControl^)mipsDisassemblerControl->Tag;
				
				if (mipsDisassemblerJoinedControl->mipsDisassemblerControls->SelectionStart != -1)
				{
					int lineStart = mipsDisassemblerJoinedControl->mipsDisassemblerControls->GetLineFromCharIndex(mipsDisassemblerJoinedControl->mipsDisassemblerControls->SelectionStart);
					int lineEnd = mipsDisassemblerJoinedControl->mipsDisassemblerControls->GetLineFromCharIndex(mipsDisassemblerJoinedControl->mipsDisassemblerControls->SelectionStart + mipsDisassemblerJoinedControl->mipsDisassemblerControls->SelectionLength);

					if ((lineStart == lineEnd) && (lineStart == 0) && (mipsDisassemblerJoinedControl->vScrollBar->Value > 0))
					{
						int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart);
						int charSameLineOffset = (mipsDisassemblerJoinedControl->mipsDisassemblerControls->SelectionStart - charStart);

						SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);
						String^ getNextLine = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[mipsDisassemblerJoinedControl->vScrollBar->Value-1];
						SysWin32::LockWindowUpdate(IntPtr::Zero);

						if ((mipsDisassemblerJoinedControl->vScrollBar->Value - 1) < 0)
							mipsDisassemblerJoinedControl->vScrollBar->Value = 0;
						else if ((mipsDisassemblerJoinedControl->vScrollBar->Value - 1) > (disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count - 1))
							mipsDisassemblerJoinedControl->vScrollBar->Value = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count - 1;
						else
							mipsDisassemblerJoinedControl->vScrollBar->Value = mipsDisassemblerJoinedControl->vScrollBar->Value - 1;
						DrawRichTextBoxData(mipsDisassemblerJoinedControl, mipsDisassemblerJoinedControl->vScrollBar->Value);

						SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);
						if (getNextLine->Length > charSameLineOffset)
							mipsDisassemblerControl->Select(mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart) + charSameLineOffset, 0);
						else
							mipsDisassemblerControl->Select(mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart) + (getNextLine->Length-1), 0);
						SysWin32::LockWindowUpdate(IntPtr::Zero);
						e->SuppressKeyPress = true;
						e->Handled = true;
					}
				}
				

				return;
			}
			else if (e->KeyCode == Keys::Down)
			{	
				if (mipsDisassemblerJoinedControl->mipsDisassemblerControls->SelectionStart != -1)
				{
					int lineStart = mipsDisassemblerJoinedControl->mipsDisassemblerControls->GetLineFromCharIndex(mipsDisassemblerJoinedControl->mipsDisassemblerControls->SelectionStart);
					int lineEnd = mipsDisassemblerJoinedControl->mipsDisassemblerControls->GetLineFromCharIndex(mipsDisassemblerJoinedControl->mipsDisassemblerControls->SelectionStart + mipsDisassemblerJoinedControl->mipsDisassemblerControls->SelectionLength);

					if ((lineStart == lineEnd) && ((GetAbsoluteLineNumber(lineStart)) < (disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count - 1)))
					{
						System::Drawing::Point clickLocation = mipsDisassemblerJoinedControl->mipsDisassemblerControls->GetPositionFromCharIndex(mipsDisassemblerJoinedControl->mipsDisassemblerControls->SelectionStart);
						int clientHeight = mipsDisassemblerJoinedControl->mipsDisassemblerControls->ClientRectangle.Height;

						int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart);
						int charSameLineOffset = (mipsDisassemblerJoinedControl->mipsDisassemblerControls->SelectionStart - charStart);

						if (lineStart == (numberLines - 1))
						{
							String^ getNextLine;
							if ((GetAbsoluteLineNumber(lineStart)) != (disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count-1))
							{
								SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);
								getNextLine = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[GetAbsoluteLineNumber(lineStart + 1)];
								SysWin32::LockWindowUpdate(IntPtr::Zero);
							}
							else
							{
								getNextLine = "";
							}


							if ((mipsDisassemblerJoinedControl->vScrollBar->Value + 1) < 0)
								mipsDisassemblerJoinedControl->vScrollBar->Value = 0;
							else if ((mipsDisassemblerJoinedControl->vScrollBar->Value + 1) > (disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count - 1))
								mipsDisassemblerJoinedControl->vScrollBar->Value = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count - 1;
							else
								mipsDisassemblerJoinedControl->vScrollBar->Value = mipsDisassemblerJoinedControl->vScrollBar->Value + 1;
							DrawRichTextBoxData(mipsDisassemblerJoinedControl, mipsDisassemblerJoinedControl->vScrollBar->Value);

							SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);
							if (getNextLine->Length > charSameLineOffset)
								mipsDisassemblerControl->Select(mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart) + charSameLineOffset, 0);
							else
								mipsDisassemblerControl->Select(mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart) + (getNextLine->Length-1), 0);
							SysWin32::LockWindowUpdate(IntPtr::Zero);
							e->SuppressKeyPress = true;
							e->Handled = true;
						}
					}
				}
				
				return;
			}

			if ((e->KeyCode == Keys::Left) || (e->KeyCode == Keys::Right))
				return;

			if (e->KeyCode == Keys::F3)
			{
				e->SuppressKeyPress = true;
				e->Handled = true;

				if ((foundIndex >= 0) && (foundWord != "") && (foundWord != nullptr))
				{
					List<String^>^ setOfLines = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines;
					int totalLines = setOfLines->Count;

					String^ findText = foundWord;

					if ((richTextBoxFinds & System::Windows::Forms::RichTextBoxFinds::Reverse) == System::Windows::Forms::RichTextBoxFinds::Reverse)
					{
						foundIndex--;
						if (foundIndex == 0)
							foundIndex = 0;

						for (int x = foundIndex; x >= 0; x--)
						{
							String^ line = setOfLines[x];

							int index = line->IndexOf(findText);
							if (index != -1)
							{				
								if (!line->StartsWith("."))
								{
									String^ addressStr = setOfLines[x]->Substring(0, 8);
									GoToAddress(mipsDisassemblerControl, addressStr, line);
									foundIndex = x;
									break;
								}
							}
						}
					}
					else
					{
						foundIndex++;
						if (foundIndex == (totalLines - 1))
							foundIndex = totalLines-1;

						for (int x = foundIndex; x < totalLines; x++)
						{
							String^ line = setOfLines[x];

							if (line->IndexOf(findText) != -1)
							{				
								if (!line->StartsWith("."))
								{
									String^ addressStr = setOfLines[x]->Substring(0, 8);
									GoToAddress(mipsDisassemblerControl, addressStr, line);

									foundIndex = x;
									break;
								}
							}
						}
					}
				}

				return;
			}

			if (e->Control)
			{
				if (e->KeyCode == Keys::Home)
				{
					mipsDisassemblerJoinedControl->vScrollBar->Value = 0;
					DrawRichTextBoxData(mipsDisassemblerJoinedControl, mipsDisassemblerJoinedControl->vScrollBar->Value);
					e->Handled = true;
					e->SuppressKeyPress = true;
					return;
				}
				else if (e->KeyCode == Keys::End)
				{
					mipsDisassemblerJoinedControl->vScrollBar->Value = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count-1;
					DrawRichTextBoxData(mipsDisassemblerJoinedControl, mipsDisassemblerJoinedControl->vScrollBar->Value);
					e->Handled = true;
					e->SuppressKeyPress = true;
					return;
				}
				else if (e->KeyCode == Keys::C)
				{
					return;
				}
				else if (e->KeyCode == Keys::Z)
				{
					e->SuppressKeyPress = true;
					e->Handled = true;
					return;
				}
				if (e->KeyCode == Keys::G)
				{
					GotoAddressDlg^ gotoAddressDlg = gcnew GotoAddressDlg();
					if (gotoAddressDlg->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					{
						int firstVisibleLine = (int)SysWin32::SendMessage(mipsDisassemblerControl->Handle, 0xCE, -1, 0);
						int totalLines = (int)SysWin32::SendMessage(mipsDisassemblerControl->Handle, 0xBA, 0, 0);

						try
						{
							UInt32::Parse(gotoAddressDlg->textBoxAddress->Text, System::Globalization::NumberStyles::HexNumber);
						}
						catch (Exception^)
						{
							MessageBox::Show("Error in goto address");
							return;
						}
						
						GoToAddress(mipsDisassemblerControl, gotoAddressDlg->textBoxAddress->Text, nullptr);
					}
				}
				if (e->KeyCode == Keys::F)
				{
					e->SuppressKeyPress = true;
					e->Handled = true;

					FindDlg^ findDlg = gcnew FindDlg();
					findDlg->textBoxFind->Text = "";
					if ((mipsDisassemblerControl->SelectionStart != -1) && (mipsDisassemblerControl->SelectionLength > 0))
					{
						int lineStart = mipsDisassemblerControl->GetLineFromCharIndex(mipsDisassemblerControl->SelectionStart);
						int lineEnd = mipsDisassemblerControl->GetLineFromCharIndex(mipsDisassemblerControl->SelectionStart + mipsDisassemblerControl->SelectionLength - 1);

						if (lineStart == lineEnd)
						{
							if ((!mipsDisassemblerControl->SelectedText->Contains("\n")) && (!mipsDisassemblerControl->SelectedText->Contains("\r")))
							{
								foundWord = mipsDisassemblerControl->SelectedText;
								findDlg->textBoxFind->Text = mipsDisassemblerControl->SelectedText;
							}
						}
					}

					if (findDlg->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					{
						if (findDlg->textBoxFind->Text->Length == 0)
							return;

						foundWord = findDlg->textBoxFind->Text;
						if (findDlg->checkBoxFindAll->Checked == false)
						{
							richTextBoxFinds = System::Windows::Forms::RichTextBoxFinds::None;
							if (findDlg->radioButtonUp->Checked == true)
								richTextBoxFinds = richTextBoxFinds | System::Windows::Forms::RichTextBoxFinds::Reverse;

							System::StringComparison strComparison = System::StringComparison::CurrentCultureIgnoreCase;
							if (findDlg->checkBoxMatchCase->Checked == true)
							{
								richTextBoxFinds = richTextBoxFinds | System::Windows::Forms::RichTextBoxFinds::MatchCase;
								strComparison = System::StringComparison::CurrentCulture;
							}

							if (findDlg->checkBoxMatchWholeWordsOnly->Checked == true)
								richTextBoxFinds = richTextBoxFinds | System::Windows::Forms::RichTextBoxFinds::WholeWord;

							List<String^>^ setOfLines = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines;
							int totalLines = setOfLines->Count;

							String^ findText = findDlg->textBoxFind->Text;

							if ((richTextBoxFinds & System::Windows::Forms::RichTextBoxFinds::Reverse) == System::Windows::Forms::RichTextBoxFinds::Reverse)
							{
								for (int x = foundIndex; x >= 0; x--)
								{
									String^ line = setOfLines[x];

									int index = line->IndexOf(findText, strComparison);
									if (index != -1)
									{				
										if (!line->StartsWith("."))
										{
											if (findDlg->checkBoxMatchWholeWordsOnly->Checked)
											{
												String^ getWord = GetWord(line, index);
												if (!getWord->Replace("\t", "")->Replace(" ", "")->Equals(findText, strComparison))
													continue;
											}

											String^ addressStr = setOfLines[x]->Substring(0, 8);
											GoToAddress(mipsDisassemblerControl, addressStr, line);
											foundIndex = x;
											break;
										}
									}
								}
							}
							else
							{
								for (int x = foundIndex; x < totalLines; x++)
								{
									String^ line = setOfLines[x];

									if (line->IndexOf(findText, strComparison) != -1)
									{				
										if (!line->StartsWith("."))
										{
											int index = line->IndexOf(findText, strComparison);
											if (findDlg->checkBoxMatchWholeWordsOnly->Checked)
											{
												String^ getWord = GetWord(line, index);
												if (!getWord->Replace("\t", "")->Replace(" ", "")->Equals(findText, strComparison))
													continue;
											}

											String^ addressStr = setOfLines[x]->Substring(0, 8);
											GoToAddress(mipsDisassemblerControl, addressStr, line);

											foundIndex = x;
											break;
										}
									}
								}
							}
						}
						else
						{
							SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);

							mipsDisassemblerFindAllControlsList[tabControlDisassembles->SelectedIndex]->listView->Items->Clear();
							mipsDisassemblerFindAllControlsList[tabControlDisassembles->SelectedIndex]->targets->Clear();

							richTextBoxFinds = System::Windows::Forms::RichTextBoxFinds::None;
							System::StringComparison strComparison = System::StringComparison::CurrentCultureIgnoreCase;

							if (findDlg->checkBoxMatchCase->Checked == true)
							{
								richTextBoxFinds = richTextBoxFinds | System::Windows::Forms::RichTextBoxFinds::MatchCase;
								strComparison = System::StringComparison::CurrentCulture;
							}

							if (findDlg->checkBoxMatchWholeWordsOnly->Checked == true)
								richTextBoxFinds = richTextBoxFinds | System::Windows::Forms::RichTextBoxFinds::WholeWord;
							
							List<String^>^ setOfLines = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines;
							int totalLines = setOfLines->Count;
							foundIndex = 0;

							String^ findText = findDlg->textBoxFind->Text;
							for (int x = 0; x < totalLines; x++)
							{
								String^ line = setOfLines[x];
 
								if (line->IndexOf(findText, strComparison) != -1)
								{				
									int spot = line->IndexOf(findText, strComparison);
									if (!line->StartsWith("."))
									{
										if (findDlg->checkBoxMatchWholeWordsOnly->Checked)
										{
											String^ getWord = GetWord(line, spot);
											if (!getWord->Replace("\t", "")->Replace(" ", "")->Equals(findText, strComparison))
												continue;
										}

										String^ addressStr = setOfLines[x]->Substring(0, 8);
										if (!mipsDisassemblerFindAllControlsList[tabControlDisassembles->SelectedIndex]->targets->ContainsKey(addressStr))
										{
											// add to label box
											ListViewItem^ item = gcnew ListViewItem( addressStr);
											item->SubItems->Add(setOfLines[x]);
											mipsDisassemblerFindAllControlsList[tabControlDisassembles->SelectedIndex]->targets->Add(addressStr, item);
											mipsDisassemblerFindAllControlsList[tabControlDisassembles->SelectedIndex]->listView->Items->Add(item);
										}
									}

									foundIndex = x;
								}
							}

							SysWin32::LockWindowUpdate(IntPtr::Zero);
						}
					}
					tabControlViews->SelectedIndex = 2;
					return;
				}
			}
			else
			{
				if ((e->KeyCode == Keys::Home) || (e->KeyCode == Keys::End))
					return;
			}

			if (IsCommentSpot(mipsDisassemblerControl))
			{
				if (e->Control)
				{
					if (e->KeyCode == Keys::X)
					{
						return;
					}
					else if (e->KeyCode == Keys::V)
					{
						if (Clipboard::ContainsText())
						 {
							System::Windows::Forms::IDataObject^ clipData = System::Windows::Forms::Clipboard::GetDataObject();
							if (clipData->GetDataPresent(DataFormats::Text))
							{
									String^ s = System::Windows::Forms::Clipboard::GetData(DataFormats::Text)->ToString()->Replace("\r\n", "")->Replace("\n", "")->Replace("\r", "");

									if (mipsDisassemblerControl->SelectionStart != -1)
									{
										mipsDisassemblerControl->SelectedText = s;
									}
							}
						 }
						e->SuppressKeyPress = true;
						e->Handled = true;
						return;
					}
				}

				if (e->Shift)
				{
					if (e->KeyCode == Keys::D3)
					{
						e->SuppressKeyPress = true;
						e->Handled = true;
						return;
					}
				}
				else if ((e->KeyCode == Keys::Enter) || (e->KeyCode == Keys::Return))
				{
					if (InsertNewLine(mipsDisassemblerControl))
					{
						e->SuppressKeyPress = true;
						e->Handled = true;
						return;
					}
				}
				else if (e->KeyCode == Keys::Back)
				{
					if (BackspaceLine(mipsDisassemblerControl))
					{
						e->SuppressKeyPress = true;
						e->Handled = true;
						return;
					}
				}
				else if (e->KeyCode == Keys::Delete)
				{
					if (DeleteLine(mipsDisassemblerControl))
					{
						e->SuppressKeyPress = true;
						e->Handled = true;
						return;
					}
				}

				return;
			}
			else
			{
				/*if (e->KeyCode == Keys::C)
				{
					int lineStart = mipsDisassemblerControl->GetLineFromCharIndex(mipsDisassemblerControl->SelectionStart);
					int lineEnd = mipsDisassemblerControl->GetLineFromCharIndex(mipsDisassemblerControl->SelectionStart + mipsDisassemblerControl->SelectionLength - 1);

					if (lineStart == lineEnd)
					{
						String^ startLineStr = GetStringFromLine(lineStart, mipsDisassemblerControl);
						String^ endLineStr = GetStringFromLine(lineEnd, mipsDisassemblerControl);

						int absNum = GetAbsoluteLineNumber(lineStart);
						for (int x = lineStart; x < lineEnd; x++)
						{
							if (GetStringFromLine(lineStart, mipsDisassemblerControl)->StartsWith("."))
							{
								e->SuppressKeyPress = true;
								e->Handled = true;
								return;
							}
						}


						IInstruction^ instrStart = GetInstruction(tabControlDisassembles->SelectedIndex, UInt32::Parse(startLineStr->Substring(0, 8), System::Globalization::NumberStyles::HexNumber));
						if (instrStart->InstrDefResults != nullptr)
						{
							e->SuppressKeyPress = true;
							e->Handled = true;
							return;
						}

						MIPSDisassembler::DisassembledFileDetails^ binaryData = GetDisassembledFileDetailsFromInstruction(tabControlDisassembles->SelectedIndex, instrStart);
						if (binaryData == nullptr)
						{
							e->SuppressKeyPress = true;
							e->Handled = true;
							return;
						}
						disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->instructionSet->ParseInstruction(binaryData->startAddress, binaryData->binarySize, instrStart->Address, instrStart);

		
						RegenSetOfLines(disassemblyContentsArray[tabControlDisassembles->SelectedIndex]);

						DrawRichTextBoxData(mipsDisassemblerJoinedControl, absNum);


						e->SuppressKeyPress = true;
						e->Handled = true;
						return;
						
					}
				}
				else if (e->KeyCode == Keys::U)
				{

				}*/
				e->Handled = true;
				e->SuppressKeyPress = true;
				return;
			}
		}

		MIPSDisassembler::DisassembledFileDetails^ MIPSMainForm::GetDisassembledFileDetailsFromInstruction(int tab, IInstruction^ instruction)
		{
			for (int x = 0; x < disassemblyContentsArray[tab]->binaryData->Count; x++)
			{
				if ((instruction->Address >= disassemblyContentsArray[tab]->binaryData[x]->startAddress) && (instruction->Address < (disassemblyContentsArray[tab]->binaryData[x]->startAddress + disassemblyContentsArray[tab]->binaryData[x]->binarySize)))
				{
					return disassemblyContentsArray[tab]->binaryData[x];
				}
			}
			return nullptr;
		}

		IInstruction^ MIPSMainForm::GetInstruction(int tab, unsigned long location)
		{
			for (int x = 0; x < disassemblyContentsArray[tab]->binaryData->Count; x++)
			{
				if ((location >= disassemblyContentsArray[tab]->binaryData[x]->startAddress) && (location < (disassemblyContentsArray[tab]->binaryData[x]->startAddress + disassemblyContentsArray[tab]->binaryData[x]->binarySize)))
				{
					int instrOffset = disassemblyContentsArray[tab]->binaryData[x]->offsetInInstructions + ((location - disassemblyContentsArray[tab]->binaryData[x]->startAddress) / 4);
					return disassemblyContentsArray[tab]->instructions[instrOffset];
				}
			}
			return nullptr;
		}

		bool MIPSMainForm::IsValidLocation(int tab, unsigned long location)
		{
			for (int x = 0; x < disassemblyContentsArray[tab]->binaryData->Count; x++)
			{
				if ((location >= disassemblyContentsArray[tab]->binaryData[x]->startAddress) && (location < (disassemblyContentsArray[tab]->binaryData[x]->startAddress + disassemblyContentsArray[tab]->binaryData[x]->binarySize)))
				{
					return true;
				}
			}
			return false;
		}

		IInstruction^ MIPSMainForm::GetInstruction(DisassemblyContents^ disassemblyContents, unsigned long location)
		{
			for (int x = 0; x < disassemblyContents->binaryData->Count; x++)
			{
				if ((location >= disassemblyContents->binaryData[x]->startAddress) && (location < (disassemblyContents->binaryData[x]->startAddress + disassemblyContents->binaryData[x]->binarySize)))
				{
					int instrOffset = disassemblyContents->binaryData[x]->offsetInInstructions + ((location - disassemblyContents->binaryData[x]->startAddress) / 4);
					return disassemblyContents->instructions[instrOffset];
				}
			}
			return nullptr;
		}

		bool MIPSMainForm::IsValidLocation(DisassemblyContents^ disassemblyContents, unsigned long location)
		{
			for (int x = 0; x < disassemblyContents->binaryData->Count; x++)
			{
				if ((location >= disassemblyContents->binaryData[x]->startAddress) && (location < (disassemblyContents->binaryData[x]->startAddress + disassemblyContents->binaryData[x]->binarySize)))
				{
					return true;
				}
			}
			return false;
		}

private: void RegenSetOfLines(DisassemblyContents^ disassemblyContents)
		{

			disassemblyContents->setOfLines->Clear();
			for (int instrOffset = 0; instrOffset < disassemblyContents->binaryData->Count; instrOffset++)
			{
				disassemblyContents->setOfLines->Add(String::Format(".start \"{0}\"\r\n", disassemblyContents->binaryData[instrOffset]->filename));
				disassemblyContents->setOfLines->Add(String::Format(".size 0x{0:X8}	\r\n", disassemblyContents->binaryData[instrOffset]->binarySize));
				unsigned long endDrawLocation = disassemblyContents->binaryData[instrOffset]->startAddress + disassemblyContents->binaryData[instrOffset]->binarySize;
				for (unsigned long drawLocation = disassemblyContents->binaryData[instrOffset]->startAddress; drawLocation < endDrawLocation; drawLocation+=4)
				{
					IInstruction^ currentInstruction = GetInstruction(disassemblyContents, drawLocation);
					if ((currentInstruction->Parents != nullptr) && (currentInstruction->Parents->Count > 0))
					{
						if (currentInstruction->IsFunction)
						{
							disassemblyContents->setOfLines->Add(String::Format("{0:X8}:\t{1}\r\n", drawLocation, "-----------------------------------------------------------------------"));
							disassemblyContents->setOfLines->Add(String::Format("{0:X8}:\t{1}\r\n", drawLocation, "------------------------------SUBROUTINE-------------------------------"));

							if (currentInstruction->InputRegisters != nullptr)
							{
								String^ registerInputLine = "Inputs: ";
								for (int r = 0; r < currentInstruction->InputRegisters->Count; r++)
								{
									if (r != (currentInstruction->InputRegisters->Count - 1))
										registerInputLine = registerInputLine + currentInstruction->InputRegisters[r] + ", ";
									else
										registerInputLine = registerInputLine + currentInstruction->InputRegisters[r];
								}
								int lineSize = registerInputLine->Length;
								
								registerInputLine = registerInputLine->PadLeft((lineSize + ((71 - lineSize) / 2)), '-');
								registerInputLine = registerInputLine->PadRight(71, '-');
								
								disassemblyContents->setOfLines->Add(String::Format("{0:X8}:\t{1}\r\n", drawLocation, registerInputLine));
							}

							if ((currentInstruction->Exits != nullptr) && (currentInstruction->Exits->Count > 0))
							{
								for (int x = 0; x < currentInstruction->Exits->Count; x++)
								{
									String^ exitLine = String::Format(".EXIT:{0:X8}", currentInstruction->Exits[x]->Address);
									int lineSize = exitLine->Length;
									
									exitLine = exitLine->PadLeft((lineSize + ((71 - lineSize) / 2)), '-');
									exitLine = exitLine->PadRight(71, '-');
									
									disassemblyContents->setOfLines->Add(String::Format("{0:X8}:\t{1}\r\n", drawLocation, exitLine));
								}
							}

							disassemblyContents->setOfLines->Add(String::Format("{0:X8}:\t{1}\r\n", drawLocation, "-----------------------------------------------------------------------"));
						}

						if ((currentInstruction->Name != nullptr) && (currentInstruction->Name != ""))
						{
							disassemblyContents->setOfLines->Add(String::Format("{0:X8}: .LABEL {1}\r\n", drawLocation, currentInstruction->Name));
						}

						System::Text::StringBuilder^ currentSubLine = gcnew System::Text::StringBuilder();
						for (int x = 0; x < currentInstruction->Parents->Count; x++)
						{
							IInstruction^ parentInstruction = currentInstruction->Parents[x];

							if (x == 0)
							{
								currentSubLine->AppendFormat("{0:X8}:\t", drawLocation);
							}
							if ((x > 0) && ((x % 4)  == 0))
							{
								disassemblyContents->setOfLines->Add(currentSubLine->ToString() + "\r\n");
								currentSubLine = gcnew System::Text::StringBuilder();
								currentSubLine->AppendFormat("{0:X8}:\t", drawLocation);
							}

							
							String^ xrefNameStr = "";
							
							if (
								(parentInstruction->Name != nullptr)
								&& (parentInstruction->Name != "")
								)
							{
								xrefNameStr = " (" + parentInstruction->Name + ")";
							}

							String^ upDown;
							if (parentInstruction->Address == currentInstruction->Address)
								upDown = "[E]";
							else if (parentInstruction->Address < currentInstruction->Address)
								upDown = "[U]";
							else
								upDown = "[D]";

							if (x != (currentInstruction->Parents->Count - 1))
							{
								if (parentInstruction->InstrType == InstructionType::Load)
								{
									currentSubLine->AppendFormat(".LREF:{0:X8}{1}{2}, ", parentInstruction->Address, upDown, xrefNameStr);
								}
								else if (parentInstruction->InstrType == InstructionType::Store)
								{
									currentSubLine->AppendFormat(".SREF:{0:X8}{1}{2}, ", parentInstruction->Address, upDown, xrefNameStr);
								}
								else
								{
									currentSubLine->AppendFormat(".XREF:{0:X8}{1}{2}, ", parentInstruction->Address, upDown, xrefNameStr);
								}
							}
							else
							{
								if (parentInstruction->InstrType == InstructionType::Load)
								{
									currentSubLine->AppendFormat(".LREF:{0:X8}{1}{2},", parentInstruction->Address, upDown, xrefNameStr);
									disassemblyContents->setOfLines->Add(currentSubLine->ToString() + "\r\n");
								}
								else if (parentInstruction->InstrType == InstructionType::Store)
								{
									currentSubLine->AppendFormat(".SREF:{0:X8}{1}{2},", parentInstruction->Address, upDown, xrefNameStr);
									disassemblyContents->setOfLines->Add(currentSubLine->ToString() + "\r\n");
								}
								else
								{
									currentSubLine->AppendFormat(".XREF:{0:X8}{1}{2},", parentInstruction->Address, upDown, xrefNameStr);
									disassemblyContents->setOfLines->Add(currentSubLine->ToString() + "\r\n");
								}
							}
						}
					}
					else
					{
						if ((currentInstruction->Name != nullptr) && (currentInstruction->Name != ""))
						{
							disassemblyContents->setOfLines->Add(String::Format("{0:X8}: .LABEL {1}\r\n", drawLocation, currentInstruction->Name));
						}
					}

					String^ targetStr = "";
					if (currentInstruction->Target != 0xFFFFFFFF)
					{
						bool isValidTarget = IsValidLocation(disassemblyContents, currentInstruction->Target);
						
						if (isValidTarget)
						{
							IInstruction^ targetInstruction = GetInstruction(disassemblyContents, currentInstruction->Target);
							if ((targetInstruction->Name != nullptr) && (targetInstruction->Name != ""))
							{
								targetStr = " (" + targetInstruction->Name + ")";
							}
						}
					}

					String^ currentTempLine = String::Format("{0:X8}:\t{1:X8}\t{2}{3}\t#", drawLocation, currentInstruction->InstructionRaw, currentInstruction->InstructionText, targetStr);
					if ((currentInstruction->Comment != nullptr) && (currentInstruction->Comment->Count > 0))
					{
						disassemblyContents->setOfLines->Add(String::Format("{0}{1}\r\n", currentTempLine, currentInstruction->Comment[0]));
						for (int x = 1; x < currentInstruction->Comment->Count; x++)
						{
							disassemblyContents->setOfLines->Add(String::Format("{0:X8}:\t\t\t#{1}\r\n", drawLocation, currentInstruction->Comment[x]));
						}
					}
					else
					{
						disassemblyContents->setOfLines->Add(String::Format("{0}\r\n", currentTempLine));
					}

					if (currentInstruction->IsEnd != nullptr)
					{
						disassemblyContents->setOfLines->Add(String::Format("{0:X8}:\t{1}\r\n", drawLocation, "-----------------------------------------------------------------------"));
						disassemblyContents->setOfLines->Add(String::Format("{0:X8}:\t{1}\r\n", drawLocation, "--------------------------END SUBROUTINE " + currentInstruction->IsEnd->Address.ToString("X8") +"----------------------"));
						disassemblyContents->setOfLines->Add(String::Format("{0:X8}:\t{1}\r\n", drawLocation, "-----------------------------------------------------------------------"));
					}

					
				}

				disassemblyContents->setOfLines->Add(".end");
			}
		}

		 void MIPSMainForm::AddTab(DisassemblyContents^ disassemblyContents)
		 {
			disassemblyContentsArray->Add(disassemblyContents);
			System::Windows::Forms::TabPage^ tabPage = gcnew System::Windows::Forms::TabPage();
			MIPSDisassembler::MIPSDisassemblerJoinedControl^  mipsDisassemblerJoinedControl = (gcnew MIPSDisassembler::MIPSDisassemblerJoinedControl());
			mipsDisassemblerJoinedControl->Dock = System::Windows::Forms::DockStyle::Fill;
			mipsDisassemblerJoinedControl->Font = gcnew System::Drawing::Font("Courier New", 10, FontStyle::Regular);

			sizeChars = System::Windows::Forms::TextRenderer::MeasureText("W", mipsDisassemblerJoinedControl->mipsDisassemblerControls->Font);
			numberLines = mipsDisassemblerJoinedControl->mipsDisassemblerControls->ClientRectangle.Height / sizeChars.Height;

			mipsDisassemblerJoinedControl->mipsDisassemblerControls->SelectionTabs = gcnew array<int>(3) { (sizeChars.Width * 9), (sizeChars.Width * 9)*2, ((sizeChars.Width *9)*4) }; 
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MIPSMainForm::KeyDown);
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MIPSMainForm::KeyUp);
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->MouseWheel += gcnew MIPSDisassembler::MouseEventHandler(this, &MIPSMainForm::MouseWheelDisassemblerEvent);
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->ScrollBars = RichTextBoxScrollBars::None;

			mipsDisassemblerJoinedControl->mipsDisassemblerControls->WordWrap = false;
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Both;
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MIPSMainForm::MouseClickDisassemblerEvent);
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->Resize += gcnew System::EventHandler(this, &MIPSMainForm::OnResize);
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->MouseHover += gcnew System::EventHandler(this, &MIPSMainForm::OnHover);
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MIPSMainForm::MouseMove);
			
			mipsDisassemblerJoinedControl->vScrollBar->Scroll += gcnew ScrollEventHandler(this, &MIPSMainForm::vScrollBar_Scroll);
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->Tag = mipsDisassemblerJoinedControl;
			mipsDisassemblerJoinedControl->vScrollBar->Tag = mipsDisassemblerJoinedControl;

			tabPage->Location = System::Drawing::Point(4, 25);
			FileInfo^ fileInfo = gcnew FileInfo(disassemblyContents->binaryData[0]->filename); 

			tabPage->Name = fileInfo->Name;
			tabPage->Size = System::Drawing::Size(998, 384);
			tabPage->TabIndex = 0;
			tabPage->Text = fileInfo->Name;
			tabPage->UseVisualStyleBackColor = true;
			tabPage->Dock = System::Windows::Forms::DockStyle::Fill;
			tabPage->Controls->Add(mipsDisassemblerJoinedControl);
			tabControlDisassembles->TabPages->Add(tabPage);
			mipsDisassemblerJoinedControlsList->Add(mipsDisassemblerJoinedControl);

			try
			{
				SysWin32::LockWindowUpdate(mipsDisassemblerJoinedControl->mipsDisassemblerControls->Handle);

				//System::Text::StringBuilder^ strBld = gcnew System::Text::StringBuilder();
				
				RegenSetOfLines(disassemblyContents);

				//String^ strCompleted = strBld->ToString();
				//int strCompletedLength = strCompleted->Length;
				mipsDisassemblerJoinedControl->mipsDisassemblerControls->Text = "";

				mipsDisassemblerJoinedControl->vScrollBar->Maximum = (disassemblyContents->setOfLines->Count - 1);
				/*vScrollBar->Minimum = 0;
				vScrollBar->Maximum = disassemblyContents->setOfLines->Count;*/
			}
			finally
			{
				SysWin32::LockWindowUpdate(IntPtr::Zero);
			}

			mipsDisassemblerJoinedControl->mipsDisassemblerControls->Focus();
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->Select(0, 0);

			SubroutinesControl^ subroutinesControl = gcnew SubroutinesControl();
			subroutinesControl->Dock = System::Windows::Forms::DockStyle::Fill;
			subroutinesControl->Visible = false;
			subroutinesControl->listView->DoubleClick += gcnew EventHandler(this, &MIPSMainForm::DoubleClickListView);

			mipsDisassemblerSubroutineControlsList->Add(subroutinesControl);
			tabControlViews->TabPages[0]->Controls->Add(subroutinesControl);
			FillSubroutineControl(disassemblyContents, subroutinesControl);


			LabelsControl^ labelsControl = gcnew LabelsControl();
			labelsControl->Dock = System::Windows::Forms::DockStyle::Fill;
			labelsControl->Visible = false;
			labelsControl->listView->DoubleClick += gcnew EventHandler(this, &MIPSMainForm::DoubleClickListView);

			mipsDisassemblerLabelControlsList->Add(labelsControl);
			tabControlViews->TabPages[1]->Controls->Add(labelsControl);
			FillLabelsControl(disassemblyContents, labelsControl);

			FindAllControl^ findAllControl = gcnew FindAllControl();
			findAllControl->Dock = System::Windows::Forms::DockStyle::Fill;
			findAllControl->Visible = false;
			findAllControl->listView->DoubleClick += gcnew EventHandler(this, &MIPSMainForm::DoubleClickListView);

			mipsDisassemblerFindAllControlsList->Add(findAllControl);
			tabControlViews->TabPages[2]->Controls->Add(findAllControl);
			//FillFindAllControl(disassemblyContents, findAllControl);


			


			tabControlDisassembles->SelectedIndex = (tabControlDisassembles->TabPages->Count-1);

			mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Value = 0;
			DrawRichTextBoxData(mipsDisassemblerJoinedControl, 0);
			SwitchTabs();

		 }

		 private: void DoubleClickListView(Object^ sender, EventArgs^ e) {
				ListView^ listView = (ListView^)sender;
				//if (listView->Parent->GetType() == SubroutinesControl::typeid)
				{
					if (listView->SelectedItems->Count == 1)
					{
						ListViewItem^ item = listView->SelectedItems[0];

						if ((item->Group == nullptr) || (!(item->Group->Header->Contains("Not in address space"))))
						{
							String^ address = item->Text->Substring(0, 8);
							GoToAddress(mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->mipsDisassemblerControls, address, item->Text);
						}
					}
				}

}


		  void FillLabelsControl(DisassemblyContents^ disassemblyContents, LabelsControl^ labelsControl)
		 {
			 ListViewGroup^ groupUser = gcnew ListViewGroup("User");
			 labelsControl->listView->Groups->Add(groupUser);

			 labelsControl->listView->BeginUpdate();
			 
			 for (int x = 0; x < disassemblyContents->binaryData->Count; x++)
			 {
				 for (unsigned long address = disassemblyContents->binaryData[x]->startAddress; address < (disassemblyContents->binaryData[x]->startAddress + disassemblyContents->binaryData[x]->binarySize); address += 4)
				 {
					IInstruction^ currentInstruction = GetInstruction(disassemblyContents, address);
					if (!labelsControl->targets->ContainsKey(currentInstruction->Address.ToString("X8")))
					{
						if ((currentInstruction->Name != nullptr) && (currentInstruction->Name != ""))
						{
							ListViewItem^ item = gcnew ListViewItem( currentInstruction->Address.ToString("X8"));
							item->SubItems->Add(currentInstruction->Name);
							labelsControl->targets->Add(currentInstruction->Address.ToString("X8"), item);
							labelsControl->listView->Items->Add(item);
						}
					}
				 }
			 }
			 labelsControl->listView->EndUpdate();
		 }


		 void FillSubroutineControl(DisassemblyContents^ disassemblyContents, SubroutinesControl^ subroutinesControl)
		 {
			 subroutinesControl->listView->BeginUpdate();
			 
			 ListViewGroup^ groupNotAddressSpace = gcnew ListViewGroup("Not in address space");
			 ListViewGroup^ groupAddressSpace = gcnew ListViewGroup("Address space");
			 subroutinesControl->listView->Groups->Add(groupAddressSpace);
			 subroutinesControl->listView->Groups->Add(groupNotAddressSpace);

			 for (int instrOffset = 0; instrOffset < disassemblyContents->binaryData->Count; instrOffset++)
			 {
				 for (unsigned long address = disassemblyContents->binaryData[instrOffset]->startAddress; address < (disassemblyContents->binaryData[instrOffset]->startAddress + disassemblyContents->binaryData[instrOffset]->binarySize); address += 4)
				 {
					IInstruction^ currentInstruction = GetInstruction(disassemblyContents, address);

					if ((currentInstruction->InstrType == InstructionType::Jump) || (currentInstruction->InstrType == InstructionType::JumpAndLink))
					{
						if (!subroutinesControl->targets->ContainsKey(currentInstruction->Target.ToString("X8")))
						{
							ListViewItem^ item;
							bool isValidInstruction = IsValidLocation(disassemblyContents, currentInstruction->Target);
							if (isValidInstruction)
							{
								IInstruction^ parentInstruction = GetInstruction(disassemblyContents, currentInstruction->Target);
								
								item = gcnew ListViewItem( parentInstruction->Address.ToString("X8"), 0, groupAddressSpace);
								item->SubItems->Add(parentInstruction->Name);
								String^ exits = "";
								if (parentInstruction->Exits != nullptr)
								{
									for (int x = 0; x < parentInstruction->Exits->Count; x++)
									{
										if (x == 0)
											exits += parentInstruction->Exits[x]->Address.ToString("X8");
										else
											exits += "," + parentInstruction->Exits[x]->Address.ToString("X8");
									}
								}
								item->SubItems->Add(exits);
								subroutinesControl->targets->Add(currentInstruction->Target.ToString("X8"), item);
							}
							else
							{
								item = gcnew ListViewItem( currentInstruction->Target.ToString("X8"), 0, groupNotAddressSpace);
								//item->SubItems->Add("Not in address space");
								subroutinesControl->targets->Add(currentInstruction->Target.ToString("X8"), item);
							}

							subroutinesControl->listView->Items->Add(item);
						}
						
						

						
					}
				 }
			 }
			 subroutinesControl->listView->EndUpdate();
		 }

private: System::Void tabControlDisassembles_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			  // check if the right mouse button was pressed
			 if(e->Button == System::Windows::Forms::MouseButtons::Right)
			{
				// iterate through all the tab pages
				for(int i = 0; i < tabControlDisassembles->TabCount; i++)
				{
					// get their rectangle area and check if it contains the mouse cursor
					Rectangle^ r = tabControlDisassembles->GetTabRect(i);
					if (r->Contains(e->Location))
					{
						Point clickScreenPoint = tabControlDisassembles->PointToScreen(Point(e->X, e->Y));
						contextMenuStripTabHeaderRightClick->Tag = i;
						contextMenuStripTabHeaderRightClick->Show(Point(clickScreenPoint));
						
						break;
					 }
				}
			}

		 }

		void Colorize(MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl, bool vscroll)
		{
			int start = mipsDisassemblerControl->SelectionStart;
			int length = mipsDisassemblerControl->SelectionLength;
			if (start == -1)
				return;

			int firstVisibleLine = (int)SysWin32::SendMessage(mipsDisassemblerControl->Handle, 0xCE, -1, 0);
			
			int totalLines = (int)SysWin32::SendMessage(mipsDisassemblerControl->Handle, 0xBA, 0, 0);
			int lastVisibleLine = firstVisibleLine + (mipsDisassemblerControl->ClientRectangle.Height / sizeChars.Height) + 1;

			//lastVisibleLine += (mipsDisassemblerControl->Height / sizeChars.Height) + 1; // preload next down too

			bool lastLine = false;
			if (lastVisibleLine >= totalLines)
			{
				lastVisibleLine = totalLines - 1;
				lastLine = true;
			}

			try
			{
				SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);

				int startChar = mipsDisassemblerControl->GetFirstCharIndexFromLine(firstVisibleLine);
				int lastLineChar = mipsDisassemblerControl->GetFirstCharIndexFromLine(lastVisibleLine);

				// TODO replace color of immediates
				mipsDisassemblerControl->Select(startChar, (lastLineChar - startChar));
				
				
				String^ currentBlock = mipsDisassemblerControl->SelectedText;

				List<IColorIndexes^>^ colorIndexes = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->instructionSet->Colorize(currentBlock);
				for (int x = 0; x < colorIndexes->Count; x++)
				{
					SysWin32::SendMessage(mipsDisassemblerControl->Handle, 0xB1, (startChar + colorIndexes[x]->Offset), (startChar + colorIndexes[x]->Offset + colorIndexes[x]->Length));
					//mipsDisassemblerControl->Select((startChar + colorIndexes[x]->Offset), colorIndexes[x]->Length);
					mipsDisassemblerControl->SelectionColor = colorIndexes[x]->ColorValue;
				}

				if (vscroll)
				{
					if (!lastLine)
						mipsDisassemblerControl->Select(startChar, 0);
					else
						mipsDisassemblerControl->Select(lastLineChar, 0);
				}
				else
				{
					mipsDisassemblerControl->Select(start, length);
				}	
			}
			finally 
			{
				SysWin32::LockWindowUpdate(IntPtr::Zero);
			}

			//SysWin32::SetScrollPos(mipsDisassemblerControl->Handle, 1, m.WParam.ToInt32(), true);
		}

		private: void VScrollRealEvent(Message% m)
		{
			MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl = (MIPSDisassembler::MIPSDisassemblerControlsControl^)this->FromHandle(m.HWnd);

			/*mipsDisassemblerControl->OnVScrollWndProc -= gcnew MIPSDisassembler::VScrollWndProcDelegate(this, &MIPSMainForm::VScrollRealEvent);
			Colorize(mipsDisassemblerControl, true);

			mipsDisassemblerControl->OnVScrollWndProc += gcnew MIPSDisassembler::VScrollWndProcDelegate(this, &MIPSMainForm::VScrollRealEvent);*/

			return;
		}

	 public: String^ GetWord(String^ input, int position) 
	 {            
		 if (position >= (input->Length - 1))
			 return "";

		 char s = input[position];            
		 int sp1 = 0, sp2 = input->Length;           

		 for (int i = position; i > 0; i--)            
		 {               
			 char ch = input[i];                
			 if (ch == ' ' || ch == '\n' || ch == '\t')                
			 {                    
				 sp1 = i;                    
				 break;                
			 }            
		 }           
		 for (int i = position; i < input->Length; i++)            
		 {                
			 char ch = input[i];                
			 if (ch == ' ' || ch == '\n' || ch == '\t')                
			 {                    
				 sp2 = i;                    
				 break;                
			 }            
		 }            
		 return input->Substring(sp1, sp2 - sp1)->Replace("\n", "");        
	 }

private: System::Void closeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 System::Windows::Forms::ToolStripMenuItem^ menu = (System::Windows::Forms::ToolStripMenuItem^)sender;
			 int tab = (int)menu->GetCurrentParent()->Tag;
			RemoveTab(tab);
		 }

	private: void MouseMove(Object^ sender, MouseEventArgs^ e)
			 {
				 lastLoc = e->Location;
			 }

		 private: System::Void OnHover(System::Object^  sender, System::EventArgs^  e) {
			MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl = (MIPSDisassembler::MIPSDisassemblerControlsControl^)sender;
			String^ curWord = GetWord(mipsDisassemblerControl->Text, mipsDisassemblerControl->GetCharIndexFromPosition(*reinterpret_cast<System::Drawing::Point^>(lastLoc)));
			if (curWord->Contains("LB") || curWord->Contains("SB"))
			{
				if (!toolTipRichTextBox->Active)
					toolTipRichTextBox->Show(curWord, mipsDisassemblerControl, *reinterpret_cast<System::Drawing::Point^>(lastLoc));
			}
			else
			{
				toolTipRichTextBox->Hide(mipsDisassemblerControl);
			}
				  }
private: System::Void OnResize(System::Object^  sender, System::EventArgs^  e) {
			 MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl = (MIPSDisassembler::MIPSDisassemblerControlsControl^)sender;
			sizeChars = System::Windows::Forms::TextRenderer::MeasureText("W", mipsDisassemblerControl->Font);
			numberLines = mipsDisassemblerControl->ClientRectangle.Height / sizeChars.Height;
			if ((mipsDisassemblerJoinedControlsList->Count > 0) && (tabControlDisassembles->SelectedIndex != -1))
			{
				if (mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Value >= 0)
					DrawRichTextBoxData(mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex], mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Value);
			}
		 }

		 void RemoveTab(int tabSelection)
		 {

			 mipsDisassemblerJoinedControlsList[tabSelection]->mipsDisassemblerControls->KeyDown -= gcnew System::Windows::Forms::KeyEventHandler(this, &MIPSMainForm::KeyDown);
			mipsDisassemblerJoinedControlsList[tabSelection]->mipsDisassemblerControls->KeyUp -= gcnew System::Windows::Forms::KeyEventHandler(this, &MIPSMainForm::KeyUp);
			mipsDisassemblerJoinedControlsList[tabSelection]->mipsDisassemblerControls->MouseWheel -= gcnew MIPSDisassembler::MouseEventHandler(this, &MIPSMainForm::MouseWheelDisassemblerEvent);

			mipsDisassemblerJoinedControlsList[tabSelection]->mipsDisassemblerControls->MouseUp -= gcnew System::Windows::Forms::MouseEventHandler(this, &MIPSMainForm::MouseClickDisassemblerEvent);
			mipsDisassemblerJoinedControlsList[tabSelection]->mipsDisassemblerControls->Resize -= gcnew System::EventHandler(this, &MIPSMainForm::OnResize);
			
			mipsDisassemblerJoinedControlsList[tabSelection]->vScrollBar->Scroll -= gcnew ScrollEventHandler(this, &MIPSMainForm::vScrollBar_Scroll);
			mipsDisassemblerJoinedControlsList[tabSelection]->mipsDisassemblerControls->MouseHover -= gcnew System::EventHandler(this, &MIPSMainForm::OnHover);
			mipsDisassemblerJoinedControlsList[tabSelection]->mipsDisassemblerControls->MouseMove -= gcnew System::Windows::Forms::MouseEventHandler(this, &MIPSMainForm::MouseMove);

			disassemblyContentsArray[tabSelection]->setOfLines = nullptr;
			disassemblyContentsArray[tabSelection]->binaryData = nullptr;
			disassemblyContentsArray[tabSelection]->instructions = nullptr;
			disassemblyContentsArray[tabSelection]->instructionSet = nullptr;

			disassemblyContentsArray->Remove(disassemblyContentsArray[tabSelection]);
			mipsDisassemblerJoinedControlsList->Remove(mipsDisassemblerJoinedControlsList[tabSelection]);
			tabControlDisassembles->TabPages->Remove(tabControlDisassembles->TabPages[tabSelection]);

			mipsDisassemblerSubroutineControlsList[tabSelection]->listView->DoubleClick -= gcnew EventHandler(this, &MIPSMainForm::DoubleClickListView);
			mipsDisassemblerSubroutineControlsList->Remove(mipsDisassemblerSubroutineControlsList[tabSelection]);
			tabControlViews->TabPages[0]->Controls->Clear();

			mipsDisassemblerLabelControlsList[tabSelection]->listView->DoubleClick -= gcnew EventHandler(this, &MIPSMainForm::DoubleClickListView);
			mipsDisassemblerLabelControlsList->Remove(mipsDisassemblerLabelControlsList[tabSelection]);
			tabControlViews->TabPages[1]->Controls->Clear();

			mipsDisassemblerFindAllControlsList[tabSelection]->listView->DoubleClick -= gcnew EventHandler(this, &MIPSMainForm::DoubleClickListView);
			mipsDisassemblerFindAllControlsList->Remove(mipsDisassemblerFindAllControlsList[tabSelection]);
			tabControlViews->TabPages[2]->Controls->Clear();

			if (tabSelection > 0)
			{
				tabControlDisassembles->SelectedIndex = (tabSelection - 1);
			}
			else if (tabControlDisassembles->TabCount > 0)
			{
				tabControlDisassembles->SelectedIndex = 0;
			}

			GC::Collect();
		 }

	void MouseWheelDisassemblerEvent(Object^ sender, MouseEventArgs^ e)
	{
		MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl = (MIPSDisassembler::MIPSDisassemblerControlsControl^)sender;	

		if (mipsDisassemblerControl->SelectionStart != -1)
		{
			int lineStart = mipsDisassemblerControl->GetLineFromCharIndex(mipsDisassemblerControl->SelectionStart);

			int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart);
			int charSameLineOffset = (mipsDisassemblerControl->SelectionStart - charStart);

			int change = -(e->Delta / 120);
			MIPSDisassembler::MIPSDisassemblerJoinedControl^ mipsDisassemblerJoinedControl = (MIPSDisassembler::MIPSDisassemblerJoinedControl^)mipsDisassemblerControl->Tag;
			if ((mipsDisassemblerJoinedControl->vScrollBar->Value + change) < 0)
				mipsDisassemblerJoinedControl->vScrollBar->Value = 0;
			else if ((mipsDisassemblerJoinedControl->vScrollBar->Value + change) > (disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count - 1))
				mipsDisassemblerJoinedControl->vScrollBar->Value = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count - 1;
			else
				mipsDisassemblerJoinedControl->vScrollBar->Value = mipsDisassemblerJoinedControl->vScrollBar->Value + change;

			SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);
			String^ getNextLine = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[mipsDisassemblerJoinedControl->vScrollBar->Value];
			SysWin32::LockWindowUpdate(IntPtr::Zero);

			DrawRichTextBoxData(mipsDisassemblerJoinedControl, mipsDisassemblerJoinedControl->vScrollBar->Value);

			SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);
			if (getNextLine->Length > charSameLineOffset)
				mipsDisassemblerControl->Select(mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart) + charSameLineOffset, 0);
			else
				mipsDisassemblerControl->Select(mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart) + (getNextLine->Length-1), 0);

			foundIndex = GetAbsoluteLineNumber(lineStart);
			SysWin32::LockWindowUpdate(IntPtr::Zero);

			Highlight(mipsDisassemblerControl);
		}
	}

	void MouseClickDisassemblerEvent(Object^ sender, MouseEventArgs^ e)
	{
		MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl = (MIPSDisassembler::MIPSDisassemblerControlsControl^)sender;		

		// set find spot
		if (mipsDisassemblerControl->SelectionStart == -1)
			foundIndex = 0;
		else
			foundIndex = GetAbsoluteLineNumber(mipsDisassemblerControl->GetLineFromCharIndex(mipsDisassemblerControl->SelectionStart));

		if (e->Button == System::Windows::Forms::MouseButtons::Right)
		{
			int charClickSel = mipsDisassemblerControl->GetCharIndexFromPosition(Point(e->X,e->Y));
			if (charClickSel == -1)
				return;

			// if out of range don't change
			if ((charClickSel < mipsDisassemblerControl->SelectionStart) || (charClickSel >= (mipsDisassemblerControl->SelectionStart + mipsDisassemblerControl->SelectionLength)))
			{
				mipsDisassemblerControl->Select(charClickSel, 0);
			}
			

			int start = mipsDisassemblerControl->SelectionStart;
			int length = mipsDisassemblerControl->SelectionLength;

			int startLine = mipsDisassemblerControl->GetLineFromCharIndex(start);
			int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(startLine);

			int endLine = mipsDisassemblerControl->GetLineFromCharIndex(start+length);
			int charEnd = mipsDisassemblerControl->GetFirstCharIndexFromLine(endLine);

			if (startLine != endLine)
			{
				return;
			}

			int charNext = mipsDisassemblerControl->GetFirstCharIndexFromLine(startLine + 1);

			if (charNext == -1)
			{
				charNext = charStart + mipsDisassemblerControl->Lines[startLine]->Length - 1;
			}

			try
			{
				SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);

				mipsDisassemblerControl->Select(charStart, (charNext - charStart));
				String^ currentLine = mipsDisassemblerControl->SelectedText; 
				if (currentLine->StartsWith("."))
				{
					mipsDisassemblerControl->Select(start, length);
					return;
				}

				unsigned long address = UInt32::Parse(currentLine->Substring(0, 8), System::Globalization::NumberStyles::HexNumber);
				IInstruction^ instruc = GetInstruction(tabControlDisassembles->SelectedIndex, address);

				int poundOffset = GetPoundOffset(mipsDisassemblerControl, currentLine, (charNext - charStart));

				int lineLength = currentLine->Length;
				// TODO check if xref
				if (((start + length) <= (charStart + 9)) && (!currentLine->StartsWith(".")))
				{
					renameAddressToolStripMenuItem->Visible = true;
					renameAddressToolStripMenuItem->Tag = currentLine;
					gotoXREFToolStripMenuItem->Visible = false;
					gotoAddressToolStripMenuItem->Visible = false;
					cutToolStripMenuItem->Visible = false;
					copyToolStripMenuItem->Visible = false;
					pasteToolStripMenuItem->Visible = false;
				}
				else if ((instruc->Target != 0xFFFFFFFF) && ((start + length) < (charStart + poundOffset)))
				{
					renameAddressToolStripMenuItem->Visible = false;
					gotoXREFToolStripMenuItem->Visible = false;
					gotoAddressToolStripMenuItem->Visible = true;
					gotoAddressToolStripMenuItem->Tag = instruc->Target;
					cutToolStripMenuItem->Visible = false;
					copyToolStripMenuItem->Visible = true;
					pasteToolStripMenuItem->Visible = false;
				}
				else if ((poundOffset == -1) && ((currentLine->IndexOf(".XREF") != -1))  && ((currentLine->IndexOf(".XREF") <= 10)))
				{
					renameAddressToolStripMenuItem->Visible = false;
					gotoXREFToolStripMenuItem->Tag = currentLine;
					gotoXREFToolStripMenuItem->Visible = true;
					gotoXREFToolStripMenuItem->Text = "Goto XREF";
					gotoAddressToolStripMenuItem->Visible = false;
					cutToolStripMenuItem->Visible = false;
					copyToolStripMenuItem->Visible = true;
					pasteToolStripMenuItem->Visible = false;
				}
				else if ((poundOffset == -1) && ((currentLine->IndexOf(".LREF") != -1))  && ((currentLine->IndexOf(".LREF") <= 10)))
				{
					renameAddressToolStripMenuItem->Visible = false;
					gotoXREFToolStripMenuItem->Tag = currentLine;
					gotoXREFToolStripMenuItem->Visible = true;
					gotoXREFToolStripMenuItem->Text = "Goto LREF";
					gotoAddressToolStripMenuItem->Visible = false;
					cutToolStripMenuItem->Visible = false;
					copyToolStripMenuItem->Visible = true;
					pasteToolStripMenuItem->Visible = false;
				}
				else if ((poundOffset == -1) && ((currentLine->IndexOf(".SREF") != -1))  && ((currentLine->IndexOf(".SREF") <= 10)))
				{
					renameAddressToolStripMenuItem->Visible = false;
					gotoXREFToolStripMenuItem->Tag = currentLine;
					gotoXREFToolStripMenuItem->Visible = true;
					gotoXREFToolStripMenuItem->Text = "Goto SREF";
					gotoAddressToolStripMenuItem->Visible = false;
					cutToolStripMenuItem->Visible = false;
					copyToolStripMenuItem->Visible = true;
					pasteToolStripMenuItem->Visible = false;
				}
				else if ((poundOffset == -1) && ((currentLine->IndexOf(".EXIT") != -1)))
				{
					renameAddressToolStripMenuItem->Visible = false;
					gotoXREFToolStripMenuItem->Visible = false;
					gotoAddressToolStripMenuItem->Visible = true;
					int exitOffset = currentLine->IndexOf(".EXIT");
					String^ subExitOffset = currentLine->Substring((exitOffset + 6), 8);
					gotoAddressToolStripMenuItem->Tag = UInt32::Parse(subExitOffset, System::Globalization::NumberStyles::HexNumber);
					cutToolStripMenuItem->Visible = false;
					copyToolStripMenuItem->Visible = true;
					pasteToolStripMenuItem->Visible = false;
				}
				else if ((poundOffset == -1) && ((currentLine->IndexOf("END SUBROUTINE") != -1)))
				{
					renameAddressToolStripMenuItem->Visible = false;
					gotoXREFToolStripMenuItem->Visible = false;
					gotoAddressToolStripMenuItem->Visible = true;
					int exitOffset = currentLine->IndexOf("END SUBROUTINE");
					String^ subExitOffset = currentLine->Substring((exitOffset + 15), 8);
					gotoAddressToolStripMenuItem->Tag = UInt32::Parse(subExitOffset, System::Globalization::NumberStyles::HexNumber);
					cutToolStripMenuItem->Visible = false;
					copyToolStripMenuItem->Visible = true;
					pasteToolStripMenuItem->Visible = false;
				}
				else if ((poundOffset != -1) && ((start) >= (charStart + poundOffset + 1)) && ((start + length) <= (charStart + lineLength - 1)))
				{
					renameAddressToolStripMenuItem->Visible = false;
					gotoXREFToolStripMenuItem->Visible = false;
					gotoAddressToolStripMenuItem->Visible = false;
					cutToolStripMenuItem->Visible = true;
					copyToolStripMenuItem->Visible = true;
					pasteToolStripMenuItem->Visible = true;
				}
				else
				{
					renameAddressToolStripMenuItem->Visible = false;
					gotoXREFToolStripMenuItem->Visible = false;
					gotoAddressToolStripMenuItem->Visible = false;
					cutToolStripMenuItem->Visible = false;
					copyToolStripMenuItem->Visible = true;
					pasteToolStripMenuItem->Visible = false;
				}
			}
			finally
			{
				mipsDisassemblerControl->Select(start, length);
				SysWin32::LockWindowUpdate(IntPtr::Zero);
			}
			

			Point clickScreenPoint = mipsDisassemblerControl->PointToScreen(Point(e->X, e->Y));
			contextMenuStripRichTextBox->Tag = mipsDisassemblerControl;
			contextMenuStripRichTextBox->Show(Point(clickScreenPoint));
		}

		Highlight(mipsDisassemblerControl);
	}

	void Highlight(MIPSDisassemblerControlsControl^ mipsDisassemblerControl)
	{
		int selectionStart = mipsDisassemblerControl->SelectionStart;
		int selectionLength = mipsDisassemblerControl->SelectionLength;
		int lineStart = mipsDisassemblerControl->GetLineFromCharIndex(selectionStart);
		int lineEnd = mipsDisassemblerControl->GetLineFromCharIndex(selectionStart+selectionLength);

		if (lineStart != lineEnd)
			return;

		try
		{
			SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);

			mipsDisassemblerControl->SelectAll();
			mipsDisassemblerControl->SelectionBackColor = System::Drawing::Color::White;

			if (selectionStart != -1)
			{
				int firstCharLine = mipsDisassemblerControl->GetFirstCharIndexFromLine(lineStart);
				String^ curLine = GetStringFromLine(lineStart, mipsDisassemblerControl);

				String^ wholeText = mipsDisassemblerControl->Text;
				String^ currentText = GetWord(wholeText, selectionStart);

				if ((!String::IsNullOrEmpty(currentText)) && (currentText->Contains("$")))
				{
					int poundIndex = GetPoundOffset(mipsDisassemblerControl, curLine, curLine->Length);
					int dollarSign = currentText->IndexOf("$");

					if ((selectionStart - firstCharLine) < poundIndex)
					{
						int commaSpot = currentText->IndexOf(",");
						int parenthSpot = currentText->IndexOf(")");

						if ((commaSpot == -1) && (parenthSpot == -1))
						{
							currentText = currentText->Substring(dollarSign);
						}
						else if (commaSpot == -1)
						{
							currentText = currentText->Substring(dollarSign, (parenthSpot - dollarSign));
						}
						else if (parenthSpot == -1)
						{
							currentText = currentText->Substring(dollarSign, (commaSpot - dollarSign));
						}
						else
						{
							if (commaSpot < parenthSpot)
								currentText = currentText->Substring(dollarSign, (commaSpot - dollarSign));
							else
								currentText = currentText->Substring(dollarSign, (parenthSpot - dollarSign));
						}

						int startIndex = wholeText->IndexOf(currentText, startIndex);
						while (startIndex != -1)
						{
							mipsDisassemblerControl->Select(startIndex, currentText->Length);
							mipsDisassemblerControl->SelectionBackColor = System::Drawing::Color::LightBlue;
							startIndex = startIndex + currentText->Length;
							startIndex = wholeText->IndexOf(currentText, startIndex);
						}
					}
				}
			}
		}
		finally
		{
			if (selectionStart != -1)
			{
				mipsDisassemblerControl->Select(selectionStart, selectionLength);
			}
			else
			{
				mipsDisassemblerControl->Select(0, 0);
			}
			SysWin32::LockWindowUpdate(IntPtr::Zero);
		}
	}

	String^ GetAddressFromLine(int lineNumber, MIPSDisassemblerControlsControl^ mipsDisassemblerControl)
	{
		String^ currentText = GetStringFromLine(lineNumber, mipsDisassemblerControl);
		if (currentText->StartsWith("."))
			return nullptr;
		else
			return currentText->Substring(0, 8);
	}

	String^ GetAddressFromCharStart(int start, MIPSDisassemblerControlsControl^ mipsDisassemblerControl)
	{
		String^ currentText = GetStringFromCharStart(start, mipsDisassemblerControl);
		if (currentText->StartsWith("."))
			return nullptr;
		else
			return currentText->Substring(0, 8);
	}

	int GetLengthOfLine(int startLine, MIPSDisassemblerControlsControl^ mipsDisassemblerControl)
	{
		int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(startLine);
		int charNext = mipsDisassemblerControl->GetFirstCharIndexFromLine(startLine + 1);

		if (charNext == -1)
		{
			return mipsDisassemblerControl->Lines[mipsDisassemblerControl->GetLineFromCharIndex(charStart)]->Length;
		}
		else
		{
			return (charNext - charStart);
		}

		
	}

	String^ GetStringFromLine(int startLine, MIPSDisassemblerControlsControl^ mipsDisassemblerControl)
	{
		int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(startLine);
		int charNext = mipsDisassemblerControl->GetFirstCharIndexFromLine(startLine + 1);

		if (charNext == -1)
		{
			String^ line = mipsDisassemblerControl->Lines[mipsDisassemblerControl->GetLineFromCharIndex(charStart)];
			line = line->Replace("\r\n", "")->Replace("\n", "")->Replace("\r", "");
			line = line + "\r\n";
			return line;
		}
		else
		{
			mipsDisassemblerControl->Select(charStart, (charNext - charStart));
			String^ line = mipsDisassemblerControl->SelectedText;
			line = line->Replace("\r\n", "")->Replace("\n", "")->Replace("\r", "");
			line = line + "\r\n";
			return line;
		}
		
	}

	String^ GetStringFromCharStart(int start, MIPSDisassemblerControlsControl^ mipsDisassemblerControl)
	{
		int lineNumber = mipsDisassemblerControl->GetLineFromCharIndex(start);
		return GetStringFromLine(lineNumber, mipsDisassemblerControl);
	}

	void CommitLineToInstructionArray(int midLine, MIPSDisassemblerControlsControl^ mipsDisassemblerControl)
	{
		if (disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[midLine]->StartsWith("."))
			return;

		String^ address = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[midLine]->Substring(0, 8);

		if (address == nullptr)
			return;
		
		int startLine = midLine - 1;
		if (startLine == -1)
			return;
		String^ addressStrPrev = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[startLine]->Substring(0, 8);

		// Find Start
		while ((startLine > 0) && (address == addressStrPrev))
		{
			startLine--;
			if (disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[startLine]->StartsWith("."))
				break;

			addressStrPrev = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[startLine]->Substring(0, 8);
			if (addressStrPrev == nullptr)
				break;
		}
		startLine++;

		// Find End
		int endLine = midLine + 1;
		if (endLine >= (disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count-1))
			return;
		if (!(disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[endLine]->StartsWith(".")))
		{
			String^ addressStrNext = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[endLine]->Substring(0, 8);
			while (address == addressStrNext)
			{
				endLine++;
				if (disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[endLine]->StartsWith("."))
					break;

				addressStrNext = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[endLine]->Substring(0, 8);
				if (addressStrNext == nullptr)
					break;
			}
			endLine--;
		}

		List<String^>^ comments = gcnew List<String^>();
		for (int x = startLine; x <= endLine; x++)
		{
			String^ currentLine = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[x];
			int poundOffset = GetPoundOffset(mipsDisassemblerControl, currentLine, currentLine->Length);
			if (poundOffset != -1)
			{
				String^ commentBlock = currentLine->Substring((poundOffset + 1), (currentLine->Length - (poundOffset + 1 + 1)));
				commentBlock = commentBlock->Replace("\r\n", "");
				comments->Add(commentBlock);
			}
		}

		unsigned long addressHex = UInt32::Parse(address, System::Globalization::NumberStyles::HexNumber);
		IInstruction^ currentInstruction = GetInstruction(tabControlDisassembles->SelectedIndex, addressHex);
		if (comments->Count == 0)
			currentInstruction->Comment = nullptr;
		else
			currentInstruction->Comment = comments;
		
	}

	private: int GetAbsoluteLineNumber(int lineNumber)
	 {
		int newLineNumber = mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Value + lineNumber;
		if (mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Value >= (disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count - numberLines))
		{
			newLineNumber = (disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count - numberLines + lineNumber);
		}
		return newLineNumber;
	 }

private: System::Void renameAddressToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 System::Windows::Forms::ToolStripMenuItem^ menu = (System::Windows::Forms::ToolStripMenuItem^)sender;
			 MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl = (MIPSDisassembler::MIPSDisassemblerControlsControl^)menu->GetCurrentParent()->Tag;

			int start = mipsDisassemblerControl->SelectionStart;
			int length = mipsDisassemblerControl->SelectionLength;

			int startLine = mipsDisassemblerControl->GetLineFromCharIndex(start);
			int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(startLine);

			int endLine = mipsDisassemblerControl->GetLineFromCharIndex(start+length);
			int charEnd = mipsDisassemblerControl->GetFirstCharIndexFromLine(endLine);
			
			if (startLine != endLine)
			{
				return;
			}

			int lineNumber = startLine;

			int charNext = mipsDisassemblerControl->GetFirstCharIndexFromLine(startLine + 1);

			if (charNext == -1)
			{
				charNext = charStart + mipsDisassemblerControl->Lines[startLine]->Length - 1;
			}

			String^ currentLine = (String^)menu->Tag;
			unsigned long address = UInt32::Parse(currentLine->Substring(0, 8), System::Globalization::NumberStyles::HexNumber);
			IInstruction^ currentInstruction = GetInstruction(tabControlDisassembles->SelectedIndex, address);
			String^ currentName = currentInstruction->Name;

			// popup
			InputDlg^ inputDlg = gcnew InputDlg();
			if (currentName != nullptr)
				inputDlg->textBoxValue->Text = currentName;
			inputDlg->textBoxValue->MaxLength = 32;
			inputDlg->labelDescription->Text = currentInstruction->Address.ToString("X8") + ": " + currentInstruction->InstructionText;
			inputDlg->Text = "Enter a name:";
			if (inputDlg->ShowDialog() != System::Windows::Forms::DialogResult::OK)
			{
				return;
			}

			String^ prevName = currentInstruction->Name;

			if (inputDlg->textBoxValue->Text == "")
				currentInstruction->Name = nullptr;
			else
				currentInstruction->Name = inputDlg->textBoxValue->Text;
			

			int endStart = start;
			int endLength = length;;

			try
			{
				SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);

				// draw text at topmost with a ., if not present, something like that
				String^  addressStr = GetAddressFromLine(startLine, mipsDisassemblerControl);
				String^  currentLine = GetStringFromLine(startLine, mipsDisassemblerControl);

				if ((currentLine->IndexOf("-") <= 10) && (currentLine->IndexOf("-") != -1))
				{
					lineNumber = GetAbsoluteLineNumber(startLine + 1);

					String^ addressStrNext = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[lineNumber]->Substring(0, 8);
					String^ nextText = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[lineNumber];
					while (
						(addressStr == addressStrNext)
						&& (addressStrNext != nullptr)

						&& (
						((nextText->IndexOf("-") <= 10))
						&& (nextText->IndexOf("-") != -1)
						)

						)
					{
						int index = nextText->IndexOf("-");
						bool isneg1 = (currentLine->IndexOf("-") != -1);
						lineNumber++;
						addressStrNext = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[lineNumber]->Substring(0, 8);
						nextText = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[lineNumber];
					}
				}
				else
				{

					lineNumber = GetAbsoluteLineNumber(startLine - 1);

					String^ addressStrPrev = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[lineNumber]->Substring(0, 8);
					String^ prevText = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[lineNumber];
					while (
						(lineNumber > 0)
						&& (addressStr == addressStrPrev)
						&& (addressStrPrev != nullptr)
						&& (((prevText->IndexOf("-") > 10)) || ((prevText->IndexOf("-") == -1)))
						)
					{
						lineNumber--;
						addressStrPrev = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[lineNumber]->Substring(0, 8);
						prevText = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[lineNumber];
					}
					lineNumber++;
				}
				
				String^ targetLine = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[lineNumber];
				int labelSpot = targetLine->IndexOf(".LABEL");


				if (labelSpot == -1)
				{
					if ((currentInstruction->Name != nullptr) && (currentInstruction->Name != ""))
					{
						// add line above
						disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Insert(lineNumber, String::Format("{0:X8}: .LABEL {1}\r\n", addressStr, currentInstruction->Name));
						endStart = 0;
						endLength = 0;

						mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Maximum++;
					}
				}
				else
				{
					// replace
					if ((currentInstruction->Name != nullptr) && (currentInstruction->Name != ""))
					{
						disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[lineNumber] = String::Format("{0:X8}: .LABEL {1}\r\n", addressStr, currentInstruction->Name);
						endStart = 0;
						endLength = 0;
					}
					else
					{
						disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->RemoveAt(lineNumber);
						endStart = 0;
						endLength = 0;

						mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Maximum--;
					}
				}

				// TODO replace branches/jumps
				DisassemblyContents^ disassemblyContents = disassemblyContentsArray[tabControlDisassembles->SelectedIndex];
				
				for (int x = 0; x < disassemblyContents->instructions->Length; x++)
				{
					if (disassemblyContents->instructions[x] != currentInstruction)
					{
						if (disassemblyContents->instructions[x]->Target == currentInstruction->Address)
						{
							String^ prevTargetStr;
							if ((prevName != nullptr) && (prevName != ""))
								prevTargetStr = " (" + prevName + ")";
							else
								prevTargetStr = "";

							String^ targetStr;
							if ((currentInstruction->Name != nullptr) && (currentInstruction->Name != ""))
								targetStr = " (" + currentInstruction->Name + ")";
							else
								targetStr = "";

							String^ matchStr = String::Format("{0:X8}:\t{1:X8}\t{2}{3}\t", disassemblyContents->instructions[x]->Address, disassemblyContents->instructions[x]->InstructionRaw, disassemblyContents->instructions[x]->InstructionText, prevTargetStr);
							String^ newStr   = String::Format("{0:X8}:\t{1:X8}\t{2}{3}\t", disassemblyContents->instructions[x]->Address, disassemblyContents->instructions[x]->InstructionRaw, disassemblyContents->instructions[x]->InstructionText, targetStr);
							
							List<String^>^ setOfLines = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines;
							for (int y = 0; y < setOfLines->Count; y++)
							{
								if (setOfLines[y]->IndexOf(matchStr) == 0)
								{
									setOfLines[y] = setOfLines[y]->Replace(matchStr, newStr);
									break;
								}
							}
							
						}
						else 
						{
							if ((disassemblyContents->instructions[x]->Parents != nullptr)
							&& (disassemblyContents->instructions[x]->Parents->Count > 0)
							&& (disassemblyContents->instructions[x]->Parents->Contains(currentInstruction))
							)
							{
								// find parent to rename j/b
								String^ xrefNameStrOld = "";
								if (
									(prevName != nullptr)
									&& (prevName != "")
									)
								{
									xrefNameStrOld = " (" + prevName + ")";
								}


								String^ upDown;
								if (currentInstruction->Address == disassemblyContents->instructions[x]->Address)
									upDown = "[E]";
								else if (currentInstruction->Address < disassemblyContents->instructions[x]->Address)
									upDown = "[U]";
								else
									upDown = "[D]";

								String^ instrType;
								if (currentInstruction->InstrType == InstructionType::Load)
								{
									instrType = ".LREF";
								}
								else if (currentInstruction->InstrType == InstructionType::Store)
								{
									instrType = ".SREF";
								}
								else
								{
									instrType = ".XREF";
								}

								String^ matchStr = String::Format("{3}:{0:X8}{1}{2}", currentInstruction->Address, upDown, xrefNameStrOld, instrType);
								String^ xrefNameStr = "";
								if (
									(currentInstruction->Name != nullptr)
									&& (currentInstruction->Name != "")
									)
								{
									xrefNameStr = " (" + currentInstruction->Name + ")";
								}
								String^ newStr = String::Format("{3}:{0:X8}{1}{2}", currentInstruction->Address, upDown, xrefNameStr, instrType);

								List<String^>^ setOfLines = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines;
								for (int y = 0; y < setOfLines->Count; y++)
								{
							
									int lineLength = setOfLines[y]->Length;
									String^ currentLineString = setOfLines[y];

									if (currentLineString->StartsWith(disassemblyContents->instructions[x]->Address.ToString("X8")))
									{
										int poundOffset = setOfLines[y]->IndexOf("#");
										if (poundOffset == -1)
										{
											setOfLines[y] = setOfLines[y]->Replace(matchStr, newStr);
										}
									}

									
								}
							}
						}
					}
				}

				// Update label
				if (mipsDisassemblerSubroutineControlsList[tabControlDisassembles->SelectedIndex]->targets->ContainsKey(currentInstruction->Address.ToString("X8")))
				{
					mipsDisassemblerSubroutineControlsList[tabControlDisassembles->SelectedIndex]->targets[currentInstruction->Address.ToString("X8")]->SubItems[1]->Text = currentInstruction->Name;
				}

				if (mipsDisassemblerLabelControlsList[tabControlDisassembles->SelectedIndex]->targets->ContainsKey(currentInstruction->Address.ToString("X8")))
				{
					if ((currentInstruction->Name != nullptr) && (currentInstruction->Name != ""))
					{
						mipsDisassemblerLabelControlsList[tabControlDisassembles->SelectedIndex]->targets[currentInstruction->Address.ToString("X8")]->SubItems[1]->Text = currentInstruction->Name;
					}
					else
					{
						mipsDisassemblerLabelControlsList[tabControlDisassembles->SelectedIndex]->listView->Items->Remove(mipsDisassemblerLabelControlsList[tabControlDisassembles->SelectedIndex]->targets[currentInstruction->Address.ToString("X8")]);
						mipsDisassemblerLabelControlsList[tabControlDisassembles->SelectedIndex]->targets->Remove(currentInstruction->Address.ToString("X8"));
					}
				}
				else
				{
					if ((currentInstruction->Name != nullptr) && (currentInstruction->Name != ""))
					{
						ListViewItem^ item = gcnew ListViewItem(currentInstruction->Address.ToString("X8"));
						item->SubItems->Add(currentInstruction->Name);
						mipsDisassemblerLabelControlsList[tabControlDisassembles->SelectedIndex]->targets->Add(currentInstruction->Address.ToString("X8"), item);
						mipsDisassemblerLabelControlsList[tabControlDisassembles->SelectedIndex]->listView->Items->Add(item);
					}
				}

			}
			finally
			{
				mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex]->vScrollBar->Value = lineNumber;
				DrawRichTextBoxData(mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex], (lineNumber));
				mipsDisassemblerControl->Select(endStart, endLength);
				SysWin32::LockWindowUpdate(IntPtr::Zero);
			}

		 }
private: System::Void gotoXREFToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 System::Windows::Forms::ToolStripMenuItem^ menu = (System::Windows::Forms::ToolStripMenuItem^)sender;
			 MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl = (MIPSDisassembler::MIPSDisassemblerControlsControl^)menu->GetCurrentParent()->Tag;
			 String^ refType = "." + menu->Text->Substring(menu->Text->Length-4, 4);

			int start = mipsDisassemblerControl->SelectionStart;
			int length = mipsDisassemblerControl->SelectionLength;

			int startLine = mipsDisassemblerControl->GetLineFromCharIndex(start);
			int charStart = mipsDisassemblerControl->GetFirstCharIndexFromLine(startLine);

			int endLine = mipsDisassemblerControl->GetLineFromCharIndex(start+length);
			int charEnd = mipsDisassemblerControl->GetFirstCharIndexFromLine(endLine);
			
			if (startLine != endLine)
			{
				return;
			}

			int offset = (start - charStart);
			String^ currentLine = (String^)menu->Tag;
			int xrefOffset = currentLine->LastIndexOf(refType, offset);

			if (xrefOffset == -1) // handle if at .
				xrefOffset = currentLine->IndexOf(refType, offset);

			String^ xrefAddress = currentLine->Substring(xrefOffset + 6, 8);

			try
			{
				GoToAddress(mipsDisassemblerControl, xrefAddress, nullptr);
			}
			finally
			{
				SysWin32::LockWindowUpdate(IntPtr::Zero);
			}
		 }

		 void GoToAddress(MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl, String^ address, String^ matchLine)
		 {
			 
			 try
			 {
				

				 UInt32 addyHex = (UInt32::Parse(address, System::Globalization::NumberStyles::HexNumber) & 0xFFFFFFFC);
				 if (IsValidLocation(tabControlDisassembles->SelectedIndex, addyHex))
				 {

					 address = addyHex.ToString("X8") + ":";
					SysWin32::LockWindowUpdate(mipsDisassemblerControl->Handle);

					 bool found = false;

					for (int currentLine = 0; currentLine < disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count; currentLine++)
					{
						int lineLength =  disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[currentLine]->Length;
						String^ currentLineString = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[currentLine];

						if (currentLineString->StartsWith(address))
						{
							if (matchLine != nullptr)
							{
								if (!(currentLineString->StartsWith(matchLine)))
									continue;
							}

							// found
							DrawRichTextBoxData(mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex], currentLine);
							
							foundIndex = currentLine;
							found = true;
							break;
						}
					}

					// ignore line match
					if (!found)
					{
						for (int currentLine = 0; currentLine < disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count; currentLine++)
						{
							int lineLength =  disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[currentLine]->Length;
							String^ currentLineString = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[currentLine];

							if (currentLineString->StartsWith(address))
							{
								// found
								DrawRichTextBoxData(mipsDisassemblerJoinedControlsList[tabControlDisassembles->SelectedIndex], currentLine);
								
								foundIndex = currentLine;
								found = true;
								break;
							}
						}
					}
				}
			}
			 finally
			 {
				 SysWin32::LockWindowUpdate(IntPtr::Zero);
			 }
		 }

private: System::Void saveDisassemblyToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			  if (tabControlDisassembles->SelectedIndex == -1)
				 return;

				DisassemblyContents^ disassemblyContext = disassemblyContentsArray[tabControlDisassembles->SelectedIndex];

			  SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog();
			 
			  saveFileDialog->Filter = "Disassembly XML files (*.xml)|*.xml";
			  //saveFileDialog->Filter = "Disassembly dsm files (*.dsm)|*.dsm";
			  saveFileDialog->FilterIndex = 1;
			  saveFileDialog->FileName = "disassembly.xml";
			  //saveFileDialog->FileName = "disassembly.dsm";
			  saveFileDialog->RestoreDirectory = true;

			  if ((saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) && (saveFileDialog->FileName != "") && (saveFileDialog->FileName != nullptr))
			  {
					DisassembleSerialized^ disassembleSerialized = gcnew DisassembleSerialized();
					
					disassembleSerialized->binaryData = gcnew List<DisassembledFileDetails^>(2);
					for (int x = 0; x < disassemblyContext->binaryData->Count; x++)
					{
						disassembleSerialized->binaryData->Add(gcnew DisassembledFileDetails(disassemblyContext->binaryData[x]->filename, disassemblyContext->binaryData[x]->startAddress, disassemblyContext->binaryData[x]->binarySize, nullptr));
						disassembleSerialized->binaryData[x]->offsetInInstructions = disassemblyContext->binaryData[x]->offsetInInstructions;
					}
					disassembleSerialized->disassemblyType = disassemblyContext->instructionSet->InstructionSetName;
					disassembleSerialized->instructions = gcnew List<InstructionSerialized^>(disassemblyContext->instructions->Length);
					for (int x = 0; x < disassemblyContext->instructions->Length; x++)
					{
						if (((disassemblyContext->instructions[x]->Name != nullptr) && (disassemblyContext->instructions[x]->Name != ""))
							|| ((disassemblyContext->instructions[x]->Comment != nullptr) && (disassemblyContext->instructions[x]->Comment->Count > 0)))
						{
							InstructionSerialized^ instructionSerialized = gcnew InstructionSerialized();
							instructionSerialized->address = disassemblyContext->instructions[x]->Address.ToString("X8");
							instructionSerialized->name = disassemblyContext->instructions[x]->Name;
							if (disassemblyContext->instructions[x]->Comment == nullptr)
								instructionSerialized->comments = nullptr;
							else
							{
								instructionSerialized->comments = gcnew List<String^>(disassemblyContext->instructions[x]->Comment->Count);
								for (int y = 0; y < disassemblyContext->instructions[x]->Comment->Count; y++)
									instructionSerialized->comments->Add(disassemblyContext->instructions[x]->Comment[y]->Replace("\r\n", "")->Replace("\n", ""));
							}
							disassembleSerialized->instructions->Add(instructionSerialized);
						}
					}

					try
					{
						XmlSerializer^ serializer = gcnew XmlSerializer( DisassembleSerialized::typeid );
						Stream^ fs = gcnew FileStream(saveFileDialog->FileName, FileMode::Create);
						XmlWriter^ writer = gcnew XmlTextWriter(fs, System::Text::Encoding::UTF8);
						serializer->Serialize(writer, disassembleSerialized);
						writer->Close();

						/*Stream^ fs = gcnew FileStream(saveFileDialog->FileName, FileMode::Create);
						BinaryFormatter^ bf = gcnew BinaryFormatter();
						bf->Serialize(fs, disassembleSerialized);
						fs->Close();*/
					}
					catch(Exception^ e)
					{
						MessageBox::Show(e->InnerException->ToString());
					}

			  }
		 }
private: System::Void openDisassemblyToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
			 
			  openFileDialog->Filter = "Disassembly XML files (*.xml)|*.xml";
			  //openFileDialog->Filter = "Disassembly dsm files (*.dsm)|*.dsm";
			  openFileDialog->FilterIndex = 1;
			  openFileDialog->FileName = "disassembly.xml";
			  //openFileDialog->FileName = "disassembly.dsm";
			  openFileDialog->RestoreDirectory = true;
			  openFileDialog->Multiselect = false;

			  if ((openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) && (openFileDialog->FileName != "") && (openFileDialog->FileName != nullptr))
			  {
				  DisassembleSerialized^ disassembleSerialized;

				  /*Stream^ fs;
				  try
				  {
					fs = gcnew FileStream(openFileDialog->FileName, FileMode::Open, System::IO::FileAccess::Read);
					BinaryFormatter^ bf = gcnew BinaryFormatter();
					disassembleSerialized = (DisassembleSerialized^)bf->Deserialize(fs);
				  }
				  catch(Exception^ e)
				  {
					  MessageBox::Show("Error deserializing " + e->ToString());
						return;
				  }
				  finally
				  {
						if (fs != nullptr)
							fs->Close();
				  }*/

				  Stream^ fs;
				  try
				  {
					fs = gcnew FileStream(openFileDialog->FileName, FileMode::Open, System::IO::FileAccess::Read);
					XmlSerializer^ serializer = gcnew XmlSerializer( disassembleSerialized::typeid );
					disassembleSerialized = (DisassembleSerialized^)serializer->Deserialize(fs);
				  }
				  catch(Exception^ e)
				  {
					  MessageBox::Show("Error deserializing " + e->ToString());
						return;
				  }
				  finally
				  {
						if (fs != nullptr)
							fs->Close();
				  }

				  

				  List<DisassembledFileDetails^>^ disassembledFileDetails = gcnew List<DisassembledFileDetails^>();
				  for (int x = 0; x < disassembleSerialized->binaryData->Count; x++)
				  {

					  array<unsigned char>^ binary = nullptr;
					  long binaryLength = 0;
					  if (!InitializeBinary(disassembleSerialized->binaryData[x]->filename, binary, binaryLength))
						  return;

					  disassembledFileDetails->Add(gcnew DisassembledFileDetails(disassembleSerialized->binaryData[x]->filename, disassembleSerialized->binaryData[x]->startAddress, binaryLength, binary));
				  }


				  MIPSDisassembler::disassembler::processorRow^ foundRow = nullptr;
				for (int x = 0; x < configFile->tab->Count; x++)
				{
					for each (MIPSDisassembler::disassembler::processorRow^  procRow in configFile->tab[x]->GetprocessorRows())
					{
						if (procRow->name == disassembleSerialized->disassemblyType)
						{
							foundRow = procRow;
							break;
						}
					}
				}

				if (foundRow == nullptr)
				{
					MessageBox::Show("Error cannot find processor " + disassembleSerialized->disassemblyType);
					return;
				}

				  IInstructionSet^ instructionSet = DisassemblerSharedFunctions::ActivateInstructionSet(foundRow->DLL, foundRow->_class);					
					if (instructionSet == nullptr)
					{

						MessageBox::Show("Error starting processor ");
						return;
					}

					DisassemblyContents^ disassemblyContents = gcnew DisassemblyContents();
					// call on new thread
					disassemblyContents->SetDisassemblyVariables(instructionSet, disassembledFileDetails, false);

					ProgressBarDlg^ processBarDlg = gcnew ProgressBarDlg(gcnew ProgressRunDelegate(disassemblyContents, &MIPSDisassembler::DisassemblyContents::Disassemble), gcnew GetProgressDelegate(disassemblyContents, &MIPSDisassembler::DisassemblyContents::GetProgress));
					if (processBarDlg->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					{
						if ((disassembleSerialized->instructions != nullptr) && (disassembleSerialized->instructions->Count > 0))
						{
							for (int x = 0; x < disassembleSerialized->instructions->Count; x++)
							{
								unsigned long offset = UInt32::Parse(disassembleSerialized->instructions[x]->address, System::Globalization::NumberStyles::HexNumber);
								IInstruction^ currentInstruction = GetInstruction(disassemblyContents, offset);
								if (disassembleSerialized->instructions[x]->comments == nullptr)
								{
									currentInstruction->Comment = nullptr;
								}
								else
								{
									currentInstruction->Comment = gcnew List<String^>();
									for (int y = 0; y < disassembleSerialized->instructions[x]->comments->Count; y++)
									{
										currentInstruction->Comment->Add(disassembleSerialized->instructions[x]->comments[y]->Replace("\r\n", "")->Replace("\n", ""));
									}
								}
								currentInstruction->Name = disassembleSerialized->instructions[x]->name;
							}
						}

						AddTab(disassemblyContents);
					}

			   }
		 }

		 void SwitchTabs()
		 {
			if (tabControlDisassembles->SelectedIndex != -1)
			 {
				for (int x = 0; x < tabControlDisassembles->TabCount; x++)
				{
					if (x == tabControlDisassembles->SelectedIndex)
					{
						mipsDisassemblerSubroutineControlsList[x]->Visible = true;
						mipsDisassemblerLabelControlsList[x]->Visible = true;
						mipsDisassemblerFindAllControlsList[x]->Visible = true;
					}
					else
					{
						mipsDisassemblerSubroutineControlsList[x]->Visible = false;
						mipsDisassemblerLabelControlsList[x]->Visible = false;
						mipsDisassemblerFindAllControlsList[x]->Visible = false;
					}
				}
			 }
		 }
private: System::Void tabControlDisassembles_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 SwitchTabs();
		 }

private: System::Void gotoAddressToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

			 System::Windows::Forms::ToolStripMenuItem^ menu = (System::Windows::Forms::ToolStripMenuItem^)sender;
			 MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl = (MIPSDisassembler::MIPSDisassemblerControlsControl^)menu->GetCurrentParent()->Tag;

			unsigned long target = (unsigned long)menu->Tag;
			
			try
			{
				GoToAddress(mipsDisassemblerControl, target.ToString("X8"), nullptr);
			}
			finally
			{
				SysWin32::LockWindowUpdate(IntPtr::Zero);
			}

		 }
private: System::Void cutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 System::Windows::Forms::ToolStripMenuItem^ menu = (System::Windows::Forms::ToolStripMenuItem^)sender;
			 MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl = (MIPSDisassembler::MIPSDisassemblerControlsControl^)menu->GetCurrentParent()->Tag;
			 mipsDisassemblerControl->Cut();
		 }
private: System::Void copyToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 System::Windows::Forms::ToolStripMenuItem^ menu = (System::Windows::Forms::ToolStripMenuItem^)sender;
			 MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl = (MIPSDisassembler::MIPSDisassemblerControlsControl^)menu->GetCurrentParent()->Tag;
			mipsDisassemblerControl->Copy();
		 }
private: System::Void pasteToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 System::Windows::Forms::ToolStripMenuItem^ menu = (System::Windows::Forms::ToolStripMenuItem^)sender;
			 MIPSDisassembler::MIPSDisassemblerControlsControl^ mipsDisassemblerControl = (MIPSDisassembler::MIPSDisassemblerControlsControl^)menu->GetCurrentParent()->Tag;

			 if (Clipboard::ContainsText())
			 {
				System::Windows::Forms::IDataObject^ clipData = System::Windows::Forms::Clipboard::GetDataObject();
				if (clipData->GetDataPresent(DataFormats::Text))
				{
						String^ s = System::Windows::Forms::Clipboard::GetData(DataFormats::Text)->ToString()->Replace("\r\n", "")->Replace("\n", "")->Replace("\r", "");

						if (mipsDisassemblerControl->SelectionStart != -1)
						{
							mipsDisassemblerControl->SelectedText = s;
						}
				}
			 }
		 }
		 
private: System::Void vScrollBar_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) {

			VScrollBar^ scrollBar = (VScrollBar^)sender;
			MIPSDisassembler::MIPSDisassemblerJoinedControl^ mipsDisassemblerJoinedControl = (MIPSDisassembler::MIPSDisassemblerJoinedControl^)scrollBar->Tag;
			DrawRichTextBoxData(mipsDisassemblerJoinedControl, e->NewValue);
		 }

		 private: void DrawRichTextBoxData(MIPSDisassembler::MIPSDisassemblerJoinedControl^ mipsDisassemblerJoinedControl, int offset)
		  {
			  mipsDisassemblerJoinedControl->vScrollBar->Value = offset;
			  int totalLines = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines->Count;
			SysWin32::LockWindowUpdate(mipsDisassemblerJoinedControl->mipsDisassemblerControls->Handle);

			int scrolledValue = mipsDisassemblerJoinedControl->vScrollBar->Value;
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->Text = "";			
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->ScrollBars = RichTextBoxScrollBars::None;
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->SelectionTabs = gcnew array<int>(3) { (sizeChars.Width * 9), (sizeChars.Width * 9)*2, ((sizeChars.Width *9)*4) }; 

			int end = offset + numberLines;
			if (end > totalLines)
			{
				end = totalLines;
				offset = end - numberLines;
				if (offset < 0)
					offset = 0;
				mipsDisassemblerJoinedControl->vScrollBar->Value = offset;
			}
			for (int x = offset; x < end; x++)
			{
				if (x != (end - 1))
					mipsDisassemblerJoinedControl->mipsDisassemblerControls->AppendText(disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[x]);
				else
					mipsDisassemblerJoinedControl->mipsDisassemblerControls->AppendText(disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[x]->Replace("\r\n", ""));
			}
			//MessageBox::Show(String::Format("{0:X8} {1:X8}", disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[offset], disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines[end-1]));
			//Colorize(mipsDisassemblerJoinedControl->mipsDisassemblerControls, false);

			List<IColorIndexes^>^ colorIndexes = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->instructionSet->Colorize(mipsDisassemblerJoinedControl->mipsDisassemblerControls->Text);
			for (int x = 0; x < colorIndexes->Count; x++)
			{
				SysWin32::SendMessage(mipsDisassemblerJoinedControl->mipsDisassemblerControls->Handle, 0xB1, (colorIndexes[x]->Offset), (colorIndexes[x]->Offset + colorIndexes[x]->Length));
				//mipsDisassemblerControl->Select((startChar + colorIndexes[x]->Offset), colorIndexes[x]->Length);
				mipsDisassemblerJoinedControl->mipsDisassemblerControls->SelectionColor = colorIndexes[x]->ColorValue;
			}

			if (end == totalLines)
				mipsDisassemblerJoinedControl->mipsDisassemblerControls->Select((end-1), 0);
			else
				mipsDisassemblerJoinedControl->mipsDisassemblerControls->Select(0, 0);
			mipsDisassemblerJoinedControl->mipsDisassemblerControls->ScrollToCaret();
			SysWin32::LockWindowUpdate(IntPtr::Zero);
		  }
private: System::Void closeDisassemblyToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (tabControlDisassembles->SelectedIndex == -1)
				 return;
			RemoveTab(tabControlDisassembles->SelectedIndex);

		 }
private: System::Void exportDisassemblyTotxtToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			  if (tabControlDisassembles->SelectedIndex == -1)
				 return;

			  SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog();
			 
			  saveFileDialog->Filter = "Disassembly Text files (*.txt)|*.txt";
			  //saveFileDialog->Filter = "Disassembly dsm files (*.dsm)|*.dsm";
			  saveFileDialog->FilterIndex = 1;
			  saveFileDialog->FileName = "disassembly.txt";
			  //saveFileDialog->FileName = "disassembly.dsm";
			  saveFileDialog->RestoreDirectory = true;

			  if ((saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) && (saveFileDialog->FileName != "") && (saveFileDialog->FileName != nullptr))
			  {
				  StreamWriter^ sw = gcnew StreamWriter(saveFileDialog->FileName);

				  List<String^>^ setOfLines = disassemblyContentsArray[tabControlDisassembles->SelectedIndex]->setOfLines;
				  for (int x = 0; x < setOfLines->Count; x++)
				  {
					sw->Write(setOfLines[x]);
				  }
				  sw->Close();
			  }
		 }
private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 Close();
		 }
private: System::Void exportFindResultsTotxtToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (tabControlDisassembles->SelectedIndex != -1)
			 {
				 SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog();
			 
				  saveFileDialog->Filter = "Find Result Text files (*.txt)|*.txt";
				  saveFileDialog->FilterIndex = 1;
				  saveFileDialog->FileName = "findresults.txt";
				  saveFileDialog->RestoreDirectory = true;

				  if ((saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) && (saveFileDialog->FileName != "") && (saveFileDialog->FileName != nullptr))
				  {
					  StreamWriter^ sw = gcnew StreamWriter(saveFileDialog->FileName);

						for each(KeyValuePair<String^, ListViewItem^>^ kvp in mipsDisassemblerFindAllControlsList[tabControlDisassembles->SelectedIndex]->targets)
						{
							if (kvp->Value->SubItems->Count > 0)
							{
								for (int x = 1; x < kvp->Value->SubItems->Count; x++)
								{
									sw->Write(" " + kvp->Value->SubItems[x]->Text);
								}
							}
						}

						sw->Close();
				  }
			}
		 }
};
}

