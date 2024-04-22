#pragma once

struct Parameters
{
	// defaults go here. these will be overridden if a valid "persistence.json" can be loaded
	int leftCameraIndex = 0;
	int rightCameraIndex = 1;

	friend std::ostream& operator<<(std::ostream& os, const Parameters& parameters);
};
#if FMT_VERSION >= 90000
template <> struct fmt::formatter<Parameters> : ostream_formatter {};
#endif