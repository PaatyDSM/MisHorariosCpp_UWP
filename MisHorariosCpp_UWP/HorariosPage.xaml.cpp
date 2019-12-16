#include "pch.h"

#include "HorariosPage.xaml.h"
#include "MainPage.xaml.h"
#include "MisHorariosCpp_UWP_MAINAPP.xaml.h"

#include "Sample-Utils\Helpers.h"

#include <fstream>
#include <iostream>

using namespace PaatyDSM;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::Web::Http;
using namespace Windows::Web::Http::Filters;

using namespace std;

/// Path for local saving
String^ localfolder = ApplicationData::Current->LocalFolder->Path;

PaatyDSM::Horarios::Horarios()
{
	InitializeComponent();

}

// OnNavigatedTo function
void Horarios::OnNavigatedTo(NavigationEventArgs^ e)
{
	// Save received parameter like a reference called str
		String^ str = (String^)e->Parameter;

	// A pointer back to the main page.  This is needed if you want to call methods in MainPage such as NotifyUser()
		rootPage = MainPage::Current;

	///Specific fix
		// Clean error messages 
		rootPage->NotifyUser("", NotifyType::StatusMessage);

	// Starting  filters and httpclient
		filter = ref new HttpBaseProtocolFilter();
		httpClient = ref new HttpClient(filter);
	
	///MyFunction3
		// Convert String^ to int
		wstring w_str(str->Data());
		wstring wide(w_str);
		string str3(wide.begin(), wide.end());
		int legajo = stoi(str3);

	// Error handler
		if (e != nullptr) // Safe to use str
		{ 
			// Goto NEXT FUNCTION
			send_pagewithlegajo(legajo);
		}
		else // Do not use data
		{ 
			// Show error
			rootPage->NotifyUser("Error interno: #31326496.\nContacte al editor de la aplicación\ne incluya el #codigo de error.", NotifyType::ErrorMessage);
			GoPageBack();
		}
}

// Send Legajo function
void Horarios::send_pagewithlegajo(int legajo)
{
	///Specific Fix
		rootPage->NotifyUser("Obteniendo horarios...", NotifyType::StatusMessage);

	//Alsea Proveedores
	///http:://proveedores.alsea.com.ar:25080/asignaciones-server/mobile/main/asignaciones/legajos/ + legajo

	///Get input legajo and append it to the url
		// Converts int to string
			string s_legajo = to_string(legajo);
		// Append legajo to the url
			string var = "http://proveedores.alsea.com.ar:25080/asignaciones-server/mobile/main/asignaciones/legajos/" + s_legajo;
		// Converts string to wstring
			wstring url_ok(var.begin(), var.end());
		// Reference to the new url page connection with legajo included
			String^ uriString = ref new String(url_ok.c_str());

	// CacheControl
		filter->CacheControl->ReadBehavior = HttpCacheReadBehavior::MostRecent;
		filter->CacheControl->WriteBehavior = HttpCacheWriteBehavior::NoCache;

	// Save last used legajo
		save_legajo(legajo);

	// Do an asynchronous GET. We need to use use_current() with the continuations since the tasks are completed on
	// background threads and we need to run on the UI thread to update the UI.
		BackgroundTask(uriString, legajo);
}

// Hyperlink buttons
void Horarios::Footer_Click(Object^ sender, RoutedEventArgs^ e)
{
	auto uri = ref new Uri((String^)((HyperlinkButton^)sender)->Tag);
	Windows::System::Launcher::LaunchUriAsync(uri);
}

// Go back to MainPage with uncleared errors
void Horarios::GoPageBack()
{
	///Specific Fix (#bug6161013)
	OutputField->Text = "{\"asignaciones\":[],\"fechaConsulta\":\"\",\"legajo\":\"\"}";
	MainPage::Current->DataContext = ref new User(OutputField->Text);

	// Go to page
	Frame->Navigate(TypeName(PaatyDSM::MisHorariosCpp_UWP_MAINAPP::typeid));
}

// Navigation: Back Button
void Horarios::Backbutton1(Object^ sender, RoutedEventArgs^ e)
{
	///Specific Fix (#bug6161013)
	OutputField->Text = "{\"asignaciones\":[],\"fechaConsulta\":\"\",\"legajo\":\"\"}";
	MainPage::Current->DataContext = ref new User(OutputField->Text);

	//Clears StatusBlock
	rootPage->NotifyUser("", NotifyType::StatusMessage);

	// Go to page
	Frame->Navigate(TypeName(PaatyDSM::MisHorariosCpp_UWP_MAINAPP::typeid));
}

