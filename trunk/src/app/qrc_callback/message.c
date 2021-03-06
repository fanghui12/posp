/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include "t_redis.h"
#include "param.h"
#include "t_signal.h"
#include "t_tools.h"
#include "t_config.h"
#include "t_log.h"
#include "t_cjson.h"
#include "t_db.h"




int RequestMsg();
int ResponseMsg(char *pcRespMsg);

int SendMsg(char *pcReqSvrId, char *pcKey, cJSON *pstDataJson) {
    cJSON *pstJson = NULL, *pcstSubJson = NULL;
    char sRepSvrId[64] = {0};
    char *pcMsg = NULL;

    /* 判断接收的svrid和应答的svrid是否为空 */
    if (NULL == pcReqSvrId || pcReqSvrId[0] == '\0') {
        tLog(ERROR, "请求队列[%s]有空值,无法发送.", pcReqSvrId);
        return -1;
    }
    /* 发送的交易对象 */
    pstJson = cJSON_CreateObject();
    if (NULL == pstJson) {
        tLog(ERROR, "创建发送Json失败.");
        return -1;
    }
    snprintf(sRepSvrId, sizeof (sRepSvrId), "%s_P", GetSvrId());
    SET_STR_KEY(pstJson, "svrid", sRepSvrId);
    SET_STR_KEY(pstJson, "key", pcKey);

    pcstSubJson = cJSON_Duplicate(pstDataJson, 1);

    SET_JSON_KEY(pstJson, "data", pcstSubJson);


    pcMsg = cJSON_PrintUnformatted(pstJson);
    if (tSendMsg(pcReqSvrId, pcMsg, strlen(pcMsg)) < 0) {
        tLog(ERROR, "发送消息到SvrId[%s]失败,data[%s].", pcReqSvrId, pcMsg);
    }
    cJSON_Delete(pstJson);
    return 0;
}

/* 请求处理 */
int RequestProc() {
    int iRet = -1, iLen = 0, iCnt = 0;
    char sMsg[MSG_MAX_LEN] = {0};
    char sSvrName[64] = {0};
    time_t tmStart;
    struct timeval tvS, tvS1;

    snprintf(sSvrName, sizeof (sSvrName), "%s_Q", GetSvrId());
    tLog(DEBUG, "[%s]进程,启动成功.", sSvrName);
    //开始时间用于5秒打印一次未查到结果的日志
    time(&tmStart);
    gettimeofday(&tvS, NULL);
    tvS1 = tvS;
    while (g_iQuitLoop) {
        QrRequestMsg(&tvS);
        EposRequestMsg(&tvS1);
        //sleep(0.5);
        //休眠半秒钟
        usleep(500000);
    }
    tLog(INFO, "[%s]进程,退出成功.", sSvrName);
    return 0;
}

int QrRequestMsg(struct timeval *tvS) {
    int iRet = 0;
    cJSON *pstJson = NULL;
    char *pcReqSvrId = NULL, *pcReqKey = NULL;
    char sReqSvrId[64] = {0}, sKey[64] = {0}, sRrn[16 + 1] = {0}, sTransCode[16 + 1] = {0};
    char sQueryCount[2] = {0}, sTransTime[16 + 1] = {0};
    int iSecs = 0;
    time_t tmEnd;
    struct timeval tvE;

    /* 扫表查找 */
    //结束时间
    time(&tmEnd);
    gettimeofday(&tvE, NULL);
    iSecs = tvE.tv_sec - (*tvS).tv_sec;
    //5秒打印一次日志防止无用日志打太多
    if (iSecs > 3) {
        *tvS = tvE;
        iRet = ScanQrTrans(tvS);
        if (iRet < 0) {
            /* -2代表未查找到二维码流水 */
            if (iRet != -2)
                tLog(ERROR, "查找待回调的二维码流水出错！");
            return -1;
        }
        return 0;
    }
    return 0;
}

int EposRequestMsg(struct timeval * tvS) {
    int iRet = 0;
    cJSON *pstJson = NULL;
    char *pcReqSvrId = NULL, *pcReqKey = NULL;
    char sReqSvrId[64] = {0}, sKey[64] = {0}, sRrn[16 + 1] = {0}, sTransCode[16 + 1] = {0};
    char sQueryCount[2] = {0}, sTransTime[16 + 1] = {0};

    pstJson = cJSON_CreateObject();

    /* 扫表查找 */
    iRet = ScanEposTrans(tvS);
    if (iRet < 0) {
        /* -2代表未查找到Epos流水 */
        if (iRet != -2)
            tLog(ERROR, "查找待回调的Epos流水出错！");
        return -1;
    }
    cJSON_Delete(pstJson);
    return 0;
}

/* 应答处理 */
int ResponseProc() {
    int iRet = -1, iLen = 0;
    char sMsg[MSG_MAX_LEN] = {0};
    char sSvrName[64] = {0};

    snprintf(sSvrName, sizeof (sSvrName), "%s_P", GetSvrId());
    tLog(DEBUG, "[%s]进程,启动成功.", sSvrName);
    while (g_iQuitLoop) {
        memset(sMsg, 0, sizeof (sMsg));
        iRet = tRecvMsg(sSvrName, sMsg, &iLen, TIMEOUT);
        switch (iRet) {
            case MSG_ERR:
                tLog(ERROR, "获取消息失败.");
                break;
            case MSG_TIMEOUT:
                tLog(DEBUG, "[%s]等待消息超时[%d].", sSvrName, TIMEOUT);
                break;
            case MSG_SUCC:
                //tLog(DEBUG, "response msg[%s]", sMsg);
                if (ResponseMsg(sMsg) < 0) {
                    tLog(ERROR, "应答处理失败,放弃处理.");
                }
                break;
        }
    }
    tLog(INFO, "[%s]进程,退出成功.", sSvrName);
    return 0;
}

int ResponseMsg(char *pcRespMsg) {
    cJSON *pstJson = NULL, *pstDataJson = NULL;
    char sMerchOrderNo[64] = {0};
    char sRespCode[2 + 1] = {0};

    pstJson = cJSON_Parse(pcRespMsg);

    /* 处理业务 */
    GET_STR_KEY(pstJson, "respCode ", sRespCode);
    GET_STR_KEY(pstJson, "orderNo", sMerchOrderNo);

    if (!memcmp(sRespCode, "00", 2)) {
        //更新valid_flag为0
        if (UpdQRNoticeFlag(pstJson) < 0) {
            tLog(ERROR, "NOTICE服务器更新Notice成功标志失败.");
        }
        tLog(ERROR, "二维码交易回调成功.");
    }

    cJSON_Delete(pstJson);
    return 0;
}