/* Public domain. */

#include <drm/drm_gem.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_framebuffer.h>
#include <drm/drm_modeset_helper.h>

struct drm_gem_object *
drm_gem_fb_get_obj(struct drm_framebuffer *fb, unsigned int plane)
{
	if (plane >= DRM_FORMAT_MAX_PLANES)
		return NULL;

	return fb->obj[plane];
}

void
drm_gem_fb_destroy(struct drm_framebuffer *fb)
{
	int i;

	for (i = 0; i < 4; i++)
		drm_gem_object_put(fb->obj[i]);
	drm_framebuffer_cleanup(fb);
	kfree(fb);
}

int
drm_gem_fb_create_handle(struct drm_framebuffer *fb, struct drm_file *file,
    unsigned int *handle)
{
	return drm_gem_handle_create(file, fb->obj[0], handle);
}

int drm_gem_fb_init_with_funcs(struct drm_device *dev,
    struct drm_framebuffer *fb, struct drm_file *file,
    const struct drm_mode_fb_cmd2 *mode_cmd,
    const struct drm_framebuffer_funcs *funcs)
{
	const struct drm_format_info *info;
	struct drm_gem_object **objs;
	int ret, i;

	info = drm_get_format_info(dev, mode_cmd);
	objs = kcalloc(info->num_planes, sizeof(*objs), GFP_KERNEL);

	for (i = 0; i < info->num_planes; ++i) {
		objs[i] = drm_gem_object_lookup(file, mode_cmd->handles[i]);

		if (objs[i] == NULL) {
			ret = -ENOENT;
			goto err;
		}
	}

	drm_helper_mode_fill_fb_struct(dev, fb, mode_cmd);
	for (i = 0; i < info->num_planes; ++i) {
		fb->obj[i] = objs[i];
	}

	ret = drm_framebuffer_init(dev, fb, funcs);
	if (ret != 0) {
		goto err;
	}

	kfree(objs);

	return 0;

err:
	for (--i; i >= 0; i--) {
		drm_gem_object_put(objs[i]);
	}
	kfree(objs);
	return ret;
}
