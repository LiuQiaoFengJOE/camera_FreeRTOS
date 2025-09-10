/*
*  (C) Copyright 2014-2016 Shenzhen South Silicon Valley microelectronics co.,limited
*
*  All Rights Reserved
*/

#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "lwip/netifapi.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/lwip_pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ethernetif.h"
#include "rtos.h"
#include <host_apis.h>
#include <net_mgr.h>
#include <netstack.h>
#include <ssv_frame.h>

#if INCLUDE_DHCPD
#include "udhcp/udhcp_common.h"
#include "udhcp/dhcpc.h"
#include "udhcp/dhcpd.h"
#endif

#if CONFIG_STATUS_CHECK
extern ssv_type_u32 g_l2_tx_packets;
extern ssv_type_u32 g_l2_tx_copy;
extern ssv_type_u32 g_notpool;
extern ssv_type_u32 g_tx_allocate_fail;
#endif
extern ssv_type_u32 g_dump_tx;
extern char *if_name[];

extern void netmgr_ifup_cb(ssv_type_u8 vif_idx);
extern void cli_ps_resume(void);
void netdev_status_change_cb(struct netif *netif)
{
    ssv_type_u8 vif_idx;
    for(vif_idx=0;vif_idx<MAX_VIF_NUM;vif_idx++)
    {
        if(!ssv6xxx_memcmp((void*)if_name[vif_idx], (void*)netif->name, sizeof(WLAN_IFNAME)))
            break;
    }

    if (netif->flags & NETIF_FLAG_UP)
    {
        netmgr_ifup_cb(vif_idx);
#if CLI_ENABLE
        cli_ps_resume();
#endif
    }
}

/* Transfer L2 packet to netstack
 * [in] data: pointer of incoming data
 * [in] len: length of real 802.3 packet
 * Transfer incoming wifi data to be processed by netstack 
 */
int netstack_input(void *data, ssv_type_u32 len, ssv_type_u8 vif_idx)
{
    //ethernetif_input(data, len);
    struct eth_hdr *ethhdr;
    struct pbuf *p;

    struct netif *pwlan = NULL;
    
 


   if (NULL == data){
          LOG_PRINTF("sentton netstack_input fail\n");
          return ERR_OK;

   }
  // return SSV6XXX_DATA_ACPT;
   

#if 1
if(vif_idx>=MAX_VIF_NUM)
   {
       LOG_PRINTF("netstack_input vif invalid =%d\r\n",vif_idx);
       SSV_ASSERT(0);
   }
     
   pwlan = netif_find((char *)if_name[vif_idx]);
    
#if CONFIG_USE_LWIP_PBUF 
    
#if ETH_PAD_SIZE
    ssv_type_u8 retry_cnt = 5;
    struct pbuf *r;
#endif
    
    p = (struct pbuf *)data;

#if ETH_PAD_SIZE
    if (pbuf_header(p, ETH_PAD_SIZE) != ERR_OK)
    {
        LOG_PRINTF ("ethernetif_input: could not allocate room for header.\r\n");
        pbuf_free(p);
        return SSV6XXX_DATA_ACPT;
    }

    if ((unsigned int)(p->payload) & 3)
    {
retry:
        r = os_frame_alloc(p->tot_len,FALSE);
        if (NULL == r)
        {
            retry_cnt--;
            if(0 == retry_cnt)
            {
                pbuf_free(p);
                return SSV6XXX_DATA_ACPT;
            }
            OS_TickDelay(1);
            goto retry;
        }
        pbuf_copy(r, p);
        pbuf_free(p);  
        p = r;
    }
#endif
    
      ethhdr = p->payload;
      
      LOG_DEBUGF(LOG_L2_DATA, ("RX Ether type[%x] len[%d]\r\n", htons(ethhdr->type), len)); 
      switch (htons(ethhdr->type)) {
      /* IP or ARP packet? */
      case ETHTYPE_IP:
      case ETHTYPE_ARP:
#if PPPOE_SUPPORT
      /* PPPoE packet? */
      case ETHTYPE_PPPOEDISC:
      case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
        //eth0=netif_default;
        if(pwlan==NULL){
            pbuf_free(p);
        }
        else{
            /* full packet send to tcpip_thread to process */
               //LOG_PRINTF("full packet send to tcpip_thread to process\n");
            if (pwlan->input(p, pwlan)!=ERR_OK)
            {
                LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\r\n"));
                pbuf_free(p);
                p = NULL;
            }
        }
        break;
      default:
        pbuf_free(p);
        p = NULL;
        break;
      }
    
      return ERR_OK;

#else//CONFIG_USE_LWIP_PBUF
    {
        struct pbuf *q;
        ssv_type_u8 retry_cnt = 5;

        ssv_type_u8 *r=OS_FRAME_GET_DATA(data);
        ssv_type_u32 offset=0;


        retry_cnt=0;
    retry:
       
        p = pbuf_alloc(PBUF_RAW, len-offset, PBUF_POOL);
       
        if (NULL == p)
        {
            retry_cnt++;
            if(100 == retry_cnt)
            {
                retry_cnt=0;
                //pbuf_free(p);
                //return SSV6XXX_DATA_ACPT;
                LOG_PRINTF("input:block to wait buf\r\n");
            }
            OS_TickDelay(1);
            goto retry;
        }

#if ETH_PAD_SIZE
        if (pbuf_header(p, ETH_PAD_SIZE) != ERR_OK)
        {
            LOG_PRINTF ("ethernetif_input: could not allocate room for header.\r\n");
            pbuf_free(p);
            return SSV6XXX_DATA_ACPT;
        }
#endif    
        /* points to packet payload, which starts with an Ethernet header */
        if(p->len!=p->tot_len) 
        {
          for( q = p; q != NULL; q = q->next ) {          
              OS_MemCPY ((void*)q->payload,(void*)r,q->len);
              r=(ssv_type_u8 *)r+q->len;
          }
          
        }
        else{
          OS_MemCPY (p->payload,r,p->len);
        }
        //  LOG_PRINTF("11p->payload11\n");
        ethhdr = p->payload;
      //  LOG_PRINTF("22p->payload22\n");
        switch (htons(ethhdr->type)) {
        /* IP or ARP packet? */
        case ETHTYPE_IP:
        case ETHTYPE_ARP:
#if PPPOE_SUPPORT
        /* PPPoE packet? */
        case ETHTYPE_PPPOEDISC:
        case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
        
        if(pwlan==NULL){
            pbuf_free(p);
        }
        else{
            /* full packet send to tcpip_thread to process */
        //    LOG_PRINTF("full packet send to tcpip_thread to process\n");
            if (pwlan->input(p, pwlan)!=ERR_OK)
            {
                LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\r\n"));
                pbuf_free(p);
                p = NULL;
            }
        }
        break;
        default:
            pbuf_free(p);
            p = NULL;
        break;
        }
       
        os_frame_free(data);
       

        //todo:
        return SSV6XXX_DATA_ACPT;
    }
#endif
#endif 

}

