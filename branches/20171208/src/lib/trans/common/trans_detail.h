/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   saf_trans_detail.h
 * Author: feng.gaoo
 *
 * Created on 2017��3��7��, ����1:54
 */

#ifndef SAF_TRANS_DETAIL_H
#define SAF_TRANS_DETAIL_H
#ifdef __cplusplus
extern "C" {
#endif
#include "t_cjson.h"

    typedef struct {
        char sId[32 + 1];
        char sTransCode[6 + 1]; /* ���״���             */
        char sCardNo[19 + 1]; /* ����1                */
        char sCardId[4 + 1]; /* ���ſ���            */
        char cCardType; /* ����1����,���/����  */
        char sCardExpDate[4 + 1]; /* ����1��Ч��          */
        double dAmount; /* ���׷�����           */
        char sTransmitTime[10 + 1]; /* ���𷽽���ʱ��       */
        char sTransDate[8 + 1]; /* ��������             */
        char sTransTime[6 + 1]; /* ����ʱ��             */
        char sSysTrace[6 + 1]; /* �յ�ϵͳ��ˮ��       */
        char sLogicDate[8 + 1]; /* �յ�ϵͳ�߼�����     */
        char sSettleDate[8 + 1]; /* ��������             */
        char sMerchId[15 + 1]; /* �̻����             */
        char sTermId[8 + 1]; /* �ն˺�               */
        char sTermSn[50]; /* �ն����к� */
        char sTraceNo[6 + 1]; /* �ն���ˮ��           */
        char sRrn[12 + 1]; /* ϵͳ�ο���           */
        char sMcc[4 + 1]; /* ҵ̬����             */
        char sInputMode[3 + 1]; /* ��������뷽ʽ��     */
        char sCondCode[2 + 1]; /* �����������         */
        char sPinCode[2 + 1]; /* �����pin��ȡ��      */
        char sAcqId[8 + 1]; /* �յ�������ʶ         */
        char sIssId[8 + 1]; /* ����������ʶ         */
        char sAgentId[8 + 1]; /* ����������ʶ         */
        char sChannelId[8 + 1]; /* ������ʶ             */
        char sChannelMerchId[15 + 1]; /* �����̻����         */
        char sChannelTermId[8 + 1]; /* �����ն˺�           */
        char sChannelTermSn[50 + 1]; /* �����ն˺�           */
        char sChannelMcc[4 + 1]; /* ����ҵ̬����         */
        char sTransType[1 + 1]; /* ��������             */
        char sValidFlag[1 + 1]; /* ��Ч��־             */
        char sCheckFlag[1 + 1]; /* ���ʱ�־             */
        char sSettleFlag[1 + 1]; /* �����־             */
        char sAuthoFlag[1 + 1]; /* Ԥ��Ȩ��ɱ�־       */
        char sBatchNo[6 + 1]; /* ���κ�               */
        char sAuthCode[6 + 1]; /* ��Ȩ��               */
        double dRefundAmt; /* ���˻����           */
        double dFee; /* ����������           */
        char sFeeFlag[1 + 1]; /* �����Ѽ����־             */
        char sFeeDesc[50]; /*�̻�����������*/
        char sRespCode[3 + 1]; /* Ӧ����               */
        char sIstRespCode[3 + 1]; /* ����Ӧ����               */
        char sRespDesc[200]; /*Ӧ������*/
        char sRespId[8 + 1]; /* Ӧ�����             */
        char sSignFlag[1 + 1]; /*����ǩ����ʶ*/
        char sMerchOrderNo[100]; /*������*/
        char sORrn[12 + 1]; /* ԭϵͳ�ο���         */
        char sOTransDate[8 + 1]; /*ԭ��������*/
        char sSettleOrderId[20]; /*���㵥��*/
    } PosTransDetail;


    int AddPosTransLs(cJSON *pstJson);
    int UpdPosTransLs(cJSON *pstJson);
    int AddInlineTransLs(cJSON *pstJson);
    int UpdInlineTransLs(cJSON *pstJson);

#ifdef __cplusplus
}
#endif

#endif /* SAF_TRANS_DETAIL_H */
