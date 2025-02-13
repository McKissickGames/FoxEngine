#ifndef DYLIBLOAD_WRAPPER_LIBUDEV
#define DYLIBLOAD_WRAPPER_LIBUDEV
// This file is generated. Do not edit!
// see https://github.com/hpvb/dynload-wrapper for details
// generated by /home/hp/Projects/Fox/pulse/generate-wrapper.py 0.3 on 2021-02-20 00:08:59
// flags: /home/hp/Projects/Fox/pulse/generate-wrapper.py --include /usr/include/libudev.h --sys-include <libudev.h> --soname libudev.so.1 --init-name libudev --omit-prefix gnu_ --output-header libudev-so_wrap.h --output-implementation libudev-so_wrap.c
//
#include <stdint.h>

#define udev_ref udev_ref_dylibloader_orig_libudev
#define udev_unref udev_unref_dylibloader_orig_libudev
#define udev_new udev_new_dylibloader_orig_libudev
#define udev_set_log_fn udev_set_log_fn_dylibloader_orig_libudev
#define udev_get_log_priority udev_get_log_priority_dylibloader_orig_libudev
#define udev_set_log_priority udev_set_log_priority_dylibloader_orig_libudev
#define udev_get_userdata udev_get_userdata_dylibloader_orig_libudev
#define udev_set_userdata udev_set_userdata_dylibloader_orig_libudev
#define udev_list_entry_get_next udev_list_entry_get_next_dylibloader_orig_libudev
#define udev_list_entry_get_by_name udev_list_entry_get_by_name_dylibloader_orig_libudev
#define udev_list_entry_get_name udev_list_entry_get_name_dylibloader_orig_libudev
#define udev_list_entry_get_value udev_list_entry_get_value_dylibloader_orig_libudev
#define udev_device_ref udev_device_ref_dylibloader_orig_libudev
#define udev_device_unref udev_device_unref_dylibloader_orig_libudev
#define udev_device_get_udev udev_device_get_udev_dylibloader_orig_libudev
#define udev_device_new_from_syspath udev_device_new_from_syspath_dylibloader_orig_libudev
#define udev_device_new_from_devnum udev_device_new_from_devnum_dylibloader_orig_libudev
#define udev_device_new_from_subsystem_sysname udev_device_new_from_subsystem_sysname_dylibloader_orig_libudev
#define udev_device_new_from_device_id udev_device_new_from_device_id_dylibloader_orig_libudev
#define udev_device_new_from_environment udev_device_new_from_environment_dylibloader_orig_libudev
#define udev_device_get_parent udev_device_get_parent_dylibloader_orig_libudev
#define udev_device_get_parent_with_subsystem_devtype udev_device_get_parent_with_subsystem_devtype_dylibloader_orig_libudev
#define udev_device_get_devpath udev_device_get_devpath_dylibloader_orig_libudev
#define udev_device_get_subsystem udev_device_get_subsystem_dylibloader_orig_libudev
#define udev_device_get_devtype udev_device_get_devtype_dylibloader_orig_libudev
#define udev_device_get_syspath udev_device_get_syspath_dylibloader_orig_libudev
#define udev_device_get_sysname udev_device_get_sysname_dylibloader_orig_libudev
#define udev_device_get_sysnum udev_device_get_sysnum_dylibloader_orig_libudev
#define udev_device_get_devnode udev_device_get_devnode_dylibloader_orig_libudev
#define udev_device_get_is_initialized udev_device_get_is_initialized_dylibloader_orig_libudev
#define udev_device_get_devlinks_list_entry udev_device_get_devlinks_list_entry_dylibloader_orig_libudev
#define udev_device_get_properties_list_entry udev_device_get_properties_list_entry_dylibloader_orig_libudev
#define udev_device_get_tags_list_entry udev_device_get_tags_list_entry_dylibloader_orig_libudev
#define udev_device_get_sysattr_list_entry udev_device_get_sysattr_list_entry_dylibloader_orig_libudev
#define udev_device_get_property_value udev_device_get_property_value_dylibloader_orig_libudev
#define udev_device_get_driver udev_device_get_driver_dylibloader_orig_libudev
#define udev_device_get_devnum udev_device_get_devnum_dylibloader_orig_libudev
#define udev_device_get_action udev_device_get_action_dylibloader_orig_libudev
#define udev_device_get_seqnum udev_device_get_seqnum_dylibloader_orig_libudev
#define udev_device_get_usec_since_initialized udev_device_get_usec_since_initialized_dylibloader_orig_libudev
#define udev_device_get_sysattr_value udev_device_get_sysattr_value_dylibloader_orig_libudev
#define udev_device_set_sysattr_value udev_device_set_sysattr_value_dylibloader_orig_libudev
#define udev_device_has_tag udev_device_has_tag_dylibloader_orig_libudev
#define udev_monitor_ref udev_monitor_ref_dylibloader_orig_libudev
#define udev_monitor_unref udev_monitor_unref_dylibloader_orig_libudev
#define udev_monitor_get_udev udev_monitor_get_udev_dylibloader_orig_libudev
#define udev_monitor_new_from_netlink udev_monitor_new_from_netlink_dylibloader_orig_libudev
#define udev_monitor_enable_receiving udev_monitor_enable_receiving_dylibloader_orig_libudev
#define udev_monitor_set_receive_buffer_size udev_monitor_set_receive_buffer_size_dylibloader_orig_libudev
#define udev_monitor_get_fd udev_monitor_get_fd_dylibloader_orig_libudev
#define udev_monitor_receive_device udev_monitor_receive_device_dylibloader_orig_libudev
#define udev_monitor_filter_add_match_subsystem_devtype udev_monitor_filter_add_match_subsystem_devtype_dylibloader_orig_libudev
#define udev_monitor_filter_add_match_tag udev_monitor_filter_add_match_tag_dylibloader_orig_libudev
#define udev_monitor_filter_update udev_monitor_filter_update_dylibloader_orig_libudev
#define udev_monitor_filter_remove udev_monitor_filter_remove_dylibloader_orig_libudev
#define udev_enumerate_ref udev_enumerate_ref_dylibloader_orig_libudev
#define udev_enumerate_unref udev_enumerate_unref_dylibloader_orig_libudev
#define udev_enumerate_get_udev udev_enumerate_get_udev_dylibloader_orig_libudev
#define udev_enumerate_new udev_enumerate_new_dylibloader_orig_libudev
#define udev_enumerate_add_match_subsystem udev_enumerate_add_match_subsystem_dylibloader_orig_libudev
#define udev_enumerate_add_nomatch_subsystem udev_enumerate_add_nomatch_subsystem_dylibloader_orig_libudev
#define udev_enumerate_add_match_sysattr udev_enumerate_add_match_sysattr_dylibloader_orig_libudev
#define udev_enumerate_add_nomatch_sysattr udev_enumerate_add_nomatch_sysattr_dylibloader_orig_libudev
#define udev_enumerate_add_match_property udev_enumerate_add_match_property_dylibloader_orig_libudev
#define udev_enumerate_add_match_sysname udev_enumerate_add_match_sysname_dylibloader_orig_libudev
#define udev_enumerate_add_match_tag udev_enumerate_add_match_tag_dylibloader_orig_libudev
#define udev_enumerate_add_match_parent udev_enumerate_add_match_parent_dylibloader_orig_libudev
#define udev_enumerate_add_match_is_initialized udev_enumerate_add_match_is_initialized_dylibloader_orig_libudev
#define udev_enumerate_add_syspath udev_enumerate_add_syspath_dylibloader_orig_libudev
#define udev_enumerate_scan_devices udev_enumerate_scan_devices_dylibloader_orig_libudev
#define udev_enumerate_scan_subsystems udev_enumerate_scan_subsystems_dylibloader_orig_libudev
#define udev_enumerate_get_list_entry udev_enumerate_get_list_entry_dylibloader_orig_libudev
#define udev_queue_ref udev_queue_ref_dylibloader_orig_libudev
#define udev_queue_unref udev_queue_unref_dylibloader_orig_libudev
#define udev_queue_get_udev udev_queue_get_udev_dylibloader_orig_libudev
#define udev_queue_new udev_queue_new_dylibloader_orig_libudev
#define udev_queue_get_kernel_seqnum udev_queue_get_kernel_seqnum_dylibloader_orig_libudev
#define udev_queue_get_udev_seqnum udev_queue_get_udev_seqnum_dylibloader_orig_libudev
#define udev_queue_get_udev_is_active udev_queue_get_udev_is_active_dylibloader_orig_libudev
#define udev_queue_get_queue_is_empty udev_queue_get_queue_is_empty_dylibloader_orig_libudev
#define udev_queue_get_seqnum_is_finished udev_queue_get_seqnum_is_finished_dylibloader_orig_libudev
#define udev_queue_get_seqnum_sequence_is_finished udev_queue_get_seqnum_sequence_is_finished_dylibloader_orig_libudev
#define udev_queue_get_fd udev_queue_get_fd_dylibloader_orig_libudev
#define udev_queue_flush udev_queue_flush_dylibloader_orig_libudev
#define udev_queue_get_queued_list_entry udev_queue_get_queued_list_entry_dylibloader_orig_libudev
#define udev_hwdb_new udev_hwdb_new_dylibloader_orig_libudev
#define udev_hwdb_ref udev_hwdb_ref_dylibloader_orig_libudev
#define udev_hwdb_unref udev_hwdb_unref_dylibloader_orig_libudev
#define udev_hwdb_get_properties_list_entry udev_hwdb_get_properties_list_entry_dylibloader_orig_libudev
#define udev_util_encode_string udev_util_encode_string_dylibloader_orig_libudev
#include <libudev.h>
#undef udev_ref
#undef udev_unref
#undef udev_new
#undef udev_set_log_fn
#undef udev_get_log_priority
#undef udev_set_log_priority
#undef udev_get_userdata
#undef udev_set_userdata
#undef udev_list_entry_get_next
#undef udev_list_entry_get_by_name
#undef udev_list_entry_get_name
#undef udev_list_entry_get_value
#undef udev_device_ref
#undef udev_device_unref
#undef udev_device_get_udev
#undef udev_device_new_from_syspath
#undef udev_device_new_from_devnum
#undef udev_device_new_from_subsystem_sysname
#undef udev_device_new_from_device_id
#undef udev_device_new_from_environment
#undef udev_device_get_parent
#undef udev_device_get_parent_with_subsystem_devtype
#undef udev_device_get_devpath
#undef udev_device_get_subsystem
#undef udev_device_get_devtype
#undef udev_device_get_syspath
#undef udev_device_get_sysname
#undef udev_device_get_sysnum
#undef udev_device_get_devnode
#undef udev_device_get_is_initialized
#undef udev_device_get_devlinks_list_entry
#undef udev_device_get_properties_list_entry
#undef udev_device_get_tags_list_entry
#undef udev_device_get_sysattr_list_entry
#undef udev_device_get_property_value
#undef udev_device_get_driver
#undef udev_device_get_devnum
#undef udev_device_get_action
#undef udev_device_get_seqnum
#undef udev_device_get_usec_since_initialized
#undef udev_device_get_sysattr_value
#undef udev_device_set_sysattr_value
#undef udev_device_has_tag
#undef udev_monitor_ref
#undef udev_monitor_unref
#undef udev_monitor_get_udev
#undef udev_monitor_new_from_netlink
#undef udev_monitor_enable_receiving
#undef udev_monitor_set_receive_buffer_size
#undef udev_monitor_get_fd
#undef udev_monitor_receive_device
#undef udev_monitor_filter_add_match_subsystem_devtype
#undef udev_monitor_filter_add_match_tag
#undef udev_monitor_filter_update
#undef udev_monitor_filter_remove
#undef udev_enumerate_ref
#undef udev_enumerate_unref
#undef udev_enumerate_get_udev
#undef udev_enumerate_new
#undef udev_enumerate_add_match_subsystem
#undef udev_enumerate_add_nomatch_subsystem
#undef udev_enumerate_add_match_sysattr
#undef udev_enumerate_add_nomatch_sysattr
#undef udev_enumerate_add_match_property
#undef udev_enumerate_add_match_sysname
#undef udev_enumerate_add_match_tag
#undef udev_enumerate_add_match_parent
#undef udev_enumerate_add_match_is_initialized
#undef udev_enumerate_add_syspath
#undef udev_enumerate_scan_devices
#undef udev_enumerate_scan_subsystems
#undef udev_enumerate_get_list_entry
#undef udev_queue_ref
#undef udev_queue_unref
#undef udev_queue_get_udev
#undef udev_queue_new
#undef udev_queue_get_kernel_seqnum
#undef udev_queue_get_udev_seqnum
#undef udev_queue_get_udev_is_active
#undef udev_queue_get_queue_is_empty
#undef udev_queue_get_seqnum_is_finished
#undef udev_queue_get_seqnum_sequence_is_finished
#undef udev_queue_get_fd
#undef udev_queue_flush
#undef udev_queue_get_queued_list_entry
#undef udev_hwdb_new
#undef udev_hwdb_ref
#undef udev_hwdb_unref
#undef udev_hwdb_get_properties_list_entry
#undef udev_util_encode_string
#ifdef __cplusplus
extern "C" {
#endif
#define udev_ref udev_ref_dylibloader_wrapper_libudev
#define udev_unref udev_unref_dylibloader_wrapper_libudev
#define udev_new udev_new_dylibloader_wrapper_libudev
#define udev_set_log_fn udev_set_log_fn_dylibloader_wrapper_libudev
#define udev_get_log_priority udev_get_log_priority_dylibloader_wrapper_libudev
#define udev_set_log_priority udev_set_log_priority_dylibloader_wrapper_libudev
#define udev_get_userdata udev_get_userdata_dylibloader_wrapper_libudev
#define udev_set_userdata udev_set_userdata_dylibloader_wrapper_libudev
#define udev_list_entry_get_next udev_list_entry_get_next_dylibloader_wrapper_libudev
#define udev_list_entry_get_by_name udev_list_entry_get_by_name_dylibloader_wrapper_libudev
#define udev_list_entry_get_name udev_list_entry_get_name_dylibloader_wrapper_libudev
#define udev_list_entry_get_value udev_list_entry_get_value_dylibloader_wrapper_libudev
#define udev_device_ref udev_device_ref_dylibloader_wrapper_libudev
#define udev_device_unref udev_device_unref_dylibloader_wrapper_libudev
#define udev_device_get_udev udev_device_get_udev_dylibloader_wrapper_libudev
#define udev_device_new_from_syspath udev_device_new_from_syspath_dylibloader_wrapper_libudev
#define udev_device_new_from_devnum udev_device_new_from_devnum_dylibloader_wrapper_libudev
#define udev_device_new_from_subsystem_sysname udev_device_new_from_subsystem_sysname_dylibloader_wrapper_libudev
#define udev_device_new_from_device_id udev_device_new_from_device_id_dylibloader_wrapper_libudev
#define udev_device_new_from_environment udev_device_new_from_environment_dylibloader_wrapper_libudev
#define udev_device_get_parent udev_device_get_parent_dylibloader_wrapper_libudev
#define udev_device_get_parent_with_subsystem_devtype udev_device_get_parent_with_subsystem_devtype_dylibloader_wrapper_libudev
#define udev_device_get_devpath udev_device_get_devpath_dylibloader_wrapper_libudev
#define udev_device_get_subsystem udev_device_get_subsystem_dylibloader_wrapper_libudev
#define udev_device_get_devtype udev_device_get_devtype_dylibloader_wrapper_libudev
#define udev_device_get_syspath udev_device_get_syspath_dylibloader_wrapper_libudev
#define udev_device_get_sysname udev_device_get_sysname_dylibloader_wrapper_libudev
#define udev_device_get_sysnum udev_device_get_sysnum_dylibloader_wrapper_libudev
#define udev_device_get_devnode udev_device_get_devnode_dylibloader_wrapper_libudev
#define udev_device_get_is_initialized udev_device_get_is_initialized_dylibloader_wrapper_libudev
#define udev_device_get_devlinks_list_entry udev_device_get_devlinks_list_entry_dylibloader_wrapper_libudev
#define udev_device_get_properties_list_entry udev_device_get_properties_list_entry_dylibloader_wrapper_libudev
#define udev_device_get_tags_list_entry udev_device_get_tags_list_entry_dylibloader_wrapper_libudev
#define udev_device_get_sysattr_list_entry udev_device_get_sysattr_list_entry_dylibloader_wrapper_libudev
#define udev_device_get_property_value udev_device_get_property_value_dylibloader_wrapper_libudev
#define udev_device_get_driver udev_device_get_driver_dylibloader_wrapper_libudev
#define udev_device_get_devnum udev_device_get_devnum_dylibloader_wrapper_libudev
#define udev_device_get_action udev_device_get_action_dylibloader_wrapper_libudev
#define udev_device_get_seqnum udev_device_get_seqnum_dylibloader_wrapper_libudev
#define udev_device_get_usec_since_initialized udev_device_get_usec_since_initialized_dylibloader_wrapper_libudev
#define udev_device_get_sysattr_value udev_device_get_sysattr_value_dylibloader_wrapper_libudev
#define udev_device_set_sysattr_value udev_device_set_sysattr_value_dylibloader_wrapper_libudev
#define udev_device_has_tag udev_device_has_tag_dylibloader_wrapper_libudev
#define udev_monitor_ref udev_monitor_ref_dylibloader_wrapper_libudev
#define udev_monitor_unref udev_monitor_unref_dylibloader_wrapper_libudev
#define udev_monitor_get_udev udev_monitor_get_udev_dylibloader_wrapper_libudev
#define udev_monitor_new_from_netlink udev_monitor_new_from_netlink_dylibloader_wrapper_libudev
#define udev_monitor_enable_receiving udev_monitor_enable_receiving_dylibloader_wrapper_libudev
#define udev_monitor_set_receive_buffer_size udev_monitor_set_receive_buffer_size_dylibloader_wrapper_libudev
#define udev_monitor_get_fd udev_monitor_get_fd_dylibloader_wrapper_libudev
#define udev_monitor_receive_device udev_monitor_receive_device_dylibloader_wrapper_libudev
#define udev_monitor_filter_add_match_subsystem_devtype udev_monitor_filter_add_match_subsystem_devtype_dylibloader_wrapper_libudev
#define udev_monitor_filter_add_match_tag udev_monitor_filter_add_match_tag_dylibloader_wrapper_libudev
#define udev_monitor_filter_update udev_monitor_filter_update_dylibloader_wrapper_libudev
#define udev_monitor_filter_remove udev_monitor_filter_remove_dylibloader_wrapper_libudev
#define udev_enumerate_ref udev_enumerate_ref_dylibloader_wrapper_libudev
#define udev_enumerate_unref udev_enumerate_unref_dylibloader_wrapper_libudev
#define udev_enumerate_get_udev udev_enumerate_get_udev_dylibloader_wrapper_libudev
#define udev_enumerate_new udev_enumerate_new_dylibloader_wrapper_libudev
#define udev_enumerate_add_match_subsystem udev_enumerate_add_match_subsystem_dylibloader_wrapper_libudev
#define udev_enumerate_add_nomatch_subsystem udev_enumerate_add_nomatch_subsystem_dylibloader_wrapper_libudev
#define udev_enumerate_add_match_sysattr udev_enumerate_add_match_sysattr_dylibloader_wrapper_libudev
#define udev_enumerate_add_nomatch_sysattr udev_enumerate_add_nomatch_sysattr_dylibloader_wrapper_libudev
#define udev_enumerate_add_match_property udev_enumerate_add_match_property_dylibloader_wrapper_libudev
#define udev_enumerate_add_match_sysname udev_enumerate_add_match_sysname_dylibloader_wrapper_libudev
#define udev_enumerate_add_match_tag udev_enumerate_add_match_tag_dylibloader_wrapper_libudev
#define udev_enumerate_add_match_parent udev_enumerate_add_match_parent_dylibloader_wrapper_libudev
#define udev_enumerate_add_match_is_initialized udev_enumerate_add_match_is_initialized_dylibloader_wrapper_libudev
#define udev_enumerate_add_syspath udev_enumerate_add_syspath_dylibloader_wrapper_libudev
#define udev_enumerate_scan_devices udev_enumerate_scan_devices_dylibloader_wrapper_libudev
#define udev_enumerate_scan_subsystems udev_enumerate_scan_subsystems_dylibloader_wrapper_libudev
#define udev_enumerate_get_list_entry udev_enumerate_get_list_entry_dylibloader_wrapper_libudev
#define udev_queue_ref udev_queue_ref_dylibloader_wrapper_libudev
#define udev_queue_unref udev_queue_unref_dylibloader_wrapper_libudev
#define udev_queue_get_udev udev_queue_get_udev_dylibloader_wrapper_libudev
#define udev_queue_new udev_queue_new_dylibloader_wrapper_libudev
#define udev_queue_get_kernel_seqnum udev_queue_get_kernel_seqnum_dylibloader_wrapper_libudev
#define udev_queue_get_udev_seqnum udev_queue_get_udev_seqnum_dylibloader_wrapper_libudev
#define udev_queue_get_udev_is_active udev_queue_get_udev_is_active_dylibloader_wrapper_libudev
#define udev_queue_get_queue_is_empty udev_queue_get_queue_is_empty_dylibloader_wrapper_libudev
#define udev_queue_get_seqnum_is_finished udev_queue_get_seqnum_is_finished_dylibloader_wrapper_libudev
#define udev_queue_get_seqnum_sequence_is_finished udev_queue_get_seqnum_sequence_is_finished_dylibloader_wrapper_libudev
#define udev_queue_get_fd udev_queue_get_fd_dylibloader_wrapper_libudev
#define udev_queue_flush udev_queue_flush_dylibloader_wrapper_libudev
#define udev_queue_get_queued_list_entry udev_queue_get_queued_list_entry_dylibloader_wrapper_libudev
#define udev_hwdb_new udev_hwdb_new_dylibloader_wrapper_libudev
#define udev_hwdb_ref udev_hwdb_ref_dylibloader_wrapper_libudev
#define udev_hwdb_unref udev_hwdb_unref_dylibloader_wrapper_libudev
#define udev_hwdb_get_properties_list_entry udev_hwdb_get_properties_list_entry_dylibloader_wrapper_libudev
#define udev_util_encode_string udev_util_encode_string_dylibloader_wrapper_libudev
extern struct udev* (*udev_ref_dylibloader_wrapper_libudev)(struct udev*);
extern struct udev* (*udev_unref_dylibloader_wrapper_libudev)(struct udev*);
extern struct udev* (*udev_new_dylibloader_wrapper_libudev)( void);
extern void (*udev_set_log_fn_dylibloader_wrapper_libudev)(struct udev*, void*);
extern int (*udev_get_log_priority_dylibloader_wrapper_libudev)(struct udev*);
extern void (*udev_set_log_priority_dylibloader_wrapper_libudev)(struct udev*, int);
extern void* (*udev_get_userdata_dylibloader_wrapper_libudev)(struct udev*);
extern void (*udev_set_userdata_dylibloader_wrapper_libudev)(struct udev*, void*);
extern struct udev_list_entry* (*udev_list_entry_get_next_dylibloader_wrapper_libudev)(struct udev_list_entry*);
extern struct udev_list_entry* (*udev_list_entry_get_by_name_dylibloader_wrapper_libudev)(struct udev_list_entry*,const char*);
extern const char* (*udev_list_entry_get_name_dylibloader_wrapper_libudev)(struct udev_list_entry*);
extern const char* (*udev_list_entry_get_value_dylibloader_wrapper_libudev)(struct udev_list_entry*);
extern struct udev_device* (*udev_device_ref_dylibloader_wrapper_libudev)(struct udev_device*);
extern struct udev_device* (*udev_device_unref_dylibloader_wrapper_libudev)(struct udev_device*);
extern struct udev* (*udev_device_get_udev_dylibloader_wrapper_libudev)(struct udev_device*);
extern struct udev_device* (*udev_device_new_from_syspath_dylibloader_wrapper_libudev)(struct udev*,const char*);
extern struct udev_device* (*udev_device_new_from_devnum_dylibloader_wrapper_libudev)(struct udev*, char, dev_t);
extern struct udev_device* (*udev_device_new_from_subsystem_sysname_dylibloader_wrapper_libudev)(struct udev*,const char*,const char*);
extern struct udev_device* (*udev_device_new_from_device_id_dylibloader_wrapper_libudev)(struct udev*,const char*);
extern struct udev_device* (*udev_device_new_from_environment_dylibloader_wrapper_libudev)(struct udev*);
extern struct udev_device* (*udev_device_get_parent_dylibloader_wrapper_libudev)(struct udev_device*);
extern struct udev_device* (*udev_device_get_parent_with_subsystem_devtype_dylibloader_wrapper_libudev)(struct udev_device*,const char*,const char*);
extern const char* (*udev_device_get_devpath_dylibloader_wrapper_libudev)(struct udev_device*);
extern const char* (*udev_device_get_subsystem_dylibloader_wrapper_libudev)(struct udev_device*);
extern const char* (*udev_device_get_devtype_dylibloader_wrapper_libudev)(struct udev_device*);
extern const char* (*udev_device_get_syspath_dylibloader_wrapper_libudev)(struct udev_device*);
extern const char* (*udev_device_get_sysname_dylibloader_wrapper_libudev)(struct udev_device*);
extern const char* (*udev_device_get_sysnum_dylibloader_wrapper_libudev)(struct udev_device*);
extern const char* (*udev_device_get_devnode_dylibloader_wrapper_libudev)(struct udev_device*);
extern int (*udev_device_get_is_initialized_dylibloader_wrapper_libudev)(struct udev_device*);
extern struct udev_list_entry* (*udev_device_get_devlinks_list_entry_dylibloader_wrapper_libudev)(struct udev_device*);
extern struct udev_list_entry* (*udev_device_get_properties_list_entry_dylibloader_wrapper_libudev)(struct udev_device*);
extern struct udev_list_entry* (*udev_device_get_tags_list_entry_dylibloader_wrapper_libudev)(struct udev_device*);
extern struct udev_list_entry* (*udev_device_get_sysattr_list_entry_dylibloader_wrapper_libudev)(struct udev_device*);
extern const char* (*udev_device_get_property_value_dylibloader_wrapper_libudev)(struct udev_device*,const char*);
extern const char* (*udev_device_get_driver_dylibloader_wrapper_libudev)(struct udev_device*);
extern dev_t (*udev_device_get_devnum_dylibloader_wrapper_libudev)(struct udev_device*);
extern const char* (*udev_device_get_action_dylibloader_wrapper_libudev)(struct udev_device*);
extern unsigned long long int (*udev_device_get_seqnum_dylibloader_wrapper_libudev)(struct udev_device*);
extern unsigned long long int (*udev_device_get_usec_since_initialized_dylibloader_wrapper_libudev)(struct udev_device*);
extern const char* (*udev_device_get_sysattr_value_dylibloader_wrapper_libudev)(struct udev_device*,const char*);
extern int (*udev_device_set_sysattr_value_dylibloader_wrapper_libudev)(struct udev_device*,const char*,const char*);
extern int (*udev_device_has_tag_dylibloader_wrapper_libudev)(struct udev_device*,const char*);
extern struct udev_monitor* (*udev_monitor_ref_dylibloader_wrapper_libudev)(struct udev_monitor*);
extern struct udev_monitor* (*udev_monitor_unref_dylibloader_wrapper_libudev)(struct udev_monitor*);
extern struct udev* (*udev_monitor_get_udev_dylibloader_wrapper_libudev)(struct udev_monitor*);
extern struct udev_monitor* (*udev_monitor_new_from_netlink_dylibloader_wrapper_libudev)(struct udev*,const char*);
extern int (*udev_monitor_enable_receiving_dylibloader_wrapper_libudev)(struct udev_monitor*);
extern int (*udev_monitor_set_receive_buffer_size_dylibloader_wrapper_libudev)(struct udev_monitor*, int);
extern int (*udev_monitor_get_fd_dylibloader_wrapper_libudev)(struct udev_monitor*);
extern struct udev_device* (*udev_monitor_receive_device_dylibloader_wrapper_libudev)(struct udev_monitor*);
extern int (*udev_monitor_filter_add_match_subsystem_devtype_dylibloader_wrapper_libudev)(struct udev_monitor*,const char*,const char*);
extern int (*udev_monitor_filter_add_match_tag_dylibloader_wrapper_libudev)(struct udev_monitor*,const char*);
extern int (*udev_monitor_filter_update_dylibloader_wrapper_libudev)(struct udev_monitor*);
extern int (*udev_monitor_filter_remove_dylibloader_wrapper_libudev)(struct udev_monitor*);
extern struct udev_enumerate* (*udev_enumerate_ref_dylibloader_wrapper_libudev)(struct udev_enumerate*);
extern struct udev_enumerate* (*udev_enumerate_unref_dylibloader_wrapper_libudev)(struct udev_enumerate*);
extern struct udev* (*udev_enumerate_get_udev_dylibloader_wrapper_libudev)(struct udev_enumerate*);
extern struct udev_enumerate* (*udev_enumerate_new_dylibloader_wrapper_libudev)(struct udev*);
extern int (*udev_enumerate_add_match_subsystem_dylibloader_wrapper_libudev)(struct udev_enumerate*,const char*);
extern int (*udev_enumerate_add_nomatch_subsystem_dylibloader_wrapper_libudev)(struct udev_enumerate*,const char*);
extern int (*udev_enumerate_add_match_sysattr_dylibloader_wrapper_libudev)(struct udev_enumerate*,const char*,const char*);
extern int (*udev_enumerate_add_nomatch_sysattr_dylibloader_wrapper_libudev)(struct udev_enumerate*,const char*,const char*);
extern int (*udev_enumerate_add_match_property_dylibloader_wrapper_libudev)(struct udev_enumerate*,const char*,const char*);
extern int (*udev_enumerate_add_match_sysname_dylibloader_wrapper_libudev)(struct udev_enumerate*,const char*);
extern int (*udev_enumerate_add_match_tag_dylibloader_wrapper_libudev)(struct udev_enumerate*,const char*);
extern int (*udev_enumerate_add_match_parent_dylibloader_wrapper_libudev)(struct udev_enumerate*,struct udev_device*);
extern int (*udev_enumerate_add_match_is_initialized_dylibloader_wrapper_libudev)(struct udev_enumerate*);
extern int (*udev_enumerate_add_syspath_dylibloader_wrapper_libudev)(struct udev_enumerate*,const char*);
extern int (*udev_enumerate_scan_devices_dylibloader_wrapper_libudev)(struct udev_enumerate*);
extern int (*udev_enumerate_scan_subsystems_dylibloader_wrapper_libudev)(struct udev_enumerate*);
extern struct udev_list_entry* (*udev_enumerate_get_list_entry_dylibloader_wrapper_libudev)(struct udev_enumerate*);
extern struct udev_queue* (*udev_queue_ref_dylibloader_wrapper_libudev)(struct udev_queue*);
extern struct udev_queue* (*udev_queue_unref_dylibloader_wrapper_libudev)(struct udev_queue*);
extern struct udev* (*udev_queue_get_udev_dylibloader_wrapper_libudev)(struct udev_queue*);
extern struct udev_queue* (*udev_queue_new_dylibloader_wrapper_libudev)(struct udev*);
extern unsigned long long int (*udev_queue_get_kernel_seqnum_dylibloader_wrapper_libudev)(struct udev_queue*);
extern unsigned long long int (*udev_queue_get_udev_seqnum_dylibloader_wrapper_libudev)(struct udev_queue*);
extern int (*udev_queue_get_udev_is_active_dylibloader_wrapper_libudev)(struct udev_queue*);
extern int (*udev_queue_get_queue_is_empty_dylibloader_wrapper_libudev)(struct udev_queue*);
extern int (*udev_queue_get_seqnum_is_finished_dylibloader_wrapper_libudev)(struct udev_queue*, unsigned long long int);
extern int (*udev_queue_get_seqnum_sequence_is_finished_dylibloader_wrapper_libudev)(struct udev_queue*, unsigned long long int, unsigned long long int);
extern int (*udev_queue_get_fd_dylibloader_wrapper_libudev)(struct udev_queue*);
extern int (*udev_queue_flush_dylibloader_wrapper_libudev)(struct udev_queue*);
extern struct udev_list_entry* (*udev_queue_get_queued_list_entry_dylibloader_wrapper_libudev)(struct udev_queue*);
extern struct udev_hwdb* (*udev_hwdb_new_dylibloader_wrapper_libudev)(struct udev*);
extern struct udev_hwdb* (*udev_hwdb_ref_dylibloader_wrapper_libudev)(struct udev_hwdb*);
extern struct udev_hwdb* (*udev_hwdb_unref_dylibloader_wrapper_libudev)(struct udev_hwdb*);
extern struct udev_list_entry* (*udev_hwdb_get_properties_list_entry_dylibloader_wrapper_libudev)(struct udev_hwdb*,const char*, unsigned);
extern int (*udev_util_encode_string_dylibloader_wrapper_libudev)(const char*, char*, size_t);
int initialize_libudev(int verbose);
#ifdef __cplusplus
}
#endif
#endif
