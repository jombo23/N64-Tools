#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "MIPSDisassemblerControlsControl.h"

namespace MIPSDisassembler {

	/// <summary>
	/// Summary for MIPSDisassemblerJoinedControl
	/// </summary>
	public ref class MIPSDisassemblerJoinedControl : public System::Windows::Forms::UserControl
	{
	public:
		MIPSDisassemblerJoinedControl(void)
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
		~MIPSDisassemblerJoinedControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::SplitContainer^  splitContainerMain;
	protected: 
	public: MIPSDisassembler::MIPSDisassemblerControlsControl^  mipsDisassemblerControls;
	public: System::Windows::Forms::VScrollBar^  vScrollBar;

	private:
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
			this->splitContainerMain = (gcnew System::Windows::Forms::SplitContainer());
			this->vScrollBar = (gcnew System::Windows::Forms::VScrollBar());
			this->mipsDisassemblerControls = (gcnew MIPSDisassembler::MIPSDisassemblerControlsControl());
			this->splitContainerMain->Panel1->SuspendLayout();
			this->splitContainerMain->Panel2->SuspendLayout();
			this->splitContainerMain->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainerMain
			// 
			this->splitContainerMain->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerMain->IsSplitterFixed = true;
			this->splitContainerMain->Location = System::Drawing::Point(0, 0);
			this->splitContainerMain->Name = L"splitContainerMain";
			// 
			// splitContainerMain.Panel1
			// 
			this->splitContainerMain->Panel1->Controls->Add(this->mipsDisassemblerControls);
			// 
			// splitContainerMain.Panel2
			// 
			this->splitContainerMain->Panel2->Controls->Add(this->vScrollBar);
			this->splitContainerMain->Size = System::Drawing::Size(601, 366);
			this->splitContainerMain->SplitterDistance = 572;
			this->splitContainerMain->TabIndex = 0;
			// 
			// vScrollBar
			// 
			this->vScrollBar->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->vScrollBar->Location = System::Drawing::Point(0, 0);
			this->vScrollBar->Name = L"vScrollBar";
			this->vScrollBar->Size = System::Drawing::Size(25, 366);
			this->vScrollBar->TabIndex = 0;
			// 
			// mipsDisassemblerControls
			// 
			this->mipsDisassemblerControls->Dock = System::Windows::Forms::DockStyle::Fill;
			this->mipsDisassemblerControls->Location = System::Drawing::Point(0, 0);
			this->mipsDisassemblerControls->Name = L"mipsDisassemblerControls";
			this->mipsDisassemblerControls->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::None;
			this->mipsDisassemblerControls->Size = System::Drawing::Size(572, 366);
			this->mipsDisassemblerControls->TabIndex = 0;
			this->mipsDisassemblerControls->Text = L"";
			// 
			// MIPSDisassemblerJoinedControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainerMain);
			this->Name = L"MIPSDisassemblerJoinedControl";
			this->Size = System::Drawing::Size(601, 366);
			this->splitContainerMain->Panel1->ResumeLayout(false);
			this->splitContainerMain->Panel2->ResumeLayout(false);
			this->splitContainerMain->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
