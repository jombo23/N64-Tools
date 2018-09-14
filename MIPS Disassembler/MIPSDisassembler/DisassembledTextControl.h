#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;



namespace MIPSDisassembler {

	/// <summary>
	/// Summary for DisassembledTextControl
	/// </summary>
	public ref class DisassembledTextControl : public System::Windows::Forms::UserControl
	{
	public:
		DisassembledTextControl(void)
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
		~DisassembledTextControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: MIPSDisassemblerControls::MIPSDisassemblerControlsControl^  richTextBoxDisassemble;
	protected: 
	private: System::Windows::Forms::SplitContainer^  splitContainerDisassembleMain;
	private: System::Windows::Forms::VScrollBar^  vScrollBarDisassemble;

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
			this->richTextBoxDisassemble = (gcnew MIPSDisassemblerControls::MIPSDisassemblerControlsControl());
			this->splitContainerDisassembleMain = (gcnew System::Windows::Forms::SplitContainer());
			this->vScrollBarDisassemble = (gcnew System::Windows::Forms::VScrollBar());
			this->splitContainerDisassembleMain->Panel1->SuspendLayout();
			this->splitContainerDisassembleMain->Panel2->SuspendLayout();
			this->splitContainerDisassembleMain->SuspendLayout();
			this->SuspendLayout();
			// 
			// richTextBoxDisassemble
			// 
			this->richTextBoxDisassemble->Dock = System::Windows::Forms::DockStyle::Fill;
			this->richTextBoxDisassemble->Location = System::Drawing::Point(0, 0);
			this->richTextBoxDisassemble->Name = L"richTextBoxDisassemble";
			this->richTextBoxDisassemble->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::None;
			this->richTextBoxDisassemble->Size = System::Drawing::Size(818, 617);
			this->richTextBoxDisassemble->TabIndex = 0;
			this->richTextBoxDisassemble->Text = L"";
			// 
			// splitContainerDisassembleMain
			// 
			this->splitContainerDisassembleMain->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainerDisassembleMain->Location = System::Drawing::Point(0, 0);
			this->splitContainerDisassembleMain->Name = L"splitContainerDisassembleMain";
			// 
			// splitContainerDisassembleMain.Panel1
			// 
			this->splitContainerDisassembleMain->Panel1->Controls->Add(this->richTextBoxDisassemble);
			// 
			// splitContainerDisassembleMain.Panel2
			// 
			this->splitContainerDisassembleMain->Panel2->Controls->Add(this->vScrollBarDisassemble);
			this->splitContainerDisassembleMain->Size = System::Drawing::Size(851, 617);
			this->splitContainerDisassembleMain->SplitterDistance = 818;
			this->splitContainerDisassembleMain->TabIndex = 1;
			// 
			// vScrollBarDisassemble
			// 
			this->vScrollBarDisassemble->Dock = System::Windows::Forms::DockStyle::Fill;
			this->vScrollBarDisassemble->Location = System::Drawing::Point(0, 0);
			this->vScrollBarDisassemble->Name = L"vScrollBarDisassemble";
			this->vScrollBarDisassemble->Size = System::Drawing::Size(29, 617);
			this->vScrollBarDisassemble->TabIndex = 0;
			// 
			// DisassembledTextControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainerDisassembleMain);
			this->Name = L"DisassembledTextControl";
			this->Size = System::Drawing::Size(851, 617);
			this->splitContainerDisassembleMain->Panel1->ResumeLayout(false);
			this->splitContainerDisassembleMain->Panel2->ResumeLayout(false);
			this->splitContainerDisassembleMain->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
