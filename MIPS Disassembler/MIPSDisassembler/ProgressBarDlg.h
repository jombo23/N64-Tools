#pragma once

//#include "IInstructionSet.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MIPSDisassembler {

	public delegate void ProgressRunDelegate();
	public delegate float GetProgressDelegate();
	public delegate void UpdateProgressDelegate(float value);

	/// <summary>
	/// Summary for ProgressBarDlg
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ProgressBarDlg : public System::Windows::Forms::Form
	{	
		void ProgressBarDlg::StartProgressRunThread()
		{
			try
			{
				this->delegateInstruction->Invoke();
			}
			catch (Exception^ e)
			{
				MessageBox::Show("Error " + e);
				error = true;
			}
			finally
			{
				completed = true;
			}
		}

		void ProgressBarDlg::UpdateProgressBar(float value)
		{
			progressBar->Value = (int)value;
			
			if (completed)
			{
				DialogResult = System::Windows::Forms::DialogResult::OK;
				Close();
			}
		}

	private:
			 ProgressRunDelegate^ delegateInstruction;
			 GetProgressDelegate^ getProgressDelegate;
			 UpdateProgressDelegate^ updateProgressDelegate;
			 System::Threading::Thread^ progressRunThread;
			 System::Timers::Timer^ timer;
			 bool completed;
			 bool error;
	public:
		ProgressBarDlg(ProgressRunDelegate^ delegateInstruction, GetProgressDelegate^ getProgressDelegate)
		{
			completed = false;
			error = false;
			updateProgressDelegate = gcnew UpdateProgressDelegate(this, &ProgressBarDlg::UpdateProgressBar);
			this->delegateInstruction = delegateInstruction;
			this->getProgressDelegate = getProgressDelegate;

			progressRunThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &ProgressBarDlg::StartProgressRunThread));
			progressRunThread->Start();

			timer = gcnew System::Timers::Timer(500);
			timer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &ProgressBarDlg::TimerElapsed);
			timer->Start();
		
			InitializeComponent();
		}

		void ProgressBarDlg::TimerElapsed(Object^ sender, System::Timers::ElapsedEventArgs^ e)
		{
			float percentage = getProgressDelegate();
			Invoke(gcnew UpdateProgressDelegate(this, &ProgressBarDlg::UpdateProgressBar), gcnew array<Object^>(1) { percentage });
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ProgressBarDlg()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ProgressBar^  progressBar;
	protected: 
	private: System::Windows::Forms::Label^  labelProgress;
	private: System::Windows::Forms::Button^  buttonCancel;
		

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
			this->progressBar = (gcnew System::Windows::Forms::ProgressBar());
			this->labelProgress = (gcnew System::Windows::Forms::Label());
			this->buttonCancel = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// progressBar
			// 
			this->progressBar->Location = System::Drawing::Point(12, 40);
			this->progressBar->Name = L"progressBar";
			this->progressBar->Size = System::Drawing::Size(323, 41);
			this->progressBar->Step = 1;
			this->progressBar->TabIndex = 0;
			// 
			// labelProgress
			// 
			this->labelProgress->AutoSize = true;
			this->labelProgress->Location = System::Drawing::Point(13, 10);
			this->labelProgress->Name = L"labelProgress";
			this->labelProgress->Size = System::Drawing::Size(65, 17);
			this->labelProgress->TabIndex = 1;
			this->labelProgress->Text = L"Progress";
			// 
			// buttonCancel
			// 
			this->buttonCancel->Location = System::Drawing::Point(79, 88);
			this->buttonCancel->Name = L"buttonCancel";
			this->buttonCancel->Size = System::Drawing::Size(169, 28);
			this->buttonCancel->TabIndex = 2;
			this->buttonCancel->Text = L"Cancel";
			this->buttonCancel->UseVisualStyleBackColor = true;
			this->buttonCancel->Click += gcnew System::EventHandler(this, &ProgressBarDlg::buttonCancel_Click);
			// 
			// ProgressBarDlg
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(347, 128);
			this->Controls->Add(this->buttonCancel);
			this->Controls->Add(this->labelProgress);
			this->Controls->Add(this->progressBar);
			this->Name = L"ProgressBarDlg";
			this->Text = L"Progress";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void buttonCancel_Click(System::Object^  sender, System::EventArgs^  e) {
				try
				{
					progressRunThread->Abort();
				}
				catch (Exception^ e)
				{

				}
				
				timer->Stop();
				timer = nullptr;

				 DialogResult = System::Windows::Forms::DialogResult::Cancel;
				 Close();
			 }
	};
}
