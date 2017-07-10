package org.dc.result;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by jay on 2016/5/11.
 */
public class Result {
    Boolean status = false;//状态500错误，200正确
    String msg = "";//消息说明
    Map<Integer,Double> data = new HashMap<Integer, Double>();//得分

    public Boolean getStatus() {
        return status;
    }

    public void setStatus(Boolean status) {
        this.status = status;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    public Map<Integer, Double> getData() {
        return data;
    }

    public void setData(Map<Integer, Double> data) {
        this.data = data;
    }

    public void put(Integer rank,Double result){
        data.put(rank,result);
    }

    @Override
    public String toString() {
        return "Result{" +
                "code='" + status + '\'' +
                ", msg='" + msg + '\'' +
                ", data=" + data.toString() +
                '}';
    }
}
