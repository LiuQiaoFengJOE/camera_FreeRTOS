#ifndef __ATBM_SPECIAL__
#define __ATBM_SPECIAL__
#include "atbm_type.h"

#include "atbm_list.h"
//#include "atbm_hal.h"




#define FILTER_BEACON 0x80
#define FILTER_PROBE_REQ 0x40
#define FILTER_PROBE_RESP 0x50




//#ifdef CONFIG_IEEE80211_SPECIAL_FILTER
enum {
	IEEE80211_SPECIAL_RX_PACKAGE_MSG						= 1,
	IEEE80211_SPECIAL_FRAME_FILTER_REGISTER_MSG				= 2,
	IEEE80211_SPECIAL_FRAME_FILTER_CLEAE_MSG    			= 3,
	IEEE80211_SPECIAL_FRAME_FILTER_REQUEST_MSG				= 4,		
	IEEE80211_SPECIAL_MAX_MSG,
};

#define IEEE80211_MAX_FILTERS		(10)
struct ieee80211_special_filter{
	unsigned char   filter_action;
	unsigned char   oui[3];
	unsigned int  flags; 
};

struct special_filter_table{
	int filter_num;
	struct ieee80211_special_filter special_filter[IEEE80211_MAX_FILTERS];
};

enum ieee80211_special_filter_flags{
	SPECIAL_F_FLAGS_FRAME_TYPE = BIT(0),
	SPECIAL_F_FLAGS_FRAME_OUI  = BIT(1),
	SPECIAL_F_FLAGS_FRAME_IE   = BIT(2),
};
struct ieee80211_special_filter_list{
	struct atbm_list_head list;
	struct ieee80211_special_filter filter;
};
struct ieee80211_special_filter_table{
	unsigned int n_filters;
	struct ieee80211_special_filter table[IEEE80211_MAX_FILTERS];
};
#define IEEE80211_SPECIAL_FILTER_MASK	(SPECIAL_F_FLAGS_FRAME_TYPE | SPECIAL_F_FLAGS_FRAME_OUI | SPECIAL_F_FLAGS_FRAME_IE)
//#endif

#if 0
void ieee80211_special_filter_rx_package_handle(struct atbmwifi_vif *priv,struct atbm_buff *skb);
int atbm_set_frame_filter(struct atbmwifi_common *hw_priv,struct atbmwifi_vif *priv,unsigned int n_filters,
		  struct ieee80211_special_filter *filter_table,char enable);


int set_special_beacon(struct atbmwifi_vif *priv,char *ie);


#endif



















#endif