/* Transfer netstack packet to L2
 * [in] data: pointer of output data
 * [in] len: length of real 802.3 packet
 * Transfer output data to be sent 
 */
int netstack_output(void* net_interface, void *data, ssv_type_u32 len)
{    

    struct pbuf *q;
    static unsigned char *ucBuffer;  
    unsigned char *pucChar;
    ssv_type_u8 *frame;
    ssv_type_u8 retry_cnt = 5;
    struct pbuf *p = (struct pbuf *)data;
    struct netif *net_if = (struct netif*)net_interface;
    ssv_type_u8 vif_idx=0;
	#if 0
	static u32 maxDataOffset;
  static u32 lastTime;
  #define RTP_MAX_IDX  52
  static u8 rtpIdx[RTP_MAX_IDX];
  u32 i,idxErr;
	#endif
    for(vif_idx=0;vif_idx<MAX_VIF_NUM;vif_idx++)
    {
        if(!ssv6xxx_memcmp((void*)if_name[vif_idx], (void*)net_if->name, sizeof(WLAN_IFNAME)))
            break;
    }
  //  LOG_PRINTF("net_if->name=%s,vif_idx=%d\r\n",net_if->name,vif_idx);

    if(vif_idx>=MAX_VIF_NUM)
    {
        LOG_PRINTF("net_if->name=%s\r\n",net_if->name);
        SSV_ASSERT(0);
    }

#if CONFIG_USE_LWIP_PBUF
    
#if  CONFIG_STATUS_CHECK
    g_l2_tx_packets++;
#endif
    
      /* Initiate transfer. */
      
    if( p->len == p->tot_len && ((p->type == PBUF_POOL)))
    {
    /* No pbuf chain,  just give it to layer 2 */

#if ETH_PAD_SIZE
        if (pbuf_header(p, 0 - ETH_PAD_SIZE) != ERR_OK)
        {
            LOG_PRINTF ("low_level_output: could not allocate room for header.\r\n");
            pbuf_free(p);
            return ERR_BUF;
        }
#endif
        frame = (void*)p;
        pbuf_ref(p);        
    }
    else 
    {

#if CONFIG_STATUS_CHECK
        g_l2_tx_copy ++;
        if(p->type != PBUF_POOL)
            g_notpool++;
#endif

retry:        
        frame = os_frame_alloc(p->tot_len*sizeof(unsigned char ),FALSE);
        if(NULL == frame)
        {
            retry_cnt--;

#if CONFIG_STATUS_CHECK
            g_tx_allocate_fail++;
#endif

            //don't xmit this frame. no enough frame.
            if(0 == retry_cnt){
				LOG_PRINTF("netstack_output:alloc buf err:%d\r\n",p->tot_len);
				return ERR_MEM;
            }
            OS_TickDelay(1);
            goto retry;
        }

        ucBuffer = OS_FRAME_GET_DATA(frame);                
        pucChar = ucBuffer;
        for( q = p; q != NULL; q = q->next ) 
        {
            /* Send the data from the pbuf to the interface, one pbuf at a
                        time. The size of the data in each pbuf is kept in the ->len
                        variable. */
            /* send data from(q->payload, q->len); */
            LWIP_DEBUGF( NETIF_DEBUG, ("NETIF: send pucChar %p q->payload %p q->len %i q->next %p\r\n", pucChar, q->payload, ( int ) q->len, ( void* ) q->next ) );
            if( q == p ) 
            {
                OS_MemCPY( pucChar, &( ( char * ) q->payload )[ ETH_PAD_SIZE ], q->len - ETH_PAD_SIZE );
                pucChar += q->len - ETH_PAD_SIZE;                   
            } 
            else 
            {                   
                OS_MemCPY( pucChar, q->payload, q->len );
                pucChar += q->len;      
            }               
        }       


    }
        
#if CONFIG_MEMP_DEBUG
    PBUF_DBG_FLAGS(((struct pbuf *)frame), PBUF_DBG_FLAG_L2);
#endif
        
    if(g_dump_tx)
    {
        packetdump("low_level_output", OS_FRAME_GET_DATA(frame), OS_FRAME_GET_DATA_LEN(frame));
    }

#else //CONFIG_USE_LWIP_PBUF
    {
        unsigned char *pucBuffer = ucBuffer;

    retry:
        frame = os_frame_alloc(p->tot_len*sizeof(unsigned char ),FALSE);
        if(NULL == frame)
        {
            retry_cnt--;

            //don't xmit this frame. no enough frame.
            if(0 == retry_cnt){
				LOG_PRINTF("netstack_output:block to wait buf:%d\r\n",p->tot_len);
                return ERR_MEM;
            }
            OS_TickDelay(1);
            goto retry;
        }
            
        ucBuffer = OS_FRAME_GET_DATA(frame);

        /* Initiate transfer. */
		pucBuffer = &( ( unsigned char * ) p->payload )[ ETH_PAD_SIZE ];
		#if 0
		if(os_time_get()-lastTime>2000)
		{
			lastTime=os_time_get();
			printf("net rtp:");
			for(i=58;i<80;i++)
			{
				printf("%x ", pucBuffer[i]);
			}
			printf("\n");
		}
		#endif
		#if 0
		  if(pucBuffer[58]==1&&pucBuffer[59]==1&&pucBuffer[60]==0x50&&pucBuffer[61]==0x2d)
		  {
		  	u32 dataOffset=(pucBuffer[55]<<16)|(pucBuffer[56]<<8)|(pucBuffer[57]);
			if(dataOffset)
			{
				dataOffset/=1420;
				if(dataOffset<RTP_MAX_IDX)
				{
					rtpIdx[dataOffset]=1;
					if(maxDataOffset<dataOffset)
						maxDataOffset=dataOffset;
				}
				else
					printf("+++rtp offset err:%d\n\n", dataOffset);
			}
			else
			{
				idxErr=0;
				if(maxDataOffset)
				{
					for(i=0;i<=maxDataOffset;i++)
					{
						if(rtpIdx[i]==0)
						{
							printf("%d ", i);
							idxErr=1;
						}
					}
					if(idxErr)
					{
						printf("+++jpg data err:%d!!!\n\n",maxDataOffset);
					}
					if(os_time_get()-lastTime>30000)
					{
						lastTime=os_time_get();
						printf("+++jpg max offset:%d\n",maxDataOffset);
					}
				}
				maxDataOffset=0;
				memset(rtpIdx,0,RTP_MAX_IDX);
				rtpIdx[0]=1;
			}
		  }
		  #endif
        if( p->len == p->tot_len ) 
        {
          /* No pbuf chain, don't have to copy -> faster. */
          pucBuffer = &( ( unsigned char * ) p->payload )[ ETH_PAD_SIZE ];
          OS_MemCPY(ucBuffer, pucBuffer, p->tot_len*sizeof(unsigned char ));
        } 
        else 
        {
          pucChar = ucBuffer;
          for( q = p; q != NULL; q = q->next ) 
          {
              /* Send the data from the pbuf to the interface, one pbuf at a
                          time. The size of the data in each pbuf is kept in the ->len
                          variable. */
              /* send data from(q->payload, q->len); */
              LWIP_DEBUGF( NETIF_DEBUG, ("NETIF: send pucChar %p q->payload %p q->len %i q->next %p\r\n", pucChar, q->payload, ( int ) q->len, ( void* ) q->next ) );
              if( q == p ) 
              {
                  OS_MemCPY( pucChar, &( ( char * ) q->payload )[ ETH_PAD_SIZE ], q->len - ETH_PAD_SIZE );
                  pucChar += q->len - ETH_PAD_SIZE;                   
              } 
              else 
              {                   
                  OS_MemCPY( pucChar, q->payload, q->len );
                  pucChar += q->len;      
              }               
          }       

        }
    }
#endif
    return (ssv6xxx_wifi_send_ethernet(vif_idx, frame, OS_FRAME_GET_DATA_LEN(frame), 0) == SSV6XXX_SUCCESS) ? ERR_OK : ERR_BUF;
    //ssv6xxx_wifi_send_ethernet(vif_idx, frame, OS_FRAME_GET_DATA_LEN(frame), 0);
	//return ERR_OK;
}

