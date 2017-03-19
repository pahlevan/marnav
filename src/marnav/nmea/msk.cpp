#include "msk.hpp"
#include <marnav/nmea/io.hpp>

namespace marnav
{
namespace nmea
{
constexpr const char * msk::TAG;

msk::msk()
	: sentence(ID, TAG, talker_id::global_positioning_system)
{
}

msk::msk(talker talk, fields::const_iterator first, fields::const_iterator last)
	: sentence(ID, TAG, talk)
{
	if (std::distance(first, last) != 5)
		throw std::invalid_argument{"invalid number of fields in msk"};

	read(*(first + 0), frequency);
	read(*(first + 1), frequency_mode);
	read(*(first + 2), bitrate);
	read(*(first + 3), bitrate_mode);
	read(*(first + 4), frequency_mss_status);
}

void msk::set_frequency(uint32_t f, selection_mode mode) noexcept
{
	frequency = f;
	frequency_mode = mode;
}

void msk::set_bitrate(uint32_t rate, selection_mode mode) noexcept
{
	bitrate = rate;
	bitrate_mode = mode;
}

void msk::append_data_to(std::string & s) const
{
	append(s, format(frequency, 3));
	append(s, to_string(frequency_mode));
	append(s, format(bitrate, 3));
	append(s, to_string(bitrate_mode));
	append(s, format(frequency_mss_status, 3));
}
}
}
