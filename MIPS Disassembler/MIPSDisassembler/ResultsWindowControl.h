#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MIPSDisassembler {

	/// <summary>
	/// Summary for ResultsWindowControl
	/// </summary>
	public ref class ResultsWindowControl : public System::Windows::Forms::UserControl
	{
	public:
		ResultsWindowControl(void)
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
		~ResultsWindowControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  tabControlResults;
	protected: 
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;

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
			this->tabControlResults = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->tabControlResults->SuspendLayout();
			this->SuspendLayout();
			// 
			// tabControlResults
			// 
			this->tabControlResults->Controls->Add(this->tabPage1);
			this->tabControlResults->Controls->Add(this->tabPage2);
			this->tabControlResults->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControlResults->Location = System::Drawing::Point(0, 0);
			this->tabControlResults->Name = L"tabControlResults";
			this->tabControlResults->SelectedIndex = 0;
			this->tabControlResults->Size = System::Drawing::Size(416, 318);
			this->tabControlResults->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->Location = System::Drawing::Point(4, 25);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(408, 289);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"tabPage1";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// tabPage2
			// 
			this->tabPage2->Location = System::Drawing::Point(4, 25);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(192, 71);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"tabPage2";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// ResultsWindowControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->tabControlResults);
			this->Name = L"ResultsWindowControl";
			this->Size = System::Drawing::Size(416, 318);
			this->tabControlResults->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