/* Init netstack
 * [in] config: pointer of config
 * Init netstack with specific config
 * You may 
 * 1)init netstack
 * 2)add default net interface
 * 3)connect io of netstack and 
 */
int netstack_init(void *config)
{
    lwip_sys_init();
    tcpip_init(NULL, NULL);    
    return NS_OK; 
}

int netstack_deinit(void *config)
{
    return NS_OK; 
}

int netdev_deinit(struct netdev *pdev)
{
    struct netif *pwlan = NULL;
    //int ret = ERR_OK;

    /* net if init */
    pwlan = netif_find(pdev->name);
    if (pwlan != NULL)
    {
        netifapi_netif_remove(pwlan);
        OS_MemFree((void *)pwlan);
        pwlan = NULL;
    }
    return NS_OK;
}

/*Init device with setting
 * [in] pdev: pointer of config
 * Init netstack with specific config
 * You may 
 * init netdev
 */

int netdev_init(struct netdev *pdev, ssv_type_bool dft_dev, ssv_type_bool init_up)
{
    struct ip_addr ipaddr, netmask, gw;
    struct netif *pwlan = NULL;
    int ret = ERR_OK;

    /* net if init */
    pwlan = netif_find(pdev->name);
    if (pwlan != NULL)
    {
        netifapi_netif_remove(pwlan);
        OS_MemSET((void *)pwlan, 0, sizeof(struct netif));
    }
    else
        pwlan = OS_MemAlloc(sizeof(struct netif));
    
    
    OS_MemCPY((void *)(pwlan->hwaddr), pdev->hwmac, 6);
    OS_MemCPY((void *)(pwlan->name),pdev->name, 6);

    ipaddr.addr = pdev->ipaddr;
    netmask.addr = pdev->netmask;
    gw.addr =  pdev->gw;

    ret = netifapi_netif_add(pwlan, &ipaddr, &netmask, &gw, NULL, ethernetif_init, tcpip_input);
    if (ret != ERR_OK)
    {
        LOG_PRINTF("netifapi_netif_add err = %d\r\n", ret);
    }

    if(dft_dev == TRUE)
        ret = netifapi_netif_set_default(pwlan);
    if (ret != ERR_OK)
    {
        LOG_PRINTF("netifapi_netif_set_default err = %d\r\n", ret);
    }

    netifapi_netif_set_link_down(pwlan);

    /* if ap mode and dhcpd enable, set ip is default ip and set netif up */
    if (init_up == true)
    {
        netifapi_netif_set_link_up(pwlan);
        netifapi_netif_set_up(pwlan);        
    }

    /* Register link change callback function */
    netif_set_status_callback(pwlan, netdev_status_change_cb);

    LOG_PRINTF("Add netif MAC[%02x:%02x:%02x:%02x:%02x:%02x]\r\n",
        pwlan->hwaddr[0], pwlan->hwaddr[1], pwlan->hwaddr[2],
        pwlan->hwaddr[3], pwlan->hwaddr[4], pwlan->hwaddr[5]);
    
    return NS_OK;
}

