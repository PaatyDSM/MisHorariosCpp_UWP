#pragma once

#include "MainPage.g.h"
#include "MainPage.xaml.h"

using namespace Platform;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Phone::UI::Input;

namespace PaatyDSM
{
	public enum class NotifyType
	{
		StatusMessage,
		ErrorMessage
	};

	/// <summary>
	/// MainPage holds the Status Block and the Frame in which all the pages are loaded.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	protected:
		virtual void OnNavigatedTo(NavigationEventArgs^ e) override;

	private:
		void HardwareButtons_BackPressed(Object^ sender, Windows::Phone::UI::Input::BackPressedEventArgs^ e);

	internal:
		static MainPage^ Current;
		void NotifyUser(String^ strMessage, NotifyType type);
	};
}
