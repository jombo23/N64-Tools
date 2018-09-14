#pragma once
#include "DisassemblerConfiguration.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MIPSDisassembler {

	/// <summary>
	/// Summary for ProcessorChoiceDlg
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ProcessorChoiceDlg : public System::Windows::Forms::Form
	{
	private:
		disassembler^ ConfigFile;
	private: System::Windows::Forms::SplitContainer^  splitContainerListviewAddress;
	private: System::Windows::Forms::TextBox^  textBoxStartAddress;
	private: System::Windows::Forms::Label^  labelStartingAddress;
			 MIPSDisassembler::disassembler::processorRow^ processorReturnRow;
	public: System::Windows::Forms::CheckBox^  checkBoxTLB;
	private: System::Windows::Forms::TextBox^  textBoxTLBAddress;
			 unsigned long processorReturnStartAddress;


			 unsigned long processorReturnStartAddressTLB;
	public:
		ProcessorChoiceDlg(disassembler^ configFile)
		{
			ConfigFile = configFile;
			InitializeComponent();
			LoadTabs();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ProcessorChoiceDlg()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  tabControlProcessorChoice;
	protected: 
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::ListView^  listViewProcessorChoice;
	private: System::Windows::Forms::SplitContainer^  splitContainerProcessorChoice;
	private: System::Windows::Forms::Button^  buttonCancel;
	private: System::Windows::Forms::Button^  buttonOK;


		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->tabControlProcessorChoice = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->listViewProcessorChoice = (gcnew System::Windows::Forms::ListView());
			this->splitContainerProcessorChoice = (gcnew System::Windows::Forms::SplitContainer());
			this->splitContainerListviewAddress = (gcnew System::Windows::Forms::SplitContainer());
			this->textBoxTLBAddress = (gcnew System::Windows::Forms::TextBox());
			this->checkBoxTLB = (gcnew System::Windows::Forms::CheckBox());
			this->textBoxStartAddress = (gcnew System::Windows::Forms::TextBox());
			this->labelStartingAddress = (gcnew System::Windows::Forms::Label());
			this->buttonCancel = (gcnew System::Windows::Forms::Button());
			this->buttonOK = (gcnew System::Windows::Forms::Button());
			this->tabControlProcessorChoice->SuspendLayout();
			this->splitContainerProcessorChoice->Panel1->SuspendLayout();
			this->splitContainerProcessorChoice->Panel2->SuspendLayout();
			this->splitContainerProcessorChoice->SuspendLayout();
			this->splitContainerListviewAddress->Panel1->SuspendLayout();
			this->splitContainerListviewAddress->Panel2->SuspendLayout();
			this->splitContainerListviewAddress->SuspendLayout();
			this->SuspendLayout();
			// 
			// tabControlProcessorChoice
			// 
			this->tabControlProcessorChoice->Controls->Add(this->tabPage1);
			this->tabControlProcessorChoice->Controls->Add(this->tabPage2);
			this->tabControlProcessorChoice->Dock = System::Windows::Forms::DockStyle::Top;
			this->tabControlProcessorChoice->Location = System::Drawing::Point(0, 0);
			this->tabControlProcessorChoice->Name = L"tabControlProcessorChoice";
			this->tabControlProcessorChoice->SelectedIndex = 0;
			this->tabControlProcessorChoice->Size = System::Drawing::Size(478, 24);
			this->tabControlProcessorChoice->TabIndex = 0;
			this->tabControlProcessorChoice->SelectedIndexChanged += gcnew System::EventHandler(this, &ProcessorChoiceDlg::tabControlProcessorChoice_SelectedIndexChanged);
			// 
			// tabPage1
			// 
			this->tabPage1->Location = System::Drawing::Point(4, 25);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(470, 0);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"tabPage1";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// tabPage2
			// 
			this->tabPage2->Location = System::Drawing::Point(4, 25);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(470, 0);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"tabPage2";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// listViewProcessorChoice
			// 
			this->listViewProcessorChoice->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listViewProcessorChoice->FullRowSelect = true;
			this->listViewProcessorChoice->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->listViewProcessorChoice->HideSelection = false;
			this->listViewProcessorChoice->Location = System::Drawing::Point(0, 0);
			this->listViewProcessorChoice->MultiSelect = false;
			this->listViewProcessorChoice->Name = L"listViewProcessorChoice";
			this->listViewProcessorChoice->Size = System::Drawing::Size(478, 256);
			this->listViewProcessorChoice->TabIndex = 1;
			this->listViewProcessorChoice->UseCompatibleStateImageBehavior = false;
			// 
			// splitContainerProcessorChoice
			// 
			this->splitContainerProcessorChoice->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerProcessorChoice->Location = System::Drawing::Point(0, 24);
			this->splitContainerProcessorChoice->Name = L"splitContainerProcessorChoice";
			this->splitContainerProcessorChoice->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainerProcessorChoice.Panel1
			// 
			this->splitContainerProcessorChoice->Panel1->Controls->Add(this->splitContainerListviewAddress);
			// 
			// splitContainerProcessorChoice.Panel2
			// 
			this->splitContainerProcessorChoice->Panel2->Controls->Add(this->buttonCancel);
			this->splitContainerProcessorChoice->Panel2->Controls->Add(this->buttonOK);
			this->splitContainerProcessorChoice->Size = System::Drawing::Size(478, 337);
			this->splitContainerProcessorChoice->SplitterDistance = 300;
			this->splitContainerProcessorChoice->TabIndex = 2;
			// 
			// splitContainerListviewAddress
			// 
			this->splitContainerListviewAddress->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerListviewAddress->Location = System::Drawing::Point(0, 0);
			this->splitContainerListviewAddress->Name = L"splitContainerListviewAddress";
			this->splitContainerListviewAddress->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainerListviewAddress.Panel1
			// 
			this->splitContainerListviewAddress->Panel1->Controls->Add(this->listViewProcessorChoice);
			// 
			// splitContainerListviewAddress.Panel2
			// 
			this->splitContainerListviewAddress->Panel2->Controls->Add(this->textBoxTLBAddress);
			this->splitContainerListviewAddress->Panel2->Controls->Add(this->checkBoxTLB);
			this->splitContainerListviewAddress->Panel2->Controls->Add(this->textBoxStartAddress);
			this->splitContainerListviewAddress->Panel2->Controls->Add(this->labelStartingAddress);
			this->splitContainerListviewAddress->Size = System::Drawing::Size(478, 300);
			this->splitContainerListviewAddress->SplitterDistance = 256;
			this->splitContainerListviewAddress->TabIndex = 2;
			// 
			// textBoxTLBAddress
			// 
			this->textBoxTLBAddress->Location = System::Drawing::Point(324, 4);
			this->textBoxTLBAddress->Name = L"textBoxTLBAddress";
			this->textBoxTLBAddress->Size = System::Drawing::Size(138, 22);
			this->textBoxTLBAddress->TabIndex = 3;
			this->textBoxTLBAddress->Text = L"7F000000";
			// 
			// checkBoxTLB
			// 
			this->checkBoxTLB->AutoSize = true;
			this->checkBoxTLB->Location = System::Drawing::Point(262, 4);
			this->checkBoxTLB->Name = L"checkBoxTLB";
			this->checkBoxTLB->Size = System::Drawing::Size(56, 21);
			this->checkBoxTLB->TabIndex = 2;
			this->checkBoxTLB->Text = L"TLB";
			this->checkBoxTLB->UseVisualStyleBackColor = true;
			this->checkBoxTLB->CheckedChanged += gcnew System::EventHandler(this, &ProcessorChoiceDlg::checkBoxTLB_CheckedChanged);
			// 
			// textBoxStartAddress
			// 
			this->textBoxStartAddress->Location = System::Drawing::Point(117, 5);
			this->textBoxStartAddress->Name = L"textBoxStartAddress";
			this->textBoxStartAddress->Size = System::Drawing::Size(138, 22);
			this->textBoxStartAddress->TabIndex = 1;
			this->textBoxStartAddress->Text = L"80000000";
			// 
			// labelStartingAddress
			// 
			this->labelStartingAddress->AutoSize = true;
			this->labelStartingAddress->Location = System::Drawing::Point(12, 6);
			this->labelStartingAddress->Name = L"labelStartingAddress";
			this->labelStartingAddress->Size = System::Drawing::Size(98, 17);
			this->labelStartingAddress->TabIndex = 0;
			this->labelStartingAddress->Text = L"Start Address:";
			this->labelStartingAddress->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// buttonCancel
			// 
			this->buttonCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->buttonCancel->Location = System::Drawing::Point(198, 6);
			this->buttonCancel->Name = L"buttonCancel";
			this->buttonCancel->Size = System::Drawing::Size(108, 23);
			this->buttonCancel->TabIndex = 1;
			this->buttonCancel->Text = L"Cancel";
			this->buttonCancel->UseVisualStyleBackColor = true;
			// 
			// buttonOK
			// 
			this->buttonOK->Location = System::Drawing::Point(84, 6);
			this->buttonOK->Name = L"buttonOK";
			this->buttonOK->Size = System::Drawing::Size(108, 23);
			this->buttonOK->TabIndex = 0;
			this->buttonOK->Text = L"OK";
			this->buttonOK->UseVisualStyleBackColor = true;
			this->buttonOK->Click += gcnew System::EventHandler(this, &ProcessorChoiceDlg::buttonOK_Click);
			// 
			// ProcessorChoiceDlg
			// 
			this->AcceptButton = this->buttonOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->buttonCancel;
			this->ClientSize = System::Drawing::Size(478, 361);
			this->Controls->Add(this->splitContainerProcessorChoice);
			this->Controls->Add(this->tabControlProcessorChoice);
			this->Name = L"ProcessorChoiceDlg";
			this->Text = L"Processor";
			this->tabControlProcessorChoice->ResumeLayout(false);
			this->splitContainerProcessorChoice->Panel1->ResumeLayout(false);
			this->splitContainerProcessorChoice->Panel2->ResumeLayout(false);
			this->splitContainerProcessorChoice->ResumeLayout(false);
			this->splitContainerListviewAddress->Panel1->ResumeLayout(false);
			this->splitContainerListviewAddress->Panel2->ResumeLayout(false);
			this->splitContainerListviewAddress->Panel2->PerformLayout();
			this->splitContainerListviewAddress->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
		void SelectedTabChanged()
		{
			listViewProcessorChoice->Clear();
			 if (tabControlProcessorChoice->SelectedIndex != -1)
			 {
				 MIPSDisassembler::disassembler::tabRow^ selectedTab = ConfigFile->tab[tabControlProcessorChoice->SelectedIndex];

				 for each (MIPSDisassembler::disassembler::processorRow^  procRow in selectedTab->GetprocessorRows())
				 {
					 listViewProcessorChoice->Items->Add(procRow->name);
				 }

				 if (listViewProcessorChoice->Items->Count > 0)
				 {
					 listViewProcessorChoice->Items[0]->Selected = true;
				 }
			 }
		}
	private: System::Void tabControlProcessorChoice_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 SelectedTabChanged();
			 }

	private: void ProcessorChoiceDlg::LoadTabs()
		 {
			tabControlProcessorChoice->TabPages->Clear();
			for (int x = 0; x < ConfigFile->tab->Count; x++)
			{
				tabControlProcessorChoice->TabPages->Add(ConfigFile->tab[x]->name);
			}
			tabControlProcessorChoice->SelectedIndex = 0;
			SelectedTabChanged();
		 }
private: System::Void buttonOK_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (tabControlProcessorChoice->SelectedIndex != -1)
			 {
				 MIPSDisassembler::disassembler::tabRow^ selectedTab = ConfigFile->tab[tabControlProcessorChoice->SelectedIndex];

				 if ((listViewProcessorChoice->Items->Count == 0) || (listViewProcessorChoice->SelectedItems->Count == 0))
				 {
					 MessageBox::Show("No processor selected");
					 DialogResult = System::Windows::Forms::DialogResult::None;
					 return;
				 }
				 else
				 {
					 int selectedProcessor = listViewProcessorChoice->SelectedItems[0]->Index;
					 processorReturnRow = selectedTab->GetprocessorRows()[selectedProcessor];

					if (textBoxStartAddress->Text)
					{
						try
						{
							processorReturnStartAddress = long::Parse(textBoxStartAddress->Text, System::Globalization::NumberStyles::HexNumber);
						}
						catch (Exception^ e)
						{
							MessageBox::Show("Invalid start address, must be hex");
							DialogResult = System::Windows::Forms::DialogResult::None;
							return;
						}
					}

					if (checkBoxTLB->Checked)
					{
						if (textBoxTLBAddress->Text)
						{
							try
							{
								processorReturnStartAddressTLB = long::Parse(textBoxTLBAddress->Text, System::Globalization::NumberStyles::HexNumber);
							}
							catch (Exception^ e)
							{
								MessageBox::Show("Invalid tlb address, must be hex");
								DialogResult = System::Windows::Forms::DialogResult::None;
								return;
							}
						}
					}


					DialogResult = System::Windows::Forms::DialogResult::OK;
				 }
				 
			 }
		 }
	
public:
   property MIPSDisassembler::disassembler::processorRow^ ProcessorReturnRow
   {
      MIPSDisassembler::disassembler::processorRow^ get()
      {
         return processorReturnRow;
      }
   }

    property unsigned long ProcessorReturnStartAddress
   {
      unsigned long get()
      {
			return processorReturnStartAddress;
      }
   }

	  property unsigned long ProcessorReturnStartAddressTlb
   {
      unsigned long get()
      {
			return processorReturnStartAddressTLB;
      }
   }

	  


private: System::Void checkBoxTLB_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 System::Windows::Forms::CheckBox^ checkBox = (System::Windows::Forms::CheckBox^)sender;
			 if (checkBox->Checked)
				 textBoxTLBAddress->Enabled = true;
			 else
				 textBoxTLBAddress->Enabled = false;

		 }
};
}
