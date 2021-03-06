/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "t_tools.h"
#include "t_log.h"
#include "t_db.h"
#include "t_cjson.h"
#include "settle_date.h"
#include "t_macro.h"
#include "status.h"

/* 获取二维码结算日 */
int GetQrSettleDate(cJSON *pstJson, int *piFlag) {
    char sDate[8 + 1] = {0}, sTransCode[6 + 1] = {0};
    char sCurTime[6+1] = {0},sTmpTime[13+1] = {0};  //add by Gjq at 20180404
    char sBeginTime[6+1] = {0},sEndTime[6+1] = {0}; //add by Gjq at 20180404
    cJSON * pstTransJson = NULL;
    
    pstTransJson = GET_JSON_KEY(pstJson, "data");
    
    /*BEGIN 为防止日切前交易入账成功率低，二维码交易暂停十分钟 add by Gjq at 20180403*/
    //获取日切前暂停交易的起止时间点(235000-000001)
    if( FindValueByKey(sTmpTime,"before.dayend.time") < 0){
        tLog(ERROR, "查找key[before.dayend.time] 失败.");
        return ( -1 );
    }
    tGetTime(sCurTime, "", -1);
    memcpy(sBeginTime,sTmpTime,6);
    memcpy(sEndTime,sTmpTime+7,6); 
    tLog(DEBUG,"当前时间为[%s],日切前暂停交易的起止时间[%s],sBeginTime[%s]-sEndTime[%s]",
            sCurTime,sTmpTime,sBeginTime,sEndTime);
    if ( memcmp(sCurTime,sBeginTime,6) > 0 || memcmp(sCurTime,sEndTime,6) <= 0 ) {
        //tLog(ERROR,"当前时间[%s]为日切时间段，不允许交易",sCurTime);
        ErrHanding(pstTransJson, "90", "当前时间点[%s]不允许交易",sCurTime);
        return ( -1 );
    }
    /*END 为防止日切前交易入账成功率低，二维码交易暂停十分钟 add by Gjq at 20180403*/
    
#if 0
    GET_STR_KEY(pstTransJson, "resp_code", sRespCode);
    if (memcmp(sRespCode, "00", 2)) {
        tLog(ERROR, "交易失败[%s],不计算结算日.", sRespCode);
        return 0;
    }
#endif
    
    GET_STR_KEY(pstTransJson, "trans_code", sTransCode);
    if(sTransCode[2] == 'Y') {
        /*获取银联二维码交易结算日期*/
        tLog(DEBUG,"获取银联二维码交易[%s]的结算日期。",sTransCode);
        FindCardSettleDate(sDate);
    }
    else if( sTransCode[2] == 'W' || sTransCode[2] == 'B' ){
        /*获取 微信、支付宝 二维码交易结算日期*/
        tLog(DEBUG,"获取%s二维码交易[%s]的结算日期。",sTransCode[2] == 'W'?"微信":"支付宝",sTransCode);
        FindQrSettleDate(sDate);
    } 
    else {
        ErrHanding(pstTransJson, "96", "非法的的交易码[%s]",sTransCode);
        return ( -1 );
    }
    SET_STR_KEY(pstTransJson, "settle_date", sDate);
    tLog(ERROR, "获取结算日成功,使用T+1日[%s]结算.", sDate);
    return 0;
}