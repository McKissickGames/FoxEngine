/*************************************************************************/
/*  webrtc_data_channel_gdnative.h                                       */
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

#ifndef WEBRTC_DATA_CHANNEL_GDNATIVE_H
#define WEBRTC_DATA_CHANNEL_GDNATIVE_H

#ifdef WEBRTC_GDNATIVE_ENABLED

#include "modules/gdnative/include/net/Fox_net.h"
#include "webrtc_data_channel.h"

class WebRTCDataChannelGDNative : public WebRTCDataChannel {
	GDCLASS(WebRTCDataChannelGDNative, WebRTCDataChannel);

protected:
	static void _bind_methods();

private:
	const Fox_net_webrtc_data_channel *interface;

public:
	void set_native_webrtc_data_channel(const Fox_net_webrtc_data_channel *p_impl);

	virtual void set_write_mode(WriteMode mode) override;
	virtual WriteMode get_write_mode() const override;
	virtual bool was_string_packet() const override;

	virtual ChannelState get_ready_state() const override;
	virtual String get_label() const override;
	virtual bool is_ordered() const override;
	virtual int get_id() const override;
	virtual int get_max_packet_life_time() const override;
	virtual int get_max_retransmits() const override;
	virtual String get_protocol() const override;
	virtual bool is_negotiated() const override;

	virtual Error poll() override;
	virtual void close() override;

	/** Inherited from PacketPeer: **/
	virtual int get_available_packet_count() const override;
	virtual Error get_packet(const uint8_t **r_buffer, int &r_buffer_size) override; ///< buffer is GONE after next get_packet
	virtual Error put_packet(const uint8_t *p_buffer, int p_buffer_size) override;

	virtual int get_max_packet_size() const override;

	WebRTCDataChannelGDNative();
	~WebRTCDataChannelGDNative();
};

#endif // WEBRTC_GDNATIVE_ENABLED

#endif // WEBRTC_DATA_CHANNEL_GDNATIVE_H
