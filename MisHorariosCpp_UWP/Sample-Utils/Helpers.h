#pragma once

#include "..\MisHorariosCpp_UWP\pch.h"

using namespace concurrency;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::Web::Http;

namespace MisHorariosCpp_UWP
{
	class Helpers
	{
	
	public:
		static task<HttpResponseMessage^> DisplayTextResultAsync(
			HttpResponseMessage^ response,
			TextBox^ output);
	};
}
