#include <marnav/ais/message_01.hpp>
#include <marnav/ais/angle.hpp>
#include <cmath>

namespace marnav
{
namespace ais
{
constexpr message_id message_01::ID;
constexpr std::size_t message_01::SIZE_BITS;

message_01::message_01()
	: message_01(ID)
{
}

message_01::message_01(message_id id)
	: message(id)
{
}

message_01::message_01(const raw & bits)
	: message_01(ID)
{
	if (bits.size() != SIZE_BITS)
		throw std::invalid_argument{"invalid number of bits in ais/message_01"};
	read_data(bits);
}

std::optional<geo::longitude> message_01::get_lon() const
{
	if (longitude_minutes == longitude_not_available)
		return std::make_optional<geo::longitude>();
	return to_geo_longitude(longitude_minutes, longitude_minutes.count, angle_scale::I4);
}

std::optional<geo::latitude> message_01::get_lat() const
{
	if (latitude_minutes == latitude_not_available)
		return std::make_optional<geo::latitude>();
	return to_geo_latitude(latitude_minutes, latitude_minutes.count, angle_scale::I4);
}

void message_01::set_lon_unavailable()
{
	longitude_minutes = longitude_not_available;
}

void message_01::set_lat_unavailable()
{
	latitude_minutes = latitude_not_available;
}

void message_01::set_lon(const geo::longitude & t)
{
	longitude_minutes = to_longitude_minutes(t, longitude_minutes.count, angle_scale::I4);
}

void message_01::set_lat(const geo::latitude & t)
{
	latitude_minutes = to_latitude_minutes(t, latitude_minutes.count, angle_scale::I4);
}

std::optional<units::knots> message_01::get_sog() const noexcept
{
	// ignores special value of 1022 = 102.2 knots or faster

	if (sog == sog_not_available)
		return {};
	return units::knots{0.1 * sog};
}

void message_01::set_sog_unavailable()
{
	sog = sog_not_available;
}

void message_01::set_sog(units::velocity t)
{
	if (t.value() < 0.0)
		throw std::invalid_argument{"SOG less than zero"};

	const auto v = t.get<units::knots>();
	sog = std::min(sog_max, static_cast<uint32_t>(round(v * 10).value()));
}

/// Returns course over ground in degrees true north.
std::optional<double> message_01::get_cog() const noexcept
{
	if (cog == cog_not_available)
		return {};
	return 0.1 * cog;
}

void message_01::set_cog(std::optional<double> t) noexcept
{
	cog = !t ? cog_not_available : static_cast<uint32_t>(std::round(*t / 0.1));
}

/// Returns heading in degrees.
std::optional<uint32_t> message_01::get_hdg() const noexcept
{
	if (hdg == hdg_not_available)
		return {};
	return {hdg};
}

void message_01::set_hdg(std::optional<uint32_t> t) noexcept
{
	hdg = !t ? hdg_not_available : *t;
}

rate_of_turn message_01::get_rot() const noexcept
{
	return rate_of_turn(rot);
}

void message_01::set_rot(rate_of_turn t) noexcept
{
	rot = t.raw();
}

void message_01::read_data(const raw & bits)
{
	get(bits, repeat_indicator);
	get(bits, mmsi);
	get(bits, nav_status);
	get(bits, rot);
	get(bits, sog);
	get(bits, position_accuracy);
	get(bits, longitude_minutes);
	get(bits, latitude_minutes);
	get(bits, cog);
	get(bits, hdg);
	get(bits, timestamp);
	get(bits, maneuver_indicator);
	get(bits, raim);
	get(bits, radio_status);
}

raw message_01::get_data() const
{
	raw bits(SIZE_BITS);

	bits.set(type(), 0, 6);
	set(bits, repeat_indicator);
	set(bits, mmsi);
	set(bits, nav_status);
	set(bits, rot);
	set(bits, sog);
	set(bits, position_accuracy);
	set(bits, longitude_minutes);
	set(bits, latitude_minutes);
	set(bits, cog);
	set(bits, hdg);
	set(bits, timestamp);
	set(bits, maneuver_indicator);
	set(bits, raim);
	set(bits, radio_status);

	return bits;
}
}
}
