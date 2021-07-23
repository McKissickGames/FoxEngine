/*************************************************************************/
/*  Fox_webrtc.h                                                       */
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

#ifndef Fox_NATIVEWEBRTC_H
#define Fox_NATIVEWEBRTC_H

#include <gdnative/gdnative.h>

#ifdef __cplusplus
extern "C" {
#endif

#define Fox_NET_WEBRTC_API_MAJOR 3
#define Fox_NET_WEBRTC_API_MINOR 2

/* Library Interface (used to set default GDNative WebRTC implementation */
typedef struct {
	Fox_gdnative_api_version version; /* version of our API */

	/* Called when the library is unset as default interface via Fox_net_set_webrtc_library */
	void (*unregistered)();

	/* Used by WebRTCPeerConnection create when GDNative is the default implementation. */
	/* Takes a pointer to WebRTCPeerConnectionGDNative, should bind and return OK, failure if binding was unsuccessful. */
	Fox_error (*create_peer_connection)(Fox_object *);

	void *next; /* For extension */
} Fox_net_webrtc_library;

/* WebRTCPeerConnection interface */
typedef struct {
	Fox_gdnative_api_version version; /* version of our API */

	Fox_object *data; /* User reference */

	/* This is WebRTCPeerConnection */
	Fox_int (*get_connection_state)(const void *);

	Fox_error (*initialize)(void *, const Fox_dictionary *);
	Fox_object *(*create_data_channel)(void *, const char *p_channel_name, const Fox_dictionary *);
	Fox_error (*create_offer)(void *);
	Fox_error (*create_answer)(void *); /* unused for now, should be done automatically on set_local_description */
	Fox_error (*set_remote_description)(void *, const char *, const char *);
	Fox_error (*set_local_description)(void *, const char *, const char *);
	Fox_error (*add_ice_candidate)(void *, const char *, int, const char *);
	Fox_error (*poll)(void *);
	void (*close)(void *);

	void *next; /* For extension? */
} Fox_net_webrtc_peer_connection;

/* WebRTCDataChannel interface */
typedef struct {
	Fox_gdnative_api_version version; /* version of our API */

	Fox_object *data; /* User reference */

	/* This is PacketPeer */
	Fox_error (*get_packet)(void *, const uint8_t **, int *);
	Fox_error (*put_packet)(void *, const uint8_t *, int);
	Fox_int (*get_available_packet_count)(const void *);
	Fox_int (*get_max_packet_size)(const void *);

	/* This is WebRTCDataChannel */
	void (*set_write_mode)(void *, Fox_int);
	Fox_int (*get_write_mode)(const void *);
	bool (*was_string_packet)(const void *);

	Fox_int (*get_ready_state)(const void *);
	const char *(*get_label)(const void *);
	bool (*is_ordered)(const void *);
	int (*get_id)(const void *);
	int (*get_max_packet_life_time)(const void *);
	int (*get_max_retransmits)(const void *);
	const char *(*get_protocol)(const void *);
	bool (*is_negotiated)(const void *);

	Fox_error (*poll)(void *);
	void (*close)(void *);

	void *next; /* For extension? */
} Fox_net_webrtc_data_channel;

/* Set the default GDNative library */
Fox_error GDAPI Fox_net_set_webrtc_library(const Fox_net_webrtc_library *);
/* Binds a WebRTCPeerConnectionGDNative to the provided interface */
void GDAPI Fox_net_bind_webrtc_peer_connection(Fox_object *p_obj, const Fox_net_webrtc_peer_connection *);
/* Binds a WebRTCDataChannelGDNative to the provided interface */
void GDAPI Fox_net_bind_webrtc_data_channel(Fox_object *p_obj, const Fox_net_webrtc_data_channel *);

#ifdef __cplusplus
}
#endif

#endif
