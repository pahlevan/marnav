#include "vpw.hpp"
#include <marnav/nmea/io.hpp>

namespace marnav
{
namespace nmea
{
constexpr const char * vpw::TAG;

vpw::vpw()
	: sentence(ID, TAG, talker_id::integrated_instrumentation)
{
}

vpw::vpw(talker talk, fields::const_iterator first, fields::const_iterator last)
	: sentence(ID, TAG, talk)
{
	if (std::distance(first, last) != 4)
		throw std::invalid_argument{"invalid number of fields in vpw"};

	read(*(first + 0), speed_knots);
	read(*(first + 1), speed_knots_unit);
	read(*(first + 2), speed_meters_per_second);
	read(*(first + 3), speed_meters_per_second_unit);
}

void vpw::set_speed_knots(double t) noexcept
{
	speed_knots = t;
	speed_knots_unit = unit::velocity::knot;
}

void vpw::set_speed_mps(double t) noexcept
{
	speed_meters_per_second = t;
	speed_meters_per_second_unit = unit::velocity::mps;
}

void vpw::append_data_to(std::string & s) const
{
	append(s, to_string(speed_knots));
	append(s, to_string(speed_knots_unit));
	append(s, to_string(speed_meters_per_second));
	append(s, to_string(speed_meters_per_second_unit));
}
}
}
