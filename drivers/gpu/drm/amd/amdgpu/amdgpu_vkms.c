/* Public domain. */

#include <drm/drm_atomic_helper.h>
#include <drm/drm_vblank.h>

#include "amdgpu.h"
#include "amdgpu_vkms.h"
#include "amdgpu_display.h"

/**
 * DOC: amdgpu_vkms
 *
 * The amdgpu vkms interface provides a virtual KMS interface for several use
 * cases: devices without display hardware, platforms where the actual display
 * hardware is not useful (e.g., servers), SR-IOV virtual functions, device
 * emulation/simulation, and device bring up prior to display hardware being
 * usable. We previously emulated a legacy KMS interface, but there was a desire
 * to move to the atomic KMS interface. The vkms driver did everything we
 * needed, but we wanted KMS support natively in the driver without buffer
 * sharing and the ability to support an instance of VKMS per device. We first
 * looked at splitting vkms into a stub driver and a helper module that other
 * drivers could use to implement a virtual display, but this strategy ended up
 * being messy due to driver specific callbacks needed for buffer management.
 * Ultimately, it proved easier to import the vkms code as it mostly used core
 * drm helpers anyway.
 */

int amdgpu_vkms_output_init(struct drm_device *dev,
			    struct amdgpu_vkms_output *output, int index)
{
	return -EOPNOTSUPP;
}
