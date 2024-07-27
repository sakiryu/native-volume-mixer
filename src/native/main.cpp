#include "volume_mixer.h"
#include "volume_mixer.h"

int main(int argc, char** argv)
{
	std::unique_ptr<IVolumeMixer> volume{ new VolumeMixer() };
	volume->set_volume(0.3f); //30%
	volume->set_volume(0.6f); //60%
	volume->set_volume(0.9f); //90%
}