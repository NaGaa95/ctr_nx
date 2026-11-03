#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "offline_subscription.h"

static int checks;
#define CHECK(condition) do { checks++; if (!(condition)) { \
  fprintf(stderr, "line %d: %s\n", __LINE__, #condition); return 1; \
} } while (0)
static float surface_width, surface_height;
static int g_offline_subscription_visible = 1;
static int g_google_iap_purchase_succeeded = 1;
static struct { int purchase; } g_offline_billing_pending;
static const char *last_product;
static float android_native_width(void) { return surface_width; }
static float android_native_height(void) { return surface_height; }
static int start_native_local_purchase(const char *product) {
  last_product = product;
  return 1;
}

/* PRODUCTION_SUBSCRIPTION_TAP */

int main(void) {
  const float sizes[][2] = {{720,1280},{1080,1920},{1280,720},{1920,1080}};
  for (unsigned i=0; i<sizeof sizes/sizeof sizes[0]; i++) {
    surface_width=sizes[i][0]; surface_height=sizes[i][1];
    float sx=surface_width/720.0f, sy=surface_height/1280.0f;
    last_product=NULL;
    CHECK(jni_offline_subscription_tap(425*sx,860*sy) == 1);
    CHECK(last_product && !strcmp(last_product,"com.zeptolab.cuttheropelite.offer4"));
    CHECK(jni_offline_subscription_tap(515*sx,1080*sy) == 1);
    CHECK(!strcmp(last_product,"com.zeptolab.cuttheropelite.offer8"));
    CHECK(jni_offline_subscription_tap(100*sx,100*sy) == 0);
    CHECK(jni_offline_subscription_tap(425*sx,970*sy) == 0);
    CHECK(jni_offline_subscription_tap(300*sx,1080*sy) == 0);
    CHECK(jni_offline_subscription_tap(0,0) == 0);
    CHECK(jni_offline_subscription_tap(surface_width,surface_height) == 0);
    g_offline_subscription_visible=0;
    CHECK(jni_offline_subscription_tap(425*sx,860*sy) == 0);
    g_offline_subscription_visible=1;
    g_offline_billing_pending.purchase=1;
    CHECK(jni_offline_subscription_tap(425*sx,860*sy) == 0);
    g_offline_billing_pending.purchase=0;
    g_google_iap_purchase_succeeded=0;
    CHECK(jni_offline_subscription_tap(425*sx,860*sy) == 0);
    g_google_iap_purchase_succeeded=1;
  }
  CHECK(offline_subscription_offer_at(NAN,860,720,1280) == 0);
  CHECK(offline_subscription_offer_at(425,INFINITY,720,1280) == 0);
  CHECK(offline_subscription_offer_at(425,860,0,1280) == 0);
  CHECK(offline_subscription_offer_at(425,860,720,0) == 0);
  CHECK(offline_subscription_offer_at(425,860,-720,1280) == 0);
  CHECK(offline_subscription_offer_at(425,860,720,INFINITY) == 0);
  CHECK(offline_subscription_offer_at(-425,860,720,1280) == 0);
  CHECK(offline_subscription_offer_at(721,860,720,1280) == 0);
  CHECK(offline_subscription_offer_at(425,1281,720,1280) == 0);
  puts("PASS: subscription fallback accepts both offers at all four render sizes");
  printf("PASS: %d subscription integration checks\n",checks);
  return 0;
}