//get hw mac
int netdev_set_default(const char *ifname)
{
    int ret = ERR_OK;
    struct netif * pwlan = NULL;

    pwlan = netif_find((char *)ifname);
    if(pwlan){
        ret = netifapi_netif_set_default(pwlan);
        LOG_PRINTF("netdev_set_default to %s,ret=%d\r\n",ifname,ret);
    }
    return ret;
}
//get hw mac
int netdev_getmacaddr(const char *ifname, ssv_type_u8 *macaddr)
{
    struct netif * pwlan = NULL;
    pwlan = netif_find((char *)ifname);
    if (pwlan)
        OS_MemCPY((void *)macaddr,(void *)(pwlan->hwaddr), 6);
    else
        return NS_ERR_ARG;
    return NS_OK;
}

//get hw mac
int netdev_getmacaddr_vif(int vif_index, ssv_type_u8 *macaddr)
{
	ssv6xxx_wifi_get_mac(macaddr, vif_index);
    return NS_OK;
}
void * netdev_get_netif(const char *ifname)
{
    struct netif * pwlan = NULL;
    pwlan = netif_find((char *)ifname);
    if(!pwlan){
      return pwlan;
    }
    LOG_PRINTF("udhcpd add netif MAC[%02x:%02x:%02x:%02x:%02x:%02x]\r\n",
        pwlan->hwaddr[0], pwlan->hwaddr[1], pwlan->hwaddr[2],
        pwlan->hwaddr[3], pwlan->hwaddr[4], pwlan->hwaddr[5]);
    return pwlan;

}

