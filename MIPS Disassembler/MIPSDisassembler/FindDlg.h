#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MIPSDisassembler {

	/// <summary>
	/// Summary for FindDlg
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FindDlg : public System::Windows::Forms::Form
	{
	public:
		FindDlg(void)
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
		~FindDlg()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::Label^  labelFindWhat;
	public: System::Windows::Forms::TextBox^  textBoxFind;
	public: System::Windows::Forms::Button^  buttonFindNext;
	public: System::Windows::Forms::Button^  buttonCancel;
	public: System::Windows::Forms::CheckBox^  checkBoxMatchWholeWordsOnly;
	public: System::Windows::Forms::CheckBox^  checkBoxMatchCase;
	public: System::Windows::Forms::GroupBox^  groupBoxDirection;
	public: System::Windows::Forms::RadioButton^  radioButtonDown;
	public: System::Windows::Forms::RadioButton^  radioButtonUp;
	public: System::Windows::Forms::CheckBox^  checkBoxFindAll;

	protected: 


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
			this->labelFindWhat = (gcnew System::Windows::Forms::Label());
			this->textBoxFind = (gcnew System::Windows::Forms::TextBox());
			this->buttonFindNext = (gcnew System::Windows::Forms::Button());
			this->buttonCancel = (gcnew System::Windows::Forms::Button());
			this->checkBoxMatchWholeWordsOnly = (gcnew System::Windows::Forms::CheckBox());
			this->checkBoxMatchCase = (gcnew System::Windows::Forms::CheckBox());
			this->groupBoxDirection = (gcnew System::Windows::Forms::GroupBox());
			this->radioButtonDown = (gcnew System::Windows::Forms::RadioButton());
			this->radioButtonUp = (gcnew System::Windows::Forms::RadioButton());
			this->checkBoxFindAll = (gcnew System::Windows::Forms::CheckBox());
			this->groupBoxDirection->SuspendLayout();
			this->SuspendLayout();
			// 
			// labelFindWhat
			// 
			this->labelFindWhat->AutoSize = true;
			this->labelFindWhat->Location = System::Drawing::Point(13, 13);
			this->labelFindWhat->Name = L"labelFindWhat";
			this->labelFindWhat->Size = System::Drawing::Size(72, 17);
			this->labelFindWhat->TabIndex = 0;
			this->labelFindWhat->Text = L"Find what:";
			// 
			// textBoxFind
			// 
			this->textBoxFind->Location = System::Drawing::Point(91, 10);
			this->textBoxFind->Name = L"textBoxFind";
			this->textBoxFind->Size = System::Drawing::Size(252, 22);
			this->textBoxFind->TabIndex = 1;
			// 
			// buttonFindNext
			// 
			this->buttonFindNext->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->buttonFindNext->Location = System::Drawing::Point(349, 10);
			this->buttonFindNext->Name = L"buttonFindNext";
			this->buttonFindNext->Size = System::Drawing::Size(100, 27);
			this->buttonFindNext->TabIndex = 2;
			this->buttonFindNext->Text = L"Find next";
			this->buttonFindNext->UseVisualStyleBackColor = true;
			// 
			// buttonCancel
			// 
			this->buttonCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->buttonCancel->Location = System::Drawing::Point(349, 39);
			this->buttonCancel->Name = L"buttonCancel";
			this->buttonCancel->Size = System::Drawing::Size(100, 27);
			this->buttonCancel->TabIndex = 3;
			this->buttonCancel->Text = L"Cancel";
			this->buttonCancel->UseVisualStyleBackColor = true;
			// 
			// checkBoxMatchWholeWordsOnly
			// 
			this->checkBoxMatchWholeWordsOnly->AutoSize = true;
			this->checkBoxMatchWholeWordsOnly->Location = System::Drawing::Point(17, 45);
			this->checkBoxMatchWholeWordsOnly->Name = L"checkBoxMatchWholeWordsOnly";
			this->checkBoxMatchWholeWordsOnly->Size = System::Drawing::Size(179, 21);
			this->checkBoxMatchWholeWordsOnly->TabIndex = 4;
			this->checkBoxMatchWholeWordsOnly->Text = L"Match whole words only";
			this->checkBoxMatchWholeWordsOnly->UseVisualStyleBackColor = true;
			// 
			// checkBoxMatchCase
			// 
			this->checkBoxMatchCase->AutoSize = true;
			this->checkBoxMatchCase->Location = System::Drawing::Point(17, 72);
			this->checkBoxMatchCase->Name = L"checkBoxMatchCase";
			this->checkBoxMatchCase->Size = System::Drawing::Size(102, 21);
			this->checkBoxMatchCase->TabIndex = 5;
			this->checkBoxMatchCase->Text = L"Match case";
			this->checkBoxMatchCase->UseVisualStyleBackColor = true;
			// 
			// groupBoxDirection
			// 
			this->groupBoxDirection->Controls->Add(this->radioButtonDown);
			this->groupBoxDirection->Controls->Add(this->radioButtonUp);
			this->groupBoxDirection->Location = System::Drawing::Point(202, 38);
			this->groupBoxDirection->Name = L"groupBoxDirection";
			this->groupBoxDirection->Size = System::Drawing::Size(137, 53);
			this->groupBoxDirection->TabIndex = 6;
			this->groupBoxDirection->TabStop = false;
			this->groupBoxDirection->Text = L"Direction";
			// 
			// radioButtonDown
			// 
			this->radioButtonDown->AutoSize = true;
			this->radioButtonDown->Checked = true;
			this->radioButtonDown->Location = System::Drawing::Point(59, 21);
			this->radioButtonDown->Name = L"radioButtonDown";
			this->radioButtonDown->Size = System::Drawing::Size(64, 21);
			this->radioButtonDown->TabIndex = 1;
			this->radioButtonDown->TabStop = true;
			this->radioButtonDown->Text = L"Down";
			this->radioButtonDown->UseVisualStyleBackColor = true;
			// 
			// radioButtonUp
			// 
			this->radioButtonUp->AutoSize = true;
			this->radioButtonUp->Location = System::Drawing::Point(6, 21);
			this->radioButtonUp->Name = L"radioButtonUp";
			this->radioButtonUp->Size = System::Drawing::Size(47, 21);
			this->radioButtonUp->TabIndex = 0;
			this->radioButtonUp->Text = L"Up";
			this->radioButtonUp->UseVisualStyleBackColor = true;
			// 
			// checkBoxFindAll
			// 
			this->checkBoxFindAll->AutoSize = true;
			this->checkBoxFindAll->Location = System::Drawing::Point(349, 72);
			this->checkBoxFindAll->Name = L"checkBoxFindAll";
			this->checkBoxFindAll->Size = System::Drawing::Size(76, 21);
			this->checkBoxFindAll->TabIndex = 7;
			this->checkBoxFindAll->Text = L"Find All";
			this->checkBoxFindAll->UseVisualStyleBackColor = true;
			// 
			// FindDlg
			// 
			this->AcceptButton = this->buttonFindNext;
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->buttonCancel;
			this->ClientSize = System::Drawing::Size(461, 104);
			this->Controls->Add(this->checkBoxFindAll);
			this->Controls->Add(this->groupBoxDirection);
			this->Controls->Add(this->checkBoxMatchCase);
			this->Controls->Add(this->checkBoxMatchWholeWordsOnly);
			this->Controls->Add(this->buttonCancel);
			this->Controls->Add(this->buttonFindNext);
			this->Controls->Add(this->textBoxFind);
			this->Controls->Add(this->labelFindWhat);
			this->Name = L"FindDlg";
			this->Text = L"Find";
			this->Load += gcnew System::EventHandler(this, &FindDlg::FindDlg_Load);
			this->groupBoxDirection->ResumeLayout(false);
			this->groupBoxDirection->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void FindDlg_Load(System::Object^  sender, System::EventArgs^  e) {
				 textBoxFind->Focus();
				 textBoxFind->SelectAll();
			 }
};
}
