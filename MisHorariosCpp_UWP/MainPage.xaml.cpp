#include "pch.h"

#include "MainPage.xaml.h"
#include "MisHorariosCpp_UWP_MAINAPP.xaml.h"

using namespace PaatyDSM;

using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::Foundation;

// Holds the Status Block and the Frame in which all the pages are loaded.
MainPage^ MainPage::Current = nullptr;

MainPage::MainPage()
{
	InitializeComponent();

	// This is a static public property that allows downstream pages to get a handle to the MainPage instance
	// in order to call methods that are in this class.
	MainPage::Current = this;

	// Called when Hardware Back Button is pressed
	HardwareButtons::BackPressed += ref new EventHandler<BackPressedEventArgs ^>(this, &MainPage::HardwareButtons_BackPressed);
}

// OnNavigatedTo function
void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	SuspensionManager::RegisterFrame(Page_Frame, "PaatyDSM.MisHorariosCpp_UWP_MAINAPP");

	///Specific Fix (bug#6161012), (bug#6161013) & AfterSuspensionRandomCrash.
	if (Page_Frame->Content == nullptr)
	{
		// When the navigation stack isn't restored navigate to the MisHorariosCpp_UWP_MAINAPP
		if (!Page_Frame->Navigate(TypeName{ "PaatyDSM.MisHorariosCpp_UWP_MAINAPP", TypeKind::Custom }))
		{
			throw ref new FailureException("Fallo al iniciar la App :(\n.\nCodigo de error: \n#31326497.\nContacte al editor de la aplicación\ne incluya el #codigo de error.");
		}
	}
	else
	{
		// null
	}
}

// StatusBlock function
void MainPage::NotifyUser(String^ strMessage, NotifyType type)
{
	switch (type)
	{
	case NotifyType::StatusMessage:
		StatusBorder->Background = ref new SolidColorBrush(Windows::UI::Colors::Green);
		break;
	case NotifyType::ErrorMessage:
		StatusBorder->Background = ref new SolidColorBrush(Windows::UI::Colors::Red);
		break;
	default:
		break;
	}
	StatusBlock->Text = strMessage;

	// Collapsed the StatusBlock if it has no text to conserve real estate.
	if (StatusBlock->Text != "")
	{
		StatusBorder->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}
	else
	{
		StatusBorder->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}
}

// On Hardware Back Button press
void MainPage::HardwareButtons_BackPressed(Object^ sender, Windows::Phone::UI::Input::BackPressedEventArgs^ e)
{
	if (this->Page_Frame->CanGoBack)
	{
		e->Handled = true;

		// Clear the status block when navigating
			NotifyUser("", NotifyType::StatusMessage);

		///Specific Fix (bug#6161022)
		// Clear the navigation stacks using the Clear method of each stack.
			Page_Frame->BackStack->Clear();
	}
	else
	{
		e->Handled = false;
	}

	///Specific Fix (bug#6161022)
	// Back to MainPage
		Page_Frame->Navigate(TypeName{ "PaatyDSM.MisHorariosCpp_UWP_MAINAPP", TypeKind::Custom });

	// Clear the navigation stacks using the Clear method of each stack.
		Page_Frame->BackStack->Clear();
}
