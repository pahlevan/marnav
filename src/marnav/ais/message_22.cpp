#include <marnav/ais/message_22.hpp>
#include <marnav/ais/angle.hpp>
#include <algorithm>
#include <cmath>

namespace marnav
{
namespace ais
{
constexpr message_id message_22::ID;
constexpr std::size_t message_22::SIZE_BITS;

message_22::message_22()
	: message(ID)
{
}

message_22::message_22(const raw & bits)
	: message(ID)
{
	if (bits.size() != SIZE_BITS)
		throw std::invalid_argument{"invalid number of bits in message_22"};
	read_data(bits);
}

void message_22::read_data(const raw & bits)
{
	get(bits, repeat_indicator);
	get(bits, mmsi);
	get(bits, channel_a);
	get(bits, channel_b);
	get(bits, txrx_mode);
	get(bits, power);
	get(bits, addressed);
	get(bits, band_a);
	get(bits, band_b);
	get(bits, zone_size);

	if (addressed) {
		get(bits, mmsi_1);
		get(bits, mmsi_2);
	} else {
		get(bits, ne_lon);
		get(bits, ne_lat);
		get(bits, sw_lon);
		get(bits, sw_lat);
	}
}

raw message_22::get_data() const
{
	raw bits(SIZE_BITS);

	bits.set(type(), 0, 6);
	set(bits, repeat_indicator);
	set(bits, mmsi);
	set(bits, channel_a);
	set(bits, channel_b);
	set(bits, txrx_mode);
	set(bits, power);
	set(bits, addressed);
	set(bits, band_a);
	set(bits, band_b);
	set(bits, zone_size);

	if (addressed) {
		set(bits, mmsi_1);
		set(bits, mmsi_2);
	} else {
		set(bits, ne_lon);
		set(bits, ne_lat);
		set(bits, sw_lon);
		set(bits, sw_lat);
	}

	return bits;
}

std::optional<geo::position> message_22::get_position_ne() const
{
	if ((ne_lat == latitude_not_available_short) || (ne_lon == longitude_not_available_short))
		return std::make_optional<geo::position>();

	return std::make_optional<geo::position>(
		to_geo_latitude(ne_lat, ne_lat.count, angle_scale::I1),
		to_geo_longitude(ne_lon, ne_lon.count, angle_scale::I1));
}

std::optional<geo::position> message_22::get_position_sw() const
{
	if ((sw_lat == latitude_not_available_short) || (sw_lon == longitude_not_available_short))
		return std::make_optional<geo::position>();

	return std::make_optional<geo::position>(
		to_geo_latitude(sw_lat, sw_lat.count, angle_scale::I1),
		to_geo_longitude(sw_lon, sw_lon.count, angle_scale::I1));
}

void message_22::set_position_ne(const geo::position & t) noexcept
{
	ne_lat = to_latitude_minutes(t.lat(), ne_lat.count, angle_scale::I1);
	ne_lon = to_longitude_minutes(t.lon(), ne_lon.count, angle_scale::I1);
}

void message_22::set_position_sw(const geo::position & t) noexcept
{
	sw_lat = to_latitude_minutes(t.lat(), sw_lat.count, angle_scale::I1);
	sw_lon = to_longitude_minutes(t.lon(), sw_lon.count, angle_scale::I1);
}
}
}
