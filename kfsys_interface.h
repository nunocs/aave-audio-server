#ifndef KFSYS_INTERFACE_H
#define KFSYS_INTERFACE_H

#include <map>

#include "aave_interface.h"
#include "kfsys_source.h"

#define SAMPLERATE 44100
#define BUFFLEN 2048
#define MAX_SOURCES 5

using namespace std;

enum class kfsys_cmds {
	listener = 0,
	source = 1,
	geometry = 2,
	reverb = 3,
	input_params = 4,
	output = 5
};

enum class output_cmds {
	iterate = 0,
	set_frame = 1,
	write_frames = 2
};

enum class input_params_cmds {
    mode = 0,
    hrtf = 1,
    reflections = 2,
    frame_rate = 3,
    audio_engine = 4
};

enum class reverb_cmds {
    add = 0,
    area = 1,
    volume = 2,
    rt60 = 3,
    reverb_active = 4,
    reverb_mix = 5
};

enum class geometry_cmds {
    add = 0,
    set_geometry = 1,
    set_posrot = 2,
    set_material = 3
};

class KFSystem {

	public:
	    static Libaave *libaave;
    	map<short, Source *> sources;
		short audio_engine; //0 = direct, 1 = aave
		int write_frames;
    	uint64_t global_position;
    	int64_t delay;
    	short render_state;
    	
    	KFSystem();
    	void render(short *buff, int bufflen);
    	void start_keyframes(int delay);
		void set_audio_engine(short ae);
    	int done();
    	
    	void handle_datagram(char *, int);
    	short handle_reverb_cmds(char *, int);
    	short handle_input_params_cmds(char *, int);
    	short handle_geometry_cmds(char *, int);
    	short cmds_listener_set_position(char *, int);
    	short cmds_output_iterate(char *, int);
    	short cmds_output_set_frame(char *, int);
		short cmds_output_write_frames(char *, int);
    	short add_source(char *, int);
    	short remove_source(char *, int);
    	short clear_sources(char *, int);
};

#endif
