#pragma once

#include "pch.h"
#include "App.xaml.h"
#include "MainPage.xaml.h"

using namespace PaatyDSM;

using namespace concurrency;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml;

/// <summary>
/// Initializes the singleton application object.
/// Esta es la primer línea de codigo ejecutado,
/// y como tal es el equivalente lógico de main() o WinMain().
/// </summary>
App::App()
{
	InitializeComponent();
	Application::Current->Suspending += ref new SuspendingEventHandler(this, &App::App_Suspending);
}

/// <summary>
/// Función que se utiliza cuando la aplicación es lanzada normalmente por el usuario.
/// </summary>
void App::OnLaunched(LaunchActivatedEventArgs^ e)
{
	// El siguiente comando admite la navegación a páginas nuevas y mantiene un historial de navegación para navegar hacia adelante y hacia atrás.
	auto rootFrame = dynamic_cast<Frame^> (Window::Current->Content);

	// No repetir la inicialización cuando la ventana ya tiene contenido, solo asegurar que la ventana está activa.
	if (rootFrame == nullptr)
	{
		// Crear un Frame que actua como contexto de navegación.
		rootFrame = ref new Frame();

		///Specific Fix (bug#6161022)
		// Caché de navegación.
		rootFrame->CacheSize = 0;

		auto prerequisite = task<void>([]() {});

		if (e->PreviousExecutionState == ApplicationExecutionState::Terminated)
		{
			// Restore the saved session state only when appropriate, scheduling the
			// final launch steps after the restore is complete
			prerequisite = SuspensionManager::RestoreAsync();
		}

		if (!e->PrelaunchActivated)
		{
			// TODO: This is not a prelaunch activation. Perform operations which
			// assume that the user explicitly launched the app such as updating
			// the online presence of the user on a social network, updating a
			// what's new feed, etc.
		}

		// Place the frame in the current Window
		Window::Current->Content = rootFrame;
	}

	if (rootFrame->Content == nullptr)
	{
		// When the navigation stack isn't restored navigate to the first page,
		// configuring the new page by passing required information as a navigation
		// parameter
		if (!rootFrame->Navigate(MainPage::typeid, e->Arguments))
		{
			throw ref new FailureException("Algo salió mal.\nError desconocido :(");
		}
	}

	// Ensure the current window is active
	Window::Current->Activate();
}

/// <summary>
/// Función que se utiliza cuando la ejecución de la aplicación  es suspendida.
/// El estado de la aplicación es guardado con el contenido en memoria intacto.
/// </summary>
void App::App_Suspending(Object^ sender, SuspendingEventArgs^ e)
{
	// This is the time to save app data in case the process is terminated
	(void)sender;	// Unused parameter
	(void)e;		// Unused parameter

	auto deferral = e->SuspendingOperation->GetDeferral();
	SuspensionManager::SaveAsync().then([=]()
	{
		deferral->Complete();
	});
}