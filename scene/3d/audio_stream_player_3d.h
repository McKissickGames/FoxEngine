/*************************************************************************/
/*  audio_stream_player_3d.h                                             */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Fox ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Fox Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef AUDIO_STREAM_PLAYER_3D_H
#define AUDIO_STREAM_PLAYER_3D_H

#include "core/templates/safe_refcount.h"
#include "scene/3d/node_3d.h"
#include "scene/3d/velocity_tracker_3d.h"
#include "servers/audio/audio_filter_sw.h"
#include "servers/audio/audio_stream.h"
#include "servers/audio_server.h"

class Camera3D;
class AudioStreamPlayer3D : public Node3D {
	GDCLASS(AudioStreamPlayer3D, Node3D);

public:
	enum AttenuationModel {
		ATTENUATION_INVERSE_DISTANCE,
		ATTENUATION_INVERSE_SQUARE_DISTANCE,
		ATTENUATION_LOGARITHMIC,
		ATTENUATION_DISABLED,
	};

	enum OutOfRangeMode {
		OUT_OF_RANGE_MIX,
		OUT_OF_RANGE_PAUSE,
	};

	enum DopplerTracking {
		DOPPLER_TRACKING_DISABLED,
		DOPPLER_TRACKING_IDLE_STEP,
		DOPPLER_TRACKING_PHYSICS_STEP
	};

private:
	enum {
		MAX_OUTPUTS = 8,
		MAX_INTERSECT_AREAS = 32

	};

	struct Output {
		AudioFilterSW filter;
		AudioFilterSW::Processor filter_process[8];
		AudioFrame vol[4];
		float filter_gain = 0.0;
		float pitch_scale = 0.0;
		int bus_index = -1;
		int reverb_bus_index = -1;
		AudioFrame reverb_vol[4];
		Viewport *viewport = nullptr; //pointer only used for reference to previous mix
	};

	Output outputs[MAX_OUTPUTS];
	SafeNumeric<int> output_count;
	SafeFlag output_ready;

	//these are used by audio thread to have a reference of previous volumes (for ramping volume and avoiding clicks)
	Output prev_outputs[MAX_OUTPUTS];
	int prev_output_count = 0;

	Ref<AudioStreamPlayback> stream_playback;
	Ref<AudioStream> stream;
	Vector<AudioFrame> mix_buffer;

	SafeNumeric<float> setseek{ -1.0 };
	SafeFlag active;
	SafeNumeric<float> setplay{ -1.0 };

	AttenuationModel attenuation_model = ATTENUATION_INVERSE_DISTANCE;
	float unit_db = 0.0;
	float unit_size = 10.0;
	float max_db = 3.0;
	float pitch_scale = 1.0;
	bool autoplay = false;
	bool stream_paused = false;
	bool stream_paused_fade_in = false;
	bool stream_paused_fade_out = false;
	StringName bus;

	static void _calc_output_vol(const Vector3 &source_dir, real_t tightness, Output &output);
	void _mix_audio();
	static void _mix_audios(void *self) { reinterpret_cast<AudioStreamPlayer3D *>(self)->_mix_audio(); }

	void _set_playing(bool p_enable);
	bool _is_active() const;

	void _bus_layout_changed();

	uint32_t area_mask = 1;

	bool emission_angle_enabled = false;
	float emission_angle = 45.0;
	float emission_angle_filter_attenuation_db = -12.0;
	float attenuation_filter_cutoff_hz = 5000.0;
	float attenuation_filter_db = -24.0;

	float max_distance = 0.0;

	Ref<VelocityTracker3D> velocity_tracker;

	DopplerTracking doppler_tracking = DOPPLER_TRACKING_DISABLED;

	OutOfRangeMode out_of_range_mode = OUT_OF_RANGE_MIX;

	float _get_attenuation_db(float p_distance) const;

protected:
	void _validate_property(PropertyInfo &property) const override;
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_stream(Ref<AudioStream> p_stream);
	Ref<AudioStream> get_stream() const;

	void set_unit_db(float p_volume);
	float get_unit_db() const;

	void set_unit_size(float p_volume);
	float get_unit_size() const;

	void set_max_db(float p_boost);
	float get_max_db() const;

	void set_pitch_scale(float p_pitch_scale);
	float get_pitch_scale() const;

	void play(float p_from_pos = 0.0);
	void seek(float p_seconds);
	void stop();
	bool is_playing() const;
	float get_playback_position();

	void set_bus(const StringName &p_bus);
	StringName get_bus() const;

	void set_autoplay(bool p_enable);
	bool is_autoplay_enabled();

	void set_max_distance(float p_metres);
	float get_max_distance() const;

	void set_area_mask(uint32_t p_mask);
	uint32_t get_area_mask() const;

	void set_emission_angle_enabled(bool p_enable);
	bool is_emission_angle_enabled() const;

	void set_emission_angle(float p_angle);
	float get_emission_angle() const;

	void set_emission_angle_filter_attenuation_db(float p_angle_attenuation_db);
	float get_emission_angle_filter_attenuation_db() const;

	void set_attenuation_filter_cutoff_hz(float p_hz);
	float get_attenuation_filter_cutoff_hz() const;

	void set_attenuation_filter_db(float p_db);
	float get_attenuation_filter_db() const;

	void set_attenuation_model(AttenuationModel p_model);
	AttenuationModel get_attenuation_model() const;

	void set_out_of_range_mode(OutOfRangeMode p_mode);
	OutOfRangeMode get_out_of_range_mode() const;

	void set_doppler_tracking(DopplerTracking p_tracking);
	DopplerTracking get_doppler_tracking() const;

	void set_stream_paused(bool p_pause);
	bool get_stream_paused() const;

	Ref<AudioStreamPlayback> get_stream_playback();

	AudioStreamPlayer3D();
	~AudioStreamPlayer3D();
};

VARIANT_ENUM_CAST(AudioStreamPlayer3D::AttenuationModel)
VARIANT_ENUM_CAST(AudioStreamPlayer3D::OutOfRangeMode)
VARIANT_ENUM_CAST(AudioStreamPlayer3D::DopplerTracking)
#endif // AUDIO_STREAM_PLAYER_3D_H
