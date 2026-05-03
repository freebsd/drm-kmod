/* Public domain. */

#ifndef _DRM_MIPI_DSI_H_
#define _DRM_MIPI_DSI_H_

#include <linux/device.h>

#include <sys/types.h>
#include <linux/errno.h>
#include <linux/types.h>

struct mipi_dsi_host;
struct mipi_dsi_device;
struct mipi_dsi_msg;
struct drm_dsc_picture_parameter_set;

struct mipi_dsi_host_ops {
	int (*attach)(struct mipi_dsi_host *, struct mipi_dsi_device *);
	int (*detach)(struct mipi_dsi_host *, struct mipi_dsi_device *);
	ssize_t (*transfer)(struct mipi_dsi_host *, const struct mipi_dsi_msg *);
};

struct mipi_dsi_host {
	const struct mipi_dsi_host_ops *ops;
};

struct mipi_dsi_device {
	struct mipi_dsi_host *host;
	struct device dev;
	bool attached;
	uint32_t channel;
	uint32_t mode_flags;
#define MIPI_DSI_MODE_LPM	(1 << 0)
};

struct mipi_dsi_multi_context {
	struct mipi_dsi_device	*dsi;
	int			 accum_err;
};

struct mipi_dsi_msg {
	uint8_t type;
	uint8_t channel;
	uint16_t flags;
#define MIPI_DSI_MSG_USE_LPM	(1 << 0)
	const void *tx_buf;
	size_t tx_len;
	uint8_t *rx_buf;
	size_t rx_len;
};

struct mipi_dsi_packet {
	size_t size;
	size_t payload_length;
	uint8_t	header[4];
	const uint8_t *payload;
};

enum mipi_dsi_dcs_tear_mode {
	MIPI_DSI_DCS_TEAR_MODE_UNUSED
};

enum mipi_dsi_pixel_format {
	MIPI_DSI_FMT_RGB888,
	MIPI_DSI_FMT_RGB666,
	MIPI_DSI_FMT_RGB666_PACKED,
	MIPI_DSI_FMT_RGB565,
};

enum mipi_dsi_compression_algo {
	MIPI_DSI_COMPRESSION_DSC = 0,
	MIPI_DSI_COMPRESSION_VENDOR = 3,
};

bool mipi_dsi_packet_format_is_short(u8);
bool mipi_dsi_packet_format_is_long(u8);

int mipi_dsi_create_packet(struct mipi_dsi_packet *,
    const struct mipi_dsi_msg *);

int mipi_dsi_host_register(struct mipi_dsi_host *);
void mipi_dsi_host_unregister(struct mipi_dsi_host *);

void mipi_dsi_device_unregister(struct mipi_dsi_device *);
int mipi_dsi_attach(struct mipi_dsi_device *);
int mipi_dsi_detach(struct mipi_dsi_device *);
int devm_mipi_dsi_attach(struct device *, struct mipi_dsi_device *);
int mipi_dsi_shutdown_peripheral(struct mipi_dsi_device *);
int mipi_dsi_turn_on_peripheral(struct mipi_dsi_device *);
int mipi_dsi_set_maximum_return_packet_size(struct mipi_dsi_device *, u16);
int mipi_dsi_compression_mode(struct mipi_dsi_device *, bool);
int mipi_dsi_compression_mode_ext(struct mipi_dsi_device *, bool,
    enum mipi_dsi_compression_algo, unsigned int);
int mipi_dsi_picture_parameter_set(struct mipi_dsi_device *,
    const struct drm_dsc_picture_parameter_set *);

void mipi_dsi_compression_mode_ext_multi(struct mipi_dsi_multi_context *,
    bool, enum mipi_dsi_compression_algo, unsigned int);
void mipi_dsi_picture_parameter_set_multi(struct mipi_dsi_multi_context *,
    const struct drm_dsc_picture_parameter_set *);

ssize_t mipi_dsi_generic_write(struct mipi_dsi_device *, const void *, size_t);
int mipi_dsi_generic_write_chatty(struct mipi_dsi_device *,
    const void *, size_t);
void mipi_dsi_generic_write_multi(struct mipi_dsi_multi_context *,
    const void *, size_t);
ssize_t mipi_dsi_generic_read(struct mipi_dsi_device *,
    const void *, size_t, void *, size_t);