//get ipinfo
int netdev_getipv4info(const char *ifname, ssv_type_u32 *ip, ssv_type_u32 *gw, ssv_type_u32 *netmask)
{
    struct netif * pwlan = NULL;
    pwlan = netif_find((char *)ifname);
    if (NULL == pwlan)        
        return NS_ERR_ARG;
    
    if (NULL != ip)
        *ip = pwlan->ip_addr.addr;
    if (NULL != gw)
        *gw = pwlan->gw.addr;
    if (NULL != netmask)
        *netmask = pwlan->netmask.addr;
    
    return NS_OK;
}

//set ipinfo
int netdev_setipv4info(const char *ifname, ssv_type_u32 ip, ssv_type_u32 gw, ssv_type_u32 netmask)
{
    struct netif * pwlan = NULL;
    struct ip_addr ipaddr, maskaddr, gwaddr;
    int err = ERR_OK;
    pwlan = netif_find((char *)ifname);
    if (NULL == pwlan)        
        return NS_ERR_ARG;
    LOG_PRINTF("setipv4info if=%s\r\n",pwlan->name);
    ipaddr.addr = ip;
    maskaddr.addr = netmask;
    gwaddr.addr = gw;
    err = netifapi_netif_set_addr(pwlan, &ipaddr, &maskaddr, &gwaddr);
        
    if(err != ERR_OK)
    {
        LOG_PRINTF("netifapi_netif_set_addr err=%d\r\n", err);
        return NS_ERR_ARG;
    }
    return NS_OK;
}

//get dns server
//int netdev_get_ipv4dnsaddr(const char *ifname, u32 *dnsserver);
//set dns server
//int netdev_set_ipv4dnsaddr(const char *ifname, const u32 *dnsserver);

//get interface status
int netdev_check_ifup(const char *ifname)
{
    struct netif * pwlan = NULL;
    pwlan = netif_find((char *)ifname);
    if (NULL == pwlan)        
        return NS_ERR_ARG;
    
    if (pwlan->flags & NETDEV_IF_UP)
    {
        return NS_OK;
    }
    else
    {
        return NS_NG;
    }
}

//set interface up
int netdev_l3_if_up(const char *ifname)
{
    struct netif *pwlan = NULL;
    pwlan = netif_find((char *)ifname);
    if (NULL == pwlan)        
        return NS_ERR_ARG;
    
    netifapi_netif_set_up(pwlan);
    return NS_OK;
}

//set interface down
int netdev_l3_if_down(const char *ifname)
{
    struct netif *pwlan = NULL;
    pwlan = netif_find((char *)ifname);
    if (NULL == pwlan)        
        return NS_ERR_ARG;
    
    if (pwlan->flags & NETIF_FLAG_UP) {
        netifapi_netif_set_down(pwlan);
    }
    return NS_OK;
}

//interface link up cb
void netdev_link_up_cb(void *ifname)
{   
    struct netif *pwlan = NULL;
    pwlan = netif_find((char *)ifname);
    if (NULL == pwlan)        
        return ;
    
    netifapi_netif_set_link_up(pwlan);
    //return NS_OK;
}

//interface link down cb
void netdev_link_down_cb(void *ifname)
{   
    struct netif *pwlan = NULL;
    pwlan = netif_find((char *)ifname);
    if (NULL == pwlan)        
        return ;
    
    if (pwlan->flags & NETIF_FLAG_LINK_UP) {
        netifapi_netif_set_link_down(pwlan);
    }
    //return NS_OK;
}

