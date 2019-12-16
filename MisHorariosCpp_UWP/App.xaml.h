#pragma once

#include "App.g.h"

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;

namespace PaatyDSM
{
	/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>
	ref class App sealed
	{
	public:
		virtual void OnLaunched(LaunchActivatedEventArgs^ e) override;
		App();

	private:
		void App_Suspending(Object^ sender, SuspendingEventArgs^ e);
	};
}