ssize_t mipi_dsi_dcs_write_buffer(struct mipi_dsi_device *,
    const void *, size_t);
int mipi_dsi_dcs_write_buffer_chatty(struct mipi_dsi_device *,
    const void *, size_t);
void mipi_dsi_dcs_write_buffer_multi(struct mipi_dsi_multi_context *,
				     const void *, size_t);
ssize_t mipi_dsi_dcs_write(struct mipi_dsi_device *, u8,
    const void *, size_t);
ssize_t mipi_dsi_dcs_read(struct mipi_dsi_device *, u8,
    void *, size_t);
int mipi_dsi_dcs_nop(struct mipi_dsi_device *);
int mipi_dsi_dcs_soft_reset(struct mipi_dsi_device *);
int mipi_dsi_dcs_get_power_mode(struct mipi_dsi_device *, u8 *);
int mipi_dsi_dcs_get_pixel_format(struct mipi_dsi_device *, u8 *);
int mipi_dsi_dcs_enter_sleep_mode(struct mipi_dsi_device *);
int mipi_dsi_dcs_exit_sleep_mode(struct mipi_dsi_device *);
int mipi_dsi_dcs_set_display_off(struct mipi_dsi_device *);
int mipi_dsi_dcs_set_display_on(struct mipi_dsi_device *);
int mipi_dsi_dcs_set_column_address(struct mipi_dsi_device *, u16, u16);
int mipi_dsi_dcs_set_page_address(struct mipi_dsi_device *, u16, u16);
int mipi_dsi_dcs_set_tear_off(struct mipi_dsi_device *);
int mipi_dsi_dcs_set_tear_on(struct mipi_dsi_device *,
    enum mipi_dsi_dcs_tear_mode);
int mipi_dsi_dcs_set_pixel_format(struct mipi_dsi_device *, u8);
int mipi_dsi_dcs_set_tear_scanline(struct mipi_dsi_device *, u16);
int mipi_dsi_dcs_set_display_brightness(struct mipi_dsi_device *, u16);
int mipi_dsi_dcs_get_display_brightness(struct mipi_dsi_device *, u16 *);
int mipi_dsi_dcs_set_display_brightness_large(struct mipi_dsi_device *, u16);
int mipi_dsi_dcs_get_display_brightness_large(struct mipi_dsi_device *, u16 *);

void mipi_dsi_dcs_nop_multi(struct mipi_dsi_multi_context *);
void mipi_dsi_dcs_enter_sleep_mode_multi(struct mipi_dsi_multi_context *);
void mipi_dsi_dcs_exit_sleep_mode_multi(struct mipi_dsi_multi_context *);
void mipi_dsi_dcs_set_display_off_multi(struct mipi_dsi_multi_context *);
void mipi_dsi_dcs_set_display_on_multi(struct mipi_dsi_multi_context *);
void mipi_dsi_dcs_set_tear_on_multi(struct mipi_dsi_multi_context *,
    enum mipi_dsi_dcs_tear_mode);
void mipi_dsi_turn_on_peripheral_multi(struct mipi_dsi_multi_context *);
void mipi_dsi_dcs_soft_reset_multi(struct mipi_dsi_multi_context *);
void mipi_dsi_dcs_set_display_brightness_multi(struct mipi_dsi_multi_context *,
    u16);
void mipi_dsi_dcs_set_pixel_format_multi(struct mipi_dsi_multi_context *, u8);
void mipi_dsi_dcs_set_column_address_multi(struct mipi_dsi_multi_context *,
    u16, u16);
void mipi_dsi_dcs_set_page_address_multi(struct mipi_dsi_multi_context *,
    u16, u16);
void mipi_dsi_dcs_set_tear_scanline_multi(struct mipi_dsi_multi_context *,
    u16);

static inline int
mipi_dsi_pixel_format_to_bpp(enum mipi_dsi_pixel_format fmt)
{
	switch (fmt) {
	case MIPI_DSI_FMT_RGB888:
	case MIPI_DSI_FMT_RGB666:
		return 24;
	case MIPI_DSI_FMT_RGB666_PACKED:
		return 18;
	case MIPI_DSI_FMT_RGB565:
		return 16;
	}
	return -EINVAL;
}

#endif
