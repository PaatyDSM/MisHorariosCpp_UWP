#include "pch.h"

#include "MisHorariosCpp_UWP_MAINAPP.xaml.h"
#include "HorariosPage.xaml.h"
#include "ReleaseNotes.xaml.h"

#include "fstream"

using namespace PaatyDSM;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::Phone::UI::Input;

using namespace std;

MisHorariosCpp_UWP_MAINAPP::MisHorariosCpp_UWP_MAINAPP()
{
	InitializeComponent();
}

// OnNavigatedTo function
void MisHorariosCpp_UWP_MAINAPP::OnNavigatedTo(NavigationEventArgs^ e)
{
	// A pointer back to the main page.  This is needed if you want to call methods in MainPage such as NotifyUser()
	rootPage = MainPage::Current;

	// Read last used legajo
	read_legajo();
}

// On click 'Consultar Horarios' validate Legajo and send it to the next page 'HorariosPage'
void MisHorariosCpp_UWP_MAINAPP::send_legajo_button(Object^ sender, RoutedEventArgs^ e)
{
	// Clear errors
		rootPage->NotifyUser("", NotifyType::StatusMessage);

	///MyFunction
	// Convert String^ to string
		String^ str = main_legajo_input->Text;
		wstring w_str(str->Data());
		wstring wide(w_str);
		string str2(wide.begin(), wide.end());

	// Check if 'legajo' is valid
		size_t check = str2.find_first_not_of("0123456789");

	// Error handler and error messages
		// If no input
		if (str2.length() == 0)
		{
			rootPage->NotifyUser("Primero debe ingresar un legajo.", NotifyType::ErrorMessage);
		}
		// If illegal character
		else if (check != string::npos)
		{
			rootPage->NotifyUser("El legajo ingresado no es válido", NotifyType::ErrorMessage);
		}
		// If valid
		else
		{
			// Goto NEXT FUNCTION
			NavigateToHorariosPage();
		}

}

// On click 'Hyperlinks'
void MisHorariosCpp_UWP_MAINAPP::Footer_Click(Object^ sender, RoutedEventArgs^ e)
{
	auto uri = ref new Uri((String^)((HyperlinkButton^)sender)->Tag);
	Windows::System::Launcher::LaunchUriAsync(uri);
}

// Go to HorariosPage and send it legajo
void PaatyDSM::MisHorariosCpp_UWP_MAINAPP::NavigateToHorariosPage()
{
	this->Frame->Navigate(TypeName(Horarios::typeid), main_legajo_input->Text);
}

// On click 'Version number' navigate to 'Release Notes Page'
void MisHorariosCpp_UWP_MAINAPP::release_notes_button(Object^ sender, RoutedEventArgs^ e)
{
	this->Frame->Navigate(TypeName(ReleaseNotes::typeid));
}

// Read last used legajo
void MisHorariosCpp_UWP_MAINAPP::read_legajo(void)
{
	// Path for local saving
		String^ localfolder = ApplicationData::Current->LocalFolder->Path;

	// Convert String^ to string
		wstring folderNameW(localfolder->Begin());
		string folderName(folderNameW.begin(), folderNameW.end());

	// Filename
		string filename = folderName + "\\lastlegajo.tmp";

	// Read file
		char fileData[256]="";
		ifstream in(filename);
		if (in)
		{
			in >> fileData;
			in.close();
		}

	// Convert char to String^
		string s_filedata = string(fileData);
		wstring w_filedata = wstring(s_filedata.begin(), s_filedata.end());
		String^ str_fileData = ref new String(w_filedata.c_str());

	// Put legajo into TextBox
		main_legajo_input->Text = str_fileData;
}
