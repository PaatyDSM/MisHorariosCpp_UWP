#include "pch.h"
#include "School.h"

using namespace PaatyDSM;

using namespace Platform;
using namespace Windows::Data::Json;

String^ School::fechaKey = "fecha";
String^ School::horaEntradaKey = "horaEntrada";
String^ School::horaSalidaKey = "horaSalida";
String^ School::tiendaKey = "tienda";
String^ School::asignacionesKey = "asignaciones";

School::School(void) : fecha(""), horaEntrada(""), horaSalida(""), tienda("")
{
}

School::School(JsonObject^ jsonObject)
{
	JsonObject^ schoolObject = jsonObject->GetNamedObject(asignacionesKey, nullptr);
	fecha = jsonObject->GetNamedString(fechaKey);
	HoraEntrada = jsonObject->GetNamedString(horaEntradaKey);
	HoraSalida = jsonObject->GetNamedString(horaSalidaKey);
	Tienda = jsonObject->GetNamedString(tiendaKey);
}


String^ School::Fecha::get()
{
	return fecha;
}

void School::Fecha::set(String^ value)
{
	fecha = value;
}

String^ School::HoraEntrada::get()
{
	return horaEntrada;
}

void School::HoraEntrada::set(String^ value)
{
	horaEntrada = value;
}

String^ School::HoraSalida::get()
{
	return horaSalida;
}

void School::HoraSalida::set(String^ value)
{
	horaSalida = value;
}

String^ School::Tienda::get()
{
	return tienda;
}

void School::Tienda::set(String^ value)
{
	tienda = value;
}