//set dhcp client on dev
int dhcpc_wrapper_set(const char *ifname, const ssv_type_bool enable)
{
    struct netif *pwlan = NULL;
    pwlan = netif_find((char *)ifname);
    if (NULL == pwlan)        
        return NS_ERR_ARG;
    //LOG_PRINTF("%s:%d: ifname=%s,enable = %d\r\n", __FUNCTION__, __LINE__, ifname,enable);
    if(TRUE == enable)
        netifapi_dhcp_start(pwlan);
    else
        netifapi_dhcp_stop(pwlan);
    
    return NS_OK;        
}

ssv_type_u32 netdev_getallnetdev(struct netdev *pdev, ssv_type_u32 num_of_pdev)
{
    extern struct netif *netif_list;
    struct netif *netif;
    ssv_type_u32 num = 0;
    if ((pdev == NULL) ||(num_of_pdev == 0))
        return 0;
        
    for(netif=netif_list; ((num < num_of_pdev) && (netif!=NULL)); netif=netif->next)        
    {
        OS_MemCPY((void *)&pdev[num].name, (void *)&netif->name, sizeof(netif->name));
        OS_MemCPY((void *)&pdev[num].hwmac, (void *)&netif->hwaddr, sizeof(netif->hwaddr));
        pdev[num].flags = netif->flags;
        pdev[num].ipaddr = netif->ip_addr.addr;
        pdev[num].gw = netif->gw.addr;
        pdev[num].netmask = netif->netmask.addr;
        pdev[num].mtu = netif->mtu;
        num++;
    }
    return num;
}

ssv_type_s32 netdev_get_netdev(struct netdev *pdev, ssv_type_u8 if_idx)
{
    struct netif *pwlan = NULL;
    pwlan = netif_find((char *)if_name[if_idx]);
//    pwlan = netif_find(WLAN_IFNAME);

    if(pwlan)
    {
        OS_MemCPY((void *)&pdev[0].name, (void *)&pwlan->name, sizeof(pwlan->name));
        OS_MemCPY((void *)&pdev[0].hwmac, (void *)&pwlan->hwaddr, sizeof(pwlan->hwaddr));
        pdev[0].flags = pwlan->flags;
        pdev[0].ipaddr = pwlan->ip_addr.addr;
        pdev[0].gw = pwlan->gw.addr;
        pdev[0].netmask = pwlan->netmask.addr;
        pdev[0].mtu = pwlan->mtu;
        return 0;
    }
    return -1;
}

int netstack_udp_send(void* data, ssv_type_u32 len, ssv_type_u32 srcip, ssv_type_u16 srcport, ssv_type_u32 dstip, ssv_type_u16 dstport, ssv_type_s16 rptsndtimes)
{
    int err=0;
#if 0
    struct netconn *conn;
    struct netbuf *buf;
    char *netbuf_data;

    ip_addr_t srcaddr, dstaddr;
    
    
    conn = netconn_new(NETCONN_UDP);    
    if(conn==NULL) err++;
    srcaddr.addr = srcip;
    dstaddr.addr = dstip;

    if(err==0){
        if(ERR_OK!=netconn_bind(conn, &srcaddr, srcport)) err++;
    }

    if(err==0){
        if(ERR_OK!=netconn_connect(conn, &dstaddr, dstport)) err++;
    }
    if(err==0){
        do{
            buf=netbuf_new();
            if(buf==NULL) {
                netbuf_delete(buf);
                err++;
                break;
            }

            netbuf_data=netbuf_alloc(buf,len);
            if(netbuf_data==NULL) {
                err++;
                break;
            }
            ssv6xxx_memcpy(netbuf_data,(void *)data,len);
            if(ERR_OK!=netconn_send(conn,buf)){
                netbuf_delete(buf);
                err++;
                break;
            }
            netbuf_delete(buf);
        }while(rptsndtimes--);
    }

    if(conn!=NULL) netconn_delete(conn);

    if(err!=0){
        LOG_PRINTF("AirKiss fail\r\n");
    }else{
        LOG_PRINTF("Airkiss ok\r\n");
    }
#else
    int sockfd;
    ssv_type_u32 nbytes;
    struct sockaddr_in dst;
    socklen_t addrlen;

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        //LOG_PRINTF("client socket failure %d\n", errno);
        return -1;
    }

    dst.sin_family             = AF_INET;
    dst.sin_port               = htons(dstport);
    dst.sin_addr.s_addr        = htonl(dstip); //broad cast address
    addrlen                       = sizeof(struct sockaddr_in);
    
    do{
        nbytes = sendto(sockfd, data, len, 0,(struct sockaddr *)&dst, addrlen);
        if (nbytes == len)
        {
            break;
        }
        rptsndtimes--;
    }while(rptsndtimes > 0);

    close(sockfd);

    if (rptsndtimes > 0)
    {
        LOG_PRINTF("Airkiss ok\r\n");
    }
    else
    {
        LOG_PRINTF("AirKiss fail\r\n");
    }
