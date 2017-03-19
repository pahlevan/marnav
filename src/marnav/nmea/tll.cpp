#include "tll.hpp"
#include <marnav/nmea/angle.hpp>
#include <marnav/nmea/io.hpp>
#include <marnav/nmea/convert.hpp>

namespace marnav
{
namespace nmea
{
constexpr const char * tll::TAG;

tll::tll()
	: sentence(ID, TAG, talker_id::global_positioning_system)
{
}

tll::tll(talker talk, fields::const_iterator first, fields::const_iterator last)
	: sentence(ID, TAG, talk)
{
	if (std::distance(first, last) != 9)
		throw std::invalid_argument{"invalid number of fields in tll"};

	read(*(first + 0), number);
	read(*(first + 1), lat);
	read(*(first + 2), lat_hem);
	read(*(first + 3), lon);
	read(*(first + 4), lon_hem);
	read(*(first + 5), name);
	read(*(first + 6), time_utc);
	read(*(first + 7), status);
	read(*(first + 8), reference_target);

	// instead of reading data into temporary lat/lon, let's correct values afterwards
	lat = correct_hemisphere(lat, lat_hem);
	lon = correct_hemisphere(lon, lon_hem);
}

geo::longitude tll::get_longitude() const
{
	return lon;
}

geo::latitude tll::get_latitude() const
{
	return lat;
}

void tll::set_lat(const geo::latitude & t)
{
	lat = t;
	lat_hem = convert_hemisphere(t);
}

void tll::set_lon(const geo::longitude & t)
{
	lon = t;
	lon_hem = convert_hemisphere(t);
}

void tll::append_data_to(std::string & s) const
{
	append(s, format(number, 2));
	append(s, to_string(lat));
	append(s, to_string(lat_hem));
	append(s, to_string(lon));
	append(s, to_string(lon_hem));
	append(s, to_string(name));
	append(s, to_string(time_utc));
	append(s, to_string(status));
	append(s, to_string(reference_target));
}
}
}
