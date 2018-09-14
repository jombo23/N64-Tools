#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;




namespace MIPSDisassembler {

	
	public delegate void VScrollWndProcDelegate(Message% m);

	/// <summary>
	/// Summary for MIPSDisassemblerControlsControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class MIPSDisassemblerControlsControl : public System::Windows::Forms::RichTextBox
	{
	public:
		VScrollWndProcDelegate^ vScrollWndProcDelegate;
		event VScrollWndProcDelegate^ OnVScrollWndProc
		{
			void add( VScrollWndProcDelegate^ handler )
			{
				vScrollWndProcDelegate += handler;
			}
			void remove( VScrollWndProcDelegate^ handler )
			{
				vScrollWndProcDelegate -= handler;
			}
			void raise(Message% m)
			{
				vScrollWndProcDelegate(m);
			}
		}


	public:
		MIPSDisassemblerControlsControl(void) : RichTextBox()
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		static const int WM_VSCROLL = 277;

	  virtual void WndProc( Message% m ) override
	  {

		 // Listen for operating system messages.
		 switch ( m.Msg )
		 {
			case WM_VSCROLL:

				if (vScrollWndProcDelegate != nullptr)
					vScrollWndProcDelegate->Invoke(m);
			   
			   break;
		 }
		 RichTextBox::WndProc( m );
	  }



	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MIPSDisassemblerControlsControl()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			
		}
#pragma endregion
	};
}