// Save last used legajo
void Horarios::save_legajo(int legajo_var)
{
	// Convert String^ to string
		wstring folderNameW(localfolder->Begin());
		string folderName(folderNameW.begin(), folderNameW.end());

	// Filename
		string filename = folderName + "\\lastlegajo.tmp";

	// Write file
		string saved_legajo = to_string(legajo_var);
		ofstream out(filename);
		if (out)
		{
			out << saved_legajo;
			out.close();
		}
}

// Save cache to a file
void Horarios::save_cache(String^ cache)
{
	// Convert String^ to string
		wstring w_data(cache->Begin());
		string s_data(w_data.begin(), w_data.end());

	// Convert String^ to string
		wstring folderNameW(localfolder->Begin());
		string folderName(folderNameW.begin(), folderNameW.end());

	// Filename
		string filename2 = folderName + "\\lasthorarios.tmp";

	// Write file
		ofstream out(filename2);
		if (out)
		{
			out << s_data;
			out.close();
		}
}

// Run tasks
void Horarios::BackgroundTask(String^ e, int legajo)
{
	// Save received parameter like a reference called str
	String^ uriString = (String^)e;

	create_task(httpClient->GetAsync(ref new Uri(uriString))).then([=](HttpResponseMessage^ response)
	{
		return Helpers::DisplayTextResultAsync(response, OutputField);
	},
		task_continuation_context::use_current()).then
		(
			[=](task<HttpResponseMessage^> previousTask)
	{
		unsigned short int internet_conn_status = 0;
		try
		{
			// Check if any previous task threw an exception.
			HttpResponseMessage^ response = previousTask.get();
		}
		///Specific Fix: bug#6161008
		catch (Exception^ NoInternetConnection)
		{
			internet_conn_status = 1;

			rootPage->NotifyUser("Error. No hay conexión a internet.", NotifyType::ErrorMessage);
		}

		// If error connection don't occurs then initialize parse_Json
		if (internet_conn_status == 0)
		{
			// Error Handlers
			try
			{
				// Parse_json
				MainPage::Current->DataContext = ref new User(OutputField->Text);

				// Convert String^ to string
					wstring w_str(OutputField->Text->Data());
					wstring wide(w_str);
					string str3(wide.begin(), wide.end());

				// Find ':[{' string to check if the data contains a valid legajo info
				size_t found = str3.find(":[{");

				// If true
				if (found != std::string::npos)
				{
					// Show successfull!
					rootPage->NotifyUser("Horarios recibidos!", NotifyType::StatusMessage);

					// Save cache
					save_cache(OutputField->Text);
				}
				else
				{
					rootPage->NotifyUser("El legajo no existe!", NotifyType::ErrorMessage);
					GoPageBack();
				}
			}
			catch (Exception^ JSONError)
			{
				rootPage->NotifyUser("Error: La base de datos del servidor está dañada.\nPor favor contacte al proveedor", NotifyType::ErrorMessage);
			}
		}
		// Else try to read from cache
		else
		{
			/// Specific Fix (bug#6161010)
			//If no internet connection is available get check if the last legajo obtained is equal to the actual legajo and read it from the cache.
			//Show an error and adverts that the content is from cache.

			// Try to read from cache
			read_cache(legajo);
		}
	}
	);
}

// Read from cache
void Horarios::read_cache(int legajo)
{
	// Convert String^ to string
	wstring folderNameW(localfolder->Begin());
	string folderName(folderNameW.begin(), folderNameW.end());

	// Filename
	string filename2 = folderName + "\\lasthorarios.tmp";

	// Read file
	string fileData2;
	ifstream in(filename2);
	if (in)
	{
		getline(in, fileData2);
		in.close();
	}

	///MyFunction4
	// Check legajo first and if valid, parse JSon

	// Search for legajo item
		string s_legajo = to_string(legajo);
		string var = "\"legajo\":\"" + s_legajo + "\"}";
		size_t found = fileData2.find(var);

	// If true
	if (found != std::string::npos)
	{
		// Convert string to String^
			wstring w_filedata2 = wstring(fileData2.begin(), fileData2.end());
			String^ str_fileData2 = ref new String(w_filedata2.c_str());

		// Parse JSon
			MainPage::Current->DataContext = ref new User(str_fileData2);
	}
	// Else Go Back
	else
	{
		GoPageBack();
	}
}
