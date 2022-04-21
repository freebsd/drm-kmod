/* Public domain. */

#ifndef _I915_MEI_HDCP_INTERFACE_H_
#define _I915_MEI_HDCP_INTERFACE_H_

#include <drm/display/drm_hdcp.h>

enum mei_fw_ddi {
	MEI_DDI_INVALID_PORT = 0x0,

	MEI_DDI_B = 1,
	MEI_DDI_C,
	MEI_DDI_D,
	MEI_DDI_E,
	MEI_DDI_F,
	MEI_DDI_A = 7,
	MEI_DDI_RANGE_END = MEI_DDI_A,
};

enum mei_fw_tc {
	MEI_INVALID_TRANSCODER = 0x00,
	MEI_TRANSCODER_EDP,
	MEI_TRANSCODER_DSI0,
	MEI_TRANSCODER_DSI1,
	MEI_TRANSCODER_A = 0x10,
	MEI_TRANSCODER_B,
	MEI_TRANSCODER_C,
	MEI_TRANSCODER_D
};

enum hdcp_wired_protocol {
	HDCP_PROTOCOL_INVALID,
	HDCP_PROTOCOL_HDMI,
	HDCP_PROTOCOL_DP
};

enum hdcp_port_type {
	HDCP_PORT_TYPE_INVALID,
	HDCP_PORT_TYPE_INTEGRATED,
	HDCP_PORT_TYPE_LSPCON,
	HDCP_PORT_TYPE_CPDP
};

struct hdcp_port_data {
	enum mei_fw_ddi fw_ddi;
	enum mei_fw_tc fw_tc;
	struct hdcp2_streamid_type *streams;
	u8 port_type;
	u8 protocol;
	uint32_t seq_num_m;
	uint16_t k;
};

struct i915_hdcp_component_ops {
	int (*initiate_hdcp2_session)(struct device *, struct hdcp_port_data *,
	    struct hdcp2_ake_init *);
	int (*verify_receiver_cert_prepare_km)(struct device *,
	    struct hdcp_port_data *, struct hdcp2_ake_send_cert *, bool *,
	    struct hdcp2_ake_no_stored_km *, size_t *);
	int (*verify_hprime)(struct device *, struct hdcp_port_data *,
	    struct hdcp2_ake_send_hprime *);
	int (*store_pairing_info)(struct device *, struct hdcp_port_data *,
	    struct hdcp2_ake_send_pairing_info *);
	int (*initiate_locality_check)(struct device *, struct hdcp_port_data *,
	    struct hdcp2_lc_init *);
	int (*verify_lprime)(struct device *, struct hdcp_port_data *,
	    struct hdcp2_lc_send_lprime *);
	int (*get_session_key)(struct device *, struct hdcp_port_data *,
	    struct hdcp2_ske_send_eks *);
	int (*repeater_check_flow_prepare_ack)(struct device *,
	    struct hdcp_port_data *, struct hdcp2_rep_send_receiverid_list *,
	    struct hdcp2_rep_send_ack *);
	int (*verify_mprime)(struct device *, struct hdcp_port_data *,
	    struct hdcp2_rep_stream_ready *);
	int (*enable_hdcp_authentication)(struct device *,
	    struct hdcp_port_data *);
	int (*close_hdcp_session)(struct device *, struct hdcp_port_data *);
};

struct i915_hdcp_comp_master {
	void *mei_dev;
	const struct i915_hdcp_component_ops *ops;
};

#endif
