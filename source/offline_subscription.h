#ifndef CTR_OFFLINE_SUBSCRIPTION_H
#define CTR_OFFLINE_SUBSCRIPTION_H

#include <math.h>

/* Input has already been rotated into render-surface coordinates by zf_input.
 * Keep the offer rectangles in their original 720x1280 reference space so the
 * fallback also works with a landscape surface or a docked resolution. */
static inline int offline_subscription_offer_at(float x, float y,
                                                float width, float height) {
  if (!isfinite(x) || !isfinite(y) || !isfinite(width) || !isfinite(height) ||
      width <= 0 || height <= 0 || x < 0 || y < 0 || x > width || y > height)
    return 0;

  x *= 720.0f / width;
  y *= 1280.0f / height;
  if (x >= 250.0f && x <= 600.0f && y >= 760.0f && y <= 960.0f)
    return 4;
  if (x >= 350.0f && x <= 680.0f && y >= 980.0f && y <= 1180.0f)
    return 8;
  return 0;
}

#endif