#endif
    return err;
}

int netstack_tcp_send(void* data, ssv_type_u32 len, ssv_type_u32 srcip, ssv_type_u16 srcport, ssv_type_u32 dstip, ssv_type_u16 dstport)
{
    int sockfd, ret = 0;
    struct sockaddr_in src;
    struct sockaddr_in dst;
    socklen_t addrlen;    
    
    ssv6xxx_memset(&src, 0, sizeof(struct sockaddr_in));
    ssv6xxx_memset(&dst, 0, sizeof(struct sockaddr_in));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        //LOG_PRINTF("client socket failure %d\n", errno);
        return -1;
    }

    src.sin_family             = AF_INET;
    src.sin_port               = htons(srcport);
    src.sin_addr.s_addr        = srcip;
    dst.sin_family             = AF_INET;
    dst.sin_port               = htons(dstport);
    dst.sin_addr.s_addr        = dstip;
    addrlen                       = sizeof(struct sockaddr_in);

    LOG_DEBUGF(LOG_SNIFFER|LOG_LEVEL_ALL,("Bind ... \r\n"));
    if (bind(sockfd, (struct sockaddr *)&src, addrlen) != 0)
    {
        
        LOG_DEBUGF(LOG_SNIFFER|LOG_LEVEL_WARNING,("Bind to Port Number %d ,IP address %x:%x:%x:%x failed\n",
            srcport,
            ((const char *)(&src.sin_addr.s_addr))[0],
            ((const char *)(&src.sin_addr.s_addr))[1],
            ((const char *)(&src.sin_addr.s_addr))[2],
            ((const char *)(&src.sin_addr.s_addr))[3]));
        close(sockfd);
        return -1;
    }    

    LOG_DEBUGF(LOG_SNIFFER|LOG_LEVEL_ALL,("Connect ... \r\n"));
    if(connect(sockfd,(const struct sockaddr*)(&dst),(socklen_t)sizeof(struct sockaddr_in)) < 0)
    {
        LOG_DEBUGF(LOG_SNIFFER|LOG_LEVEL_WARNING,("Connect failed \n"));
        close(sockfd);
        return -1;
    }

    LOG_DEBUGF(LOG_SNIFFER|LOG_LEVEL_ALL,("Send ... \r\n"));
    if( send(sockfd, data, len, 0) < 0)
        ret = -1;            

    close(sockfd);
    return ret;
}


int netstack_dhcps_info_set(dhcps_info *if_dhcps, dhcps_info *des_if_dhcps,ssv_type_u8 vif_idx)
{
#if DHCPD_SUPPORT
    extern int dhcps_set_info_api(dhcps_info *if_dhcps,char *if_name);
    if (if_dhcps)
    {
        dhcps_info temp;

        LOG_DEBUGF(LOG_L4_DHCPD, ("netmgr_dhcps_info_set start_ip=%08x end_ip=%08x\r\n",if_dhcps->start_ip, if_dhcps->end_ip));
        OS_MemCPY((void*)&temp, (void*)if_dhcps, sizeof(dhcps_info));
        if (dhcps_set_info_api(&temp, if_name[vif_idx]) < 0)
        {
            return -1;
        }

        OS_MemCPY((void*)des_if_dhcps, (void*)if_dhcps, sizeof(dhcps_info));
    }

    return 0;
#else
    return -1;
#endif //#if DHCPD_SUPPORT    
}

int netstack_udhcpd_start(void)
{
#if DHCPD_SUPPORT
    extern int udhcpd_start(void);
    return udhcpd_start();
#else
    return 0;
#endif
}

int netstack_udhcpd_stop(void)
{
#if DHCPD_SUPPORT 
    extern int udhcpd_stop(void);
    return udhcpd_stop();
#else
    return 0;
#endif
}

