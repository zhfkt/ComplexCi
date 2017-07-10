package org.dc;

import org.dc.result.Result;

/**
 * Created by 赵仁杰 on 2015-06-24.
 */
public interface Judge {

    Result judge(String produceFile, String judgeFile);
}
