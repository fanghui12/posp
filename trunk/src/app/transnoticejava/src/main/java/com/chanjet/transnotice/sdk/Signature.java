package com.chanjet.transnotice.sdk;

import com.chanjet.transnotice.entity.RecvMsg;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.security.NoSuchAlgorithmException;

public class Signature {
    private static final Logger logger = LoggerFactory.getLogger(Signature.class);
    public static String getMD5(String sKey, String sMsg) {
        String src = sKey + sMsg;
        byte[] source = src.getBytes();
        String s = null;
        char hexDigits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                'a', 'b', 'c', 'd', 'e', 'f' };// 用来将字节转换成16进制表示的字符
        java.security.MessageDigest md = null;
        try {
            md = java.security.MessageDigest.getInstance("MD5");
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        md.update(source);
        byte tmp[] = md.digest();// MD5 的计算结果是一个 128 位的长整数，
        // 用字节表示就是 16 个字节
        char str[] = new char[16 * 2];// 每个字节用 16 进制表示的话，使用两个字符， 所以表示成 16
        // 进制需要 32 个字符
        int k = 0;// 表示转换结果中对应的字符位置
        for (int i = 0; i < 16; i++) {// 从第一个字节开始，对 MD5 的每一个字节// 转换成 16
            // 进制字符的转换
            byte byte0 = tmp[i];// 取第 i 个字节
            str[k++] = hexDigits[byte0 >>> 4 & 0xf];// 取字节中高 4 位的数字转换,// >>>
            // 为逻辑右移，将符号位一起右移
            str[k++] = hexDigits[byte0 & 0xf];// 取字节中低 4 位的数字转换

        }
        s = new String(str);// 换后的结果转换为字符串
        return s;
    }

    /*验证签名*/
    public static boolean verificationSign(String key, RecvMsg recvMsg) throws Exception {
        String localSign = getMD5(key,Sort.sort(recvMsg));
        String oriSign = recvMsg.getSign();
        logger.info("原签名数据: ["+ oriSign +"],本地签名数据: ["+ localSign +"]");
        return ignoreCaseEquals(localSign,oriSign);
    }

    /*不区英文字母大小写比较*/
    public static boolean ignoreCaseEquals(String str1,String str2){
        return str1 == null ? str2 == null :str1.equalsIgnoreCase(str2);
    }
}