int netstack_dhcp_ipmac_get(dhcpdipmac *ipmac, int *size_count)
{
#if DHCPD_SUPPORT
    extern int dhcpd_lease_get_api(struct dyn_lease *lease, int *count);
    int i = 0;
    struct dyn_lease *lease = NULL;
    int ret = 0;

    if (!ipmac || !size_count || (*size_count <= 0))
    {
        return -1;
    }

    lease = (struct dyn_lease *)OS_MemAlloc(sizeof(struct dyn_lease) * (*size_count));
    if (!lease)
    {
        return -1;
    }

    ret = dhcpd_lease_get_api(lease, size_count);

    if (ret == 0)
    {
        for (i = 0; i < *size_count; i++)
        {
           ((dhcpdipmac *) (ipmac + i))->ip = lease[i].lease_nip;
            OS_MemCPY((void*)(((dhcpdipmac *) (ipmac + i))->mac), (void*)(lease[i].lease_mac), 6);
        }
    }

    OS_MemFree((void*)lease);

    return 0;
#else
    return 0;
#endif //#if DHCPD_SUPPORT
}

int netstack_dhcp_ip_get_by_mac(ssv_type_u8* Mac, ssv_type_u32* ip)
{
#if DHCPD_SUPPORT
    extern int dhcpd_lease_get_api(struct dyn_lease *lease, int *count);
    int i = 0;
    struct dyn_lease *lease = NULL;
    int ret = 0;
    int size_count=10;

    //LOG_PRINTF("Input [%02x:%02x:%02x:%02x:%02x:%02x]\r\n", 
    //    Mac[0], Mac[1], Mac[2],Mac[3], Mac[4], Mac[5]);
    
    lease = (struct dyn_lease *)OS_MemAlloc(sizeof(struct dyn_lease) * 10);
    if (!lease)
    {
        return -1;
    }

    ret = dhcpd_lease_get_api(lease, &size_count);

    if (ret == 0)
    {
        for (i = 0; i < size_count; i++)
        {            
            if(!ssv6xxx_memcmp((void*)Mac, (void*)(lease[i].lease_mac), 6))
            {
                *ip = lease[i].lease_nip;
            }
                
        }
    }

    OS_MemFree((void*)lease);

    return 0;
#else
    return 0;
#endif //#if DHCPD_SUPPORT
}
extern s8_t etharp_find_addr_by_mac(struct netif *netif, struct eth_addr *ethaddr,
         struct eth_addr **eth_ret, ip_addr_t **ip_ret);

int netstack_find_ip_in_arp_table(ssv_type_u8 * mac,netstack_ip_addr_t *ipaddr)
{
    struct eth_addr ethaddr;
    struct eth_addr* ethaddr_ret;
    ip_addr_t* ipaddr_ret;
    struct netif *netif = NULL;

    if((mac==NULL) || (ipaddr==NULL))
    {
        return 0;
    }

    MEMCPY(ethaddr.addr, mac, 6);
    if (etharp_find_addr_by_mac(netif, &ethaddr, &ethaddr_ret, &ipaddr_ret) > -1)
    {
        ipaddr->addr = ipaddr_ret->addr;
        return 1;
    }
    return 0;
}

int netstack_etharp_unicast (ssv_type_u8 *dst_mac, netstack_ip_addr_t *ipaddr)
{
    ip_addr_t tmp_ipaddr;
    extern s8_t etharp_unicast (ssv_type_u8 *dst_mac, ip_addr_t *ipaddr);
    
    tmp_ipaddr.addr = ipaddr->addr;
    return etharp_unicast(dst_mac, &tmp_ipaddr);
}

ssv_type_u32 netstack_ipaddr_addr(const char *cp)
{
  netstack_ip_addr_t val;

  if (ipaddr_aton(cp, (ip_addr_t *)&val)) {
    return ip4_addr_get_u32(&val);
  }
  return (IPADDR_NONE);
}

char *netstack_inet_ntoa(netstack_ip_addr_t addr)
{
    return ipaddr_ntoa((ip_addr_t*)&(addr));

}

ssv_type_u16 netstack_ip4_addr1_16(ssv_type_u32* ipaddr)
{
    return ip4_addr1_16(ipaddr);
}
ssv_type_u16 netstack_ip4_addr2_16(ssv_type_u32* ipaddr)
{
    return ip4_addr2_16(ipaddr);
}
ssv_type_u16 netstack_ip4_addr3_16(ssv_type_u32* ipaddr)
{
    return ip4_addr3_16(ipaddr);
}
ssv_type_u16 netstack_ip4_addr4_16(ssv_type_u32* ipaddr)
{
    return ip4_addr4_16(ipaddr);
}

ssv_type_u32 netdev_wifi_get_ipaddr(int index)
{
    struct netif *pwlan = NULL;
    if(index>=MAX_VIF_NUM)
    {
        LOG_PRINTF("netstack_input vif invalid =%d\r\n",index);
        SSV_ASSERT(0);
    }
    
    pwlan = netif_find((char *)if_name[index]);
    if (pwlan)
    {
        return  pwlan->ip_addr.addr;
    }
    
    return 0;
}

