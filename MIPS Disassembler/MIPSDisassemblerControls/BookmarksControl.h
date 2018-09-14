#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MIPSDisassembler {

	/// <summary>
	/// Summary for BookmarksControl
	/// </summary>
	public ref class BookmarksControl : public System::Windows::Forms::UserControl
	{
	public:
		BookmarksControl(void)
		{
			InitializeComponent();
			targets = gcnew Dictionary<String^, ListViewItem^>();
		}

		public:
		Dictionary<String^, ListViewItem^>^ targets;
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~BookmarksControl()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::ListView^  listView;
	protected: 
	private: System::Windows::Forms::ColumnHeader^  columnHeaderAddress;
	public: 
	private: System::Windows::Forms::ColumnHeader^  columnHeaderCount;
	private: System::Windows::Forms::ColumnHeader^  columnHeaderName;

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
			this->listView = (gcnew System::Windows::Forms::ListView());
			this->columnHeaderAddress = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeaderCount = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeaderName = (gcnew System::Windows::Forms::ColumnHeader());
			this->SuspendLayout();
			// 
			// listView
			// 
			this->listView->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->listView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->columnHeaderAddress, 
				this->columnHeaderCount, this->columnHeaderName});
			this->listView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listView->FullRowSelect = true;
			this->listView->GridLines = true;
			this->listView->LabelWrap = false;
			this->listView->Location = System::Drawing::Point(0, 0);
			this->listView->MultiSelect = false;
			this->listView->Name = L"listView";
			this->listView->Size = System::Drawing::Size(258, 209);
			this->listView->Sorting = System::Windows::Forms::SortOrder::Ascending;
			this->listView->TabIndex = 1;
			this->listView->UseCompatibleStateImageBehavior = false;
			this->listView->View = System::Windows::Forms::View::Details;
			// 
			// columnHeaderAddress
			// 
			this->columnHeaderAddress->Text = L"Address";
			this->columnHeaderAddress->Width = 100;
			// 
			// columnHeaderCount
			// 
			this->columnHeaderCount->Text = L"Count";
			this->columnHeaderCount->Width = 100;
			// 
			// columnHeaderName
			// 
			this->columnHeaderName->Text = L"Name";
			this->columnHeaderName->Width = 200;
			// 
			// BookmarksControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->listView);
			this->Name = L"BookmarksControl";
			this->Size = System::Drawing::Size(258, 209);
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
