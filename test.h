#ifndef TEST_H
#define TEST_H

#include "kfsys_interface.h"

extern "C" {

// AAVE only commands
void set_hrtf (short hrtf);
void set_source_position(int id, float x, float y, float z);
void set_listener_position(float x, float y, float z);
void set_listener_orientation(float x, float y, float z);
void set_geometry(const char* obj);
void set_reflection_order(int n);
void init_reverb();
void set_reverb_rt60(unsigned short);
void set_reverb_area(unsigned short);
void set_reverb_volume(unsigned short);
void aave_update_engine();
void set_gain(float);

// AAVE + DIRECT common commands
void set_audio_engine(short ae);
void add_source(int id);
void set_source_sound(int id, const char* name);
void source_start_sound(int id);
void source_add_keyframe(int id, int start, int flags, float posx, float posy, float posz);
void source_clear_keyframes(int id);
void start_keyframes(int delay);
int render_frames_tofile(int nframes);
int render_frames_todriver(int nframes);

//utils
void source_convert_stereo_to_mono(int source_id);
void source_write_sound_file(int source_id);
}

#endif
