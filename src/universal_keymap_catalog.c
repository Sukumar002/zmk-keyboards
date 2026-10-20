/* Auto-generated experimental keymap catalogue from saved Studio backups.
 * Hana source: 2026-10-11 (6 layers, 46 slots).
 * Chocofi source: 2026-10-20 (4 layers, 40 slots).
 * These are DATA ONLY: the active ZMK keymap is unchanged.
 * SPDX-License-Identifier: MIT
 */
#include <zephyr/init.h>
#include <zephyr/logging/log.h>
#include <stdint.h>
LOG_MODULE_REGISTER(universal_catalog, LOG_LEVEL_INF);
struct ud_catalog_binding { uint8_t behavior; uint32_t p1; uint32_t p2; };
struct ud_catalog_combo { uint8_t behavior; uint32_t p1; uint32_t p2; uint8_t count; uint8_t key1; uint8_t key2; uint16_t timeout; };
static const char *const ud_behavior_names[] = {"Layer-Tap","Key Press","Mod-Tap","Studio Unlock","Transparent","Toggle Layer","None","bt_profile","Bootloader","Output Selection","Bluetooth","Mouse Scroll","Mouse Move","Mouse Key Press",""};
static const struct ud_catalog_binding ud_hana_layer_0[46] = {{0, 4u, 458795u},{1, 458772u, 0u},{1, 458778u, 0u},{1, 458760u, 0u},{1, 458773u, 0u},{1, 458775u, 0u},{1, 458780u, 0u},{1, 458776u, 0u},{1, 458764u, 0u},{1, 458770u, 0u},{1, 458771u, 0u},{1, 458794u, 0u},{2, 458977u, 458805u},{2, 458977u, 458756u},{1, 458774u, 0u},{1, 458759u, 0u},{1, 458761u, 0u},{1, 458762u, 0u},{1, 458763u, 0u},{1, 458765u, 0u},{1, 458766u, 0u},{1, 458767u, 0u},{2, 458981u, 458803u},{2, 458981u, 458804u},{2, 458976u, 458793u},{2, 458976u, 458781u},{1, 458779u, 0u},{1, 458758u, 0u},{1, 458777u, 0u},{1, 458757u, 0u},{1, 786658u, 0u},{1, 786637u, 0u},{1, 458769u, 0u},{1, 458768u, 0u},{1, 458806u, 0u},{1, 458807u, 0u},{2, 458980u, 458808u},{2, 458980u, 458828u},{2, 458976u, 458809u},{2, 458978u, 458794u},{2, 458979u, 458792u},{0, 2u, 458796u},{0, 3u, 458792u},{2, 458979u, 458796u},{2, 458978u, 458828u},{3, 0u, 0u}};
static const struct ud_catalog_binding ud_hana_layer_1[46] = {{0, 4u, 458795u},{1, 458772u, 0u},{1, 458778u, 0u},{1, 458760u, 0u},{1, 458773u, 0u},{1, 458775u, 0u},{1, 458780u, 0u},{1, 458776u, 0u},{1, 458764u, 0u},{1, 458770u, 0u},{1, 458771u, 0u},{1, 458794u, 0u},{2, 458977u, 458805u},{2, 458977u, 458756u},{1, 458774u, 0u},{1, 458759u, 0u},{1, 458761u, 0u},{1, 458762u, 0u},{1, 458763u, 0u},{1, 458765u, 0u},{1, 458766u, 0u},{1, 458767u, 0u},{2, 458981u, 458803u},{2, 458981u, 458804u},{2, 458976u, 458793u},{2, 458976u, 458781u},{1, 458779u, 0u},{1, 458758u, 0u},{1, 458777u, 0u},{1, 458757u, 0u},{1, 786658u, 0u},{1, 786637u, 0u},{1, 458769u, 0u},{1, 458768u, 0u},{1, 458806u, 0u},{1, 458807u, 0u},{2, 458980u, 458808u},{2, 458980u, 458828u},{2, 458976u, 458809u},{2, 458979u, 458794u},{2, 458978u, 458792u},{0, 2u, 458796u},{0, 3u, 458792u},{2, 458978u, 458796u},{2, 458979u, 458828u},{3, 0u, 0u}};
static const struct ud_catalog_binding ud_hana_layer_2[46] = {{1, 458820u, 0u},{1, 458810u, 0u},{1, 458811u, 0u},{1, 458812u, 0u},{1, 458813u, 0u},{1, 458814u, 0u},{1, 458815u, 0u},{1, 458816u, 0u},{1, 458817u, 0u},{1, 458818u, 0u},{1, 458819u, 0u},{4, 0u, 0u},{1, 458821u, 0u},{2, 458977u, 458782u},{1, 458783u, 0u},{1, 458784u, 0u},{1, 458785u, 0u},{1, 458786u, 0u},{1, 458787u, 0u},{1, 458788u, 0u},{1, 458789u, 0u},{1, 458790u, 0u},{2, 458981u, 458791u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{1, 458801u, 0u},{1, 458799u, 0u},{1, 458800u, 0u},{4, 0u, 0u},{4, 0u, 0u},{1, 458798u, 0u},{1, 458797u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u}};
static const struct ud_catalog_binding ud_hana_layer_3[46] = {{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{1, 786665u, 0u},{1, 458826u, 0u},{1, 458834u, 0u},{1, 458829u, 0u},{1, 458827u, 0u},{1, 458794u, 0u},{4, 0u, 0u},{4, 0u, 0u},{1, 786614u, 0u},{1, 786637u, 0u},{1, 786613u, 0u},{1, 786665u, 0u},{1, 786666u, 0u},{1, 458832u, 0u},{1, 458833u, 0u},{1, 458831u, 0u},{1, 458830u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{1, 786544u, 0u},{1, 786543u, 0u},{1, 786658u, 0u},{1, 458809u, 0u},{4, 0u, 0u},{4, 0u, 0u},{1, 786658u, 0u},{1, 458798u, 0u},{1, 34013231u, 0u},{1, 34013232u, 0u},{4, 0u, 0u},{1, 458828u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{5, 1u, 0u}};
static const struct ud_catalog_binding ud_hana_layer_4[46] = {{6, 0u, 0u},{7, 0u, 0u},{7, 1u, 1u},{7, 2u, 2u},{7, 3u, 3u},{7, 4u, 4u},{8, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{9, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{10, 4u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{9, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u},{6, 0u, 0u}};
static const struct ud_catalog_binding ud_hana_layer_5[46] = {{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{11, 10u, 0u},{11, 4294311936u, 0u},{12, 64936u, 0u},{11, 655360u, 0u},{11, 655360u, 0u},{4, 0u, 0u},{4, 0u, 0u},{13, 1u, 0u},{13, 4u, 0u},{13, 2u, 0u},{4, 0u, 0u},{4, 0u, 0u},{11, 65526u, 0u},{12, 4255645696u, 0u},{12, 600u, 0u},{12, 39321600u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{13, 1u, 0u},{13, 4u, 0u},{13, 2u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u}};
static const struct ud_catalog_binding *const ud_hana_layers[] = {ud_hana_layer_0,ud_hana_layer_1,ud_hana_layer_2,ud_hana_layer_3,ud_hana_layer_4,ud_hana_layer_5};
static const uint8_t ud_hana_layer_lengths[] = {46,46,46,46,46,46};
static const char *const ud_hana_layer_names[] = {"MAC Base","WIN Base","NUM","NAV","SET","MOUSE"};
static const struct ud_catalog_combo ud_hana_combos[] = {{2,458978u,458794u,2,7,19,50},{2,458983u,458828u,2,9,21,50},{2,458978u,458795u,2,15,16,50},{2,458979u,458805u,2,14,15,50},{1,458804u,0u,2,21,22,50},{1,458793u,0u,2,1,2,50},{0,5u,458835u,2,3,40,50},{1,458978u,0u,2,19,20,50},{1,458983u,0u,2,20,21,50}};
static const struct ud_catalog_binding ud_chocofi_layer_0[40] = {{1, 458772u, 0u},{1, 458778u, 0u},{1, 458760u, 0u},{1, 458773u, 0u},{1, 458775u, 0u},{1, 458780u, 0u},{1, 458776u, 0u},{1, 458764u, 0u},{1, 458770u, 0u},{1, 458771u, 0u},{2, 458977u, 458756u},{2, 458978u, 458774u},{2, 458979u, 458759u},{2, 458976u, 458761u},{1, 458762u, 0u},{1, 458763u, 0u},{2, 458980u, 458765u},{2, 458983u, 458766u},{2, 458978u, 458767u},{2, 458981u, 458803u},{1, 458781u, 0u},{1, 458779u, 0u},{1, 458758u, 0u},{1, 458777u, 0u},{1, 458757u, 0u},{1, 458769u, 0u},{1, 458768u, 0u},{1, 458806u, 0u},{1, 458807u, 0u},{1, 458808u, 0u},{0, 3u, 458793u},{0, 1u, 458792u},{0, 1u, 458795u},{0, 2u, 458792u},{2, 458978u, 458796u},{0, 3u, 458794u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u}};
static const struct ud_catalog_binding ud_chocofi_layer_1[40] = {{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{1, 458799u, 0u},{1, 458800u, 0u},{1, 458788u, 0u},{1, 458789u, 0u},{1, 458790u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{1, 458798u, 0u},{1, 458785u, 0u},{1, 458786u, 0u},{1, 458787u, 0u},{1, 34013235u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{1, 458801u, 0u},{1, 458797u, 0u},{1, 458782u, 0u},{1, 458783u, 0u},{1, 458784u, 0u},{1, 458807u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{1, 458791u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u}};
static const struct ud_catalog_binding ud_chocofi_layer_2[40] = {{1, 458819u, 0u},{1, 458816u, 0u},{1, 458817u, 0u},{1, 458818u, 0u},{1, 458814u, 0u},{1, 458827u, 0u},{1, 458826u, 0u},{1, 458834u, 0u},{1, 458829u, 0u},{4, 0u, 0u},{1, 458820u, 0u},{1, 458813u, 0u},{1, 458814u, 0u},{1, 458815u, 0u},{1, 458799u, 0u},{1, 458830u, 0u},{1, 458832u, 0u},{1, 458833u, 0u},{1, 458831u, 0u},{4, 0u, 0u},{1, 458821u, 0u},{1, 458810u, 0u},{1, 458811u, 0u},{1, 458812u, 0u},{1, 34013233u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{4, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u}};
static const struct ud_catalog_binding ud_chocofi_layer_3[40] = {{10, 3u, 0u},{10, 3u, 1u},{10, 3u, 2u},{10, 3u, 3u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u},{14, 0u, 0u}};
static const struct ud_catalog_binding *const ud_chocofi_layers[] = {ud_chocofi_layer_0,ud_chocofi_layer_1,ud_chocofi_layer_2,ud_chocofi_layer_3};
static const uint8_t ud_chocofi_layer_lengths[] = {40,40,40,40};
static const char *const ud_chocofi_layer_names[] = {"Base","Num","Nav","Set"};
static const struct ud_catalog_combo ud_chocofi_combos[] = {{3,0u,0u,2,30,35,50},{2,458978u,458795u,2,12,13,50},{2,458979u,458805u,2,11,12,50},{2,458978u,458794u,2,16,17,50},{2,458976u,458804u,2,18,19,50},{2,458983u,458828u,2,17,18,50}};
/* Referencing every entry prevents linker GC from removing the inactive board. */
static uint32_t ud_catalog_checksum(void) {
 uint32_t s=2166136261u;
 for(unsigned board=0;board<2;board++){
  const struct ud_catalog_binding *const *layers=board ? ud_chocofi_layers : ud_hana_layers;
  const uint8_t *lens=board ? ud_chocofi_layer_lengths : ud_hana_layer_lengths;
  const char *const *names=board ? ud_chocofi_layer_names : ud_hana_layer_names;
  unsigned nl=board ? 4 : 6;
  for(unsigned l=0;l<nl;l++){
   for(const char *p=names[l];*p;p++) s=(s^(uint8_t)*p)*16777619u;
   for(unsigned k=0;k<lens[l];k++){ const struct ud_catalog_binding *v=&layers[l][k];s=(s^v->behavior)*16777619u;s=(s^v->p1)*16777619u;s=(s^v->p2)*16777619u; }
  }
  const struct ud_catalog_combo *combos=board ? ud_chocofi_combos : ud_hana_combos;
  unsigned nc=board ? 6 : 9;
  for(unsigned i=0;i<nc;i++){s=(s^combos[i].key1)*16777619u;s=(s^combos[i].key2)*16777619u;s=(s^combos[i].p1)*16777619u;s=(s^combos[i].p2)*16777619u;s=(s^combos[i].behavior)*16777619u;s=(s^combos[i].timeout)*16777619u;}
 }
 for(unsigned i=0;i<sizeof(ud_behavior_names)/sizeof(ud_behavior_names[0]);i++) for(const char *p=ud_behavior_names[i];*p;p++) s=(s^(uint8_t)*p)*16777619u;
 return s;
}
static int ud_catalog_probe(void) {
 LOG_INF("Universal keymap CATALOG ONLY: Hana 6x46 + %u combos, Chocofi 4x40 + %u combos; checksum %08x",9,6,ud_catalog_checksum());
 return 0;
}
SYS_INIT(ud_catalog_probe, APPLICATION, 95);
