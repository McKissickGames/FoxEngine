/*************************************************************************/
/*  Fox_videodecoder.h                                                 */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           Fox ENGINE                                */
/*                      https://Foxengine.org                          */
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

#ifndef Fox_NATIVEVIDEODECODER_H
#define Fox_NATIVEVIDEODECODER_H

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FoxAV_API_MAJOR 0
#define FoxAV_API_MINOR 1

typedef struct
{
	Fox_gdnative_api_version version;
	void *next;
	void *(*constructor)(Fox_object *);
	void (*destructor)(void *);
	const char *(*get_plugin_name)();
	const char **(*get_supported_extensions)(int *count);
	Fox_bool (*open_file)(void *, void *); // data struct, and a FileAccess pointer
	Fox_float (*get_length)(const void *);
	Fox_float (*get_playback_position)(const void *);
	void (*seek)(void *, Fox_float);
	void (*set_audio_track)(void *, Fox_int);
	void (*update)(void *, Fox_float);
	Fox_packed_byte_array *(*get_videoframe)(void *);
	Fox_int (*get_audioframe)(void *, float *, int);
	Fox_int (*get_channels)(const void *);
	Fox_int (*get_mix_rate)(const void *);
	Fox_vector2 (*get_texture_size)(const void *);
} Fox_videodecoder_interface_gdnative;

typedef int (*GDNativeAudioMixCallback)(void *, const float *, int);

// FileAccess wrappers for custom FFmpeg IO
Fox_int GDAPI Fox_videodecoder_file_read(void *file_ptr, uint8_t *buf, int buf_size);
int64_t GDAPI Fox_videodecoder_file_seek(void *file_ptr, int64_t pos, int whence);
void GDAPI Fox_videodecoder_register_decoder(const Fox_videodecoder_interface_gdnative *p_interface);

#ifdef __cplusplus
}
#endif

#endif /* Fox_NATIVEVIDEODECODER_H */
